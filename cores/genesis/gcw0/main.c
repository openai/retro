#ifdef __WIN32__
#include <windows.h>
#else
#define MessageBox(owner, text, caption, type) printf("%s: %s\n", caption, text)
#endif

#include "SDL.h"
#include "SDL_thread.h"

#include "shared.h"
#include "sms_ntsc.h"
#include "md_ntsc.h"
#include "utils.h"

#ifdef GCWZERO
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>

static int do_once = 1;
static int gcw0_w = 320;
static int gcw0_h = 240;
static int gotomenu;
static int show_lightgun;
time_t current_time;

const char *cursor[4]=
{
    "./CLASSIC_01_RED.png", //doesn't flash (for epileptics it's default)
    "./CLASSIC_02.png", //square flashing red and white
    "./CLASSIC_01.png",
    "./SQUARE_02.png",
};

#define JOY_DEADZONE 1000
#endif

#ifdef GCWZERO
#define SOUND_FREQUENCY 44100
#else
#define SOUND_FREQUENCY 48000
#endif
#define SOUND_SAMPLES_SIZE 2048

#define VIDEO_WIDTH  320
#define VIDEO_HEIGHT 240

int joynum = 0;

int log_error   = 0;
int debug_on    = 0;
int turbo_mode  = 0;
int use_sound   = 1;
int fullscreen  = 1; /* SDL_FULLSCREEN */

char rom_filename[256];

/* sound */

struct
{
    char* current_pos;
    char* buffer;
    int current_emulated_samples;
} sdl_sound;


static uint8 brm_format[0x40] =
{
    0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x5f,0x00,0x00,0x00,0x00,0x40,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x53,0x45,0x47,0x41,0x5f,0x43,0x44,0x5f,0x52,0x4f,0x4d,0x00,0x01,0x00,0x00,0x00,
    0x52,0x41,0x4d,0x5f,0x43,0x41,0x52,0x54,0x52,0x49,0x44,0x47,0x45,0x5f,0x5f,0x5f
};


static short soundframe[SOUND_SAMPLES_SIZE];

static void sdl_sound_callback(void *userdata, Uint8 *stream, int len)
{
    if(sdl_sound.current_emulated_samples < len)
    {
        memset(stream, 0, len);
    }
    else
    {
        memcpy(stream, sdl_sound.buffer, len);
        /* loop to compensate desync */
        do
        {
            sdl_sound.current_emulated_samples -= len;
        }
        while(sdl_sound.current_emulated_samples > 2 * len);
        memcpy(sdl_sound.buffer,
               sdl_sound.current_pos - sdl_sound.current_emulated_samples,
               sdl_sound.current_emulated_samples);
        sdl_sound.current_pos = sdl_sound.buffer + sdl_sound.current_emulated_samples;
    }
}

static int sdl_sound_init()
{
    int n;
    SDL_AudioSpec as_desired, as_obtained;

    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        MessageBox(NULL, "SDL Audio initialization failed", "Error", 0);
        return 0;
    }

    as_desired.freq     = SOUND_FREQUENCY;
    as_desired.format   = AUDIO_S16LSB;
    as_desired.channels = 2;
    as_desired.samples  = SOUND_SAMPLES_SIZE;
    as_desired.callback = sdl_sound_callback;

    if(SDL_OpenAudio(&as_desired, &as_obtained) == -1)
    {
        MessageBox(NULL, "SDL Audio open failed", "Error", 0);
        return 0;
    }
 
    if(as_desired.samples != as_obtained.samples)
    {
        MessageBox(NULL, "SDL Audio wrong setup", "Error", 0);
        return 0;
    }
 
    sdl_sound.current_emulated_samples = 0;
    n = SOUND_SAMPLES_SIZE * 2 * sizeof(short) * 20;
    sdl_sound.buffer = (char*)malloc(n);
    if(!sdl_sound.buffer)
    {
        MessageBox(NULL, "Can't allocate audio buffer", "Error", 0);
        return 0;
    }
    memset(sdl_sound.buffer, 0, n);
    sdl_sound.current_pos = sdl_sound.buffer;
    return 1;
}
 
static void sdl_sound_update(enabled)
{
    int size = audio_update(soundframe) * 2;
 
    if (enabled)
    {
        int i;
        short *out;
 
        SDL_LockAudio();
        out = (short*)sdl_sound.current_pos;
        for(i = 0; i < size; i++)
        {
            *out++ = soundframe[i];
        }
        sdl_sound.current_pos = (char*)out;
        sdl_sound.current_emulated_samples += size * sizeof(short);
        SDL_UnlockAudio();
    }
}
 
static void sdl_sound_close()
{
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    if (sdl_sound.buffer)
        free(sdl_sound.buffer);
}

#ifdef GCWZERO //A-stick support
static void sdl_joystick_init()
{
    if(SDL_Init(SDL_INIT_JOYSTICK) < 0)
    {
        MessageBox(NULL, "SDL Joystick initialization failed", "Error", 0);
        return 0;
    } 
    else
        MessageBox(NULL, "SDL Joystick initialisation successful", "Success", 0);
    return 1;
}
#endif
 
/* video */
md_ntsc_t *md_ntsc;
sms_ntsc_t *sms_ntsc;
 
struct
{
    SDL_Surface* surf_screen;
    SDL_Surface* surf_bitmap;
    SDL_Rect srect;
    SDL_Rect drect;
    Uint32 frames_rendered;
} sdl_video;
 
static int sdl_video_init()
{
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        MessageBox(NULL, "SDL Video initialization failed", "Error", 0);
        return;
    }
#ifdef GCWZERO
    sdl_video.surf_screen  = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT, 16, SDL_HWSURFACE |
#else
    sdl_video.surf_screen  = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT, 16, SDL_HWSURFACE | fullscreen |
#endif
#ifdef SDL_TRIPLEBUF
                             SDL_TRIPLEBUF
#else
                             SDL_DOUBLEBUF
#endif
                                             );
    sdl_video.surf_bitmap = SDL_CreateRGBSurface(SDL_HWSURFACE, 720, 576, 16, 0, 0, 0, 0);
    sdl_video.frames_rendered = 0;
    SDL_ShowCursor(0);
    return;
}
 
static void sdl_video_update()
{
static int test;
    if (system_hw == SYSTEM_MCD)
    {
#ifdef GCWZERO
        if (test >= config.gcw0_frameskip)  // >= in case frameskip has just been lowered
        {
            system_frame_scd(0); //render frame
            test = 0;
        } else {
            system_frame_scd(1); //skip frame render
            test ++;
        }
#else
        system_frame_scd(0);
#endif
    }
    else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD)
#ifdef GCWZERO
    {
        if (test >= config.gcw0_frameskip) 
        {
            system_frame_gen(0);
            test = 0;
        } else {
            system_frame_gen(1);
            test ++;
        }
#else
        system_frame_gen(0);
#endif
    }
    else
    {
#ifdef GCWZERO
        if (test >= config.gcw0_frameskip) 
        {
            system_frame_sms(0);
            test = 0;
        } else {
            system_frame_sms(1);
            test ++;
        }
#else
        system_frame_sms(0);
#endif
    }
 
    /* viewport size changed */
    if(bitmap.viewport.changed & 1)
    {
        bitmap.viewport.changed &= ~1;
 
        /* source bitmap */
#ifdef GCWZERO //remove left bar bug with SMS roms
        if ( (system_hw == SYSTEM_MARKIII) || (system_hw == SYSTEM_SMS) || (system_hw == SYSTEM_SMS2) || (system_hw == SYSTEM_PBC) )
        {
            if (config.smsmaskleftbar)
                sdl_video.srect.x = 8;
            else
                sdl_video.srect.x = 0;

        }
        else
        {
            sdl_video.srect.x = 0;
        }
#else
            sdl_video.srect.x = 0;
#endif
        sdl_video.srect.y = 0;
        sdl_video.srect.w = bitmap.viewport.w+2*bitmap.viewport.x;
        sdl_video.srect.h = bitmap.viewport.h+2*bitmap.viewport.y;
        if (sdl_video.srect.w > VIDEO_WIDTH)
        {
#ifdef GCWZERO
            if ( (system_hw == SYSTEM_MARKIII) || (system_hw == SYSTEM_SMS) || (system_hw == SYSTEM_SMS2) || (system_hw == SYSTEM_PBC) )
            {
                if (config.smsmaskleftbar)
                    sdl_video.srect.x = (sdl_video.srect.w - VIDEO_WIDTH) / 2 + 8;
                else
                    sdl_video.srect.x = (sdl_video.srect.w - VIDEO_WIDTH) / 2;
                sdl_video.srect.w = VIDEO_WIDTH;
            }
            else
            {
                sdl_video.srect.x = (sdl_video.srect.w - VIDEO_WIDTH) / 2;
                sdl_video.srect.w = VIDEO_WIDTH;
            }
#else
            sdl_video.srect.x = (sdl_video.srect.w - VIDEO_WIDTH) / 2;
            sdl_video.srect.w = VIDEO_WIDTH;
#endif
        }
        if (sdl_video.srect.h > VIDEO_HEIGHT)
        {
            sdl_video.srect.y = (sdl_video.srect.h - VIDEO_HEIGHT) / 2;
            sdl_video.srect.h = VIDEO_HEIGHT;
        }
 
        /* destination bitmap */
        sdl_video.drect.w = sdl_video.srect.w;
        sdl_video.drect.h = sdl_video.srect.h;
        sdl_video.drect.x = (VIDEO_WIDTH  - sdl_video.drect.w) / 2;
        sdl_video.drect.y = (VIDEO_HEIGHT - sdl_video.drect.h) / 2;
 
        /* clear destination surface */
        SDL_FillRect(sdl_video.surf_screen, 0, 0);
#ifdef GCWZERO //triple buffering so stop flicker
        SDL_Flip(sdl_video.surf_screen);
        SDL_FillRect(sdl_video.surf_screen, 0, 0);
        SDL_Flip(sdl_video.surf_screen);
        SDL_FillRect(sdl_video.surf_screen, 0, 0);
#endif
#if 0
        if (config.render && (interlaced || config.ntsc))  rect.h *= 2;
        if (config.ntsc) rect.w = (reg[12]&1) ? MD_NTSC_OUT_WIDTH(rect.w) : SMS_NTSC_OUT_WIDTH(rect.w);
        if (config.ntsc)
        {
            sms_ntsc = (sms_ntsc_t *)malloc(sizeof(sms_ntsc_t));
            md_ntsc = (md_ntsc_t *)malloc(sizeof(md_ntsc_t));
 
            switch (config.ntsc)
            {
            case 1:
                sms_ntsc_init(sms_ntsc, &sms_ntsc_composite);
                md_ntsc_init(md_ntsc, &md_ntsc_composite);
                break;
            case 2:
                sms_ntsc_init(sms_ntsc, &sms_ntsc_svideo);
                md_ntsc_init(md_ntsc, &md_ntsc_svideo);
                break;
            case 3:
                sms_ntsc_init(sms_ntsc, &sms_ntsc_rgb);
                md_ntsc_init(md_ntsc, &md_ntsc_rgb);
                break;
            }
        }
        else
        {
            if (sms_ntsc)
            {
                free(sms_ntsc);
                sms_ntsc = NULL;
            }
 
            if (md_ntsc)
            {
                free(md_ntsc);
                md_ntsc = NULL;
            }
        }
#endif
    }
 
