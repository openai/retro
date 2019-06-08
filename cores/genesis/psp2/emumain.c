#include "emumain.h"

#include <psp2/types.h>
#include <psp2/kernel/threadmgr.h>
#include "time.h"
#include <psp2/rtc.h>

#define SCE_KERNEL_OK 0

#include "psplib/pl_rewind.h"
#include "psplib/pl_file.h"
#include "psplib/pl_snd.h"
#include "psplib/pl_perf.h"
#include "psplib/pl_util.h"
#include "psplib/pl_psp.h"
#include "psplib/video.h"
#include "psplib/ctrl.h"

#include "shared.h"
#include "sound.h"
#include "system.h"
#include "md_ntsc.h"
#include "sms_ntsc.h"

//#include <debugnet.h>

#define ip_server "192.168.1.130"
#define port_server 18194

PspImage *Screen;
pl_rewind Rewinder;

static pl_perf_counter FpsCounter;
static int ClearScreen;
static int ScreenX, ScreenY, ScreenW, ScreenH;
static int TicksPerUpdate;
static u32 TicksPerSecond;
static u64 LastTick;
static u64 CurrentTick;
static int Frame;

static int Rewinding;

extern pl_file_path CurrentGame;
extern EmulatorOptions Options;
extern const u64 ButtonMask[];
extern const int ButtonMapId[];
extern struct ButtonConfig ActiveConfig;
extern char *ScreenshotPath;

static short soundbuffer[SOUND_SAMPLES*2*10];
static int soundPosWrite = 0;
static int soundPosRead = 0;
static SceUID console_mtx;



static inline void RenderVideo();
static void AudioCallback(pl_snd_sample* buf,
                          unsigned int samples,
                          void *userdata);
void MixerCallback(int16 **stream, int16 **output, int length);
md_ntsc_t *md_ntsc;
sms_ntsc_t *sms_ntsc;

int bEmulate;

void osd_input_update()
{

    /* Reset input */
    input.pad[0]    = 0;
    input.analog[0][0] = 0x7F;

    static SceCtrlData pad;
    static int autofire_status = 0;

    /* Check the input */
    if (pspCtrlPollControls(&pad))
    {
      if (--autofire_status < 0)
        autofire_status = Options.AutoFire;

      /* Parse input */
      int i, on, code;
      for (i = 0; ButtonMapId[i] >= 0; i++)
      {
        code = ActiveConfig.ButtonMap[ButtonMapId[i]];
        on = (pad.buttons & ButtonMask[i]) == ButtonMask[i];

        /* Check to see if a button set is pressed. If so, unset it, so it */
        /* doesn't trigger any other combination presses. */
        if (on) pad.buttons &= ~ButtonMask[i];

          if (code & AFI)
          {
            if (on && (autofire_status == 0))
              input.pad[0] |= CODE_MASK(code);
            continue;
          }
          else if (code & JOY)
          {
            if (on) {
              input.pad[0] |= CODE_MASK(code);
            }
            continue;
          }
          else if (code & SYS)
          {
            if (on)
            {
              if (CODE_MASK(code) == (INPUT_START))
                input.system[0] |= INPUT_START;
            }
            continue;
          }


        if (code & SPC)
        {
          switch (CODE_MASK(code))
          {
          case SPC_MENU:
            if (on) bEmulate=0;
            break;
          case SPC_REWIND:
            Rewinding = on;
            break;
          }
        }
      }
    }

    return;
  }

void InitEmulator()
{

	//debugNetInit(ip_server,port_server,DEBUG);

  set_config_defaults();

  ClearScreen = 0;

  /* Initialize screen buffer */
  Screen = pspImageCreateVram(720, 576, PSP_IMAGE_16BPP);

  // pspImageClear(Screen, 0x8000);

  console_mtx = sceKernelCreateSema("sound_sema", 0, 0, 1, 0);
	/*if (console_mtx > 0) {
		debugNetPrintf(DEBUG,"Sound Mutex UID: 0x%08X\n", console_mtx);
	}*/

  /* Set up bitmap structure */
  memset(&bitmap, 0, sizeof(t_bitmap));
  bitmap.width  = Screen->Width;
  bitmap.height = Screen->Height;
  bitmap.pitch  = (bitmap.width * 2);
  bitmap.data   = (unsigned char *)Screen->Pixels;
  bitmap.viewport.changed = 3;

  pl_snd_set_callback(0, AudioCallback, NULL);
}

