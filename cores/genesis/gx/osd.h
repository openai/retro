/*************************************************/
/* port specific stuff should be put there       */
/*************************************************/

#ifndef _OSD_H_
#define _OSD_H_

#include <gccore.h>
#include <ogcsys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/dir.h>
#include <unistd.h>
#include <asndlib.h>
#include <oggplayer.h>
#include <zlib.h>

#ifdef HW_RVL
#include <di/di.h>
#include <wiiuse/wpad.h>
#include "vi_encoder.h"
#endif

#include "gx_input.h"
#include "gx_audio.h"
#include "gx_video.h"
#include "file_load.h"
#include "cheats.h"

#include "config.h"
#include "fileio.h"

#define DEFAULT_PATH  "/genplus"

/*************************************************/
/* required by Genesis Plus GX core              */
/*************************************************/
#define CD_BIOS_US    config.sys_rom[0]
#define CD_BIOS_EU    config.sys_rom[1]
#define CD_BIOS_JP    config.sys_rom[2]
#define MD_BIOS       config.sys_rom[3]
#define MS_BIOS_US    config.sys_rom[4]
#define MS_BIOS_EU    config.sys_rom[5]
#define MS_BIOS_JP    config.sys_rom[6]
#define GG_BIOS       config.sys_rom[7]
#define GG_ROM        config.sys_rom[8]
#define AR_ROM        config.sys_rom[9]
#define SK_ROM        config.sys_rom[10]
#define SK_UPMEM      config.sys_rom[11]

#define CD_BRAM_US    "/genplus/saves/cd/scd_U.brm"
#define CD_BRAM_EU    "/genplus/saves/cd/scd_E.brm"
#define CD_BRAM_JP    "/genplus/saves/cd/scd_J.brm"
#define CART_BRAM     "/genplus/saves/cd/cart.brm"

/*********************************************************/
/* implemented by Genesis Plus GX core (GC/Wii specific) */
/*********************************************************/

/* 32 bytes aligned sound buffers (8 samples alignment) */
#define ALIGN_SND 0xfffffff8

/* use Wii DVD LED to indicate when virtual CD tray is open */
#ifdef HW_RVL
#define CD_TRAY_CALLBACK *(u32*)0xcd0000c0 = (*(u32*)0xcd0000c0 & ~0x20) | ((cdd.status == CD_OPEN) << 5);
#endif

/*************************************************/

#define VERSION "Genesis Plus GX 1.7.5"

#define VIDEO_WAIT    0x01
#define AUDIO_WAIT    0x02
#define VIDEO_UPDATE  0x04
#define AUDIO_UPDATE  0x08

/* globals */
extern void legal(void);
extern double get_framerate(void);
extern void reloadrom(void);
extern void shutdown(void);
extern u32 Shutdown;
extern u32 ConfigRequested;

#endif /* _OSD_H_ */
