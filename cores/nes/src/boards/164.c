/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel 2006 CaH4e3
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
 * It seems that 162/163/164 mappers are the same mapper with just different
 * mapper modes enabled or disabled in software or hardware, need more nanjing
 * carts
 */

#include "mapinc.h"

static uint8 laststrobe, trigger;
static uint8 reg[8];
static uint8 *WRAM = NULL;
static uint32 WRAMSIZE;

static writefunc pcmwrite;

static void (*WSync)(void);

static SFORMAT StateRegs[] =
{
	{ &laststrobe, 1, "STB" },
	{ &trigger, 1, "TRG" },
	{ reg, 8, "REGS" },
	{ 0 }
};

static int16 step_size[49] = {
	16, 17, 19, 21, 23, 25, 28, 31, 34, 37,
	41, 45, 50, 55, 60, 66, 73, 80, 88, 97,
	107, 118, 130, 143, 157, 173, 190, 209, 230, 253,
	279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
	724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552
};	//49 items
static int32 step_adj[16] = { -1, -1, -1, -1, 2, 5, 7, 9, -1, -1, -1, -1, 2, 5, 7, 9 };

//decode stuff
static int32 jedi_table[16 * 49];
static int32 acc = 0;	//ADPCM accumulator, initial condition must be 0
static int32 decstep = 0;	//ADPCM decoding step, initial condition must be 0

static void jedi_table_init() {
	int step, nib;

	for (step = 0; step < 49; step++) {
		for (nib = 0; nib < 16; nib++) {
			int value = (2 * (nib & 0x07) + 1) * step_size[step] / 8;
			jedi_table[step * 16 + nib] = ((nib & 0x08) != 0) ? -value : value;
		}
	}
}

static uint8 decode(uint8 code) {
	acc += jedi_table[decstep + code];
	if ((acc & ~0x7ff) != 0)	// acc is > 2047
		acc |= ~0xfff;
	else acc &= 0xfff;
	decstep += step_adj[code & 7] * 16;
	if (decstep < 0) decstep = 0;
	if (decstep > 48 * 16) decstep = 48 * 16;
	return (acc >> 8) & 0xff;
}

