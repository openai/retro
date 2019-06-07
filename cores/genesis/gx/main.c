/****************************************************************************
 *  main.c
 *
 *  Genesis Plus GX
 *
 *  Copyright Eke-Eke (2007-2014), based on original work from Softdev (2006)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/

#include "shared.h"
#include "font.h"
#include "gui.h"
#include "menu.h"
#include "history.h"
#include "file_slot.h"
#include "file_load.h"
#include "filesel.h"
#include "cheats.h"
#include "md_ntsc.h"

#include <fat.h>

#ifdef HW_RVL
#include <iso9660.h>
#include <ogc/usbmouse.h>
extern bool sdio_Deinitialize();
extern void USBStorage_Deinitialize();
#endif

u32 Shutdown = 0;
u32 ConfigRequested = 1;

#ifdef HW_RVL
/****************************************************************************
 * Power Button callbacks
 ***************************************************************************/
static void PowerOff_cb(void)
{
  Shutdown = 1;
  ConfigRequested = 1;
  reload = 0;
}

static void Reload_cb(void)
{
  Shutdown = 1;
  ConfigRequested = 1;
}
#endif

/****************************************************************************
 * Reset Button callback 
 ***************************************************************************/
static void Reset_cb(void)
{
  if (system_hw & SYSTEM_MD)
  {
    /* Soft Reset */
    gen_reset(0);
  }
  else if (system_hw == SYSTEM_SMS)
  {
    /* assert RESET input (Master System model 1 only) */
    io_reg[0x0D] &= ~IO_RESET_HI;
  }
}

/***************************************************************************
 * Genesis Plus Virtual Machine
 *
 ***************************************************************************/
static void init_machine(void)
{
  /* system is not initialized */
  config.hot_swap &= 0x01;

  /* mark all BIOS as unloaded */
  system_bios = 0;

  /* try to load Genesis BOOT ROM (2KB max) */
  memset(boot_rom, 0xFF, 0x800);
  if (load_archive(MD_BIOS, boot_rom, 0x800, NULL) > 0)
  {
    /* check if BOOT ROM header is valid */
    if (!memcmp((char *)(boot_rom + 0x120),"GENESIS OS", 10))
    {
      /* mark Genesis BIOS as loaded */
      system_bios = SYSTEM_MD;
    }
  }

  /* allocate global work bitmap */
  memset(&bitmap, 0, sizeof (bitmap));
  bitmap.width = MD_NTSC_OUT_WIDTH(320 + 2 * 14);
  bitmap.height = 576;
  bitmap.pitch = bitmap.width * 2;
  bitmap.data = memalign(32, bitmap.pitch * bitmap.height);

  if (!bitmap.data)
  {
    GUI_WaitPrompt("Error","Unable to allocate memory");
    exit(0);
  }
}

static void run_emulation(void)
{
  u32 sync;

  /* main emulation loop */
  while (1)
  {
    /* emulated system */
    if (system_hw == SYSTEM_MCD)
    {
      /* 16-bit hardware + CD */
      while (!ConfigRequested)
      {
        /* render frame */
        system_frame_scd(0);

        /* audio/video sync */
        sync = VIDEO_WAIT | VIDEO_UPDATE | AUDIO_WAIT | AUDIO_UPDATE;
        while (sync)
        {
          /* update audio */
          sync &= gx_audio_Update(sync);

          /* update video */
          sync &= gx_video_Update(sync);
        }
      }
    }
    else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
    {
      /* 16-bit hardware */
      while (!ConfigRequested)
      {
        /* render frame */
        system_frame_gen(0);

        /* audio/video sync */
        sync = VIDEO_WAIT | VIDEO_UPDATE | AUDIO_WAIT | AUDIO_UPDATE;
        while (sync)
        {
          /* update audio */
          sync &= gx_audio_Update(sync);

          /* update video */
          sync &= gx_video_Update(sync);
        }
      }
    }
    else
    {
      /* 8-bit hardware */
      while (!ConfigRequested)
      {
        /* render frame */
        system_frame_sms(0);

        /* audio/video sync */
        sync = VIDEO_WAIT | VIDEO_UPDATE | AUDIO_WAIT | AUDIO_UPDATE;
        while (sync)
        {
          /* update audio */
          sync &= gx_audio_Update(sync);

          /* update video */
          sync &= gx_video_Update(sync);
        }
      }
    }

    /* stop video & audio */
    gx_audio_Stop();
    gx_video_Stop();
    
    /* show menu */
    ConfigRequested = 0;
    mainmenu();

    /* restart video & audio */
    gx_audio_Start();
    gx_video_Start();
  }
}

