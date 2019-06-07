/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "cart.h"
#include "ppu.h"

#include "ines.h"
#include "unif.h"
#include "state.h"
#include "file.h"
#include "general.h"
#include "fceu-memory.h"
#include "crc32.h"
#include "md5.h"
#include "cheat.h"
#include "vsuni.h"

extern SFORMAT FCEUVSUNI_STATEINFO[];

uint8 *trainerpoo = NULL;
uint8 *ROM = NULL;
uint8 *VROM = NULL;
uint8 *ExtraNTARAM = NULL;
iNES_HEADER head;

CartInfo iNESCart;

uint8 Mirroring = 0;
uint32 ROM_size = 0;
uint32 VROM_size = 0;

static int CHRRAMSize = -1;
static int iNES_Init(int num);

static int MapperNo = 0;

static DECLFR(TrainerRead) {
	return(trainerpoo[A & 0x1FF]);
}

static void iNES_ExecPower() {
	if (iNESCart.Power)
		iNESCart.Power();

	if (trainerpoo) {
		int x;
		for (x = 0; x < 512; x++) {
			X6502_DMW(0x7000 + x, trainerpoo[x]);
			if (X6502_DMR(0x7000 + x) != trainerpoo[x]) {
				SetReadHandler(0x7000, 0x71FF, TrainerRead);
				break;
			}
		}
	}
}

static void iNESGI(int h) {
   switch (h)
   {
      case GI_RESETM2:
         if (iNESCart.Reset)
            iNESCart.Reset();
         break;
      case GI_POWER:
         iNES_ExecPower();
         break;
      case GI_CLOSE:
         if (iNESCart.Close)
            iNESCart.Close();
         if (ROM) {
            free(ROM);
            ROM = NULL;
         }
         if (VROM) {
            free(VROM);
            VROM = NULL;
         }
         if (trainerpoo) {
            free(trainerpoo);
            trainerpoo = NULL;
         }
         if (ExtraNTARAM) {
            free(ExtraNTARAM);
            ExtraNTARAM = NULL;
         }
         break;
   }
}

uint32 iNESGameCRC32 = 0;

struct CRCMATCH {
	uint32 crc;
	char *name;
};

struct INPSEL {
	uint32 crc32;
	int input1;
	int input2;
	int inputfc;
};

