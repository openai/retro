/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2003 Xodnizel
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

#include  <string.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <stdarg.h>

#include "fceu.h"
#include  "fceu-types.h"
#include  "x6502.h"
#include  "fceu.h"
#include  "ppu.h"
#include  "sound.h"
#include  "general.h"
#include  "fceu-endian.h"
#include  "fceu-memory.h"

#include  "cart.h"
#include  "nsf.h"
#include  "fds.h"
#include  "ines.h"
#include  "unif.h"
#include  "cheat.h"
#include  "palette.h"
#include  "state.h"
#include  "video.h"
#include  "input.h"
#include  "file.h"
#include  "crc32.h"
#include  "vsuni.h"

uint64 timestampbase;

extern int totalscanlines;

FCEUGI *GameInfo = NULL;
void (*GameInterface)(int h);

void (*GameStateRestore)(int version);

readfunc ARead[0x10000];
writefunc BWrite[0x10000];
static readfunc *AReadG;
static writefunc *BWriteG;
static int RWWrap = 0;

static DECLFW(BNull)
{
}

static DECLFR(ANull)
{
	return(X.DB);
}

int AllocGenieRW(void)
{
	if (!(AReadG = (readfunc*)FCEU_malloc(0x8000 * sizeof(readfunc))))
		return 0;
	if (!(BWriteG = (writefunc*)FCEU_malloc(0x8000 * sizeof(writefunc))))
		return 0;
	RWWrap = 1;
	return 1;
}

void FlushGenieRW(void)
{
   int32 x;

   if (RWWrap)
   {
      for (x = 0; x < 0x8000; x++)
      {
         ARead[x + 0x8000] = AReadG[x];
         BWrite[x + 0x8000] = BWriteG[x];
      }
      free(AReadG);
      free(BWriteG);
      AReadG = 0;
      BWriteG = 0;
   }
   RWWrap = 0;
}

readfunc FASTAPASS(1) GetReadHandler(int32 a)
{
	if (a >= 0x8000 && RWWrap)
		return AReadG[a - 0x8000];
	else
		return ARead[a];
}

void FASTAPASS(3) SetReadHandler(int32 start, int32 end, readfunc func)
{
	int32 x;

	if (!func)
		func = ANull;

	if (RWWrap)
		for (x = end; x >= start; x--)
      {
         if (x >= 0x8000)
            AReadG[x - 0x8000] = func;
         else
            ARead[x] = func;
      }
	else
		for (x = end; x >= start; x--)
			ARead[x] = func;
}

writefunc FASTAPASS(1) GetWriteHandler(int32 a)
{
	if (RWWrap && a >= 0x8000)
		return BWriteG[a - 0x8000];
	else
		return BWrite[a];
}

void FASTAPASS(3) SetWriteHandler(int32 start, int32 end, writefunc func)
{
	int32 x;

	if (!func)
		func = BNull;

	if (RWWrap)
		for (x = end; x >= start; x--)
      {
			if (x >= 0x8000)
				BWriteG[x - 0x8000] = func;
			else
				BWrite[x] = func;
		}
	else
		for (x = end; x >= start; x--)
			BWrite[x] = func;
}

#ifdef COPYFAMI
uint8 RAM[0x4000];
#else
uint8 RAM[0x800];
#endif

uint8 PAL = 0;

static DECLFW(BRAML)
{
	RAM[A] = V;
}

static DECLFR(ARAML)
{
	return RAM[A];
}

#ifndef COPYFAMI
static DECLFW(BRAMH)
{
	RAM[A & 0x7FF] = V;
}

static DECLFR(ARAMH)
{
	return RAM[A & 0x7FF];
}
#endif

void FCEUI_CloseGame(void)
{
	if (!GameInfo)
      return;

   if (GameInfo->name)
      free(GameInfo->name);
   GameInfo->name = 0;
   if (GameInfo->type != GIT_NSF)
      FCEU_FlushGameCheats(0, 0);
   GameInterface(GI_CLOSE);
   ResetExState(0, 0);
   FCEU_CloseGenie();
   free(GameInfo);
   GameInfo = 0;
}

void ResetGameLoaded(void)
{
	if (GameInfo)
      FCEUI_CloseGame();

	GameStateRestore = NULL;
	PPU_hook = NULL;
	GameHBIRQHook = NULL;

	if (GameExpSound.Kill)
		GameExpSound.Kill();
	memset(&GameExpSound, 0, sizeof(GameExpSound));

	MapIRQHook = NULL;
	MMC5Hack = 0;
	PEC586Hack = 0;
	PAL &= 1;
	pale = 0;
}

int UNIFLoad(const char *name, FCEUFILE *fp);
int iNESLoad(const char *name, FCEUFILE *fp);
int FDSLoad(const char *name, FCEUFILE *fp);
int NSFLoad(FCEUFILE *fp);

