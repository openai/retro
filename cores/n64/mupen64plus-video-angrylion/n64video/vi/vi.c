// anamorphic NTSC resolution
#define H_RES_NTSC 640
#define V_RES_NTSC 480

// anamorphic PAL resolution
#define H_RES_PAL 768
#define V_RES_PAL 576

// typical VI_V_SYNC values for NTSC and PAL
#define V_SYNC_NTSC 525
#define V_SYNC_PAL 625

// maximum possible size of the prescale area
#define PRESCALE_WIDTH H_RES_NTSC
#define PRESCALE_HEIGHT V_SYNC_PAL

enum vi_type
{
    VI_TYPE_BLANK,      // no data, no sync
    VI_TYPE_RESERVED,   // unused, should never be set
    VI_TYPE_RGBA5551,   // 16 bit color (internally 18 bit RGBA5553)
    VI_TYPE_RGBA8888    // 32 bit color
};

enum vi_aa
{
    VI_AA_RESAMP_EXTRA_ALWAYS,  // resample and AA (always fetch extra lines)
    VI_AA_RESAMP_EXTRA,         // resample and AA (fetch extra lines if needed)
    VI_AA_RESAMP_ONLY,          // only resample (treat as all fully covered)
    VI_AA_REPLICATE             // replicate pixels, no interpolation
};

struct vi_reg_ctrl
{
    uint8_t type;
    bool gamma_dither_enable;
    bool gamma_enable;
    bool divot_enable;
    bool vbus_clock_enable;
    bool serrate;
    bool test_mode;
    uint8_t aa_mode;
    bool reserved;
    bool kill_we;
    uint8_t pixel_advance;
    bool dither_filter_enable;
};

typedef void(*vi_fetch_filter_func)(struct rgba*, uint32_t, uint32_t, struct vi_reg_ctrl, uint32_t, uint32_t);

#include "gamma.c"
#include "lerp.c"
#include "divot.c"
#include "video.c"
#include "restore.c"
#include "fetch.c"

// states
static uint32_t prevvicurrent;
static int32_t emucontrolsvicurrent;
static bool prevserrate;
static bool lowerfield;
static int32_t oldvstart;
static bool prevwasblank;
static int32_t vactivelines;
static bool ispal;
static int32_t minhpass;
static int32_t maxhpass;
static uint32_t x_add;
static uint32_t x_start;
static uint32_t y_add;
static uint32_t y_start;
static int32_t v_sync;
static int32_t vi_width_low;
static uint32_t frame_buffer;
static uint32_t tvfadeoutstate[PRESCALE_HEIGHT];
static uint32_t rseed;
static uint32_t zb_address;

// prescale buffer
static struct rgba prescale[PRESCALE_WIDTH * PRESCALE_HEIGHT];
static uint32_t prescale_ptr;
static int32_t linecount;

// parsed VI registers
static uint32_t** vi_reg_ptr;
static struct vi_reg_ctrl ctrl;
static int32_t hres, vres;
static int32_t hres_raw, vres_raw;
static int32_t v_start;
static int32_t h_start;
static int32_t v_current_line;

static void vi_init(void)
{
    vi_gamma_init();
    vi_restore_init();

    memset(prescale, 0, sizeof(prescale));

    prevvicurrent = 0;
    emucontrolsvicurrent = -1;
    prevserrate = false;
    oldvstart = 1337;
    prevwasblank = false;
    rseed = 3;
    zb_address = 0;
}

