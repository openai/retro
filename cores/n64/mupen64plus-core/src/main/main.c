/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - main.c                                                  *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2012 CasualJames                                        *
 *   Copyright (C) 2008-2009 Richard Goedeken                              *
 *   Copyright (C) 2008 Ebenblues Nmn Okaygo Tillin9                       *
 *   Copyright (C) 2002 Hacktarux                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* This is MUPEN64's main entry point. It contains code that is common
 * to both the gui and non-gui versions of mupen64. See
 * gui subdirectories for the gui-specific code.
 * if you want to implement an interface, you should look here
 */

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M64P_CORE_PROTOTYPES 1
#include "api/callbacks.h"
#include "api/config.h"
#include "api/debugger.h"
#include "api/m64p_config.h"
#include "api/m64p_types.h"
#include "api/m64p_vidext.h"
#include "api/vidext.h"
#include "backends/api/audio_out_backend.h"
#include "backends/api/clock_backend.h"
#include "backends/api/controller_input_backend.h"
#include "backends/api/joybus.h"
#include "backends/api/rumble_backend.h"
#include "backends/api/storage_backend.h"
#include "backends/api/video_capture_backend.h"
#include "backends/plugins_compat/plugins_compat.h"
#include "backends/clock_ctime_plus_delta.h"
#include "backends/file_storage.h"
#include "cheat.h"
#include "device/device.h"
#include "device/controllers/paks/biopak.h"
#include "device/controllers/paks/mempak.h"
#include "device/controllers/paks/rumblepak.h"
#include "device/controllers/paks/transferpak.h"
#include "device/gb/gb_cart.h"
#include "device/pif/bootrom_hle.h"
#include "eventloop.h"
#include "main.h"
#include "callbacks.h"
#include "plugin/plugin.h"
#if defined(PROFILE)
#include "profile.h"
#endif
#include "rom.h"
#include "savestates.h"
#include "screenshot.h"
#include "util.h"

#include <libretro_private.h>
#include <libco.h>

#ifdef HAVE_LIBNX
#include <sys/stat.h>
#endif

#ifdef __LIBRETRO__
#include <file/file_path.h>
#include "../../../libretro/libretro_memory.h"
#include "../../../custom/GLideN64/GLideN64_libretro.h"
extern retro_environment_t environ_cb;
extern char* retro_dd_path_img;
extern char* retro_dd_path_rom;
#endif // __LIBRETRO__

#ifdef DBG
#include "debugger/dbg_debugger.h"
#endif

#ifdef WITH_LIRC
#include "lirc.h"
#endif //WITH_LIRC

/* version number for Core config section */
#define CONFIG_PARAM_VERSION 1.01

/** globals **/
m64p_handle g_CoreConfig = NULL;

m64p_frame_callback g_FrameCallback = NULL;

int         g_RomWordsLittleEndian = 0; // after loading, ROM words are in native N64 byte order (big endian). We will swap them on x86
int         g_EmulatorRunning = 0;      // need separate boolean to tell if emulator is running, since --nogui doesn't use a thread


int g_rom_pause;

struct cheat_ctx g_cheat_ctx;

/* g_mem_base is global to allow plugins early access (before device is initialized).
 * Do not use this variable directly in emulation code.
 * Initialization and DeInitialization of this variable is done at CoreStartup and CoreShutdown.
 */
void* g_mem_base = NULL;

struct device g_dev;

m64p_media_loader g_media_loader;

int g_gs_vi_counter = 0;

/** static (local) variables **/
static int   l_CurrentFrame = 0;         // frame counter
static int   l_TakeScreenshot = 0;       // Tell OSD Rendering callback to take a screenshot just before drawing the OSD
static int   l_SpeedFactor = 100;        // percentage of nominal game speed at which emulator is running
static int   l_FrameAdvance = 0;         // variable to check if we pause on next frame
static int   l_MainSpeedLimit = 1;       // insert delay during vi_interrupt to keep speed at real-time

/* compatible paks */
enum { PAK_MAX_SIZE = 5 };
static size_t l_paks_idx[GAME_CONTROLLERS_COUNT];
static void* l_paks[GAME_CONTROLLERS_COUNT][PAK_MAX_SIZE];
static const struct pak_interface* l_ipaks[PAK_MAX_SIZE];
static size_t l_pak_type_idx[6];

/*********************************************************************************************************
* static functions
*/

static const char *get_savepathdefault(const char *configpath)
{
    return "";
}

static char *get_mempaks_path(void)
{
    return "";
}

static char *get_eeprom_path(void)
{
    return "";
}

static char *get_sram_path(void)
{
    return "";
}

static char *get_flashram_path(void)
{
    return "";
}

static char *get_gb_ram_path(const char* gbrom, unsigned int control_id)
{
    return "";
}

const char *get_savestatepath(void)
{
    return "";
}

const char *get_savesrampath(void)
{
    return "";
}

void main_message(m64p_msg_level level, unsigned int corner, const char *format, ...)
{
    va_list ap;
    char buffer[2049];
    va_start(ap, format);
    vsnprintf(buffer, 2047, format, ap);
    buffer[2048]='\0';
    va_end(ap);

    /* send message to front-end */
    DebugMessage(level, "%s", buffer);
}

extern retro_input_poll_t poll_cb;
static void main_check_inputs(void)
{
#ifdef WITH_LIRC
    lircCheckInput();
#endif
    poll_cb();
}

/*********************************************************************************************************
* global functions, for adjusting the core emulator behavior
*/
extern void Config_LoadConfig();
int main_set_core_defaults(void)
{
    Config_LoadConfig();
    return 1;
}

