#include <stdarg.h>
#include "mednafen/mednafen.h"
#include "mednafen/git.h"
#include "mednafen/general.h"
#include "libretro.h"

#include <retro_miscellaneous.h>
#include <string/stdstring.h>

#include "mednafen/pce_fast/pce.h"
#include "mednafen/pce_fast/vdc.h"
#include "mednafen/pce_fast/psg.h"
#include "mednafen/pce_fast/input.h"
#include "mednafen/pce_fast/huc.h"
#include "mednafen/pce_fast/pcecd.h"
#include "mednafen/pce_fast/pcecd_drive.h"
#include "mednafen/settings-driver.h"
#include "mednafen/hw_misc/arcade_card/arcade_card.h"
#include "mednafen/mempatcher.h"
#include "mednafen/cdrom/cdromif.h"
#include "mednafen/cdrom/CDUtility.h"

#ifdef _MSC_VER
#include "mednafen/msvc_compat.h"
#endif

#define MEDNAFEN_CORE_NAME_MODULE "pce_fast"
#define MEDNAFEN_CORE_NAME "Mednafen PCE Fast"
#define MEDNAFEN_CORE_VERSION "v0.9.38.7"
#define MEDNAFEN_CORE_EXTENSIONS "pce|cue|ccd|chd"
#define MEDNAFEN_CORE_TIMING_FPS 59.82
#define MEDNAFEN_CORE_GEOMETRY_BASE_W 512
#define MEDNAFEN_CORE_GEOMETRY_BASE_H 243
#define MEDNAFEN_CORE_GEOMETRY_MAX_W 512
#define MEDNAFEN_CORE_GEOMETRY_MAX_H 243
#define MEDNAFEN_CORE_GEOMETRY_ASPECT_RATIO (6.0 / 5.0)
#define FB_WIDTH 512
#define FB_HEIGHT 243

static bool old_cdimagecache = false;

extern MDFNGI EmulatedPCE_Fast;
MDFNGI *MDFNGameInfo = &EmulatedPCE_Fast;