FCEUGI *FCEUI_LoadGame(const char *name, uint8_t *databuf, size_t databufsize)
{
   FCEUFILE *fp;

   ResetGameLoaded();

   GameInfo = malloc(sizeof(FCEUGI));
   memset(GameInfo, 0, sizeof(FCEUGI));

   GameInfo->soundchan = 0;
   GameInfo->soundrate = 0;
   GameInfo->name = 0;
   GameInfo->type = GIT_CART;
   GameInfo->vidsys = GIV_USER;
   GameInfo->input[0] = GameInfo->input[1] = -1;
   GameInfo->inputfc = -1;
   GameInfo->cspecial = 0;

   FCEU_printf("Loading %s...\n\n", name);

   GetFileBase(name);

   fp = FCEU_fopen(name, NULL, "rb", 0, databuf, databufsize);

   if (!fp) {
      FCEU_PrintError("Error opening \"%s\"!", name);
      return 0;
   }

   if (iNESLoad(name, fp))
      goto endlseq;
   if (NSFLoad(fp))
      goto endlseq;
   if (UNIFLoad(name, fp))
      goto endlseq;
   if (FDSLoad(name, fp))
      goto endlseq;

   FCEU_PrintError("An error occurred while loading the file.\n");
   FCEU_fclose(fp);
   return 0;

endlseq:
   FCEU_fclose(fp);

   FCEU_ResetVidSys();
   if (GameInfo->type != GIT_NSF)
      if (FSettings.GameGenie)
         FCEU_OpenGenie();

   PowerNES();

   if (GameInfo->type != GIT_NSF) {
      FCEU_LoadGamePalette();
      FCEU_LoadGameCheats(0);
   }

   FCEU_ResetPalette();
   FCEU_ResetMessages();	// Save state, status messages, etc.

   return(GameInfo);
}

int CopyFamiLoad(void);

FCEUGI *FCEUI_CopyFamiStart(void)
{
	ResetGameLoaded();

	GameInfo = (FCEUGI*)malloc(sizeof(FCEUGI));
	memset(GameInfo, 0, sizeof(FCEUGI));

	GameInfo->soundchan = 0;
	GameInfo->soundrate = 0;
	GameInfo->name = (uint8_t*)"copyfami";
	GameInfo->type = GIT_CART;
	GameInfo->vidsys = GIV_USER;
	GameInfo->input[0] = GameInfo->input[1] = -1;
	GameInfo->inputfc = -1;
	GameInfo->cspecial = 0;

	FCEU_printf("Starting CopyFamicom...\n\n");

	if (!CopyFamiLoad()) {
		FCEU_PrintError("An error occurred while starting CopyFamicom.");
		return 0;
	}

	FCEU_ResetVidSys();
	if (GameInfo->type != GIT_NSF)
		if (FSettings.GameGenie)
			FCEU_OpenGenie();

	PowerNES();

	if (GameInfo->type != GIT_NSF) {
		FCEU_LoadGamePalette();
		FCEU_LoadGameCheats(0);
	}

	FCEU_ResetPalette();
	FCEU_ResetMessages();	// Save state, status messages, etc.

	return(GameInfo);
}

int FCEUI_Initialize(void) {
	if (!FCEU_InitVirtualVideo())
		return 0;
	memset(&FSettings, 0, sizeof(FSettings));
	FSettings.UsrFirstSLine[0] = 8;
	FSettings.UsrFirstSLine[1] = 0;
	FSettings.UsrLastSLine[0] = 231;
	FSettings.UsrLastSLine[1] = 239;
	FSettings.SoundVolume = 100;
	FCEUPPU_Init();
	X6502_Init();
	return 1;
}

void FCEUI_Kill(void) {
	FCEU_KillVirtualVideo();
	FCEU_KillGenie();
}

void FCEUI_Emulate(uint8 **pXBuf, int32 **SoundBuf, int32 *SoundBufSize, int skip) {
	int r, ssize;

	FCEU_UpdateInput();
	if (geniestage != 1) FCEU_ApplyPeriodicCheats();
	r = FCEUPPU_Loop(skip);

	ssize = FlushEmulateSound();

	timestampbase += timestamp;

	timestamp = 0;

	*pXBuf = skip ? 0 : XBuf;
	*SoundBuf = WaveFinal;
	*SoundBufSize = ssize;
}


void ResetNES(void)
{
	if (!GameInfo)
      return;
	GameInterface(GI_RESETM2);
	FCEUSND_Reset();
	FCEUPPU_Reset();
	X6502_Reset();
}