//DK IPU scaling for gg/sms roms
#ifdef GCWZERO
    if (config.gcw0_fullscreen)
    {
        if( (gcw0_w != sdl_video.drect.w) || (gcw0_h != sdl_video.drect.h) )
        {
            if ( (system_hw == SYSTEM_MARKIII) || (system_hw == SYSTEM_SMS) || (system_hw == SYSTEM_SMS2) || (system_hw == SYSTEM_PBC) )
            {
                if (config.smsmaskleftbar)
                {
                    sdl_video.srect.w = sdl_video.srect.w - 8;
                    sdl_video.drect.w = sdl_video.srect.w;
                    sdl_video.drect.x = 4;
                }
                else
                {
                    sdl_video.srect.w = sdl_video.srect.w ;
                    sdl_video.drect.w = sdl_video.srect.w;
                    sdl_video.drect.x = 0;
                }

            }
            else
            {
                sdl_video.drect.x = 0;
                sdl_video.drect.w = sdl_video.srect.w;
            }

            sdl_video.drect.h = sdl_video.srect.h;
            sdl_video.drect.y = 0;
            gcw0_w=sdl_video.drect.w;
            gcw0_h=sdl_video.drect.h;

            if ( (system_hw == SYSTEM_MARKIII) || (system_hw == SYSTEM_SMS) || (system_hw == SYSTEM_SMS2) || (system_hw == SYSTEM_PBC) )
            {
                sdl_video.surf_screen  = SDL_SetVideoMode(256,gcw0_h, 16, SDL_HWSURFACE |
#ifdef SDL_TRIPLEBUF
                                         SDL_TRIPLEBUF);
#else
                                         SDL_DOUBLEBUF);
#endif
            }
            else
            { 
            sdl_video.surf_screen  = SDL_SetVideoMode(gcw0_w,gcw0_h, 16, SDL_HWSURFACE |
#ifdef SDL_TRIPLEBUF
                                     SDL_TRIPLEBUF);
#else
                                     SDL_DOUBLEBUF);
#endif
            } 
        }
    }
    if (show_lightgun && !config.gcw0_fullscreen) // hack to remove cursor corruption of over game screen edge
    {
        SDL_FillRect(sdl_video.surf_screen, 0, 0);
    }
#endif
    SDL_BlitSurface(sdl_video.surf_bitmap, &sdl_video.srect, sdl_video.surf_screen, &sdl_video.drect);
    //SDL_UpdateRect(sdl_video.surf_screen, 0, 0, 0, 0);
#ifdef GCWZERO
//  Add scanlines to Game Gear games if requested
    if ( (system_hw == SYSTEM_GG) && config.gg_scanlines)
    {
        SDL_Surface *scanlinesSurface;
        scanlinesSurface = IMG_Load("./scanlines.png");
        SDL_BlitSurface(scanlinesSurface, NULL, sdl_video.surf_screen, &sdl_video.drect);
	SDL_FreeSurface(scanlinesSurface);
    }
    if (show_lightgun)
    {
//      Remove previous cursor from black bars
        if (config.gcw0_fullscreen)
        {
            if (config.smsmaskleftbar)
            {
                if(system_hw == SYSTEM_SMS2)
                {
                SDL_Rect srect;
                srect.x = 0;
                srect.y = 0;
                srect.w = 4;
                srect.h = 192;
                SDL_FillRect(sdl_video.surf_screen, &srect, SDL_MapRGB(sdl_video.surf_screen->format, 0, 0, 0));
                srect.x = 252;
                SDL_FillRect(sdl_video.surf_screen, &srect, SDL_MapRGB(sdl_video.surf_screen->format, 0, 0, 0));
                }
            }
        }
        /* get mouse coordinates (absolute values) */
        int x,y;
        int state = SDL_GetMouseState(&x,&y);

        SDL_Rect lrect;
        lrect.x = x-7;
        lrect.y = y-7;
        lrect.w = 15;
        lrect.h = 15;

        SDL_Surface *lightgunSurface;
        lightgunSurface = IMG_Load(cursor[config.cursor]);
        static lightgun_af = 0;
        SDL_Rect srect;
        srect.y = 0;
        srect.w = 15;
        srect.h = 15;

        //only show cursor if movement occurred within 3 seconds.
        time_t current_time2;
        current_time2 = time(NULL);

        if (lightgun_af >= 10)
        {
            srect.x = 0;
            if((current_time2 - current_time) < 3)
                SDL_BlitSurface(lightgunSurface, &srect, sdl_video.surf_screen, &lrect);
        } else
        {
            if (config.cursor != 0)
                srect.x = 15;
            else 
                srect.x = 0;
            if((current_time2 - current_time) < 3)
                SDL_BlitSurface(lightgunSurface, &srect, sdl_video.surf_screen, &lrect);
        }
        lightgun_af++;
        if (lightgun_af == 20) lightgun_af = 0;
        SDL_FreeSurface(lightgunSurface);
    } //show_lightgun
#endif

    SDL_Flip(sdl_video.surf_screen);
    ++sdl_video.frames_rendered;
}
 
static void sdl_video_close()
{
    if (sdl_video.surf_bitmap)
        SDL_FreeSurface(sdl_video.surf_bitmap);
    if (sdl_video.surf_screen)
        SDL_FreeSurface(sdl_video.surf_screen);
}
 
/* Timer Sync */
 
struct
{
    SDL_sem* sem_sync;
    unsigned ticks;
} sdl_sync;
 
static Uint32 sdl_sync_timer_callback(Uint32 interval)
{
#ifdef GCWZERO
    if (!gotomenu) 
    {
        SDL_SemPost(sdl_sync.sem_sync);
        sdl_sync.ticks++;
    }
#else
    SDL_SemPost(sdl_sync.sem_sync);
    sdl_sync.ticks++;
#endif
    if (sdl_sync.ticks == (vdp_pal ? 50 : 20))
    {
        SDL_Event event;
        SDL_UserEvent userevent;
 
        userevent.type = SDL_USEREVENT;
        userevent.code = vdp_pal ? (sdl_video.frames_rendered / 3) : sdl_video.frames_rendered;
        userevent.data1 = NULL;
        userevent.data2 = NULL;
        sdl_sync.ticks = sdl_video.frames_rendered = 0;
 
        event.type = SDL_USEREVENT;
        event.user = userevent;
 
        SDL_PushEvent(&event);
    }
    return interval;
}
 
static int sdl_sync_init()
{
    if(SDL_InitSubSystem(SDL_INIT_TIMER|SDL_INIT_EVENTTHREAD) < 0)
    {
        MessageBox(NULL, "SDL Timer initialization failed", "Error", 0);
        return 0;
    }
 
    sdl_sync.sem_sync = SDL_CreateSemaphore(0);
    sdl_sync.ticks = 0;
    return 1;
}
 
static void sdl_sync_close()
{
    if(sdl_sync.sem_sync)
        SDL_DestroySemaphore(sdl_sync.sem_sync);
}
 
static const uint16 vc_table[4][2] =
{
    /* NTSC, PAL */
    {0xDA , 0xF2},  /* Mode 4 (192 lines) */
    {0xEA , 0x102}, /* Mode 5 (224 lines) */
    {0xDA , 0xF2},  /* Mode 4 (192 lines) */
    {0x106, 0x10A}  /* Mode 5 (240 lines) */
};
 
static int sdl_control_update(SDLKey keystate)
{
    switch (keystate)
    {
#ifndef GCWZERO
    case SDLK_TAB:
    {
        system_reset();
        break;
    }
#endif
 
    case SDLK_F1:
    {
        if (SDL_ShowCursor(-1)) SDL_ShowCursor(0);
        else SDL_ShowCursor(1);
        break;
    }
 
    case SDLK_F2:
    {
        if (fullscreen) fullscreen = 0;
        else fullscreen = SDL_FULLSCREEN;
        sdl_video.surf_screen = SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT, 16,  SDL_SWSURFACE | fullscreen);
        break;
    }
 
    case SDLK_F3:
    {
        if (config.bios == 0) config.bios = 3;
        else if (config.bios == 3) config.bios = 1;
        break;
    }
 
    case SDLK_F4:
    {
        if (!turbo_mode) use_sound ^= 1;
        break;
    }
 
    case SDLK_F5:
    {
        log_error ^= 1;
        break;
    }
 
    case SDLK_F6:
    {
        if (!use_sound)
        {
            turbo_mode ^=1;
            sdl_sync.ticks = 0;
        }
        break;
    }
 
    case SDLK_F7:
    {
        char save_state_file[256];
        sprintf(save_state_file,"%s/%X.gp0", get_save_directory(), rominfo.realchecksum);
        FILE *f = fopen(save_state_file,"rb");
 
        if (f)
        {
            uint8 buf[STATE_SIZE];
            fread(&buf, STATE_SIZE, 1, f);
            state_load(buf);
            fclose(f);
        }
        break;
    }
 
    case SDLK_F8:
    {
        char save_state_file[256];
        sprintf(save_state_file,"%s/%X.gp0", get_save_directory(), rominfo.realchecksum);
        FILE *f = fopen(save_state_file,"wb");
        if (f)
        {
            uint8 buf[STATE_SIZE];
            int len = state_save(buf);
            fwrite(&buf, len, 1, f);
            fclose(f);
        }
        break;
    }
 
    case SDLK_F9:
    {
        config.region_detect = (config.region_detect + 1) % 5;
        get_region(0);
 
        /* framerate has changed, reinitialize audio timings */
        audio_init(snd.sample_rate, 0);
 
        /* system with region BIOS should be reinitialized */
        if ((system_hw == SYSTEM_MCD) || ((system_hw & SYSTEM_SMS) && (config.bios & 1)))
        {
            system_init();
            system_reset();
        }
        else
        {
            /* reinitialize I/O region register */
            if (system_hw == SYSTEM_MD)
            {
                io_reg[0x00] = 0x20 | region_code | (config.bios & 1);
            }
            else
            {
                io_reg[0x00] = 0x80 | (region_code >> 1);
            }
 
            /* reinitialize VDP */
            if (vdp_pal)
            {
                status |= 1;
                lines_per_frame = 313;
            }
            else
            {
                status &= ~1;
                lines_per_frame = 262;
            }
 
            /* reinitialize VC max value */
            switch (bitmap.viewport.h)
            {
            case 192:
                vc_max = vc_table[0][vdp_pal];
                break;
            case 224:
                vc_max = vc_table[1][vdp_pal];
                break;
            case 240:
                vc_max = vc_table[3][vdp_pal];
                break;
            }
        }
        break;
    }
 
    case SDLK_F10:
    {
        gen_reset(0);
        break;
    }
 
    case SDLK_F11:
    {
        config.overscan =  (config.overscan + 1) & 3;
        if ((system_hw == SYSTEM_GG) && !config.gg_extra)
        {
            bitmap.viewport.x = (config.overscan & 2) ? 14 : -48;
        }
        else
        {
            bitmap.viewport.x = (config.overscan & 2) * 7;
        }
        bitmap.viewport.changed = 3;
        break;
    }
    case SDLK_F12:
    {
        joynum = (joynum + 1) % MAX_DEVICES;
        while (input.dev[joynum] == NO_DEVICE)
        {
            joynum = (joynum + 1) % MAX_DEVICES;
        }
        break;
    }
 
    case SDLK_ESCAPE:
    {
#ifndef GCWZERO
        /* exit */
        return 0;
#endif
    }
 
    default:
        break;
    }
 
    return 1;
}
 