void RunEmulator()
{
  float ratio;
  //debugNetPrintf(DEBUG,"RunEmulator\n");

  pspImageClear(Screen, 0);
  //debugNetPrintf(DEBUG,"ImageClear\n");


  if(bitmap.viewport.changed & 1)
  {
    bitmap.viewport.changed &= ~1;

    /* source bitmap */
    Screen->Viewport.Width = bitmap.viewport.w+2*bitmap.viewport.x;
    Screen->Viewport.Height = bitmap.viewport.h+2*bitmap.viewport.y;

  }

  /* Recompute screen size/position */
  switch (Options.DisplayMode)
  {
  default:
  case DISPLAY_MODE_UNSCALED:
    ScreenW = Screen->Viewport.Width;
    ScreenH = Screen->Viewport.Height;
    break;
  case DISPLAY_MODE_FIT_HEIGHT:
    ratio = (float)SCR_HEIGHT / (float)Screen->Viewport.Height;
    ScreenW = (float)bitmap.viewport.w * ratio - 2;
    ScreenH = SCR_HEIGHT;
    break;
  case DISPLAY_MODE_FILL_SCREEN:
    ScreenW = SCR_WIDTH;
    ScreenH = SCR_HEIGHT;
    break;
  case DISPLAY_MODE_2X:
      ScreenW = Screen->Viewport.Width*2;
      ScreenH = Screen->Viewport.Height*2;
      break;
  case DISPLAY_MODE_3X:
      ScreenW = Screen->Viewport.Width*3;
      ScreenH = Screen->Viewport.Height*3;
      break;
  }
  //debugNetPrintf(DEBUG,"screensize %d %d\n" ,Screen->Viewport.Width ,Screen->Viewport.Height);

  ScreenX = (SCR_WIDTH / 2) - (ScreenW / 2);
  ScreenY = (SCR_HEIGHT / 2) - (ScreenH / 2);

  /* Init performance counter */
  pl_perf_init_counter(&FpsCounter);

  /* Recompute update frequency */
  TicksPerSecond = sceRtcGetTickResolution();
  if (Options.UpdateFreq)
  {
    TicksPerUpdate = TicksPerSecond
      / (Options.UpdateFreq / (Options.Frameskip + 1));
    sceRtcGetCurrentTick(&LastTick);
  }
  Frame = 0;
  ClearScreen = 2;
  Rewinding = 0;

//pl_rewind_realloc(&Rewinder);

  int frames_until_save = 0;

  /* Resume sound */
  pl_snd_resume(0);

  /* Wait for V. refresh */
  pspVideoWaitVSync();

  bEmulate = 1;
  /* Main emulation loop */
  while (!ExitPSP&&bEmulate)
  {
    /* Rewind/save state */
    /*if (!Rewinding)
    {
      if (--frames_until_save <= 0)
      {
        frames_until_save = Options.RewindSaveRate;
        pl_rewind_save(&Rewinder);
      }
    }
    else
    {
      frames_until_save = Options.RewindSaveRate;
      if (!pl_rewind_restore(&Rewinder))
        continue;
    }*/

    /* Run the system emulation for a frame */
    if (++Frame <= Options.Frameskip)
    {
      /* Skip frame */
      if (system_hw == SYSTEM_MCD)
      {
        system_frame_scd(1);
      }
      else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
      {
        system_frame_gen(1);
      }
      else
      {
        system_frame_sms(1);
      }
    }
    else
    {
      if (system_hw == SYSTEM_MCD)
      {
        system_frame_scd(0);
      }
      else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
      {
        system_frame_gen(0);
      }
      else
      {
        system_frame_sms(0);
      }

      Frame = 0;
      /* Display */
      if(bitmap.viewport.changed & 1)
      {
        bitmap.viewport.changed &= ~1;

        /* source bitmap */
        Screen->Viewport.Width = bitmap.viewport.w+2*bitmap.viewport.x;
        Screen->Viewport.Height = bitmap.viewport.h+2*bitmap.viewport.y;

        /* Recompute screen size/position */
        switch (Options.DisplayMode)
        {
        default:
        case DISPLAY_MODE_UNSCALED:
          ScreenW = Screen->Viewport.Width;
          ScreenH = Screen->Viewport.Height;
          break;
        case DISPLAY_MODE_FIT_HEIGHT:
          ratio = (float)SCR_HEIGHT / (float)Screen->Viewport.Height;
          ScreenW = (float)bitmap.viewport.w * ratio - 2;
          ScreenH = SCR_HEIGHT;
          break;
        case DISPLAY_MODE_FILL_SCREEN:
          ScreenW = SCR_WIDTH;
          ScreenH = SCR_HEIGHT;
          break;
        case DISPLAY_MODE_2X:
            ScreenW = Screen->Viewport.Width*2;
            ScreenH = Screen->Viewport.Height*2;
            break;
        case DISPLAY_MODE_3X:
            ScreenW = Screen->Viewport.Width*3;
            ScreenH = Screen->Viewport.Height*3;
            break;
        }

        ScreenX = (SCR_WIDTH / 2) - (ScreenW / 2);
        ScreenY = (SCR_HEIGHT / 2) - (ScreenH / 2);

      }
      //debugNetPrintf(DEBUG,"main %d %d \n",soundPosRead,soundPosWrite);
      int size = audio_update(&soundbuffer[soundPosRead])*2;
      //debugNetPrintf(DEBUG,"filling %d \n",size);

      soundPosRead +=size;
      if(soundPosRead+size>=(SOUND_SAMPLES*2*10)){
          soundPosRead = 0;
      }
      if((soundPosRead-soundPosWrite)>=(SOUND_SAMPLES*2)||(soundPosRead-soundPosWrite)<0){
        sceKernelSignalSema(console_mtx, 1); //lock
      }
      RenderVideo();

    }
  }

  /* Stop sound */
  pl_snd_pause(0);
}

