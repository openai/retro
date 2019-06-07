#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "libretro.h"

#include "../../fceu.h"
#include "../../fceu-endian.h"
#include "../../input.h"
#include "../../state.h"
#include "../../ppu.h"
#include "../../cart.h"
#include "../../x6502.h"
#include "../../git.h"
#include "../../palette.h"
#include "../../sound.h"
#include "../../file.h"
#include "../../cheat.h"
#include "../../ines.h"
#include "../../unif.h"
#include "../../fds.h"
#include "../../vsuni.h"
#include "../../video.h"

#include "libretro-common/include/streams/memory_stream.h"

#define NES_8_7_PAR (width * (8.0 / 7.0)) / height
#define NES_4_3 4.0 / 3.0

#if defined(_3DS)
void* linearMemAlign(size_t size, size_t alignment);
void linearFree(void* mem);
#endif

static retro_video_refresh_t video_cb = NULL;
static retro_input_poll_t poll_cb = NULL;
static retro_input_state_t input_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_environment_t environ_cb = NULL;
static bool use_overscan;
static bool use_raw_palette;
static bool use_par;
int turbo_enabler;
int turbo_delay;
static int regionoverride = -1;

/* emulator-specific variables */

/* overclock the console by adding dummy scanlines to PPU loop
 * disables DMC DMA and WaveHi filling for these dummies
 * doesn't work with new PPU */
unsigned overclocked = 0;
/* 7-bit samples have priority over overclocking */
unsigned skip_7bit_overclocking = 1;
unsigned normal_scanlines = 240;
unsigned extrascanlines = 0;
unsigned overclock_state = -1;
unsigned dendy = 0;


int FCEUnetplay;
#ifdef PSP
#include "pspgu.h"
static __attribute__((aligned(16))) uint16_t retro_palette[256];
#else
static uint16_t retro_palette[256];
#endif
static uint16_t* fceu_video_out;


/* Some timing-related variables. */
static int soundo = 1;

static volatile int nofocus = 0;

static int32_t *sound = 0;
static uint32_t JSReturn[2];
static uint32_t current_palette = 0;

int PPUViewScanline=0;
int PPUViewer=0;

/* extern forward decls.*/
extern FCEUGI *GameInfo;
extern uint8 *XBuf;
extern CartInfo iNESCart;
extern CartInfo UNIFCart;

/* emulator-specific callback functions */

void UpdatePPUView(int refreshchr) { }

const char * GetKeyboard(void)
{
   return "";
}

int FCEUD_SendData(void *data, uint32 len)
{
   return 1;
}

#define BUILD_PIXEL_RGB565(R,G,B) (((int) ((R)&0x1f) << RED_SHIFT) | ((int) ((G)&0x3f) << GREEN_SHIFT) | ((int) ((B)&0x1f) << BLUE_SHIFT))

#if defined (PSP)
#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 11
#define RED_EXPAND 3
#define GREEN_EXPAND 2
#define BLUE_EXPAND 3
#elif defined (FRONTEND_SUPPORTS_RGB565)
#define RED_SHIFT 11
#define GREEN_SHIFT 5
#define BLUE_SHIFT 0
#define RED_EXPAND 3
#define GREEN_EXPAND 2
#define BLUE_EXPAND 3
#define RED_MASK 0xF800
#define GREEN_MASK 0x7e0
#define BLUE_MASK 0x1f
#else
#define RED_SHIFT 10
#define GREEN_SHIFT 5
#define BLUE_SHIFT 0
#define RED_EXPAND 3
#define GREEN_EXPAND 3
#define BLUE_EXPAND 3
#endif


void FCEUD_SetPalette(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
#ifdef FRONTEND_SUPPORTS_RGB565
   retro_palette[index] = BUILD_PIXEL_RGB565(r >> RED_EXPAND, g >> GREEN_EXPAND, b >> BLUE_EXPAND);
#else
   retro_palette[index] =
      ((r >> RED_EXPAND) << RED_SHIFT) | ((g >> GREEN_EXPAND) << GREEN_SHIFT) | ((b >> BLUE_EXPAND) << BLUE_SHIFT);
#endif
}

void FCEUD_GetPalette(unsigned char i, unsigned char *r, unsigned char *g, unsigned char *b)
{
}


bool FCEUD_ShouldDrawInputAids (void)
{
   return 1;
}

static struct retro_log_callback log_cb;

static void default_logger(enum retro_log_level level, const char *fmt, ...) {}

void FCEUD_PrintError(char *c)
{
   log_cb.log(RETRO_LOG_WARN, "%s", c);
}

void FCEUD_Message(char *s)
{
   log_cb.log(RETRO_LOG_INFO, "%s", s);
}

void FCEUD_DispMessage(char *m)
{  struct retro_message msg;
   msg.msg = m;
   msg.frames = 180;
   environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
}

void FCEUD_NetworkClose(void)
{ }

void FCEUD_SoundToggle (void)
{
   FCEUI_SetSoundVolume(100);
}

void FCEUD_VideoChanged (void)
{ }

FILE *FCEUD_UTF8fopen(const char *n, const char *m)
{
   if (n)
      return fopen(n, m);
   else
      return NULL;
}

#define MAX_PATH 1024

/*palette for FCEU*/
#define MAXPAL 25

struct st_palettes {
	char name[32];
	char desc[32];
	unsigned int data[64];
};


