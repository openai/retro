/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-Next - libretro.c                                       *
 *   Copyright (C) 2020 M4xw <m4x@m4xw.net                               *
 *                                                                    *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                 *
 *                                                                    *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                         *
 *                                                                    *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                        *
 *   Free Software Foundation, Inc.,                                     *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libretro.h"
#include "libretro_private.h"
#include "GLideN64_libretro.h"
#include "mupen64plus-next_common.h"

#include <libco.h>

#ifdef HAVE_LIBNX
#include <switch.h>
#endif

#include <glsm/glsmsym.h>

#include "api/m64p_frontend.h"
#include "api/m64p_types.h"
#include "device/r4300/r4300_core.h"
#include "device/memory/memory.h"
#include "main/main.h"
#include "api/callbacks.h"
#include "main/cheat.h"
#include "main/version.h"
#include "main/util.h"
#include "main/savestates.h"
#include "main/mupen64plus.ini.h"
#include "api/m64p_config.h"
#include "osal_files.h"
#include "main/rom.h"
#include "plugin/plugin.h"
#include "device/rcp/pi/pi_controller.h"
#include "device/pif/pif.h"
#include "libretro_memory.h"

#include "audio_plugin.h"

#ifndef CORE_NAME
#define CORE_NAME "mupen64plus"
#endif

#ifndef PRESCALE_WIDTH
#define PRESCALE_WIDTH  640
#endif

#ifndef PRESCALE_HEIGHT
#define PRESCALE_HEIGHT 625
#endif

#define PATH_SIZE 2048

#define ISHEXDEC ((codeLine[cursor]>='0') && (codeLine[cursor]<='9')) || ((codeLine[cursor]>='a') && (codeLine[cursor]<='f')) || ((codeLine[cursor]>='A') && (codeLine[cursor]<='F'))

/* Forward declarations */
#ifdef HAVE_THR_AL
void angrylion_set_filtering(unsigned filter_type);
void angrylion_set_vi_blur(unsigned value);
void angrylion_set_threads(unsigned value);
void angrylion_set_overscan(unsigned value);
void angrylion_set_synclevel(unsigned value);
void angrylion_set_vi_dedither(unsigned value);
void angrylion_set_vi(unsigned value);

struct rgba
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
};
extern struct rgba prescale[PRESCALE_WIDTH * PRESCALE_HEIGHT];
#endif // HAVE_THR_AL

// Option entries
#define OPTION_ENTRY_RDP_GLIDEN64 "gliden64"
#define OPTION_ENTRY_RSP_HLE "hle"

#ifdef HAVE_THR_AL
#define OPTION_ENTRY_RDP_ANGRYLION "|angrylion"
#else
#define OPTION_ENTRY_RDP_ANGRYLION ""
#endif // HAVE_THR_AL

#ifdef HAVE_PARALLEL_RSP
#define OPTION_ENTRY_RSP_PARALLEL "|parallel"
#else
#define OPTION_ENTRY_RSP_PARALLEL ""
#endif // HAVE_PARALLEL_RSP

#ifdef HAVE_LLE
#define OPTION_ENTRY_RSP_CXD4 "|cxd4"
#else
#define OPTION_ENTRY_RSP_CXD4 ""
#endif // HAVE_LLE

struct retro_perf_callback perf_cb;
retro_get_cpu_features_t perf_get_cpu_features_cb = NULL;

retro_log_printf_t log_cb = NULL;
retro_video_refresh_t video_cb = NULL;
retro_input_poll_t poll_cb = NULL;
retro_input_state_t input_cb = NULL;
retro_audio_sample_batch_t audio_batch_cb = NULL;
retro_environment_t environ_cb = NULL;

struct retro_rumble_interface rumble;

save_memory_data saved_memory;

static cothread_t game_thread;
cothread_t retro_thread;

int astick_deadzone;
int astick_sensitivity;
int r_cbutton;
int l_cbutton;
int d_cbutton;
int u_cbutton;
bool alternate_mapping;

static uint8_t* game_data = NULL;
static uint32_t game_size = 0;

static bool     emu_initialized     = false;
static unsigned initial_boot        = true;
static unsigned audio_buffer_size   = 2048;

static unsigned retro_filtering     = 0;
static bool     first_context_reset = false;
static bool     initializing        = true;

bool libretro_swap_buffer;

uint32_t *blitter_buf = NULL;
uint32_t *blitter_buf_lock = NULL;
uint32_t retro_screen_width = 640;
uint32_t retro_screen_height = 480;
uint32_t screen_pitch = 0;

float retro_screen_aspect = 4.0 / 3.0;

char* retro_dd_path_img;
char* retro_dd_path_rom;

uint32_t bilinearMode = 0;
uint32_t EnableHWLighting = 0;
uint32_t CorrectTexrectCoords = 0;
uint32_t enableNativeResTexrects = 0;
uint32_t enableLegacyBlending = 0;
uint32_t EnableCopyColorToRDRAM = 0;
uint32_t EnableCopyDepthToRDRAM = 0;
uint32_t AspectRatio = 0;
uint32_t MaxTxCacheSize = 0;
uint32_t txFilterMode = 0;
uint32_t txEnhancementMode = 0;
uint32_t txHiresEnable = 0;
uint32_t txHiresFullAlphaChannel = 0;
uint32_t txFilterIgnoreBG = 0;
uint32_t EnableFXAA = 0;
uint32_t MultiSampling = 0;
uint32_t EnableFragmentDepthWrite = 0;
uint32_t EnableShadersStorage = 0;
uint32_t EnableTextureCache = 0;
uint32_t EnableFBEmulation = 0;
uint32_t EnableFrameDuping = 0;
uint32_t EnableNoiseEmulation = 0;
uint32_t EnableLODEmulation = 0;
uint32_t EnableFullspeed = 0;
uint32_t CountPerOp = 0;
uint32_t CountPerScanlineOverride = 0;
uint32_t BackgroundMode = 0; // 0 is bgOnePiece
uint32_t EnableEnhancedTextureStorage;
uint32_t EnableEnhancedHighResStorage;
uint32_t EnableTxCacheCompression = 0;
uint32_t ForceDisableExtraMem = 0;
uint32_t EnableNativeResFactor = 0;
uint32_t EnableN64DepthCompare = 0;