static void vi_process_full_parallel(uint32_t worker_id)
{
    int32_t y;
    struct rgba viaa_array[0xa10 << 1];
    struct rgba divot_array[0xa10 << 1];

    int32_t cache_marker = 0, cache_next_marker = 0, divot_cache_marker = 0, divot_cache_next_marker = 0;
    int32_t cache_marker_init = (x_start >> 10) - 1;

    struct rgba *viaa_cache = &viaa_array[0];
    struct rgba *viaa_cache_next = &viaa_array[0xa10];
    struct rgba *divot_cache = &divot_array[0];
    struct rgba *divot_cache_next = &divot_array[0xa10];

    struct rgba color, nextcolor, scancolor, scannextcolor;

    vi_fetch_filter_func vi_fetch_filter_ptr = ctrl.type & 1 ? vi_fetch_filter32 : vi_fetch_filter16;

    uint32_t pixels = 0, nextpixels = 0, fetchbugstate = 0;

    int32_t xfrac = 0, yfrac = 0;
    int32_t line_x = 0, next_line_x = 0, prev_line_x = 0, far_line_x = 0;
    int32_t prev_scan_x = 0, scan_x = 0, next_scan_x = 0, far_scan_x = 0;
    int32_t prev_x = 0, cur_x = 0, next_x = 0, far_x = 0;

    bool cache_init = false;

    pixels = 0;

    int32_t y_begin = 0;
    int32_t y_end = vres;
    int32_t y_inc = 1;

    if (config.parallel) {
        y_begin = worker_id;
        y_inc = parallel_num_workers();
    }

    for (y = y_begin; y < y_end; y += y_inc) {
        int32_t x;
        uint32_t x_offs = x_start;
        uint32_t curry = y_start + y * y_add;
        uint32_t nexty = y_start + (y + 1) * y_add;
        uint32_t prevy = curry >> 10;

        cache_marker = cache_next_marker = cache_marker_init;
        if (ctrl.divot_enable) {
            divot_cache_marker = divot_cache_next_marker = cache_marker_init;
        }

        struct rgba* pixel_row = &prescale[prescale_ptr + linecount * y];

        yfrac = (curry >> 5) & 0x1f;
        pixels = vi_width_low * prevy;
        nextpixels = vi_width_low + pixels;

        if (prevy == (nexty >> 10)) {
            fetchbugstate = 2;
        } else {
            fetchbugstate >>= 1;
        }

        for (x = 0; x < hres; x++, x_offs += x_add) {
            line_x = x_offs >> 10;
            prev_line_x = line_x - 1;
            next_line_x = line_x + 1;
            far_line_x = line_x + 2;

            cur_x = pixels + line_x;
            prev_x = pixels + prev_line_x;
            next_x = pixels + next_line_x;
            far_x = pixels + far_line_x;

            scan_x = nextpixels + line_x;
            prev_scan_x = nextpixels + prev_line_x;
            next_scan_x = nextpixels + next_line_x;
            far_scan_x = nextpixels + far_line_x;

            line_x++;
            prev_line_x++;
            next_line_x++;
            far_line_x++;

            xfrac = (x_offs >> 5) & 0x1f;

            if (prev_line_x > cache_marker) {
                vi_fetch_filter_ptr(&viaa_cache[prev_line_x], frame_buffer, prev_x, ctrl, vi_width_low, 0);
                vi_fetch_filter_ptr(&viaa_cache[line_x], frame_buffer, cur_x, ctrl, vi_width_low, 0);
                vi_fetch_filter_ptr(&viaa_cache[next_line_x], frame_buffer, next_x, ctrl, vi_width_low, 0);
                cache_marker = next_line_x;
            } else if (line_x > cache_marker) {
                vi_fetch_filter_ptr(&viaa_cache[line_x], frame_buffer, cur_x, ctrl, vi_width_low, 0);
                vi_fetch_filter_ptr(&viaa_cache[next_line_x], frame_buffer, next_x, ctrl, vi_width_low, 0);
                cache_marker = next_line_x;
            } else if (next_line_x > cache_marker) {
                vi_fetch_filter_ptr(&viaa_cache[next_line_x], frame_buffer, next_x, ctrl, vi_width_low, 0);
                cache_marker = next_line_x;
            }

            if (prev_line_x > cache_next_marker) {
                vi_fetch_filter_ptr(&viaa_cache_next[prev_line_x], frame_buffer, prev_scan_x, ctrl, vi_width_low, fetchbugstate);
                vi_fetch_filter_ptr(&viaa_cache_next[line_x], frame_buffer, scan_x, ctrl, vi_width_low, fetchbugstate);
                vi_fetch_filter_ptr(&viaa_cache_next[next_line_x], frame_buffer, next_scan_x, ctrl, vi_width_low, fetchbugstate);
                cache_next_marker = next_line_x;
            } else if (line_x > cache_next_marker) {
                vi_fetch_filter_ptr(&viaa_cache_next[line_x], frame_buffer, scan_x, ctrl, vi_width_low, fetchbugstate);
                vi_fetch_filter_ptr(&viaa_cache_next[next_line_x], frame_buffer, next_scan_x, ctrl, vi_width_low, fetchbugstate);
                cache_next_marker = next_line_x;
            } else if (next_line_x > cache_next_marker) {
                vi_fetch_filter_ptr(&viaa_cache_next[next_line_x], frame_buffer, next_scan_x, ctrl, vi_width_low, fetchbugstate);
                cache_next_marker = next_line_x;
            }

            if (ctrl.divot_enable) {
                if (far_line_x > cache_marker) {
                    vi_fetch_filter_ptr(&viaa_cache[far_line_x], frame_buffer, far_x, ctrl, vi_width_low, 0);
                    cache_marker = far_line_x;
                }

                if (far_line_x > cache_next_marker) {
                    vi_fetch_filter_ptr(&viaa_cache_next[far_line_x], frame_buffer, far_scan_x, ctrl, vi_width_low, fetchbugstate);
                    cache_next_marker = far_line_x;
                }

                if (line_x > divot_cache_marker) {
                    divot_filter(&divot_cache[line_x], viaa_cache[line_x], viaa_cache[prev_line_x], viaa_cache[next_line_x]);
                    divot_filter(&divot_cache[next_line_x], viaa_cache[next_line_x], viaa_cache[line_x], viaa_cache[far_line_x]);
                    divot_cache_marker = next_line_x;
                } else if (next_line_x > divot_cache_marker) {
                    divot_filter(&divot_cache[next_line_x], viaa_cache[next_line_x], viaa_cache[line_x], viaa_cache[far_line_x]);
                    divot_cache_marker = next_line_x;
                }

                if (line_x > divot_cache_next_marker) {
                    divot_filter(&divot_cache_next[line_x], viaa_cache_next[line_x], viaa_cache_next[prev_line_x], viaa_cache_next[next_line_x]);
                    divot_filter(&divot_cache_next[next_line_x], viaa_cache_next[next_line_x], viaa_cache_next[line_x], viaa_cache_next[far_line_x]);
                    divot_cache_next_marker = next_line_x;
                } else if (next_line_x > divot_cache_next_marker) {
                    divot_filter(&divot_cache_next[next_line_x], viaa_cache_next[next_line_x], viaa_cache_next[line_x], viaa_cache_next[far_line_x]);
                    divot_cache_next_marker = next_line_x;
                }

                color = divot_cache[line_x];
            } else {
                color = viaa_cache[line_x];
            }

            bool lerping = ctrl.aa_mode != VI_AA_REPLICATE && (xfrac || yfrac);

            if (lerping) {
                if (ctrl.divot_enable) {
                    nextcolor = divot_cache[next_line_x];
                    scancolor = divot_cache_next[line_x];
                    scannextcolor = divot_cache_next[next_line_x];
                } else {
                    nextcolor = viaa_cache[next_line_x];
                    scancolor = viaa_cache_next[line_x];
                    scannextcolor = viaa_cache_next[next_line_x];
                }

                vi_vl_lerp(&color, scancolor, yfrac);
                vi_vl_lerp(&nextcolor, scannextcolor, yfrac);
                vi_vl_lerp(&color, nextcolor, xfrac);
            }

            struct rgba* pixel = &pixel_row[x];

            if (x >= minhpass && x < maxhpass) {
                *pixel = color;
                gamma_filters(pixel, ctrl, &rseed);
            } else {
                pixel->r = pixel->g = pixel->b = 0;
            }
        }

        if (!cache_init && y_add == 0x400) {
            cache_marker = cache_next_marker;
            cache_next_marker = cache_marker_init;

            struct rgba* tempccvgptr = viaa_cache;
            viaa_cache = viaa_cache_next;
            viaa_cache_next = tempccvgptr;
            if (ctrl.divot_enable) {
                divot_cache_marker = divot_cache_next_marker;
                divot_cache_next_marker = cache_marker_init;
                tempccvgptr = divot_cache;
                divot_cache = divot_cache_next;
                divot_cache_next = tempccvgptr;
            }

            cache_init = true;
        }
    }
}