/* Mednafen - Multi-system Emulator
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

static PCEFast_PSG *psg = NULL;
extern ArcadeCard *arcade_card; // Bah, lousy globals.

static Blip_Buffer sbuf[2];

bool PCE_ACEnabled;

int pce_overclocked;

// Statically allocated for speed...or something.
uint8 ROMSpace[0x88 * 8192 + 8192];	// + 8192 for PC-as-pointer safety padding

uint8 BaseRAM[32768 + 8192]; // 8KB for PCE, 32KB for Super Grafx // + 8192 for PC-as-pointer safety padding

uint8 PCEIODataBuffer;
readfunc PCERead[0x100];
writefunc PCEWrite[0x100];

static DECLFR(PCEBusRead)
{
 //printf("BUS Read: %02x %04x\n", A >> 13, A);
 return(0xFF);
}

static DECLFW(PCENullWrite)
{
 //printf("Null Write: %02x, %08x %02x\n", A >> 13, A, V);
}

static DECLFR(BaseRAMRead)
{
 return((BaseRAM - (0xF8 * 8192))[A]);
}

static DECLFR(BaseRAMRead_Mirrored)
{
 return(BaseRAM[A & 0x1FFF]);
}

static DECLFW(BaseRAMWrite)
{
 (BaseRAM - (0xF8 * 8192))[A] = V;
}

static DECLFW(BaseRAMWrite_Mirrored)
{
 BaseRAM[A & 0x1FFF] = V;
}

static DECLFR(IORead)
{
 #include "mednafen/pce_fast/ioread.inc"
}

static DECLFW(IOWrite)
{
 A &= 0x1FFF;
  
 switch(A >> 10)
 {
  case 0: HuC6280_StealCycle();
	       VDC_Write(A, V);
	       break;

  case 1: HuC6280_StealCycle();
	       VCE_Write(A, V);
	       break;

  case 2: PCEIODataBuffer = V;
	       psg->Write(HuCPU.timestamp / pce_overclocked, A, V);
	       break;

  case 3: PCEIODataBuffer = V;
	       HuC6280_TimerWrite(A, V);
	       break;

  case 4: PCEIODataBuffer = V; INPUT_Write(A, V); break;
  case 5: PCEIODataBuffer = V; HuC6280_IRQStatusWrite(A, V); break;
  case 6: 
	  if(!PCE_IsCD)
	   break;

	  if((A & 0x1E00) == 0x1A00)
	  {
	   if(arcade_card)
	    arcade_card->Write(A & 0x1FFF, V);
	  }
	  else
	  {
	   PCECD_Write(HuCPU.timestamp * 3, A, V);
	  }
	  break;

  case 7: break;	// Expansion.
 }
}

static void PCECDIRQCB(bool asserted)
{
 if(asserted)
  HuC6280_IRQBegin(MDFN_IQIRQ2);
 else
  HuC6280_IRQEnd(MDFN_IQIRQ2);
}

bool PCE_InitCD(void)
{
 PCECD_Settings cd_settings;
 memset(&cd_settings, 0, sizeof(PCECD_Settings));

 cd_settings.CDDA_Volume = (double)MDFN_GetSettingUI("pce_fast.cddavolume") / 100;
 cd_settings.CD_Speed = MDFN_GetSettingUI("pce_fast.cdspeed");

 cd_settings.ADPCM_Volume = (double)MDFN_GetSettingUI("pce_fast.adpcmvolume") / 100;
 cd_settings.ADPCM_LPF = MDFN_GetSettingB("pce_fast.adpcmlp");

 if(cd_settings.CDDA_Volume != 1.0)
  MDFN_printf(_("CD-DA Volume: %d%%\n"), (int)(100 * cd_settings.CDDA_Volume));

 if(cd_settings.ADPCM_Volume != 1.0)
  MDFN_printf(_("ADPCM Volume: %d%%\n"), (int)(100 * cd_settings.ADPCM_Volume));

 return(PCECD_Init(&cd_settings, PCECDIRQCB, PCE_MASTER_CLOCK, pce_overclocked, &sbuf[0], &sbuf[1]));
}


static int LoadCommon(void);
static void LoadCommonPre(void);

static bool TestMagic(const char *name, MDFNFILE *fp)
{
 return(TRUE);
}

static int Load(const char *name, MDFNFILE *fp)
{
 uint32 headerlen = 0;
 uint32 r_size;

 LoadCommonPre();

 {
  if(GET_FSIZE_PTR(fp) & 0x200) // 512 byte header!
   headerlen = 512;
 }

 r_size = GET_FSIZE_PTR(fp) - headerlen;
 if(r_size > 4096 * 1024) r_size = 4096 * 1024;

 for(int x = 0; x < 0x100; x++)
 {
  PCERead[x] = PCEBusRead;
  PCEWrite[x] = PCENullWrite;
 }

  HuCLoad(GET_FDATA_PTR(fp) + headerlen, GET_FSIZE_PTR(fp) - headerlen);

 return(LoadCommon());
}

static void LoadCommonPre(void)
{
 // FIXME:  Make these globals less global!
 pce_overclocked = MDFN_GetSettingUI("pce_fast.ocmultiplier");
 PCE_ACEnabled = MDFN_GetSettingB("pce_fast.arcadecard");

 if(pce_overclocked > 1)
  MDFN_printf(_("CPU overclock: %dx\n"), pce_overclocked);

 if(MDFN_GetSettingUI("pce_fast.cdspeed") > 1)
  MDFN_printf(_("CD-ROM speed:  %ux\n"), (unsigned int)MDFN_GetSettingUI("pce_fast.cdspeed"));

 memset(HuCPUFastMap, 0, sizeof(HuCPUFastMap));
 for(int x = 0; x < 0x100; x++)
 {
  PCERead[x] = PCEBusRead;
  PCEWrite[x] = PCENullWrite;
 }

 MDFNMP_Init(1024, (1 << 21) / 1024);
}

static int LoadCommon(void)
{ 
 VDC_Init(false);

 {
  PCERead[0xF8] = BaseRAMRead;
  PCERead[0xF9] = PCERead[0xFA] = PCERead[0xFB] = BaseRAMRead_Mirrored;

  PCEWrite[0xF8] = BaseRAMWrite;
  PCEWrite[0xF9] = PCEWrite[0xFA] = PCEWrite[0xFB] = BaseRAMWrite_Mirrored;

  for(int x = 0xf8; x < 0xfb; x++)
   HuCPUFastMap[x] = BaseRAM - x * 8192;

  PCERead[0xFF] = IORead;
 }

 MDFNMP_AddRAM(8192, 0xf8 * 8192, BaseRAM);

 PCEWrite[0xFF] = IOWrite;

 HuC6280_Init();

 psg = new PCEFast_PSG(&sbuf[0], &sbuf[1]);

 psg->SetVolume(1.0);

 if(PCE_IsCD)
 {
  unsigned int cdpsgvolume = MDFN_GetSettingUI("pce_fast.cdpsgvolume");

  if(cdpsgvolume != 100)
  {
   MDFN_printf(_("CD PSG Volume: %d%%\n"), cdpsgvolume);
  }

  psg->SetVolume(0.678 * cdpsgvolume / 100);

 }

 PCEINPUT_Init();

 PCE_Power();

#if 0
 MDFNGameInfo->LayerNames = "Background\0Sprites\0";
#endif
 MDFNGameInfo->fps = (uint32)((double)7159090.90909090 / 455 / 263 * 65536 * 256);
 return(1);
}

static bool TestMagicCD(std::vector<CDIF *> *CDInterfaces)
{
 static const uint8 magic_test[0x20] = { 0x82, 0xB1, 0x82, 0xCC, 0x83, 0x76, 0x83, 0x8D, 0x83, 0x4F, 0x83, 0x89, 0x83, 0x80, 0x82, 0xCC,
                                         0x92, 0x98, 0x8D, 0xEC, 0x8C, 0xA0, 0x82, 0xCD, 0x8A, 0x94, 0x8E, 0xAE, 0x89, 0xEF, 0x8E, 0xD0
                                       };
 uint8 sector_buffer[2048];
 CDIF *cdiface = (*CDInterfaces)[0];
 TOC toc;
 bool ret = FALSE;

 memset(sector_buffer, 0, sizeof(sector_buffer));

 cdiface->ReadTOC(&toc);

 for(int32 track = toc.first_track; track <= toc.last_track; track++)
 {
  if(toc.tracks[track].control & 0x4)
  {
   cdiface->ReadSector(sector_buffer, toc.tracks[track].lba, 1);

   if(!memcmp((char*)sector_buffer, (char *)magic_test, 0x20))
    ret = TRUE;

   // PCE CD BIOS apparently only looks at the first data track.
   break;
  }
 }


 // If it's a PC-FX CD(Battle Heat), return false.
 // This is very kludgy.
 for(int32 track = toc.first_track; track <= toc.last_track; track++)
 {
  if(toc.tracks[track].control & 0x4)
  {
   cdiface->ReadSector(sector_buffer, toc.tracks[track].lba, 1);
   if(!strncmp("PC-FX:Hu_CD-ROM", (char*)sector_buffer, strlen("PC-FX:Hu_CD-ROM")))
   {
    return(false);
   }
  }
 }


 // Now, test for the Games Express CD games.  The GE BIOS seems to always look at sector 0x10, but only if the first track is a
 // data track.
 if(toc.first_track == 1 && (toc.tracks[1].control & 0x4))
 {
  if(cdiface->ReadSector(sector_buffer, 0x10, 1))
  {
   if(!memcmp((char *)sector_buffer + 0x8, "HACKER CD ROM SYSTEM", 0x14))
   {
    ret = TRUE;
   }
  }
 }

 return(ret);
}

static int LoadCD(std::vector<CDIF *> *CDInterfaces)
{
 std::string bios_path = MDFN_MakeFName(MDFNMKF_FIRMWARE, 0, MDFN_GetSettingS("pce_fast.cdbios").c_str() );

 LoadCommonPre();

 if(!HuCLoadCD(bios_path.c_str()))
  return(0);

 PCECD_Drive_SetDisc(true, NULL, true);
 PCECD_Drive_SetDisc(false, (*CDInterfaces)[0], true);

 return(LoadCommon());
}

static void Cleanup_PCE(void)
{
   HuC_Close();

   VDC_Close();

   if(psg)
      delete psg;
   psg = NULL;
}

static void CloseGame(void)
{
   Cleanup_PCE();
}

static void Emulate(EmulateSpecStruct *espec)
{
 INPUT_Frame();

 MDFNMP_ApplyPeriodicCheats();

 #if 0
 {
  static bool firstcat = true;
  MDFN_PixelFormat nf;

  nf.bpp = 16;
  nf.colorspace = MDFN_COLORSPACE_RGB;
  nf.Rshift = 11;
  nf.Gshift = 5;
  nf.Bshift = 0;
  nf.Ashift = 16;
  
  nf.Rprec = 5;
  nf.Gprec = 6;
  nf.Bprec = 5;
  nf.Aprec = 8;

  espec->surface->SetFormat(nf, false);
  espec->VideoFormatChanged = firstcat;
  firstcat = false;
 }
 #endif


 if(espec->SoundFormatChanged)
 {
  for(int y = 0; y < 2; y++)
  {
     Blip_Buffer_set_sample_rate(&sbuf[y],
           espec->SoundRate ? espec->SoundRate : 44100, 50);
     Blip_Buffer_set_clock_rate(&sbuf[y], (long)(PCE_MASTER_CLOCK / 3));
     Blip_Buffer_bass_freq(&sbuf[y], 10);
  }
 }
 VDC_RunFrame(espec, false);

 if(PCE_IsCD)
 {
  PCECD_Run(HuCPU.timestamp * 3);
 }

 psg->EndFrame(HuCPU.timestamp / pce_overclocked);

 if(espec->SoundBuf)
 {
  for(int y = 0; y < 2; y++)
  {
     Blip_Buffer_end_frame(&sbuf[y], HuCPU.timestamp / pce_overclocked);
     espec->SoundBufSize = Blip_Buffer_read_samples(&sbuf[y], espec->SoundBuf + y,
           espec->SoundBufMaxSize);
  }
 }

 espec->MasterCycles = HuCPU.timestamp * 3;

 INPUT_FixTS();

 HuC6280_ResetTS();

 if(PCE_IsCD)
  PCECD_ResetTS();
}

int StateAction(StateMem *sm, int load, int data_only)
{
 SFORMAT StateRegs[] =
 {
  SFARRAY(BaseRAM, 8192),
  SFVAR(PCEIODataBuffer),
  SFEND
 };

 //for(int i = 8192; i < 32768; i++)
 // if(BaseRAM[i] != 0xFF)
 //  printf("%d %02x\n", i, BaseRAM[i]);

 int ret = MDFNSS_StateAction(sm, load, data_only, StateRegs, "MAIN");

 ret &= HuC6280_StateAction(sm, load, data_only);
 ret &= VDC_StateAction(sm, load, data_only);
 ret &= psg->StateAction(sm, load, data_only);
 ret &= INPUT_StateAction(sm, load, data_only);
 ret &= HuC_StateAction(sm, load, data_only);

 if(load)
 {

 }

 return(ret);
}

void PCE_Power(void)
{
 memset(BaseRAM, 0x00, sizeof(BaseRAM));

 for(int i = 8192; i < 32768; i++)
    BaseRAM[i] = 0xFF;

 PCEIODataBuffer = 0xFF;

 HuC6280_Power();
 VDC_Power();
 psg->Power(HuCPU.timestamp / pce_overclocked);
 HuC_Power();

 if(PCE_IsCD)
 {
  PCECD_Power(HuCPU.timestamp * 3);
 }
}

static void DoSimpleCommand(int cmd)
{
 switch(cmd)
 {
  case MDFN_MSC_RESET: PCE_Power(); break;
  case MDFN_MSC_POWER: PCE_Power(); break;
 }
}

static MDFNSetting PCESettings[] = 
{
  { "pce_fast.slstart", MDFNSF_NOFLAGS, "First rendered scanline.", NULL, MDFNST_UINT, "4", "0", "239" },
  { "pce_fast.slend", MDFNSF_NOFLAGS, "Last rendered scanline.", NULL, MDFNST_UINT, "235", "0", "239" },
  { "pce_fast.mouse_sensitivity", MDFNSF_NOFLAGS, "Mouse sensitivity.", NULL, MDFNST_FLOAT, "0.50", NULL, NULL, NULL, PCEINPUT_SettingChanged },
  { "pce_fast.disable_softreset", MDFNSF_NOFLAGS, "If set, when RUN+SEL are pressed simultaneously, disable both buttons temporarily.", NULL, MDFNST_BOOL, "0", NULL, NULL, NULL, PCEINPUT_SettingChanged },
  { "pce_fast.arcadecard", MDFNSF_EMU_STATE | MDFNSF_UNTRUSTED_SAFE, "Enable Arcade Card emulation.", NULL, MDFNST_BOOL, "1" },
  { "pce_fast.ocmultiplier", MDFNSF_EMU_STATE | MDFNSF_UNTRUSTED_SAFE, "CPU overclock multiplier.", NULL, MDFNST_UINT, "1", "1", "100"},
  { "pce_fast.cdspeed", MDFNSF_EMU_STATE | MDFNSF_UNTRUSTED_SAFE, "CD-ROM data transfer speed multiplier.", NULL, MDFNST_UINT, "1", "1", "100" },
  { "pce_fast.nospritelimit", MDFNSF_NOFLAGS, "Remove 16-sprites-per-scanline hardware limit.", NULL, MDFNST_BOOL, "0" },
  { "pce_fast.hoverscan", MDFNSF_NOFLAGS, "Reduce 352 pixels width overscan.", NULL, MDFNST_UINT, "300", "300", "352" },

  { "pce_fast.cdbios", MDFNSF_EMU_STATE, "Path to the CD BIOS", NULL, MDFNST_STRING, "syscard3.pce" },

  { "pce_fast.adpcmlp", MDFNSF_NOFLAGS, "Enable dynamic ADPCM lowpass filter.", NULL, MDFNST_BOOL, "0" },
  { "pce_fast.cdpsgvolume", MDFNSF_NOFLAGS, "PSG volume when playing a CD game.", NULL, MDFNST_UINT, "100", "0", "200" },
  { "pce_fast.cddavolume", MDFNSF_NOFLAGS, "CD-DA volume.", NULL, MDFNST_UINT, "100", "0", "200" },
  { "pce_fast.adpcmvolume", MDFNSF_NOFLAGS, "ADPCM volume.", NULL, MDFNST_UINT, "100", "0", "200" },
  { NULL }
};

uint8 MemRead(uint32 addr)
{
 return(PCERead[(addr / 8192) & 0xFF](addr));
}

static const FileExtensionSpecStruct KnownExtensions[] =
{
 { ".pce", "PC Engine ROM Image" },
 { NULL, NULL }
};

static const uint8 BRAM_Init_String[8] = { 'H', 'U', 'B', 'M', 0x00, 0x88, 0x10, 0x80 }; //"HUBM\x00\x88\x10\x80";

ArcadeCard *arcade_card = NULL;

static uint8 *HuCROM = NULL;

static bool IsPopulous;
bool PCE_IsCD;

uint8 SaveRAM[2048];

static DECLFW(ACPhysWrite)
{
 arcade_card->PhysWrite(A, V);
}

static DECLFR(ACPhysRead)
{
 return(arcade_card->PhysRead(A));
}

static DECLFR(SaveRAMRead)
{
 if((!PCE_IsCD || PCECD_IsBRAMEnabled()) && (A & 8191) < 2048)
  return(SaveRAM[A & 2047]);
 else
  return(0xFF);
}

static DECLFW(SaveRAMWrite)
{
 if((!PCE_IsCD || PCECD_IsBRAMEnabled()) && (A & 8191) < 2048)
  SaveRAM[A & 2047] = V;
}

static DECLFR(HuCRead)
{
 return ROMSpace[A];
}

static DECLFW(HuCRAMWrite)
{
 ROMSpace[A] = V;
}

static DECLFW(HuCRAMWriteCDSpecial) // Hyper Dyne Special hack
{
 BaseRAM[0x2000 | (A & 0x1FFF)] = V;
 ROMSpace[A] = V;
}

static uint8 HuCSF2Latch = 0;

static DECLFR(HuCSF2Read)
{
 return(HuCROM[(A & 0x7FFFF) + 0x80000 + HuCSF2Latch * 0x80000 ]); // | (HuCSF2Latch << 19) ]);
}

static DECLFW(HuCSF2Write)
{
 if((A & 0x1FFC) == 0x1FF0)
 {
  HuCSF2Latch = (A & 0x3);
 }
}

static void Cleanup(void)
{
   if(arcade_card)
      delete arcade_card;
   arcade_card = NULL;

   if(PCE_IsCD)
      PCECD_Close();

   if(HuCROM)
      free(HuCROM);
   HuCROM = NULL;
}

int HuCLoad(const uint8 *data, uint32 len)
{
 uint32 sf2_threshold = 2048 * 1024;
 uint32 sf2_required_size = 2048 * 1024 + 512 * 1024;
 uint32 m_len = (len + 8191)&~8191;
 bool sf2_mapper = FALSE;

 if(m_len >= sf2_threshold)
 {
  sf2_mapper = TRUE;

  if(m_len != sf2_required_size)
   m_len = sf2_required_size;
 }

 IsPopulous = 0;
 PCE_IsCD = 0;


 MDFN_printf(_("ROM:       %dKiB\n"), (len + 1023) / 1024);

 if(!(HuCROM = (uint8 *)malloc(m_len)))
 {
  return(0);
 }

 memset(HuCROM, 0xFF, m_len);
 memcpy(HuCROM, data, (m_len < len) ? m_len : len);

 memset(ROMSpace, 0xFF, 0x88 * 8192 + 8192);

 if(m_len == 0x60000)
 {
  memcpy(ROMSpace + 0x00 * 8192, HuCROM, 0x20 * 8192);
  memcpy(ROMSpace + 0x20 * 8192, HuCROM, 0x20 * 8192);
  memcpy(ROMSpace + 0x40 * 8192, HuCROM + 0x20 * 8192, 0x10 * 8192);
  memcpy(ROMSpace + 0x50 * 8192, HuCROM + 0x20 * 8192, 0x10 * 8192);
  memcpy(ROMSpace + 0x60 * 8192, HuCROM + 0x20 * 8192, 0x10 * 8192);
  memcpy(ROMSpace + 0x70 * 8192, HuCROM + 0x20 * 8192, 0x10 * 8192);
 }
 else if(m_len == 0x80000)
 {
  memcpy(ROMSpace + 0x00 * 8192, HuCROM, 0x40 * 8192);
  memcpy(ROMSpace + 0x40 * 8192, HuCROM + 0x20 * 8192, 0x20 * 8192);
  memcpy(ROMSpace + 0x60 * 8192, HuCROM + 0x20 * 8192, 0x20 * 8192);
 }
 else
 {
  memcpy(ROMSpace + 0x00 * 8192, HuCROM, (m_len < 1024 * 1024) ? m_len : 1024 * 1024);
 }

 for(int x = 0x00; x < 0x80; x++)
 {
  HuCPUFastMap[x] = ROMSpace;
  PCERead[x] = HuCRead;
 }

 if(!memcmp(HuCROM + 0x1F26, "POPULOUS", strlen("POPULOUS")))
 {
  uint8 *PopRAM = ROMSpace + 0x40 * 8192;
  
  memset(PopRAM, 0xFF, 32768);

  IsPopulous = 1;

  MDFN_printf("Populous\n");

  for(int x = 0x40; x < 0x44; x++)
  {
   HuCPUFastMap[x] = &PopRAM[(x & 3) * 8192] - x * 8192;
   PCERead[x] = HuCRead;
   PCEWrite[x] = HuCRAMWrite;
  }
  MDFNMP_AddRAM(32768, 0x40 * 8192, PopRAM);
 }
 else
 {
  memset(SaveRAM, 0x00, 2048);
  memcpy(SaveRAM, BRAM_Init_String, 8);    // So users don't have to manually intialize the file cabinet
                                                // in the CD BIOS screen.
  PCEWrite[0xF7] = SaveRAMWrite;
  PCERead[0xF7] = SaveRAMRead;
  MDFNMP_AddRAM(2048, 0xF7 * 8192, SaveRAM);
 }

 // 0x1A558
 //if(len >= 0x20000 && !memcmp(HuCROM + 0x1A558, "STREET FIGHTER#", strlen("STREET FIGHTER#")))
 if(sf2_mapper)
 {
  for(int x = 0x40; x < 0x80; x++)
  {
   // FIXME: PCE_FAST
   HuCPUFastMap[x] = NULL; // Make sure our reads go through our read function, and not a table lookup
   PCERead[x] = HuCSF2Read;
  }
  PCEWrite[0] = HuCSF2Write;
  MDFN_printf("Street Fighter 2 Mapper\n");
  HuCSF2Latch = 0;
 }

 return(1);
}
 
bool IsBRAMUsed(void)
{
 if(memcmp(SaveRAM, BRAM_Init_String, 8)) // HUBM string is modified/missing
  return(1);

 for(int x = 8; x < 2048; x++)
  if(SaveRAM[x]) return(1);

 return(0);
}

int HuCLoadCD(const char *bios_path)
{
 static const FileExtensionSpecStruct KnownBIOSExtensions[] =
 {
  { ".pce", "PC Engine ROM Image" },
  { ".bin", "PC Engine ROM Image" },
  { ".bios", "BIOS Image" },
  { NULL, NULL }
 };

 MDFNFILE *fp = file_open(bios_path);

 if(!fp)
  return(0);

 memset(ROMSpace, 0xFF, 262144);

 memcpy(ROMSpace, GET_FDATA_PTR(fp) + (GET_FSIZE_PTR(fp) & 512), ((GET_FSIZE_PTR(fp) & ~512) > 262144) ? 262144 : (GET_FSIZE_PTR(fp) &~ 512) );

 if (fp)
    file_close(fp);
 fp = NULL;

 PCE_IsCD = 1;
 PCE_InitCD();

 MDFN_printf(_("Arcade Card Emulation:  %s\n"), PCE_ACEnabled ? _("Enabled") : _("Disabled")); 
 for(int x = 0; x < 0x40; x++)
 {
  HuCPUFastMap[x] = ROMSpace;
  PCERead[x] = HuCRead;
 }

 for(int x = 0x68; x < 0x88; x++)
 {
  HuCPUFastMap[x] = ROMSpace;
  PCERead[x] = HuCRead;
  PCEWrite[x] = HuCRAMWrite;
 }
 PCEWrite[0x80] = HuCRAMWriteCDSpecial; 	// Hyper Dyne Special hack
 MDFNMP_AddRAM(262144, 0x68 * 8192, ROMSpace + 0x68 * 8192);

 if(PCE_ACEnabled)
 {
    if (!(arcade_card = new ArcadeCard()))
    {
       MDFN_PrintError(_("Error creating %s object.\n"), "ArcadeCard");
       Cleanup();
       return(0);
    }

    for(int x = 0x40; x < 0x44; x++)
    {
       HuCPUFastMap[x] = NULL;
       PCERead[x] = ACPhysRead;
       PCEWrite[x] = ACPhysWrite;
    }
 }

 memset(SaveRAM, 0x00, 2048);
 memcpy(SaveRAM, BRAM_Init_String, 8);	// So users don't have to manually intialize the file cabinet
						// in the CD BIOS screen.

 PCEWrite[0xF7] = SaveRAMWrite;
 PCERead[0xF7] = SaveRAMRead;
 MDFNMP_AddRAM(2048, 0xF7 * 8192, SaveRAM);
 return(1);
}

int HuC_StateAction(StateMem *sm, int load, int data_only)
{
 SFORMAT StateRegs[] = 
 {
  SFARRAY(ROMSpace + 0x40 * 8192, IsPopulous ? 32768 : 0),
  SFARRAY(SaveRAM, IsPopulous ? 0 : 2048),
  SFARRAY(ROMSpace + 0x68 * 8192, PCE_IsCD ? 262144 : 0),
  SFVAR(HuCSF2Latch),
  SFEND
 };
 int ret = MDFNSS_StateAction(sm, load, data_only, StateRegs, "HuC");

 if(load)
  HuCSF2Latch &= 0x3;

 if(PCE_IsCD)
 {
  ret &= PCECD_StateAction(sm, load, data_only);

  if(arcade_card)
   ret &= arcade_card->StateAction(sm, load, data_only);
 }
 return(ret);
}

void HuC_Close(void)
{
   Cleanup();
}

void HuC_Power(void)
{
 if(PCE_IsCD)
  memset(ROMSpace + 0x68 * 8192, 0x00, 262144);

 if(arcade_card)
  arcade_card->Power();
}

MDFNGI EmulatedPCE_Fast =
{
 PCESettings,
 MDFN_MASTERCLOCK_FIXED(PCE_MASTER_CLOCK),
 0,

 true,  // Multires possible?

 0,   // lcm_width
 0,   // lcm_height           
 NULL,  // Dummy

 MEDNAFEN_CORE_GEOMETRY_BASE_W,   // Nominal width
 MEDNAFEN_CORE_GEOMETRY_BASE_H,   // Nominal height

 FB_WIDTH,	// Framebuffer width
 FB_HEIGHT,	// Framebuffer height

 2,     // Number of output sound channels
};

static bool ReadM3U(std::vector<std::string> &file_list, std::string path, unsigned depth = 0)
{
   std::string dir_path;
   char linebuf[2048];
   FILE *fp = fopen(path.c_str(), "rb");

   if (!fp)
      return false;

   MDFN_GetFilePathComponents(path, &dir_path);

   while(fgets(linebuf, sizeof(linebuf), fp))
   {
      std::string efp;

      if(linebuf[0] == '#')
         continue;
      string_trim_whitespace_right(linebuf);
      if(linebuf[0] == 0)
         continue;

      efp = MDFN_EvalFIP(dir_path, std::string(linebuf));

      if(efp.size() >= 4 && efp.substr(efp.size() - 4) == ".m3u")
      {
         if(efp == path)
         {
            log_cb(RETRO_LOG_ERROR, "M3U at \"%s\" references self.\n", efp.c_str());
            fclose(fp);
            return false;
         }

         if(depth == 99)
         {
            log_cb(RETRO_LOG_ERROR, "M3U load recursion too deep!\n");
            fclose(fp);
            return false;
         }

         ReadM3U(file_list, efp, depth++);
      }
      else
         file_list.push_back(efp);
   }

   fclose(fp);

   return true;
}

 static std::vector<CDIF *> CDInterfaces;	// FIXME: Cleanup on error out.
// TODO: LoadCommon()

static MDFNGI *MDFNI_LoadCD(const char *devicename)
{
   bool ret = false;
   log_cb(RETRO_LOG_INFO, "Loading %s...\n\n", devicename);

   if(devicename && strlen(devicename) > 4 && !strcasecmp(devicename + strlen(devicename) - 4, ".m3u"))
   {
      std::vector<std::string> file_list;

      if (ReadM3U(file_list, devicename))
         ret = true;

      for(unsigned i = 0; i < file_list.size(); i++)
      {
         CDIF *cdif = CDIF_Open(file_list[i].c_str(), false);
         CDInterfaces.push_back(cdif);
      }
   }
   else
   {
      CDIF *cdif = CDIF_Open(devicename, false);

      if (cdif)
      {
         ret = true;
         CDInterfaces.push_back(cdif);
      }
   }

   if (!ret)
   {
      log_cb(RETRO_LOG_ERROR, "Error opening CD.\n");
      return NULL;
   }

   /* Print out a track list for all discs. */
   MDFN_indent(1);
   for(unsigned i = 0; i < CDInterfaces.size(); i++)
   {
      TOC toc;

      CDInterfaces[i]->ReadTOC(&toc);

      MDFN_printf(_("CD %d Layout:\n"), i + 1);
      MDFN_indent(1);

      for(int32 track = toc.first_track; track <= toc.last_track; track++)
      {
         MDFN_printf(_("Track %2d, LBA: %6d  %s\n"), track, toc.tracks[track].lba, (toc.tracks[track].control & 0x4) ? "DATA" : "AUDIO");
      }

      MDFN_printf("Leadout: %6d\n", toc.tracks[100].lba);
      MDFN_indent(-1);
      MDFN_printf("\n");
   }
   MDFN_indent(-1);

   MDFN_printf("Using module: pce_fast.\n");

   if(!(LoadCD(&CDInterfaces)))
   {
      for(unsigned i = 0; i < CDInterfaces.size(); i++)
         delete CDInterfaces[i];
      CDInterfaces.clear();

      MDFNGameInfo = NULL;
      return(0);
   }

   //MDFNI_SetLayerEnableMask(~0ULL);

   MDFN_LoadGameCheats(NULL);
   MDFNMP_InstallReadPatches();

   return(MDFNGameInfo);
}