void main_speeddown(int percent)
{
    if (l_SpeedFactor - percent > 10)  /* 10% minimum speed */
    {
        l_SpeedFactor -= percent;
        audio.setSpeedFactor(l_SpeedFactor);
        StateChanged(M64CORE_SPEED_FACTOR, l_SpeedFactor);
    }
}

void main_speedup(int percent)
{
    if (l_SpeedFactor + percent < 300) /* 300% maximum speed */
    {
        l_SpeedFactor += percent;
        audio.setSpeedFactor(l_SpeedFactor);
        StateChanged(M64CORE_SPEED_FACTOR, l_SpeedFactor);
    }
}

static void main_speedset(int percent)
{
    if (percent < 1 || percent > 1000)
    {
        DebugMessage(M64MSG_WARNING, "Invalid speed setting %i percent", percent);
        return;
    }
    // disable fast-forward if it's enabled
    main_set_fastforward(0);
    // set speed
    l_SpeedFactor = percent;
    audio.setSpeedFactor(l_SpeedFactor);
    StateChanged(M64CORE_SPEED_FACTOR, l_SpeedFactor);
}

void main_set_fastforward(int enable)
{
    static int ff_state = 0;
    static int SavedSpeedFactor = 100;

    if (enable && !ff_state)
    {
        ff_state = 1; /* activate fast-forward */
        SavedSpeedFactor = l_SpeedFactor;
        l_SpeedFactor = 250;
        audio.setSpeedFactor(l_SpeedFactor);
        StateChanged(M64CORE_SPEED_FACTOR, l_SpeedFactor);
    }
    else if (!enable && ff_state)
    {
        ff_state = 0; /* de-activate fast-forward */
        l_SpeedFactor = SavedSpeedFactor;
        audio.setSpeedFactor(l_SpeedFactor);
        StateChanged(M64CORE_SPEED_FACTOR, l_SpeedFactor);
    }

}

static void main_set_speedlimiter(int enable)
{
    l_MainSpeedLimit = enable ? 1 : 0;
}

static int main_is_paused(void)
{
    return (g_EmulatorRunning && g_rom_pause);
}

void main_toggle_pause(void)
{
    if (!g_EmulatorRunning)
        return;

    if (g_rom_pause)
    {
        DebugMessage(M64MSG_STATUS, "Emulation continued.");
        StateChanged(M64CORE_EMU_STATE, M64EMU_RUNNING);
    }
    else
    {
        DebugMessage(M64MSG_STATUS, "Emulation paused.");
        StateChanged(M64CORE_EMU_STATE, M64EMU_PAUSED);
    }

    g_rom_pause = !g_rom_pause;
    l_FrameAdvance = 0;
}

void main_advance_one(void)
{
    l_FrameAdvance = 1;
    g_rom_pause = 0;
    StateChanged(M64CORE_EMU_STATE, M64EMU_RUNNING);
}

static void main_draw_volume_osd(void)
{
    return;
}

/* this function could be called as a result of a keypress, joystick/button movement,
   LIRC command, or 'testshots' command-line option timer */
void main_take_next_screenshot(void)
{
    l_TakeScreenshot = l_CurrentFrame + 1;
}

void main_state_set_slot(int slot)
{
    if (slot < 0 || slot > 9)
    {
        DebugMessage(M64MSG_WARNING, "Invalid savestate slot '%i' in main_state_set_slot().  Using 0", slot);
        slot = 0;
    }

    savestates_select_slot(slot);
}

void main_state_inc_slot(void)
{
    savestates_inc_slot();
}

void main_state_load(const char *filename)
{
    if (filename == NULL) // Save to slot
        savestates_set_job(savestates_job_load, savestates_type_m64p, NULL);
    else
        savestates_set_job(savestates_job_load, savestates_type_unknown, filename);
}

void main_state_save(int format, const char *filename)
{
    if (filename == NULL) // Save to slot
        savestates_set_job(savestates_job_save, savestates_type_m64p, NULL);
    else // Save to file
        savestates_set_job(savestates_job_save, (savestates_type)format, filename);
}

m64p_error main_core_state_query(m64p_core_param param, int *rval)
{
    switch (param)
    {
        case M64CORE_EMU_STATE:
            if (!g_EmulatorRunning)
                *rval = M64EMU_STOPPED;
            else if (g_rom_pause)
                *rval = M64EMU_PAUSED;
            else
                *rval = M64EMU_RUNNING;
            break;
        case M64CORE_VIDEO_MODE:
                *rval = M64VIDEO_FULLSCREEN;
            break;
        case M64CORE_SAVESTATE_SLOT:
            *rval = savestates_get_slot();
            break;
        case M64CORE_SPEED_FACTOR:
            *rval = l_SpeedFactor;
            break;
        case M64CORE_SPEED_LIMITER:
            *rval = l_MainSpeedLimit;
            break;
        case M64CORE_VIDEO_SIZE:
        {
            int width, height;
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;
            main_get_screen_size(&width, &height);
            *rval = (width << 16) + height;
            break;
        }
        case M64CORE_AUDIO_VOLUME:
        {
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;    
            return main_volume_get_level(rval);
        }
        case M64CORE_AUDIO_MUTE:
            *rval = main_volume_get_muted();
            break;
        case M64CORE_INPUT_GAMESHARK:
            *rval = event_gameshark_active();
            break;
        // these are only used for callbacks; they cannot be queried or set
        case M64CORE_STATE_LOADCOMPLETE:
        case M64CORE_STATE_SAVECOMPLETE:
            return M64ERR_INPUT_INVALID;
        default:
            return M64ERR_INPUT_INVALID;
    }

    return M64ERR_SUCCESS;
}

