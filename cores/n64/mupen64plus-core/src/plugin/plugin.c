/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - plugin.c                                                *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2002 Hacktarux                                          *
 *   Copyright (C) 2009 Richard Goedeken                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "api/callbacks.h"
#include "api/m64p_common.h"
#include "api/m64p_plugin.h"
#include "api/m64p_types.h"
#include "device/memory/memory.h"
#include "device/rcp/ai/ai_controller.h"
#include "device/rcp/mi/mi_controller.h"
#include "device/rcp/rdp/rdp_core.h"
#include "device/rcp/rsp/rsp_core.h"
#include "device/rcp/vi/vi_controller.h"
#include "dummy_audio.h"
#include "dummy_input.h"
#include "main/main.h"
#include "main/rom.h"
#include "main/version.h"
#include "osal/dynamiclib.h"
#include "plugin.h"
#ifdef __LIBRETRO__
#include "mupen64plus-next_common.h"
#endif

#include <stdio.h>

CONTROL Controls[4];
void ResizeVideoOutput(int width, int height){

}
/* local data structures and functions */
#define DEFINE_GFX(X) \
    EXPORT m64p_error CALL X##PluginGetVersion(m64p_plugin_type *, int *, int *, const char **, int *); \
    EXPORT void CALL X##ChangeWindow(void); \
    EXPORT int  CALL X##InitiateGFX(GFX_INFO Gfx_Info); \
    EXPORT void CALL X##MoveScreen(int x, int y); \
    EXPORT void CALL X##ProcessDList(void); \
    EXPORT void CALL X##ProcessRDPList(void); \
    EXPORT void CALL X##RomClosed(void); \
    EXPORT int  CALL X##RomOpen(void); \
    EXPORT void CALL X##ShowCFB(void); \
    EXPORT void CALL X##UpdateScreen(void); \
    EXPORT void CALL X##ViStatusChanged(void); \
    EXPORT void CALL X##ViWidthChanged(void); \
    EXPORT void CALL X##ReadScreen2(void *dest, int *width, int *height, int front); \
    EXPORT void CALL X##SetRenderingCallback(void (*callback)(int)); \
    EXPORT void CALL X##ResizeVideoOutput(int width, int height); \
    EXPORT void CALL X##FBRead(unsigned int addr); \
    EXPORT void CALL X##FBWrite(unsigned int addr, unsigned int size); \
    EXPORT void CALL X##FBGetFrameBufferInfo(void *p); \
    \
    gfx_plugin_functions gfx_##X = { \
        X##PluginGetVersion, \
        X##ChangeWindow, \
        X##InitiateGFX, \
        X##MoveScreen, \
        X##ProcessDList, \
        X##ProcessRDPList, \
        X##RomClosed, \
        X##RomOpen, \
        X##ShowCFB, \
        X##UpdateScreen, \
        X##ViStatusChanged, \
        X##ViWidthChanged, \
        X##ReadScreen2, \
        X##SetRenderingCallback, \
        ResizeVideoOutput, \
        X##FBRead, \
        X##FBWrite, \
        X##FBGetFrameBufferInfo \
    }

DEFINE_GFX(gln64);
#if defined(HAVE_THR_AL)
DEFINE_GFX(angrylion);
#endif

gfx_plugin_functions gfx;
GFX_INFO gfx_info;
audio_plugin_functions audio;
input_plugin_functions input;
rsp_plugin_functions rsp;
RSP_INFO rsp_info;

const audio_plugin_functions dummy_audio = {
    dummyaudio_PluginGetVersion,
    dummyaudio_AiDacrateChanged,
    dummyaudio_AiLenChanged,
    dummyaudio_InitiateAudio,
    dummyaudio_ProcessAList,
    dummyaudio_RomClosed,
    dummyaudio_RomOpen,
    dummyaudio_SetSpeedFactor,
    dummyaudio_VolumeUp,
    dummyaudio_VolumeDown,
    dummyaudio_VolumeGetLevel,
    dummyaudio_VolumeSetLevel,
    dummyaudio_VolumeMute,
    dummyaudio_VolumeGetString
};


