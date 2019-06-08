/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012
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
 *
 * Pegasus 5-in-1 (Golden Five) (Unl)
 */

#include "mapinc.h"

static uint8 preg[2];
static uint8 *WRAM = NULL;
static uint32 WRAMSIZE;

static SFORMAT StateRegs[] =
{
	{ preg, 2, "PREG" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, preg[0]);
	setprg16(0xC000, preg[1]);
	setchr8(0);
}

static DECLFW(M104WriteBank) {
	if ((V & 8) > 0) {
		preg[0]  = ((V << 4) & 0x70) | (preg[0] & 0x0F);
		preg[1]  = ((V << 4) & 0x70) | 0x0F;
		Sync();
	}
}

static DECLFW(M104WritePreg) {
	preg[0] = (preg[0] & 0x70) | (V & 0x0F);
	Sync();
}

static void M104Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void M104Power(void) {
	preg[1] = 0x0F;
	Sync();
	SetReadHandler(0x6000, 0x7fff, CartBR);
	SetWriteHandler(0x6000, 0x7fff, CartBW);
	SetWriteHandler(0x8000, 0x9FFF, M104WriteBank);
	SetWriteHandler(0xC000, 0xFFFF, M104WritePreg);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	setmirror(MI_V);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper104_Init(CartInfo *info) {
	info->Power = M104Power;
	info->Close = M104Close;
	AddExState(&StateRegs, ~0, 0, 0);

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}

	GameStateRestore = StateRestore;
}
