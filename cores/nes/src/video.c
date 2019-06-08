/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "fceu-types.h"
#include "video.h"
#include "fceu.h"
#include "general.h"
#include "fceu-memory.h"
#include "crc32.h"
#include "state.h"
#include "palette.h"
#include "nsf.h"
#include "input.h"
#include "vsuni.h"

uint8 *XBuf = NULL;

void FCEU_KillVirtualVideo(void)
{
	if (XBuf)
		free(XBuf);
   XBuf = 0;
}

int FCEU_InitVirtualVideo(void)
{
   /* 256 bytes per scanline, * 240 scanline maximum, +8 for alignment, */
   if (!XBuf)
      XBuf = (uint8*)(FCEU_malloc(256 * (256 + extrascanlines + 8)));

   if (!XBuf)
      return 0;

   memset(XBuf, 128, 256 * (256 + extrascanlines));
   return 1;
}

static int howlong;
static char errmsg[65];

#include "drawing.h"

void FCEUI_SaveSnapshot(void) { }

void FCEU_PutImage(void)
{
	if (GameInfo->type == GIT_NSF)
		DrawNSF(XBuf);
   else
   {
		if (GameInfo->type == GIT_VSUNI)
			FCEU_VSUniDraw(XBuf);
	}
	if (howlong) howlong--;
}

void FCEU_PutImageDummy(void)
{
}

void FCEU_DispMessage(char *format, ...)
{
   va_list ap;

   va_start(ap, format);
   vsprintf(errmsg, format, ap);
   va_end(ap);

   howlong = 180;
   FCEUD_DispMessage(errmsg);
}

void FCEU_ResetMessages(void)
{
	howlong = 180;
}

int SaveSnapshot(void)
{
	return(0);
}