static bool vi_process_full(void)
{
    bool isblank = (ctrl.type & 2) == 0;
    bool validinterlace = !isblank && ctrl.serrate;

    if (validinterlace) {
        if (prevserrate && emucontrolsvicurrent < 0) {
            emucontrolsvicurrent = v_current_line != prevvicurrent;
        }

        if (emucontrolsvicurrent == 1) {
            lowerfield = v_current_line ^ 1;
        } else if (!emucontrolsvicurrent) {
            if (v_start == oldvstart) {
                lowerfield ^= true;
            } else {
                lowerfield = v_start < oldvstart;
            }
        }

        prevvicurrent = v_current_line;
        oldvstart = v_start;
    }

    prevserrate = validinterlace;

    bool validh = hres > 0 && h_start < PRESCALE_WIDTH;
    int32_t h_end = hres + h_start; // note: the result appears to be different to VI_H_END
    int32_t hrightblank = PRESCALE_WIDTH - h_end;

    if (isblank && prevwasblank) {
        return false;
    }

    prevwasblank = isblank;

    linecount = PRESCALE_WIDTH << ctrl.serrate;
    prescale_ptr = v_start * linecount + h_start + (lowerfield ? PRESCALE_WIDTH : 0);

    int32_t i;
    if (isblank) {
        // blank signal, clear entire screen buffer
        memset(tvfadeoutstate, 0, PRESCALE_HEIGHT * sizeof(uint32_t));
        memset(prescale, 0, sizeof(prescale));
    } else {
        // clear left border
        int32_t j;
        if (h_start > 0 && h_start < PRESCALE_WIDTH) {
            for (i = 0; i < vactivelines; i++) {
                memset(&prescale[i * PRESCALE_WIDTH], 0, h_start * sizeof(uint32_t));
            }
        }

        // clear right border
        if (h_end >= 0 && h_end < PRESCALE_WIDTH) {
            for (i = 0; i < vactivelines; i++) {
                memset(&prescale[i * PRESCALE_WIDTH + h_end], 0, hrightblank * sizeof(uint32_t));
            }
        }

        // clear top border
        for (i = 0; i < ((v_start << ctrl.serrate) + lowerfield); i++) {
            if (tvfadeoutstate[i]) {
                tvfadeoutstate[i]--;
                if (!tvfadeoutstate[i]) {
                    if (validh) {
                        memset(&prescale[i * PRESCALE_WIDTH + h_start], 0, hres * sizeof(uint32_t));
                    } else {
                        memset(&prescale[i * PRESCALE_WIDTH], 0, PRESCALE_WIDTH * sizeof(uint32_t));
                    }
                }
            }
        }

        if (!ctrl.serrate) {
            for(j = 0; j < vres; j++) {
                if (validh) {
                    tvfadeoutstate[i] = 2;
                } else if (tvfadeoutstate[i]) {
                    tvfadeoutstate[i]--;
                    if (!tvfadeoutstate[i]) {
                        memset(&prescale[i * PRESCALE_WIDTH], 0, PRESCALE_WIDTH * sizeof(uint32_t));
                    }
                }

                i++;
            }
        } else {
            for(j = 0; j < vres; j++) {
                if (validh) {
                    tvfadeoutstate[i] = 2;
                } else if (tvfadeoutstate[i]) {
                    tvfadeoutstate[i]--;
                    if (!tvfadeoutstate[i]) {
                        memset(&prescale[i * PRESCALE_WIDTH], 0, PRESCALE_WIDTH * sizeof(uint32_t));
                    }
                }

                if (tvfadeoutstate[i + 1]) {
                    tvfadeoutstate[i + 1]--;
                    if (!tvfadeoutstate[i + 1]) {
                        if (validh) {
                            memset(&prescale[(i + 1) * PRESCALE_WIDTH + h_start], 0, hres * sizeof(uint32_t));
                        } else {
                            memset(&prescale[(i + 1) * PRESCALE_WIDTH], 0, PRESCALE_WIDTH * sizeof(uint32_t));
                        }
                    }
                }

                i += 2;
            }
        }

        // clear bottom border
        for (; i < vactivelines; i++) {
            if (tvfadeoutstate[i]) {
                tvfadeoutstate[i]--;
            }
            if (!tvfadeoutstate[i]) {
                if (validh) {
                    memset(&prescale[i * PRESCALE_WIDTH + h_start], 0, hres * sizeof(uint32_t));
                } else {
                    memset(&prescale[i * PRESCALE_WIDTH], 0, PRESCALE_WIDTH * sizeof(uint32_t));
                }
            }
        }
    }

    if (!validh) {
        return false;
    }

    // run filter update in parallel if enabled
    if (config.parallel) {
        parallel_run(vi_process_full_parallel);
    } else {
        vi_process_full_parallel(0);
    }

    // finish and send buffer to screen
    struct frame_buffer fb;
    fb.pixels = prescale;
    fb.pitch = PRESCALE_WIDTH;

    int32_t output_height;

    if (config.vi.hide_overscan) {
        // crop away overscan area from prescale
        fb.width = maxhpass - minhpass;
        fb.height = vres << ctrl.serrate;
        output_height = (vres << 1) * V_SYNC_NTSC / v_sync;
        int32_t x = h_start + minhpass;
        int32_t y = (v_start + (emucontrolsvicurrent ? lowerfield : 0)) << ctrl.serrate;
        fb.pixels += x + y * fb.pitch;
    } else {
        // use entire prescale buffer
        fb.width = PRESCALE_WIDTH;
        fb.height = (ispal ? V_RES_PAL : V_RES_NTSC) >> !ctrl.serrate;
        output_height = V_RES_NTSC;
    }

    // convert to 16:9 if enabled
    if (config.vi.widescreen) {
        output_height = output_height * 3 / 4;
    }

    screen_write(&fb, output_height);
    return fb.width > 0 && fb.height > 0;
}