m64p_error main_core_state_set(m64p_core_param param, int val)
{
    switch (param)
    {
        case M64CORE_EMU_STATE:
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;
            if (val == M64EMU_STOPPED)
            {        
                /* this stop function is asynchronous.  The emulator may not terminate until later */
                main_stop();
                return M64ERR_SUCCESS;
            }
            else if (val == M64EMU_RUNNING)
            {
                if (main_is_paused())
                    main_toggle_pause();
                return M64ERR_SUCCESS;
            }
            else if (val == M64EMU_PAUSED)
            {    
                if (!main_is_paused())
                    main_toggle_pause();
                return M64ERR_SUCCESS;
            }
            return M64ERR_INPUT_INVALID;
        case M64CORE_VIDEO_MODE:
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;
            gfx.changeWindow();
            return M64ERR_SUCCESS;
        case M64CORE_SAVESTATE_SLOT:
            if (val < 0 || val > 9)
                return M64ERR_INPUT_INVALID;
            savestates_select_slot(val);
            return M64ERR_SUCCESS;
        case M64CORE_SPEED_FACTOR:
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;
            main_speedset(val);
            return M64ERR_SUCCESS;
        case M64CORE_SPEED_LIMITER:
            main_set_speedlimiter(val);
            return M64ERR_SUCCESS;
        case M64CORE_VIDEO_SIZE:
        {
            // the front-end app is telling us that the user has resized the video output frame, and so
            // we should try to update the video plugin accordingly.  First, check state
            int width, height;
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;
            width = (val >> 16) & 0xffff;
            height = val & 0xffff;
            // then call the video plugin.  if the video plugin supports resizing, it will resize its viewport and call
            // VidExt_ResizeWindow to update the window manager handling our opengl output window
            gfx.resizeVideoOutput(width, height);
            return M64ERR_SUCCESS;
        }
        case M64CORE_AUDIO_VOLUME:
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;
            if (val < 0 || val > 100)
                return M64ERR_INPUT_INVALID;
            return main_volume_set_level(val);
        case M64CORE_AUDIO_MUTE:
            if ((main_volume_get_muted() && !val) || (!main_volume_get_muted() && val))
                return main_volume_mute();
            return M64ERR_SUCCESS;
        case M64CORE_INPUT_GAMESHARK:
            if (!g_EmulatorRunning)
                return M64ERR_INVALID_STATE;
            event_set_gameshark(val);
            return M64ERR_SUCCESS;
        // these are only used for callbacks; they cannot be queried or set
        case M64CORE_STATE_LOADCOMPLETE:
        case M64CORE_STATE_SAVECOMPLETE:
            return M64ERR_INPUT_INVALID;
        default:
            return M64ERR_INPUT_INVALID;
    }
}

m64p_error main_get_screen_size(int *width, int *height)
{
    gfx.readScreen(NULL, width, height, 0);
    return M64ERR_SUCCESS;
}

m64p_error main_read_screen(void *pixels, int bFront)
{
    int width_trash, height_trash;
    gfx.readScreen(pixels, &width_trash, &height_trash, bFront);
    return M64ERR_SUCCESS;
}

m64p_error main_volume_up(void)
{
    int level = 0;
    audio.volumeUp();
    main_volume_get_level(&level);
    StateChanged(M64CORE_AUDIO_VOLUME, level);
    return M64ERR_SUCCESS;
}

m64p_error main_volume_down(void)
{
    int level = 0;
    audio.volumeDown();
    main_volume_get_level(&level);
    StateChanged(M64CORE_AUDIO_VOLUME, level);
    return M64ERR_SUCCESS;
}

m64p_error main_volume_get_level(int *level)
{
    *level = audio.volumeGetLevel();
    return M64ERR_SUCCESS;
}

m64p_error main_volume_set_level(int level)
{
    audio.volumeSetLevel(level);
    level = audio.volumeGetLevel();
    StateChanged(M64CORE_AUDIO_VOLUME, level);
    return M64ERR_SUCCESS;
}

m64p_error main_volume_mute(void)
{
    audio.volumeMute();
    StateChanged(M64CORE_AUDIO_MUTE, main_volume_get_muted());
    return M64ERR_SUCCESS;
}

int main_volume_get_muted(void)
{
    return (audio.volumeGetLevel() == 0);
}

m64p_error main_reset(int do_hard_reset)
{
    if (do_hard_reset) {
        hard_reset_device(&g_dev);
    }
    else {
        soft_reset_device(&g_dev);
    }

    return M64ERR_SUCCESS;
}

/*********************************************************************************************************
* global functions, callbacks from the r4300 core or from other plugins
*/

static void video_plugin_render_callback(int bScreenRedrawn)
{
    // if the input plugin specified a render callback, call it now
    if(input.renderCallback)
    {
        input.renderCallback();
    }
}

void new_frame(void)
{
    if (g_FrameCallback != NULL)
        (*g_FrameCallback)(l_CurrentFrame);

    /* advance the current frame */
    l_CurrentFrame++;

    if (l_FrameAdvance) {
        g_rom_pause = 1;
        l_FrameAdvance = 0;
        StateChanged(M64CORE_EMU_STATE, M64EMU_PAUSED);
    }
}

#define SAMPLE_COUNT 3
static void apply_speed_limiter(void)
{
    return;
}