/*********************************************************************************************************************************************************
  Get emulator input framerate (actually used by audio emulation to approximate number of samples rendered on each frame, see audio_init in system.c)
*********************************************************************************************************************************************************/
double get_framerate(void)
{
  /* Run emulator at original VDP framerate if console TV mode does not match emulated TV mode or VSYNC is disabled */
  if (!config.vsync || (config.tv_mode == !vdp_pal))
  {
    return 0.0;
  }

  /* Otherwise, run emulator at Wii/Gamecube framerate to ensure perfect video synchronization */
  if (vdp_pal)
  {
    /* 288p      -> 13500000 pixels/sec, 864 pixels/line, 312 lines/field -> fps = 13500000/864/312 = 50.08 hz */
    /* 288i,576i -> 13500000 pixels/sec, 864 pixels/line, 312.5 lines/field (two fields = one frame = 625 lines) -> fps = 13500000/864/312.5 = 50.00 hz */
    return (config.render || interlaced) ? (27000000.0/864.0/625.0) : (13500000.0/864.0/312.0);
  }
  else
  {
    /* 240p      -> 13500000 pixels/sec, 858 pixels/line, 263 lines/field -> fps = 13500000/858/263 = 59.83 hz */
    /* 240i,480i -> 13500000 pixels/sec, 858 pixels/line, 262.5 lines/field (two fields = one frame = 525 lines) -> fps = 13500000/858/262.5 = 59.94 hz */
    /* 480p      -> 27000000 pixels/sec, 858 pixels/line, 525 lines/field -> fps = 27000000/858/525 = 59.94 hz */
    return (config.render || interlaced) ? (27000000.0/858.0/525.0) : (13500000.0/858.0/263.0);
  }
}

/*******************************************
  Restart emulation when loading a new game 
********************************************/
void reloadrom(void)
{
  /* Cartridge "Hot Swap" support (make sure system has already been inited once and use cartridges) */
  if ((config.hot_swap == 3) && ((system_hw != SYSTEM_MCD) || scd.cartridge.boot))
  {
    /* Only initialize cartridge hardware */
    if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
    {
      /* 16-bit cartridge */
      md_cart_init();
      md_cart_reset(1);
    }
    else
    {
      /* 8-bit cartridge */
      sms_cart_init();
      sms_cart_reset();
    }
  }

  /* Disc Swap support (automatically enabled if CD tray is open) */
  else if ((system_hw != SYSTEM_MCD) || (cdd.status != CD_OPEN))
  {
    /* Initialize audio emulation */
    interlaced = 0;
    audio_init(48000, get_framerate());
     
    /* System Power-On */
    system_init();
    system_reset();

    /* Allow hot swap */
    config.hot_swap |= 2;
  }

  /* Auto-Load Backup RAM */
  slot_autoload(0,config.s_device);
            
  /* Auto-Load State */
  slot_autoload(config.s_default,config.s_device);

  /* Load Cheat file */
  CheatLoad();
}

/**************************************************
  Shutdown everything properly
***************************************************/
void shutdown(void)
{
  /* save current config */
  config_save();

  /* auto-save State file */
  slot_autosave(config.s_default,config.s_device);

  /* shutdown emulation core */
  audio_shutdown();

  /* shutdown audio & video engines */
  gx_audio_Shutdown();
  gx_video_Shutdown();
  if (bitmap.data) free(bitmap.data);

#ifdef HW_RVL
  /* unmount all devices */
  ISO9660_Unmount("dvd:");
  fatUnmount("sd");
  fatUnmount("usb");

  /* shutdown all devices */
  DI_Close();
  sdio_Deinitialize();
  USBStorage_Deinitialize();
  MOUSE_Deinit();
#endif
}

/***************************************************************************
 *  M A I N
 *
 ***************************************************************************/