/*
const EEPROM_interface eeprom_interface. =
{
	9,					// address bits 9
	8,					// data bits    8
	"*110",				// read         1 10 aaaaaaaaa
	"*101",				// write        1 01 aaaaaaaaa dddddddd
	"*10000xxxxxxx",	// lock         1 00 00xxxxxxx
	"*10011xxxxxxx",	// unlock       1 00 11xxxxxxx
	1,
	5
};

static const EEPROM_interface *intf;

static int serial_count = 0;
static u8  serial_buffer[SERIAL_BUFFER_LENGTH];

static int eeprom_data_bits;
static int eeprom_clock_count;
static int eeprom_read_address;
static u8  *eeprom_data;

static int latch = 0;
static int locked = 1;
static int sending = 0;
static int reset_line = ASSERT_LINE;
static int clock_line = ASSERT_LINE;
static int reset_delay;

void EEPROM_Init(u8 *data, u8 bit)
{
	eeprom_data = data;
	if(bit == 8)
		intf = &eeprom_interface_93C46_8;
	else
		intf = &eeprom_interface_93C46_16;
}

u8 *EEPROM_GetData()
{
	return eeprom_data;
}

static int EEPROM_command_match(const char *buf, const char *cmd, int len)
{
	if ( cmd == 0 )	return 0;
	if ( len == 0 )	return 0;

	for (;len>0;)
	{
		char b = *buf;
		char c = *cmd;

		if ((b==0) || (c==0))
			return (b==c);

		switch ( c )
		{
		case '0':
		case '1':
			if (b != c)	return 0;
		case 'X':
		case 'x':
			buf++;
			len--;
			cmd++;
			break;

		case '*':
			c = cmd[1];
			switch( c )
			{
			case '0':
			case '1':
				if (b == c)	{	cmd++;			}
				else		{	buf++;	len--;	}
				break;
			default:	return 0;
			}
		}
	}
	return (*cmd==0);
}

static void EEPROM_write(int bit)
{
	if (serial_count >= SERIAL_BUFFER_LENGTH-1)
	{
		return;
	}

	serial_buffer[serial_count++] = (bit ? '1' : '0');
	serial_buffer[serial_count] = 0;

	if ( (serial_count > intf->address_bits) &&
				EEPROM_command_match((char*)serial_buffer,intf->cmd_read,(int)strlen((char*)serial_buffer)-intf->address_bits) )
	{
		int i,address;

		address = 0;
		for (i = serial_count-intf->address_bits;i < serial_count;i++)
		{
			address <<= 1;
			if (serial_buffer[i] == '1') address |= 1;
		}
		if (intf->data_bits == 16)
			eeprom_data_bits = (eeprom_data[2*address+0] << 8) + eeprom_data[2*address+1];
		else
			eeprom_data_bits = eeprom_data[address];
		eeprom_read_address = address;
		eeprom_clock_count = 0;
		sending = 1;
		serial_count = 0;
	}
	else if ( (serial_count > intf->address_bits) &&
						EEPROM_command_match((char*)serial_buffer,intf->cmd_erase,(int)strlen((char*)serial_buffer)-intf->address_bits) )
	{
		int i,address;

		address = 0;
		for (i = serial_count-intf->address_bits;i < serial_count;i++)
		{
			address <<= 1;
			if (serial_buffer[i] == '1') address |= 1;
		}

		if (locked == 0)
		{
			if (intf->data_bits == 16)
			{
				eeprom_data[2*address+0] = 0x00;
				eeprom_data[2*address+1] = 0x00;
			}
			else
				eeprom_data[address] = 0x00;
		}
		else
		serial_count = 0;
	}
	else if ( (serial_count > (intf->address_bits + intf->data_bits)) &&
						EEPROM_command_match((char*)serial_buffer,intf->cmd_write,(int)strlen((char*)serial_buffer)-(intf->address_bits + intf->data_bits)) )
	{
		int i,address,data;

		address = 0;
		for (i = serial_count-intf->data_bits-intf->address_bits;i < (serial_count-intf->data_bits);i++)
		{
			address <<= 1;
			if (serial_buffer[i] == '1') address |= 1;
		}
		data = 0;
		for (i = serial_count-intf->data_bits;i < serial_count;i++)
		{
			data <<= 1;
			if (serial_buffer[i] == '1') data |= 1;
		}
		if (locked == 0)
		{
			if (intf->data_bits == 16)
			{
				eeprom_data[2*address+0] = data >> 8;
				eeprom_data[2*address+1] = data & 0xff;
			}
			else
				eeprom_data[address] = data;
		}
		else
		serial_count = 0;
	}
	else if ( EEPROM_command_match((char*)serial_buffer,intf->cmd_lock,(int)strlen((char*)serial_buffer)) )
	{
		locked = 1;
		serial_count = 0;
	}
	else if ( EEPROM_command_match((char*)serial_buffer,intf->cmd_unlock,(int)strlen((char*)serial_buffer)) )
	{
		locked = 0;
		serial_count = 0;
	}
}

static void EEPROM_reset()
{
	serial_count = 0;
	sending = 0;
	reset_delay = intf->reset_delay;
}

void EEPROM_set_cs_line(int state)
{
	reset_line = state;

	if (reset_line != CLEAR_LINE)
		EEPROM_reset();
}

void EEPROM_set_clock_line(int state)
{
	if (state == PULSE_LINE || (clock_line == CLEAR_LINE && state != CLEAR_LINE))
	{
		if (reset_line == CLEAR_LINE)
		{
			if (sending)
			{
				if (eeprom_clock_count == intf->data_bits)
				{
					if(intf->enable_multi_read)
					{
						eeprom_read_address = (eeprom_read_address + 1) & ((1 << intf->address_bits) - 1);
						if (intf->data_bits == 16)
							eeprom_data_bits = (eeprom_data[2*eeprom_read_address+0] << 8) + eeprom_data[2*eeprom_read_address+1];
						else
							eeprom_data_bits = eeprom_data[eeprom_read_address];
						eeprom_clock_count = 0;
					}
					else
					{
						sending = 0;
					}
				}
				eeprom_data_bits = (eeprom_data_bits << 1) | 1;
				eeprom_clock_count++;
			}
			else
				EEPROM_write(latch);
		}
	}

	clock_line = state;
}


void EEPROM_write_bit(int bit)
{
	latch = bit;
}

int EEPROM_read_bit(void)
{
	int res;

	if (sending)
		res = (eeprom_data_bits >> intf->data_bits) & 1;
	else
	{
		if (reset_delay > 0)
		{
			reset_delay--;
			res = 0;
		}
		else
			res = 1;
	}

	return res;
}
*/

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg32(0x8000, (reg[0] << 4) | (reg[1] & 0xF));
	setchr8(0);
}

static void StateRestore(int version) {
	WSync();
}