static void vi_process_fast_parallel(uint32_t worker_id)
{
    int32_t y;
    int32_t y_begin = 0;
    int32_t y_end = vres_raw;
    int32_t y_inc = 1;

    // drop every other interlaced frame to avoid "wobbly" output due to the
    // vertical offset
    // TODO: completely skip rendering these frames in unfiltered to improve
    // performance?
    if (ctrl.serrate && v_current_line) {
        return;
    }

    if (config.parallel) {
        y_begin = worker_id;
        y_inc = parallel_num_workers();
    }

    for (y = y_begin; y < y_end; y += y_inc) {
        int32_t x;
        int32_t line = y * vi_width_low;

        struct rgba* pixel_row = &prescale[y * hres_raw];

        for (x = 0; x < hres_raw; x++) {
            struct rgba* pixel = &pixel_row[x];

            switch (config.vi.mode) {
                case VI_MODE_COLOR:
                    switch (ctrl.type) {
                        case VI_TYPE_RGBA5551: {
                            uint16_t pix = rdram_read_idx16((frame_buffer >> 1) + line + x);
                            pixel->r = RGBA16_R(pix);
                            pixel->g = RGBA16_G(pix);
                            pixel->b = RGBA16_B(pix);
                            break;
                        }

                        case VI_TYPE_RGBA8888: {
                            uint32_t pix = rdram_read_idx32((frame_buffer >> 2) + line + x);
                            pixel->r = RGBA32_R(pix);
                            pixel->g = RGBA32_G(pix);
                            pixel->b = RGBA32_B(pix);
                            break;
                        }

                        default:
                            return;
                    }

                    gamma_filters(pixel, ctrl, &rseed);
                    break;

                case VI_MODE_DEPTH: {
                    if (zb_address) {
                        pixel->r = pixel->g = pixel->b = rdram_read_idx16((zb_address >> 1) + line + x) >> 8;
                    }
                    break;
                }

                case VI_MODE_COVERAGE: {
                    // TODO: incorrect for RGBA8888?
                    uint8_t hval;
                    uint16_t pix;
                    rdram_read_pair16(&pix, &hval, (frame_buffer >> 1) + line + x);
                    pixel->r = pixel->g = pixel->b = (((pix & 1) << 2) | hval) << 5;
                    break;
                }

                default:
                    return;
            }
        }
    }
}