// Overscan options
#define GLN64_OVERSCAN_SCALING "0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50"
uint32_t EnableOverscan = 0;
uint32_t OverscanTop = 0;
uint32_t OverscanLeft = 0;
uint32_t OverscanRight = 0;
uint32_t OverscanBottom = 0;

extern struct device g_dev;
extern unsigned int r4300_emumode;
extern struct cheat_ctx g_cheat_ctx;

// after the controller's CONTROL* member has been assigned we can update
// them straight from here...
extern struct
{
    CONTROL *control;
    BUTTONS buttons;
} controller[4];
// ...but it won't be at least the first time we're called, in that case set
// these instead for input_plugin to read.
int pad_pak_types[4];
int pad_present[4] = {1, 1, 1, 1};

static void n64DebugCallback(void* aContext, int aLevel, const char* aMessage)
{
    char buffer[1024];
    snprintf(buffer, 1024, CORE_NAME ": %s\n", aMessage);
    if (log_cb)
        log_cb(RETRO_LOG_INFO, buffer);
}

extern m64p_rom_header ROM_HEADER;

static void setup_variables(void)
{
    struct retro_variable variables[] = {
        { CORE_NAME "-cpucore",
#ifdef DYNAREC
            "CPU Core; dynamic_recompiler|cached_interpreter|pure_interpreter" },
#else
            "CPU Core; cached_interpreter|pure_interpreter" },
#endif
        { CORE_NAME "-rdp-plugin",
            "RDP Mode; " OPTION_ENTRY_RDP_GLIDEN64 OPTION_ENTRY_RDP_ANGRYLION },
        { CORE_NAME "-rsp-plugin",
            "RSP Mode; " OPTION_ENTRY_RSP_HLE OPTION_ENTRY_RSP_PARALLEL OPTION_ENTRY_RSP_CXD4 },
        { CORE_NAME "-43screensize",
            "(GLN64) 4:3 Resolution; 640x480|320x240|960x720|1280x960|1440x1080|1600x1200|1920x1440|2240x1680|2560x1920|2880x2160|3200x2400|3520x2640|3840x2880" },
        { CORE_NAME "-169screensize",
            "(GLN64) 16:9 Resolution; 960x540|640x360|1280x720|1920x1080|2560x1440|3840x2160|4096x2160|7680x4320" },
        { CORE_NAME "-aspect",
            "(GLN64) Aspect Ratio; 4:3|16:9|16:9 adjusted" },
        { CORE_NAME "-BilinearMode",
            "(GLN64) Bilinear filtering mode; standard|3point" },
#ifndef HAVE_OPENGLES2
        { CORE_NAME "-MultiSampling",
            "(GLN64) MSAA level; 0|2|4|8|16" },
#endif
        { CORE_NAME "-FXAA",
            "(GLN64) FXAA; 0|1" },

        { CORE_NAME "-NoiseEmulation",
            "(GLN64) Noise Emulation; True|False" },

        { CORE_NAME "-EnableFBEmulation",
#ifdef VC
            "(GLN64) Framebuffer Emulation; False|True" },
#else
            "(GLN64) Framebuffer Emulation; True|False" },
#endif

        { CORE_NAME "-EnableLODEmulation",
            "(GLN64) LOD Emulation; True|False" },
        { CORE_NAME "-EnableCopyColorToRDRAM",
#ifndef HAVE_OPENGLES
            "(GLN64) Color buffer to RDRAM; Async|Sync|Off" },
#else
            "(GLN64) Color buffer to RDRAM; Off|Async|Sync" },
#endif
        { CORE_NAME "-EnableCopyDepthToRDRAM",
            "(GLN64) Depth buffer to RDRAM; Software|FromMem|Off" },
        { CORE_NAME "-BackgroundMode",
            "(GLN64) Background Mode; OnePiece|Stripped" },
        { CORE_NAME "-EnableHWLighting",
            "(GLN64) Hardware per-pixel lighting; False|True" },
        { CORE_NAME "-CorrectTexrectCoords",
            "(GLN64) Continuous texrect coords; Off|Auto|Force" },
        { CORE_NAME "-EnableNativeResTexrects",
            "(GLN64) Native res. 2D texrects; Disabled|Optimized|Unoptimized" },
#if defined(HAVE_OPENGLES)
        { CORE_NAME "-EnableLegacyBlending",
            "(GLN64) Less accurate blending mode; True|False" },
        { CORE_NAME "-EnableFragmentDepthWrite",
            "(GLN64) GPU shader depth write; False|True" },
#else
        { CORE_NAME "-EnableLegacyBlending",
            "(GLN64) Less accurate blending mode; False|True" },
        { CORE_NAME "-EnableFragmentDepthWrite",
            "(GLN64) GPU shader depth write; True|False" },
#endif
#if !defined(VC) && !defined(HAVE_OPENGLES)
        // Not supported on all GPU's
        { CORE_NAME "-EnableN64DepthCompare",
            "(GLN64) N64 Depth Compare; False|True" },
        { CORE_NAME "-EnableShadersStorage",
            "(GLN64) Cache GPU Shaders; True|False" },
#endif // !defined(VC) && !defined(HAVE_OPENGLES)
        { CORE_NAME "-EnableTextureCache",
            "(GLN64) Cache Textures; True|False" },
        { CORE_NAME "-EnableOverscan",
            "(GLN64) Overscan; Enabled|Disabled" },
        { CORE_NAME "-OverscanTop",
            "(GLN64) Overscan Offset (Top); " GLN64_OVERSCAN_SCALING },
        { CORE_NAME "-OverscanLeft",
            "(GLN64) Overscan Offset (Left); " GLN64_OVERSCAN_SCALING },
        { CORE_NAME "-OverscanRight",
            "(GLN64) Overscan Offset (Right); " GLN64_OVERSCAN_SCALING },
        { CORE_NAME "-OverscanBottom",
            "(GLN64) Overscan Offset (Bottom); " GLN64_OVERSCAN_SCALING },

        { CORE_NAME "-MaxTxCacheSize",
#if defined(VC)
            "(GLN64) Max texture cache size; 1500|8000|4000" },
#elif defined(HAVE_LIBNX)
            "(GLN64) Max texture cache size; 4000|1500|8000" },
#else
            "(GLN64) Max texture cache size; 8000|4000|1500" },