/* TODO: make a GameShark module and move that there */
static void gs_apply_cheats(struct cheat_ctx* ctx)
{
    struct r4300_core* r4300 = &g_dev.r4300;

    if (g_gs_vi_counter < 60)
    {
        if (g_gs_vi_counter == 0)
            cheat_apply_cheats(ctx, r4300, ENTRY_BOOT);
        g_gs_vi_counter++;
    }
    else
    {
        cheat_apply_cheats(ctx, r4300, ENTRY_VI);
    }
}

static void pause_loop(void)
{
    if(g_rom_pause)
    {
        while(g_rom_pause)
        {
            main_check_inputs();
        }
    }
}

/* called on vertical interrupt.
 * Allow the core to perform various things */
void new_vi(void)
{
#if defined(PROFILE)
    timed_sections_refresh();
#endif

    gs_apply_cheats(&g_cheat_ctx);

    main_check_inputs();

    retro_return();
}

static void main_switch_pak(int control_id)
{
    struct game_controller* cont = &g_dev.controllers[control_id];

    change_pak(cont, l_paks[control_id][l_paks_idx[control_id]], l_ipaks[l_paks_idx[control_id]]);

    if (cont->ipak != NULL) {
        DebugMessage(M64MSG_INFO, "Controller %u pak changed to %s", control_id, cont->ipak->name);
    }
    else {
        DebugMessage(M64MSG_INFO, "Removing pak from controller %u", control_id);
    }
}

void main_switch_next_pak(int control_id)
{
    if (l_ipaks[l_paks_idx[control_id]] == NULL ||
        ++l_paks_idx[control_id] >= PAK_MAX_SIZE) {
        l_paks_idx[control_id] = 0;
    }

    main_switch_pak(control_id);
}

void main_switch_plugin_pak(int control_id)
{
    //Don't switch to the selected pak if it's not available for the game
    if (l_ipaks[l_pak_type_idx[Controls[control_id].Plugin]] == NULL) {
        Controls[control_id].Plugin = PLUGIN_NONE;
    }

    l_paks_idx[control_id] = l_pak_type_idx[Controls[control_id].Plugin];

    main_switch_pak(control_id);
}

void save_storage_file_libretro(void* storage)
{
}

static void open_mpk_file(struct file_storage* storage)
{
    storage->data = saved_memory.mempack;
    storage->size = MEMPAK_SIZE * 4;
}

static void open_fla_file(struct file_storage* storage)
{
    storage->data = saved_memory.flashram;
    storage->size = FLASHRAM_SIZE;
}

static void open_sra_file(struct file_storage* storage)
{
    storage->data = saved_memory.sram;
    storage->size = SRAM_SIZE;
}

static void open_eep_file(struct file_storage* storage)
{
    storage->data = saved_memory.eeprom;
    storage->size = EEPROM_MAX_SIZE;
}

static void load_dd_rom(uint8_t* rom, size_t* rom_size)
{
    /* ask the core loader for DD disk filename */
    char* dd_ipl_rom_filename = (g_media_loader.get_dd_rom == NULL)
        ? NULL
        : g_media_loader.get_dd_rom(g_media_loader.cb_data);

    char* sys_pathname;
    environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &sys_pathname);
    char* pathname = (char*)malloc(2048);
    strncpy(pathname, sys_pathname, 2048 - 1);
    if (pathname[(strlen(pathname)-1)] != '/' && pathname[(strlen(pathname)-1)] != '\\')
        strcat(pathname, path_default_slash());
    strcat(pathname, "Mupen64plus");
    strcat(pathname, path_default_slash());
    strcat(pathname, "IPL.n64");

    if(retro_dd_path_img)
    {
        dd_ipl_rom_filename = pathname;
    }
    else
    {
        dd_ipl_rom_filename = NULL;
    }

    if ((dd_ipl_rom_filename == NULL) || (strlen(dd_ipl_rom_filename) == 0)) {
        goto no_dd;
    }

    struct file_storage dd_rom;
    memset(&dd_rom, 0, sizeof(dd_rom));

    if (open_rom_file_storage(&dd_rom, dd_ipl_rom_filename) != file_ok) {
        DebugMessage(M64MSG_ERROR, "Failed to load DD IPL ROM: %s. Disabling 64DD", dd_ipl_rom_filename);
        goto no_dd;
    }

    DebugMessage(M64MSG_INFO, "DD IPL ROM: %s", dd_ipl_rom_filename);

    /* load and swap DD IPL ROM */
    *rom_size = g_ifile_storage_ro.size(&dd_rom);
    memcpy(rom, g_ifile_storage_ro.data(&dd_rom), *rom_size);
    close_file_storage(&dd_rom);

    /* fetch 1st word to identify IPL ROM format */
    /* FIXME: use more robust ROM detection heuristic - do the same for regular ROMs */
    uint32_t pi_bsd_dom1_config = 0
        | ((uint32_t)rom[0] << 24)
        | ((uint32_t)rom[1] << 16)
        | ((uint32_t)rom[2] <<  8)
        | ((uint32_t)rom[3] <<  0);

    switch (pi_bsd_dom1_config)
    {
    case 0x80270740: /* Z64 - big endian */
        to_big_endian_buffer(rom, 4, *rom_size/4);
        break;

    case 0x40072780: /* N64 - little endian */
        to_little_endian_buffer(rom, 4, *rom_size/4);
        break;

    case 0x27804007: /* V64 - bi-endian */
        swap_buffer(rom, 2, *rom_size/2);
        break;

    default: /* unknown */
        DebugMessage(M64MSG_ERROR, "Invalid DD IPL ROM: Disabling 64DD.");
        *rom_size = 0;
        return;
    }

    return;

