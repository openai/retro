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

/* Various macros for faster memory stuff
	(at least that's the idea)
*/

#ifndef _FCEU_MEMORY_H_
#define _FCEU_MEMORY_H_

#include "fceu-types.h"

#define FCEU_dwmemset(d, c, n) { int _x; for (_x = n - 4; _x >= 0; _x -= 4) *(uint32*)& (d)[_x] = c; }

#if defined(STATE_LIBRETRO) || defined(ENDIAN_LIBRETRO) || defined(GENERAL_LIBRETRO)
#include "drivers/libretro/libretro-common/include/streams/memstream.h"

#define HAVE_MEMSTREAM
#define MEM_TYPE memstream_t

#define fwrite(ptr, size, nmemb, stream) memstream_write((stream), (ptr), (nmemb))
#define fclose(fp) memstream_close((fp))
#define fgetc(stream) memstream_getc((stream))
#define fputc(c, stream) memstream_putc((stream), (c))
#define ftell(a) memstream_pos((a))
#define fread(ptr, size, nmemb, stream) memstream_read((stream), (ptr), (nmemb))
#define fseek(stream, offset, whence) memstream_seek((stream), (offset), (whence))
#else
#define MEM_TYPE FILE
#endif

void *FCEU_malloc(uint32 size);
void *FCEU_gmalloc(uint32 size);
void FCEU_gfree(void *ptr);
void FCEU_free(void *ptr);
void FASTAPASS(3) FCEU_memmove(void *d, void *s, uint32 l);

#endif