#endif
        { CORE_NAME "-txFilterMode",
            "(GLN64) Texture filter; None|Smooth filtering 1|Smooth filtering 2|Smooth filtering 3|Smooth filtering 4|Sharp filtering 1|Sharp filtering 2" },
        { CORE_NAME "-txEnhancementMode",
            "(GLN64) Texture Enhancement; None|As Is|X2|X2SAI|HQ2X|HQ2XS|LQ2X|LQ2XS|HQ4X|2xBRZ|3xBRZ|4xBRZ|5xBRZ|6xBRZ" },
        { CORE_NAME "-txFilterIgnoreBG",
            "(GLN64) Filter background textures; True|False" },
        { CORE_NAME "-txHiresEnable",
            "(GLN64) Use High-Res textures; False|True" },
        { CORE_NAME "-txCacheCompression",
            "(GLN64) Use High-Res Texture Cache Compression; True|False" },
        { CORE_NAME "-txHiresFullAlphaChannel",
            "(GLN64) Use High-Res Full Alpha Channel; False|True" },
        { CORE_NAME "-EnableEnhancedTextureStorage",
            "(GLN64) Use enhanced Texture Storage; False|True" },
        { CORE_NAME "-EnableEnhancedHighResStorage",
            "(GLN64) Use enhanced Hi-Res Storage; False|True" },
#ifdef HAVE_THR_AL
        { CORE_NAME "-angrylion-vioverlay",
            "(AL) VI Overlay; Filtered|AA+Blur|AA+Dedither|AA only|Unfiltered|Depth|Coverage" },
        { CORE_NAME "-angrylion-sync",
            "(AL) Thread sync level; Low|Medium|High" },
        { CORE_NAME "-angrylion-multithread",
            "(AL) Multi-threading; all threads|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57|58|59|60|61|62|63" },
        { CORE_NAME "-angrylion-overscan",
            "(AL) Hide overscan; disabled|enabled" },
#endif // HAVE_THR_AL
        { CORE_NAME "-FrameDuping",
#ifdef HAVE_LIBNX
            "Frame Duplication; True|False" },
#else
            "Frame Duplication; False|True" },
#endif
        { CORE_NAME "-Framerate",
            "Framerate; Original|Fullspeed" },
        { CORE_NAME "-virefresh",
            "VI Refresh (Overclock); Auto|1500|2200" },
        { CORE_NAME "-astick-deadzone",
           "Analog Deadzone (percent); 15|20|25|30|0|5|10"},
        { CORE_NAME "-astick-sensitivity",
           "Analog Sensitivity (percent); 100|105|110|115|120|125|130|135|140|145|150|50|55|60|65|70|75|80|85|90|95"},
        { CORE_NAME "-r-cbutton",
           "Right C Button; C1|C2|C3|C4"},
        { CORE_NAME "-l-cbutton",
           "Left C Button; C2|C3|C4|C1"},
        { CORE_NAME "-d-cbutton",
           "Down C Button; C3|C4|C1|C2"},
        { CORE_NAME "-u-cbutton",
           "Up C Button; C4|C1|C2|C3"},
        { CORE_NAME "-alt-map",
           "Independent C-button Controls; False|True" },
        { CORE_NAME "-ForceDisableExtraMem",
           "Disable Expansion Pak; False|True"},
        { CORE_NAME "-pak1",
           "Player 1 Pak; memory|rumble|none"},
        { CORE_NAME "-pak2",
           "Player 2 Pak; none|memory|rumble"},
        { CORE_NAME "-pak3",
           "Player 3 Pak; none|memory|rumble"},
        { CORE_NAME "-pak4",
           "Player 4 Pak; none|memory|rumble"},
        { CORE_NAME "-CountPerOp",
            "Count Per Op; 0|1|2|3" },
        { NULL, NULL },
    };

    static const struct retro_controller_description port[] = {
        { "Controller", RETRO_DEVICE_JOYPAD },
        { "RetroPad", RETRO_DEVICE_JOYPAD },
    };

    static const struct retro_controller_info ports[] = {
        { port, 2 },
        { port, 2 },
        { port, 2 },
        { port, 2 },
        { 0, 0 }
    };

    environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
    environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}


static bool emu_step_load_data()
{
    m64p_error ret = CoreStartup(FRONTEND_API_VERSION, ".", ".", "Core", n64DebugCallback, 0, 0);
    if(ret && log_cb)
        log_cb(RETRO_LOG_ERROR, CORE_NAME ": failed to initialize core (err=%i)\n", ret);

    log_cb(RETRO_LOG_DEBUG, CORE_NAME ": [EmuThread] M64CMD_ROM_OPEN\n");

    if(CoreDoCommand(M64CMD_ROM_OPEN, game_size, (void*)game_data))
    {
        if (log_cb)
            log_cb(RETRO_LOG_ERROR, CORE_NAME ": failed to load ROM\n");
        goto load_fail;
    }

    free(game_data);
    game_data = NULL;

    log_cb(RETRO_LOG_DEBUG, CORE_NAME ": [EmuThread] M64CMD_ROM_GET_HEADER\n");

    if(CoreDoCommand(M64CMD_ROM_GET_HEADER, sizeof(ROM_HEADER), &ROM_HEADER))
    {
        if (log_cb)
            log_cb(RETRO_LOG_ERROR, CORE_NAME ": failed to query ROM header information\n");
        goto load_fail;
    }

    return true;

load_fail:
    free(game_data);
    game_data = NULL;
    //stop = 1;

    return false;
}

static void emu_step_initialize(void)
{
    if (emu_initialized)
        return;

    emu_initialized = true;

    plugin_connect_all();
}

static void EmuThreadFunction(void)
{
    log_cb(RETRO_LOG_DEBUG, CORE_NAME ": [EmuThread] M64CMD_EXECUTE\n");

    initializing = false;
    CoreDoCommand(M64CMD_EXECUTE, 0, NULL);
}

void reinit_gfx_plugin(void)
{
    if(first_context_reset)
    {
        first_context_reset = false;
        emu_step_initialize();
    }
}

const char* retro_get_system_directory(void)
{
    const char* dir;
    environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir);

    return dir ? dir : ".";
}