int main (int argc, char *argv[])
{
 #ifdef HW_RVL
  /* enable 64-byte fetch mode for L2 cache */
  L2Enhance();
  
  /* disable DVD cache */
  DI_UseCache(0);

  /* autodetect loader arguments */
  if ((argc >= 3) && (argv[1] != NULL))
  {
    /* check if autoloading from USB is requested */ 
    if (!strncasecmp(argv[1], "usb:/", 5))
    {
      /* reload to IOS58 for USB2 support  */
      if (IOS_GetVersion() != 58)
      {
        /* warning: DVD support will be disabled after IOS reloading ! */
        IOS_ReloadIOS(58);
      }
    }
  }
#endif

  /* initialize video engine */
  gx_video_Init();

  /* initialize input engine */
  gx_input_Init();

  /* initialize FAT devices */
  int retry = 0;
  int fatMounted = 0;

  /* try to mount FAT devices during 3 seconds */
  while (!fatMounted && (retry < 12))
  {
    fatMounted = fatInitDefault();
    usleep(250000);
    retry++;
  }

  if (fatMounted)
  {
    /* base directory */
    char pathname[MAXPATHLEN];
    sprintf (pathname, DEFAULT_PATH);
    DIR *dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default SRAM & Savestate files directories */ 
    sprintf (pathname, "%s/saves",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/saves/md",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/saves/ms",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/saves/gg",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/saves/sg",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/saves/cd",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default Snapshot files directories */ 
    sprintf (pathname, "%s/snaps",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/snaps/md",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/snaps/ms",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/snaps/gg",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/snaps/sg",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/snaps/cd",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default Cheat files directories */ 
    sprintf (pathname, "%s/cheats",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/cheats/md",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/cheats/ms",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/cheats/gg",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/cheats/sg",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
    sprintf (pathname, "%s/cheats/cd",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default BIOS ROM files directories */ 
    sprintf (pathname, "%s/bios",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);

    /* default LOCK-ON ROM files directories */ 
    sprintf (pathname, "%s/lock-on",DEFAULT_PATH);
    dir = opendir(pathname);
    if (dir) closedir(dir);
    else mkdir(pathname,S_IRWXU);
  }

  /* initialize audio engine */
  gx_audio_Init();

  /* initialize emulation */
  history_default();
  config_default();
  init_machine();

  /* file autoloading */
  int autoload = config.autoload;

  /* autodetect loader arguments */
  if ((argc >= 3) && (argv[1] != NULL) && (argv[2] != NULL))
  {
    /* automatically load any file passed as argument */
    autoload = 1;

    /* add the file to the top of the history. */
    history_add_file(argv[1], argv[2], 0);
  }

  /* automatically load first file from history list if requested */
  if (autoload)
  {
    SILENT = 1;
    if (OpenDirectory(TYPE_RECENT, -1))
    {
      if (LoadFile(0))
      {
        reloadrom();
        gx_video_Start();
        gx_audio_Start();
        ConfigRequested = 0;
      }
    }
    SILENT = 0;
  }

  /* show disclaimer before entering menu */
  if (ConfigRequested)
  {
    legal();
  }

  /* initialize stub loader detection */
  reload = 0;

#ifdef HW_RVL
  /* autodetect loader arguments */
  if ((argc >= 4) && (argv[3] != NULL))
  {
    /* assume proper loader stub exists */
    reload = (void(*)())0x80001800;

    /* return to loader when POWER buttons are pressed */
    SYS_SetPowerCallback(Reload_cb);
  }
  else
  {
    /* autodetect HomeBrew Channel stub */
    u32 *sig = (u32*)0x80001800;
    if ((sig[1] == 0x53545542) && (sig[2] == 0x48415858))
    {
      reload = (void(*)())0x80001800;
    }

    /* by default, shutdown system when POWER buttons are pressed */
    SYS_SetPowerCallback(PowerOff_cb);
  }
#else
  /* autodetect SDLoad stub */
  u32 *sig = (u32*)0x80001800;
  if (sig[0] == 0x7c6000a6)
  {
    reload = (void(*)())0x80001800;
  }
#endif

  /* RESET button callback */
  SYS_SetResetCallback(Reset_cb);

  /* main emulation loop */
  run_emulation();

  /* we should never return anyway */
  return 0;
}