static DECLFR(ReadLow) {
	switch (A & 0x7700) {
	case 0x5100: return reg[2] | reg[0] | reg[1] | reg[3] ^ 0xff; break;
	case 0x5500:
		if (trigger)
			return reg[2] | reg[1];   // Lei Dian Huang Bi Ka Qiu Chuan Shuo (NJ046) may broke other games
		else
			return 0;
	}
	return 4;
}

static void M163HB(void) {
	if (reg[1] & 0x80) {
		if (scanline == 239) {
			setchr4(0x0000, 0);
			setchr4(0x1000, 0);
		} else if (scanline == 127) {
			setchr4(0x0000, 1);
			setchr4(0x1000, 1);
		}
/*
			if(scanline>=127)     // Hu Lu Jin Gang (NJ039) (Ch) [!] don't like it
			{
				setchr4(0x0000,1);
				setchr4(0x1000,1);
			}
			else
			{
				setchr4(0x0000,0);
				setchr4(0x1000,0);
			}
*/
	}
}

static DECLFW(Write) {
	switch (A & 0x7300) {
	case 0x5100: reg[0] = V; WSync(); break;
	case 0x5000: reg[1] = V; WSync(); break;
	case 0x5300: reg[2] = V; break;
	case 0x5200: reg[3] = V; WSync(); break;
	}
}

static void Power(void) {
	memset(reg, 0, 8);
	reg[1] = 0xFF;
	SetWriteHandler(0x5000, 0x5FFF, Write);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	WSync();
}

static void Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void Mapper164_Init(CartInfo *info) {
	info->Power = Power;
	info->Close = Close;
	WSync = Sync;

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

static DECLFW(Write2) {
	if (A == 0x5101) {
		if (laststrobe && !V) {
			trigger ^= 1;
		}
		laststrobe = V;
	} else if (A == 0x5100 && V == 6) //damn thoose protected games
		setprg32(0x8000, 3);
	else
		switch (A & 0x7300) {
		case 0x5200: /*FCEU_printf("%04x %02x (5000 = %02x)\n", A, V, reg[1]); */ reg[0] = V; WSync(); break;
		case 0x5000: reg[1] = V; WSync(); if (!(reg[1] & 0x80) && (scanline < 128)) setchr8(0); /* setchr8(0); */ break;
		case 0x5300: /*FCEU_printf("%04x %02x (5000 = %02x)\n", A, V, reg[1]);*/ reg[2] = V; break;
		case 0x5100: /*FCEU_printf("%04x %02x (5000 = %02x)\n", A, V, reg[1]);*/ reg[3] = V; /*pcmwrite(0x4011, (decode(reg[0]) & 0xf) << 3);*/ WSync(); break;
		}
}

static void Power2(void) {
	memset(reg, 0, 8);
	laststrobe = 1;
	pcmwrite = GetWriteHandler(0x4011);
	SetReadHandler(0x5000, 0x5FFF, ReadLow);
	SetWriteHandler(0x5000, 0x5FFF, Write2);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	WSync();
}

void Mapper163_Init(CartInfo *info) {
	info->Power = Power2;
	info->Close = Close;
	WSync = Sync;
	GameHBIRQHook = M163HB;

//	jedi_table_init();

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}

static void Sync3(void) {
	setchr8(0);
	setprg8r(0x10, 0x6000, 0);
	switch (reg[3] & 7) {
	case 0:
	case 2: setprg32(0x8000, (reg[0] & 0xc) | (reg[1] & 2) | ((reg[2] & 0xf) << 4)); break;
	case 1:
	case 3: setprg32(0x8000, (reg[0] & 0xc) | (reg[2] & 0xf) << 4); break;
	case 4:
	case 6: setprg32(0x8000, (reg[0] & 0xe) | ((reg[1] >> 1) & 1) | ((reg[2] & 0xf) << 4)); break;
	case 5:
	case 7: setprg32(0x8000, (reg[0] & 0xf) | ((reg[2] & 0xf) << 4)); break;
	}
}

static DECLFW(Write3) {
//	FCEU_printf("bs %04x %02x\n",A,V);
	reg[(A >> 8) & 3] = V;
	WSync();
}

static void Power3(void) {
	reg[0] = 3;
	reg[1] = 0;
	reg[2] = 0;
	reg[3] = 7;
	SetWriteHandler(0x5000, 0x5FFF, Write3);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	WSync();
}

void UNLFS304_Init(CartInfo *info) {
	info->Power = Power3;
	info->Close = Close;
	WSync = Sync3;

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}

	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