static bool vi_process_fast(void)
{
    // note: this is probably a very, very crude method to get the frame size,
    // but should hopefully work most of the time
    hres_raw = (int32_t)x_add * hres / 1024;
    vres_raw = (int32_t)y_add * vres / 1024;

    // skip invalid frame sizes
    if (hres_raw <= 0 || vres_raw <= 0) {
        return false;
    }

    // skip blank/invalid modes
    if (!(ctrl.type & 2)) {
        return false;
    }

    // run filter update in parallel if enabled
    if (config.parallel) {
        parallel_run(vi_process_fast_parallel);
    } else {
        vi_process_fast_parallel(0);
    }

    // finish and send buffer to screen
    struct frame_buffer fb;
    fb.pixels = prescale;
    fb.width = hres_raw;
    fb.height = vres_raw;
    fb.pitch = hres_raw;

    // get display size of filtered mode
    int32_t filtered_width = maxhpass - minhpass;
    int32_t filtered_height = (vres << 1) * V_SYNC_NTSC / v_sync;

    // re-calculate cropped 8 pixel area on the left and right from filtered mode
    int32_t border_width = (hres - filtered_width) * hres_raw / hres;
    fb.pixels += (border_width / 2) + 1;
    fb.width -= border_width;

    // force aspect ratio of filtered mode
    int32_t output_height = fb.width * filtered_height / filtered_width;

    // convert to 16:9 if enabled
    if (config.vi.widescreen) {
        output_height = output_height * 3 / 4;
    }

    screen_write(&fb, output_height);
    return fb.width > 0 && fb.height > 0;
}