struct st_palettes palettes[] = {
   { "asqrealc", "AspiringSquire's Real palette",
      { 0x6c6c6c, 0x00268e, 0x0000a8, 0x400094,
	      0x700070, 0x780040, 0x700000, 0x621600,
	      0x442400, 0x343400, 0x005000, 0x004444,
	      0x004060, 0x000000, 0x101010, 0x101010,
	      0xbababa, 0x205cdc, 0x3838ff, 0x8020f0,
	      0xc000c0, 0xd01474, 0xd02020, 0xac4014,
	      0x7c5400, 0x586400, 0x008800, 0x007468,
	      0x00749c, 0x202020, 0x101010, 0x101010,
	      0xffffff, 0x4ca0ff, 0x8888ff, 0xc06cff,
	      0xff50ff, 0xff64b8, 0xff7878, 0xff9638,
	      0xdbab00, 0xa2ca20, 0x4adc4a, 0x2ccca4,
	      0x1cc2ea, 0x585858, 0x101010, 0x101010,
	      0xffffff, 0xb0d4ff, 0xc4c4ff, 0xe8b8ff,
	      0xffb0ff, 0xffb8e8, 0xffc4c4, 0xffd4a8,
	      0xffe890, 0xf0f4a4, 0xc0ffc0, 0xacf4f0,
	      0xa0e8ff, 0xc2c2c2, 0x202020, 0x101010 }
	},
   { "loopy", "Loopy's palette",
	   { 0x757575, 0x271b8f, 0x0000ab, 0x47009f,
		   0x8f0077, 0xab0013, 0xa70000, 0x7f0b00,
		   0x432f00, 0x004700, 0x005100, 0x003f17,
		   0x1b3f5f, 0x000000, 0x000000, 0x000000,
		   0xbcbcbc, 0x0073ef, 0x233bef, 0x8300f3,
		   0xbf00bf, 0xe7005b, 0xdb2b00, 0xcb4f0f,
		   0x8b7300, 0x009700, 0x00ab00, 0x00933b,
		   0x00838b, 0x000000, 0x000000, 0x000000,
		   0xffffff, 0x3fbfff, 0x5f97ff, 0xa78bfd,
		   0xf77bff, 0xff77b7, 0xff7763, 0xff9b3b,
		   0xf3bf3f, 0x83d313, 0x4fdf4b, 0x58f898,
		   0x00ebdb, 0x000000, 0x000000, 0x000000,
		   0xffffff, 0xabe7ff, 0xc7d7ff, 0xd7cbff,
		   0xffc7ff, 0xffc7db, 0xffbfb3, 0xffdbab,
		   0xffe7a3, 0xe3ffa3, 0xabf3bf, 0xb3ffcf,
		   0x9ffff3, 0x000000, 0x000000, 0x000000 }
   },
   { "quor", "Quor's palette",
	   { 0x3f3f3f, 0x001f3f, 0x00003f, 0x1f003f,
		   0x3f003f, 0x3f0020, 0x3f0000, 0x3f2000,
		   0x3f3f00, 0x203f00, 0x003f00, 0x003f20,
		   0x003f3f, 0x000000, 0x000000, 0x000000,
		   0x7f7f7f, 0x405f7f, 0x40407f, 0x5f407f,
		   0x7f407f, 0x7f4060, 0x7f4040, 0x7f6040,
		   0x7f7f40, 0x607f40, 0x407f40, 0x407f60,
		   0x407f7f, 0x000000, 0x000000, 0x000000,
		   0xbfbfbf, 0x809fbf, 0x8080bf, 0x9f80bf,
		   0xbf80bf, 0xbf80a0, 0xbf8080, 0xbfa080,
		   0xbfbf80, 0xa0bf80, 0x80bf80, 0x80bfa0,
		   0x80bfbf, 0x000000, 0x000000, 0x000000,
		   0xffffff, 0xc0dfff, 0xc0c0ff, 0xdfc0ff,
		   0xffc0ff, 0xffc0e0, 0xffc0c0, 0xffe0c0,
		   0xffffc0, 0xe0ffc0, 0xc0ffc0, 0xc0ffe0,
		   0xc0ffff, 0x000000, 0x000000, 0x000000 }
   },
   { "chris", "Chris Covell's palette",
	   { 0x808080, 0x003DA6, 0x0012B0, 0x440096,
		   0xA1005E, 0xC70028, 0xBA0600, 0x8C1700,
		   0x5C2F00, 0x104500, 0x054A00, 0x00472E,
		   0x004166, 0x000000, 0x050505, 0x050505,
		   0xC7C7C7, 0x0077FF, 0x2155FF, 0x8237FA,
		   0xEB2FB5, 0xFF2950, 0xFF2200, 0xD63200,
		   0xC46200, 0x358000, 0x058F00, 0x008A55,
		   0x0099CC, 0x212121, 0x090909, 0x090909,
		   0xFFFFFF, 0x0FD7FF, 0x69A2FF, 0xD480FF,
		   0xFF45F3, 0xFF618B, 0xFF8833, 0xFF9C12,
		   0xFABC20, 0x9FE30E, 0x2BF035, 0x0CF0A4,
		   0x05FBFF, 0x5E5E5E, 0x0D0D0D, 0x0D0D0D,
		   0xFFFFFF, 0xA6FCFF, 0xB3ECFF, 0xDAABEB,
		   0xFFA8F9, 0xFFABB3, 0xFFD2B0, 0xFFEFA6,
		   0xFFF79C, 0xD7E895, 0xA6EDAF, 0xA2F2DA,
		   0x99FFFC, 0xDDDDDD, 0x111111, 0x111111 }
   },
   { "matt", "Matthew Conte's palette",
	   { 0x808080, 0x0000bb, 0x3700bf, 0x8400a6,
		   0xbb006a, 0xb7001e, 0xb30000, 0x912600,
		   0x7b2b00, 0x003e00, 0x00480d, 0x003c22,
		   0x002f66, 0x000000, 0x050505, 0x050505,
		   0xc8c8c8, 0x0059ff, 0x443cff, 0xb733cc,
		   0xff33aa, 0xff375e, 0xff371a, 0xd54b00,
		   0xc46200, 0x3c7b00, 0x1e8415, 0x009566,
		   0x0084c4, 0x111111, 0x090909, 0x090909,
		   0xffffff, 0x0095ff, 0x6f84ff, 0xd56fff,
		   0xff77cc, 0xff6f99, 0xff7b59, 0xff915f,
		   0xffa233, 0xa6bf00, 0x51d96a, 0x4dd5ae,
		   0x00d9ff, 0x666666, 0x0d0d0d, 0x0d0d0d,
		   0xffffff, 0x84bfff, 0xbbbbff, 0xd0bbff,
		   0xffbfea, 0xffbfcc, 0xffc4b7, 0xffccae,
		   0xffd9a2, 0xcce199, 0xaeeeb7, 0xaaf7ee,
		   0xb3eeff, 0xdddddd, 0x111111, 0x111111 }
   },
   { "pasofami", "PasoFami/99 palette",
	   { 0x7f7f7f, 0x0000ff, 0x0000bf, 0x472bbf,
		   0x970087, 0xab0023, 0xab1300, 0x8b1700,
		   0x533000, 0x007800, 0x006b00, 0x005b00,
		   0x004358, 0x000000, 0x000000, 0x000000,
		   0xbfbfbf, 0x0078f8, 0x0058f8, 0x6b47ff,
		   0xdb00cd, 0xe7005b, 0xf83800, 0xe75f13,
		   0xaf7f00, 0x00b800, 0x00ab00, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x000000,
		   0xf8f8f8, 0x3fbfff, 0x6b88ff, 0x9878f8,
		   0xf878f8, 0xf85898, 0xf87858, 0xffa347,
		   0xf8b800, 0xb8f818, 0x5bdb57, 0x58f898,
		   0x00ebdb, 0x787878, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0xb8b8f8, 0xd8b8f8,
		   0xf8b8f8, 0xfba7c3, 0xf0d0b0, 0xffe3ab,
		   0xfbdb7b, 0xd8f878, 0xb8f8b8, 0xb8f8d8,
		   0x00ffff, 0xf8d8f8, 0x000000, 0x000000 }
   },
   { "crashman", "CrashMan's palette",
	   { 0x585858, 0x001173, 0x000062, 0x472bbf,
		   0x970087, 0x910009, 0x6f1100, 0x4c1008,
		   0x371e00, 0x002f00, 0x005500, 0x004d15,
		   0x002840, 0x000000, 0x000000, 0x000000,
		   0xa0a0a0, 0x004499, 0x2c2cc8, 0x590daa,
		   0xae006a, 0xb00040, 0xb83418, 0x983010,
		   0x704000, 0x308000, 0x207808, 0x007b33,
		   0x1c6888, 0x000000, 0x000000, 0x000000,
		   0xf8f8f8, 0x267be1, 0x5870f0, 0x9878f8,
		   0xff73c8, 0xf060a8, 0xd07b37, 0xe09040,
		   0xf8b300, 0x8cbc00, 0x40a858, 0x58f898,
		   0x00b7bf, 0x787878, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0xb8b8f8, 0xd8b8f8,
		   0xe6a6ff, 0xf29dc4, 0xf0c0b0, 0xfce4b0,
		   0xe0e01e, 0xd8f878, 0xc0e890, 0x95f7c8,
		   0x98e0e8, 0xf8d8f8, 0x000000, 0x000000 }
   },
   { "mess", "MESS palette",
	   { 0x747474, 0x24188c, 0x0000a8, 0x44009c,
		   0x8c0074, 0xa80010, 0xa40000, 0x7c0800,
		   0x402c00, 0x004400, 0x005000, 0x003c14,
		   0x183c5c, 0x000000, 0x000000, 0x000000,
		   0xbcbcbc, 0x0070ec, 0x2038ec, 0x8000f0,
		   0xbc00bc, 0xe40058, 0xd82800, 0xc84c0c,
		   0x887000, 0x009400, 0x00a800, 0x009038,
		   0x008088, 0x000000, 0x000000, 0x000000,
		   0xfcfcfc, 0x3cbcfc, 0x5c94fc, 0x4088fc,
		   0xf478fc, 0xfc74b4, 0xfc7460, 0xfc9838,
		   0xf0bc3c, 0x80d010, 0x4cdc48, 0x58f898,
		   0x00e8d8, 0x000000, 0x000000, 0x000000,
		   0xfcfcfc, 0xa8e4fc, 0xc4d4fc, 0xd4c8fc,
		   0xfcc4fc, 0xfcc4d8, 0xfcbcb0, 0xfcd8a8,
		   0xfce4a0, 0xe0fca0, 0xa8f0bc, 0xb0fccc,
		   0x9cfcf0, 0x000000, 0x000000, 0x000000 }
   },
   { "zaphod-cv", "Zaphod's VS Castlevania palette",
	   { 0x7f7f7f, 0xffa347, 0x0000bf, 0x472bbf,
		   0x970087, 0xf85898, 0xab1300, 0xf8b8f8,
		   0xbf0000, 0x007800, 0x006b00, 0x005b00,
		   0xffffff, 0x9878f8, 0x000000, 0x000000,
		   0xbfbfbf, 0x0078f8, 0xab1300, 0x6b47ff,
		   0x00ae00, 0xe7005b, 0xf83800, 0x7777ff,
		   0xaf7f00, 0x00b800, 0x00ab00, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x472bbf,
		   0xf8f8f8, 0xffe3ab, 0xf87858, 0x9878f8,
		   0x0078f8, 0xf85898, 0xbfbfbf, 0xffa347,
		   0xc800c8, 0xb8f818, 0x7f7f7f, 0x007800,
		   0x00ebdb, 0x000000, 0x000000, 0xffffff,
		   0xffffff, 0xa7e7ff, 0x5bdb57, 0xe75f13,
		   0x004358, 0x0000ff, 0xe7005b, 0x00b800,
		   0xfbdb7b, 0xd8f878, 0x8b1700, 0xffe3ab,
		   0x00ffff, 0xab0023, 0x000000, 0x000000 }
   },
   { "zaphod-smb", "Zaphod's VS SMB palette",
	   { 0x626a00, 0x0000ff, 0x006a77, 0x472bbf,
		   0x970087, 0xab0023, 0xab1300, 0xb74800,
		   0xa2a2a2, 0x007800, 0x006b00, 0x005b00,
		   0xffd599, 0xffff00, 0x009900, 0x000000,
		   0xff66ff, 0x0078f8, 0x0058f8, 0x6b47ff,
		   0x000000, 0xe7005b, 0xf83800, 0xe75f13,
		   0xaf7f00, 0x00b800, 0x5173ff, 0x00ab47,
		   0x008b8b, 0x000000, 0x91ff88, 0x000088,
		   0xf8f8f8, 0x3fbfff, 0x6b0000, 0x4855f8,
		   0xf878f8, 0xf85898, 0x595958, 0xff009d,
		   0x002f2f, 0xb8f818, 0x5bdb57, 0x58f898,
		   0x00ebdb, 0x787878, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0x590400, 0xbb0000,
		   0xf8b8f8, 0xfba7c3, 0xffffff, 0x00e3e1,
		   0xfbdb7b, 0xffae00, 0xb8f8b8, 0xb8f8d8,
		   0x00ff00, 0xf8d8f8, 0xffaaaa, 0x004000 }
   },
   { "vs-drmar", "VS Dr. Mario palette",
	   { 0x5f97ff, 0x000000, 0x000000, 0x47009f,
		   0x00ab00, 0xffffff, 0xabe7ff, 0x000000,
		   0x000000, 0x000000, 0x000000, 0x000000,
		   0xe7005b, 0x000000, 0x000000, 0x000000,
		   0x5f97ff, 0x000000, 0x000000, 0x000000,
		   0x000000, 0x8b7300, 0xcb4f0f, 0x000000,
		   0xbcbcbc, 0x000000, 0x000000, 0x000000,
		   0x000000, 0x000000, 0x000000, 0x000000,
		   0x00ebdb, 0x000000, 0x000000, 0x000000,
		   0x000000, 0xff9b3b, 0x000000, 0x000000,
		   0x83d313, 0x000000, 0x3fbfff, 0x000000,
		   0x0073ef, 0x000000, 0x000000, 0x000000,
		   0x00ebdb, 0x000000, 0x000000, 0x000000,
		   0x000000, 0x000000, 0xf3bf3f, 0x000000,
		   0x005100, 0x000000, 0xc7d7ff, 0xffdbab,
		   0x000000, 0x000000, 0x000000, 0x000000 }
   },
   { "vs-cv", "VS Castlevania palette",
	   { 0xaf7f00, 0xffa347, 0x008b8b, 0x472bbf,
		   0x970087, 0xf85898, 0xab1300, 0xf8b8f8,
		   0xf83800, 0x007800, 0x006b00, 0x005b00,
		   0xffffff, 0x9878f8, 0x00ab00, 0x000000,
		   0xbfbfbf, 0x0078f8, 0xab1300, 0x6b47ff,
		   0x000000, 0xe7005b, 0xf83800, 0x6b88ff,
		   0xaf7f00, 0x00b800, 0x6b88ff, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x472bbf,
		   0xf8f8f8, 0xffe3ab, 0xf87858, 0x9878f8,
		   0x0078f8, 0xf85898, 0xbfbfbf, 0xffa347,
		   0x004358, 0xb8f818, 0x7f7f7f, 0x007800,
		   0x00ebdb, 0x000000, 0x000000, 0xffffff,
		   0xffffff, 0xa7e7ff, 0x5bdb57, 0x6b88ff,
		   0x004358, 0x0000ff, 0xe7005b, 0x00b800,
		   0xfbdb7b, 0xffa347, 0x8b1700, 0xffe3ab,
		   0xb8f818, 0xab0023, 0x000000, 0x007800 }
   },
   { "vs-smb", "VS SMB/VS Ice Climber palette",
	   { 0xaf7f00, 0x0000ff, 0x008b8b, 0x472bbf,
		   0x970087, 0xab0023, 0x0000ff, 0xe75f13,
		   0xbfbfbf, 0x007800, 0x5bdb57, 0x005b00,
		   0xf0d0b0, 0xffe3ab, 0x00ab00, 0x000000,
		   0xbfbfbf, 0x0078f8, 0x0058f8, 0x6b47ff,
		   0x000000, 0xe7005b, 0xf83800, 0xf87858,
		   0xaf7f00, 0x00b800, 0x6b88ff, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x3fbfff,
		   0xf8f8f8, 0x006b00, 0x8b1700, 0x9878f8,
		   0x6b47ff, 0xf85898, 0x7f7f7f, 0xe7005b,
		   0x004358, 0xb8f818, 0x0078f8, 0x58f898,
		   0x00ebdb, 0xfbdb7b, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0xb8b8f8, 0xf83800,
		   0xf8b8f8, 0xfba7c3, 0xffffff, 0x00ffff,
		   0xfbdb7b, 0xffa347, 0xb8f8b8, 0xb8f8d8,
		   0xb8f818, 0xf8d8f8, 0x000000, 0x007800 }
   },
   { "nintendo-vc", "Virtual Console palette",
	   { 0x494949, 0x00006a, 0x090063, 0x290059,
		   0x42004a, 0x490000, 0x420000, 0x291100,
		   0x182700, 0x003010, 0x003000, 0x002910,
		   0x012043, 0x000000, 0x000000, 0x000000,
		   0x747174, 0x003084, 0x3101ac, 0x4b0194,
		   0x64007b, 0x6b0039, 0x6b2101, 0x5a2f00,
		   0x424900, 0x185901, 0x105901, 0x015932,
		   0x01495a, 0x101010, 0x000000, 0x000000,
		   0xadadad, 0x4a71b6, 0x6458d5, 0x8450e6,
		   0xa451ad, 0xad4984, 0xb5624a, 0x947132,
		   0x7b722a, 0x5a8601, 0x388e31, 0x318e5a,
		   0x398e8d, 0x383838, 0x000000, 0x000000,
		   0xb6b6b6, 0x8c9db5, 0x8d8eae, 0x9c8ebc,
		   0xa687bc, 0xad8d9d, 0xae968c, 0x9c8f7c,
		   0x9c9e72, 0x94a67c, 0x84a77b, 0x7c9d84,
		   0x73968d, 0xdedede, 0x000000, 0x000000 }
   },
   { "yuv-v3", "FBX's YUV-V3 palette",
	   { 0x666666, 0x002A88, 0x1412A7, 0x3B00A4,
		   0x5C007E, 0x6E0040, 0x6C0700, 0x561D00,
		   0x333500, 0x0C4800, 0x005200, 0x004C18,
		   0x003E5B, 0x000000, 0x000000, 0x000000,
		   0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE,
		   0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
		   0x6B6D00, 0x388700, 0x0D9300, 0x008C47,
		   0x007AA0, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x64B0FF, 0x9290FF, 0xC676FF,
		   0xF26AFF, 0xFF6ECC, 0xFF8170, 0xEA9E22,
		   0xBCBE00, 0x88D800, 0x5CE430, 0x45E082,
		   0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,
		   0xFFFFFF, 0xC0DFFF, 0xD3D2FF, 0xE8C8FF,
		   0xFAC2FF, 0xFFC4EA, 0xFFCCC5, 0xF7D8A5,
		   0xE4E594, 0xCFEF96, 0xBDF4AB, 0xB3F3CC,
		   0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000 }
   },
   { "unsaturated-final", "FBX's Unsaturated-Final palette",
	   { 0x676767, 0x001F8E, 0x23069E, 0x40008E,
		   0x600067, 0x67001C, 0x5B1000, 0x432500,
		   0x313400, 0x074800, 0x004F00, 0x004622,
		   0x003A61, 0x000000, 0x000000, 0x000000,
		   0xB3B3B3, 0x205ADF, 0x5138FB, 0x7A27EE,
		   0xA520C2, 0xB0226B, 0xAD3702, 0x8D5600,
		   0x6E7000, 0x2E8A00, 0x069200, 0x008A47,
		   0x037B9B, 0x101010, 0x000000, 0x000000,
		   0xFFFFFF, 0x62AEFF, 0x918BFF, 0xBC78FF,
		   0xE96EFF, 0xFC6CCD, 0xFA8267, 0xE29B26,
		   0xC0B901, 0x84D200, 0x58DE38, 0x46D97D,
		   0x49CED2, 0x494949, 0x000000, 0x000000,
		   0xFFFFFF, 0xC1E3FF, 0xD5D4FF, 0xE7CCFF,
		   0xFBC9FF, 0xFFC7F0, 0xFFD0C5, 0xF8DAAA,
		   0xEBE69A, 0xD1F19A, 0xBEF7AF, 0xB6F4CD,
		   0xB7F0EF, 0xB2B2B2, 0x000000, 0x000000 }
   },
   { "sony-cxa2025as-us", "Sony CXA2025AS US palette",
	   { 0x585858, 0x00238C, 0x00139B, 0x2D0585,
		   0x5D0052, 0x7A0017, 0x7A0800, 0x5F1800,
		   0x352A00, 0x093900, 0x003F00, 0x003C22,
		   0x00325D, 0x000000, 0x000000, 0x000000,
		   0xA1A1A1, 0x0053EE, 0x153CFE, 0x6028E4,
		   0xA91D98, 0xD41E41, 0xD22C00, 0xAA4400,
		   0x6C5E00, 0x2D7300, 0x007D06, 0x007852,
		   0x0069A9, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x1FA5FE, 0x5E89FE, 0xB572FE,
		   0xFE65F6, 0xFE6790, 0xFE773C, 0xFE9308,
		   0xC4B200, 0x79CA10, 0x3AD54A, 0x11D1A4,
		   0x06BFFE, 0x424242, 0x000000, 0x000000,
		   0xFFFFFF, 0xA0D9FE, 0xBDCCFE, 0xE1C2FE,
		   0xFEBCFB, 0xFEBDD0, 0xFEC5A9, 0xFED18E,
		   0xE9DE86, 0xC7E992, 0xA8EEB0, 0x95ECD9,
		   0x91E4FE, 0xACACAC, 0x000000, 0x000000 }
   },
   { "pal", "PAL palette",
	   { 0x808080, 0x0000BA, 0x3700BF, 0x8400A6,
		   0xBB006A, 0xB7001E, 0xB30000, 0x912600,
		   0x7B2B00, 0x003E00, 0x00480D, 0x003C22,
		   0x002F66, 0x000000, 0x050505, 0x050505,
		   0xC8C8C8, 0x0059FF, 0x443CFF, 0xB733CC,
		   0xFE33AA, 0xFE375E, 0xFE371A, 0xD54B00,
		   0xC46200, 0x3C7B00, 0x1D8415, 0x009566,
		   0x0084C4, 0x111111, 0x090909, 0x090909,
		   0xFEFEFE, 0x0095FF, 0x6F84FF, 0xD56FFF,
		   0xFE77CC, 0xFE6F99, 0xFE7B59, 0xFE915F,
		   0xFEA233, 0xA6BF00, 0x51D96A, 0x4DD5AE,
		   0x00D9FF, 0x666666, 0x0D0D0D, 0x0D0D0D,
		   0xFEFEFE, 0x84BFFF, 0xBBBBFF, 0xD0BBFF,
		   0xFEBFEA, 0xFEBFCC, 0xFEC4B7, 0xFECCAE,
		   0xFED9A2, 0xCCE199, 0xAEEEB7, 0xAAF8EE,
		   0xB3EEFF, 0xDDDDDD, 0x111111, 0x111111 }
   },
   { "bmf-final2", "BMF's Final 2 palette",
	   { 0x525252, 0x000080, 0x08008A, 0x2C007E,
		   0x4A004E, 0x500006, 0x440000, 0x260800,
		   0x0A2000, 0x002E00, 0x003200, 0x00260A,
		   0x001C48, 0x000000, 0x000000, 0x000000,
		   0xA4A4A4, 0x0038CE, 0x3416EC, 0x5E04DC,
		   0x8C00B0, 0x9A004C, 0x901800, 0x703600,
		   0x4C5400, 0x0E6C00, 0x007400, 0x006C2C,
		   0x005E84, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x4C9CFF, 0x7C78FF, 0xA664FF,
		   0xDA5AFF, 0xF054C0, 0xF06A56, 0xD68610,
		   0xBAA400, 0x76C000, 0x46CC1A, 0x2EC866,
		   0x34C2BE, 0x3A3A3A, 0x000000, 0x000000,
		   0xFFFFFF, 0xB6DAFF, 0xC8CAFF, 0xDAC2FF,
		   0xF0BEFF, 0xFCBCEE, 0xFAC2C0, 0xF2CCA2,
		   0xE6DA92, 0xCCE68E, 0xB8EEA2, 0xAEEABE,
		   0xAEE8E2, 0xB0B0B0, 0x000000, 0x000000 }
   },
   { "bmf-final3", "BMF's Final 3 palette",
	   { 0x686868, 0x001299, 0x1A08AA, 0x51029A,
		   0x7E0069, 0x8E001C, 0x7E0301, 0x511800,
		   0x1F3700, 0x014E00, 0x005A00, 0x00501C,
		   0x004061, 0x000000, 0x000000, 0x000000,
		   0xB9B9B9, 0x0C5CD7, 0x5035F0, 0x8919E0,
		   0xBB0CB3, 0xCE0C61, 0xC02B0E, 0x954D01,
		   0x616F00, 0x1F8B00, 0x01980C, 0x00934B,
		   0x00819B, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x63B4FF, 0x9B91FF, 0xD377FF,
		   0xEF6AFF, 0xF968C0, 0xF97D6C, 0xED9B2D,
		   0xBDBD16, 0x7CDA1C, 0x4BE847, 0x35E591,
		   0x3FD9DD, 0x606060, 0x000000, 0x000000,
		   0xFFFFFF, 0xACE7FF, 0xD5CDFF, 0xEDBAFF,
		   0xF8B0FF, 0xFEB0EC, 0xFDBDB5, 0xF9D28E,
		   0xE8EB7C, 0xBBF382, 0x99F7A2, 0x8AF5D0,
		   0x92F4F1, 0xBEBEBE, 0x000000, 0x000000 }
   },
   { "composite-direct-fbx", "FBX's Composite Direct palette",
	   { 0x656565, 0x00127D, 0x18008E, 0x360082,
		   0x56005D, 0x5A0018, 0x4F0500, 0x381900,
		   0x1D3100, 0x003D00, 0x004100, 0x003B17,
		   0x002E55, 0x000000, 0x000000, 0x000000,
		   0xAFAFAF, 0x194EC8, 0x472FE3, 0x6B1FD7,
		   0x931BAE, 0x9E1A5E, 0x993200, 0x7B4B00,
		   0x5B6700, 0x267A00, 0x008200, 0x007A3E,
		   0x006E8A, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x64A9FF, 0x8E89FF, 0xB676FF,
		   0xE06FFF, 0xEF6CC4, 0xF0806A, 0xD8982C,
		   0xB9B40A, 0x83CB0C, 0x5BD63F, 0x4AD17E,
		   0x4DC7CB, 0x4C4C4C, 0x000000, 0x000000,
		   0xFFFFFF, 0xC7E5FF, 0xD9D9FF, 0xE9D1FF,
		   0xF9CEFF, 0xFFCCF1, 0xFFD4CB, 0xF8DFB1,
		   0xEDEAA4, 0xD6F4A4, 0xC5F8B8, 0xBEF6D3,
		   0xBFF1F1, 0xB9B9B9, 0x000000, 0x000000 }
   },
   { "pvm-style-d93-fbx", "FBX's PVM Style D93 palette",
	   { 0x696B63, 0x001774, 0x1E0087, 0x340073,
		   0x560057, 0x5E0013, 0x531A00, 0x3B2400,
		   0x243000, 0x063A00, 0x003F00, 0x003B1E,
		   0x00334E, 0x000000, 0x000000, 0x000000,
		   0xB9BBB3, 0x1453B9, 0x4D2CDA, 0x671EDE,
		   0x98189C, 0x9D2344, 0xA03E00, 0x8D5500,
		   0x656D00, 0x2C7900, 0x008100, 0x007D42,
		   0x00788A, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x69A8FF, 0x9691FF, 0xB28AFA,
		   0xEA7DFA, 0xF37BC7, 0xF28E59, 0xE6AD27,
		   0xD7C805, 0x90DF07, 0x64E53C, 0x45E27D,
		   0x48D5D9, 0x4E5048, 0x000000, 0x000000,
		   0xFFFFFF, 0xD2EAFF, 0xE2E2FF, 0xE9D8FF,
		   0xF5D2FF, 0xF8D9EA, 0xFADEB9, 0xF9E89B,
		   0xF3F28C, 0xD3FA91, 0xB8FCA8, 0xAEFACA,
		   0xCAF3F3, 0xBEC0B8, 0x000000, 0x000000 }
   },
   { "ntsc-hardware-fbx", "FBX's NTSC Hardware palette",
	   { 0x6A6D6A, 0x001380, 0x1E008A, 0x39007A,
		   0x550056, 0x5A0018, 0x4F1000, 0x382100,
		   0x213300, 0x003D00, 0x004000, 0x003924,
		   0x002E55, 0x000000, 0x000000, 0x000000,
		   0xB9BCB9, 0x1850C7, 0x4B30E3, 0x7322D6,
		   0x951FA9, 0x9D285C, 0x963C00, 0x7A5100,
		   0x5B6700, 0x227700, 0x027E02, 0x007645,
		   0x006E8A, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x68A6FF, 0x9299FF, 0xB085FF,
		   0xD975FD, 0xE377B9, 0xE58D68, 0xCFA22C,
		   0xB3AF0C, 0x7BC211, 0x55CA47, 0x46CB81,
		   0x47C1C5, 0x4A4D4A, 0x000000, 0x000000,
		   0xFFFFFF, 0xCCEAFF, 0xDDDEFF, 0xECDAFF,
		   0xF8D7FE, 0xFCD6F5, 0xFDDBCF, 0xF9E7B5,
		   0xF1F0AA, 0xDAFAA9, 0xC9FFBC, 0xC3FBD7,
		   0xC4F6F6, 0xBEC1BE, 0x000000, 0x000000 }
   },
   { "nes-classic-fbx-fs", "FBX's NES-Classic FS palette",
	   { 0x60615F, 0x000083, 0x1D0195, 0x340875,
		   0x51055E, 0x56000F, 0x4C0700, 0x372308,
		   0x203A0B, 0x0F4B0E, 0x194C16, 0x02421E,
		   0x023154, 0x000000, 0x000000, 0x000000,
		   0xA9AAA8, 0x104BBF, 0x4712D8, 0x6300CA,
		   0x8800A9, 0x930B46, 0x8A2D04, 0x6F5206,
		   0x5C7114, 0x1B8D12, 0x199509, 0x178448,
		   0x206B8E, 0x000000, 0x000000, 0x000000,
		   0xFBFBFB, 0x6699F8, 0x8974F9, 0xAB58F8,
		   0xD557EF, 0xDE5FA9, 0xDC7F59, 0xC7A224,
		   0xA7BE03, 0x75D703, 0x60E34F, 0x3CD68D,
		   0x56C9CC, 0x414240, 0x000000, 0x000000,
		   0xFBFBFB, 0xBED4FA, 0xC9C7F9, 0xD7BEFA,
		   0xE8B8F9, 0xF5BAE5, 0xF3CAC2, 0xDFCDA7,
		   0xD9E09C, 0xC9EB9E, 0xC0EDB8, 0xB5F4C7,
		   0xB9EAE9, 0xABABAB, 0x000000, 0x000000 }
   },
   { "nescap", "RGBSource's NESCAP palette",
	   { 0x646365, 0x001580, 0x1D0090, 0x380082,
		   0x56005D, 0x5A001A, 0x4F0900, 0x381B00,
		   0x1E3100, 0x003D00, 0x004100, 0x003A1B,
		   0x002F55, 0x000000, 0x000000, 0x000000,
		   0xAFADAF, 0x164BCA, 0x472AE7, 0x6B1BDB,
		   0x9617B0, 0x9F185B, 0x963001, 0x7B4800,
		   0x5A6600, 0x237800, 0x017F00, 0x00783D,
		   0x006C8C, 0x000000, 0x000000, 0x000000,
		   0xFFFFFF, 0x60A6FF, 0x8F84FF, 0xB473FF,
		   0xE26CFF, 0xF268C3, 0xEF7E61, 0xD89527,
		   0xBAB307, 0x81C807, 0x57D43D, 0x47CF7E,
		   0x4BC5CD, 0x4C4B4D, 0x000000, 0x000000,
		   0xFFFFFF, 0xC2E0FF, 0xD5D2FF, 0xE3CBFF,
		   0xF7C8FF, 0xFEC6EE, 0xFECEC6, 0xF6D7AE,
		   0xE9E49F, 0xD3ED9D, 0xC0F2B2, 0xB9F1CC,
		   0xBAEDED, 0xBAB9BB, 0x000000, 0x000000 }
   }
};

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{ }

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_cb = cb;
}