static MDFNGI *MDFNI_LoadGame(const char *name)
{
   std::vector<FileExtensionSpecStruct> valid_iae;
   MDFNFILE *GameFile = NULL;
   MDFNGameInfo = &EmulatedPCE_Fast;

   if(strlen(name) > 4 && (!strcasecmp(name + strlen(name) - 4, ".cue") || !strcasecmp(name + strlen(name) - 4, ".ccd") || !strcasecmp(name + strlen(name) - 4, ".chd") || !strcasecmp(name + strlen(name) - 4, ".toc") || !strcasecmp(name + strlen(name) - 4, ".m3u")))
      return(MDFNI_LoadCD(name));

   MDFN_printf(_("Loading %s...\n"),name);

   MDFN_indent(1);

   // Construct a NULL-delimited list of known file extensions for MDFN_fopen()
   const FileExtensionSpecStruct *curexts = KnownExtensions;

   while(curexts->extension && curexts->description)
   {
      valid_iae.push_back(*curexts);
      curexts++;
   }

   GameFile = file_open(name);

   if(!GameFile)
      goto error;

   MDFN_printf(_("Using module: pce_fast.\n\n"));
   MDFN_indent(1);

   //
   // Load per-game settings
   //
   // Maybe we should make a "pgcfg" subdir, and automatically load all files in it?
   // End load per-game settings
   //

   if(Load(name, GameFile) <= 0)
      goto error;

   MDFN_LoadGameCheats(NULL);
   MDFNMP_InstallReadPatches();

   MDFN_indent(-2);

   return(MDFNGameInfo);

error:
   if (GameFile)
      file_close(GameFile);
   MDFNGameInfo = NULL;
   return NULL;
}

