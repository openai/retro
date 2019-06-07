/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright (C) 2017 FCEUX Team
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
 * Magic Kid GooGoo
 */

#include "mapinc.h"

static uint8 prgr, chrr[4];
static uint8 *WRAM = NULL;

static void Mapper190_Sync(void) {
	setprg8r(0x10, 0x6000, 0);

	setprg16(0x8000, prgr);
	setprg16(0xC000, 0);
	setchr2(0x0000, chrr[0]);
	setchr2(0x0800, chrr[1]);
	setchr2(0x1000, chrr[2]);
	setchr2(0x1800, chrr[3]);
}

static DECLFW(Mapper190_Write89) { prgr = V&7; Mapper190_Sync(); }
static DECLFW(Mapper190_WriteCD) { prgr = 8|(V&7); Mapper190_Sync(); }

static DECLFW(Mapper190_WriteAB) {
	int bank = A&3;
	chrr[bank] = V&63;
	Mapper190_Sync();
}


static void Mapper190_Power(void) {
	FCEU_CheatAddRAM(0x2000 >> 10, 0x6000, WRAM);

	SetReadHandler(0x6000, 0xFFFF, CartBR);

	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0x9FFF, Mapper190_Write89);
	SetWriteHandler(0xA000, 0xBFFF, Mapper190_WriteAB);
	SetWriteHandler(0xC000, 0xDFFF, Mapper190_WriteCD);
	Mapper190_Sync();

	setmirror(MI_V);
}

static void Mapper190_Close(void) {
	FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void Mapper190_Restore(int version) {
	Mapper190_Sync();
}


void Mapper190_Init(CartInfo *info) {
	info->Power = Mapper190_Power;
	info->Close = Mapper190_Close;
	GameStateRestore = Mapper190_Restore;

	WRAM = (uint8*)FCEU_gmalloc(0x2000);
	SetupCartPRGMapping(0x10, WRAM, 0x2000, 1);

	chrr[0] = chrr[1] = chrr[2] = chrr[3] = prgr = 0;

	AddExState(&prgr, 1, 0, "PRGR");
	AddExState(chrr, 4, 0, "CHRR");
	AddExState(WRAM, 0x2000, 0, "WRAM");
}