void retro_set_controller_port_device(unsigned a, unsigned b)
{}


void retro_set_environment(retro_environment_t cb)
{
   static const struct retro_variable vars[] = {
      { "fceumm_palette", "Color Palette; asqrealc|loopy|quor|chris|matt|pasofami|crashman|mess|zaphod-cv|zaphod-smb|vs-drmar|vs-cv|vs-smb|nintendo-vc|yuv-v3|unsaturated-final|sony-cxa2025as-us|pal|bmf-final2|bmf-final3|composite-direct-fbx|pvm-style-d93-fbx|ntsc-hardware-fbx|nes-classic-fbx-fs|nescap|raw" },
      { "fceumm_nospritelimit", "No Sprite Limit; disabled|enabled" },
      { "fceumm_overclocking", "Overclocking; disabled|2x" },
      { "fceumm_overscan", "Crop Overscan; enabled|disabled" },
      { "fceumm_turbo_enable", "Turbo Enable; None|Player 1|Player 2|Both" },
      { "fceumm_turbo_delay", "Turbo Delay (in frames); 3|5|10|15|30|60|1|2" },
      { "fceumm_aspect", "Preferred aspect ratio; 8:7 PAR|4:3" },
      { "fceumm_region", "Region Override; Auto|NTSC|PAL|Dendy" },
      { NULL, NULL },
   };

   environ_cb = cb;
   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
}