static void SetInput(void) {
	static struct INPSEL moo[] =
	{
		{0x19b0a9f1,	SI_GAMEPAD,		SI_ZAPPER,		SIFC_NONE		},	// 6-in-1 (MGC-023)(Unl)[!]
		{0x29de87af,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERB	},	// Aerobics Studio
		{0xd89e5a67,	SI_UNSET,		SI_UNSET,		SIFC_ARKANOID	},	// Arkanoid (J)
		{0x0f141525,	SI_UNSET,		SI_UNSET,		SIFC_ARKANOID	},	// Arkanoid 2(J)
		{0x32fb0583,	SI_UNSET,		SI_ARKANOID,	SIFC_NONE		},	// Arkanoid(NES)
		{0x60ad090a,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERA	},	// Athletic World
		{0x48ca0ee1,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_BWORLD		},	// Barcode World
		{0x4318a2f8,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Barker Bill's Trick Shooting
		{0x6cca1c1f,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERB	},	// Dai Undoukai
		{0x24598791,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Duck Hunt
		{0xd5d6eac4,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// Edu (As)
		{0xe9a7fe9e,	SI_UNSET,		SI_MOUSE,		SIFC_NONE		},	// Educational Computer 2000
		{0x8f7b1669,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// FP BASIC 3.3 by maxzhou88
		{0xf7606810,	SI_UNSET,		SI_UNSET,		SIFC_FKB		},	// Family BASIC 2.0A
		{0x895037bc,	SI_UNSET,		SI_UNSET,		SIFC_FKB		},	// Family BASIC 2.1a
		{0xb2530afc,	SI_UNSET,		SI_UNSET,		SIFC_FKB		},	// Family BASIC 3.0
		{0xea90f3e2,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERB	},	// Family Trainer:  Running Stadium
		{0xbba58be5,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERB	},	// Family Trainer: Manhattan Police
		{0x3e58a87e,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Freedom Force
		{0xd9f45be9,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_QUIZKING	},	// Gimme a Break ...
		{0x1545bd13,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_QUIZKING	},	// Gimme a Break ... 2
		{0x4e959173,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Gotcha! - The Sport!
		{0xbeb8ab01,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Gumshoe
		{0xff24d794,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Hogan's Alley
		{0x21f85681,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_HYPERSHOT	},	// Hyper Olympic (Gentei Ban)
		{0x980be936,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_HYPERSHOT	},	// Hyper Olympic
		{0x915a53a7,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_HYPERSHOT	},	// Hyper Sports
		{0x9fae4d46,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_MAHJONG	},	// Ide Yousuke Meijin no Jissen Mahjong
		{0x7b44fb2a,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_MAHJONG	},	// Ide Yousuke Meijin no Jissen Mahjong 2
		{0x2f128512,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERA	},	// Jogging Race
		{0xbb33196f,	SI_UNSET,		SI_UNSET,		SIFC_FKB		},	// Keyboard Transformer
		{0x8587ee00,	SI_UNSET,		SI_UNSET,		SIFC_FKB		},	// Keyboard Transformer
		{0x543ab532,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// LIKO Color Lines
		{0x368c19a8,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// LIKO Study Cartridge
		{0x5ee6008e,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Mechanized Attack
		{0x370ceb65,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERB	},	// Meiro Dai Sakusen
		{0x3a1694f9,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_4PLAYER	},	// Nekketsu Kakutou Densetsu
		{0x9d048ea4,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_OEKAKIDS	},	// Oeka Kids
		{0x2a6559a1,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Operation Wolf (J)
		{0xedc3662b,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Operation Wolf
		{0x912989dc,	SI_UNSET,		SI_UNSET,		SIFC_FKB		},	// Playbox BASIC
		{0x9044550e,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERA	},	// Rairai Kyonshizu
		{0xea90f3e2,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERB	},	// Running Stadium
		{0x851eb9be,	SI_GAMEPAD,		SI_ZAPPER,		SIFC_NONE		},	// Shooting Range
		{0x6435c095,	SI_GAMEPAD,		SI_POWERPADB,	SIFC_UNSET		},	// Short Order/Eggsplode
		{0xc043a8df,	SI_UNSET,		SI_MOUSE,		SIFC_NONE		},	// Shu Qi Yu - Shu Xue Xiao Zhuan Yuan (Ch)
		{0x2cf5db05,	SI_UNSET,		SI_MOUSE,		SIFC_NONE		},	// Shu Qi Yu - Zhi Li Xiao Zhuan Yuan (Ch)
		{0xad9c63e2,	SI_GAMEPAD,		SI_UNSET,		SIFC_SHADOW		},	// Space Shadow
		{0x61d86167,	SI_GAMEPAD,		SI_POWERPADB,	SIFC_UNSET		},	// Street Cop
		{0xabb2f974,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// Study and Game 32-in-1
		{0x41ef9ac4,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// Subor
		{0x8b265862,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// Subor
		{0x82f1fb96,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// Subor 1.0 Russian
		{0x9f8f200a,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERA	},	// Super Mogura Tataki!! - Pokkun Moguraa
		{0xd74b2719,	SI_GAMEPAD,		SI_POWERPADB,	SIFC_UNSET		},	// Super Team Games
		{0x74bea652,	SI_GAMEPAD,		SI_ZAPPER,		SIFC_NONE		},	// Supergun 3-in-1
		{0x5e073a1b,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// Supor English (Chinese)
		{0x589b6b0d,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// SuporV20
		{0x41401c6d,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	},	// SuporV40
		{0x23d17f5e,	SI_GAMEPAD,		SI_ZAPPER,		SIFC_NONE		},	// The Lone Ranger
		{0xc3c0811d,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_OEKAKIDS	},	// The two "Oeka Kids" games
		{0xde8fd935,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// To the Earth
		{0x47232739,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_TOPRIDER	},	// Top Rider
		{0x8a12a7d9,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_FTRAINERB	},	// Totsugeki Fuuun Takeshi Jou
		{0xb8b9aca3,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Wild Gunman
		{0x5112dc21,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		},	// Wild Gunman
		{0xaf4010ea,	SI_GAMEPAD,		SI_POWERPADB,	SIFC_UNSET		},	// World Class Track Meet
		{0x00000000,	SI_UNSET,		SI_UNSET,		SIFC_UNSET		}
	};
	int x = 0;

	while (moo[x].input1 >= 0 || moo[x].input2 >= 0 || moo[x].inputfc >= 0) {
		if (moo[x].crc32 == iNESGameCRC32) {
			GameInfo->input[0] = moo[x].input1;
			GameInfo->input[1] = moo[x].input2;
			GameInfo->inputfc = moo[x].inputfc;
			break;
		}
		x++;
	}
}

#define INESB_INCOMPLETE  1
#define INESB_CORRUPT     2
#define INESB_HACKED      4

struct BADINF {
	uint64 md5partial;
	uint8 *name;
	uint32 type;
};

static struct BADINF BadROMImages[] =
{
	#include "ines-bad.h"
};

void CheckBad(uint64 md5partial) {
	int32 x = 0;
	while (BadROMImages[x].name) {
		if (BadROMImages[x].md5partial == md5partial) {
			FCEU_PrintError("The copy game you have loaded, \"%s\", is bad, and will not work properly in FCE Ultra.", BadROMImages[x].name);
			return;
		}
		x++;
	}
}


struct CHINF {
	uint32 crc32;
	int32 mapper;
	int32 mirror;
};

static void CheckHInfo(void) {
	/* ROM images that have the battery-backed bit set in the header that really
	don't have battery-backed RAM is not that big of a problem, so I'll
	treat this differently by only listing games that should have battery-backed RAM.

	Lower 64 bits of the MD5 hash.
	*/

	static uint64 savie[] =
	{
		0xc04361e499748382LL,	/* AD&D Heroes of the Lance */
		0xb72ee2337ced5792LL,	/* AD&D Hillsfar */
		0x2b7103b7a27bd72fLL,	/* AD&D Pool of Radiance */
		0x498c10dc463cfe95LL,	/* Battle Fleet */
		0x854d7947a3177f57LL,	/* Crystalis */
		0x4a1f5336b86851b6LL,	/* DW */
		0xb0bcc02c843c1b79LL,	/* DW */
		0x2dcf3a98c7937c22LL,	/* DW 2 */
		0x98e55e09dfcc7533LL,	/* DW 4*/
		0x733026b6b72f2470LL,	/* Dw 3 */
		0x6917ffcaca2d8466LL,	/* Famista '90 */
		0x8da46db592a1fcf4LL,	/* Faria */
		0xedba17a2c4608d20LL,	/* Final Fantasy */
		0x91a6846d3202e3d6LL,	/* Final Fantasy */
		0x012df596e2b31174LL,	/* Final Fantasy 1+2 */
		0xf6b359a720549ecdLL,	/* Final Fantasy 2 */
		0x5a30da1d9b4af35dLL,	/* Final Fantasy 3 */
		0xd63dcc68c2b20adcLL,	/* Final Fantasy J */
		0x2ee3417ba8b69706LL,	/* Hydlide 3*/
		0xebbce5a54cf3ecc0LL,	/* Justbreed */
		0x6a858da551ba239eLL,	/* Kaijuu Monogatari */
		0x2db8f5d16c10b925LL,	/* Kyonshiizu 2 */
		0x04a31647de80fdabLL,	/* Legend of Zelda */
		0x94b9484862a26cbaLL,	/* Legend of Zelda */
		0xa40666740b7d22feLL,	/* Mindseeker */
		0x82000965f04a71bbLL,	/* Mirai Shinwa Jarvas */
		0x77b811b2760104b9LL,	/* Mouryou Senki Madara */
		0x11b69122efe86e8cLL,	/* RPG Jinsei Game */
		0x9aa1dc16c05e7de5LL,	/* Startropics */
		0x1b084107d0878bd0LL,	/* Startropics 2*/
		0xa70b495314f4d075LL,	/* Ys 3 */
		0x836c0ff4f3e06e45LL,	/* Zelda 2 */
		0						/* Abandon all hope if the game has 0 in the lower 64-bits of its MD5 hash */
	};

	static struct CHINF moo[] =
	{
		#include "ines-correct.h"
	};
	int32 tofix = 0, x;
	uint64 partialmd5 = 0;

	for (x = 0; x < 8; x++)
		partialmd5 |= (uint64)iNESCart.MD5[15 - x] << (x * 8);
	CheckBad(partialmd5);

	x = 0;
	do {
		if (moo[x].crc32 == iNESGameCRC32) {
			if (moo[x].mapper >= 0) {
				if (moo[x].mapper & 0x800 && VROM_size) {
					VROM_size = 0;
					free(VROM);
					VROM = NULL;
					tofix |= 8;
				}
				if (MapperNo != (moo[x].mapper & 0xFF)) {
					tofix |= 1;
					MapperNo = moo[x].mapper & 0xFF;
				}
			}
			if (moo[x].mirror >= 0) {
				if (moo[x].mirror == 8) {
					if (Mirroring == 2) {	/* Anything but hard-wired(four screen). */
						tofix |= 2;
						Mirroring = 0;
					}
				} else if (Mirroring != moo[x].mirror) {
					if (Mirroring != (moo[x].mirror & ~4))
						if ((moo[x].mirror & ~4) <= 2)	/* Don't complain if one-screen mirroring
														needs to be set(the iNES header can't
														hold this information).
														*/
							tofix |= 2;
					Mirroring = moo[x].mirror;
				}
			}
			break;
		}
		x++;
	} while (moo[x].mirror >= 0 || moo[x].mapper >= 0);

	x = 0;
	while (savie[x] != 0) {
		if (savie[x] == partialmd5) {
			if (!(head.ROM_type & 2)) {
				tofix |= 4;
				head.ROM_type |= 2;
			}
		}
		x++;
	}

	/* Games that use these iNES mappers tend to have the four-screen bit set
	when it should not be.
	*/
	if ((MapperNo == 118 || MapperNo == 24 || MapperNo == 26) && (Mirroring == 2)) {
		Mirroring = 0;
		tofix |= 2;
	}

	/* Four-screen mirroring implicitly set. */
	if (MapperNo == 99)
		Mirroring = 2;

	if (tofix) {
		char gigastr[768];
		strcpy(gigastr, "The iNES header contains incorrect information.  For now, the information will be corrected in RAM.  ");
		if (tofix & 1)
			sprintf(gigastr + strlen(gigastr), "The mapper number should be set to %d.  ", MapperNo);
		if (tofix & 2) {
			uint8 *mstr[3] = { (uint8_t*)"Horizontal", (uint8_t*)"Vertical", (uint8_t*)"Four-screen" };
			sprintf(gigastr + strlen(gigastr), "Mirroring should be set to \"%s\".  ", mstr[Mirroring & 3]);
		}
		if (tofix & 4)
			strcat(gigastr, "The battery-backed bit should be set.  ");
		if (tofix & 8)
			strcat(gigastr, "This game should not have any CHR ROM.  ");
		strcat(gigastr, "\n");
		FCEU_printf("%s", gigastr);
	}
}

typedef struct {
	int32 mapper;
	void (*init)(CartInfo *);
} NewMI;

typedef struct {
	uint8 *name;
	int32 number;
	void (*init)(CartInfo *);
} BMAPPINGLocal;

static BMAPPINGLocal bmap[] = {
	{(uint8_t*)"NROM",				  0, NROM_Init},
	{(uint8_t*)"MMC1",				  1, Mapper1_Init},
	{(uint8_t*)"UNROM",				  2, UNROM_Init},
	{(uint8_t*)"CNROM",				  3, CNROM_Init},
	{(uint8_t*)"MMC3",				  4, Mapper4_Init},
	{(uint8_t*)"MMC5",				  5, Mapper5_Init},
	{(uint8_t*)"FFE Rev. A",			  6, Mapper6_Init},
	{(uint8_t*)"ANROM",				  7, ANROM_Init},
	{(uint8_t*)"",					  8, Mapper8_Init},		// Nogaems, it's worthless
	{(uint8_t*)"MMC2",				  9, Mapper9_Init},
	{(uint8_t*)"MMC4",				 10, Mapper10_Init},
	{(uint8_t*)"Color Dreams",		 11, Mapper11_Init},
	{(uint8_t*)"REX DBZ 5",			 12, Mapper12_Init},
	{(uint8_t*)"CPROM",				 13, CPROM_Init},
	{(uint8_t*)"REX SL-1632",			 14, UNLSL1632_Init},
	{(uint8_t*)"100-in-1",			 15, Mapper15_Init},
	{(uint8_t*)"BANDAI 24C02",		 16, Mapper16_Init},
	{(uint8_t*)"FFE Rev. B",			 17, Mapper17_Init},
	{(uint8_t*)"JALECO SS880006",		 18, Mapper18_Init},	// JF-NNX (EB89018-30007) boards
	{(uint8_t*)"Namcot 106",			 19, Mapper19_Init},
//	{(uint8_t*)"",					 20, Mapper20_Init},
	{(uint8_t*)"Konami VRC2/VRC4 A",	 21, Mapper21_Init},
	{(uint8_t*)"Konami VRC2/VRC4 B",	 22, Mapper22_Init},
	{(uint8_t*)"Konami VRC2/VRC4 C",	 23, Mapper23_Init},
	{(uint8_t*)"Konami VRC6 Rev. A",	 24, Mapper24_Init},
	{(uint8_t*)"Konami VRC2/VRC4 D",	 25, Mapper25_Init},
	{(uint8_t*)"Konami VRC6 Rev. B",	 26, Mapper26_Init},
	{(uint8_t*)"CC-21 MI HUN CHE",	 27, UNLCC21_Init},		// Former dupe for VRC2/VRC4 mapper, redefined with crc to mihunche boards
//	{(uint8_t*)"",					 28, Mapper28_Init},	// Custom Multidiscrete mapper for PDs
//	{(uint8_t*)"",					 29, Mapper29_Init},
	{(uint8_t*)"UNROM 512",				 30, UNROM512_Init},
//	{(uint8_t*)"",					 31, Mapper31_Init},
	{(uint8_t*)"IREM G-101",			 32, Mapper32_Init},
	{(uint8_t*)"TC0190FMC/TC0350FMR",	 33, Mapper33_Init},
	{(uint8_t*)"IREM I-IM/BNROM",		 34, Mapper34_Init},
	{(uint8_t*)"Wario Land 2",		 35, UNLSC127_Init},
	{(uint8_t*)"TXC Policeman",		 36, Mapper36_Init},
	{(uint8_t*)"PAL-ZZ SMB/TETRIS/NWC",37, Mapper37_Init},
	{(uint8_t*)"Bit Corp.",			 38, Mapper38_Init},	// Crime Busters
//	{(uint8_t*)"",					 39, Mapper39_Init},
	{(uint8_t*)"SMB2j FDS",			 40, Mapper40_Init},
	{(uint8_t*)"CALTRON 6-in-1",		 41, Mapper41_Init},
	{(uint8_t*)"BIO MIRACLE FDS",		 42, Mapper42_Init},
	{(uint8_t*)"FDS SMB2j LF36",		 43, Mapper43_Init},
	{(uint8_t*)"MMC3 BMC PIRATE A",	 44, Mapper44_Init},
	{(uint8_t*)"MMC3 BMC PIRATE B",	 45, Mapper45_Init},
	{(uint8_t*)"RUMBLESTATION 15-in-1",46, Mapper46_Init},
	{(uint8_t*)"NES-QJ SSVB/NWC",		 47, Mapper47_Init},
	{(uint8_t*)"TAITO TCxxx",			 48, Mapper48_Init},
	{(uint8_t*)"MMC3 BMC PIRATE C",	 49, Mapper49_Init},
	{(uint8_t*)"SMB2j FDS Rev. A",	 50, Mapper50_Init},
	{(uint8_t*)"11-in-1 BALL SERIES",	 51, Mapper51_Init},	// 1993 year version
	{(uint8_t*)"MMC3 BMC PIRATE D",	 52, Mapper52_Init},
	{(uint8_t*)"SUPERVISION 16-in-1",	 53, Supervision16_Init},
//	{(uint8_t*)"",					 54, Mapper54_Init},
//	{(uint8_t*)"",					 55, Mapper55_Init},
//	{(uint8_t*)"",					 56, Mapper56_Init},
	{(uint8_t*)"SIMBPLE BMC PIRATE A", 57, Mapper57_Init},
	{(uint8_t*)"SIMBPLE BMC PIRATE B", 58, BMCGK192_Init},
	{(uint8_t*)"",					 59, Mapper59_Init},	// Check this out
	{(uint8_t*)"SIMBPLE BMC PIRATE C", 60, BMCD1038_Init},
	{(uint8_t*)"20-in-1 KAISER Rev. A",61, Mapper61_Init},
	{(uint8_t*)"700-in-1",			 62, Mapper62_Init},
//	{(uint8_t*)"",					 63, Mapper63_Init},
	{(uint8_t*)"TENGEN RAMBO1",		 64, Mapper64_Init},
	{(uint8_t*)"IREM-H3001",			 65, Mapper65_Init},
	{(uint8_t*)"MHROM",				 66, MHROM_Init},
	{(uint8_t*)"SUNSOFT-FZII",		 67, Mapper67_Init},
	{(uint8_t*)"Sunsoft Mapper #4",	 68, Mapper68_Init},
	{(uint8_t*)"SUNSOFT-5/FME-7",		 69, Mapper69_Init},
	{(uint8_t*)"BA KAMEN DISCRETE",	 70, Mapper70_Init},
	{(uint8_t*)"CAMERICA BF9093",		 71, Mapper71_Init},
	{(uint8_t*)"JALECO JF-17",		 72, Mapper72_Init},
	{(uint8_t*)"KONAMI VRC3",			 73, Mapper73_Init},
	{(uint8_t*)"TW MMC3+VRAM Rev. A",	 74, Mapper74_Init},
	{(uint8_t*)"KONAMI VRC1",			 75, Mapper75_Init},
	{(uint8_t*)"NAMCOT 108 Rev. A",	 76, Mapper76_Init},
	{(uint8_t*)"IREM LROG017",		 77, Mapper77_Init},
	{(uint8_t*)"Irem 74HC161/32",		 78, Mapper78_Init},
	{(uint8_t*)"AVE/C&E/TXC BOARD",	 79, Mapper79_Init},
	{(uint8_t*)"TAITO X1-005 Rev. A",	 80, Mapper80_Init},
//	{(uint8_t*)"",					 81, Mapper81_Init},
	{(uint8_t*)"TAITO X1-017",		 82, Mapper82_Init},
	{(uint8_t*)"YOKO VRC Rev. B",		 83, Mapper83_Init},
//	{(uint8_t*)"",					 84, Mapper84_Init},
	{(uint8_t*)"KONAMI VRC7",			 85, Mapper85_Init},
	{(uint8_t*)"JALECO JF-13",		 86, Mapper86_Init},
	{(uint8_t*)"74*139/74 DISCRETE",	 87, Mapper87_Init},
	{(uint8_t*)"NAMCO 3433",			 88, Mapper88_Init},
	{(uint8_t*)"SUNSOFT-3",			 89, Mapper89_Init},	// SUNSOFT-2 mapper
	{(uint8_t*)"HUMMER/JY BOARD",		 90, Mapper90_Init},
	{(uint8_t*)"EARLY HUMMER/JY BOARD",91, Mapper91_Init},
	{(uint8_t*)"JALECO JF-19",		 92, Mapper92_Init},
	{(uint8_t*)"SUNSOFT-3R",			 93, SUNSOFT_UNROM_Init},// SUNSOFT-2 mapper with VRAM, different wiring
	{(uint8_t*)"HVC-UN1ROM",			 94, Mapper94_Init},
	{(uint8_t*)"NAMCOT 108 Rev. B",	 95, Mapper95_Init},
	{(uint8_t*)"BANDAI OEKAKIDS",		 96, Mapper96_Init},
	{(uint8_t*)"IREM TAM-S1",			 97, Mapper97_Init},
//	{(uint8_t*)"",					 98, Mapper98_Init},
	{(uint8_t*)"VS Uni/Dual- system",	 99, Mapper99_Init},
//	{(uint8_t*)"",					100, Mapper100_Init},
	{(uint8_t*)"",					101, Mapper101_Init},
//	{(uint8_t*)"",					102, Mapper102_Init},
	{(uint8_t*)"FDS DOKIDOKI FULL",	103, Mapper103_Init},
	{(uint8_t*)"CAMERICA GOLDENFIVE",	104, Mapper104_Init},
	{(uint8_t*)"NES-EVENT NWC1990",	105, Mapper105_Init},
	{(uint8_t*)"SMB3 PIRATE A",		106, Mapper106_Init},
	{(uint8_t*)"MAGIC CORP A",		107, Mapper107_Init},
	{(uint8_t*)"FDS UNROM BOARD",		108, Mapper108_Init},
//	{(uint8_t*)"",					109, Mapper109_Init},
//	{(uint8_t*)"",					110, Mapper110_Init},
//	{(uint8_t*)"",					111, Mapper111_Init},
	{(uint8_t*)"ASDER/NTDEC BOARD",	112, Mapper112_Init},
	{(uint8_t*)"HACKER/SACHEN BOARD",	113, Mapper113_Init},
	{(uint8_t*)"MMC3 SG PROT. A",		114, Mapper114_Init},
	{(uint8_t*)"MMC3 PIRATE A",		115, Mapper115_Init},
	{(uint8_t*)"MMC1/MMC3/VRC PIRATE",116, UNLSL12_Init},
	{(uint8_t*)"FUTURE MEDIA BOARD",	117, Mapper117_Init},
	{(uint8_t*)"TSKROM",				118, TKSROM_Init},
	{(uint8_t*)"NES-TQROM",			119, Mapper119_Init},
	{(uint8_t*)"FDS TOBIDASE",		120, Mapper120_Init},
	{(uint8_t*)"MMC3 PIRATE PROT. A",	121, Mapper121_Init},
//	{(uint8_t*)"",					122, Mapper122_Init},
	{(uint8_t*)"MMC3 PIRATE H2288",	123, UNLH2288_Init},
//	{(uint8_t*)"",					124, Mapper124_Init},
	{(uint8_t*)"FDS LH32",			125, LH32_Init},
//	{(uint8_t*)"",					126, Mapper126_Init},
//	{(uint8_t*)"",					127, Mapper127_Init},
//	{(uint8_t*)"",					128, Mapper128_Init},
//	{(uint8_t*)"",					129, Mapper129_Init},
//	{(uint8_t*)"",					130, Mapper130_Init},
//	{(uint8_t*)"",					131, Mapper131_Init},
	{(uint8_t*)"TXC/MGENIUS 22111",	132, UNL22211_Init},
	{(uint8_t*)"SA72008",				133, SA72008_Init},
	{(uint8_t*)"MMC3 BMC PIRATE",		134, Mapper134_Init},
//	{(uint8_t*)"",					135, Mapper135_Init},
	{(uint8_t*)"TCU02",				136, TCU02_Init},
	{(uint8_t*)"S8259D",				137, S8259D_Init},
	{(uint8_t*)"S8259B",				138, S8259B_Init},
	{(uint8_t*)"S8259C",				139, S8259C_Init},
	{(uint8_t*)"JALECO JF-11/14",		140, Mapper140_Init},
	{(uint8_t*)"S8259A",				141, S8259A_Init},
	{(uint8_t*)"UNLKS7032",			142, UNLKS7032_Init},
	{(uint8_t*)"TCA01",				143, TCA01_Init},
	{(uint8_t*)"AGCI 50282",			144, Mapper144_Init},
	{(uint8_t*)"SA72007",				145, SA72007_Init},
	{(uint8_t*)"SA0161M",				146, SA0161M_Init},
	{(uint8_t*)"TCU01",				147, TCU01_Init},
	{(uint8_t*)"SA0037",				148, SA0037_Init},
	{(uint8_t*)"SA0036",				149, SA0036_Init},
	{(uint8_t*)"S74LS374N",			150, S74LS374N_Init},
	{(uint8_t*)"",					151, Mapper151_Init},
	{(uint8_t*)"",					152, Mapper152_Init},
	{(uint8_t*)"BANDAI SRAM",			153, Mapper153_Init},	// Bandai board 16 with SRAM instead of EEPROM
	{(uint8_t*)"",					154, Mapper154_Init},
	{(uint8_t*)"",					155, Mapper155_Init},
	{(uint8_t*)"",					156, Mapper156_Init},
	{(uint8_t*)"BANDAI BARCODE",		157, Mapper157_Init},
	{(uint8_t*)"TENGEN 800037",		158, Mapper158_Init},
	{(uint8_t*)"BANDAI 24C01",		159, Mapper159_Init},	// Different type of EEPROM on the  bandai board
	{(uint8_t*)"SA009",				160, SA009_Init},
//	{(uint8_t*)"",					161, Mapper161_Init},
	{(uint8_t*)"",					162, UNLFS304_Init},
	{(uint8_t*)"",					163, Mapper163_Init},
	{(uint8_t*)"",					164, Mapper164_Init},
	{(uint8_t*)"",					165, Mapper165_Init},
	{(uint8_t*)"SUBOR Rev. A",		166, Mapper166_Init},
	{(uint8_t*)"SUBOR Rev. B",		167, Mapper167_Init},
	{(uint8_t*)"",					168, Mapper168_Init},
//	{(uint8_t*)"",					169, Mapper169_Init},
	{(uint8_t*)"",					170, Mapper170_Init},
	{(uint8_t*)"",					171, Mapper171_Init},
	{(uint8_t*)"",					172, Mapper172_Init},
	{(uint8_t*)"",					173, Mapper173_Init},
//	{(uint8_t*)"",					174, Mapper174_Init},
	{(uint8_t*)"",					175, Mapper175_Init},
	{(uint8_t*)"BMCFK23C",			176, BMCFK23C_Init},	// zero 26-may-2012 - well, i have some WXN junk games that use 176 for instance ????. i dont know what game uses this BMCFK23C as mapper 176. we'll have to make a note when we find it.
	{(uint8_t*)"",					177, Mapper177_Init},
	{(uint8_t*)"",					178, Mapper178_Init},
//	{(uint8_t*)"",					179, Mapper179_Init},
	{(uint8_t*)"",					180, Mapper180_Init},
	{(uint8_t*)"",					181, Mapper181_Init},
//	{(uint8_t*)"",					182, Mapper182_Init},	// Deprecated, dupe
	{(uint8_t*)"",					183, Mapper183_Init},
	{(uint8_t*)"",					184, Mapper184_Init},
	{(uint8_t*)"",					185, Mapper185_Init},
	{(uint8_t*)"",					186, Mapper186_Init},
	{(uint8_t*)"",					187, Mapper187_Init},
	{(uint8_t*)"",					188, Mapper188_Init},
	{(uint8_t*)"",					189, Mapper189_Init},
	{(uint8_t*)"",					190, Mapper190_Init},
	{(uint8_t*)"",					191, Mapper191_Init},
	{(uint8_t*)"TW MMC3+VRAM Rev. B",	192, Mapper192_Init},
	{(uint8_t*)"NTDEC TC-112",		193, Mapper193_Init},	// War in the Gulf
	{(uint8_t*)"TW MMC3+VRAM Rev. C",	194, Mapper194_Init},
	{(uint8_t*)"TW MMC3+VRAM Rev. D",	195, Mapper195_Init},
	{(uint8_t*)"",					196, Mapper196_Init},
	{(uint8_t*)"",					197, Mapper197_Init},
	{(uint8_t*)"TW MMC3+VRAM Rev. E",	198, Mapper198_Init},
	{(uint8_t*)"",					199, Mapper199_Init},
	{(uint8_t*)"",					200, Mapper200_Init},
	{(uint8_t*)"",					201, Mapper201_Init},
	{(uint8_t*)"",					202, Mapper202_Init},
	{(uint8_t*)"",					203, Mapper203_Init},
	{(uint8_t*)"",					204, Mapper204_Init},
	{(uint8_t*)"",					205, Mapper205_Init},
	{(uint8_t*)"NAMCOT 108 Rev. C",	206, Mapper206_Init},	// Deprecated, Used to be "DEIROM" whatever it means, but actually simple version of MMC3
	{(uint8_t*)"TAITO X1-005 Rev. B",	207, Mapper207_Init},
	{(uint8_t*)"",					208, Mapper208_Init},
	{(uint8_t*)"HUMMER/JY BOARD",	209, Mapper209_Init},
	{(uint8_t*)"",					210, Mapper210_Init},
	{(uint8_t*)"HUMMER/JY BOARD",	211, Mapper211_Init},
	{(uint8_t*)"",					212, Mapper212_Init},
	{(uint8_t*)"",					213, Mapper213_Init},
	{(uint8_t*)"",					214, Mapper214_Init},
	{(uint8_t*)"",					215, UNL8237_Init},
	{(uint8_t*)"",					216, Mapper216_Init},
	{(uint8_t*)"",					217, Mapper217_Init},	// Redefined to a new Discrete BMC mapper
//	{(uint8_t*)"",					218, Mapper218_Init},
	{(uint8_t*)"UNLA9746",			219, UNLA9746_Init},
	{(uint8_t*)"Debug Mapper",		220, UNLKS7057_Init},
	{(uint8_t*)"UNLN625092",			221, UNLN625092_Init},
	{(uint8_t*)"",					222, Mapper222_Init},
//	{(uint8_t*)"",					223, Mapper223_Init},
//	{(uint8_t*)"",					224, Mapper224_Init},
	{(uint8_t*)"",					225, Mapper225_Init},
	{(uint8_t*)"BMC 22+20-in-1",		226, Mapper226_Init},
	{(uint8_t*)"",					227, Mapper227_Init},
	{(uint8_t*)"",					228, Mapper228_Init},
	{(uint8_t*)"",					229, Mapper229_Init},
	{(uint8_t*)"BMC Contra+22-in-1",	230, Mapper230_Init},
	{(uint8_t*)"",					231, Mapper231_Init},
	{(uint8_t*)"BMC QUATTRO",			232, Mapper232_Init},
	{(uint8_t*)"BMC 22+20-in-1 RST",	233, Mapper233_Init},
	{(uint8_t*)"BMC MAXI",			234, Mapper234_Init},
	{(uint8_t*)"",					235, Mapper235_Init},
//	{(uint8_t*)"",					236, Mapper236_Init},
//	{(uint8_t*)"",					237, Mapper237_Init},
	{(uint8_t*)"UNL6035052",			238, UNL6035052_Init},
//	{(uint8_t*)"",					239, Mapper239_Init},
	{(uint8_t*)"",					240, Mapper240_Init},
	{(uint8_t*)"",					241, Mapper241_Init},
	{(uint8_t*)"",					242, Mapper242_Init},
	{(uint8_t*)"S74LS374NA",			243, S74LS374NA_Init},
	{(uint8_t*)"DECATHLON",			244, Mapper244_Init},
	{(uint8_t*)"",					245, Mapper245_Init},
	{(uint8_t*)"FONG SHEN BANG",		246, Mapper246_Init},
//	{(uint8_t*)"",					247, Mapper247_Init},
//	{(uint8_t*)"",					248, Mapper248_Init},
	{(uint8_t*)"",					249, Mapper249_Init},
	{(uint8_t*)"",					250, Mapper250_Init},
//	{(uint8_t*)"",					251, Mapper251_Init},	// No good dumps for this mapper, use UNIF version
	{(uint8_t*)"SAN GUO ZHI PIRATE",	252, Mapper252_Init},
	{(uint8_t*)"DRAGON BALL PIRATE",	253, Mapper253_Init},
	{(uint8_t*)"",					254, Mapper254_Init},
//	{(uint8_t*)"",					255, Mapper255_Init},	// No good dumps for this mapper
	{(uint8_t*)"",					0, NULL}
};

int iNESLoad(const char *name, FCEUFILE *fp) {
	struct md5_context md5;
	 char* mappername;
	 uint32 mappertest;

	if (FCEU_fread(&head, 1, 16, fp) != 16)
		return 0;

	if (memcmp(&head, "NES\x1a", 4))
		return 0;

	memset(&iNESCart, 0, sizeof(iNESCart));

	if (!memcmp((char*)(&head) + 0x7, "DiskDude", 8)) {
		memset((char*)(&head) + 0x7, 0, 0x9);
	}

	if (!memcmp((char*)(&head) + 0x7, "demiforce", 9)) {
		memset((char*)(&head) + 0x7, 0, 0x9);
	}

	if (!memcmp((char*)(&head) + 0xA, "Ni03", 4)) {
		if (!memcmp((char*)(&head) + 0x7, "Dis", 3))
			memset((char*)(&head) + 0x7, 0, 0x9);
		else
			memset((char*)(&head) + 0xA, 0, 0x6);
	}

	MapperNo = (head.ROM_type >> 4);
	MapperNo |= (head.ROM_type2 & 0xF0);
	if (head.ROM_type & 8) {
		Mirroring = 2;
	} else
		Mirroring = (head.ROM_type & 1);

	if (!head.ROM_size)
		ROM_size = 256;
	else
		ROM_size = uppow2(head.ROM_size);

	VROM_size = uppow2(head.VROM_size);


	if ((ROM = (uint8*)FCEU_malloc(ROM_size << 14)) == NULL)
		return 0;
	memset(ROM, 0xFF, ROM_size << 14);

	if (VROM_size) {
		if ((VROM = (uint8*)FCEU_malloc(VROM_size << 13)) == NULL) {
			free(ROM);
			ROM = NULL;
			return 0;
		}
		memset(VROM, 0xFF, VROM_size << 13);
	}

	if (head.ROM_type & 4) {	/* Trainer */
		trainerpoo = (uint8*)FCEU_gmalloc(512);
		FCEU_fread(trainerpoo, 512, 1, fp);
	}

	ResetCartMapping();
	ResetExState(0, 0);

	SetupCartPRGMapping(0, ROM, ROM_size << 14, 0);

	if (head.ROM_size)
		FCEU_fread(ROM, 0x4000, head.ROM_size, fp);
	else
		FCEU_fread(ROM, 0x4000, ROM_size, fp);

	if (VROM_size)
		FCEU_fread(VROM, 0x2000, VROM_size, fp);

	md5_starts(&md5);
	if (head.ROM_size) {
		md5_update(&md5, ROM, head.ROM_size << 14);
		iNESGameCRC32 = CalcCRC32(0, ROM, head.ROM_size << 14);
	} else {
		md5_update(&md5, ROM, ROM_size << 14);
		iNESGameCRC32 = CalcCRC32(0, ROM, ROM_size << 14);
	}

	if (VROM_size) {
		iNESGameCRC32 = CalcCRC32(iNESGameCRC32, VROM, VROM_size << 13);
		md5_update(&md5, VROM, VROM_size << 13);
	}
	md5_finish(&md5, iNESCart.MD5);
	memcpy(&GameInfo->MD5, &iNESCart.MD5, sizeof(iNESCart.MD5));

	iNESCart.CRC32 = iNESGameCRC32;

	FCEU_printf(" PRG ROM:  %3d x 16KiB\n", head.ROM_size ?  head.ROM_size : 256);
	FCEU_printf(" CHR ROM:  %3d x  8KiB\n", head.VROM_size);
	FCEU_printf(" ROM CRC32:  0x%08lx\n", iNESGameCRC32);
	FCEU_printf(" ROM MD5:  0x%s\n", md5_asciistr(iNESCart.MD5));
	mappername = "Not Listed";

	for (mappertest = 0; mappertest < (sizeof bmap / sizeof bmap[0]) - 1; mappertest++) {
		if (bmap[mappertest].number == MapperNo) {
			mappername = (char*)bmap[mappertest].name;
			break;
		}
	}

	FCEU_printf(" Mapper #:  %d\n", MapperNo);
	FCEU_printf(" Mapper name: %s\n", mappername);
	FCEU_printf(" Mirroring: %s\n", Mirroring == 2 ? "None (Four-screen)" : Mirroring ? "Vertical" : "Horizontal");
	FCEU_printf(" Battery-backed: %s\n", (head.ROM_type & 2) ? "Yes" : "No");
	FCEU_printf(" Trained: %s\n", (head.ROM_type & 4) ? "Yes" : "No");

	SetInput();
	CheckHInfo();
	{
		int x;
		uint64 partialmd5 = 0;

		for (x = 0; x < 8; x++) {
			partialmd5 |= (uint64)iNESCart.MD5[7 - x] << (x * 8);
		}

		FCEU_VSUniCheck(partialmd5, &MapperNo, &Mirroring);
	}
	/* Must remain here because above functions might change value of
	VROM_size and free(VROM).
	*/
	if (VROM_size)
		SetupCartCHRMapping(0, VROM, VROM_size * 0x2000, 0);

	if (Mirroring == 2) {
		ExtraNTARAM = (uint8*)FCEU_gmalloc(2048);
		SetupCartMirroring(4, 1, ExtraNTARAM);
	} else if (Mirroring >= 0x10)
		SetupCartMirroring(2 + (Mirroring & 1), 1, 0);
	else
		SetupCartMirroring(Mirroring & 1, (Mirroring & 4) >> 2, 0);

	iNESCart.battery = (head.ROM_type & 2) ? 1 : 0;
	iNESCart.mirror = Mirroring;

	if (!iNES_Init(MapperNo))
		FCEU_PrintError("iNES mapper #%d is not supported at all.", MapperNo);

	GameInterface = iNESGI;
	FCEU_printf("\n");

	if (strstr(name, "(E)") || strstr(name, "(e)")
		|| strstr(name, "(Europe)") || strstr(name, "(PAL)")
		|| strstr(name, "(F)") || strstr(name, "(f)")
		|| strstr(name, "(G)") || strstr(name, "(g)")
		|| strstr(name, "(I)") || strstr(name, "(i)")
		|| strstr(name, "(S)") || strstr(name, "(s)")
		|| strstr(name, "(France)") || strstr(name, "(Germany)")
		|| strstr(name, "(Italy)") || strstr(name, "(Spain)")
		|| strstr(name, "(Sweden)") || strstr(name, "(Sw)")
		|| strstr(name, "(Australia)") || strstr(name, "(A)")
		|| strstr(name, "(a)")) {
		GameInfo->vidsys = GIV_PAL;
	}

	return 1;
}

static int iNES_Init(int num) {
	BMAPPINGLocal *tmp = bmap;

	CHRRAMSize = -1;

	if (GameInfo->type == GIT_VSUNI)
		AddExState(FCEUVSUNI_STATEINFO, ~0, 0, 0);

	while (tmp->init) {
		if (num == tmp->number) {
			UNIFchrrama = 0;	// need here for compatibility with UNIF mapper code
			if (!VROM_size) {
				switch (num) {	// FIXME, mapper or game data base with the board parameters and ROM/RAM sizes
				case 13:  CHRRAMSize = 16 * 1024; break;
				case 6:
				case 30:
				case 45:
				case 96:  CHRRAMSize = 32 * 1024; break;
				case 176: CHRRAMSize = 128 * 1024; break;
				default:  CHRRAMSize = 8 * 1024; break;
				}
				iNESCart.vram_size = CHRRAMSize;
				if ((VROM = (uint8*)malloc(CHRRAMSize)) == NULL) return 0;
				FCEU_MemoryRand(VROM, CHRRAMSize);
				UNIFchrrama = VROM;
				SetupCartCHRMapping(0, VROM, CHRRAMSize, 1);
				AddExState(VROM, CHRRAMSize, 0, "CHRR");
			}
			if (head.ROM_type & 8)
				AddExState(ExtraNTARAM, 2048, 0, "EXNR");
			tmp->init(&iNESCart);
			return 1;
		}
		tmp++;
	}
	return 0;
}
