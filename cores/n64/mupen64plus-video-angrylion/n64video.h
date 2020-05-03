#pragma once

#include <stdint.h>
#include <stdbool.h>

#define RDRAM_MAX_SIZE 0x800000

// register enums
enum dp_register
{
    DP_START,
    DP_END,
    DP_CURRENT,
    DP_STATUS,
    DP_CLOCK,
    DP_BUFBUSY,
    DP_PIPEBUSY,
    DP_TMEM,
    DP_NUM_REG
};

enum vi_register
{
    VI_STATUS,  // aka VI_CONTROL
    VI_ORIGIN,  // aka VI_DRAM_ADDR
    VI_WIDTH,
    VI_INTR,
    VI_V_CURRENT_LINE,
    VI_TIMING,
    VI_V_SYNC,
    VI_H_SYNC,
    VI_LEAP,    // aka VI_H_SYNC_LEAP
    VI_H_START, // aka VI_H_VIDEO
    VI_V_START, // aka VI_V_VIDEO
    VI_V_BURST,
    VI_X_SCALE,
    VI_Y_SCALE,
    VI_NUM_REG
};

// config enums
enum vi_mode
{
    VI_MODE_NORMAL,     // color buffer with VI filter
    VI_MODE_COLOR,      // direct color buffer, unfiltered
    VI_MODE_DEPTH,      // depth buffer as grayscale
    VI_MODE_COVERAGE,   // coverage as grayscale
    VI_MODE_NUM
};

enum vi_interp
{
    VI_INTERP_NEAREST,
    VI_INTERP_LINEAR,
    VI_INTERP_NUM
};

enum dp_compat_profile
{
    DP_COMPAT_LOW,
    DP_COMPAT_MEDIUM,
    DP_COMPAT_HIGH,
    DP_COMPAT_NUM
};

struct n64video_config
{
    struct {
        uint8_t* rdram;             // RDRAM pointer
        uint32_t rdram_size;        // size of RDRAM, typically 4 or 8 MiB
        uint8_t* dmem;              // RSP data memory pointer
        uint32_t** vi_reg;          // video interface registers
        uint32_t** dp_reg;          // display processor registers
        uint32_t* mi_intr_reg;      // MIPS interface interrupt register
        void (*mi_intr_cb)(void);   // interrupt callback function
    } gfx;
    struct {
        enum vi_mode mode;          // output mode
        enum vi_interp interp;      // output interpolation method
        bool widescreen;            // force 16:9 aspect ratio if true
        bool hide_overscan;         // crop to visible area if true
        bool vsync;                 // enable vsync if true
        bool exclusive;             // run in exclusive mode when in fullscreen if true
        bool vi_dedither;           // enable dedithering if true
        bool vi_blur;               // enable bilateral blur if true
    } vi;
    struct {
        enum dp_compat_profile compat;  // multithreading compatibility mode
    } dp;
    bool parallel;                  // use multithreaded renderer if true
    bool dithering;                 // enable dithering
    uint32_t num_workers;           // number of rendering workers
};

void n64video_config_init(struct n64video_config* config);
void n64video_init(struct n64video_config* config);
void n64video_update_screen(void);
void n64video_process_list(void);
void n64video_close(void);
