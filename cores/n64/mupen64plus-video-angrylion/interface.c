#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <boolean.h>

#include "mupen64plus-next_common.h"
#include "m64p_plugin.h"

extern void DebugMessage(int level, const char *message, ...);

#include "Gfx #1.3.h"
#include "common.h"
#include "n64video.h"
#include "m64p_types.h"
#include "m64p_config.h"
#include "vdac.h"

#define DP_INTERRUPT    0x20

static bool angrylion_init = false;

int ProcessDListShown = 0;

extern GFX_INFO gfx_info;

extern uint32_t retro_screen_width;
extern uint32_t retro_screen_height;
extern uint32_t screen_pitch;

// Some macro trickery to avoid conflicting symbols and ease maintenance efforts
#define config _al_config
#define screen_width retro_screen_width
#define screen_height retro_screen_height

struct n64video_config config;

void plugin_init(void)
{
}

void plugin_sync_dp(void)
{
    *gfx_info.MI_INTR_REG |= DP_INTERRUPT;
    gfx_info.CheckInterrupts();
}

uint32_t** plugin_get_dp_registers(void)
{
    // HACK: this only works because the ordering of registers in GFX_INFO is
    // the same as in dp_register
    return (uint32_t**)&gfx_info.DPC_START_REG;
}

uint32_t** plugin_get_vi_registers(void)
{
    // HACK: this only works because the ordering of registers in GFX_INFO is
    // the same as in vi_register
    return (uint32_t**)&gfx_info.VI_STATUS_REG;
}

uint8_t* plugin_get_rdram(void)
{
    return gfx_info.RDRAM;
}

uint32_t plugin_get_rdram_size(void)
{
    return 0x800000;
}

uint8_t* plugin_get_dmem(void)
{
    return gfx_info.DMEM;
}

uint8_t* plugin_get_rom_header(void)
{
    return gfx_info.HEADER;
}

void plugin_close(void)
{
}

static char filter_char(char c)
{
    if (isalnum(c) || c == '_' || c == '-' || c == '.') {
        return c;
    } else {
        return ' ';
    }
}

uint32_t plugin_get_rom_name(char* name, uint32_t name_size)
{
    if (name_size < 21) {
        // buffer too small
        return 0;
    }

    uint8_t* rom_header = plugin_get_rom_header();
    if (!rom_header) {
        // not available
        return 0;
    }

    // copy game name from ROM header, which is encoded in Shift_JIS.
    // most games just use the ASCII subset, so filter out the rest.
    int i = 0;
    for (; i < 20; i++) {
        name[i] = filter_char(rom_header[(32 + i) ^ BYTE_ADDR_XOR]);
    }

    // make sure there's at least one whitespace that will terminate the string
    // below
    name[i] = ' ';

    // trim trailing whitespaces
    for (; i > 0; i--) {
        if (name[i] != ' ') {
            break;
        }
        name[i] = 0;
    }

    // game title is empty or invalid, use safe fallback using the four-character
    // game ID
    if (i == 0) {
        for (; i < 4; i++) {
            name[i] = filter_char(rom_header[(59 + i) ^ BYTE_ADDR_XOR]);
        }
        name[i] = 0;
    }

    return i;
}

void vdac_init(struct n64video_config* config) { }
void vdac_read(struct frame_buffer* fb, bool alpha) { }
void vdac_write(struct frame_buffer* fb)
{
   screen_width = fb->width;
   screen_height = fb->height;
   screen_pitch = fb->pitch * 4;
}

void vdac_sync(bool invalid) { 
   libretro_swap_buffer = !invalid;
}

void vdac_close(void) { }

void angrylion_set_vi(unsigned value)
{
 
   if(config.vi.mode != (enum vi_mode)value)
   {
      config.vi.mode = (enum vi_mode)value;
      if (angrylion_init)
      {
          n64video_close();
          n64video_init(&config);
      }
   }
}

void angrylion_set_threads(unsigned value)
{
  
    if(config.num_workers != value)
    {
     config.num_workers = value;
     if (angrylion_init)
     {
         n64video_close();
         n64video_init(&config);
     }
    }
    
}

void angrylion_set_overscan(unsigned value)
{
   if(config.vi.hide_overscan != (bool)value)
   {
      config.vi.hide_overscan = (bool)value;
      if (angrylion_init)
      {
         n64video_close();
         n64video_init(&config);
      }
   }
    
}

void angrylion_set_vi_dedither(unsigned value)
{
   if(config.vi.vi_dedither != (bool)value)
   {
      config.vi.vi_dedither = (bool)value;
      if (angrylion_init)
      {
         n64video_close();
         n64video_init(&config);
      }
   }
    
}

void angrylion_set_vi_blur(unsigned value)
{
   if(config.vi.vi_blur != (bool)value)
   {
      config.vi.vi_blur = (bool)value;
      if (angrylion_init)
      {
         n64video_close();
         n64video_init(&config);
      }
   }
    
}

void angrylion_set_synclevel(unsigned value)
{
   if(config.dp.compat != (enum dp_compat_profile)value)
   {
      config.dp.compat= (enum dp_compat_profile)value;
      if (angrylion_init)
      {
         n64video_close();
         n64video_init(&config);
      }
   }
}