void vi_set_zbuffer_address(uint32_t address)
{
    zb_address = address;
}

void n64video_update_screen(void)
{
    // check for configuration errors
    if (config.vi.mode >= VI_MODE_NUM) {
        msg_error("Invalid VI mode: %d", config.vi.mode);
    }

    // parse and check some common registers
    vi_reg_ptr = plugin_get_vi_registers();

    v_start = (*vi_reg_ptr[VI_V_START] >> 16) & 0x3ff;
    h_start = (*vi_reg_ptr[VI_H_START] >> 16) & 0x3ff;

    int32_t v_end = *vi_reg_ptr[VI_V_START] & 0x3ff;
    int32_t h_end = *vi_reg_ptr[VI_H_START] & 0x3ff;

    hres =  h_end - h_start;
    vres = (v_end - v_start) >> 1; // vertical is measured in half-lines

    x_add = *vi_reg_ptr[VI_X_SCALE] & 0xfff;
    x_start = (*vi_reg_ptr[VI_X_SCALE] >> 16) & 0xfff;

    y_add = *vi_reg_ptr[VI_Y_SCALE] & 0xfff;
    y_start = (*vi_reg_ptr[VI_Y_SCALE] >> 16) & 0xfff;

    v_sync = *vi_reg_ptr[VI_V_SYNC] & 0x3ff;
    v_current_line = *vi_reg_ptr[VI_V_CURRENT_LINE] & 1;

    vi_width_low = *vi_reg_ptr[VI_WIDTH] & 0xfff;
    frame_buffer = *vi_reg_ptr[VI_ORIGIN] & 0xffffff;

    // cancel if the frame buffer contains no valid address
    if (!frame_buffer) {
        screen_swap(true);
        return;
    }

    // split up VI_CONTROL bits
    uint32_t vi_control = *vi_reg_ptr[VI_STATUS];
    ctrl.type = vi_control & 3;
    ctrl.gamma_dither_enable = (vi_control >> 2) & 1;
    ctrl.gamma_enable = (vi_control >> 3) & 1;
    ctrl.divot_enable = (vi_control >> 4) & 1;
    ctrl.vbus_clock_enable = (vi_control >> 5) & 1;
    ctrl.serrate = (vi_control >> 6) & 1;
    ctrl.test_mode = (vi_control >> 7) & 1;
    ctrl.aa_mode = (vi_control >> 8) & 3;
    ctrl.reserved = (vi_control >> 9) & 1;
    ctrl.kill_we = (vi_control >> 10) & 1;
    ctrl.pixel_advance = (vi_control >> 12) & 0xf;
    ctrl.dither_filter_enable = (vi_control >> 16) & 1;

    // check for unexpected VI type bits set
    if (ctrl.type & ~3) {
        msg_error("Unknown framebuffer format %d", ctrl.type);
    }

    // warn about AA glitches in certain cases
    if (ctrl.aa_mode == VI_AA_REPLICATE && ctrl.type == VI_TYPE_RGBA5551 &&
        h_start < 0x80 && x_add <= 0x200 && !onetimewarnings.nolerp) {
        msg_warning("vi_update: Disabling VI interpolation in 16-bit color "
                    "modes causes glitches on hardware if h_start is less than "
                    "128 pixels and x_scale is less or equal to 0x200.");
        onetimewarnings.nolerp = true;
    }

    // check for the dangerous vbus_clock_enable flag. it was introduced to
    // configure Ultra 64 prototypes and enabling it on final hardware will
    // enable two output drivers on the same bus at the same time
    if (ctrl.vbus_clock_enable && !onetimewarnings.vbusclock) {
        msg_warning("vi_update: vbus_clock_enable bit set in VI_CONTROL_REG "
                    "register. Never run this code on your N64! It's rumored "
                    "that turning this bit on will result in permanent damage "
                    "to the hardware! Emulation will now continue.");
        onetimewarnings.vbusclock = true;
    }

    // adjust sizes and offsets
    ispal = v_sync > (V_SYNC_NTSC + 25);
    h_start -= (ispal ? 128 : 108);

    bool h_start_clamped = false;

    if (h_start < 0) {
        x_start += (x_add * (-h_start));
        hres += h_start;

        h_start = 0;
        h_start_clamped = true;
    }

    int32_t vstartoffset = ispal ? 44 : 34;
    v_start = (v_start - vstartoffset) / 2;

    if (v_start < 0) {
        y_start += (y_add * (uint32_t)(-v_start));
        v_start = 0;
    }

    bool hres_clamped = false;

    if ((hres + h_start) > PRESCALE_WIDTH) {
        hres = PRESCALE_WIDTH - h_start;
        hres_clamped = true;
    }

    if ((vres + v_start) > PRESCALE_HEIGHT) {
        vres = PRESCALE_HEIGHT - v_start;
        msg_warning("vres = %d v_start = %d v_video_start = %d", vres, v_start, (*vi_reg_ptr[VI_V_START] >> 16) & 0x3ff);
    }

    vactivelines = v_sync - vstartoffset;

    if (vactivelines > PRESCALE_HEIGHT) {
        msg_error("VI_V_SYNC_REG too big");
    }

    bool blank = true;

    if (vactivelines >= 0) {
        uint32_t lineshifter = !ctrl.serrate;
        vactivelines >>= lineshifter;

        minhpass = h_start_clamped ? 0 : 8;
        maxhpass = hres_clamped ? hres : (hres - 7);

        // run filter update in parallel if enabled
        if (config.vi.mode == VI_MODE_NORMAL) {
            blank = !vi_process_full();
        } else {
            blank = !vi_process_fast();
        }
    }

    // render frame to screen or blank screen if the frame is invalid
    screen_swap(blank);
}

static void vi_close(void)
{
}