void retro_get_system_info(struct retro_system_info *info)
{
   info->need_fullpath    = false;
   info->valid_extensions = "fds|nes|unf|unif";
#ifdef GIT_VERSION
   info->library_version  = "git" GIT_VERSION;
#else
   info->library_version  = "(SVN)";
#endif
   info->library_name     = "FCEUmm";
   info->block_extract    = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   unsigned width = use_overscan ? 256 : (256 - 16);
   unsigned height = use_overscan ? 240 : (240 - 16);
   info->geometry.base_width = width;
   info->geometry.base_height = height;
   info->geometry.max_width = width;
   info->geometry.max_height = height;
   info->geometry.aspect_ratio = use_par ? NES_8_7_PAR : NES_4_3;
   info->timing.sample_rate = 32050.0;
   if (FSettings.PAL || dendy)
      info->timing.fps = 838977920.0/16777215.0;
   else
      info->timing.fps = 1008307711.0/16777215.0;
   info->timing.sample_rate = 32040.5;
}

static void check_system_specs(void)
{
   // TODO - when we get it running at fullspeed on PSP, set to 4
   unsigned level = 5;
   environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
}

void retro_init(void)
{
   enum retro_pixel_format rgb565;
   log_cb.log=default_logger;
   environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log_cb);