no_dd:
    free(dd_ipl_rom_filename);
    *rom_size = 0;
}

static void load_dd_disk(struct file_storage* dd_disk, const struct storage_backend_interface** dd_idisk)
{
    const char* format_desc;
    /* ask the core loader for DD disk filename */
    char* dd_disk_filename = (g_media_loader.get_dd_disk == NULL)
        ? retro_dd_path_img
        : g_media_loader.get_dd_disk(g_media_loader.cb_data);

    printf("Load DD disk %s\n", dd_disk_filename);
    fflush(stdout);

    /* handle the no disk case */
    if (dd_disk_filename == NULL || strlen(dd_disk_filename) == 0) {
        goto no_disk;
    }

    /* open file */
    if (open_rom_file_storage(dd_disk, dd_disk_filename) != file_ok) {
        DebugMessage(M64MSG_ERROR, "Failed to load DD Disk: %s.", dd_disk_filename);
        goto no_disk;
    }

    /* FIXME: handle byte swapping */


    switch (dd_disk->size)
    {
    case MAME_FORMAT_DUMP_SIZE:
        /* already in a compatible format */
        *dd_idisk = &g_ifile_storage;
        format_desc = "MAME";
        break;

    case SDK_FORMAT_DUMP_SIZE: {
        /* convert to mame format */
        uint8_t* buffer = malloc(MAME_FORMAT_DUMP_SIZE);
        if (buffer == NULL) {
            DebugMessage(M64MSG_ERROR, "Failed to allocate memory for MAME disk dump");
            close_file_storage(dd_disk);
            goto no_disk;
        }

        dd_convert_to_mame(buffer, dd_disk->data);
        free(dd_disk->data);
        dd_disk->data = buffer;
        dd_disk->size = MAME_FORMAT_DUMP_SIZE;
        *dd_idisk = &g_ifile_storage_dd_sdk_dump;
        format_desc = "SDK";
        } break;

    default:
        format_desc = "ERR";
        DebugMessage(M64MSG_ERROR, "Invalid DD Disk size %u.", (uint32_t) dd_disk->size);
        close_file_storage(dd_disk);
        goto no_disk;
    }
    
    DebugMessage(M64MSG_INFO, "DD Disk: %s - %u - %s",
            dd_disk->filename,
            dd_disk->size,
            format_desc);

    uint32_t w = *(uint32_t*)dd_disk->data;
    if (w == DD_REGION_JP || w == DD_REGION_US) {
        DebugMessage(M64MSG_WARNING, "Loading a saved disk ");
    }

    return;

no_disk:
    free(dd_disk_filename);
    *dd_idisk = NULL;
}


struct gb_cart_data
{
    int control_id;
    struct file_storage rom_fstorage;
    struct file_storage ram_fstorage;
    void* gbcam_backend;
    const struct video_capture_backend_interface* igbcam_backend;
};

static struct gb_cart_data l_gb_carts_data[GAME_CONTROLLERS_COUNT];

static void init_gb_rom(void* opaque, void** storage, const struct storage_backend_interface** istorage)
{
    struct gb_cart_data* data = (struct gb_cart_data*)opaque;

    /* Ask the core loader for rom filename */
    char* rom_filename = (g_media_loader.get_gb_cart_rom == NULL)
        ? NULL
        : g_media_loader.get_gb_cart_rom(g_media_loader.cb_data, data->control_id);

    /* Handle the no cart case */
    if (rom_filename == NULL || strlen(rom_filename) == 0) {
        goto no_cart;
    }

    /* Open ROM file */
    if (open_rom_file_storage(&data->rom_fstorage, rom_filename) != file_ok) {
        DebugMessage(M64MSG_ERROR, "Failed to load ROM file: %s", rom_filename);
        goto no_cart;
    }

    DebugMessage(M64MSG_INFO, "GB Loader ROM: %s - %zu",
            data->rom_fstorage.filename,
            data->rom_fstorage.size);

    /* init GB ROM storage */
    *storage = &data->rom_fstorage;
    *istorage = &g_ifile_storage_ro;
    return;

no_cart:
    free(rom_filename);
    *storage = NULL;
    *istorage = NULL;
}

static void release_gb_rom(void* opaque)
{
    struct gb_cart_data* data = (struct gb_cart_data*)opaque;

    close_file_storage(&data->rom_fstorage);

    memset(&data->rom_fstorage, 0, sizeof(data->rom_fstorage));
}

static void init_gb_ram(void* opaque, size_t ram_size, void** storage, const struct storage_backend_interface** istorage)
{
    struct gb_cart_data* data = (struct gb_cart_data*)opaque;

    /* Ask the core loader for ram filename */
    char* ram_filename = (g_media_loader.get_gb_cart_ram == NULL)
        ? NULL
        : g_media_loader.get_gb_cart_ram(g_media_loader.cb_data, data->control_id);

    /* Handle the no RAM case
     * if NULL or empty string generate a filename
     */
    if (ram_filename == NULL || strlen(ram_filename) == 0) {
        free(ram_filename);
        ram_filename = get_gb_ram_path(namefrompath(data->rom_fstorage.filename), data->control_id+1);
    }

    /* Open RAM file
     * if file doesn't exists provide default content */
    int err = open_file_storage(&data->ram_fstorage, ram_size, ram_filename);
    if (err == file_open_error) {
        memset(data->ram_fstorage.data, 0, data->ram_fstorage.size);
        DebugMessage(M64MSG_INFO, "Providing default RAM content");
    }
    else if (err == file_read_error) {
        DebugMessage(M64MSG_WARNING, "Size mismatch between expected RAM size and effective file size");
    }

    DebugMessage(M64MSG_INFO, "GB Loader RAM: %s - %zu",
            data->ram_fstorage.filename,
            data->ram_fstorage.size);

    /* init GB RAM storage */
    *storage = &data->ram_fstorage;
    *istorage = &g_ifile_storage;
}

