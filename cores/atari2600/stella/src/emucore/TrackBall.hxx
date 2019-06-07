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
// $Id: TrackBall.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef TRACKBALL_HXX
#define TRACKBALL_HXX

#include "bspf.hxx"
#include "Control.hxx"
#include "Event.hxx"

/**
  The various trackball-like controllers supported by the Atari 2600.
  They're all placed in one class, since other than a few minor
  differences, they work almost exactly the same.  This code was
  heavily borrowed from z26.

  The supported controllers include:
    TrackBall22: Atari 2600 CX-22 Trakball
    TrackBall80: Atari ST CX-80 Trakball
    AmigaMouse:  Amiga Mouse

  @author  Stephen Anthony & z26 team
  @version $Id: TrackBall.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class TrackBall : public Controller
{
  public:
    /**
      Create a new TrackBall controller plugged into the specified jack

      @param jack   The jack the controller is plugged into
      @param event  The event object to use for events
      @param system The system using this controller
      @param type   The type of trackball controller
    */
    TrackBall(Jack jack, const Event& event, const System& system, Type type);

    /**
      Destructor
    */
    virtual ~TrackBall();

  public:
    using Controller::read;

    /**
      Read the entire state of all digital pins for this controller.
      Note that this method must use the lower 4 bits, and zero the upper bits.

      @return The state of all digital pins
    */
    uInt8 read();

    /**
      Update the entire digital and analog pin state according to the
      events currently set.
    */
    void update();

    /**
      Determines how this controller will treat values received from the
      X/Y axis and left/right buttons of the mouse.  Since not all controllers
      use the mouse the same way (or at all), it's up to the specific class to
      decide how to use this data.

      In the current implementation, the left button is tied to the X axis,
      and the right one tied to the Y axis.

      @param xtype  The controller to use for x-axis data
      @param xid    The controller ID to use for x-axis data (-1 for no id)
      @param ytype  The controller to use for y-axis data
      @param yid    The controller ID to use for y-axis data (-1 for no id)

      @return  Whether the controller supports using the mouse
    */
    bool setMouseControl(
      Controller::Type xtype, int xid, Controller::Type ytype, int yid);

  private:
    // Counter to iterate through the gray codes
    int myHCounter, myVCounter;

    // How many new horizontal and vertical values this frame
    int myTrakBallCountH, myTrakBallCountV;

    // How many lines to wait before sending new horz and vert val
    int myTrakBallLinesH, myTrakBallLinesV;

    // Was TrakBall moved left or moved right instead
    int myTrakBallLeft;

    // Was TrakBall moved down or moved up instead
    int myTrakBallDown;

    int myScanCountH, myScanCountV, myCountH, myCountV;

    // Whether to use the mouse to emulate this controller
    int myMouseEnabled;  

    // CX-22
    static const uInt32 ourTrakBallTableTB_H[2][2];
    static const uInt32 ourTrakBallTableTB_V[2][2];

    // ST mouse / CX-80
    static const uInt32 ourTrakBallTableST_H[4];
    static const uInt32 ourTrakBallTableST_V[4];

    // Amiga mouse
    static const uInt32 ourTrakBallTableAM_H[4];
    static const uInt32 ourTrakBallTableAM_V[4];
};

#endif
