
#ifndef _OSD_H_
#define _OSD_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <SDL.h>
#include <stdlib.h>

#include "shared.h"
#include "main.h"
#include "config.h"
#include "error.h"
#include "unzip.h"
#include "fileio.h"

#define MAXPATHLEN 1024
#define DEFAULT_PATH  "/.genplus"
#define osd_input_update sdl_input_update


#define GG_ROM      "/usr/local/home/.genplus/lock-on/ggenie.bin"
#define AR_ROM      "/usr/local/home/.genplus/lock-on/areplay.bin"
#define SK_ROM      "/usr/local/home/.genplus/lock-on/sk.bin"
#define SK_UPMEM    "/usr/local/home/.genplus/lock-on/sk2chip.bin"
#define CD_BIOS_US  "/usr/local/home/.genplus/bios/bios_CD_U.bin"
#define CD_BIOS_EU  "/usr/local/home/.genplus/bios/bios_CD_E.bin"
#define CD_BIOS_JP  "/usr/local/home/.genplus/bios/bios_CD_J.bin"
#define MD_BIOS     "/usr/local/home/.genplus/bios/bios_MD.bin"
#define MS_BIOS_US  "/usr/local/home/.genplus/bios/bios_U.sms"
#define MS_BIOS_EU  "/usr/local/home/.genplus/bios/bios_E.sms"
#define MS_BIOS_JP  "/usr/local/home/.genplus/bios/bios_J.sms"
#define GG_BIOS     "/usr/local/home/.genplus/bios/bios.gg"

#endif /* _OSD_H_ */