static void release_gb_ram(void* opaque)
{
    struct gb_cart_data* data = (struct gb_cart_data*)opaque;

    close_file_storage(&data->ram_fstorage);

    memset(&data->ram_fstorage, 0, sizeof(data->ram_fstorage));
}

void main_change_gb_cart(int control_id)
{
    struct transferpak* tpk = &g_dev.transferpaks[control_id];
    struct gb_cart* gb_cart = &g_dev.gb_carts[control_id];
    struct gb_cart_data* data = &l_gb_carts_data[control_id];

    /* reset gb_cart_data */
    memset(data, 0, sizeof(*data));
    data->control_id = control_id;

    init_gb_cart(gb_cart,
            data, init_gb_rom, release_gb_rom,
            data, init_gb_ram, release_gb_ram,
            NULL, &g_iclock_ctime_plus_delta,
            &data->control_id, &g_irumble_backend_plugin_compat,
            data->gbcam_backend, data->igbcam_backend);

    if (gb_cart->read_gb_cart == NULL) {
        gb_cart = NULL;
    }

    change_gb_cart(tpk, gb_cart);

    if (tpk->gb_cart != NULL) {
        const uint8_t* rom_data = gb_cart->irom_storage->data(gb_cart->rom_storage);
        DebugMessage(M64MSG_INFO, "Inserting GB cart %s into transferpak %u", rom_data + 0x134, control_id);
    }
    else {
        DebugMessage(M64MSG_INFO, "Removing GB cart from transferpak %u", control_id);
    }
}


/*********************************************************************************************************
* emulation thread - runs the core
*/
extern gfx_plugin_functions gfx_gln64;
extern rsp_plugin_functions rsp_hle;
extern input_plugin_functions dummy_input;
extern audio_plugin_functions dummy_audio;

unsigned int r4300_emumode;

uint32_t rdram_size;
struct file_storage eep;
struct file_storage fla;
struct file_storage sra;
struct file_storage dd_disk;
size_t dd_rom_size;

