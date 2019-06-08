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
// $Id: FrameBuffer.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef FRAMEBUFFER_HXX
#define FRAMEBUFFER_HXX

#include "EventHandler.hxx"
class OSystem;

// Return values for initialization of framebuffer window
enum FBInitStatus {
  kSuccess,
  kFailComplete,
  kFailTooLarge,
  kFailNotSupported,
};


class FrameBuffer
{
  public:
    /**
      Creates a new Frame Buffer
    */
    FrameBuffer(/*OSystem* osystem*/);

    /**
      Destructor
    */
    ~FrameBuffer();

    /**
      (Re)initializes the framebuffer display.  This must be called before any
      calls are made to derived methods.

      @param title   The title of the window
      @param width   The width of the framebuffer
      @param height  The height of the framebuffer

      @return  Status of initialization (see FBInitStatus 'enum')
    */
    FBInitStatus initialize(const string& title, uInt32 width, uInt32 height);

    /**
      Updates the display, which depending on the current mode could mean
      drawing the TIA, any pending menus, etc.
    */
    void update();

    /**
      Toggles showing or hiding framerate statistics.
    */
    void toggleFrameStats();

    /**
      Shows a message containing frame statistics for the current frame.
    */
    void showFrameStats(bool enable);

    /**
      Enable/disable any pending messages.  Disabled messages aren't removed
      from the message queue; they're just not redrawn into the framebuffer.
    */
    void enableMessages(bool enable);

    /**
      Allocate a new surface with a unique ID.  The FrameBuffer class takes
      all responsibility for freeing this surface (ie, other classes must not
      delete it directly).

      @param w       The requested width of the new surface.
      @param h       The requested height of the new surface.
      @param useBase Use the base surface instead of creating a new one

      @return  A unique ID used to identify this surface
    */
    int allocateSurface(int w, int h, bool useBase = false);

    /**
      Refresh display according to the current state, taking single vs.
      double-buffered modes into account, and redrawing accordingly.
    */
    void refresh();

    /**
      Toggles between fullscreen and window mode.
      Grabmouse activated when in fullscreen mode.
    */
    void toggleFullscreen();

    /**
      Enables/disables fullscreen mode.
      Grabmouse activated when in fullscreen mode.  

      @param enable  Set the fullscreen mode to this value
    */
    void setFullscreen(bool enable);

    /**
      This method is called when the user wants to switch to the next available
      video mode (functionality depends on fullscreen or windowed mode).
      direction = -1 means go to the next lower video mode
      direction =  0 means to reload the current video mode
      direction = +1 means go to the next higher video mode

      @param direction  Described above
    */
    bool changeVidMode(int direction);

    /**
      Sets the state of the cursor (hidden or grabbed) based on the
      current mode.
    */
    void setCursorState();

    /**
      Toggles the use of grabmouse (only has effect in emulation mode).
      The method changes the 'grabmouse' setting and saves it.
    */
    void toggleGrabMouse();

    /**
      Shows or hides the cursor based on the given boolean value.
    */
    void showCursor(bool show);

    /**
      Answers if the display is currently in fullscreen mode.
    */
    bool fullScreen() const;

    /**
      Set the title for the main SDL window.
    */
    void setWindowTitle(const string& title);

    /**
      Get the TIA pixel associated with the given TIA buffer index,
      shifting by the given offset (for greyscale values).
    */
    uInt32 tiaPixel(uInt32 idx, uInt8 shift = 0) const;

    /**
      Set up the TIA/emulation palette for a screen of any depth > 8.

      @param palette  The array of colors
    */
    void setTIAPalette(const uInt32* palette);

    /**
      Set up the user interface palette for a screen of any depth > 8.

      @param palette  The array of colors
    */
    void setUIPalette(const uInt32* palette);

    /**
      Shows a message onscreen.

      @param message  The message to be shown
      @param position Onscreen position for the message
      @param force    Force showing this message, even if messages are disabled
      @param color    Color of text in the message
    */
    void showMessage(const string& message,
                     int position = 0,
                     bool force = false,
                     uInt32 color = 0) { };

  //////////////////////////////////////////////////////////////////////
  // The following methods are system-specific and must be implemented
  // in derived classes.
  //////////////////////////////////////////////////////////////////////
  public:
    /**
      Enable/disable phosphor effect.
    */
    void enablePhosphor(bool enable, int blend) { }

    /**
      This method is called to get the specified scanline data from the
      viewable FrameBuffer area.  Note that this isn't the same as any
      internal surfaces that may be in use; it should return the actual
      data as it is currently seen onscreen.

      @param row  The row we are looking for
      @param data The actual pixel data (in bytes)
    */
    //virtual void scanline(uInt32 row, uInt8* data) const = 0;
    
    //virtual uInt32 mapRGB(uInt8 r, uInt8 g, uInt8 b) const = 0;
};

#endif