void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb)   { }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_cb = cb; }

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    if(retro_dd_path_img)
    {
        free(retro_dd_path_img);
        retro_dd_path_img = NULL;
    }

    if(retro_dd_path_rom)
    {
        free(retro_dd_path_rom);
        retro_dd_path_rom = NULL;
    }

    switch(game_type)
    {
        case RETRO_GAME_TYPE_DD:
            if(num_info == 1)
            {
                retro_dd_path_img = strdup(info[0].path);
            }
            else if(num_info == 2)
            {
                retro_dd_path_img = strdup(info[0].path);
                retro_dd_path_rom = strdup(info[1].path);
            } else {
                return false;
            }
            
            printf("Loading %s...\n", info[0].path);
            load_file(info[1].path, (void**)&info[1].data, &info[1].size);
            return retro_load_game(&info[1]);
        default:
            return false;
    }
    
	return false;
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;

    static const struct retro_subsystem_memory_info memory_info[] = {
        { "srm", RETRO_MEMORY_DD },
    };

    static const struct retro_subsystem_rom_info dd_roms[] = {
        { "Disk", "ndd", true, false, true, memory_info, 1 },
        { "Cartridge", "n64|v64|z64|bin|u1", true, false, true, NULL, 0 },
    };

    static const struct retro_subsystem_info subsystems[] = {
        { "N64 Disk Drive", "ndd", dd_roms, 2, RETRO_GAME_TYPE_DD },
        {}
    };

    environ_cb(RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO, (void*)subsystems);
    
    setup_variables();
}

void retro_get_system_info(struct retro_system_info *info)
{
#if defined(HAVE_OPENGLES2)
    info->library_name = "Mupen64Plus-Next GLES2";
#elif defined(HAVE_OPENGLES3)
    info->library_name = "Mupen64Plus-Next GLES3";
#else
    info->library_name = "Mupen64Plus-Next OpenGL";
#endif
#ifndef GIT_VERSION
#define GIT_VERSION " git"
#endif
    info->library_version = "2.0.5" GIT_VERSION;
    info->valid_extensions = "n64|v64|z64|bin|u1";
    info->need_fullpath = false;
    info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
    info->geometry.base_width   = retro_screen_width;
    info->geometry.base_height  = retro_screen_height;
    info->geometry.max_width    = retro_screen_width;
    info->geometry.max_height   = retro_screen_height;
    info->geometry.aspect_ratio = retro_screen_aspect;
    info->timing.fps = vi_expected_refresh_rate_from_tv_standard(ROM_PARAMS.systemtype);
    info->timing.sample_rate = 44100.0;
}

unsigned retro_get_region (void)
{
    return ((ROM_PARAMS.systemtype == SYSTEM_PAL) ? RETRO_REGION_PAL : RETRO_REGION_NTSC);
}

void copy_file(char * ininame, char * fileName)
{
    const char* filename = ConfigGetSharedDataFilepath(fileName);
    FILE *fp = fopen(filename, "w");
    if (fp != NULL)    {
        fputs(ininame, fp);
        fclose(fp);
    }
}

void retro_init(void)
{
    char* sys_pathname;
    wchar_t w_pathname[PATH_SIZE];
    environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &sys_pathname);
    char pathname[PATH_SIZE];
    strncpy(pathname, sys_pathname, PATH_SIZE);
    if (pathname[(strlen(pathname)-1)] != '/' && pathname[(strlen(pathname)-1)] != '\\')
        strcat(pathname, "/");
    strcat(pathname, "Mupen64plus/");
    mbstowcs(w_pathname, pathname, PATH_SIZE);
    if (!osal_path_existsW(w_pathname) || !osal_is_directory(w_pathname))
        osal_mkdirp(w_pathname);
    copy_file(inifile, "mupen64plus.ini");

    struct retro_log_callback log;
    unsigned colorMode = RETRO_PIXEL_FORMAT_XRGB8888;

    if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
        log_cb = log.log;
    else
        log_cb = NULL;

    if (environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb))
        perf_get_cpu_features_cb = perf_cb.get_cpu_features;
    else
        perf_get_cpu_features_cb = NULL;

    environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &colorMode);
    environ_cb(RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE, &rumble);
    initializing = true;

    retro_thread = co_active();
    game_thread = co_create(65536 * sizeof(void*) * 16, EmuThreadFunction);
}

void retro_deinit(void)
{
    CoreDoCommand(M64CMD_STOP, 0, NULL);
    co_switch(game_thread); /* Let the core thread finish */
    deinit_audio_libretro();

    if (perf_cb.perf_log)
        perf_cb.perf_log();
}

void update_controllers()
{
    struct retro_variable pk1var = { CORE_NAME "-pak1" };
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &pk1var) && pk1var.value)
    {
        int p1_pak = PLUGIN_NONE;
        if (!strcmp(pk1var.value, "rumble"))
            p1_pak = PLUGIN_RAW;
        else if (!strcmp(pk1var.value, "memory"))
            p1_pak = PLUGIN_MEMPAK;

        // If controller struct is not initialised yet, set pad_pak_types instead
        // which will be looked at when initialising the controllers.
        if (controller[0].control)
            controller[0].control->Plugin = p1_pak;
        else
            pad_pak_types[0] = p1_pak;
    }

    struct retro_variable pk2var = { CORE_NAME "-pak2" };
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &pk2var) && pk2var.value)
    {
        int p2_pak = PLUGIN_NONE;
        if (!strcmp(pk2var.value, "rumble"))
            p2_pak = PLUGIN_RAW;
        else if (!strcmp(pk2var.value, "memory"))
            p2_pak = PLUGIN_MEMPAK;

        if (controller[1].control)
            controller[1].control->Plugin = p2_pak;
        else
            pad_pak_types[1] = p2_pak;
    }

    struct retro_variable pk3var = { CORE_NAME "-pak3" };
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &pk3var) && pk3var.value)
    {
        int p3_pak = PLUGIN_NONE;
        if (!strcmp(pk3var.value, "rumble"))
            p3_pak = PLUGIN_RAW;
        else if (!strcmp(pk3var.value, "memory"))
            p3_pak = PLUGIN_MEMPAK;

        if (controller[2].control)
            controller[2].control->Plugin = p3_pak;
        else
            pad_pak_types[2] = p3_pak;
    }

    struct retro_variable pk4var = { CORE_NAME "-pak4" };
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &pk4var) && pk4var.value)
    {
        int p4_pak = PLUGIN_NONE;
        if (!strcmp(pk4var.value, "rumble"))
            p4_pak = PLUGIN_RAW;
        else if (!strcmp(pk4var.value, "memory"))
            p4_pak = PLUGIN_MEMPAK;

        if (controller[3].control)
            controller[3].control->Plugin = p4_pak;
        else
            pad_pak_types[3] = p4_pak;
    }
}