#ifdef FRONTEND_SUPPORTS_RGB565
   rgb565 = RETRO_PIXEL_FORMAT_RGB565;
   if(environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &rgb565))
      log_cb.log(RETRO_LOG_INFO, "Frontend supports RGB565 - will use that instead of XRGB1555.\n");
#endif
}

static void retro_set_custom_palette (void)
{
   uint8_t i,r,g,b;

   use_raw_palette = false;

   if (current_palette == 0)
   {
      FCEU_ResetPalette();	/* Do palette reset*/
      return;
   }

   if (current_palette == 26) /* raw palette */
   {
      use_raw_palette = true;
      for (i = 0; i < 64; i++)
      {
         r = (((i >> 0) & 0xF) * 255) / 15;
         g = (((i >> 4) & 0x3) * 255) / 3;
         FCEUD_SetPalette( i, r, g, 0);
      }
      return;
   }


   /* Setup this palette*/

   for ( i = 0; i < 64; i++ )
   {
      r = palettes[current_palette-1].data[i] >> 16;
      g = ( palettes[current_palette-1].data[i] & 0xff00 ) >> 8;
      b = ( palettes[current_palette-1].data[i] & 0xff );
      FCEUD_SetPalette( i, r, g, b);
      FCEUD_SetPalette( i+64, r, g, b);
      FCEUD_SetPalette( i+128, r, g, b);
      FCEUD_SetPalette( i+192, r, g, b);
   }
}

/* Set variables for NTSC(1) / PAL(2) / Dendy(3)
 * Dendy has PAL framerate and resolution, but ~NTSC timings,
 * and has 50 dummy scanlines to force 50 fps. */
void FCEUD_RegionOverride(int region)
{
   static int w = 0;
   struct retro_system_av_info av_info;

   switch (region)
   {
      case 0: /* auto */
         dendy = 0;
         w = (GameInfo->vidsys == GIV_PAL) ? 1 : 0;
         break;
      case 1: /* ntsc */
         dendy = 0;
         w = 0;
         FCEU_DispMessage("Switched to NTSC");
         break;
      case 2: /* pal */
         dendy = 0;
         w = 1;
         FCEU_DispMessage("Switched to PAL");
         break;
      case 3: /* dendy */
         dendy = 1;
         w = 0;
         FCEU_DispMessage("Switched to Dendy");
         break;
   }

   FSettings.PAL = w ;
   PAL = w ? 1 : 0;
   normal_scanlines = dendy ? 290 : 240;
   totalscanlines = normal_scanlines + (overclock_state ? extrascanlines : 0);
	FCEUPPU_SetVideoSystem(w || dendy);
	SetSoundVariables();

   // Update the geometry
   retro_get_system_av_info(&av_info);
   environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &av_info);
}