unsigned angrylion_get_synclevel()
{
    return config.dp.compat;
}

unsigned angrylion_get_threads(void)
{
   return  config.num_workers;
}


unsigned angrylion_get_vi(void)
{
   return config.vi.mode;
}

void angrylion_set_filtering(unsigned filter_type)
{
   if (filter_type != 2)
      filter_type=1;
   else
      filter_type=0;

   if(config.vi.interp != (enum vi_interp)filter_type)
   {
      config.vi.interp = (enum vi_interp)filter_type;
      if (angrylion_init)
      {
         n64video_close();
         n64video_init(&config);
      }
   }
}

unsigned angrylion_get_filtering(void)
{
    return  (unsigned)config.vi.interp;
}

void angrylion_set_dithering(unsigned dither_type)
{
   config.dithering    = dither_type;
}

void angrylionChangeWindow (void) { }

void angrylionReadScreen2(void *dest, int *width, int *height, int front) { }
 
void angrylionDrawScreen (void) { }

void angrylionGetDllInfo(PLUGIN_INFO* PluginInfo)
{
    PluginInfo -> Version = 0x0103;
    PluginInfo -> Type  = 2;
    strcpy(
    PluginInfo -> Name, "angrylion's RDP"
    );
    PluginInfo -> NormalMemory = true;
    PluginInfo -> MemoryBswaped = true;
}

void angrylionSetRenderingCallback(void (*callback)(int)) { }

int angrylionInitiateGFX (GFX_INFO Gfx_Info)
{
   n64video_config_init(&config);
   return 1;
}
 
void angrylionMoveScreen (int xpos, int ypos) { }
 
void angrylionProcessDList(void)
{
   if (!ProcessDListShown)
      ProcessDListShown = 1;
}

void angrylionProcessRDPList(void)
{
  n64video_process_list();
}

void angrylionRomClosed (void)
{
  n64video_close();
}

int angrylionRomOpen(void)
{

   /* TODO/FIXME: For now just force it to 640x480.
    *
    * Later on we might want a low-res mode (320x240)
    * for better performance as well in case screen_width
    * is 320 and height is 240.
    */
   if (screen_width < 640)
      screen_width = 640;
   if (screen_width > 640)
      screen_width = 640;

   if (screen_height < 480)
      screen_height = 480;
   if (screen_height > 480)
      screen_height = 480;

   screen_pitch  = 640 << 2;

  config.gfx.rdram       = plugin_get_rdram();
  config.gfx.rdram_size  = plugin_get_rdram_size();

  config.gfx.dmem        = plugin_get_dmem();
  config.gfx.mi_intr_reg = (uint32_t*)gfx_info.MI_INTR_REG;
  config.gfx.mi_intr_cb  = gfx_info.CheckInterrupts;

  config.gfx.vi_reg      = plugin_get_vi_registers();
  config.gfx.dp_reg      = plugin_get_dp_registers();

   n64video_init(&config);
   angrylion_init        = true;
   return 1;
}

void angrylionUpdateScreen(void)
{
#ifdef HAVE_FRAMESKIP
    static int counter;
    if (counter++ < skip)
        return;
    counter = 0;
#endif
    n64video_update_screen();
    
}

void angrylionShowCFB (void)
{
   angrylionUpdateScreen();
}


void angrylionViStatusChanged (void) { }

void angrylionViWidthChanged (void) { }

void angrylionFBWrite(unsigned int addr, unsigned int size) { }

void angrylionFBRead(unsigned int addr) { }

void angrylionFBGetFrameBufferInfo(void *pinfo) { }

m64p_error angrylionPluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities)
{
   /* set version info */
   if (PluginType != NULL)
      *PluginType = M64PLUGIN_GFX;

   if (PluginVersion != NULL)
      *PluginVersion = 0x016304;

   if (APIVersion != NULL)
      *APIVersion = 0x020100;

   if (PluginNamePtr != NULL)
      *PluginNamePtr = "MAME/Angrylion/HatCat/ata4 video Plugin";

   if (Capabilities != NULL)
      *Capabilities = 0;

   return M64ERR_SUCCESS;
}

#define MSG_BUFFER_LEN 256
void msg_error(const char * err, ...)
{
   va_list ap;
   char buffer[2049];
   va_start(ap, err);
   vsnprintf(buffer, 2047, err, ap);
   buffer[2048] = '\0';
   va_end(ap);

   DebugMessage(M64MSG_ERROR, "%s", buffer);
}

void msg_warning(const char* err, ...)
{
   va_list ap;
   char buffer[2049];
   va_start(ap, err);
   vsnprintf(buffer, 2047, err, ap);
   buffer[2048] = '\0';
   va_end(ap);

   DebugMessage(M64MSG_WARNING, "%s", buffer);
}

void msg_debug(const char* err, ...)
{
   va_list ap;
   char buffer[2049];
   va_start(ap, err);
   vsnprintf(buffer, 2047, err, ap);
   buffer[2048] = '\0';
   va_end(ap);

   DebugMessage(M64MSG_INFO, "%s", buffer);
}