static void update_variables(bool startup)
{
    struct retro_variable var;

    if (startup)
    {
       var.key = CORE_NAME "-rdp-plugin";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "gliden64"))
          {
            plugin_connect_rdp_api(RDP_PLUGIN_GLIDEN64);
          }
          else if (!strcmp(var.value, "angrylion"))
          {
             plugin_connect_rdp_api(RDP_PLUGIN_ANGRYLION);
          }
       }
       else
       {
          plugin_connect_rdp_api(RDP_PLUGIN_GLIDEN64);
       }

       var.key = CORE_NAME "-rsp-plugin";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "hle"))
          {
             // If we use angrylion with hle, we will force parallel if available!
             if(current_rdp_type != RDP_PLUGIN_ANGRYLION)
             {
                plugin_connect_rsp_api(RSP_PLUGIN_HLE);
             }
             else
             {
#if defined(HAVE_PARALLEL_RSP)
                plugin_connect_rsp_api(RSP_PLUGIN_PARALLEL);
                log_cb(RETRO_LOG_INFO, "Selected HLE RSP with Angrylion, falling back to Parallel RSP!\n");
#elif defined(HAVE_LLE)
                plugin_connect_rsp_api(RSP_PLUGIN_CXD4);
                log_cb(RETRO_LOG_INFO, "Selected HLE RSP with Angrylion, falling back to CXD4!\n");
#else
                log_cb(RETRO_LOG_INFO, "Requested Angrylion but no LLE RSP available, falling back to GLideN64!\n");
                plugin_connect_rsp_api(RSP_PLUGIN_HLE);
                plugin_connect_rdp_api(RDP_PLUGIN_GLIDEN64);
#endif 
             }
          }
          else if (!strcmp(var.value, "cxd4"))
          {
             plugin_connect_rsp_api(RSP_PLUGIN_CXD4);
          }
          else if (!strcmp(var.value, "parallel"))
          {
             plugin_connect_rsp_api(RSP_PLUGIN_PARALLEL);
          }
       }
       else
       {
          if(current_rdp_type != RDP_PLUGIN_ANGRYLION)
          {
                plugin_connect_rsp_api(RSP_PLUGIN_HLE);
          }
          else
          {
#if defined(HAVE_PARALLEL_RSP)
             plugin_connect_rsp_api(RSP_PLUGIN_PARALLEL);
#elif defined(HAVE_LLE)
             plugin_connect_rsp_api(RSP_PLUGIN_CXD4);
#else
             log_cb(RETRO_LOG_INFO, "Requested Angrylion but no LLE RSP available, falling back to GLideN64!\n");
             plugin_connect_rdp_api(RDP_PLUGIN_GLIDEN64);
             plugin_connect_rsp_api(RSP_PLUGIN_HLE);
#endif 
          }
       }

       var.key = CORE_NAME "-BilinearMode";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          bilinearMode = !strcmp(var.value, "3point") ? 0 : 1;
       }

       var.key = CORE_NAME "-FXAA";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableFXAA = atoi(var.value);
       }

       var.key = CORE_NAME "-MultiSampling";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          MultiSampling = atoi(var.value);
       }

       var.key = CORE_NAME "-FrameDuping";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableFrameDuping = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-Framerate";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableFullspeed = !strcmp(var.value, "Original") ? 0 : 1;
       }

       var.key = CORE_NAME "-virefresh";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          CountPerScanlineOverride = !strcmp(var.value, "Auto") ? 0 : atoi(var.value);
       }

       var.key = CORE_NAME "-NoiseEmulation";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableNoiseEmulation = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableLODEmulation";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableLODEmulation = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableFBEmulation";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableFBEmulation = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableN64DepthCompare";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableN64DepthCompare = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableCopyColorToRDRAM";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "Async"))
             EnableCopyColorToRDRAM = 2;
          else if (!strcmp(var.value, "Sync"))
             EnableCopyColorToRDRAM = 1;
          else
             EnableCopyColorToRDRAM = 0;
       }

       var.key = CORE_NAME "-EnableCopyDepthToRDRAM";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "Software"))
             EnableCopyDepthToRDRAM = 2;
          else if (!strcmp(var.value, "FromMem"))
             EnableCopyDepthToRDRAM = 1;
          else
             EnableCopyDepthToRDRAM = 0;
       }

       var.key = CORE_NAME "-EnableHWLighting";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableHWLighting = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-CorrectTexrectCoords";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "Force"))
             CorrectTexrectCoords = 2;
          else if (!strcmp(var.value, "Auto"))
             CorrectTexrectCoords = 1;
          else
             CorrectTexrectCoords = 0;
       }

       var.key = CORE_NAME "-BackgroundMode";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          BackgroundMode = !strcmp(var.value, "OnePiece") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableNativeResTexrects";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if(!strcmp(var.value, "False") || !strcmp(var.value, "Disabled"))
          {
             enableNativeResTexrects = 0; // NativeResTexrectsMode::ntDisable
          }
          else if(!strcmp(var.value, "Optimized"))
          {
             enableNativeResTexrects = 1; // NativeResTexrectsMode::ntOptimized
          }
          else if(!strcmp(var.value, "Unoptimized"))
          {
             enableNativeResTexrects = 2; // NativeResTexrectsMode::ntUnptimized (Note: upstream typo)
          }
       }

       var.key = CORE_NAME "-txFilterMode";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "Smooth filtering 1"))
             txFilterMode = 1;
          else if (!strcmp(var.value, "Smooth filtering 2"))
             txFilterMode = 2;
          else if (!strcmp(var.value, "Smooth filtering 3"))
             txFilterMode = 3;
          else if (!strcmp(var.value, "Smooth filtering 4"))
             txFilterMode = 4;
          else if (!strcmp(var.value, "Sharp filtering 1"))
             txFilterMode = 5;
          else if (!strcmp(var.value, "Sharp filtering 2"))
             txFilterMode = 6;
          else
             txFilterMode = 0;
       }

       var.key = CORE_NAME "-txEnhancementMode";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "As Is"))
             txEnhancementMode = 1;
          else if (!strcmp(var.value, "X2"))
             txEnhancementMode = 2;
          else if (!strcmp(var.value, "X2SAI"))
             txEnhancementMode = 3;
          else if (!strcmp(var.value, "HQ2X"))
             txEnhancementMode = 4;
          else if (!strcmp(var.value, "HQ2XS"))
             txEnhancementMode = 5;
          else if (!strcmp(var.value, "LQ2X"))
             txEnhancementMode = 6;
          else if (!strcmp(var.value, "LQ2XS"))
             txEnhancementMode = 7;
          else if (!strcmp(var.value, "HQ4X"))
             txEnhancementMode = 8;
          else if (!strcmp(var.value, "2xBRZ"))
             txEnhancementMode = 9;
          else if (!strcmp(var.value, "3xBRZ"))
             txEnhancementMode = 10;
          else if (!strcmp(var.value, "4xBRZ"))
             txEnhancementMode = 11;
          else if (!strcmp(var.value, "5xBRZ"))
             txEnhancementMode = 12;
          else if (!strcmp(var.value, "6xBRZ"))
             txEnhancementMode = 13;
          else
             txEnhancementMode = 0;
       }

       var.key = CORE_NAME "-txFilterIgnoreBG";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          // "Filter background textures; True|False" (true=filter, false=ignore)
          txFilterIgnoreBG = !strcmp(var.value, "False") ? 1 : 0;
       }

       var.key = CORE_NAME "-txHiresEnable";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          txHiresEnable = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-txCacheCompression";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableTxCacheCompression = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-txHiresFullAlphaChannel";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          txHiresFullAlphaChannel = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-MaxTxCacheSize";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          MaxTxCacheSize = atoi(var.value);
       }

       var.key = CORE_NAME "-EnableLegacyBlending";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          enableLegacyBlending = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableFragmentDepthWrite";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableFragmentDepthWrite = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableShadersStorage";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableShadersStorage = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableTextureCache";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableTextureCache = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableEnhancedTextureStorage";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableEnhancedTextureStorage = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-EnableEnhancedHighResStorage";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableEnhancedHighResStorage = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-cpucore";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "pure_interpreter"))
             r4300_emumode = EMUMODE_PURE_INTERPRETER;
          else if (!strcmp(var.value, "cached_interpreter"))
             r4300_emumode = EMUMODE_INTERPRETER;
          else if (!strcmp(var.value, "dynamic_recompiler"))
             r4300_emumode = EMUMODE_DYNAREC;
       }

       var.key = CORE_NAME "-aspect";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "16:9 adjusted")) {
             AspectRatio = 3;
             retro_screen_aspect = 16.0 / 9.0;
          } else if (!strcmp(var.value, "16:9")) {
             AspectRatio = 2;
             retro_screen_aspect = 16.0 / 9.0;
          } else {
             AspectRatio = 1;
             retro_screen_aspect = 4.0 / 3.0;
          }
       }

       var.key = (AspectRatio == 1 ? CORE_NAME "-43screensize" : CORE_NAME "-169screensize");
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          sscanf(var.value, "%dx%d", &retro_screen_width, &retro_screen_height);

          // Sanity check... not optimal since we will render at a higher res, but otherwise
          // GLideN64 might blit a bigger image onto a smaller framebuffer
          // This is a recent regression.
          if((retro_screen_width == 320 && retro_screen_height == 240) ||
             (retro_screen_width == 640 && retro_screen_height == 360))
          {
            EnableNativeResFactor = 1; // Force factor == 1
          }
       }

       // If we use Angrylion, we force 640x480
       // TODO: ?