void retro_deinit (void)
{
   FCEUI_CloseGame();
   FCEUI_Sound(0);
   FCEUI_Kill();
#if defined(_3DS)
   linearFree(fceu_video_out);
#else
   if (fceu_video_out)
      free(fceu_video_out);
   fceu_video_out = NULL;
#endif
}

void retro_reset(void)
{
   ResetNES();
}


typedef struct
{
   unsigned retro;
   unsigned nes;
} keymap;

static const keymap bindmap[] = {
   { RETRO_DEVICE_ID_JOYPAD_A, JOY_A },
   { RETRO_DEVICE_ID_JOYPAD_B, JOY_B },
   { RETRO_DEVICE_ID_JOYPAD_SELECT, JOY_SELECT },
   { RETRO_DEVICE_ID_JOYPAD_START, JOY_START },
   { RETRO_DEVICE_ID_JOYPAD_UP, JOY_UP },
   { RETRO_DEVICE_ID_JOYPAD_DOWN, JOY_DOWN },
   { RETRO_DEVICE_ID_JOYPAD_LEFT, JOY_LEFT },
   { RETRO_DEVICE_ID_JOYPAD_RIGHT, JOY_RIGHT },
   { RETRO_DEVICE_ID_JOYPAD_X, JOY_A },
   { RETRO_DEVICE_ID_JOYPAD_Y, JOY_B },
};

static void check_variables(bool startup)
{
   struct retro_variable var = {0};
   struct retro_system_av_info av_info;
   bool geometry_update = false;
   unsigned old_regionoverride = regionoverride;

   var.key = "fceumm_palette";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned orig_value = current_palette;

      if (!strcmp(var.value, "asqrealc"))
         current_palette = 0;
      else if (!strcmp(var.value, "loopy"))
         current_palette = 1;
      else if (!strcmp(var.value, "quor"))
         current_palette = 2;
      else if (!strcmp(var.value, "chris"))
         current_palette = 3;
      else if (!strcmp(var.value, "matt"))
         current_palette = 4;
      else if (!strcmp(var.value, "matt"))
         current_palette = 5;
      else if (!strcmp(var.value, "pasofami"))
         current_palette = 6;
      else if (!strcmp(var.value, "crashman"))
         current_palette = 7;
      else if (!strcmp(var.value, "mess"))
         current_palette = 8;
      else if (!strcmp(var.value, "zaphod-cv"))
         current_palette = 9;
      else if (!strcmp(var.value, "zaphod-smb"))
         current_palette = 10;
      else if (!strcmp(var.value, "vs-drmar"))
         current_palette = 11;
      else if (!strcmp(var.value, "vs-cv"))
         current_palette = 12;
      else if (!strcmp(var.value, "vs-smb"))
         current_palette = 13;
      else if (!strcmp(var.value, "nintendo-vc"))
         current_palette = 14;
      else if (!strcmp(var.value, "yuv-v3"))
         current_palette = 15;
      else if (!strcmp(var.value, "unsaturated-final"))
         current_palette = 16;
      else if (!strcmp(var.value, "sony-cxa2025as-us"))
         current_palette = 17;
      else if (!strcmp(var.value, "pal"))
         current_palette = 18;
      else if (!strcmp(var.value, "bmf-final2"))
         current_palette = 19;
      else if (!strcmp(var.value, "bmf-final3"))
         current_palette = 20;
      else if (!strcmp(var.value, "composite-direct-fbx"))
         current_palette = 21;
      else if (!strcmp(var.value, "pvm-style-d93-fbx"))
         current_palette = 22;
      else if (!strcmp(var.value, "ntsc-hardware-fbx"))
         current_palette = 23;
      else if (!strcmp(var.value, "nes-classic-fbx-fs"))
         current_palette = 24;
      else if (!strcmp(var.value, "nescap"))
         current_palette = 25;
      else if (!strcmp(var.value, "raw"))
         current_palette = 26;

      if (current_palette != orig_value)
         retro_set_custom_palette();
   }

   var.key = "fceumm_nospritelimit";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      int no_sprite_limit = (!strcmp(var.value, "enabled")) ? 1 : 0;
      FCEUI_DisableSpriteLimitation(no_sprite_limit);
   }

   var.key = "fceumm_overclocking";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool do_reinit = false;

      if (!strcmp(var.value, "disabled")
            && overclock_state != 0)
      {
         overclocked            = 0;
         skip_7bit_overclocking = 1;
         extrascanlines         = 0;
         overclock_state        = 0;
         do_reinit              = true;
      }
      else if (!strcmp(var.value, "2x")
            && overclock_state != 1)
      {
         overclocked            = 1;
         skip_7bit_overclocking = 1;
         extrascanlines         = 266;
         overclock_state        = 1;
         do_reinit              = true;
      }

      if (do_reinit && startup)
      {
         FCEU_KillVirtualVideo();
         FCEU_InitVirtualVideo();
      }
   }

   var.key = "fceumm_overscan";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "disabled"));
      if (newval != use_overscan)
      {
         use_overscan = newval;
         geometry_update = true;
      }
   }

   var.key = "fceumm_turbo_enable";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "None"))
      {
         turbo_enabler = 0;
      }
      else if (!strcmp(var.value, "Player 1"))
      {
         turbo_enabler = 1;
      }
      else if (!strcmp(var.value, "Player 2"))
      {
         turbo_enabler = 2;
      }
      else if (!strcmp(var.value, "Both"))
      {
         turbo_enabler = 3;
      }
   }

   var.key = "fceumm_turbo_delay";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "1"))
      {
         turbo_delay = 1;
      }
      if (!strcmp(var.value, "2"))
      {
         turbo_delay = 2;
      }
      else if (!strcmp(var.value, "3"))
      {
         turbo_delay = 3;
      }
      else if (!strcmp(var.value, "5"))
      {
         turbo_delay = 5;
      }
      else if (!strcmp(var.value, "10"))
      {
         turbo_delay = 10;
      }
	  else if (!strcmp(var.value, "15"))
      {
         turbo_delay = 15;
      }
      else if (!strcmp(var.value, "30"))
      {
         turbo_delay = 30;
      }
      else if (!strcmp(var.value, "60"))
      {
         turbo_delay = 60;
      }
   }
   var.key = "fceumm_region";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "Auto"))
         regionoverride = 0;
      else if (!strcmp(var.value, "NTSC"))
         regionoverride = 1;
      else if (!strcmp(var.value, "PAL"))
         regionoverride = 2;
      else if (!strcmp(var.value, "Dendy"))
         regionoverride = 3;
   }
   if (regionoverride != old_regionoverride)
      FCEUD_RegionOverride(regionoverride);

   var.key = "fceumm_aspect";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "8:7 PAR"));
      if (newval != use_par)
      {
         use_par = newval;
         geometry_update = true;
      }
   }

   if (geometry_update)
   {
      retro_get_system_av_info(&av_info);
      environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &av_info);
   }
}

/*
 * Flags to keep track of whether turbo
 * was toggled on or off
 * p0 - Player 1
 * p1 - Player 2
 * There are two values in array
 * for Turbo A and Turbo B for
 * each player
 */

unsigned char turbo_p0_toggle[] = {0,0};
unsigned char turbo_p1_toggle[] = {0,0};