extern m64p_error inputPluginGetVersion(m64p_plugin_type *PluginType, int *PluginVersion,
                                              int *APIVersion, const char **PluginNamePtr, int *Capabilities);
extern void inputInitiateControllers (CONTROL_INFO ControlInfo);
extern void inputGetKeys_default(int Control, BUTTONS * Keys );
extern void inputControllerCommand(int Control, unsigned char *Command);
extern void inputInitiateControllers(CONTROL_INFO ControlInfo);
extern void inputReadController(int Control, unsigned char *Command);
extern int  inputRomOpen(void);
extern void inputRomClosed(void);


const input_plugin_functions dummy_input = {
    inputPluginGetVersion,
    inputControllerCommand,
    inputGetKeys_default,
    inputInitiateControllers,
    inputReadController,
    inputRomClosed,
    inputRomOpen,
    dummyinput_SDL_KeyDown,
    dummyinput_SDL_KeyUp,
    dummyinput_RenderCallback
};

static AUDIO_INFO audio_info;
static CONTROL_INFO control_info;
static int l_RspAttached = 0;
static int l_InputAttached = 0;
static int l_AudioAttached = 0;
static int l_GfxAttached = 0;

static unsigned int dummy;

/* local functions */
static void EmptyFunc(void)
{
}
/* RSP */
#define DEFINE_RSP(X) \
    EXPORT m64p_error CALL X##PluginGetVersion(m64p_plugin_type *, int *, int *, const char **, int *); \
    EXPORT unsigned int CALL X##DoRspCycles(unsigned int Cycles); \
    EXPORT void CALL X##InitiateRSP(RSP_INFO Rsp_Info, unsigned int *CycleCount); \
    EXPORT void CALL X##RomClosed(void); \
    \
    const rsp_plugin_functions rsp_##X = { \
        X##PluginGetVersion, \
        X##DoRspCycles, \
        X##InitiateRSP, \
        X##RomClosed \
    }

// Define RSP Interfaces
DEFINE_RSP(hle);

#ifdef HAVE_PARALLEL_RSP
DEFINE_RSP(parallelRSP);
#endif // HAVE_PARALLEL_RSP

#if HAVE_LLE
DEFINE_RSP(cxd4);
#endif // HAVE_LLE

static void                     (*l_mainRenderCallback)(int) = NULL;
static ptr_SetRenderingCallback   l_old1SetRenderingCallback = NULL;

static void backcompat_videoRenderCallback(int unused)  // this function will be called by the video plugin as the render callback
{
    if (l_mainRenderCallback != NULL)
        l_mainRenderCallback(1);  // assume screen is always freshly redrawn (otherwise screenshots won't work w/ OSD enabled)
}

static void backcompat_setRenderCallbackIntercept(void (*callback)(int))
{
    l_mainRenderCallback = callback;
}

