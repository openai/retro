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
// $Id: OSystem.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef OSYSTEM_HXX
#define OSYSTEM_HXX

class Cartridge;
class CheatManager;
class CommandMenu;
class Console;
class Debugger;
class Launcher;
class Menu;
class Properties;
class PropertiesSet;
class SerialPort;
class Settings;
class Sound;
class StateManager;
class VideoDialog;

namespace GUI {
  class Font;
}

#include "Array.hxx"
#include "FrameBuffer.hxx"
#include "bspf.hxx"

struct Resolution {
  uInt32 width;
  uInt32 height;
  string name;
};
typedef Common::Array<Resolution> ResolutionList;

/**
  This class provides an interface for accessing operating system specific
  functions.  It also comprises an overall parent object, to which all the
  other objects belong.

  @author  Stephen Anthony
  @version $Id: OSystem.hxx 2359 2012-01-17 22:20:20Z stephena $
*/
class OSystem
{
  friend class EventHandler;
  friend class VideoDialog;

  public:
    /**
      Create a new OSystem abstract class
    */
    OSystem();

    /**
      Destructor
    */
    virtual ~OSystem();

    /**
      Create all child objects which belong to this OSystem
    */
    virtual bool create();

  public:
    /**
      Adds the specified settings object to the system.

      @param settings The settings object to add 
    */
    void attach(Settings* settings) { mySettings = settings; }

    /**
      Get the event handler of the system

      @return The event handler
    */
    EventHandler& eventHandler() const { return *myEventHandler; }

    /**
      Get the frame buffer of the system

      @return The frame buffer
    */
    FrameBuffer& frameBuffer() const { return *myFrameBuffer; }

    /**
      Get the sound object of the system

      @return The sound object
    */
    Sound& sound() const { return *mySound; }

    /**
      Get the settings object of the system

      @return The settings object
    */
    Settings& settings() const { return *mySettings; }

    /**
      Get the set of game properties for the system

      @return The properties set object
    */
    PropertiesSet& propSet() const { return *myPropSet; }

    /**
      Get the console of the system.

      @return The console object
    */
    Console& console() const { return *myConsole; }

    /**
      Get the serial port of the system.

      @return The serial port object
    */
    SerialPort& serialPort() const { return *mySerialPort; }

#ifdef DEBUGGER_SUPPORT
    /**
      Create all child objects which belong to this OSystem
    */
    void createDebugger(Console& console);

    /**
      Get the ROM debugger of the system.

      @return The debugger object
    */
    Debugger& debugger() const { return *myDebugger; }
#endif

#ifdef CHEATCODE_SUPPORT
    /**
      Get the cheat manager of the system.

      @return The cheatmanager object
    */
    CheatManager& cheat() const { return *myCheatManager; }
#endif

    /**
      Set the framerate for the video system.  It's placed in this class since
      the mainLoop() method is defined here.

      @param framerate  The video framerate to use
    */
    virtual void setFramerate(float framerate);

    /**
      Set all config file paths for the OSystem.
    */
    void setConfigPaths();

    /**
      Set the user-interface palette which is specified in current settings.
    */
    void setUIPalette();

    uInt32 desktopHeight() const { return 512; }

    /**
      Return the full/complete directory name for storing state files.
    */
    const string& stateDir() const { static string dir("."); return dir; }

    /**
     Return the full/complete directory name for storing nvram
     (flash/EEPROM) files.
     */
    const string& nvramDir() const { return myNVRamDir; }

    /**
      This method should be called to get the full path of the
      (optional) palette file.

      @return String representing the full path of the properties filename.
    */
    const string& paletteFile() const { return myPaletteFile; }

    /**
      Creates a new game console from the specified romfile, and correctly
      initializes the system state to start emulation of the Console.

      @param romfile  The full pathname of the ROM to use
      @param md5      The MD5sum of the ROM

      @return  True on successful creation, otherwise false
    */
    bool createConsole(const string& romfile = "", const string& md5 = "");

    /**
      Deletes the currently defined console, if it exists.
      Also prints some statistics (fps, total frames, etc).
    */
    void deleteConsole();

    /**
      Creates a new ROM launcher, to select a new ROM to emulate.

      @param startdir  The directory to use when opening the launcher;
                       if blank, use 'romdir' setting.

      @return  True on successful creation, otherwise false
    */
    bool createLauncher(const string& startdir = "");

    /**
      Gets all possible info about the ROM by creating a temporary
      Console object and querying it.

      @param romfile  The full pathname of the ROM to use
      @return  Some information about this ROM
    */
    string getROMInfo(const string& romfile);

    /**
      Calculate the MD5sum of the given file.

      @param filename  Filename of potential ROM file
    */
    string MD5FromFile(const string& filename);

    /**
      Append a message to the internal log.

      @param message  The message to be appended
      @param level    If 0, always output the message, only append when
                      level is less than or equal to that in 'loglevel'
    */
	#ifdef NDEBUG
    void logMessage(const string& message, uInt8 level) { }
	#else
    void logMessage(const string& message, uInt8 level);
	#endif