static int curindent = 0;

void MDFN_indent(int indent)
{
 curindent += indent;
}

static uint8 lastchar = 0;

void MDFN_printf(const char *format, ...)
{
   char *format_temp;
   char *temp;
   unsigned int x, newlen;

   va_list ap;
   va_start(ap,format);


   // First, determine how large our format_temp buffer needs to be.
   uint8 lastchar_backup = lastchar; // Save lastchar!
   for(newlen=x=0;x<strlen(format);x++)
   {
      if(lastchar == '\n' && format[x] != '\n')
      {
         int y;
         for(y=0;y<curindent;y++)
            newlen++;
      }
      newlen++;
      lastchar = format[x];
   }

   format_temp = (char *)malloc(newlen + 1); // Length + NULL character, duh

   // Now, construct our format_temp string
   lastchar = lastchar_backup; // Restore lastchar
   for(newlen=x=0;x<strlen(format);x++)
   {
      if(lastchar == '\n' && format[x] != '\n')
      {
         int y;
         for(y=0;y<curindent;y++)
            format_temp[newlen++] = ' ';
      }
      format_temp[newlen++] = format[x];
      lastchar = format[x];
   }

   format_temp[newlen] = 0;

   temp = new char[4096];
   vsnprintf(temp, 4096, format_temp, ap);
   free(format_temp);

   MDFND_Message(temp);
   free(temp);

   va_end(ap);
}