#ifdef HAVE_THR_AL
       if(current_rdp_type == RDP_PLUGIN_ANGRYLION)
       {
          retro_screen_width = 640;
          retro_screen_height = 480;
          retro_screen_aspect = 4.0 / 3.0;
          AspectRatio = 1;
       }
#endif // HAVE_THR_AL

       var.key = CORE_NAME "-astick-deadzone";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
          astick_deadzone = (int)(atoi(var.value) * 0.01f * 0x8000);

       var.key = CORE_NAME "-astick-sensitivity";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
          astick_sensitivity = atoi(var.value);

       var.key = CORE_NAME "-CountPerOp";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          CountPerOp = atoi(var.value);
       }
       
       if(EnableFullspeed)
       {
          CountPerOp = 1; // Force CountPerOp == 1
          if(current_rdp_type == RDP_PLUGIN_GLIDEN64 && !EnableFBEmulation)
             EnableFrameDuping = 1;
       }

#ifdef HAVE_THR_AL
       if(current_rdp_type == RDP_PLUGIN_ANGRYLION)
       {
           // We always want frame duping here, the result will be different from GLideN64
           // This is always prefered here!
           EnableFrameDuping = 1;
       }
#endif // HAVE_THR_AL

       var.key = CORE_NAME "-r-cbutton";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "C1"))
             r_cbutton = RETRO_DEVICE_ID_JOYPAD_A;
          else if (!strcmp(var.value, "C2"))
             r_cbutton = RETRO_DEVICE_ID_JOYPAD_Y;
          else if (!strcmp(var.value, "C3"))
             r_cbutton = RETRO_DEVICE_ID_JOYPAD_B;
          else if (!strcmp(var.value, "C4"))
             r_cbutton = RETRO_DEVICE_ID_JOYPAD_X;
       }

       var.key = CORE_NAME "-l-cbutton";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "C1"))
             l_cbutton = RETRO_DEVICE_ID_JOYPAD_A;
          else if (!strcmp(var.value, "C2"))
             l_cbutton = RETRO_DEVICE_ID_JOYPAD_Y;
          else if (!strcmp(var.value, "C3"))
             l_cbutton = RETRO_DEVICE_ID_JOYPAD_B;
          else if (!strcmp(var.value, "C4"))
             l_cbutton = RETRO_DEVICE_ID_JOYPAD_X;
       }

       var.key = CORE_NAME "-d-cbutton";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "C1"))
             d_cbutton = RETRO_DEVICE_ID_JOYPAD_A;
          else if (!strcmp(var.value, "C2"))
             d_cbutton = RETRO_DEVICE_ID_JOYPAD_Y;
          else if (!strcmp(var.value, "C3"))
             d_cbutton = RETRO_DEVICE_ID_JOYPAD_B;
          else if (!strcmp(var.value, "C4"))
             d_cbutton = RETRO_DEVICE_ID_JOYPAD_X;
       }

       var.key = CORE_NAME "-u-cbutton";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          if (!strcmp(var.value, "C1"))
             u_cbutton = RETRO_DEVICE_ID_JOYPAD_A;
          else if (!strcmp(var.value, "C2"))
             u_cbutton = RETRO_DEVICE_ID_JOYPAD_Y;
          else if (!strcmp(var.value, "C3"))
             u_cbutton = RETRO_DEVICE_ID_JOYPAD_B;
          else if (!strcmp(var.value, "C4"))
             u_cbutton = RETRO_DEVICE_ID_JOYPAD_X;
       }

       var.key = CORE_NAME "-EnableOverscan";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          EnableOverscan = !strcmp(var.value, "Enabled") ? 1 : 0;
       }

       var.key = CORE_NAME "-OverscanTop";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          OverscanTop = atoi(var.value);
       }

       var.key = CORE_NAME "-OverscanLeft";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          OverscanLeft = atoi(var.value);
       }

       var.key = CORE_NAME "-OverscanRight";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          OverscanRight = atoi(var.value);
       }

       var.key = CORE_NAME "-OverscanBottom";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          OverscanBottom = atoi(var.value);
       }

       var.key = CORE_NAME "-alt-map";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          alternate_mapping = !strcmp(var.value, "False") ? 0 : 1;
       }

       var.key = CORE_NAME "-ForceDisableExtraMem";
       var.value = NULL;
       if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
       {
          ForceDisableExtraMem = !strcmp(var.value, "False") ? 0 : 1;
       }
    }

