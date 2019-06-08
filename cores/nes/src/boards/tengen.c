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

/* Mapper 64 - 	Tengen 800032 Rambo-1
 * Mapper 158 -	Tengen 800037 (Alien Syndrome Unl)
*/

#include "mapinc.h"

static uint8 cmd, mirr, regs[11];
static uint8 rmode, IRQmode, IRQCount, IRQa, IRQLatch;

static void (*cwrap)(uint32 A, uint8 V);
static void (*mwrap)(uint8 V);
static int _isM158;

static SFORMAT StateRegs[] = {
	{ regs, 11, "REGS" },
	{ &cmd, 1, "CMDR" },
	{ &mirr, 1, "MIRR" },
	{ &rmode, 1, "RMOD" },
	{ &IRQmode, 1, "IRQM" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQLatch, 1, "IRQL" },
	{ 0 }
};

static void FP_FASTAPASS(1) RAMBO1IRQHook(int a) {
	static int32 smallcount;
	if (IRQmode) {
		smallcount += a;
		while (smallcount >= 4) {
			smallcount -= 4;
			IRQCount--;
			if (IRQCount == 0xFF)
				if (IRQa) X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void RAMBO1HBHook(void) {
	if ((!IRQmode) && (scanline != 240)) {
		rmode = 0;
		IRQCount--;
		if (IRQCount == 0xFF) {
			if (IRQa) {
				rmode = 1;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void Sync(void) {
	if (cmd & 0x20) {
		cwrap(0x0000, regs[0]);
		cwrap(0x0400, regs[8]);
		cwrap(0x0800, regs[1]);
		cwrap(0x0C00, regs[9]);
	} else {
		cwrap(0x0000, (regs[0] & 0xFE));
		cwrap(0x0400, (regs[0] & 0xFE) | 1);
		cwrap(0x0800, (regs[1] & 0xFE));
		cwrap(0x0C00, (regs[1] & 0xFE) | 1);
	}
	cwrap(0x1000, regs[2]);
	cwrap(0x1400, regs[3]);
	cwrap(0x1800, regs[4]);
	cwrap(0x1C00, regs[5]);
	setprg8(0x8000, regs[6]);
	setprg8(0xA000, regs[7]);
	setprg8(0xC000, regs[10]);
	setprg8(0xE000, ~0);
	if (!_isM158)
		setmirror(mirr);
}


static DECLFW(RAMBO1_Write) {
	switch (A & 0xF001) {
	case 0xA000:
		if (!_isM158) {
			mirr = (V & 1) ^ 1;
			Sync();
		}
		break;
	case 0x8000: cmd = V; break;
	case 0x8001:
		if ((cmd & 0xF) < 10)
			regs[cmd & 0xF] = V;
		else if ((cmd & 0xF) == 0xF)
			regs[10] = V;
		Sync();
		break;
	case 0xC000:
		IRQLatch = V;
		if (rmode == 1)
			IRQCount = IRQLatch;
		break;
	case 0xC001:
		rmode = 1;
		IRQCount = IRQLatch;
		IRQmode = V & 1;
		break;
	case 0xE000:
		IRQa = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		if (rmode == 1)
			IRQCount = IRQLatch;
		break;
	case 0xE001:
		IRQa = 1;
		if (rmode == 1)
			IRQCount = IRQLatch;
		break;
	}
}

static void RAMBO1Power(void) {
	cmd = mirr = 0;
	regs[0] = regs[1] = regs[2] = regs[3] = regs[4] = regs[5] = ~0;
	regs[6] = regs[7] = regs[8] = regs[9] = regs[10] = ~0;
	Sync();
	if (!_isM158) setmirror(1);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, RAMBO1_Write);
}

static void StateRestore(int version) {
	Sync();
}

static void RAMBO1_Init(CartInfo *info) {
	info->Power = RAMBO1Power;
	GameHBIRQHook = RAMBO1HBHook;
	MapIRQHook = RAMBO1IRQHook;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

static void M64CWRAP(uint32 A, uint8 V) {
	setchr1(A, V);
}

void Mapper64_Init(CartInfo *info) {
	_isM158 = 0;
	cwrap = M64CWRAP;
	RAMBO1_Init(info);
}

static uint8 M158MIR[8];
static uint8 PPUCHRBus;

static void FP_FASTAPASS(1) M158PPU(uint32 A) {
	A &= 0x1FFF;
	A >>= 10;
	PPUCHRBus = A;
	setmirror(MI_0 + M158MIR[A]);
}

static void M158CWRAP(uint32 A, uint8 V) {
	M158MIR[A >> 10] = (V >> 7) & 1;
	setchr1(A, V);
	if (PPUCHRBus == (A >> 10))
		setmirror(MI_0 + ((V >> 7) & 1));
}

void Mapper158_Init(CartInfo *info) {
	_isM158 = 1;
	cwrap = M158CWRAP;
	PPU_hook = M158PPU;
	RAMBO1_Init(info);
	AddExState(&PPUCHRBus, 1, 0, "PPUC");
}