void MDFN_PrintError(const char *format, ...)
{
 char *temp;

 va_list ap;

 va_start(ap, format);

 temp = new char[4096];
 vsnprintf(temp, 4096, format, ap);
 MDFND_PrintError(temp);
 free(temp);

 va_end(ap);
}

void MDFN_DebugPrintReal(const char *file, const int line, const char *format, ...)
{
 char *temp;

 va_list ap;

 va_start(ap, format);

 temp = new char[4096];
 vsnprintf(temp, 4096, format, ap);
 fprintf(stderr, "%s:%d  %s\n", file, line, temp);
 free(temp);

 va_end(ap);
}


static MDFNGI *game;

struct retro_perf_callback perf_cb;
retro_get_cpu_features_t perf_get_cpu_features_cb = NULL;
retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static double last_sound_rate;

static MDFN_Surface *surf;

static bool failed_init;

std::string retro_base_directory;
std::string retro_base_name;
std::string retro_save_directory;

static void set_basename(const char *path)
{
   const char *base = strrchr(path, '/');
   if (!base)
      base = strrchr(path, '\\');

   if (base)
      retro_base_name = base + 1;
   else
      retro_base_name = path;

   retro_base_name = retro_base_name.substr(0, retro_base_name.find_last_of('.'));
}

#include "mednafen/pce_fast/pcecd.h"

static void check_system_specs(void)
{
   unsigned level = 5;
   environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
}