static void shutdown()
{
    FILE *fp;
 
    if (system_hw == SYSTEM_MCD)
    {
        /* save internal backup RAM (if formatted) */
        char brm_file[256];
        if (!memcmp(scd.bram + 0x2000 - 0x20, brm_format + 0x20, 0x20))
        {
			sprintf(brm_file,"%s/", get_save_directory(), "scd.brm");
			fp = fopen(brm_file, "wb");
            if (fp!=NULL)
            {
                fwrite(scd.bram, 0x2000, 1, fp);
                fclose(fp);
            }
        }
 
        /* save cartridge backup RAM (if formatted) */
        if (scd.cartridge.id)
        {
            if (!memcmp(scd.cartridge.area + scd.cartridge.mask + 1 - 0x20, brm_format + 0x20, 0x20))
            {
				sprintf(brm_file,"%s/", get_save_directory(), "cart.brm");
				fp = fopen(brm_file, "wb");
                if (fp!=NULL)
                {
                    fwrite(scd.cartridge.area, scd.cartridge.mask + 1, 1, fp);
                    fclose(fp);
                }
            }
        }
    }
 
    if (sram.on)
    {
        /* save SRAM */
        char save_file[256];
        if (rom_filename[0] != '\0') {
			sprintf(save_file,"%s/%s.srm", get_save_directory(), rom_filename);
			fp = fopen(save_file, "wb");
			if (fp!=NULL)
			{
				fwrite(sram.sram,0x10000,1, fp);
				fclose(fp);
			}
		}
    }
    audio_shutdown();
    error_shutdown();
 
    sdl_video_close();
    sdl_sound_close();
    sdl_sync_close();
    SDL_Quit();
}
 
#ifdef GCWZERO //menu!
static int gcw0menu(void)
{
    SDL_PauseAudio(1);

    /* display menu */
//  change video mode
    sdl_video.surf_screen  = SDL_SetVideoMode(320,240, 32, SDL_HWSURFACE |
#ifdef SDL_TRIPLEBUF
       SDL_TRIPLEBUF);
#else
       SDL_DOUBLEBUF);
#endif
//  blank screen
    SDL_FillRect(sdl_video.surf_screen, 0, 0);
 
    enum {MAINMENU = 0, GRAPHICS_OPTIONS = 1, REMAP_OPTIONS = 2, SAVE_STATE = 3, LOAD_STATE = 4, MISC_OPTIONS = 5};
    static int menustate  = MAINMENU;
//  Menu text
    const char *gcw0menu_mainlist[9]=
    {
        "Resume game",
        "Save state",
        "Load state",
        "Graphics options",
        "Remap buttons",
        "Misc. Options",
            
        "", //spacer
        "Reset",
        "Quit"
    };
    const char *gcw0menu_gfxlist[6]=
    {
        "Scaling",
        "Keep aspect ratio",
        "Scanlines (GG)",
        "Mask left bar (SMS)",
        "Frameskip",
        "Return to main menu",
    };
    const char *gcw0menu_numericlist[4]=
    {
        "0",
        "1",
        "2",
        "3",
    };
    const char *gcw0menu_onofflist[2]=
    {
        "Off",
        "On",
    };
    const char *gcw0menu_remapoptionslist[9]=
    {
        "A",
        "B",
        "C",
        "X",
        "Y",
        "Z",
        "Start",
        "Mode",
        "Return to main menu",
    };
    const char *gcw0menu_savestate[10]=
    {
        "Back to main menu",
        "Save state 1 (Quicksave)",
        "Save state 2",
        "Save state 3",
        "Save state 4",
        "Save state 5",
        "Save state 6",
        "Save state 7",
        "Save state 8",
        "Save state 9",
    };
    const char *gcw0menu_loadstate[10]=
    {
        "Back to main menu",
        "Load state 1 (Quickload)",
        "Load state 2",
        "Load state 3",
        "Load state 4",
        "Load state 5",
        "Load state 6",
        "Load state 7",
        "Load state 8",
        "Load state 9",
    };
    const char *gcw0menu_misc[7]=
    {
        "Back to main menu",
        "Resume on Save/Load",
        "A-stick",
        "Lock-on",
        "FM sound (SMS)",
        "Lightgun speed",
        "Lightgun Cursor",
    };

    const char *lock_on_desc[4]=
    {
		"             Off",
		"      Game Genie",
		"   Action Replay",
		"Sonic & Knuckles",
	};

//  start menu loop
    bitmap.viewport.changed=1; //change screen res if required
    while(gotomenu)
    {
//      set up menu surface
        SDL_Surface *menuSurface = NULL;
        menuSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 16, 0, 0, 0, 0);
 
//  identify system we are using to show correct background just cos we can :P
        if      (  system_hw == SYSTEM_PICO) //Sega Pico
        {
            SDL_Surface *tempbgSurface;
            SDL_Surface *bgSurface;
            tempbgSurface = IMG_Load( "./PICO.png" );
            bgSurface = SDL_DisplayFormat( tempbgSurface );
            SDL_BlitSurface(bgSurface, NULL, menuSurface, NULL);
            SDL_FreeSurface(tempbgSurface);
            SDL_FreeSurface(bgSurface);
        }
        else if ( (system_hw == SYSTEM_SG)      || (system_hw == SYSTEM_SGII) ) //SG-1000 I&II
        {
            SDL_Surface *tempbgSurface;
            SDL_Surface *bgSurface;
            tempbgSurface = IMG_Load( "./SG1000.png" );
            bgSurface = SDL_DisplayFormat( tempbgSurface );
            SDL_BlitSurface(bgSurface, NULL, menuSurface, NULL);
            SDL_FreeSurface(tempbgSurface);
            SDL_FreeSurface(bgSurface);
        }
        else if ( (system_hw == SYSTEM_MARKIII) || (system_hw == SYSTEM_SMS) || ( system_hw == SYSTEM_GGMS) || (system_hw == SYSTEM_SMS2) || (system_hw == SYSTEM_PBC) ) //Mark III & Sega Master System I&II & Megadrive with power base converter
        {
            SDL_Surface *tempbgSurface;
            SDL_Surface *bgSurface;
            tempbgSurface = IMG_Load( "./SMS.png" );
            bgSurface = SDL_DisplayFormat( tempbgSurface );
            SDL_BlitSurface(bgSurface, NULL, menuSurface, NULL);
            SDL_FreeSurface(tempbgSurface);
            SDL_FreeSurface(bgSurface);
        }
        else if (system_hw == SYSTEM_GG)
        {
            SDL_Surface *tempbgSurface;
            SDL_Surface *bgSurface;
            tempbgSurface = IMG_Load( "./GG.png" );
            bgSurface = SDL_DisplayFormat( tempbgSurface );
            SDL_BlitSurface(bgSurface, NULL, menuSurface, NULL);
            SDL_FreeSurface(tempbgSurface);
            SDL_FreeSurface(bgSurface);
        }
        else if (  system_hw == SYSTEM_MD)   //Megadrive
        {
            SDL_Surface *tempbgSurface;
            SDL_Surface *bgSurface;
            tempbgSurface = IMG_Load( "./MD.png" );
            bgSurface = SDL_DisplayFormat( tempbgSurface );
            SDL_BlitSurface(bgSurface, NULL, menuSurface, NULL);
            SDL_FreeSurface(tempbgSurface);
            SDL_FreeSurface(bgSurface);
        }
        else if (  system_hw == SYSTEM_MCD)  //MegaCD
        {
            SDL_Surface *tempbgSurface;
            SDL_Surface *bgSurface;
            tempbgSurface = IMG_Load( "./MCD.png" );
            bgSurface = SDL_DisplayFormat( tempbgSurface );
            SDL_BlitSurface(bgSurface, NULL, menuSurface, NULL);
            SDL_FreeSurface(tempbgSurface);
            SDL_FreeSurface(bgSurface);
        }

//      show menu
        TTF_Init();
        TTF_Font *ttffont = NULL;
        SDL_Color text_color = {180, 180, 180};
        SDL_Color selected_text_color = {23, 86, 155}; //selected colour = Sega blue ;)
        SDL_Surface *textSurface;

        int i;
        static int selectedoption = 0;

//      Fill menu box
        SDL_Surface *MenuBackground;
        if (menustate == REMAP_OPTIONS)
        {
            MenuBackground = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 185, 16, 0, 0, 0, 0);
            SDL_Rect rect;
            rect.x = 0;
            rect.y = 35;
            rect.w = 320;
            rect.h = 185;
            SDL_FillRect(MenuBackground, 0, 0);
            SDL_SetAlpha(MenuBackground, SDL_SRCALPHA, 50);
            SDL_BlitSurface(MenuBackground, NULL, menuSurface, &rect);
            SDL_FreeSurface(MenuBackground);
        }
        else
        {
            MenuBackground = SDL_CreateRGBSurface(SDL_HWSURFACE, 180, 185, 16, 0, 0, 0, 0);
            SDL_Rect rect;
            rect.x = 60;
            rect.y = 35;
            rect.w = 180;
            rect.h = 185;
            SDL_FillRect(MenuBackground, 0, 0);
            SDL_SetAlpha(MenuBackground, SDL_SRCALPHA, 50);
            SDL_BlitSurface(MenuBackground, NULL, menuSurface, &rect);
            SDL_FreeSurface(MenuBackground);
        }