m64p_error plugin_start_gfx(void)
{
    printf("plugin_start_gfx\n");

    uint8_t media = *((uint8_t*)mem_base_u32(g_mem_base, MM_CART_ROM) + (0x3b ^ S8));

    /* Here we feed 64DD IPL ROM header to GFX plugin if 64DD is present.
     * We use g_media_loader.get_dd_rom to detect 64DD presence
     * instead of g_dev because the latter is not yet initialized at plugin_start time */
    /* XXX: Not sure it is the best way to convey which game is being played to the GFX plugin
     * as 64DD IPL is the same for all 64DD games... */
    char* dd_ipl_rom_filename = (g_media_loader.get_dd_rom == NULL)
        ? NULL
        : g_media_loader.get_dd_rom(g_media_loader.cb_data);

    uint32_t rom_base = (dd_ipl_rom_filename != NULL && strlen(dd_ipl_rom_filename) != 0 && media != 'C')
        ? MM_DD_ROM
        : MM_CART_ROM;

    free(dd_ipl_rom_filename);

    /* fill in the GFX_INFO data structure */
    gfx_info.HEADER = (unsigned char *)mem_base_u32(g_mem_base, rom_base);
    gfx_info.RDRAM = (unsigned char *)mem_base_u32(g_mem_base, MM_RDRAM_DRAM);
    gfx_info.DMEM = (unsigned char *)mem_base_u32(g_mem_base, MM_RSP_MEM);
    gfx_info.IMEM = (unsigned char *)mem_base_u32(g_mem_base, MM_RSP_MEM + 0x1000);
    gfx_info.MI_INTR_REG = &(g_dev.mi.regs[MI_INTR_REG]);
    gfx_info.DPC_START_REG = &(g_dev.dp.dpc_regs[DPC_START_REG]);
    gfx_info.DPC_END_REG = &(g_dev.dp.dpc_regs[DPC_END_REG]);
    gfx_info.DPC_CURRENT_REG = &(g_dev.dp.dpc_regs[DPC_CURRENT_REG]);
    gfx_info.DPC_STATUS_REG = &(g_dev.dp.dpc_regs[DPC_STATUS_REG]);
    gfx_info.DPC_CLOCK_REG = &(g_dev.dp.dpc_regs[DPC_CLOCK_REG]);
    gfx_info.DPC_BUFBUSY_REG = &(g_dev.dp.dpc_regs[DPC_BUFBUSY_REG]);
    gfx_info.DPC_PIPEBUSY_REG = &(g_dev.dp.dpc_regs[DPC_PIPEBUSY_REG]);
    gfx_info.DPC_TMEM_REG = &(g_dev.dp.dpc_regs[DPC_TMEM_REG]);
    gfx_info.VI_STATUS_REG = &(g_dev.vi.regs[VI_STATUS_REG]);
    gfx_info.VI_ORIGIN_REG = &(g_dev.vi.regs[VI_ORIGIN_REG]);
    gfx_info.VI_WIDTH_REG = &(g_dev.vi.regs[VI_WIDTH_REG]);
    gfx_info.VI_INTR_REG = &(g_dev.vi.regs[VI_V_INTR_REG]);
    gfx_info.VI_V_CURRENT_LINE_REG = &(g_dev.vi.regs[VI_CURRENT_REG]);
    gfx_info.VI_TIMING_REG = &(g_dev.vi.regs[VI_BURST_REG]);
    gfx_info.VI_V_SYNC_REG = &(g_dev.vi.regs[VI_V_SYNC_REG]);
    gfx_info.VI_H_SYNC_REG = &(g_dev.vi.regs[VI_H_SYNC_REG]);
    gfx_info.VI_LEAP_REG = &(g_dev.vi.regs[VI_LEAP_REG]);
    gfx_info.VI_H_START_REG = &(g_dev.vi.regs[VI_H_START_REG]);
    gfx_info.VI_V_START_REG = &(g_dev.vi.regs[VI_V_START_REG]);
    gfx_info.VI_V_BURST_REG = &(g_dev.vi.regs[VI_V_BURST_REG]);
    gfx_info.VI_X_SCALE_REG = &(g_dev.vi.regs[VI_X_SCALE_REG]);
    gfx_info.VI_Y_SCALE_REG = &(g_dev.vi.regs[VI_Y_SCALE_REG]);
    gfx_info.CheckInterrupts = EmptyFunc;
    
    gfx_info.version = 2; //Version 2 added SP_STATUS_REG and RDRAM_SIZE
    gfx_info.SP_STATUS_REG = &g_dev.sp.regs[SP_STATUS_REG];
    gfx_info.RDRAM_SIZE = (unsigned int*) &g_dev.rdram.dram_size;

    /* call the audio plugin */
    if (!gfx.initiateGFX(gfx_info))
        return M64ERR_PLUGIN_FAIL;

    return M64ERR_SUCCESS;
}

static void plugin_disconnect_audio(void)
{
    audio = dummy_audio;
    l_AudioAttached = 0;
}