void retro_init(void)
{
   struct retro_log_callback log;
   if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      log_cb = log.log;
   else 
      log_cb = NULL;

   CDUtility_Init();

   const char *dir = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
   {
      retro_base_directory = dir;
      // Make sure that we don't have any lingering slashes, etc, as they break Windows.
      size_t last = retro_base_directory.find_last_not_of("/\\");
      if (last != std::string::npos)
         last++;

      retro_base_directory = retro_base_directory.substr(0, last);
   }
   else
   {
      /* TODO: Add proper fallback */
      if (log_cb)
         log_cb(RETRO_LOG_WARN, "System directory is not defined. Fallback on using same dir as ROM for system directory later ...\n");
      failed_init = true;
   }
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &dir) && dir)
   {
	  // If save directory is defined use it, otherwise use system directory
      retro_save_directory = *dir ? dir : retro_base_directory;
      // Make sure that we don't have any lingering slashes, etc, as they break Windows.
      size_t last = retro_save_directory.find_last_not_of("/\\");
      if (last != std::string::npos)
         last++;

      retro_save_directory = retro_save_directory.substr(0, last);      
   }
   else
   {
      /* TODO: Add proper fallback */
      if (log_cb)
         log_cb(RETRO_LOG_WARN, "Save directory is not defined. Fallback on using SYSTEM directory ...\n");
	  retro_save_directory = retro_base_directory;
   }      

   enum retro_pixel_format rgb565 = RETRO_PIXEL_FORMAT_RGB565;
   if (environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &rgb565) && log_cb)
      log_cb(RETRO_LOG_INFO, "Frontend supports RGB565 - will use that instead of XRGB1555.\n");

   if (environ_cb(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb))
      perf_get_cpu_features_cb = perf_cb.get_cpu_features;
   else
      perf_get_cpu_features_cb = NULL;
   
   bool yes = true;
   environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS, &yes);
   
   setting_initial_scanline = 0;
   setting_last_scanline = 242;

   check_system_specs();
}

void retro_reset(void)
{
   DoSimpleCommand(MDFN_MSC_RESET);
}

bool retro_load_game_special(unsigned, const struct retro_game_info *, size_t)
{
   return false;
}

static void set_volume (uint32_t *ptr, unsigned number)
{
   switch(number)
   {
      default:
         *ptr = number;
         break;
   }
}

#define MAX_PLAYERS 5
#define MAX_BUTTONS 13
static uint8_t input_buf[MAX_PLAYERS][2] = {0};

// Array to keep track of whether a given player's button is turbo
static int turbo_enable[MAX_PLAYERS][MAX_BUTTONS] = {};
// Array to keep track of each buttons turbo status
static int turbo_counter[MAX_PLAYERS][MAX_BUTTONS] = {};
// The number of frames between each firing of a turbo button
static int Turbo_Delay;
static int Turbo_Toggling = 1;
static int turbo_toggle_down[MAX_PLAYERS][MAX_BUTTONS] = {};

static void check_variables(void)
{
   struct retro_variable var = {0};

   var.key = "pce_fast_cdimagecache";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool cdimage_cache = true;

      if (strcmp(var.value, "enabled") == 0)
         cdimage_cache = true;
      else if (strcmp(var.value, "disabled") == 0)
         cdimage_cache = false;

      if (cdimage_cache != old_cdimagecache)
         old_cdimagecache = cdimage_cache;
   }

   var.key = "pce_fast_cdbios";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "System Card 3") == 0)
         setting_pce_fast_cdbios = "syscard3.pce";
      else if (strcmp(var.value, "System Card 2") == 0)
         setting_pce_fast_cdbios = "syscard2.pce";
      else if (strcmp(var.value, "System Card 1") == 0)
         setting_pce_fast_cdbios = "syscard1.pce";
      else if (strcmp(var.value, "Games Express") == 0)
         setting_pce_fast_cdbios = "gexpress.pce";
   }

   var.key = "pce_nospritelimit";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "disabled") == 0)
         setting_pce_fast_nospritelimit = 0;
      else if (strcmp(var.value, "enabled") == 0)
         setting_pce_fast_nospritelimit = 1;
   }
   
   var.key = "pce_ocmultiplier";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      setting_pce_overclocked = atoi(var.value);
   }
   	
	var.key = "pce_hoverscan";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      setting_pce_hoverscan = atoi(var.value);
   }
	
   var.key = "pce_initial_scanline";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      setting_initial_scanline = atoi(var.value);
   }

   var.key = "pce_last_scanline";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      setting_last_scanline = atoi(var.value);
   }

   bool do_cdsettings = false;
   var.key = "pce_cddavolume";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      do_cdsettings = true;
      setting_pce_fast_cddavolume = atoi(var.value);
   }

   var.key = "pce_adpcmvolume";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      do_cdsettings = true;
      setting_pce_fast_adpcmvolume = atoi(var.value);
   }

   var.key = "pce_cdpsgvolume";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      do_cdsettings = true;
      setting_pce_fast_cdpsgvolume = atoi(var.value);
   }

   var.key = "pce_cdspeed";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      do_cdsettings = true;
      setting_pce_fast_cdspeed = atoi(var.value);
   }

   if (do_cdsettings)
   {
      PCECD_Settings settings = {0};
      settings.CDDA_Volume = (double)setting_pce_fast_cddavolume / 100;
      settings.CD_Speed = setting_pce_fast_cdspeed;
      settings.ADPCM_Volume = (double)setting_pce_fast_adpcmvolume / 100;

      if (PCECD_SetSettings(&settings) && log_cb)
         log_cb(RETRO_LOG_INFO, "PCE CD Audio settings changed.\n");
   }

   // Set Turbo_Toggling
   var.key = "Turbo_Toggling";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
         Turbo_Toggling = 1;
      else
         Turbo_Toggling = 0;
   }

   // Set TURBO_DELAY 
   var.key = "pce_Turbo_Delay";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "Fast") == 0)
      Turbo_Delay = 3;
      else if (strcmp(var.value, "Medium") == 0)
      Turbo_Delay = 8;
      else if (strcmp(var.value, "Slow") == 0)
      Turbo_Delay = 15;
   }

   // Enable turbo for each player's I+II buttons   
   var.key = "pce_p0_turbo_I_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[0][0] = 1;
	  else
		turbo_enable[0][0] = 0;
   }
   
   var.key = "pce_p0_turbo_II_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[0][1] = 1;
	  else
		turbo_enable[0][1] = 0;
   }
   
   var.key = "pce_p1_turbo_I_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[1][0] = 1;
  	  else
		turbo_enable[1][0] = 0;
   }
   
   var.key = "pce_p1_turbo_II_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[1][1] = 1;
	  else
		turbo_enable[1][1] = 0;
   }
   
   var.key = "pce_p2_turbo_I_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[2][0] = 1;
	  else
		turbo_enable[2][0] = 0;
   }
   
   var.key = "pce_p2_turbo_II_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[2][1] = 1;
	  else
		turbo_enable[2][1] = 0;
   }
   
      var.key = "pce_p3_turbo_I_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[3][0] = 1;
	  else
		turbo_enable[3][0] = 0;
   }
   
   var.key = "pce_p3_turbo_II_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[3][1] = 1;
	  else
		turbo_enable[3][1] = 0;
   }
   
   var.key = "pce_p4_turbo_I_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[4][0] = 1;
	  else
		turbo_enable[4][0] = 0;
   }
   
   var.key = "pce_p4_turbo_II_enable";
   
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (strcmp(var.value, "enabled") == 0)
		turbo_enable[4][1] = 1;
	  else
		turbo_enable[4][1] = 0;
   }
	  
}

bool retro_load_game(const struct retro_game_info *info)
{
   struct retro_input_descriptor desc[] = {
      #define button_ids(INDEX) \
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "II" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "I" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "IV" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "III" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "V" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "VI" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "Mode Switch" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,    "Select" },\
      { INDEX, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Run" },
      button_ids(0)
      button_ids(1)
      button_ids(2)
      button_ids(3)
      button_ids(4)
	  
      { 0 },
   };

   if (!info || failed_init)
      return false;

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
   set_basename(info->path);

   check_variables();

   game = MDFNI_LoadGame(info->path);
   if (!game)
      return false;

   surf = (MDFN_Surface*)calloc(1, sizeof(*surf));

   if (!surf)
      return false;
   
   surf->width  = FB_WIDTH;
   surf->height = FB_HEIGHT;
   surf->pitch  = FB_WIDTH;
   surf->pixels = (uint16_t*) calloc(2, FB_WIDTH * FB_HEIGHT);

   if (!surf->pixels)
   {
      free(surf);
      return false;
   }

   // Possible endian bug ...
   for (unsigned i = 0; i < MAX_PLAYERS; i++)
      PCEINPUT_SetInput(i, "gamepad", &input_buf[i][0]);

   VDC_SetPixelFormat();

   return game;
}

