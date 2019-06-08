#ifndef _MSC_VER
#include <stdbool.h>
#include <sched.h>
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include "libretro.h"

#include "Console.hxx"
#include "Cart.hxx"
#include "Props.hxx"
#include "MD5.hxx"
#include "Sound.hxx"
#include "SerialPort.hxx"
#include "TIA.hxx"
#include "Switches.hxx"
#include "StateManager.hxx"
#include "PropsSet.hxx"
#include "Paddles.hxx"
#include "SoundSDL.hxx"
#include "M6532.hxx"
#include "Version.hxx"

#include "Stubs.hxx"

static Console *console = 0;
static Cartridge *cartridge = 0;
static Settings *settings = 0;
static OSystem osystem;
static StateManager stateManager(&osystem);

static int videoWidth, videoHeight;

static retro_log_printf_t log_cb;
static retro_video_refresh_t video_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_environment_t environ_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static struct retro_system_av_info g_av_info;

static void update_input()
{

   if (!input_poll_cb)
      return;

   input_poll_cb();

   Event &ev = osystem.eventHandler().event();

   //Update stella's event structure
   ev.set(Event::Type(Event::JoystickZeroUp), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP));
   ev.set(Event::Type(Event::JoystickZeroDown), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN));
   ev.set(Event::Type(Event::JoystickZeroLeft), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT));
   ev.set(Event::Type(Event::JoystickZeroRight), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT));
   ev.set(Event::Type(Event::JoystickZeroFire), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B));
   ev.set(Event::Type(Event::ConsoleLeftDiffA), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L));
   ev.set(Event::Type(Event::ConsoleLeftDiffB), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2));
   ev.set(Event::Type(Event::ConsoleColor), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3));
   ev.set(Event::Type(Event::ConsoleRightDiffA), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R));
   ev.set(Event::Type(Event::ConsoleRightDiffB), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2));
   ev.set(Event::Type(Event::ConsoleBlackWhite), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3));
   ev.set(Event::Type(Event::ConsoleSelect), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT));
   ev.set(Event::Type(Event::ConsoleReset), input_state_cb(Controller::Left, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START));

   //Events for right player's joystick 
   ev.set(Event::Type(Event::JoystickOneUp), input_state_cb(Controller::Right, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP));
   ev.set(Event::Type(Event::JoystickOneDown), input_state_cb(Controller::Right, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN));
   ev.set(Event::Type(Event::JoystickOneLeft), input_state_cb(Controller::Right, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT));
   ev.set(Event::Type(Event::JoystickOneRight), input_state_cb(Controller::Right, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT));
   ev.set(Event::Type(Event::JoystickOneFire), input_state_cb(Controller::Right, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B));

   //Tell all input devices to read their state from the event structure
   console->controller(Controller::Left).update();
   console->controller(Controller::Right).update();
   console->switches().update();
}

/************************************
 * libretro implementation
 ************************************/


void retro_set_environment(retro_environment_t cb) { environ_cb = cb; }
void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name = "Stella";
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
   info->library_version = STELLA_VERSION GIT_VERSION;
   info->need_fullpath = false;
   info->valid_extensions = "a26|bin";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   memset(info, 0, sizeof(*info));
   info->timing.fps            = console->getFramerate();
   info->timing.sample_rate    = 31400;
   info->geometry.base_width   = 160 * 2;
   info->geometry.base_height  = videoHeight;
   info->geometry.max_width    = 320;
   info->geometry.max_height   = 256;
   info->geometry.aspect_ratio = 4.0f / 3.0f;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   (void)port;
   (void)device;
}

size_t retro_serialize_size(void) 
{
   Serializer state;
   if(!stateManager.saveState(state))
      return 0;
   return state.get().size();
}

bool retro_serialize(void *data, size_t size)
{
    Serializer state;
    if(!stateManager.saveState(state))
        return false;
    std::string s = state.get();
    memcpy(data, s.data(), s.size());
    return true;
}