static void FCEUD_UpdateInput(void)
{
   unsigned i;
   unsigned char pad[2];

   pad[0] = 0;
   pad[1] = 0;

   poll_cb();

   for ( i = 0; i < 8; i++)
      pad[0] |= input_cb(0, RETRO_DEVICE_JOYPAD, 0, bindmap[i].retro) ? bindmap[i].nes : 0;

   for ( i = 0; i < 8; i++)
      pad[1] |= input_cb(1, RETRO_DEVICE_JOYPAD, 0, bindmap[i].retro) ? bindmap[i].nes : 0;

   /*
    * Turbo A and Turbo B buttons are
    * mapped to Joypad X and Joypad Y
    * in RetroArch joypad.
    *
    * We achieve this by keeping track of
    * the number of times it increments
    * the toggle counter and fire or not fire
    * depending on whether the delay value has
    * been reached.
    */
   if (turbo_enabler == 1 || turbo_enabler == 3)
      {
         // Handle turbo buttons - player 1
         for ( i = 8; i < 10; i++) {
            if(input_cb(0, RETRO_DEVICE_JOYPAD, 0, bindmap[i].retro)) {
               if (turbo_p0_toggle[i-8] == 0) {
                  pad[0] |= bindmap[i].nes;
          }
          turbo_p0_toggle[i-8]++;
          if (turbo_p0_toggle[i-8] > turbo_delay) {
             // Reset the toggle if
             // delay value is reached
             pad[0] |= bindmap[i].nes;
             turbo_p0_toggle[i-8] = 0;
          }
          } else {
             // If the button is not pressed, just reset the toggle
             turbo_p0_toggle[i-8] = 0;
             }
         }
      }
   if (turbo_enabler == 2 || turbo_enabler == 3)
      {
         // Handle turbo buttons - player 2
         for ( i = 8; i < 10; i++) {
            if(input_cb(1, RETRO_DEVICE_JOYPAD, 0, bindmap[i].retro)) {
               if (turbo_p1_toggle[i-8] == 0) {
                  pad[1] |= bindmap[i].nes;
          }
          turbo_p1_toggle[i-8]++;
          if (turbo_p1_toggle[i-8] > turbo_delay) {
             // Reset the toggle if
             // delay value is reached
             pad[1] |= bindmap[i].nes;
             turbo_p1_toggle[i-8] = 0;
          }
          } else {
             // If the button is not pressed, just reset the toggle
             turbo_p1_toggle[i-8] = 0;
             }
         }
      }

   JSReturn[0] = pad[0] | (pad[1] << 8);

   if (input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2))
   {
      FCEU_VSUniCoin(); /* Insert Coin VS System */
   }

   if (GameInfo->type == GIT_FDS) /* Famicom Disk System */
   {
      bool curL = input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
      bool curR = input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R);
      static bool prevL = false, prevR = false;

      if (curL && !prevL)
      {
         FCEU_FDSSelect(); /* Swap FDisk side */
      }
      prevL = curL;

      if (curR && !prevR)
      {
         FCEU_FDSInsert(-1); /* Insert or eject the disk */
      }
      prevR = curR;
   }
}

void FCEUD_Update(uint8 *XBuf, int32 *Buffer, int Count)
{
}

static void retro_run_blit(uint8_t *gfx)
{
   unsigned x, y;
#ifdef PSP
   static unsigned int __attribute__((aligned(16))) d_list[32];
   void* texture_vram_p = NULL;
#endif
   uint16_t *fceu_video_out_ptr = NULL;
   struct retro_framebuffer fb = {0};
   unsigned incr   = 0;
   unsigned width  = 256;
   unsigned height = 240;
   unsigned pitch  = 512;

   if (!use_overscan)
   {
      incr    = 16;
      width  -= 16;
      height -= 16;
#ifndef PSP
      pitch  -= 32;
      gfx     = gfx + 8 + 256 * 8;

#endif
   }

#ifdef PSP
   texture_vram_p = (void*) (0x44200000 - (256 * 256)); // max VRAM address - frame size

   sceKernelDcacheWritebackRange(retro_palette,256 * 2);
   sceKernelDcacheWritebackRange(XBuf, 256*240 );

   sceGuStart(GU_DIRECT, d_list);

   /* sceGuCopyImage doesnt seem to work correctly with GU_PSM_T8
    * so we use GU_PSM_4444 ( 2 Bytes per pixel ) instead
    * with half the values for pitch / width / x offset
    */
   if (use_overscan)
      sceGuCopyImage(GU_PSM_4444, 0, 0, 128, 240, 128, XBuf, 0, 0, 128, texture_vram_p);
   else
      sceGuCopyImage(GU_PSM_4444, 4, 4, 120, 224, 128, XBuf, 0, 0, 128, texture_vram_p);

   sceGuTexSync();
   sceGuTexImage(0, 256, 256, 256, texture_vram_p);
   sceGuTexMode(GU_PSM_T8, 0, 0, GU_FALSE);
   sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
   sceGuDisable(GU_BLEND);
   sceGuClutMode(GU_PSM_5650, 0, 0xFF, 0);
   sceGuClutLoad(32, retro_palette);

   sceGuFinish();

   video_cb(texture_vram_p, width, height, 256);
#else
   fb.width           = width;
   fb.height          = height;
   fb.access_flags    = RETRO_MEMORY_ACCESS_WRITE;

   fceu_video_out_ptr = fceu_video_out;

   if (environ_cb(RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER, &fb) && fb.format == RETRO_PIXEL_FORMAT_RGB565)
   {
      fceu_video_out_ptr = (uint16_t*)fb.data;
      pitch = fb.pitch >> 1;
   }

   if (use_raw_palette)
   {
      extern uint8 PPU[4];
      int deemp = (PPU[1] >> 5) << 2;
      for (y = 0; y < height; y++, gfx += incr)
         for ( x = 0; x < width; x++, gfx++)
            fceu_video_out_ptr[y * width + x] = retro_palette[*gfx & 0x3F] | deemp;
   }
   else
   {
      for (y = 0; y < height; y++, gfx += incr)
         for ( x = 0; x < width; x++, gfx++)
            fceu_video_out_ptr[y * width + x] = retro_palette[*gfx];
   }

   video_cb(fceu_video_out_ptr, width, height, pitch);
#endif

}

void retro_run(void)
{
   unsigned i;
   uint8_t *gfx;
   int32_t ssize = 0;
   bool updated = false;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables(false);

   FCEUD_UpdateInput();
   FCEUI_Emulate(&gfx, &sound, &ssize, 0);

   for (i = 0; i < ssize; i++)
      sound[i] = (sound[i] << 16) | (sound[i] & 0xffff);

   audio_batch_cb((const int16_t*)sound, ssize);

   retro_run_blit(gfx);
}

static unsigned serialize_size = 0;

size_t retro_serialize_size(void)
{
   if (serialize_size == 0)
   {
      /* Something arbitrarily big.*/
      uint8_t *buffer = (uint8_t*)malloc(1000000);
      memstream_set_buffer(buffer, 1000000);

      FCEUSS_Save_Mem();
      serialize_size = memstream_get_last_size();
      free(buffer);
   }

   return serialize_size;
}

bool retro_serialize(void *data, size_t size)
{
   if (size != retro_serialize_size())
      return false;

   memstream_set_buffer((uint8_t*)data, size);
   FCEUSS_Save_Mem();
   return true;
}

bool retro_unserialize(const void * data, size_t size)
{
   if (size != retro_serialize_size())
      return false;

   memstream_set_buffer((uint8_t*)data, size);
   FCEUSS_Load_Mem();
   return true;
}

void retro_cheat_reset(void)
{
   FCEU_ResetCheats();
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   char name[256];
   uint16 a;
   uint8  v;
   int    c;
   int    type = 1;
   sprintf(name, "N/A");

   if (FCEUI_DecodeGG(code, &a, &v, &c))
      goto input_cheat;

   /* Not a Game Genie code. */

   if (FCEUI_DecodePAR(code, &a, &v, &c, &type))
      goto input_cheat;

   /* Not a Pro Action Replay code. */

   return;
input_cheat:
   FCEUI_AddCheat(name, a, v, c, type);
}

typedef struct cartridge_db
{
   char title[256];
   uint32_t crc;
} cartridge_db_t;