void retro_unload_game(void)
{
   if(!MDFNGameInfo)
      return;

   MDFN_FlushGameCheats(0);

   CloseGame();

   MDFNMP_Kill();

   MDFNGameInfo = NULL;

   for(unsigned i = 0; i < CDInterfaces.size(); i++)
      delete CDInterfaces[i];
   CDInterfaces.clear();
}

static void update_input(void)
{
   static int turbo_map[] = { -1,-1,-1,-1,-1,-1,-1,-1,1,0,-1,-1,-1 };
   static unsigned map[] = {
      RETRO_DEVICE_ID_JOYPAD_A,
      RETRO_DEVICE_ID_JOYPAD_B,
      RETRO_DEVICE_ID_JOYPAD_SELECT,
      RETRO_DEVICE_ID_JOYPAD_START,
      RETRO_DEVICE_ID_JOYPAD_UP,
      RETRO_DEVICE_ID_JOYPAD_RIGHT,
      RETRO_DEVICE_ID_JOYPAD_DOWN,
      RETRO_DEVICE_ID_JOYPAD_LEFT,
      RETRO_DEVICE_ID_JOYPAD_Y,
      RETRO_DEVICE_ID_JOYPAD_X,
      RETRO_DEVICE_ID_JOYPAD_L,
      RETRO_DEVICE_ID_JOYPAD_R,
      RETRO_DEVICE_ID_JOYPAD_L2
   };

   for (unsigned j = 0; j < MAX_PLAYERS; j++)
   {
      uint16_t input_state = 0;
      for (unsigned i = 0; i < MAX_BUTTONS; i++)
      {
         if (turbo_enable[j][i] == 1) //Check whether a given button is turbo-capable
         {
            if (turbo_counter[j][i] == (Turbo_Delay+1)) //Turbo buttons only fire when their counter exceeds the turbo delay
            {
               input_state |= input_state_cb(j, RETRO_DEVICE_JOYPAD, 0, map[i]) ? (1 << i) : 0;
            }
            else
            {
               turbo_counter[j][i]++; //Otherwise, their counter is incremented by 1
            }
            if (turbo_counter[j][i] > (Turbo_Delay)) //When the counter exceeds turbo delay, fire and return to zero
            {
               input_state |= input_state_cb(j, RETRO_DEVICE_JOYPAD, 0, map[i]) ? (1 << i) : 0;
               turbo_counter[j][i] = 0;
            }
         }
         else if (turbo_map[i] != -1 && Turbo_Toggling && !AVPad6Enabled[j])
         {
            if (input_state_cb(j, RETRO_DEVICE_JOYPAD, 0, map[i]))
            {
               if (turbo_toggle_down[j][i] == 0)
               {
                  turbo_toggle_down[j][i] = 1;
                  turbo_enable[j][turbo_map[i]] = turbo_enable[j][turbo_map[i]] ^ 1;
                  MDFN_DispMessage("Pad %i Button %s Turbo %s", j + 1, i == 9 ? "I" : "II", turbo_enable[j][turbo_map[i]] ? "ON" : "OFF" );
               }
            }
            else turbo_toggle_down[j][i] = 0;

         }
         else input_state |= input_state_cb(j, RETRO_DEVICE_JOYPAD, 0, map[i]) ? (1 << i) : 0;
      }

      // Input data must be little endian.
      input_buf[j][0] = (input_state >> 0) & 0xff;
      input_buf[j][1] = (input_state >> 8) & 0xff;
   }
}

static uint64_t video_frames, audio_frames;

void update_geometry(unsigned width, unsigned height)
{
   struct retro_system_av_info system_av_info;
   system_av_info.geometry.base_width = width;
   system_av_info.geometry.base_height = height;
   system_av_info.geometry.max_width = MEDNAFEN_CORE_GEOMETRY_MAX_W;
   system_av_info.geometry.max_height = MEDNAFEN_CORE_GEOMETRY_MAX_H;
   system_av_info.geometry.aspect_ratio = MEDNAFEN_CORE_GEOMETRY_ASPECT_RATIO;
   environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &system_av_info);
}



void retro_run(void)
{
   MDFNGI *curgame = (MDFNGI*)game;

   input_poll_cb();

   update_input();

   static int16_t sound_buf[0x10000];
   static int32_t rects[FB_HEIGHT];
   static unsigned width, height;
   bool resolution_changed = false;
   rects[0] = ~0;

   EmulateSpecStruct spec = {0};
   spec.surface = surf;
   spec.SoundRate = 44100;
   spec.SoundBuf = sound_buf;
   spec.LineWidths = rects;
   spec.SoundBufMaxSize = sizeof(sound_buf) / 2;
   spec.SoundVolume = 1.0;
   spec.soundmultiplier = 1.0;
   spec.SoundBufSize = 0;
   spec.VideoFormatChanged = false;
   spec.SoundFormatChanged = false;

   if (spec.SoundRate != last_sound_rate)
   {
      spec.SoundFormatChanged = true;
      last_sound_rate = spec.SoundRate;
   }

   Emulate(&spec);

   int16 *const SoundBuf = spec.SoundBuf + spec.SoundBufSizeALMS * curgame->soundchan;
   int32 SoundBufSize = spec.SoundBufSize - spec.SoundBufSizeALMS;
   const int32 SoundBufMaxSize = spec.SoundBufMaxSize - spec.SoundBufSizeALMS;

   spec.SoundBufSize = spec.SoundBufSizeALMS + SoundBufSize;

   if (width  != spec.DisplayRect.w || height != spec.DisplayRect.h)
      resolution_changed = true;

   width  = spec.DisplayRect.w;
   height = spec.DisplayRect.h;
   video_cb(surf->pixels + surf->pitch * spec.DisplayRect.y, width, height, FB_WIDTH * 2);

   audio_batch_cb(spec.SoundBuf, spec.SoundBufSize);

   bool updated = false;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
   {
      check_variables();
      if(PCE_IsCD){
            psg->SetVolume(0.678 * setting_pce_fast_cdpsgvolume / 100);
      }
      update_geometry(width, height);
   }
   
   if (resolution_changed)
      update_geometry(width, height);
   video_frames++;
   audio_frames += spec.SoundBufSize;
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = MEDNAFEN_CORE_NAME;
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
   info->library_version  = MEDNAFEN_CORE_VERSION GIT_VERSION;
   info->need_fullpath    = true;
   info->valid_extensions = MEDNAFEN_CORE_EXTENSIONS;
   info->block_extract    = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   memset(info, 0, sizeof(*info));
   info->timing.fps            = MEDNAFEN_CORE_TIMING_FPS;
   info->timing.sample_rate    = 44100;
   info->geometry.base_width   = MEDNAFEN_CORE_GEOMETRY_BASE_W;
   info->geometry.base_height  = MEDNAFEN_CORE_GEOMETRY_BASE_H;
   info->geometry.max_width    = MEDNAFEN_CORE_GEOMETRY_MAX_W;
   info->geometry.max_height   = MEDNAFEN_CORE_GEOMETRY_MAX_H;
   info->geometry.aspect_ratio = MEDNAFEN_CORE_GEOMETRY_ASPECT_RATIO;
}

