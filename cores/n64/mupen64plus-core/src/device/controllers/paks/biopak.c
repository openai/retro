/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - biopak.c                                             *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2017 Bobby Smiles                                       *
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

/* Implementation based on notes from raphnet
 * See http://www.raphnet.net/divers/n64_bio_sensor/index_en.php
 */

#include "biopak.h"

#include "api/m64p_types.h"
#include "api/callbacks.h"

#ifdef HAVE_LIBNX
#include <switch.h>
#endif

#include <time.h>
#include <string.h>

void init_biopak(struct biopak* bpk,
    unsigned int bpm)
{
    bpk->bpm = bpm;
}

static void plug_biopak(void* pak)
{
}

static void unplug_biopak(void* pak)
{
}

static void read_biopak(void* pak, uint16_t address, uint8_t* data, size_t size)
{
    struct biopak* bpk = (struct biopak*)pak;

    if (address == 0xc000) {
        time_t now = time(NULL) * 1000;
        uint32_t period = UINT32_C(60*1000) / bpk->bpm;
        uint32_t k = now % period;

        memset(data, (2*k < period) ? 0x00 : 0x03, size);
    }
    else {
        DebugMessage(M64MSG_WARNING, "Unexpected bio sensor read address %04x", address);
    }
}

static void write_biopak(void* pak, uint16_t address, const uint8_t* data, size_t size)
{
    DebugMessage(M64MSG_WARNING, "Unexpected bio sensor write address %04x", address);
}


/* bio pak definition */
const struct pak_interface g_ibiopak =
{
    "Bio pak",
    plug_biopak,
    unplug_biopak,
    read_biopak,
    write_biopak
};