void TrashEmulator()
{
  pl_rewind_destroy(&Rewinder);

  /* Trash screen */
  if (Screen) pspImageDestroy(Screen);

  if (CurrentGame[0] != '\0')
  {
    /* Release emulation resources */
    audio_shutdown();
    error_shutdown();
  }
  //debugNetFinish();

}



void RenderVideo()
{
  /* Update the display */
  pspVideoBegin();

  /* Clear the buffer first, if necessary */
  if (ClearScreen >= 0)
  {
    ClearScreen--;
    pspVideoClearScreen();
  }

  pspVideoPutImage(Screen, ScreenX, ScreenY, ScreenW, ScreenH);

  /* Show FPS counter */
  if (Options.ShowFps)
  {
    static char fps_display[32];
    sprintf(fps_display, " %3.02f", pl_perf_update_counter(&FpsCounter));

    int width = pspFontGetTextWidth(&PspStockFont, fps_display);
    int height = pspFontGetLineHeight(&PspStockFont);

    pspVideoFillRect(SCR_WIDTH - width, 0, SCR_WIDTH, height, PSP_COLOR_BLACK);
    pspVideoPrint(&PspStockFont, SCR_WIDTH - width, 0, fps_display, PSP_COLOR_WHITE);
  }

  pspVideoEnd();

  /* Wait if needed */
  if (Options.UpdateFreq)
  {
    do { sceRtcGetCurrentTick(&CurrentTick); }
    while (CurrentTick - LastTick < TicksPerUpdate);
    LastTick = CurrentTick;
  }

  /* Wait for VSync signal */
  if (Options.VSync)
    pspVideoWaitVSync();

  /* Swap buffers */
  pspVideoSwapBuffers();
}

static void AudioCallback(pl_snd_sample *buf,
                          unsigned int samples,
                          void *userdata)
{
  int i;
  //debugNetPrintf(DEBUG,"wait %d %d \n",totalSamples,samples);
  if (!Rewinding)
  {
      short* ptr_s = (short*)buf;
      //debugNetPrintf(DEBUG,"wait %d %d \n",soundPosRead,soundPosWrite);
      if((soundPosRead-soundPosWrite)<SOUND_SAMPLES*2){
        sceKernelWaitSema(console_mtx, 1, 0); //lock
        //debugNetPrintf(DEBUG,"start %d %d \n",soundPosRead,soundPosWrite);
      }
      memcpy(ptr_s,&soundbuffer[soundPosWrite],sizeof(short)*samples*2);
      soundPosWrite +=samples*2;
      if(soundPosWrite+(samples*2)>=(SOUND_SAMPLES*2*10)){
          soundPosWrite = 0;
      }
  }
  else /* Render silence */
    for (i = 0; i < samples; i++)
      buf[i].stereo.l = buf[i].stereo.r = 0;
}
