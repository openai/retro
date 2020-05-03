#include "n64video.h"
#include "common.h"
#include "msg.h"
#include "vdac.h"
#include "parallel_al.h"

#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void angrylion_set_threads(unsigned value);
void angrylion_set_synclevel(unsigned value);
void angrylion_set_vi(unsigned value);

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(x, lo, hi) (((x) > (hi)) ? (hi) : (((x) < (lo)) ? (lo) : (x)))

#define SIGN16(x)   ((int16_t)(x))
#define SIGN8(x)    ((int8_t)(x))

#define SIGN(x, numb)	(((x) & ((1 << (numb)) - 1)) | -((x) & (1 << ((numb) - 1))))
#define SIGNF(x, numb)	((x) | -((x) & (1 << ((numb) - 1))))

#define TRELATIVE(x, y)     ((x) - ((y) << 3))

#define PIXELS_TO_BYTES(pix, siz) (((pix) << (siz)) >> 1)

// RGBA5551 to RGBA8888 helper
#define RGBA16_R(x) (((x) >> 8) & 0xf8)
#define RGBA16_G(x) (((x) & 0x7c0) >> 3)
#define RGBA16_B(x) (((x) & 0x3e) << 2)

// RGBA8888 helper
#define RGBA32_R(x) (((x) >> 24) & 0xff)
#define RGBA32_G(x) (((x) >> 16) & 0xff)
#define RGBA32_B(x) (((x) >> 8) & 0xff)
#define RGBA32_A(x) ((x) & 0xff)

// maximum number of commands to buffer for parallel processing
#define CMD_BUFFER_SIZE 1024

// maximum data size of a single command in bytes
#define CMD_MAX_SIZE 176

// maximum data size of a single command in 32 bit integers
#define CMD_MAX_INTS (CMD_MAX_SIZE / sizeof(int32_t))

// extracts the command ID from a command buffer
#define CMD_ID(cmd) ((*(cmd) >> 24) & 0x3f)

// list of command IDs
#define CMD_ID_NO_OP                           0x00
#define CMD_ID_FILL_TRIANGLE                   0x08
#define CMD_ID_FILL_ZBUFFER_TRIANGLE           0x09
#define CMD_ID_TEXTURE_TRIANGLE                0x0a
#define CMD_ID_TEXTURE_ZBUFFER_TRIANGLE        0x0b
#define CMD_ID_SHADE_TRIANGLE                  0x0c
#define CMD_ID_SHADE_ZBUFFER_TRIANGLE          0x0d
#define CMD_ID_SHADE_TEXTURE_TRIANGLE          0x0e
#define CMD_ID_SHADE_TEXTURE_Z_BUFFER_TRIANGLE 0x0f
#define CMD_ID_TEXTURE_RECTANGLE               0x24
#define CMD_ID_TEXTURE_RECTANGLE_FLIP          0x25
#define CMD_ID_SYNC_LOAD                       0x26
#define CMD_ID_SYNC_PIPE                       0x27
#define CMD_ID_SYNC_TILE                       0x28
#define CMD_ID_SYNC_FULL                       0x29
#define CMD_ID_SET_KEY_GB                      0x2a
#define CMD_ID_SET_KEY_R                       0x2b
#define CMD_ID_SET_CONVERT                     0x2c
#define CMD_ID_SET_SCISSOR                     0x2d
#define CMD_ID_SET_PRIM_DEPTH                  0x2e
#define CMD_ID_SET_OTHER_MODES                 0x2f
#define CMD_ID_LOAD_TLUT                       0x30
#define CMD_ID_SET_TILE_SIZE                   0x32
#define CMD_ID_LOAD_BLOCK                      0x33
#define CMD_ID_LOAD_TILE                       0x34
#define CMD_ID_SET_TILE                        0x35
#define CMD_ID_FILL_RECTANGLE                  0x36
#define CMD_ID_SET_FILL_COLOR                  0x37
#define CMD_ID_SET_FOG_COLOR                   0x38
#define CMD_ID_SET_BLEND_COLOR                 0x39
#define CMD_ID_SET_PRIM_COLOR                  0x3a
#define CMD_ID_SET_ENV_COLOR                   0x3b
#define CMD_ID_SET_COMBINE                     0x3c
#define CMD_ID_SET_TEXTURE_IMAGE               0x3d
#define CMD_ID_SET_MASK_IMAGE                  0x3e
#define CMD_ID_SET_COLOR_IMAGE                 0x3f

#define config al_config
static struct n64video_config config;

static struct
{
    bool fillmbitcrashes, vbusclock, nolerp;
} onetimewarnings;

static int rdp_pipeline_crashed = 0;

static STRICTINLINE int32_t clamp(int32_t value, int32_t min, int32_t max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    return value;
}

static STRICTINLINE uint32_t irand(uint32_t* state)
{
    *state = *state * 0x343fd + 0x269ec3;
    return ((*state >> 16) & 0x7fff);
}

#include "n64video/rdp.c"
#include "n64video/vi.c"

static uint32_t rdp_cmd_buf[CMD_BUFFER_SIZE][CMD_MAX_INTS];
static uint32_t rdp_cmd_buf_pos;

static uint32_t rdp_cmd_pos;
static uint32_t rdp_cmd_id;
static uint32_t rdp_cmd_len;

// table of commands that require thread synchronization in
// multithreaded mode
static bool rdp_cmd_sync[64];

static void cmd_run_buffered(uint32_t worker_id)
{
    uint32_t pos;
    for (pos = 0; pos < rdp_cmd_buf_pos; pos++)
        rdp_cmd(worker_id, rdp_cmd_buf[pos]);
}