//	Show title
        ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
        SDL_Rect destination;
        destination.x = 80;
        destination.y = 40;
        destination.w = 100;
        destination.h = 50;
        textSurface = TTF_RenderText_Solid(ttffont, "Genesis Plus GX", text_color);
        SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
        SDL_FreeSurface(textSurface);
        TTF_CloseFont (ttffont);
 
        if (menustate == MAINMENU)
        {
          //there's no need to open/close font each cycle :P
            ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            for(i=0; i<9; i++)
            {
                SDL_Rect destination;
                destination.x = 80;
                destination.y = 70+(15*i);
                destination.w = 100;
                destination.h = 50;
                if (i == selectedoption)
                    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_mainlist[i], selected_text_color);
                else
                    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_mainlist[i], text_color);
                SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
                SDL_FreeSurface(textSurface);
            }
            TTF_CloseFont (ttffont);
        }
        else if (menustate == GRAPHICS_OPTIONS)
        {
			ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            for(i=0; i<6; i++)
            {
                SDL_Rect destination;
                destination.x = 80;
                destination.y = 70+(15*i);
                destination.w = 100;
                destination.h = 50;
                if ((i+10) == selectedoption)
		            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_gfxlist[i], selected_text_color);
		        else
		            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_gfxlist[i], text_color);
                SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
                SDL_FreeSurface(textSurface);
            }
            /* Display On/Off */
            SDL_Rect destination;
	        destination.x = 220;
	        destination.w = 100; 
	        destination.h = 50;
//          Scaling
            destination.y = 70+(15*0);
            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_onofflist[config.gcw0_fullscreen], selected_text_color);
    	    SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
	        SDL_FreeSurface(textSurface);
//          Aspect ratio
            destination.y = 70+(15*1);
    	    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_onofflist[config.keepaspectratio], selected_text_color);
	        SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
	        SDL_FreeSurface(textSurface);
//          Scanlines
            destination.y = 70+(15*2);
    	    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_onofflist[config.gg_scanlines], selected_text_color);
	        SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
	        SDL_FreeSurface(textSurface);
//          Mask left bar
            destination.y = 70+(15*3);
    	    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_onofflist[config.smsmaskleftbar], selected_text_color);
	        SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
	        SDL_FreeSurface(textSurface);
//          Frameskip
            destination.y = 70+(15*4);
    	    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_numericlist[config.gcw0_frameskip], selected_text_color);
	        SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
	        SDL_FreeSurface(textSurface);

            TTF_CloseFont (ttffont);

        }
        else if (menustate == REMAP_OPTIONS)
        {
            char* remap_text[256];
            ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            sprintf(remap_text, "%s%25s", "GenPlus", "GCW-Zero");
            SDL_Rect destination = {30, 60, 100, 50};
            textSurface = TTF_RenderText_Solid(ttffont, remap_text, text_color);
            SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);

            for(i=0; i < 9; i++)
            {
                if (i < 8)
                {
                    sprintf(remap_text, "%-5s                   %-7s", gcw0menu_remapoptionslist[i], gcw0_get_key_name(config.buttons[i]));
                } else
                {
                    sprintf(remap_text, gcw0menu_remapoptionslist[i]); // for return option
                }
                SDL_Rect destination = {30, 80 + (15 * i), 100, 50};
                if ((i+20) == selectedoption)
                {
                    textSurface = TTF_RenderText_Solid(ttffont, remap_text, selected_text_color);
                } else
                {
                    textSurface = TTF_RenderText_Solid(ttffont, remap_text, text_color);
                }
                SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
                SDL_FreeSurface(textSurface);
            }
            TTF_CloseFont (ttffont);
        }
        else if (menustate == SAVE_STATE)
        {
          //Show saved BMP as background if available
            SDL_Surface* screenshot;
            char load_state_screenshot[256];
            sprintf(load_state_screenshot,"%s/%s.%d.bmp", get_save_directory(), rom_filename, selectedoption-30);
            screenshot = SDL_LoadBMP(load_state_screenshot);
            if (screenshot)
            {
                SDL_Rect destination;
                destination.x = (320 - screenshot->w) / 2;
                destination.y = (240 - screenshot->h) / 2;
                destination.w = 320;
                destination.h = 240;
                SDL_BlitSurface(screenshot, NULL, menuSurface, &destination);

//              Fill menu box
                SDL_Surface *MenuBackground = SDL_CreateRGBSurface(SDL_HWSURFACE, 180, 185, 16, 0, 0, 0, 0);
                SDL_Rect rect;
                rect.x = 60;
                rect.y = 35;
                rect.w = 180;
                rect.h = 185;
                SDL_FillRect(MenuBackground, 0, 0);
                SDL_SetAlpha(MenuBackground, SDL_SRCALPHA, 180);
                SDL_BlitSurface(MenuBackground, NULL, menuSurface, &rect);
                SDL_FreeSurface(MenuBackground);
            }
            SDL_FreeSurface(screenshot);

//          Show title
            ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            SDL_Rect destination;
            destination.x = 80;
            destination.y = 40;
            destination.w = 100;
            destination.h = 50;
            textSurface = TTF_RenderText_Solid(ttffont, "Genesis Plus GX", text_color);
            SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);
            TTF_CloseFont (ttffont);

            ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            for(i=0; i<10; i++)
            {
                SDL_Rect destination;
                destination.x = 80;
                destination.y = 70+(15*i);
                destination.w = 100;
                destination.h = 50;
                if ((i+30) == selectedoption)
                    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_savestate[i], selected_text_color);
	        else
                    textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_savestate[i], text_color);
                SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
                SDL_FreeSurface(textSurface);
            }
            TTF_CloseFont (ttffont);
        }
        else if (menustate == LOAD_STATE)
        {
          //Show saved BMP as background if available
            SDL_Surface* screenshot;
            char load_state_screenshot[256];
            sprintf(load_state_screenshot,"%s/%s.%d.bmp", get_save_directory(), rom_filename, selectedoption-40);
            screenshot = SDL_LoadBMP(load_state_screenshot);
            if (screenshot)
            {
                SDL_Rect destination;
                destination.x = (320 - screenshot->w) / 2;
                destination.y = (240 - screenshot->h) / 2;
                destination.w = 320;
                destination.h = 240;
                SDL_BlitSurface(screenshot, NULL, menuSurface, &destination);

//              Fill menu box
                SDL_Surface *MenuBackground = SDL_CreateRGBSurface(SDL_HWSURFACE, 180, 185, 16, 0, 0, 0, 0);
                SDL_Rect rect;
                rect.x = 60;
                rect.y = 35;
                rect.w = 180;
                rect.h = 185;
                SDL_FillRect(MenuBackground, 0, 0);
                SDL_SetAlpha(MenuBackground, SDL_SRCALPHA, 180);
                SDL_BlitSurface(MenuBackground, NULL, menuSurface, &rect);
                SDL_FreeSurface(MenuBackground);
            }
            SDL_FreeSurface(screenshot);

//          Show title
            ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            SDL_Rect destination;
            destination.x = 80;
            destination.y = 40;
            destination.w = 100;
            destination.h = 50;
            textSurface = TTF_RenderText_Solid(ttffont, "Genesis Plus GX", text_color);
            SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);
            TTF_CloseFont (ttffont);

            ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            for(i=0; i<10; i++)
            {
                SDL_Rect destination;
                destination.x = 80;
                destination.y = 70+(15*i);
                destination.w = 100;
                destination.h = 50;
                if ((i+40) == selectedoption)
		            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_loadstate[i], selected_text_color);
		        else
		            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_loadstate[i], text_color);
                SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
                SDL_FreeSurface(textSurface);
            }
            TTF_CloseFont (ttffont);
        }
        else if (menustate == MISC_OPTIONS)
        {
            ttffont = TTF_OpenFont("./ProggyTiny.ttf", 16);
            for(i=0; i<7; i++)
            {
                SDL_Rect destination;
                destination.x = 80;
                destination.y = 70+(15*i);
                destination.w = 100;
                destination.h = 50;
                if ((i+50) == selectedoption)
		            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_misc[i], selected_text_color);
		        else
		            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_misc[i], text_color);
                SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
                SDL_FreeSurface(textSurface);
            }
            /* Display On/Off */
            SDL_Rect destination;
            destination.x = 220;
            destination.w = 100; 
            destination.h = 50;
//          Save/load autoresume
            destination.y = 70+(15*1);
            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_onofflist[config.sl_autoresume], selected_text_color);
    	    SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);
//          A-stick
            destination.y = 70+(15*2);
            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_onofflist[config.a_stick], selected_text_color);
    	    SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);
            /* Display Lock-on Types */
            destination.x = 142;
            destination.y = 70+(15*3);
            textSurface = TTF_RenderText_Solid(ttffont, lock_on_desc[config.lock_on], selected_text_color);
    	    SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);
//          FM sound(SMS)
            destination.x = 220;
            destination.y = 70+(15*4);
            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_onofflist[config.ym2413], selected_text_color);
    	    SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);
//          Lightgun speed
            destination.x = 220;
            destination.y = 70+(15*5);
            textSurface = TTF_RenderText_Solid(ttffont, gcw0menu_numericlist[config.lightgun_speed], selected_text_color);
    	    SDL_BlitSurface(textSurface, NULL, menuSurface, &destination);
            SDL_FreeSurface(textSurface);
//          Lightgun Cursor
            destination.y = 70+(15*6);
            SDL_Surface *lightgunSurface;
            lightgunSurface = IMG_Load(cursor[config.cursor]);
            static lightgun_af_demo = 0;
            SDL_Rect srect;
            srect.x = 0;
            srect.y = 0;
            srect.w = 15;
            srect.h = 15;
            if (lightgun_af_demo >= 10 && config.cursor != 0)
            {
                srect.x = 15;
            }
            lightgun_af_demo++;
            if (lightgun_af_demo == 20) lightgun_af_demo = 0;
            SDL_BlitSurface(lightgunSurface, &srect, menuSurface, &destination);
            SDL_FreeSurface(lightgunSurface);

            TTF_CloseFont (ttffont);

        }