m64p_error main_run(void)
{
    size_t i, k;
    unsigned int count_per_op;
    unsigned int disable_extra_mem;
    int si_dma_duration;
    int no_compiled_jump;
    int randomize_interrupt;
    struct audio_out_backend_interface audio_out_backend_libretro;

    int control_ids[GAME_CONTROLLERS_COUNT];
    struct controller_input_compat cin_compats[GAME_CONTROLLERS_COUNT];

    struct file_storage mpk_storages[GAME_CONTROLLERS_COUNT];
    struct file_storage mpk;

    void* gbcam_backend;
    const struct video_capture_backend_interface* igbcam_backend;

    /* XXX: select type of flashram from db */
    uint32_t flashram_type = MX29L1100_ID;

    count_per_op = CountPerOp;
    disable_extra_mem = ROM_PARAMS.disableextramem;

    if (ForceDisableExtraMem == 1)
        disable_extra_mem = 1;

    rdram_size = (disable_extra_mem == 0) ? 0x800000 : 0x400000;

    if (count_per_op <= 0)
        count_per_op = ROM_PARAMS.countperop;

    si_dma_duration = ROM_PARAMS.sidmaduration;

    cheat_add_hacks(&g_cheat_ctx, ROM_PARAMS.cheats);

    /* do byte-swapping if it hasn't been done yet */
#if !defined(M64P_BIG_ENDIAN)
    if (g_RomWordsLittleEndian == 0)
    {
        swap_buffer((uint8_t*)mem_base_u32(g_mem_base, MM_CART_ROM), 4, g_rom_size/4);
        g_RomWordsLittleEndian = 1;
    }
#endif

    /* setup backends */
    extern void set_audio_format_via_libretro(void* user_data, unsigned int frequency, unsigned int bits);
    extern void push_audio_samples_via_libretro(void* user_data, const void* buffer, size_t size);
    audio_out_backend_libretro = (struct audio_out_backend_interface){ set_audio_format_via_libretro, push_audio_samples_via_libretro };
    
    /* Fill-in l_pak_type_idx and l_ipaks according to game compatibility */
    k = 0;
    if (ROM_SETTINGS.biopak) {
        l_ipaks[k++] = &g_ibiopak;
    }
    if (ROM_SETTINGS.mempak) {
        l_pak_type_idx[PLUGIN_MEMPAK] = k;
        l_ipaks[k] = &g_imempak;
        ++k;
    }
    if (ROM_SETTINGS.rumble) {
        l_pak_type_idx[PLUGIN_RUMBLE_PAK] = k;
        l_pak_type_idx[PLUGIN_RAW] = k;
        l_ipaks[k] = &g_irumblepak;
        ++k;
    }
    if (ROM_SETTINGS.transferpak) {
        l_pak_type_idx[PLUGIN_TRANSFER_PAK] = k;
        l_ipaks[k] = &g_itransferpak;
        ++k;
    }
    l_pak_type_idx[PLUGIN_NONE] = k;
    l_ipaks[k] = NULL;

    if (!ROM_SETTINGS.mempak) {
        l_pak_type_idx[PLUGIN_MEMPAK] = k;
    }
    if (!ROM_SETTINGS.rumble) {
        l_pak_type_idx[PLUGIN_RUMBLE_PAK] = k;
        l_pak_type_idx[PLUGIN_RAW] = k;
    }
    if (!ROM_SETTINGS.transferpak) {
        l_pak_type_idx[PLUGIN_TRANSFER_PAK] = k;
    }

    /* open storage files, provide default content if not present */
    open_mpk_file(&mpk);
    open_eep_file(&eep);
    open_fla_file(&fla);
    open_sra_file(&sra);

    /* Load 64DD IPL ROM and Disk */
    const struct clock_backend_interface* dd_rtc_iclock = NULL;
    const struct storage_backend_interface* dd_idisk = NULL;
    memset(&dd_disk, 0, sizeof(dd_disk));

    load_dd_rom((uint8_t*)mem_base_u32(g_mem_base, MM_DD_ROM), &dd_rom_size);
    if (dd_rom_size > 0) {
        dd_rtc_iclock = &g_iclock_ctime_plus_delta;
        load_dd_disk(&dd_disk, &dd_idisk);
    }

    /* setup pif channel devices */
    void* joybus_devices[PIF_CHANNELS_COUNT];
    const struct joybus_device_interface* ijoybus_devices[PIF_CHANNELS_COUNT];

    memset(&g_dev.gb_carts, 0, GAME_CONTROLLERS_COUNT*sizeof(*g_dev.gb_carts));
    memset(&l_gb_carts_data, 0, GAME_CONTROLLERS_COUNT*sizeof(*l_gb_carts_data));
    memset(cin_compats, 0, GAME_CONTROLLERS_COUNT*sizeof(*cin_compats));

    for (i = 0; i < GAME_CONTROLLERS_COUNT; ++i) {

        control_ids[i] = (int)i;

        /* if input plugin requests RawData let the input plugin do the channel device processing */
        if (Controls[i].RawData) {
            joybus_devices[i] = &control_ids[i];
            ijoybus_devices[i] = &g_ijoybus_device_plugin_compat;
        }
        /* otherwise let the core do the processing */
        else {
            /* select appropriate controller
             * FIXME: assume for now that only standard controller is compatible
             * Use the rom db to know if other peripherals are compatibles (VRU, mouse, train, ...)
             */
            const struct game_controller_flavor* cont_flavor =
                &g_standard_controller_flavor;

            joybus_devices[i] = &g_dev.controllers[i];
            ijoybus_devices[i] = &g_ijoybus_device_controller;

            cin_compats[i].control_id = (int)i;
            cin_compats[i].cont = &g_dev.controllers[i];
            cin_compats[i].tpk = &g_dev.transferpaks[i];
            cin_compats[i].last_pak_type = Controls[i].Plugin;

            l_gb_carts_data[i].control_id = (int)i;

            l_gb_carts_data[i].gbcam_backend = gbcam_backend;
            l_gb_carts_data[i].igbcam_backend = igbcam_backend;

            l_paks_idx[i] = 0;

            //Don't use the selected pak if it's not available for the game, instead use NONE
            if (l_ipaks[l_pak_type_idx[Controls[i].Plugin]] == NULL) {
                Controls[i].Plugin = PLUGIN_NONE;
            }

            /* init all compatibles paks */
            for(k = 0; k < PAK_MAX_SIZE; ++k) {
                /* Bio Pak */
                if (l_ipaks[k] == &g_ibiopak) {
                    init_biopak(&g_dev.biopaks[i], 64);
                    l_paks[i][k] = &g_dev.biopaks[i];

                    if (Controls[i].Plugin == PLUGIN_BIO_PAK) {
                        l_paks_idx[i] = k;
                    }
                }
                /* Memory Pak */
                else if (l_ipaks[k] == &g_imempak) {
                    mpk_storages[i].data = mpk.data + i * MEMPAK_SIZE;
                    mpk_storages[i].size = MEMPAK_SIZE;
                    mpk_storages[i].filename = (void*)&mpk; /* OK for isubfile_storage */

                    init_mempak(&g_dev.mempaks[i], &mpk_storages[i], &g_isubfile_storage);
                    l_paks[i][k] = &g_dev.mempaks[i];

                    if (Controls[i].Plugin == PLUGIN_MEMPAK) {
                        l_paks_idx[i] = k;
                    }
                }
                /* Rumble Pak */
                else if (l_ipaks[k] == &g_irumblepak) {
                    init_rumblepak(&g_dev.rumblepaks[i], &control_ids[i], &g_irumble_backend_plugin_compat);
                    l_paks[i][k] = &g_dev.rumblepaks[i];

                    if (Controls[i].Plugin == PLUGIN_RUMBLE_PAK
                     || Controls[i].Plugin == PLUGIN_RAW) {
                        l_paks_idx[i] = k;
                    }
                }
                /* Transfer Pak */
                else if (l_ipaks[k] == &g_itransferpak) {

                    /* init GB cart */
                    init_gb_cart(&g_dev.gb_carts[i],
                            &l_gb_carts_data[i], init_gb_rom, release_gb_rom,
                            &l_gb_carts_data[i], init_gb_ram, release_gb_ram,
                            NULL, &g_iclock_ctime_plus_delta,
                            &l_gb_carts_data[i].control_id, &g_irumble_backend_plugin_compat,
                            l_gb_carts_data[i].gbcam_backend, l_gb_carts_data[i].igbcam_backend);

                    init_transferpak(&g_dev.transferpaks[i], (g_dev.gb_carts[i].read_gb_cart == NULL) ? NULL : &g_dev.gb_carts[i]);
                    l_paks[i][k] = &g_dev.transferpaks[i];

                    if (Controls[i].Plugin == PLUGIN_TRANSFER_PAK) {
                        l_paks_idx[i] = k;
                    }

                    /* enable GB cart switch */
                    cin_compats[i].gb_cart_switch_enabled = 1;
                }
                /* No Pak */
                else {
                    l_ipaks[k] = NULL;
                    l_paks[i][k] = NULL;

                    if (Controls[i].Plugin == PLUGIN_NONE) {
                        l_paks_idx[i] = k;
                    }

                    break;
                }
            }

            /* init game_controller */
            init_game_controller(&g_dev.controllers[i],
                    cont_flavor,
                    &cin_compats[i], &g_icontroller_input_backend_plugin_compat,
                    l_paks[i][l_paks_idx[i]], l_ipaks[l_paks_idx[i]]);

            if (l_ipaks[l_paks_idx[i]] != NULL) {
                DebugMessage(M64MSG_INFO, "Game controller %u (%s) has a %s plugged in",
                    (uint32_t) i, cont_flavor->name, l_ipaks[l_paks_idx[i]]->name);
            } else {
                DebugMessage(M64MSG_INFO, "Game controller %u (%s) has nothing plugged in",
                    (uint32_t) i, cont_flavor->name);
            }
        }
    }

    for (i = GAME_CONTROLLERS_COUNT; i < PIF_CHANNELS_COUNT; ++i) {
        joybus_devices[i] = &g_dev.cart;
        ijoybus_devices[i] = &g_ijoybus_device_cart;
    }

    init_device(&g_dev,
                g_mem_base,
                r4300_emumode,
                count_per_op,
                no_compiled_jump,
                randomize_interrupt,
                &g_dev.ai, &audio_out_backend_libretro,
                si_dma_duration,
                rdram_size,
                joybus_devices, ijoybus_devices,
                vi_clock_from_tv_standard(ROM_PARAMS.systemtype), vi_expected_refresh_rate_from_tv_standard(ROM_PARAMS.systemtype),
                NULL, &g_iclock_ctime_plus_delta,
                g_rom_size,
                (ROM_SETTINGS.savetype != EEPROM_16KB) ? JDT_EEPROM_4K : JDT_EEPROM_16K,
                &eep, &g_ifile_storage,
                flashram_type,
                &fla, &g_ifile_storage,
                &sra, &g_ifile_storage,
                NULL, dd_rtc_iclock,
                dd_rom_size,
                &dd_disk, dd_idisk);

    // Attach rom to plugins
    if (!gfx.romOpen())
    {
        goto on_gfx_open_failure;
    }
    if (!audio.romOpen())
    {
        goto on_audio_open_failure;
    }
    if (!input.romOpen())
    {
        goto on_input_open_failure;
    }

    // setup rendering callback from video plugin to the core, for screenshots and On-Screen-Display
    gfx.setRenderingCallback(video_plugin_render_callback);

    g_EmulatorRunning = 1;
    StateChanged(M64CORE_EMU_STATE, M64EMU_RUNNING);

    poweron_device(&g_dev);
    pif_bootrom_hle_execute(&g_dev.r4300);

    run_device(&g_dev);

    /* release gb_carts */
    for(i = 0; i < GAME_CONTROLLERS_COUNT; ++i) {
        if (!Controls[i].RawData && g_dev.gb_carts[i].read_gb_cart != NULL) {
            release_gb_rom(&l_gb_carts_data[i]);
            release_gb_ram(&l_gb_carts_data[i]);
        }
    }

#if 0
    igbcam_backend->close(gbcam_backend);
    igbcam_backend->release(gbcam_backend);

    close_file_storage(&sra);
    close_file_storage(&fla);
    close_file_storage(&eep);
    close_file_storage(&mpk);
    close_file_storage(&dd_disk);
#endif

    /* Emulation stopped */
    rsp.romClosed();
    input.romClosed();
    audio.romClosed();
    gfx.romClosed();

    // clean up
    g_EmulatorRunning = 0;
    StateChanged(M64CORE_EMU_STATE, M64EMU_STOPPED);

    /**
     * Actually never returns.
     * Jump back to frontend for deinit
     */
    extern cothread_t retro_thread;
    co_switch(retro_thread);

    return M64ERR_SUCCESS;

on_input_open_failure:
    audio.romClosed();
on_audio_open_failure:
    gfx.romClosed();
on_gfx_open_failure:
    /* release gb_carts */
    for(i = 0; i < GAME_CONTROLLERS_COUNT; ++i) {
        if (!Controls[i].RawData && g_dev.gb_carts[i].read_gb_cart != NULL) {
            release_gb_rom(&l_gb_carts_data[i]);
            release_gb_ram(&l_gb_carts_data[i]);
        }
    }

#if 0
    igbcam_backend->close(gbcam_backend);
    igbcam_backend->release(gbcam_backend);

    /* release storage files */
    close_file_storage(&sra);
    close_file_storage(&fla);
    close_file_storage(&eep);
    close_file_storage(&mpk);
    close_file_storage(&dd_disk);
#endif

    return M64ERR_PLUGIN_FAIL;
}

void main_stop(void)
{
    /* note: this operation is asynchronous.  It may be called from a thread other than the
       main emulator thread, and may return before the emulator is completely stopped */
    if (!g_EmulatorRunning)
        return;

    DebugMessage(M64MSG_STATUS, "Stopping emulation.");

    if (g_rom_pause)
    {
        g_rom_pause = 0;
        StateChanged(M64CORE_EMU_STATE, M64EMU_RUNNING);
    }

    stop_device(&g_dev);
}
