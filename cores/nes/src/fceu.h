#ifndef _FCEUH
#define _FCEUH

#include "fceu-types.h"

extern int fceuindbg;

extern unsigned overclock_state;
extern unsigned overclocked;
extern unsigned skip_7bit_overclocking;
extern unsigned DMC_7bit;

extern unsigned normal_scanlines;
extern unsigned extrascanlines;
extern unsigned dendy;

void ResetGameLoaded(void);

#define DECLFR(x) uint8 FP_FASTAPASS(1) x(uint32 A)
#define DECLFW(x) void FP_FASTAPASS(2) x(uint32 A, uint8 V)

void FCEU_MemoryRand(uint8 *ptr, uint32 size);
void FASTAPASS(3) SetReadHandler(int32 start, int32 end, readfunc func);
void FASTAPASS(3) SetWriteHandler(int32 start, int32 end, writefunc func);
writefunc FASTAPASS(1) GetWriteHandler(int32 a);
readfunc FASTAPASS(1) GetReadHandler(int32 a);

int AllocGenieRW(void);
void FlushGenieRW(void);

void FCEU_ResetVidSys(void);

void ResetMapping(void);
void ResetNES(void);
void PowerNES(void);


extern uint64 timestampbase;
extern uint32 MMC5HackVROMMask;
extern uint8 *MMC5HackExNTARAMPtr;
extern int MMC5Hack, PEC586Hack;
extern uint8 *MMC5HackVROMPTR;
extern uint8 MMC5HackCHRMode;
extern uint8 MMC5HackSPMode;
extern uint8 MMC50x5130;
extern uint8 MMC5HackSPScroll;
extern uint8 MMC5HackSPPage;

#ifdef COPYFAMI
extern uint8 RAM[0x4000];
#else
extern uint8 RAM[0x800];
#endif

extern readfunc ARead[0x10000];
extern writefunc BWrite[0x10000];

extern void (*GameInterface)(int h);
extern void (*GameStateRestore)(int version);

#define GI_RESETM2  1
#define GI_POWER  2
#define GI_CLOSE  3

#include "git.h"
extern FCEUGI *GameInfo;
extern int GameAttributes;

extern uint8 PAL;

#include "driver.h"

typedef struct {
	int PAL;
	int NetworkPlay;
	int SoundVolume;
	int GameGenie;

	// Current first and last rendered scanlines.
	int FirstSLine;
	int LastSLine;

	// Driver code(user)-specified first and last rendered scanlines.
	// Usr*SLine[0] is for NTSC, Usr*SLine[1] is for PAL.
	int UsrFirstSLine[2];
	int UsrLastSLine[2];
	int SnapName;
	uint32 SndRate;
	int soundq;
	int lowpass;
} FCEUS;

extern FCEUS FSettings;

void FCEU_PrintError(char *format, ...);
void FCEU_printf(char *format, ...);
void FCEU_DispMessage(char *format, ...);

void SetNESDeemph(uint8 d, int force);
void DrawTextTrans(uint8 *dest, uint32 width, uint8 *textmsg, uint8 fgcolor);
void FCEU_PutImage(void);
#ifdef FRAMESKIP
void FCEU_PutImageDummy(void);
#endif

extern uint8 Exit;
extern uint8 pale;
extern uint8 vsdip;

#define JOY_A        0x01
#define JOY_B        0x02
#define JOY_SELECT   0x04
#define JOY_START    0x08
#define JOY_UP       0x10
#define JOY_DOWN     0x20
#define JOY_LEFT     0x40
#define JOY_RIGHT    0x80

#endif