//TODO other menu's go here
 
 
        /* Update display */
        SDL_Rect dest;
        dest.w = 320;
        dest.h = 240;
        dest.x = 0;
        dest.y = 0;
        SDL_BlitSurface(menuSurface, NULL, sdl_video.surf_screen, &dest);
        SDL_FreeSurface(menuSurface);
        SDL_Flip(sdl_video.surf_screen);
        /* Check for user input */
        SDL_EnableKeyRepeat(0,0);
        static int keyheld=0;
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_KEYDOWN:
                sdl_control_update(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                keyheld = 0;
                break;
            default:
                break;
            }
        }
        if (event.type == SDL_KEYDOWN && !keyheld)
        {
            keyheld++;
            uint8 *keystate2;
            keystate2 = SDL_GetKeyState(NULL);
            if(keystate2[SDLK_DOWN])
            {
                if        (selectedoption >  9 && selectedoption < 20) //graphics menu
                {
                    selectedoption++;
                    if (selectedoption == 16) selectedoption = 10;
                } 
                else if (selectedoption > 19 && selectedoption < 30) //remap menu
                {
                    selectedoption++;
                    if (selectedoption == 29)    selectedoption = 20;
                } 
                else if (selectedoption > 29 && selectedoption < 40) //save menu
                {
                    selectedoption++;
                    if (selectedoption == 40)    selectedoption = 30;
    	        } 
                else if (selectedoption > 39 && selectedoption < 50) //load menu
    	        {
                    selectedoption++;
                    if (selectedoption == 50)    selectedoption = 40;
                } 
                else if (selectedoption > 49 && selectedoption < 60) //misc menu
    	        {
                    selectedoption++;
                    if (selectedoption == 57)    selectedoption = 50;
                } 
                else  //main menu
                {
                    selectedoption++;
                    if (selectedoption == 6)     selectedoption = 7;
	            if (selectedoption > 8)	     selectedoption=0;
    	        }
                SDL_Delay(100);
    	    }
            else if(keystate2[SDLK_UP])
            {
                if        (selectedoption > 9  && selectedoption < 20) //graphics menu
                {
                    selectedoption--;
                    if (selectedoption == 9)     selectedoption = 15;
    	        }
                else if (selectedoption > 19 && selectedoption < 30) //remap menu
    	        {
                    selectedoption--;
                    if (selectedoption == 19)    selectedoption = 28;
                }
                else if (selectedoption > 29 && selectedoption < 40) //save menu
                {
                    selectedoption--;
                    if (selectedoption == 29)    selectedoption = 39;
                }
                else if (selectedoption > 39 && selectedoption < 50) //load menu
                {
                    selectedoption--;
                    if (selectedoption == 39)    selectedoption = 49;
                }
                else if (selectedoption > 49 && selectedoption < 60) //misc menu
                {
                    selectedoption--;
                    if (selectedoption == 49)    selectedoption = 56;
                }
                else
    	        { //main menu
    	            if (!selectedoption)         selectedoption = 8;
  	            else                         selectedoption--;
	            if (selectedoption == 6)     selectedoption = 5;
    	        }
                SDL_Delay(100);
            }
	    else if(keystate2[SDLK_LALT] && menustate != REMAP_OPTIONS) //back to last menu or quit menu
            {
                if (menustate == GRAPHICS_OPTIONS)
                {
                    menustate = MAINMENU;
                    selectedoption = 3;
                    SDL_Delay(130);
                }
                else if (menustate == SAVE_STATE)
                {
                    menustate = MAINMENU;
                    selectedoption = 1;
                    SDL_Delay(130);
                }
                else if (menustate == LOAD_STATE)
                {
                    menustate = MAINMENU;
                    selectedoption = 2;
                    SDL_Delay(130);
                }
                else if (menustate == MISC_OPTIONS)
                {
                    menustate = MAINMENU;
                    selectedoption = 5;
                    SDL_Delay(130);
                }
                else if (menustate == MAINMENU)
                {
                    gotomenu=0;
                    selectedoption=0;
                    SDL_Delay(130);
	            break;
                }
            }
            else if(keystate2[SDLK_LCTRL] && menustate != REMAP_OPTIONS)
            {
                if (selectedoption == 0) 
	        { //Resume
	            gotomenu=0;
                    selectedoption=0;
                    SDL_Delay(130);
	            break;
                }
                else if (selectedoption == 1)   //Save
                {
                    menustate = SAVE_STATE;
                    selectedoption = 30;
                    SDL_Delay(130);
                }
                else if (selectedoption == 2)   //Load
                {
                    menustate = LOAD_STATE;
                    selectedoption = 40;
                    SDL_Delay(130);
                }
                else if (selectedoption == 3)   //Graphics
                {
                    menustate = GRAPHICS_OPTIONS;
                    selectedoption = 10;
	            SDL_Delay(200);
                }
                else if (selectedoption == 4)   //Remap
                {
                    menustate = REMAP_OPTIONS;
                    selectedoption=20;
                    SDL_Delay(200);
                }
                else if (selectedoption == 5)   //Misc.
                {
                    menustate = MISC_OPTIONS;
                    selectedoption=50;
                    SDL_Delay(200);
                }
                else if (selectedoption == 7)   //Reset
                {
                    gotomenu = 0;
                    selectedoption=0;
                    system_reset();
                    SDL_Delay(130);
                    break;
                }
                else if (selectedoption == 8)   //Quit
                {
                    exit(0);
                    SDL_Delay(130);
                    break;
                }
                else if (selectedoption == 10) 
                { //Scaling
                    config.gcw0_fullscreen = !config.gcw0_fullscreen;
                    SDL_Delay(130);
                    config_save();
                }
                else if (selectedoption == 11) 
                { //Keep aspect ratio
                    SDL_Delay(130);
                    config.keepaspectratio = !config.keepaspectratio;
                    config_save();
                    do_once = 1;
       	        }
                else if (selectedoption == 12) 
                { //Scanlines (GG)
                    SDL_Delay(130);
                    config.gg_scanlines = !config.gg_scanlines;
                    config_save();
                }
                else if (selectedoption == 13) 
                { //Mask left bar
                    SDL_Delay(130);
                    config.smsmaskleftbar = !config.smsmaskleftbar;
                    config_save();
                }
                else if (selectedoption == 14) 
                { //Frameskip
                    SDL_Delay(130);
                    config.gcw0_frameskip ++;
                    if (config.gcw0_frameskip == 4) config.gcw0_frameskip = 0;
                    config_save();
                }
                else if (selectedoption == 15) 
                { //Back to main menu
                    menustate = MAINMENU;
                    selectedoption = 3;
                    SDL_Delay(130);
                }
                else if (selectedoption == 30)
                {
                  //Return to main menu
                    menustate = MAINMENU;
                    selectedoption = 1;
                    SDL_Delay(130);
                }
                else if (selectedoption > 30 && selectedoption < 40)
                {
                  //save to selected savestate
                    char save_state_file[256];
                    sprintf(save_state_file,"%s/%s.gp%d", get_save_directory(), rom_filename, selectedoption-30);
                    FILE *f = fopen(save_state_file,"wb");
                    if (f)
                    {
                        uint8 buf[STATE_SIZE];
                        int len = state_save(buf);
                        fwrite(&buf, len, 1, f);
                        fclose(f);
                    }

                  //Save BMP screenshot
                    char save_state_screenshot[256];
                    sprintf(save_state_screenshot,"%s/%s.%d.bmp", get_save_directory(), rom_filename, selectedoption-30);
                    SDL_Surface* screenshot;
                    if (!config.gcw0_fullscreen)
                    {
                        screenshot = SDL_CreateRGBSurface(SDL_HWSURFACE, sdl_video.srect.w, sdl_video.srect.h, 16, 0, 0, 0, 0);
                        SDL_Rect temp;
                        temp.x = 0;
                        temp.y = 0;
                        temp.w = sdl_video.srect.w;
                        temp.h = sdl_video.srect.h;

                        SDL_BlitSurface(sdl_video.surf_bitmap, &temp, screenshot, &temp);
                        SDL_SaveBMP(screenshot, save_state_screenshot);
                        SDL_FreeSurface(screenshot);
                    }
                    else
                    {
                        screenshot = SDL_CreateRGBSurface(SDL_HWSURFACE, gcw0_w, gcw0_h, 16, 0, 0, 0, 0);
                        SDL_Rect temp;
                        temp.x = 0;
                        temp.y = 0;
                        temp.w = gcw0_w;
                        temp.h = gcw0_h;

                        SDL_BlitSurface(sdl_video.surf_bitmap, &temp, screenshot, &temp);
                        SDL_SaveBMP(screenshot, save_state_screenshot);
                        SDL_FreeSurface(screenshot);
                    }

                    SDL_Delay(250);
                    if (config.sl_autoresume)
                    {
                        menustate = MAINMENU;
                        selectedoption = 0;
                        gotomenu = 0;
                        break;
                    }
                }
                else if (selectedoption == 40)
                {
                  //return to main menu
                    menustate = MAINMENU;
                    selectedoption = 2;
                    SDL_Delay(130);
                }
                else if (selectedoption > 40 && selectedoption < 50)
                {
                  //load selected loadstate
                    char save_state_file[256];
                    sprintf(save_state_file,"%s/%s.gp%d", get_save_directory(), rom_filename, selectedoption-40 );
                    FILE *f = fopen(save_state_file,"rb");
                    if (f)
                    {
                        uint8 buf[STATE_SIZE];
                        fread(&buf, STATE_SIZE, 1, f);
                        state_load(buf);
                        fclose(f);
                    }
                    if (config.sl_autoresume)
                    {
                        gotomenu = 0;
                        menustate = MAINMENU;
                        selectedoption = 0;
                        SDL_Delay(250);
                        break;
                    }
                }
                else if (selectedoption == 50)
                {
                  //return to main menu
                    menustate = MAINMENU;
                    selectedoption = 5;
                    SDL_Delay(130);
                }
                else if (selectedoption == 51)
                {
                  //toggle auto resume when save/loading
                    config.sl_autoresume=!config.sl_autoresume;
                    config_save();
                    SDL_Delay(130);
                }
                else if (selectedoption == 52)
                {
                  //toggle A-Stick
                    config.a_stick=!config.a_stick;
                    config_save();
                    SDL_Delay(130);
                }
                else if (selectedoption == 53)
                {
                    config.lock_on = (++config.lock_on == 4)? 0 : config.lock_on;
                    config_save();
                    SDL_Delay(130);
                }
                else if (selectedoption == 54)
                {
                    config.ym2413 = !config.ym2413;
                    config_save();
                    SDL_Delay(130);
                }
                else if (selectedoption == 55)
                {
                    config.lightgun_speed++;
                    if (config.lightgun_speed == 4)
                        config.lightgun_speed = 1;
                    config_save();
                    SDL_Delay(130);
                }
                else if (selectedoption == 56)
                {
                    config.cursor++;
                    if (config.cursor == 4)
                        config.cursor = 0;
                    config_save();
                    SDL_Delay(130);
                }

            }
            else if(menustate == REMAP_OPTIONS)
            {// REMAP_OPTIONS needs to capture all input
                SDLKey pressed_key = 0;

                if (keystate2[SDLK_RETURN])
                    pressed_key = SDLK_RETURN;
                else if (keystate2[SDLK_LCTRL])
                    pressed_key = SDLK_LCTRL;
                else if (keystate2[SDLK_LALT]) 
                    pressed_key = SDLK_LALT;
                else if (keystate2[SDLK_LSHIFT]) 
                    pressed_key = SDLK_LSHIFT;
                else if (keystate2[SDLK_SPACE]) 
                    pressed_key = SDLK_SPACE;
                else if (keystate2[SDLK_TAB]) 
                    pressed_key = SDLK_TAB;
                else if (keystate2[SDLK_BACKSPACE]) 
                    pressed_key = SDLK_BACKSPACE;
                else if (keystate2[SDLK_ESCAPE]) 
                    pressed_key = SDLK_ESCAPE;

                if (pressed_key)
                {
                    if (selectedoption == 20)
                    {
                      //button a remap
                        config.buttons[A] = (pressed_key==SDLK_ESCAPE)? 0: pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 21)
                    {
                      //button b remap
                        config.buttons[B] = (pressed_key==SDLK_ESCAPE)? 0: pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 22)
                    {
                      //button c remap
                        config.buttons[C] = (pressed_key==SDLK_ESCAPE)? 0: pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 23)
                    {
                      //button x remap
                        config.buttons[X] = (pressed_key==SDLK_ESCAPE)? 0: pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 24)
                    {
                      //button y remap
                        config.buttons[Y] = (pressed_key==SDLK_ESCAPE)? 0: pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 25)
                    {
                      //button z remap
                        config.buttons[Z] = (pressed_key==SDLK_ESCAPE)? 0: pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 26)
                    {
                      //button start remap
                        config.buttons[START] = (pressed_key==SDLK_ESCAPE)? 0: pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 27)
                    {
                      //button mode remap
                        config.buttons[MODE] = pressed_key;
                        config_save();
                        SDL_Delay(130);
                        selectedoption++;
                    }
                    else if (selectedoption == 28)
                    {
                      //return to main menu
                        menustate = MAINMENU;
                        selectedoption = 4;
                        SDL_Delay(130);
                    }
                }
            }
        }
    }//menu loop
    SDL_PauseAudio(0);

    if(config.gcw0_fullscreen) {
        if ( (system_hw == SYSTEM_MARKIII) || (system_hw == SYSTEM_SMS) || (system_hw == SYSTEM_SMS2) || (system_hw == SYSTEM_PBC) )
        {
            gcw0_w=sdl_video.drect.w;
            gcw0_h=sdl_video.drect.h;
            sdl_video.surf_screen  = SDL_SetVideoMode(256,gcw0_h, 16, SDL_HWSURFACE |
#ifdef SDL_TRIPLEBUF
                                     SDL_TRIPLEBUF);
#else
                                     SDL_DOUBLEBUF);
#endif
        }
        else
        { 
            sdl_video.drect.w = sdl_video.srect.w;
            sdl_video.drect.h = sdl_video.srect.h;
            sdl_video.drect.x = 0;
            sdl_video.drect.y = 0;
            gcw0_w=sdl_video.drect.w;
            gcw0_h=sdl_video.drect.h;
            sdl_video.surf_screen  = SDL_SetVideoMode(gcw0_w,gcw0_h, 16, SDL_HWSURFACE |
#ifdef SDL_TRIPLEBUF
                                     SDL_TRIPLEBUF);
#else
                                     SDL_DOUBLEBUF);