#ifdef HAVE_THR_AL
    if (current_rdp_type == RDP_PLUGIN_ANGRYLION)
    {
        var.key = CORE_NAME "-angrylion-vioverlay";
        var.value = NULL;

        environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var);

        if (var.value)
        {
           if (!strcmp(var.value, "Filtered"))
           {
              angrylion_set_vi(0);
              angrylion_set_vi_dedither(1);
              angrylion_set_vi_blur(1);
           }
           else if (!strcmp(var.value, "AA+Blur"))
           {
              angrylion_set_vi(0);
              angrylion_set_vi_dedither(0);
              angrylion_set_vi_blur(1);
           }
           else if (!strcmp(var.value, "AA+Dedither"))
           {
              angrylion_set_vi(0);
              angrylion_set_vi_dedither(1);
              angrylion_set_vi_blur(0);
           }
           else if (!strcmp(var.value, "AA only"))
           {
              angrylion_set_vi(0);
              angrylion_set_vi_dedither(0);
              angrylion_set_vi_blur(0);
           }
           else if (!strcmp(var.value, "Unfiltered"))
           {
              angrylion_set_vi(1);
              angrylion_set_vi_dedither(1);
              angrylion_set_vi_blur(1);
           }
           else if (!strcmp(var.value, "Depth"))
           {
              angrylion_set_vi(2);
              angrylion_set_vi_dedither(1);
              angrylion_set_vi_blur(1);
           }
           else if (!strcmp(var.value, "Coverage"))
           {
              angrylion_set_vi(3);
              angrylion_set_vi_dedither(1);
              angrylion_set_vi_blur(1);
           }
        }
        else
        {
           angrylion_set_vi(0);
           angrylion_set_vi_dedither(1);
           angrylion_set_vi_blur(1);
        }

        var.key = CORE_NAME "-angrylion-sync";
        var.value = NULL;

        environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var);

        if (var.value)
        {
           if (!strcmp(var.value, "High"))
              angrylion_set_synclevel(2);
           else if (!strcmp(var.value, "Medium"))
              angrylion_set_synclevel(1);
           else if (!strcmp(var.value, "Low"))
              angrylion_set_synclevel(0);
        }
        else
           angrylion_set_synclevel(0);

        var.key = CORE_NAME "-angrylion-multithread";
        var.value = NULL;

        if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
        {
           if (!strcmp(var.value, "all threads"))
              angrylion_set_threads(0);
           else
              angrylion_set_threads(atoi(var.value));
        }
        else
           angrylion_set_threads(0);

        var.key = CORE_NAME "-angrylion-overscan";
        var.value = NULL;

        environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var);

        if (var.value)
        {
           if (!strcmp(var.value, "enabled"))
              angrylion_set_overscan(1);
           else if (!strcmp(var.value, "disabled"))
              angrylion_set_overscan(0);
        }
        else
        {
           angrylion_set_overscan(0);
        }
    }
#endif // HAVE_THR_AL

    update_controllers();
}

static void format_saved_memory(void)
{
    format_sram(saved_memory.sram);
    format_eeprom(saved_memory.eeprom, EEPROM_MAX_SIZE);
    format_flashram(saved_memory.flashram);
    format_mempak(saved_memory.mempack + 0 * MEMPAK_SIZE);
    format_mempak(saved_memory.mempack + 1 * MEMPAK_SIZE);
    format_mempak(saved_memory.mempack + 2 * MEMPAK_SIZE);
    format_mempak(saved_memory.mempack + 3 * MEMPAK_SIZE);
}

static void context_reset(void)
{
    static bool first_init = true;

    if(current_rdp_type == RDP_PLUGIN_GLIDEN64)
    {
       log_cb(RETRO_LOG_DEBUG, CORE_NAME ": context_reset()\n");
       glsm_ctl(GLSM_CTL_STATE_CONTEXT_RESET, NULL);
       if (first_init)
       {
          glsm_ctl(GLSM_CTL_STATE_SETUP, NULL);
          first_init = false;
       }
    }

    reinit_gfx_plugin();
}

static void context_destroy(void)
{
    if(current_rdp_type == RDP_PLUGIN_GLIDEN64)
    {
       glsm_ctl(GLSM_CTL_STATE_CONTEXT_DESTROY, NULL);
    }
}

static bool context_framebuffer_lock(void *data)
{
    //if (!stop)
     //   return false;
    return true;
}