static m64p_error plugin_start_audio(void)
{
    /* fill in the AUDIO_INFO data structure */
    audio_info.RDRAM = (unsigned char *)mem_base_u32(g_mem_base, MM_RDRAM_DRAM);
    audio_info.DMEM = (unsigned char *)mem_base_u32(g_mem_base, MM_RSP_MEM);
    audio_info.IMEM = (unsigned char *)mem_base_u32(g_mem_base, MM_RSP_MEM + 0x1000);
    audio_info.MI_INTR_REG = &(g_dev.mi.regs[MI_INTR_REG]);
    audio_info.AI_DRAM_ADDR_REG = &(g_dev.ai.regs[AI_DRAM_ADDR_REG]);
    audio_info.AI_LEN_REG = &(g_dev.ai.regs[AI_LEN_REG]);
    audio_info.AI_CONTROL_REG = &(g_dev.ai.regs[AI_CONTROL_REG]);
    audio_info.AI_STATUS_REG = &dummy;
    audio_info.AI_DACRATE_REG = &(g_dev.ai.regs[AI_DACRATE_REG]);
    audio_info.AI_BITRATE_REG = &(g_dev.ai.regs[AI_BITRATE_REG]);
    audio_info.CheckInterrupts = EmptyFunc;

    /* call the audio plugin */
    if (!audio.initiateAudio(audio_info))
        return M64ERR_PLUGIN_FAIL;

    return M64ERR_SUCCESS;
}

static void plugin_disconnect_input(void)
{
    input = dummy_input;
    l_InputAttached = 0;
}

static m64p_error plugin_start_input(void)
{
    int i;

    /* fill in the CONTROL_INFO data structure */
    control_info.Controls = Controls;
    for (i=0; i<4; i++)
      {
         Controls[i].Present = 0;
         Controls[i].RawData = 0;
         Controls[i].Plugin = PLUGIN_NONE;
      }

    /* call the input plugin */
    input.initiateControllers(control_info);

    return M64ERR_SUCCESS;
}

static m64p_error plugin_start_rsp(void)
{
    /* fill in the RSP_INFO data structure */
    rsp_info.RDRAM = (unsigned char *)mem_base_u32(g_mem_base, MM_RDRAM_DRAM);
    rsp_info.DMEM = (unsigned char *)mem_base_u32(g_mem_base, MM_RSP_MEM);
    rsp_info.IMEM = (unsigned char *)mem_base_u32(g_mem_base, MM_RSP_MEM + 0x1000);
    rsp_info.MI_INTR_REG = &g_dev.mi.regs[MI_INTR_REG];
    rsp_info.SP_MEM_ADDR_REG = &g_dev.sp.regs[SP_MEM_ADDR_REG];
    rsp_info.SP_DRAM_ADDR_REG = &g_dev.sp.regs[SP_DRAM_ADDR_REG];
    rsp_info.SP_RD_LEN_REG = &g_dev.sp.regs[SP_RD_LEN_REG];
    rsp_info.SP_WR_LEN_REG = &g_dev.sp.regs[SP_WR_LEN_REG];
    rsp_info.SP_STATUS_REG = &g_dev.sp.regs[SP_STATUS_REG];
    rsp_info.SP_DMA_FULL_REG = &g_dev.sp.regs[SP_DMA_FULL_REG];
    rsp_info.SP_DMA_BUSY_REG = &g_dev.sp.regs[SP_DMA_BUSY_REG];
    rsp_info.SP_PC_REG = &g_dev.sp.regs2[SP_PC_REG];
    rsp_info.SP_SEMAPHORE_REG = &g_dev.sp.regs[SP_SEMAPHORE_REG];
    rsp_info.DPC_START_REG = &g_dev.dp.dpc_regs[DPC_START_REG];
    rsp_info.DPC_END_REG = &g_dev.dp.dpc_regs[DPC_END_REG];
    rsp_info.DPC_CURRENT_REG = &g_dev.dp.dpc_regs[DPC_CURRENT_REG];
    rsp_info.DPC_STATUS_REG = &g_dev.dp.dpc_regs[DPC_STATUS_REG];
    rsp_info.DPC_CLOCK_REG = &g_dev.dp.dpc_regs[DPC_CLOCK_REG];
    rsp_info.DPC_BUFBUSY_REG = &g_dev.dp.dpc_regs[DPC_BUFBUSY_REG];
    rsp_info.DPC_PIPEBUSY_REG = &g_dev.dp.dpc_regs[DPC_PIPEBUSY_REG];
    rsp_info.DPC_TMEM_REG = &g_dev.dp.dpc_regs[DPC_TMEM_REG];
    rsp_info.CheckInterrupts = EmptyFunc;
    rsp_info.ProcessDlistList = gfx.processDList;
    rsp_info.ProcessAlistList = audio.processAList;
    rsp_info.ProcessRdpList = gfx.processRDPList;
    rsp_info.ShowCFB = gfx.showCFB;

    /* call the RSP plugin  */
    rsp.initiateRSP(rsp_info, NULL);

    return M64ERR_SUCCESS;
}