#endif
        } 
    } else {
        SDL_FillRect(sdl_video.surf_screen, 0, 0);
        SDL_Flip(sdl_video.surf_screen);
        SDL_FillRect(sdl_video.surf_screen, 0, 0);
        SDL_Flip(sdl_video.surf_screen);
        SDL_FillRect(sdl_video.surf_screen, 0, 0);
        SDL_Flip(sdl_video.surf_screen);
    }
    return 1;
}
#endif
 
int sdl_input_update(void)
{
    uint8 *keystate = SDL_GetKeyState(NULL);

    /* reset input */
    input.pad[joynum] = 0;
    if(show_lightgun)
        input.pad[4] = 0; //player2:
    switch (input.dev[4])
    {
    case DEVICE_LIGHTGUN:
        show_lightgun = 1;
        /* get mouse coordinates (absolute values) */
        int x,y;
        int state = SDL_GetMouseState(&x,&y);
 
        if (config.gcw0_fullscreen)
        {
            input.analog[4][0] =  x;
            input.analog[4][1] =  y;
        } else
        {
            input.analog[4][0] =  x - (VIDEO_WIDTH-bitmap.viewport.w)/2;
            input.analog[4][1] =  y - (VIDEO_HEIGHT-bitmap.viewport.h)/2;
        } 
        if (config.smsmaskleftbar) x += 8;
        /* TRIGGER, B, C (Menacer only), START (Menacer & Justifier only) */
        if(keystate[SDLK_ESCAPE])  input.pad[4] |= INPUT_START;
    default:
        break;
    }
    switch (input.dev[joynum])
    {
    case DEVICE_LIGHTGUN:
    {

#ifdef GCWZERO
        show_lightgun = 2;
        /* get mouse coordinates (absolute values) */
        int x,y;
        int state = SDL_GetMouseState(&x,&y);
 
        if (config.gcw0_fullscreen)
        {
            input.analog[0][0] =  x;
            input.analog[0][1] =  y;
        } else
        {
            input.analog[0][0] =  x - (VIDEO_WIDTH-bitmap.viewport.w)/2;
            input.analog[0][1] =  y - (VIDEO_HEIGHT-bitmap.viewport.h)/2;
        } 
        if (config.smsmaskleftbar) x += 8;
        /* TRIGGER, B, C (Menacer only), START (Menacer & Justifier only) */
        if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_A;
        if(state & SDL_BUTTON_RMASK) input.pad[joynum] |= INPUT_B;
        if(state & SDL_BUTTON_MMASK) input.pad[joynum] |= INPUT_C;
        if(keystate[SDLK_ESCAPE])  input.pad[0] |= INPUT_START;
#else
        /* get mouse coordinates (absolute values) */
        int x,y;
        int state = SDL_GetMouseState(&x,&y);
 
        /* X axis */
	        input.analog[joynum][0] =  x - (VIDEO_WIDTH-bitmap.viewport.w)/2;
 
        /* Y axis */
        input.analog[joynum][1] =  y - (VIDEO_HEIGHT-bitmap.viewport.h)/2;
 
        /* TRIGGER, B, C (Menacer only), START (Menacer & Justifier only) */
        if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_A;
        if(state & SDL_BUTTON_RMASK) input.pad[joynum] |= INPUT_B;
        if(state & SDL_BUTTON_MMASK) input.pad[joynum] |= INPUT_C;
        if(keystate[SDLK_f])  input.pad[joynum] |= INPUT_START;
        break;
#endif
    }
#ifndef GCWZERO
    case DEVICE_PADDLE:
    {
        /* get mouse (absolute values) */
        int x;
        int state = SDL_GetMouseState(&x, NULL);
 
        /* Range is [0;256], 128 being middle position */
        input.analog[joynum][0] = x * 256 /VIDEO_WIDTH;
 
        /* Button I -> 0 0 0 0 0 0 0 I*/
        if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_B;
 
        break;
    }
 
    case DEVICE_SPORTSPAD:
    {
        /* get mouse (relative values) */
        int x,y;
        int state = SDL_GetRelativeMouseState(&x,&y);
 
        /* Range is [0;256] */
        input.analog[joynum][0] = (unsigned char)(-x & 0xFF);
        input.analog[joynum][1] = (unsigned char)(-y & 0xFF);
 
        /* Buttons I & II -> 0 0 0 0 0 0 II I*/
        if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_B;
        if(state & SDL_BUTTON_RMASK) input.pad[joynum] |= INPUT_C;
 
        break;
    }
 
    case DEVICE_MOUSE:
    {
    SDL_ShowCursor(1);
        /* get mouse (relative values) */
        int x,y;
        int state = SDL_GetRelativeMouseState(&x,&y);
 
        /* Sega Mouse range is [-256;+256] */
        input.analog[joynum][0] = x * 2;
        input.analog[joynum][1] = y * 2;
 
        /* Vertical movement is upsidedown */
        if (!config.invert_mouse)
            input.analog[joynum][1] = 0 - input.analog[joynum][1];
 
        /* Start,Left,Right,Middle buttons -> 0 0 0 0 START MIDDLE RIGHT LEFT */
        if(state & SDL_BUTTON_LMASK) input.pad[joynum] |= INPUT_B;
        if(state & SDL_BUTTON_RMASK) input.pad[joynum] |= INPUT_C;
        if(state & SDL_BUTTON_MMASK) input.pad[joynum] |= INPUT_A;
        if(keystate[SDLK_f])  input.pad[joynum] |= INPUT_START;
 
        break;
    }
 
    case DEVICE_XE_1AP:
    {
        /* A,B,C,D,Select,START,E1,E2 buttons -> E1(?) E2(?) START SELECT(?) A B C D */
        if(keystate[SDLK_a])  input.pad[joynum] |= INPUT_START;
        if(keystate[SDLK_s])  input.pad[joynum] |= INPUT_A;
        if(keystate[SDLK_d])  input.pad[joynum] |= INPUT_C;
        if(keystate[SDLK_f])  input.pad[joynum] |= INPUT_Y;
        if(keystate[SDLK_z])  input.pad[joynum] |= INPUT_B;
        if(keystate[SDLK_x])  input.pad[joynum] |= INPUT_X;
        if(keystate[SDLK_c])  input.pad[joynum] |= INPUT_MODE;
        if(keystate[SDLK_v])  input.pad[joynum] |= INPUT_Z;
 
        /* Left Analog Stick (bidirectional) */
        if(keystate[SDLK_UP])     input.analog[joynum][1]-=2;
        else if(keystate[SDLK_DOWN])   input.analog[joynum][1]+=2;
        else input.analog[joynum][1] = 128;
        if(keystate[SDLK_LEFT])   input.analog[joynum][0]-=2;
        else if(keystate[SDLK_RIGHT])  input.analog[joynum][0]+=2;
        else input.analog[joynum][0] = 128;
 
        /* Right Analog Stick (unidirectional) */
        if(keystate[SDLK_KP8])    input.analog[joynum+1][0]-=2;
        else if(keystate[SDLK_KP2])   input.analog[joynum+1][0]+=2;
        else if(keystate[SDLK_KP4])   input.analog[joynum+1][0]-=2;
        else if(keystate[SDLK_KP6])  input.analog[joynum+1][0]+=2;
        else input.analog[joynum+1][0] = 128;
 
        /* Limiters */
        if (input.analog[joynum][0] > 0xFF) input.analog[joynum][0] = 0xFF;
        else if (input.analog[joynum][0] < 0) input.analog[joynum][0] = 0;
        if (input.analog[joynum][1] > 0xFF) input.analog[joynum][1] = 0xFF;
        else if (input.analog[joynum][1] < 0) input.analog[joynum][1] = 0;
        if (input.analog[joynum+1][0] > 0xFF) input.analog[joynum+1][0] = 0xFF;
        else if (input.analog[joynum+1][0] < 0) input.analog[joynum+1][0] = 0;
        if (input.analog[joynum+1][1] > 0xFF) input.analog[joynum+1][1] = 0xFF;
        else if (input.analog[joynum+1][1] < 0) input.analog[joynum+1][1] = 0;
 
        break;
    }
 
    case DEVICE_PICO:
    {
        /* get mouse (absolute values) */
        int x,y;
        int state = SDL_GetMouseState(&x,&y);
 
        /* Calculate X Y axis values */
        input.analog[0][0] = 0x3c  + (x * (0x17c-0x03c+1)) / VIDEO_WIDTH;
        input.analog[0][1] = 0x1fc + (y * (0x2f7-0x1fc+1)) / VIDEO_HEIGHT;
 
        /* Map mouse buttons to player #1 inputs */
        if(state & SDL_BUTTON_MMASK) pico_current = (pico_current + 1) & 7;
        if(state & SDL_BUTTON_RMASK) input.pad[0] |= INPUT_PICO_RED;
        if(state & SDL_BUTTON_LMASK) input.pad[0] |= INPUT_PICO_PEN;
 
        break;
    }
 
    case DEVICE_TEREBI:
    {
        /* get mouse (absolute values) */
        int x,y;
        int state = SDL_GetMouseState(&x,&y);
 
        /* Calculate X Y axis values */
        input.analog[0][0] = (x * 250) / VIDEO_WIDTH;
        input.analog[0][1] = (y * 250) / VIDEO_HEIGHT;
 
        /* Map mouse buttons to player #1 inputs */
        if(state & SDL_BUTTON_RMASK) input.pad[0] |= INPUT_B;
 
        break;
    }
 
    case DEVICE_GRAPHIC_BOARD:
    {
        /* get mouse (absolute values) */
        int x,y;
        int state = SDL_GetMouseState(&x,&y);
 
        /* Calculate X Y axis values */
        input.analog[0][0] = (x * 255) / VIDEO_WIDTH;
        input.analog[0][1] = (y * 255) / VIDEO_HEIGHT;
 
        /* Map mouse buttons to player #1 inputs */
        if(state & SDL_BUTTON_LMASK) input.pad[0] |= INPUT_GRAPHIC_PEN;
        if(state & SDL_BUTTON_RMASK) input.pad[0] |= INPUT_GRAPHIC_MENU;
        if(state & SDL_BUTTON_MMASK) input.pad[0] |= INPUT_GRAPHIC_DO;
 
        break;
    }
 
    case DEVICE_ACTIVATOR:
    {
        if(keystate[SDLK_g])  input.pad[joynum] |= INPUT_ACTIVATOR_7L;
        if(keystate[SDLK_h])  input.pad[joynum] |= INPUT_ACTIVATOR_7U;
        if(keystate[SDLK_j])  input.pad[joynum] |= INPUT_ACTIVATOR_8L;
        if(keystate[SDLK_k])  input.pad[joynum] |= INPUT_ACTIVATOR_8U;
    }
#endif
    default:
    {
#ifdef GCWZERO
        if(keystate[config.buttons[A]])     input.pad[joynum] |= INPUT_A;
        if(keystate[config.buttons[B]])     input.pad[joynum] |= INPUT_B;
        if(keystate[config.buttons[C]])     input.pad[joynum] |= INPUT_C;
        if(keystate[config.buttons[START]]) input.pad[joynum] |= INPUT_START;
        if (show_lightgun == 1)
        {
            if(keystate[config.buttons[X]]) input.pad[4]    |= INPUT_A; //player 2
            if(keystate[config.buttons[Y]]) input.pad[4]    |= INPUT_B; //player 2
            if(keystate[config.buttons[Z]]) input.pad[4]    |= INPUT_C; //player 2
        } else
        if (show_lightgun == 2)
        {
            if(keystate[config.buttons[X]]) input.pad[4]    |= INPUT_A; //player 2
            if(keystate[config.buttons[Y]]) input.pad[4]    |= INPUT_B; //player 2
            if(keystate[config.buttons[Z]]) input.pad[4]    |= INPUT_C; //player 2
        } else
        {
            if(keystate[config.buttons[X]]) input.pad[joynum] |= INPUT_X;
            if(keystate[config.buttons[Y]]) input.pad[joynum] |= INPUT_Y;
            if(keystate[config.buttons[Z]]) input.pad[joynum] |= INPUT_Z;
        }
        if(keystate[config.buttons[MODE]])  input.pad[joynum] |= INPUT_MODE;
        if (keystate[SDLK_ESCAPE] && keystate[SDLK_RETURN])
        {
            gotomenu=1;
        }
        if (keystate[SDLK_ESCAPE] && keystate[SDLK_TAB])
        {
          //save to quicksave slot
            char save_state_file[256];
            sprintf(save_state_file,"%s/%s.gp1", get_save_directory(), rom_filename);
                FILE *f = fopen(save_state_file,"wb");
                if (f)
                {
                    uint8 buf[STATE_SIZE];
                    int len = state_save(buf);
                    fwrite(&buf, len, 1, f);
                    fclose(f);
                }
          //Save BMP screenshot
            char save_state_screenshot[256];
            sprintf(save_state_screenshot,"%s/%s.1.bmp", get_save_directory(), rom_filename);
            SDL_Surface* screenshot;
            if (!config.gcw0_fullscreen)
            {
                screenshot = SDL_CreateRGBSurface(SDL_HWSURFACE, sdl_video.srect.w, sdl_video.srect.h, 16, 0, 0, 0, 0);
                SDL_Rect temp;
                temp.x = 0;
                temp.y = 0;
                temp.w = sdl_video.srect.w;
                temp.h = sdl_video.srect.h;

                SDL_BlitSurface(sdl_video.surf_bitmap, &temp, screenshot, &temp);
                SDL_SaveBMP(screenshot, save_state_screenshot);
                SDL_FreeSurface(screenshot);
            }
            else
            {
                screenshot = SDL_CreateRGBSurface(SDL_HWSURFACE, gcw0_w, gcw0_h, 16, 0, 0, 0, 0);
                SDL_Rect temp;
                temp.x = 0;
                temp.y = 0;
                temp.w = gcw0_w;
                temp.h = gcw0_h;

                SDL_BlitSurface(sdl_video.surf_bitmap, &temp, screenshot, &temp);
                SDL_SaveBMP(screenshot, save_state_screenshot);
                SDL_FreeSurface(screenshot);
            }

            SDL_Delay(250);
        }
        if (keystate[SDLK_ESCAPE] && keystate[SDLK_BACKSPACE])
        {
          //load quicksave slot
            char save_state_file[256];
            sprintf(save_state_file,"%s/%s.gp1", get_save_directory(), rom_filename );
            FILE *f = fopen(save_state_file,"rb");
            if (f)
            {
                uint8 buf[STATE_SIZE];
                fread(&buf, STATE_SIZE, 1, f);
                state_load(buf);
                fclose(f);
            }
            SDL_Delay(250);

        }
#else
        if(keystate[SDLK_a])  input.pad[joynum] |= INPUT_A;
        if(keystate[SDLK_s])  input.pad[joynum] |= INPUT_B;
        if(keystate[SDLK_d])  input.pad[joynum] |= INPUT_C;
        if(keystate[SDLK_f])  input.pad[joynum] |= INPUT_START;
        if(keystate[SDLK_z])  input.pad[joynum] |= INPUT_X;
        if(keystate[SDLK_x])  input.pad[joynum] |= INPUT_Y;
        if(keystate[SDLK_c])  input.pad[joynum] |= INPUT_Z;
        if(keystate[SDLK_v])  input.pad[joynum] |= INPUT_MODE;
#endif

#ifdef GCWZERO //A-stick support
        static int MoveLeft  = 0;
        static int MoveRight = 0;
        static int MoveUp    = 0;
        static int MoveDown  = 0;
        Sint16 x_move = 0;
        Sint16 y_move = 0;
        static int lg_left   = 0;
        static int lg_right  = 0;
        static int lg_up     = 0;
        static int lg_down   = 0;
        SDL_Joystick* joy;
        if(SDL_NumJoysticks() > 0)
        {
            joy    = SDL_JoystickOpen(0);
            x_move = SDL_JoystickGetAxis(joy, 0);
            y_move = SDL_JoystickGetAxis(joy, 1);
        }

//      Control lightgun with A-stick if activated
        if (show_lightgun)
        {
            lg_left   = 0;
            lg_right  = 0;
            lg_up     = 0;
            lg_down   = 0;

            if (x_move < -1000 || x_move > 1000)
            {
                if (x_move < -1000 ) lg_left  = 1;
                if (x_move < -20000) lg_left  = 3;
                if (x_move >  1000 ) lg_right = 1;
                if (x_move >  20000) lg_right = 3;
                current_time = time(NULL); //cursor disappears after 3 seconds...
            }
            if (y_move < -1000 || y_move > 1000)
            {
                if (y_move < -1000 ) lg_up   = 1;
                if (y_move < -20000) lg_up   = 3;
                if (y_move >  1000 ) lg_down = 1;
                if (y_move >  20000) lg_down = 3;
                current_time = time(NULL);
            }
//      Keep mouse within screen, wrap around!
        int x,y;
        int state = SDL_GetMouseState(&x,&y);
        if (!config.gcw0_fullscreen)
        {
            if ((x - lg_left ) < sdl_video.drect.x ) x = VIDEO_WIDTH  - sdl_video.drect.x;
            if ((y - lg_up   ) < sdl_video.drect.y ) y = VIDEO_HEIGHT - sdl_video.drect.y;
//            if ((x + lg_right) > 288) x = 288;
//            if ((y + lg_down ) > 216) y = 216;
//            if ((x + lg_right) > 320) x = 320;
//            if ((y + lg_down ) > 240) y = 240;
            if ((x + lg_right) > VIDEO_WIDTH  - sdl_video.drect.x) x = sdl_video.drect.x;
            if ((y + lg_down ) > VIDEO_HEIGHT - sdl_video.drect.y) y = sdl_video.drect.y;

//        sdl_video.drect.x = (VIDEO_WIDTH  - sdl_video.drect.w) / 2;
  //      sdl_video.drect.y = (VIDEO_HEIGHT - sdl_video.drect.h) / 2;


        } else //scaling on
        {
            if ((x - lg_left) < 0) x = gcw0_w;
            if ((y - lg_up  ) < 0) y = gcw0_h;
            if ((x + lg_right) > gcw0_w) x = 0;
            if ((y + lg_down ) > gcw0_h) y = 0;
        }
        SDL_WarpMouse( ( x+ ( ( lg_right - lg_left ) * config.lightgun_speed ) ) ,
                       ( y+ ( ( lg_down  - lg_up    ) * config.lightgun_speed ) ) );

        } else
//      otherwise it's just mirroring the D-pad controls
        if (config.a_stick)
        {
            MoveLeft  = 0;
            MoveRight = 0;
            MoveUp    = 0;
            MoveDown  = 0;
            if (x_move < -1000 || x_move > 1000)
            {
                if (x_move < -1000) MoveLeft  = 1;
                if (x_move >  1000) MoveRight = 1;
            }
            if (y_move < -1000 || y_move > 1000)
            {
                if (y_move < -1000) MoveUp   = 1;
                if (y_move >  1000) MoveDown = 1;
            }
        }
if(show_lightgun == 1) //Genesis/MD D-pad controls player 2
{
        if(MoveUp              == 1)  input.pad[4] |= INPUT_UP;
        if(MoveDown            == 1)  input.pad[4] |= INPUT_DOWN;
        if(MoveLeft            == 1)  input.pad[4] |= INPUT_LEFT;
        if(MoveRight           == 1)  input.pad[4] |= INPUT_RIGHT;
        if(keystate[SDLK_UP]   == 1)  input.pad[joynum]      |= INPUT_UP;
        if(keystate[SDLK_DOWN] == 1)  input.pad[joynum]      |= INPUT_DOWN;
        if(keystate[SDLK_LEFT] == 1)  input.pad[joynum]      |= INPUT_LEFT;
        if(keystate[SDLK_RIGHT]== 1)  input.pad[joynum]      |= INPUT_RIGHT;
} else
if(show_lightgun == 2) //SMS D-pad controls player 2
{
        if(MoveUp              == 1)  input.pad[joynum] |= INPUT_UP;
        if(MoveDown            == 1)  input.pad[joynum] |= INPUT_DOWN;
        if(MoveLeft            == 1)  input.pad[joynum] |= INPUT_LEFT;
        if(MoveRight           == 1)  input.pad[joynum] |= INPUT_RIGHT;
        if(keystate[SDLK_UP]   == 1)  input.pad[4]      |= INPUT_UP;
        if(keystate[SDLK_DOWN] == 1)  input.pad[4]      |= INPUT_DOWN;
        if(keystate[SDLK_LEFT] == 1)  input.pad[4]      |= INPUT_LEFT;
        if(keystate[SDLK_RIGHT]== 1)  input.pad[4]      |= INPUT_RIGHT;
} else
{
        if     (keystate[SDLK_UP]    || MoveUp    == 1)  input.pad[joynum] |= INPUT_UP;
        else if(keystate[SDLK_DOWN]  || MoveDown  == 1)  input.pad[joynum] |= INPUT_DOWN;
        if     (keystate[SDLK_LEFT]  || MoveLeft  == 1)  input.pad[joynum] |= INPUT_LEFT;
        else if(keystate[SDLK_RIGHT] || MoveRight == 1)  input.pad[joynum] |= INPUT_RIGHT;
}
#else
        if     (keystate[SDLK_UP]   )  input.pad[joynum] |= INPUT_UP;
        else if(keystate[SDLK_DOWN] )  input.pad[joynum] |= INPUT_DOWN;
        if     (keystate[SDLK_LEFT] )  input.pad[joynum] |= INPUT_LEFT;
        else if(keystate[SDLK_RIGHT])  input.pad[joynum] |= INPUT_RIGHT;
#endif 
    }
    }
    return 1;
}
 