void retro_deinit()
{
   if (surf)
      free(surf);
   surf = NULL;

   if (log_cb)
   {
      log_cb(RETRO_LOG_INFO, "[%s]: Samples / Frame: %.5f\n",
            MEDNAFEN_CORE_NAME, (double)audio_frames / video_frames);
      log_cb(RETRO_LOG_INFO, "[%s]: Estimated FPS: %.5f\n",
            MEDNAFEN_CORE_NAME, (double)video_frames * 44100 / audio_frames);
   }
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC; // FIXME: Regions for other cores.
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned in_port, unsigned device)
{
   switch(device)
   {
      case RETRO_DEVICE_JOYPAD:
         PCEINPUT_SetInput(in_port, "gamepad", &input_buf[in_port][0]);
         break;
      case RETRO_DEVICE_MOUSE:
         PCEINPUT_SetInput(in_port, "mouse", &input_buf[in_port][0]);
         break;
   }
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   static const struct retro_variable vars[] = {
      { "pce_fast_cdimagecache", "CD Image Cache (Restart); disabled|enabled" },
      { "pce_fast_cdbios", "CD Bios (Restart); System Card 3|Games Express|System Card 1|System Card 2" },
      { "pce_nospritelimit", "No Sprite Limit (Restart); disabled|enabled" },
      { "pce_ocmultiplier", "CPU Overclock Multiplier (Restart); 1|2|3|4|5|6|7|8|9|10|20|30|40|50" },
      { "pce_hoverscan", "Horizontal Overscan (352 Width Mode Only); 352|300|302|304|306|308|310|312|314|316|318|320|322|324|326|328|330|332|334|336|338|340|342|344|346|348|350" },
      { "pce_initial_scanline", "Initial scanline; 3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|0|1|2" },
      { "pce_last_scanline", "Last scanline; 242|208|209|210|211|212|213|214|215|216|217|218|219|220|221|222|223|224|225|226|227|228|229|230|231|232|233|234|235|236|237|238|239|240|241" },
      { "pce_cddavolume", "(CD) CDDA Volume %; 100|110|120|130|140|150|160|170|180|190|200|0|10|20|30|40|50|60|70|80|90" },
      { "pce_adpcmvolume", "(CD) ADPCM Volume %; 100|110|120|130|140|150|160|170|180|190|200|0|10|20|30|40|50|60|70|80|90" },
      { "pce_cdpsgvolume", "(CD) CD PSG Volume %; 100|110|120|130|140|150|160|170|180|190|200|0|10|20|30|40|50|60|70|80|90" },
      { "pce_cdspeed", "(CD) CD Speed; 1|2|4|8" },
      { "pce_Turbo_Delay", "Turbo Delay; Fast|Medium|Slow" },
      { "pce_p0_turbo_I_enable", "P1 Turbo I; disabled|enabled" },
      { "pce_p0_turbo_II_enable", "P1 Turbo II; disabled|enabled" },
      { "pce_p1_turbo_I_enable", "P2 Turbo I; disabled|enabled" },
      { "pce_p1_turbo_II_enable", "P2 Turbo II; disabled|enabled" },
      { "pce_p2_turbo_I_enable", "P3 Turbo I; disabled|enabled" },
      { "pce_p2_turbo_II_enable", "P3 Turbo II; disabled|enabled" },
      { "pce_p3_turbo_I_enable", "P4 Turbo I; disabled|enabled" },
      { "pce_p3_turbo_II_enable", "P4 Turbo II; disabled|enabled" },
      { "pce_p4_turbo_I_enable", "P5 Turbo I; disabled|enabled" },
      { "pce_p4_turbo_II_enable", "P5 Turbo II; disabled|enabled" },
      { NULL, NULL },
   };

   static const struct retro_controller_description pads[] = {
      { "PCE Joypad", RETRO_DEVICE_JOYPAD },
      { "Mouse", RETRO_DEVICE_MOUSE },
   };

   static const struct retro_controller_info ports[] = {
      { pads, 2 },
      { pads, 2 },
      { 0 },
   };

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
   environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

static size_t serialize_size;

size_t retro_serialize_size(void)
{
   StateMem st;
   memset(&st, 0, sizeof(st));

   if (!MDFNSS_SaveSM(&st, 0, 0, NULL, NULL, NULL))
      return 0;

   free(st.data);
   return serialize_size = st.len;
}

bool retro_serialize(void *data, size_t size)
{
   StateMem st;
   memset(&st, 0, sizeof(st));
   st.data     = (uint8_t*)data;
   st.malloced = size;

   return MDFNSS_SaveSM(&st, 0, 0, NULL, NULL, NULL);
}

bool retro_unserialize(const void *data, size_t size)
{
   StateMem st;
   memset(&st, 0, sizeof(st));
   st.data = (uint8_t*)data;
   st.len  = size;

   return MDFNSS_LoadSM(&st, 0, 0);
}

void *retro_get_memory_data(unsigned type)
{
   switch (type)
   {
      case RETRO_MEMORY_SAVE_RAM:
         if (IsPopulous)
            return (uint8_t*)(ROMSpace + 0x40 * 8192);
         return (uint8_t*)SaveRAM;
      case RETRO_MEMORY_SYSTEM_RAM:
         return BaseRAM;
      default:
         break;
   }

   return NULL;
}

size_t retro_get_memory_size(unsigned type)
{
   switch (type)
   {
      case RETRO_MEMORY_SAVE_RAM:
         if (IsPopulous)
            return 32768;
         return 2048;
      case RETRO_MEMORY_SYSTEM_RAM:
         return sizeof(BaseRAM) - 8192;
      default:
         break;
   }

   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned, bool, const char *)
{}

#ifdef _WIN32
static void sanitize_path(std::string &path)
{
   size_t size = path.size();
   for (size_t i = 0; i < size; i++)
      if (path[i] == '/')
         path[i] = '\\';
}
#endif

// Use a simpler approach to make sure that things go right for libretro.
std::string MDFN_MakeFName(MakeFName_Type type, int id1, const char *cd1)
{
   char slash;
#ifdef _WIN32
   slash = '\\';
#else
   slash = '/';
#endif
   std::string ret;
   switch (type)
   {
      case MDFNMKF_SAV:
         ret = retro_save_directory +slash + retro_base_name +
            std::string(".") + std::string(cd1);
         break;
      case MDFNMKF_FIRMWARE:
         ret = retro_base_directory + slash + std::string(cd1);
#ifdef _WIN32
         sanitize_path(ret); // Because Windows path handling is mongoloid.
#endif
         break;
      default:	  
         break;
   }

   if (log_cb)
      log_cb(RETRO_LOG_INFO, "MDFN_MakeFName: %s\n", ret.c_str());
   return ret;
}

void MDFND_Message(const char *str)
{
   if (log_cb)
      log_cb(RETRO_LOG_INFO, "%s\n", str);
}

void MDFND_PrintError(const char* err)
{
   if (log_cb)
      log_cb(RETRO_LOG_ERROR, "%s\n", err);
}

/* forward declarations */
extern void MDFND_DispMessage(unsigned char *str);

void MDFND_DispMessage(unsigned char *str)
{
   const char *strc = (const char*)str;
   struct retro_message msg =
   {
      strc,
      180
   };

   environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
}

void MDFN_DispMessage(const char *format, ...)
{
   struct retro_message msg;
   va_list ap;
   va_start(ap,format);
   char *str = new char[4096];
   const char *strc = NULL;

   vsnprintf(str, 4096, format,ap);
   va_end(ap);
   strc = str;

   msg.frames = 180;
   msg.msg = strc;

   environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
}

void MDFN_ResetMessages(void)
{
}