static const struct cartridge_db fourscore_db_list[] =
{
   {
      "Bomberman II (USA)",
      0x1ebb5b42
   },
#if 0
   {
      "Championship Bowling (USA)",
      0xeac38105
   },
#endif
   {
      "Chris Evert & Ivan Lendl in Top Players' Tennis (USA)",
      0xf99e37eb
   },
#if 0
   {
      "Crash 'n' the Boys - Street Challenge (USA)",
      0xc7f0c457
   },
#endif
   {
      "Four Players' Tennis (Europe)",
      0x48b8ee58
   },
   {
      "Danny Sullivan's Indy Heat (Europe)",
      0x27ca0679,
   },
   {
      "Gauntlet II (Europe)",
      0x79f688bc
   },
   {
      "Gauntlet II (USA)",
      0x1b71ccdb
   },
   {
      "Greg Norman's Golf Power (USA)",
      0x1352f1b9
   },
   {
      "Harlem Globetrotters (USA)",
      0x2e6ee98d
   },
   {
      "Ivan 'Ironman' Stewart's Super Off Road (Europe)",
      0x05104517
   },
   {
      "Ivan 'Ironman' Stewart's Super Off Road (USA)",
      0x4b041b6b
   },
   {
      "Kings of the Beach - Professional Beach Volleyball (USA)",
      0xf54b34bd
   },
   {
      "Magic Johnson's Fast Break (USA)",
      0xc6c2edb5
   },
   {
      "M.U.L.E. (USA)",
      0x0939852f
   },
   {
      "Monster Truck Rally (USA)",
      0x2f698c4d
   },
   {
      "NES Play Action Football (USA)",
      0xb9b4d9e0
   },
   {
      "Nightmare on Elm Street, A (USA)",
      0xda2cb59a
   },
   {
      "Nintendo World Cup (Europe)",
      0x8da6667d
   },
   {
      "Nintendo World Cup (Europe) (Rev A)",
      0x7c16f819
   },
   {
      "Nintendo World Cup (Europe) (Rev B)",
      0x7f08d0d9
   },
   {
      "Nintendo World Cup (USA)",
      0xa22657fa
   },
   {
      "R.C. Pro-Am II (Europe)",
      0x308da987
   },
   {
      "R.C. Pro-Am II (USA)",
      0x9edd2159
   },
   {
      "Rackets & Rivals (Europe)",
      0x8fa6e92c
   },
   {
      "Roundball - 2-on-2 Challenge (Europe)",
      0xad0394f0
   },
   {
      "Roundball - 2-on-2 Challenge (USA)",
      0x6e4dcfd2
   },
   {
      "Spot - The Video Game (Japan)",
      0x0abdd5ca
   },
   {
      "Spot - The Video Game (USA)",
      0xcfae9dfa
   },
   {
      "Smash T.V. (Europe)",
      0x0b8f8128
   },
   {
      "Smash T.V. (USA)",
      0x6ee94d32
   },
   {
      "Super Jeopardy! (USA)",
      0xcf4487a2
   },
   {
      "Super Spike V'Ball (Europe)",
      0xc05a63b2
   },
   {
      "Super Spike V'Ball (USA)",
      0xe840fd21
   },
   {
      "Super Spike V'Ball + Nintendo World Cup (USA)",
      0x407d6ffd
   },
   {
      "Swords and Serpents (Europe)",
      0xd153caf6
   },
   {
      "Swords and Serpents (France)",
      0x46135141
   },
   {
      "Swords and Serpents (USA)",
      0x3417ec46
   },
   {
      "Battle City (Japan) (4 Players Hack) http://www.romhacking.net/hacks/2142/",
      0x69977c9e
   },
   {
      "Bomberman 3 (Homebrew) http://tempect.de/senil/games.html",
      0x2da5ece0
   },
   {
      "K.Y.F.F. (Homebrew) http://slydogstudios.org/index.php/k-y-f-f/",
      0x90d2e9f0
   },
   {
      "Super PakPak (Homebrew) http://wiki.nesdev.com/w/index.php/Super_PakPak",
      0x1394ded0
   },
   {
      "Super Mario Bros. + Tetris + Nintendo World Cup (Europe)",
      0x73298c87
   },
   {
      "Super Mario Bros. + Tetris + Nintendo World Cup (Europe) (Rev A)",
      0xf46ef39a
   }
};

static const struct cartridge_db famicom_4p_db_list[] =
{
   {
      "Bakutoushi Patton-Kun (Japan) (FDS)",
      0xc39b3bb2
   },
   {
      "Bomber Man II (Japan)",
      0x0c401790
   },
   {
      "Championship Bowling (Japan)",
      0x9992f445
   },
   {
      "Downtown - Nekketsu Koushinkyoku - Soreyuke Daiundoukai (Japan)",
      0x3e470fe0
   },
   {
      "Ike Ike! Nekketsu Hockey-bu - Subette Koronde Dairantou (Japan)",
      0x4f032933
   },
   {
      "Kunio-kun no Nekketsu Soccer League (Japan)",
      0x4b5177e9
   },
   {
      "Moero TwinBee - Cinnamon Hakase o Sukue! (Japan)",
      0x9f03b11f
   },
   {
      "Moero TwinBee - Cinnamon Hakase wo Sukue! (Japan) (FDS)",
      0x13205221
   },
   {
      "Nekketsu Kakutou Densetsu (Japan)",
      0x37e24797
   },
   {
      "Nekketsu Koukou Dodgeball-bu (Japan)",
      0x62c67984
   },
   {
      "Nekketsu! Street Basket - Ganbare Dunk Heroes (Japan)",
      0x88062d9a
   },
   {
      "Super Dodge Ball (USA) (3-4p with Game Genie code GEUOLZZA)",
      0x689971f9
   },
   {
      "Super Dodge Ball (USA) (patched) http://www.romhacking.net/hacks/71/",
      0x4ff17864
   },
   {
      "U.S. Championship V'Ball (Japan)",
      0x213cb3fb
   },
   {
      "U.S. Championship V'Ball (Japan) (Beta)",
      0xd7077d96
   },
   {
      "Wit's (Japan)",
      0xb1b16b8a
   }
};

extern uint32_t iNESGameCRC32;

bool retro_load_game(const struct retro_game_info *game)
{
   unsigned i, j;
   char* dir=NULL;
   char* sav_dir=NULL;
   size_t fourscore_len = sizeof(fourscore_db_list)   / sizeof(fourscore_db_list[0]);
   size_t famicom_4p_len = sizeof(famicom_4p_db_list) / sizeof(famicom_4p_db_list[0]);

   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "(VSSystem) Insert Coin" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "(FDS) Disk Side Change" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "(FDS) Insert/Eject Disk" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 0 },
   };

   struct retro_memory_descriptor descs[64];
   struct retro_memory_map        mmaps;

   if (!game)
      return false;

   PowerNES();
   check_system_specs();
#if defined(_3DS)
   fceu_video_out = (uint16_t*)linearMemAlign(256 * 240 * sizeof(uint16_t), 128);
#elif !defined(PSP)
   fceu_video_out = (uint16_t*)malloc(256 * 240 * sizeof(uint16_t));
#endif

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
      FCEUI_SetBaseDirectory(dir);
   if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &sav_dir) && sav_dir)
      FCEUI_SetSaveDirectory(sav_dir);

   FCEUI_Initialize();

   FCEUI_SetSoundVolume(256);
   FCEUI_Sound(32050);

   GameInfo = (FCEUGI*)FCEUI_LoadGame(game->path, (uint8_t*)game->data, game->size);
   if (!GameInfo)
   {
      struct retro_message msg;
      char msg_local[256];

      sprintf(msg_local, "ROM loading failed...");
      msg.msg    = msg_local;
      msg.frames = 360;
      if (environ_cb)
         environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, (void*)&msg);
      return false;
   }

   FCEUI_SetInput(0, SI_GAMEPAD, &JSReturn[0], 0);
   FCEUI_SetInput(1, SI_GAMEPAD, &JSReturn[0], 0);

   retro_set_custom_palette();

   FCEUD_SoundToggle();
   check_variables(true);

   FCEUI_DisableFourScore(1);

   for (i = 0; i < fourscore_len; i++)
   {
      if (fourscore_db_list[i].crc == iNESGameCRC32)
      {
         FCEUI_DisableFourScore(0);
         break;
      }
   }

   for (i = 0; i < famicom_4p_len; i++)
   {
      if (famicom_4p_db_list[i].crc == iNESGameCRC32)
      {
         FCEUI_SetInputFC(SIFC_4PLAYER, &JSReturn[0], 0);
         break;
      }
   }

   memset(descs, 0, sizeof(descs));
   i = 0;

   for (j = 0; j < 64; j++)
   {
      if (MMapPtrs[j] != NULL)
      {
         descs[i].ptr    = MMapPtrs[j];
         descs[i].start  = j * 1024;
         descs[i].len    = 1024;
         descs[i].select = 0;
         i++;
      }
   }

   mmaps.descriptors = descs;
   mmaps.num_descriptors = i;
   environ_cb(RETRO_ENVIRONMENT_SET_MEMORY_MAPS, &mmaps);
 
   return true;
}

bool retro_load_game_special(
  unsigned game_type,
  const struct retro_game_info *info, size_t num_info
)
{
   return false;
}

void retro_unload_game(void)
{
   FCEUI_CloseGame();
#if defined(_3DS)
   if (fceu_video_out)
      linearFree(fceu_video_out);
#else
   if (fceu_video_out)
      free(fceu_video_out);
   fceu_video_out = NULL;
#endif
}

unsigned retro_get_region(void)
{
   return FSettings.PAL ? RETRO_REGION_PAL : RETRO_REGION_NTSC;
}


void *retro_get_memory_data(unsigned type)
{
   uint8_t* data;

   switch(type)
   {
      case RETRO_MEMORY_SAVE_RAM:
         if (iNESCart.battery)
	         return iNESCart.SaveGame[0];
         else if (UNIFCart.battery)
            return UNIFCart.SaveGame[0];
         else
            data = NULL;
         break;
      case RETRO_MEMORY_SYSTEM_RAM:
         // TODO: add ExWRAM(0x2000) support ?
         data = RAM;
         break;
      default:
         data = NULL;
         break;
   }

   return data;
}

size_t retro_get_memory_size(unsigned type)
{
   unsigned size;

   switch(type)
   {
      case RETRO_MEMORY_SAVE_RAM:
         if (iNESCart.battery)
            size = iNESCart.SaveGameLen[0];
         else if (UNIFCart.battery)
            size = UNIFCart.SaveGameLen[0];
         else
            size = 0;
         break;
      case RETRO_MEMORY_SYSTEM_RAM:
         // TODO: detect ExWRAM(0x2000) ?
         // NES BASE RAM (0x800)
         size = 0x800;
         break;
      default:
         size = 0;
         break;
   }

   return size;
}