int main (int argc, char **argv)
{
    FILE *fp;
    int running = 1;
    atexit(shutdown);
 
    /* Print help if no game specified */
    if(argc < 2)
    {
        char caption[256];
        sprintf(caption, "Genesis Plus GX\\SDL\nusage: %s gamename\n", argv[0]);
        MessageBox(NULL, caption, "Information", 0);
        exit(1);
    }
 
    error_init();
    create_default_directories();
    /* set default config */
    set_config_defaults();
    
    /* using rom file name instead of crc code to save files */
    sprintf(rom_filename, "%s",  get_file_name(argv[1]));
 
    /* mark all BIOS as unloaded */
    system_bios = 0;
 
    /* Genesis BOOT ROM support (2KB max) */
    memset(boot_rom, 0xFF, 0x800);
    fp = fopen(MD_BIOS, "rb");
    if (fp != NULL)
    {
        int i;
 
        /* read BOOT ROM */
        fread(boot_rom, 1, 0x800, fp);
        fclose(fp);
 
        /* check BOOT ROM */
        if (!memcmp((char *)(boot_rom + 0x120),"GENESIS OS", 10))
        {
            /* mark Genesis BIOS as loaded */
            system_bios = SYSTEM_MD;
        }
 
        /* Byteswap ROM */
        for (i=0; i<0x800; i+=2)
        {
            uint8 temp = boot_rom[i];
            boot_rom[i] = boot_rom[i+1];
            boot_rom[i+1] = temp;
        }
    }
 
    /* initialize SDL */
    if(SDL_Init(0) < 0)
    {
        char caption[256];
        sprintf(caption, "SDL initialization failed");
        MessageBox(NULL, caption, "Error", 0);
        exit(1);
    }
#ifdef GCWZERO
    sdl_joystick_init();
#endif
    sdl_video_init();
    if (use_sound) sdl_sound_init();
    sdl_sync_init();
 
    /* initialize Genesis virtual system */
    SDL_LockSurface(sdl_video.surf_bitmap);
    memset(&bitmap, 0, sizeof(t_bitmap));
    bitmap.width        = 720;
    bitmap.height       = 576;
#if defined(USE_8BPP_RENDERING)
    bitmap.pitch        = (bitmap.width * 1);
#elif defined(USE_15BPP_RENDERING)
    bitmap.pitch        = (bitmap.width * 2);
#elif defined(USE_16BPP_RENDERING)
    bitmap.pitch        = (bitmap.width * 2);
#elif defined(USE_32BPP_RENDERING)
    bitmap.pitch        = (bitmap.width * 4);
#endif
    bitmap.data         = sdl_video.surf_bitmap->pixels;
    SDL_UnlockSurface(sdl_video.surf_bitmap);
    bitmap.viewport.changed = 3;
 
    /* Load game file */
    if(!load_rom(argv[1]))
    {
        char caption[256];
        sprintf(caption, "Error loading file `%s'.", argv[1]);
        MessageBox(NULL, caption, "Error", 0);
        exit(1);
    }
 

    /* initialize system hardware */
//NOTE gcw0 second value determines framerate
    audio_init(SOUND_FREQUENCY, 0);
    system_init();
 
    /* Mega CD specific */
    char brm_file[256];
    if (system_hw == SYSTEM_MCD)
    {
        /* load internal backup RAM */
		sprintf(brm_file,"%s/", get_save_directory(), "scd.brm");
		fp = fopen(brm_file, "rb");
        if (fp!=NULL)
        {
            fread(scd.bram, 0x2000, 1, fp);
            fclose(fp);
        }
 
        /* check if internal backup RAM is formatted */
        if (memcmp(scd.bram + 0x2000 - 0x20, brm_format + 0x20, 0x20))
        {
            /* clear internal backup RAM */
            memset(scd.bram, 0x00, 0x200);
 
            /* Internal Backup RAM size fields */
            brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] = 0x00;
            brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] = (sizeof(scd.bram) / 64) - 3;
 
            /* format internal backup RAM */
            memcpy(scd.bram + 0x2000 - 0x40, brm_format, 0x40);
        }
 
        /* load cartridge backup RAM */
        if (scd.cartridge.id)
        {
			sprintf(brm_file,"%s/", get_save_directory(), "cart.brm");
			fp = fopen(brm_file, "rb");
            if (fp!=NULL)
            {
                fread(scd.cartridge.area, scd.cartridge.mask + 1, 1, fp);
                fclose(fp);
            }
 
            /* check if cartridge backup RAM is formatted */
            if (memcmp(scd.cartridge.area + scd.cartridge.mask + 1 - 0x20, brm_format + 0x20, 0x20))
            {
                /* clear cartridge backup RAM */
                memset(scd.cartridge.area, 0x00, scd.cartridge.mask + 1);
 
                /* Cartridge Backup RAM size fields */
                brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] = (((scd.cartridge.mask + 1) / 64) - 3) >> 8;
                brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] = (((scd.cartridge.mask + 1) / 64) - 3) & 0xff;
 
                /* format cartridge backup RAM */
                memcpy(scd.cartridge.area + scd.cartridge.mask + 1 - sizeof(brm_format), brm_format, sizeof(brm_format));
            }
        }
    }
 
    if (sram.on)
    {
        /* load SRAM */
        char save_file[256];
        sprintf(save_file,"%s/%s.srm", get_save_directory(), rom_filename);
        fp = fopen(save_file, "rb");
        if (fp!=NULL)
        {
            fread(sram.sram,0x10000,1, fp);
            fclose(fp);
        }
    }
 
    /* reset system hardware */
    system_reset();
 
    if(use_sound) SDL_PauseAudio(0);
 
    /* 3 frames = 50 ms (60hz) or 60 ms (50hz) */
    if(sdl_sync.sem_sync)
        SDL_SetTimer(vdp_pal ? 60 : 50, sdl_sync_timer_callback);
 
    /* emulation loop */
    while(running)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_USEREVENT:
                {
                    char caption[100];
                    sprintf(caption,"Genesis Plus GX - %d fps - %s)", event.user.code, (rominfo.international[0] != 0x20) ? rominfo.international : rominfo.domestic);
                    SDL_WM_SetCaption(caption, NULL);
                    break;
                }
 
                case SDL_QUIT:
                {
                    running = 0;
                    break;
                }
 
                case SDL_KEYDOWN:
                {
                    running = sdl_control_update(event.key.keysym.sym);
                    break;
                }
            }
        }