m64p_error plugin_start(m64p_plugin_type type)
{
    switch(type)
    {
        case M64PLUGIN_RSP:
            return plugin_start_rsp();
        case M64PLUGIN_GFX:
            return plugin_start_gfx();
        case M64PLUGIN_AUDIO:
            return plugin_start_audio();
        case M64PLUGIN_INPUT:
            return plugin_start_input();
        default:
            return M64ERR_INPUT_INVALID;
    }

    return M64ERR_INTERNAL;
}

m64p_error plugin_check(void)
{
    if (!l_GfxAttached)
        DebugMessage(M64MSG_WARNING, "No video plugin attached.  There will be no video output.");
    if (!l_RspAttached)
        DebugMessage(M64MSG_WARNING, "No RSP plugin attached.  The video output will be corrupted.");
    if (!l_AudioAttached)
        DebugMessage(M64MSG_WARNING, "No audio plugin attached.  There will be no sound output.");
    if (!l_InputAttached)
        DebugMessage(M64MSG_WARNING, "No input plugin attached.  You won't be able to control the game.");

    return M64ERR_SUCCESS;
}

#ifdef __LIBRETRO__
enum rdp_plugin_type current_rdp_type = RDP_PLUGIN_NONE;
enum rsp_plugin_type current_rsp_type = RSP_PLUGIN_NONE;

void plugin_connect_rdp_api(enum rdp_plugin_type type)
{
   switch (type)
   {
      case RDP_PLUGIN_GLIDEN64:
      case RDP_PLUGIN_ANGRYLION:
         current_rdp_type = type;
         break;
      case RSP_PLUGIN_NONE:
      default:
         break;
   }
}

void plugin_connect_rsp_api(enum rsp_plugin_type type)
{
   switch (type)
   {
      case RSP_PLUGIN_HLE:
      case RSP_PLUGIN_CXD4:
      case RSP_PLUGIN_PARALLEL:
         current_rsp_type = type;
         break;
      case RSP_PLUGIN_NONE:
      default:
         break;
   }
}

/* global functions */
void plugin_connect_all()
{
    switch (current_rdp_type)
    {
       case RDP_PLUGIN_ANGRYLION:
#ifdef HAVE_THR_AL
          gfx = gfx_angrylion;
#endif
          break;
       case RDP_PLUGIN_GLIDEN64:
          gfx = gfx_gln64;
          break;
      case RDP_PLUGIN_NONE:
      default:
         break;
    }

    l_GfxAttached = 1;
    plugin_start_gfx();

    switch (current_rsp_type)
    {
      case RSP_PLUGIN_HLE:
         rsp = rsp_hle;
         break;
      case RSP_PLUGIN_CXD4:
#ifdef HAVE_LLE
         rsp = rsp_cxd4;
#endif // HAVE_LLE
         break;
      case RSP_PLUGIN_PARALLEL:
#ifdef HAVE_PARALLEL_RSP
         rsp = rsp_parallelRSP;
#endif // HAVE_PARALLEL_RSP
         break;
      case RSP_PLUGIN_NONE:
      default:
         break;
    }

    l_RspAttached = 1;
    plugin_start_rsp();

    audio = dummy_audio;
    l_AudioAttached = 1;
    //plugin_start_audio();
    
    input = dummy_input;
    l_InputAttached = 1;
    plugin_start_input();
}
#else
/* global functions */
void plugin_connect_all()
{
    gfx = gfx_gln64;
    l_GfxAttached = 1;
    plugin_start_gfx();

    rsp = rsp_hle;
    l_RspAttached = 1;
    plugin_start_rsp();

    audio = dummy_audio;
    l_AudioAttached = 1;
    //plugin_start_audio();
    
    input = dummy_input;
    l_InputAttached = 1;
    plugin_start_input();
}
#endif