bool retro_load_game(const struct retro_game_info *game)
{
    char* gamePath;
    char* newPath;

    // Workaround for broken subsystem on static platforms
    if(!retro_dd_path_img)
    {
        gamePath = (char*)game->path;
        newPath = (char*)calloc(1, strlen(gamePath)+5);
        strcpy(newPath, gamePath);
        strcat(newPath, ".ndd");
        FILE* fileTest = fopen(newPath, "r");
        if(!fileTest)
        {
            free(newPath);
        } else {
            fclose(fileTest);
            // Free'd later in Mupen Core
            retro_dd_path_img = newPath;
        }
    }

    glsm_ctx_params_t params = {0};
    format_saved_memory();

    update_variables(true);
    initial_boot = false;

    init_audio_libretro(audio_buffer_size);

    params.context_reset         = context_reset;
    params.context_destroy       = context_destroy;
    params.environ_cb            = environ_cb;
    params.stencil               = false;

    params.framebuffer_lock      = context_framebuffer_lock;
    if (current_rdp_type == RDP_PLUGIN_GLIDEN64 && !glsm_ctl(GLSM_CTL_STATE_CONTEXT_INIT, &params))
    {
        if (log_cb)
            log_cb(RETRO_LOG_ERROR, CORE_NAME ": libretro frontend doesn't have OpenGL support\n");
        return false;
    }

    game_data = malloc(game->size);
    memcpy(game_data, game->data, game->size);
    game_size = game->size;

    if (!emu_step_load_data())
        return false;

    if(current_rdp_type == RDP_PLUGIN_GLIDEN64)
    {
       first_context_reset = true;
    }
    else
    {
       // Prevents later emu_step_initialize call
       first_context_reset = false;
       emu_step_initialize();
       /* Additional check for vioverlay not set at start */
       update_variables(false);
    }

    return true;
}

void retro_unload_game(void)
{
    CoreDoCommand(M64CMD_ROM_CLOSE, 0, NULL);
    emu_initialized = false;
}

void retro_run (void)
{
    libretro_swap_buffer = false;
    static bool updated = false;
    
    if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated) {
       update_variables(false);
       update_controllers();
    }

    if(current_rdp_type == RDP_PLUGIN_GLIDEN64)
    {
       glsm_ctl(GLSM_CTL_STATE_BIND, NULL);
    }

    co_switch(game_thread);

    if(current_rdp_type == RDP_PLUGIN_GLIDEN64)
    {
       glsm_ctl(GLSM_CTL_STATE_UNBIND, NULL);
    }
    
    if (libretro_swap_buffer)
    {
       if(current_rdp_type == RDP_PLUGIN_GLIDEN64)
       {
          video_cb(RETRO_HW_FRAME_BUFFER_VALID, retro_screen_width, retro_screen_height, 0);
       }
#ifdef HAVE_THR_AL
       else if(current_rdp_type == RDP_PLUGIN_ANGRYLION)
       {
          video_cb(prescale, retro_screen_width, retro_screen_height, screen_pitch);
       }
#endif // HAVE_THR_AL
    }
    else if(EnableFrameDuping)
    {
        // screen_pitch will be 0 for GLN
        video_cb(NULL, retro_screen_width, retro_screen_height, screen_pitch);
    }
        
}

void retro_reset (void)
{
    CoreDoCommand(M64CMD_RESET, 0, (void*)0);
}

void *retro_get_memory_data(unsigned type)
{
    switch (type)
    {
        case RETRO_MEMORY_SYSTEM_RAM: return g_dev.rdram.dram;
        case RETRO_MEMORY_DD:
        case RETRO_MEMORY_SAVE_RAM:   return &saved_memory;
    }
    return NULL;
}

size_t retro_get_memory_size(unsigned type)
{
    switch (type)
    {
        case RETRO_MEMORY_SYSTEM_RAM: return RDRAM_MAX_SIZE;
        case RETRO_MEMORY_DD:
        case RETRO_MEMORY_SAVE_RAM:   return sizeof(saved_memory);
    }
    return 0;
}

size_t retro_serialize_size (void)
{
    return 16788288 + 1024 + 4 + 4096;
}

bool retro_serialize(void *data, size_t size)
{
    if (initializing)
        return false;

    int success = savestates_save_m64p(&g_dev, data);
    if (success)
        return true;

    return false;
}

bool retro_unserialize(const void * data, size_t size)
{
    if (initializing)
        return false;

    int success = savestates_load_m64p(&g_dev, data);
    if (success)
        return true;

    return false;
}

//Needed to be able to detach controllers for Lylat Wars multiplayer
//Only sets if controller struct is initialised as addon paks do.
void retro_set_controller_port_device(unsigned in_port, unsigned device) {
    if (in_port < 4){
        switch(device)
        {
            case RETRO_DEVICE_NONE:
               if (controller[in_port].control){
                   controller[in_port].control->Present = 0;
                   break;
               } else {
                   pad_present[in_port] = 0;
                   break;
               }

            case RETRO_DEVICE_JOYPAD:
            default:
               if (controller[in_port].control){
                   controller[in_port].control->Present = 1;
                   break;
               } else {
                   pad_present[in_port] = 1;
                   break;
               }
        }
    }
}

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

void retro_cheat_reset(void)
{
    cheat_delete_all(&g_cheat_ctx);
}

void retro_cheat_set(unsigned index, bool enabled, const char* codeLine)
{
    char name[256];
    m64p_cheat_code mupenCode[256];
    int matchLength=0,partCount=0;
    uint32_t codeParts[256];
    int cursor;

    //Generate a name
    sprintf(name, "cheat_%u",index);

    //Break the code into Parts
    for (cursor=0;;cursor++)
    {
        if (ISHEXDEC){
            matchLength++;
        } else {
            if (matchLength){
               char codePartS[matchLength];
               strncpy(codePartS,codeLine+cursor-matchLength,matchLength);
               codePartS[matchLength]=0;
               codeParts[partCount++]=strtoul(codePartS,NULL,16);
               matchLength=0;
            }
        }
        if (!codeLine[cursor]){
            break;
        }
    }

    //Assign the parts to mupenCode
    for (cursor=0;2*cursor+1<partCount;cursor++){
        mupenCode[cursor].address=codeParts[2*cursor];
        mupenCode[cursor].value=codeParts[2*cursor+1];
    }

    //Assign to mupenCode
    cheat_add_new(&g_cheat_ctx, name, mupenCode, partCount / 2);
    cheat_set_enabled(&g_cheat_ctx, name, enabled);
}

void retro_return(void)
{
    co_switch(retro_thread);
}

uint32_t get_retro_screen_width()
{
    return retro_screen_width;
}

uint32_t get_retro_screen_height()
{
    return retro_screen_height;
}

static int GamesharkActive = 0;

int event_gameshark_active(void)
{
    return GamesharkActive;
}

void event_set_gameshark(int active)
{
    // if boolean value doesn't change then just return
    if (!active == !GamesharkActive)
        return;

    // set the button state
    GamesharkActive = (active ? 1 : 0);

    // notify front-end application that gameshark button state has changed
    StateChanged(M64CORE_INPUT_GAMESHARK, GamesharkActive);
}