#ifdef GCWZERO
        if (do_once) 
        {
            do_once--; //don't waste write cycles!
            if (config.keepaspectratio)
            {
                FILE* aspect_ratio_file = fopen("/sys/devices/platform/jz-lcd.0/keep_aspect_ratio", "w");
                if (aspect_ratio_file)
                { 
                    fwrite("Y", 1, 1, aspect_ratio_file);
                    fclose(aspect_ratio_file);
                }
            }
            if (!config.keepaspectratio)
    	    {
                FILE* aspect_ratio_file = fopen("/sys/devices/platform/jz-lcd.0/keep_aspect_ratio", "w");
                if (aspect_ratio_file)
                { 
                    fwrite("N", 1, 1, aspect_ratio_file);
                    fclose(aspect_ratio_file);
                }
            }
	}

#endif
        sdl_video_update();
        sdl_sound_update(use_sound);

        if(!turbo_mode && sdl_sync.sem_sync && sdl_video.frames_rendered % 3 == 0)
        {
            if (!config.gcw0_frameskip || (config.gcw0_frameskip && (system_hw != SYSTEM_MCD))) //we really only need this for fmv sequences
                if(!gotomenu)
                    SDL_SemWait(sdl_sync.sem_sync);
#ifdef GCWZERO
        }
        else
        {
            if (gotomenu)
	        gcw0menu();
#endif
        }
    }

    return 0;

}

