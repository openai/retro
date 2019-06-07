//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: Settings.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <cassert>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "bspf.hxx"

//#include "DebuggerDialog.hxx"
#include "OSystem.hxx"
#include "Version.hxx"

#include "Settings.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::Settings(OSystem* osystem)
  : myOSystem(osystem)
{
  // Add this settings object to the OSystem
  myOSystem->attach(this);

  // Add options that are common to all versions of Stella
  setInternal("video", "soft");

  // OpenGL specific options
  setInternal("gl_inter", "false");
  setInternal("gl_aspectn", "90");
  setInternal("gl_aspectp", "100");
  setInternal("gl_fsscale", "false");
  setInternal("gl_lib", "libGL.so");
  setInternal("gl_vsync", "true");
  setInternal("gl_vbo", "true");

  // Framebuffer-related options
  setInternal("tia_filter", "zoom2x");
  setInternal("fullscreen", "0");
  setInternal("fullres", "auto");
  setInternal("center", "false");
  setInternal("grabmouse", "true");
  setInternal("palette", "standard");
  setInternal("colorloss", "true");
  setInternal("timing", "sleep");
  setInternal("uimessages", "true");

  // TV filtering options
  setInternal("tv_filter", "0");
  setInternal("tv_scanlines", "25");
  setInternal("tv_scaninter", "true");
  // TV options when using 'custom' mode
  setInternal("tv_contrast", "0.0");
  setInternal("tv_brightness", "0.0");
  setInternal("tv_hue", "0.0");
  setInternal("tv_saturation", "0.0");
  setInternal("tv_gamma", "0.0");
  setInternal("tv_sharpness", "0.0");
  setInternal("tv_resolution", "0.0");
  setInternal("tv_artifacts", "0.0");
  setInternal("tv_fringing", "0.0");
  setInternal("tv_bleed", "0.0");

  // Sound options
  setInternal("sound", "true");
  setInternal("fragsize", "512");
  setInternal("freq", "31400");
  setInternal("volume", "100");

  // Input event options
  setInternal("keymap", "");
  setInternal("joymap", "");
  setInternal("combomap", "");
  setInternal("joydeadzone", "13");
  setInternal("joyallow4", "false");
  setInternal("usemouse", "analog");
  setInternal("dsense", "5");
  setInternal("msense", "7");
  setInternal("saport", "lr");
  setInternal("ctrlcombo", "true");

  // Snapshot options
  setInternal("snapsavedir", "");
  setInternal("snaploaddir", "");
  setInternal("snapname", "int");
  setInternal("sssingle", "false");
  setInternal("ss1x", "false");
  setInternal("ssinterval", "2");

  // Config files and paths
  setInternal("romdir", "");
  setInternal("statedir", "");
  setInternal("cheatfile", "");
  setInternal("palettefile", "");
  setInternal("propsfile", "");
  setInternal("nvramdir", "");
  setInternal("cfgdir", "");

  // ROM browser options
  setInternal("exitlauncher", "false");
  setInternal("launcherres", GUI::Size(640, 480));
  setInternal("launcherfont", "medium");
  setInternal("launcherexts", "allroms");
  setInternal("romviewer", "0");
  setInternal("lastrom", "");

  // UI-related options
//  setInternal("dbg.res",
//    GUI::Size(DebuggerDialog::kMediumFontMinW, DebuggerDialog::kMediumFontMinH));
  setInternal("uipalette", "0");
  setInternal("listdelay", "300");
  setInternal("mwheel", "4");

  // Misc options
  setInternal("autoslot", "false");
  setInternal("loglevel", "1");
  setInternal("logtoconsole", "0");
  setInternal("tiadriven", "false");
  setInternal("cpurandom", "true");
  setInternal("ramrandom", "true");
  setInternal("avoxport", "");
  setInternal("stats", "false");
  setInternal("fastscbios", "false");
  setExternal("romloadcount", "0");
  setExternal("maxres", "");

  // Debugger/disassembly options
  setInternal("dbg.fontstyle", "0");
  setInternal("dbg.uhex", "true");
  setInternal("dis.resolve", "true");
  setInternal("dis.gfxformat", "2");
  setInternal("dis.showaddr", "true");
  setInternal("dis.relocate", "false");

  // Thumb ARM emulation options
  setInternal("thumb.trapfatal", "true");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::~Settings()
{
  myInternalSettings.clear();
  myExternalSettings.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Settings::loadCommandLine(int argc, char** argv)
{
  for(int i = 1; i < argc; ++i)
  {
    // strip off the '-' character
    string key = argv[i];
    if(key[0] == '-')
    {
      key = key.substr(1, key.length());

      // Take care of the arguments which are meant to be executed immediately
      // (and then Stella should exit)
      if(key == "help" || key == "listrominfo")
      {
        setExternal(key, "true");
        return "";
      }

      // Take care of arguments without an option or ones that shouldn't
      // be saved to the config file
      if(key == "rominfo" || key == "debug" || key == "holdreset" ||
         key == "holdselect" || key == "takesnapshot")
      {
        setExternal(key, "true");
        continue;
      }

      ostringstream buf;
      if(++i >= argc)
      {
        buf << "Missing argument for '" << key << "'" << endl;
        return "";
      }
      string value = argv[i];

      buf.str("");
      buf << "  key = '" << key << "', value = '" << value << "'";

      // Settings read from the commandline must not be saved to 
      // the rc-file, unless they were previously set
      if(int idx = getInternalPos(key) != -1)
      {
        setInternal(key, value, idx);   // don't set initialValue here
        buf << "(I)\n";
      }
      else
      {
        setExternal(key, value);
        buf << "(E)\n";
      }
    }
    else
      return key;
  }

  return "";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::validate()
{
  string s;
  int i;

  s = getString("video");
  if(s != "soft" && s != "gl")  setInternal("video", "soft");

  s = getString("timing");
  if(s != "sleep" && s != "busy")  setInternal("timing", "sleep");

#ifdef SOUND_SUPPORT
  i = getInt("volume");
  if(i < 0 || i > 100)    setInternal("volume", "100");
  i = getInt("freq");
  if(!(i == 11025 || i == 22050 || i == 31400 || i == 44100 || i == 48000))
    setInternal("freq", "31400");
#endif

  i = getInt("joydeadzone");
  if(i < 0)        setInternal("joydeadzone", "0");
  else if(i > 29)  setInternal("joydeadzone", "29");

  if(i < 1)        setInternal("dsense", "1");
  else if(i > 10)  setInternal("dsense", "10");

  i = getInt("dsense");
  if(i < 1)        setInternal("dsense", "1");
  else if(i > 10)  setInternal("dsense", "10");

  i = getInt("msense");
  if(i < 1)        setInternal("msense", "1");
  else if(i > 15)  setInternal("msense", "15");

  i = getInt("ssinterval");
  if(i < 1)        setInternal("ssinterval", "2");
  else if(i > 10)  setInternal("ssinterval", "10");

  s = getString("palette");
  if(s != "standard" && s != "z26" && s != "user")
    setInternal("palette", "standard");

  s = getString("launcherfont");
  if(s != "small" && s != "medium" && s != "large")
    setInternal("launcherfont", "medium");

  i = getInt("romviewer");
  if(i < 0)       setInternal("romviewer", "0");
  else if(i > 2)  setInternal("romviewer", "2");

  i = getInt("loglevel");
  if(i < 0 || i > 2)
    setInternal("loglevel", "1");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void Settings::usage()
//{
//  cout << endl
//    << "Stella version " << STELLA_VERSION << endl
//    << endl
//    << "Usage: stella [options ...] romfile" << endl
//    << "       Run without any options or romfile to use the ROM launcher" << endl
//    << "       Consult the manual for more in-depth information" << endl
//    << endl
//    << "Valid options are:" << endl
//    << endl
//    << "  -video        <type>         Type is one of the following:\n"
//    << "                 soft            SDL software mode\n"
//    << "  -tia_filter   <filter>       Use the specified filter in emulation mode\n"
//    << "  -fullscreen   <1|0|-1>       Use fullscreen mode (1 or 0), or disable switching to fullscreen entirely\n"
//    << "  -fullres      <auto|WxH>     The resolution to use in fullscreen mode\n"
//    << "  -center       <1|0>          Centers game window (if possible)\n"
//    << "  -grabmouse    <1|0>          Keeps the mouse in the game window\n"
//    << "  -palette      <standard|     Use the specified color palette\n"
//    << "                 z26|\n"
//    << "                 user>\n"
//    << "  -colorloss    <1|0>          Enable PAL color-loss effect\n"
//    << "  -framerate    <number>       Display the given number of frames per second (0 to auto-calculate)\n"
//    << "  -timing       <sleep|busy>   Use the given type of wait between frames\n"
//    << "  -uimessages   <1|0>          Show onscreen UI messages for different events\n"
//    << endl
//  #ifdef SOUND_SUPPORT
//    << "  -sound        <1|0>          Enable sound generation\n"
//    << "  -fragsize     <number>       The size of sound fragments (must be a power of two)\n"
//    << "  -freq         <number>       Set sound sample output frequency (11025|22050|31400|44100|48000)\n"
//    << "  -volume       <number>       Set the volume (0 - 100)\n"
//    << endl
//  #endif
//    << "  -cheat        <code>         Use the specified cheatcode (see manual for description)\n"
//    << "  -loglevel     <0|1|2>        Set level of logging during application run\n"
//    << "  -logtoconsole <1|0>          Log output to console/commandline\n"
//    << "  -joydeadzone  <number>       Sets 'deadzone' area for analog joysticks (0-29)\n"
//    << "  -joyallow4    <1|0>          Allow all 4 directions on a joystick to be pressed simultaneously\n"
//    << "  -usemouse     <always|\n"
//    << "                 analog|\n"
//    << "                 never>        Use mouse as a controller as specified by ROM properties in given mode(see manual)\n"
//    << "  -dsense       <number>       Sensitivity of digital emulated paddle movement (1-10)\n"
//    << "  -msense       <number>       Sensitivity of mouse emulated paddle movement (1-15)\n"
//    << "  -saport       <lr|rl>        How to assign virtual ports to multiple Stelladaptor/2600-daptors\n"
//    << "  -ctrlcombo    <1|0>          Use key combos involving the Control key (Control-Q for quit may be disabled!)\n"
//    << "  -autoslot     <1|0>          Automatically switch to next save slot when state saving\n"
//    << "  -stats        <1|0>          Overlay console info during emulation\n"
//    << "  -fastscbios   <1|0>          Disable Supercharger BIOS progress loading bars\n"
//    << "  -snapsavedir  <path>         The directory to save snapshot files to\n"
//    << "  -snaploaddir  <path>         The directory to load snapshot files from\n"
//    << "  -snapname     <int|rom>      Name snapshots according to internal database or ROM\n"
//    << "  -sssingle     <1|0>          Generate single snapshot instead of many\n"
//    << "  -ss1x         <1|0>          Generate TIA snapshot in 1x mode (ignore scaling/effects)\n"
//    << "  -ssinterval   <number        Number of seconds between snapshots in continuous snapshot mode\n"
//    << endl
//    << "  -rominfo      <rom>          Display detailed information for the given ROM\n"
//    << "  -listrominfo                 Display contents of stella.pro, one line per ROM entry\n"
//    << "  -exitlauncher <1|0>          On exiting a ROM, go back to the ROM launcher\n"
//    << "  -launcherres  <WxH>          The resolution to use in ROM launcher mode\n"
//    << "  -launcherfont <small|medium| Use the specified font in the ROM launcher\n"
//    << "                 large>\n"
//    << "  -launcherexts <allfiles|     Show files with the given extensions in ROM launcher\n"
//    << "                 allroms|        (exts is a ':' separated list of extensions)\n"
//    << "                 exts\n"
//    << "  -romviewer    <0|1|2>        Show ROM info viewer at given zoom level in ROM launcher (0 for off)\n"
//    << "  -uipalette    <1|2>          Used the specified palette for UI elements\n"
//    << "  -listdelay    <delay>        Time to wait between keypresses in list widgets (300-1000)\n"
//    << "  -mwheel       <lines>        Number of lines the mouse wheel will scroll in UI\n"
//    << "  -statedir     <dir>          Directory in which to save/load state files\n"
//    << "  -cheatfile    <file>         Full pathname of cheatfile database\n"
//    << "  -palettefile  <file>         Full pathname of user-defined palette file\n"
//    << "  -propsfile    <file>         Full pathname of ROM properties file\n"
//    << "  -nvramdir     <dir>          Directory in which to save/load flash/EEPROM files\n"
//    << "  -cfgdir       <dir>          Directory in which to save Distella config files\n"
//    << "  -avoxport     <name>         The name of the serial port where an AtariVox is connected\n"
//    << "  -maxres       <WxH>          Used by developers to force the maximum size of the application window\n"
//    << "  -holdreset                   Start the emulator with the Game Reset switch held down\n"
//    << "  -holdselect                  Start the emulator with the Game Select switch held down\n"
//    << "  -holdjoy0     <U,D,L,R,F>    Start the emulator with the left joystick direction/fire button held down\n"
//    << "  -holdjoy1     <U,D,L,R,F>    Start the emulator with the right joystick direction/fire button held down\n"
//    << "  -tiadriven    <1|0>          Drive unused TIA pins randomly on a read/peek\n"
//    << "  -cpurandom    <1|0>          Randomize the contents of CPU registers on reset\n"
//    << "  -ramrandom    <1|0>          Randomize the contents of RAM on reset\n"
//    << "  -help                        Show the text you're now reading\n"
//  #ifdef DEBUGGER_SUPPORT
//    << endl
//    << " The following options are meant for developers\n"
//    << " Arguments are more fully explained in the manual\n"
//    << endl
//    << "   -dis.resolve   <1|0>        Attempt to resolve code sections in disassembler\n"
//    << "   -dis.gfxformat <2|16>       Set base to use for displaying GFX sections in disassembler\n"
//    << "   -dis.showaddr  <1|0>        Show opcode addresses in disassembler\n"
//    << "   -dis.relocate  <1|0>        Relocate calls out of address range in disassembler\n"
//    << endl
//    << "   -dbg.res       <WxH>        The resolution to use in debugger mode\n"
//    << "   -dbg.fontstyle <0-3>        Font style to use in debugger window (bold vs. normal)\n"
//    << "   -break         <address>    Set a breakpoint at 'address'\n"
//    << "   -debug                      Start in debugger mode\n"
//    << endl
//    << "   -bs          <arg>          Sets the 'Cartridge.Type' (bankswitch) property\n"
//    << "   -type        <arg>          Same as using -bs\n"
//    << "   -channels    <arg>          Sets the 'Cartridge.Sound' property\n"
//    << "   -ld          <arg>          Sets the 'Console.LeftDifficulty' property\n"
//    << "   -rd          <arg>          Sets the 'Console.RightDifficulty' property\n"
//    << "   -tv          <arg>          Sets the 'Console.TelevisionType' property\n"
//    << "   -sp          <arg>          Sets the 'Console.SwapPorts' property\n"
//    << "   -lc          <arg>          Sets the 'Controller.Left' property\n"
//    << "   -rc          <arg>          Sets the 'Controller.Right' property\n"
//    << "   -bc          <arg>          Same as using both -lc and -rc\n"
//    << "   -cp          <arg>          Sets the 'Controller.SwapPaddles' property\n"
//    << "   -format      <arg>          Sets the 'Display.Format' property\n"
//    << "   -ystart      <arg>          Sets the 'Display.YStart' property\n"
//    << "   -height      <arg>          Sets the 'Display.Height' property\n"
//    << "   -pp          <arg>          Sets the 'Display.Phosphor' property\n"
//    << "   -ppblend     <arg>          Sets the 'Display.PPBlend' property\n"
//  #endif
//    << endl << flush;
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const Variant& Settings::value(const string& key) const
{
  // Try to find the named setting and answer its value
  int idx = -1;
  if((idx = getInternalPos(key)) != -1)
    return myInternalSettings[idx].value;
  else if((idx = getExternalPos(key)) != -1)
    return myExternalSettings[idx].value;
  else
    return EmptyVariant;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Settings::setValue(const string& key, const Variant& value)
{
  if(int idx = getInternalPos(key) != -1)
    setInternal(key, value, idx);
  else
    setExternal(key, value);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::getInternalPos(const string& key) const
{
  for(unsigned int i = 0; i < myInternalSettings.size(); ++i)
    if(myInternalSettings[i].key == key)
      return i;

  return -1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::getExternalPos(const string& key) const
{
  for(unsigned int i = 0; i < myExternalSettings.size(); ++i)
    if(myExternalSettings[i].key == key)
      return i;

  return -1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::setInternal(const string& key, const Variant& value,
                          int pos, bool useAsInitial)
{
  int idx = -1;

  if(pos != -1 && pos >= 0 && pos < (int)myInternalSettings.size() &&
     myInternalSettings[pos].key == key)
  {
    idx = pos;
  }
  else
  {
    for(unsigned int i = 0; i < myInternalSettings.size(); ++i)
    {
      if(myInternalSettings[i].key == key)
      {
        idx = i;
        break;
      }
    }
  }

  if(idx != -1)
  {
    myInternalSettings[idx].key   = key;
    myInternalSettings[idx].value = value;
    if(useAsInitial) myInternalSettings[idx].initialValue = value;

    /*cerr << "modify internal: key = " << key
         << ", value  = " << value
         << ", ivalue = " << myInternalSettings[idx].initialValue
         << " @ index = " << idx
         << endl;*/
  }
  else
  {
    Setting setting;
    setting.key   = key;
    setting.value = value;
    if(useAsInitial) setting.initialValue = value;

    myInternalSettings.push_back(setting);
    idx = myInternalSettings.size() - 1;

    /*cerr << "insert internal: key = " << key
         << ", value  = " << value
         << ", ivalue = " << setting.initialValue
         << " @ index = " << idx
         << endl;*/
  }

  return idx;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Settings::setExternal(const string& key, const Variant& value,
                          int pos, bool useAsInitial)
{
  int idx = -1;

  if(pos != -1 && pos >= 0 && pos < (int)myExternalSettings.size() &&
     myExternalSettings[pos].key == key)
  {
    idx = pos;
  }
  else
  {
    for(unsigned int i = 0; i < myExternalSettings.size(); ++i)
    {
      if(myExternalSettings[i].key == key)
      {
        idx = i;
        break;
      }
    }
  }

  if(idx != -1)
  {
    myExternalSettings[idx].key   = key;
    myExternalSettings[idx].value = value;
    if(useAsInitial) myExternalSettings[idx].initialValue = value;

    /*cerr << "modify external: key = " << key
         << ", value = " << value
         << " @ index = " << idx
         << endl;*/
  }
  else
  {
    Setting setting;
    setting.key   = key;
    setting.value = value;
    if(useAsInitial) setting.initialValue = value;

    myExternalSettings.push_back(setting);
    idx = myExternalSettings.size() - 1;

    /*cerr << "insert external: key = " << key
         << ", value = " << value
         << " @ index = " << idx
         << endl;*/
  }

  return idx;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings::Settings(const Settings&)
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Settings& Settings::operator = (const Settings&)
{
  assert(false);

  return *this;
}