bool retro_unserialize(const void *data, size_t size)
{
    std::string s((const char*)data, size);
    Serializer state;
    state.set(s);
   if(stateManager.loadState(state))
      return true;
   return false;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

bool retro_load_game(const struct retro_game_info *info)
{
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;

   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "Right" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "Fire" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,      "Left Difficulty A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,     "Left Difficulty B" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3,     "Color" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,      "Right Difficulty A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "Right Difficulty B" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3,     "Black/White" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Reset" },

      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "Fire" },

      { 0 },
   };

   if (!info || info->size >= 96*1024)
      return false;

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      if (log_cb)
         log_cb(RETRO_LOG_INFO, "[Stella]: XRGB8888 is not supported.\n");
      return false;
   }


   // Get the game properties
   string cartMD5 = MD5((const uInt8*)info->data, (uInt32)info->size);
   Properties props;
   osystem.propSet().getMD5(cartMD5, props);

   // Load the cart
   string cartType = props.get(Cartridge_Type);
   string cartId;//, romType("AUTO-DETECT");
   settings = new Settings(&osystem);
   settings->setValue("romloadcount", false);
   cartridge = Cartridge::create((const uInt8*)info->data, (uInt32)info->size, cartMD5, cartType, cartId, osystem, *settings);

   if(cartridge == 0)
   {
      if (log_cb)
         log_cb(RETRO_LOG_ERROR, "Stella: Failed to load cartridge.\n");
      return false;
   }

   // Create the console
   console = new Console(&osystem, cartridge, props);
   osystem.myConsole = console;

   // Init sound and video
   console->initializeVideo();
   console->initializeAudio();

   // Get the ROM's width and height
   TIA& tia = console->tia();
   videoWidth = tia.width();
   videoHeight = tia.height();

   return true;
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
   (void)game_type;
   (void)info;
   (void)num_info;
   return false;
}

void retro_unload_game(void) 
{
   if (console)
   {
      delete console;
      console = 0;
   }
   else if (cartridge)
   {
      delete cartridge;
      cartridge = 0;
   }
   if (settings)
   {
      delete settings;
      settings = 0;
   }
}

unsigned retro_get_region(void)
{
   //console->getFramerate();
   return RETRO_REGION_NTSC;
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void *retro_get_memory_data(unsigned id)
{
   switch (id)
   {
      case RETRO_MEMORY_SYSTEM_RAM: return console->system().m6532().getRAM();
      default: return NULL;
   }
}

size_t retro_get_memory_size(unsigned id)
{
   switch (id)
   {
      case RETRO_MEMORY_SYSTEM_RAM: return 128;
      default: return 0;
   }
}

void retro_init(void)
{
   struct retro_log_callback log;
   unsigned level = 4;

   if (environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log))
      log_cb = log.log;
   else
      log_cb = NULL;

   environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
}

void retro_deinit(void)
{
}

void retro_reset(void)
{
   console->system().reset();
}

void retro_run(void)
{
    static int16_t *sampleBuffer[2048];
    static uint32_t frameBuffer[256*160];
    //Get the number of samples in a frame
    static uint32_t tiaSamplesPerFrame = (uint32_t)(31400.0f/console->getFramerate());

   //INPUT
   update_input();

   //EMULATE
   TIA& tia = console->tia();
   tia.update();

   //VIDEO
   //Get the frame info from stella
   videoWidth = tia.width();
   videoHeight = tia.height();

   const uint32_t *palette = console->getPalette(0);
   //Copy the frame from stella to libretro
   for (int i = 0; i < videoHeight * videoWidth; ++i)
      frameBuffer[i] = palette[tia.currentFrameBuffer()[i]];

   video_cb(frameBuffer, videoWidth, videoHeight, videoWidth << 2);

   //AUDIO
   //Process one frame of audio from stella
   SoundSDL *sound = (SoundSDL*)&osystem.sound();
   sound->processFragment((int16_t*)sampleBuffer, tiaSamplesPerFrame);

   audio_batch_cb((int16_t*)sampleBuffer, tiaSamplesPerFrame);
}