    /**
      Get the system messages logged up to this point.

      @return The list of log messages
    */
    //const string& logMessages() const { return myLogMessages; }

  public:
    //////////////////////////////////////////////////////////////////////
    // The following methods are system-specific and can be overrided in
    // derived classes.  Otherwise, the base methods will be used.
    //////////////////////////////////////////////////////////////////////
    /**
      This method returns number of ticks in microseconds since some
      pre-defined time in the past.  *NOTE*: it is necessary that this
      pre-defined time exists between runs of the application, and must
      be (relatively) unique.  For example, the time since the system
      started running is not a good choice, since it can be duplicated.
      The current implementation uses time since the UNIX epoch.

      @return Current time in microseconds.
    */
    virtual uInt64 getTicks() const;

    /**
      This method runs the main loop.  Since different platforms
      may use different timing methods and/or algorithms, this method can
      be overrided.  However, the port then takes all responsibility for
      running the emulation and taking care of timing.
    */
    virtual void mainLoop();

    /**
      This method determines the default mapping of joystick actions to
      Stella events for a specific system/platform.

      @param event  The event which to (re)set (Event::NoType resets all)
      @param mode   The mode for which the defaults are set
    */
    virtual void setDefaultJoymap(Event::Type event, EventMode mode);

    /**
      This method creates events from platform-specific hardware.
    */
    virtual void pollEvent();

    /**
      Informs the OSystem of a change in EventHandler state.
    */
    virtual void stateChanged(EventHandler::State state);

    /**
      Returns the default path for the snapshot directory.
      Since this varies greatly among different systems and is the one
      directory that most end-users care about (vs. config file stuff
      that usually isn't user-modifiable), we create a special method
      for it.
    */
    virtual string defaultSnapDir() { return "~"; }

    /**
      Set the position of the application window, generally using
      platform-specific code.  Note that this method is only ever
      called for windowed mode, so no provisions need be made
      for fullscreen mode.
    */
    virtual void setAppWindowPos(int x, int y, int w, int h) { };

    // Pointer to the (currently defined) Console object
    Console* myConsole;

  protected:
    /**
      Query the OSystem video hardware for resolution information.
    */
    virtual bool queryVideoHardware();

    /**
      Set the base directory for all Stella files (these files may be
      located in other places through settings).
    */
    void setBaseDir(const string& basedir);

    /**
      Set the locations of config file
    */
    void setConfigFile(const string& file);

  protected:
    // Pointer to the EventHandler object
    EventHandler* myEventHandler;

    // Pointer to the FrameBuffer object
    FrameBuffer* myFrameBuffer;

    // Pointer to the Sound object
    Sound* mySound;

    // Pointer to the Settings object
    Settings* mySettings;

    // Pointer to the PropertiesSet object
    PropertiesSet* myPropSet;

    // Pointer to the serial port object
    SerialPort* mySerialPort;

  private:
    string myNVRamDir;
    string myPaletteFile;

  private:
    /**
      Creates the various framebuffers/renderers available in this system
      (for now, that means either 'software' or 'opengl').  Note that
      it will only create one type per run of Stella.

      @return  Success or failure of the framebuffer creation
               Note that if OpenGL mode fails because OpenGL is not
               available, rendering will attempt to fall back to
               software mode
    */
    FBInitStatus createFrameBuffer();

    /**
      Creates the various sound devices available in this system
      (for now, that means either 'SDL' or 'Null').
    */
    void createSound();

    /**
      Creates an actual Console object based on the given info.

      @param romfile  The full pathname of the ROM to use
      @param md5      The MD5sum of the ROM
      @param type     The bankswitch type of the ROM
      @param id       The additional id (if any) used by the ROM

      @return  The actual Console object, otherwise NULL
               (calling method is responsible for deleting it)
    */
    Console* openConsole(const string& romfile, string& md5, string& type, string& id);

    /**
      Open the given ROM and return an array containing its contents.
      Also, the properties database is updated with a valid ROM name
      for this ROM (if necessary).

      @param rom    The absolute pathname of the ROM file
      @param md5    The md5 calculated from the ROM file
                    (will be recalculated if necessary)
      @param size   The amount of data read into the image array

      @return  Pointer to the array, with size >=0 indicating valid data
               (calling method is responsible for deleting it)
    */
    uInt8* openROM(string rom, string& md5, uInt32& size);

    /**
      Gets all possible info about the given console.

      @param console  The console to use
      @return  Some information about this console
    */
    string getROMInfo(const Console* console);

    /**
      Initializes the timing so that the mainloop is reset to its
      initial values.
    */
    void resetLoopTiming();

    /**
      Validate the directory name, and create it if necessary.
      Also, update the settings with the new name.  For now, validation
      means that the path must always end with the appropriate separator.

      @param path     The actual path being accessed and created
      @param setting  The setting corresponding to the path being considered
      @param defaultpath  The default path to use if the settings don't exist
    */
    void validatePath(string& path, const string& setting,
    		              const string& defaultpath);

    // Copy constructor isn't supported by this class so make it private
    OSystem(const OSystem&);

    // Assignment operator isn't supported by this class so make it private
    OSystem& operator = (const OSystem&);
};

#endif