void FCEU_MemoryRand(uint8 *ptr, uint32 size)
{
	int x = 0;
	while (size) {
//		*ptr = (x & 4) ? 0xFF : 0x00;	// Huang Di DEBUG MODE enabled by default
										// Cybernoid NO MUSIC by default
//		*ptr = (x & 4) ? 0x7F : 0x00;	// Huang Di DEBUG MODE enabled by default
										// Minna no Taabou no Nakayoshi Daisakusen DOESN'T BOOT
										// Cybernoid NO MUSIC by default
//		*ptr = (x & 1) ? 0x55 : 0xAA;	// F-15 Sity War HISCORE is screwed...
										// 1942 SCORE/HISCORE is screwed...
		*ptr = 0xFF;
		x++;
		size--;
		ptr++;
	}
}

void hand(X6502 *X, int type, uint32 A)
{
}

void PowerNES(void)
{
	if (!GameInfo)
      return;

	FCEU_CheatResetRAM();
	FCEU_CheatAddRAM(2, 0, RAM);

	FCEU_GeniePower();

#ifndef COPYFAMI
	FCEU_MemoryRand(RAM, 0x800);
#endif

	SetReadHandler(0x0000, 0xFFFF, ANull);
	SetWriteHandler(0x0000, 0xFFFF, BNull);

#ifdef COPYFAMI
	SetReadHandler(0, 0x3FFF, ARAML);
	SetWriteHandler(0, 0x3FFF, BRAML);
#else
	SetReadHandler(0, 0x7FF, ARAML);
	SetWriteHandler(0, 0x7FF, BRAML);

	SetReadHandler(0x800, 0x1FFF, ARAMH);	/* Part of a little */
	SetWriteHandler(0x800, 0x1FFF, BRAMH);	/* hack for a small speed boost. */
#endif
	InitializeInput();
	FCEUSND_Power();
	FCEUPPU_Power();

	/* Have the external game hardware "powered" after the internal NES stuff.
		Needed for the NSF code and VS System code.
	*/
	GameInterface(GI_POWER);
	if (GameInfo->type == GIT_VSUNI)
		FCEU_VSUniPower();

	timestampbase = 0;
	X6502_Power();
	FCEU_PowerCheats();
}

void FCEU_ResetVidSys(void)
{
	int w;

	if (GameInfo->vidsys == GIV_NTSC)
		w = 0;
	else if (GameInfo->vidsys == GIV_PAL)
   {
      w = 1;
      dendy = 0;
   }
	else
		w = FSettings.PAL;

	PAL = w ? 1 : 0;

   if (PAL)
      dendy = 0;

   normal_scanlines = dendy ? 290 : 240;
   totalscanlines = normal_scanlines + (overclock_state ? extrascanlines : 0);

	FCEUPPU_SetVideoSystem(w || dendy);
	SetSoundVariables();
}

FCEUS FSettings;

void FCEU_printf(char *format, ...)
{
	char temp[2048];

	va_list ap;

	va_start(ap, format);
	vsprintf(temp, format, ap);
	FCEUD_Message(temp);

	va_end(ap);
}

void FCEU_PrintError(char *format, ...)
{
	char temp[2048];

	va_list ap;

	va_start(ap, format);
	vsprintf(temp, format, ap);
	FCEUD_PrintError(temp);

	va_end(ap);
}

void FCEUI_SetRenderedLines(int ntscf, int ntscl, int palf, int pall)
{
	FSettings.UsrFirstSLine[0] = ntscf;
	FSettings.UsrLastSLine[0] = ntscl;
	FSettings.UsrFirstSLine[1] = palf;
	FSettings.UsrLastSLine[1] = pall;
	if (PAL || dendy)
   {
		FSettings.FirstSLine = FSettings.UsrFirstSLine[1];
		FSettings.LastSLine = FSettings.UsrLastSLine[1];
	}
   else
   {
		FSettings.FirstSLine = FSettings.UsrFirstSLine[0];
		FSettings.LastSLine = FSettings.UsrLastSLine[0];
	}
}

void FCEUI_SetVidSystem(int a)
{
	FSettings.PAL = a ? 1 : 0;

	if (!GameInfo)
      return;

   FCEU_ResetVidSys();
   FCEU_ResetPalette();
}

int FCEUI_GetCurrentVidSystem(int *slstart, int *slend)
{
	if (slstart)
		*slstart = FSettings.FirstSLine;
	if (slend)
		*slend = FSettings.LastSLine;
	return(PAL);
}

void FCEUI_SetGameGenie(int a)
{
	FSettings.GameGenie = a ? 1 : 0;
}

void FCEUI_SetSnapName(int a)
{
	FSettings.SnapName = a;
}

int32 FCEUI_GetDesiredFPS(void)
{
	if (PAL || dendy)
		return(838977920);	// ~50.007
	else
		return(1008307711);	// ~60.1
}