static void cmd_flush(void)
{
    // only run if there's something buffered
    if (rdp_cmd_buf_pos) {
        // let workers run all buffered commands in parallel
        parallel_run(cmd_run_buffered);
        // reset buffer by starting from the beginning
        rdp_cmd_buf_pos = 0;
    }
}

static void cmd_init(void)
{
    rdp_cmd_pos = 0;
    rdp_cmd_id = 0;
    rdp_cmd_len = CMD_MAX_INTS;
}

void n64video_config_init(struct n64video_config* config)
{
    memset(config, 0, sizeof(*config));

    // config defaults that aren't false or 0
    config->parallel = true;
    config->vi.vsync = true;
    config->dp.compat = DP_COMPAT_MEDIUM;
}

void rdp_init_worker(uint32_t worker_id)
{
    rdp_init(worker_id, parallel_num_workers());
}

void n64video_init(struct n64video_config* xconfig)
{
    if (xconfig)
        config = *xconfig;

    // initialize static lookup tables and RDP state, once is enough
    static bool static_init;
    if (!static_init)
    {
        blender_init_lut();
        coverage_init_lut();
        combiner_init_lut();
        tex_init_lut();
        z_init_lut();

        fb_init(0);
        combiner_init(0);
        tex_init(0);
        rasterizer_init(0);

        static_init = true;
    }

    // enable sync switches depending on compatibility mode
    memset(rdp_cmd_sync, 0, sizeof(rdp_cmd_sync));
    switch (config.dp.compat) {
        case DP_COMPAT_HIGH:
            rdp_cmd_sync[CMD_ID_SET_TEXTURE_IMAGE] = true;
        case DP_COMPAT_MEDIUM:
            rdp_cmd_sync[CMD_ID_SET_MASK_IMAGE] = true;
            rdp_cmd_sync[CMD_ID_SET_COLOR_IMAGE] = true;
        case DP_COMPAT_LOW:
            rdp_cmd_sync[CMD_ID_SYNC_FULL] = true;
    }

    // init internals
    rdram_init();
    vi_init();
    cmd_init();

    rdp_pipeline_crashed = 0;
    memset(&onetimewarnings, 0, sizeof(onetimewarnings));

    if (config.parallel)
    {
       uint32_t i;
       // init worker system
       parallel_alinit(config.num_workers);

       // sync states from main worker
       for (i = 1; i < parallel_num_workers(); i++)
          memcpy(&state[i], &state[0], sizeof(struct rdp_state));

       // init workers
       parallel_run(rdp_init_worker);
    }
    else
        rdp_init(0, 1);
}

void n64video_process_list(void)
{
    uint32_t** dp_reg = config.gfx.dp_reg;
    uint32_t dp_current_al = (*dp_reg[DP_CURRENT] & ~7) >> 2;
    uint32_t dp_end_al = (*dp_reg[DP_END] & ~7) >> 2;

    // don't do anything if the RDP has crashed or the registers are not set up correctly
    if (rdp_pipeline_crashed || dp_end_al <= dp_current_al) {
        return;
    }

    // while there's data in the command buffer...
    while (dp_end_al - dp_current_al > 0) {
        uint32_t i, toload;
        bool xbus_dma = (*dp_reg[DP_STATUS] & DP_STATUS_XBUS_DMA) != 0;
        uint32_t* dmem = (uint32_t*)config.gfx.dmem;
        uint32_t* cmd_buf = rdp_cmd_buf[rdp_cmd_buf_pos];

        // when reading the first int, extract the command ID and update the buffer length
        if (rdp_cmd_pos == 0) {
            if (xbus_dma) {
                cmd_buf[rdp_cmd_pos++] = dmem[dp_current_al++ & 0x3ff];
            } else {
                cmd_buf[rdp_cmd_pos++] = rdram_read_idx32(dp_current_al++);
            }

            rdp_cmd_id = CMD_ID(cmd_buf);
            rdp_cmd_len = rdp_commands[rdp_cmd_id].length >> 2;
        }

        // copy more data from the N64 to the local command buffer
        toload = MIN(dp_end_al - dp_current_al, rdp_cmd_len - 1);

        if (xbus_dma) {
            for (i = 0; i < toload; i++) {
                cmd_buf[rdp_cmd_pos++] = dmem[dp_current_al++ & 0x3ff];
            }
        } else {
            for (i = 0; i < toload; i++) {
                cmd_buf[rdp_cmd_pos++] = rdram_read_idx32(dp_current_al++);
            }
        }

        // if there's enough data for the current command...
        if (rdp_cmd_pos == rdp_cmd_len) {
            // check if parallel processing is enabled
            if (config.parallel) {
                // special case: sync_full always needs to be run in main thread
                if (rdp_cmd_id == CMD_ID_SYNC_FULL) {
                    // first, run all pending commands
                    cmd_flush();

                    // parameters are unused, so NULL is fine
                    rdp_sync_full(0, NULL);
                } else {
                    // increment buffer position
                    rdp_cmd_buf_pos++;

                    // flush buffer when it is full or when the current command requires a sync
                    if (rdp_cmd_buf_pos >= CMD_BUFFER_SIZE || rdp_cmd_sync[rdp_cmd_id]) {
                        cmd_flush();
                    }
                }
            } else {
                // run command directly
                rdp_cmd(0, cmd_buf);
            }

            // send Z-buffer address to VI for "depth" output mode
            if (rdp_cmd_id == CMD_ID_SET_MASK_IMAGE) {
                vi_set_zbuffer_address(cmd_buf[1] & 0x0ffffff);
            }

            // reset current command buffer to prepare for the next one
            cmd_init();
        }
    }

    // update DP registers to indicate that all bytes have been read
    *dp_reg[DP_START] = *dp_reg[DP_CURRENT] = *dp_reg[DP_END];
}

void n64video_close(void)
{
    vi_close();
    parallel_close();
}
