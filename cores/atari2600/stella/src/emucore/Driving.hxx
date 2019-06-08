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
// $Id: Driving.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef DRIVING_HXX
#define DRIVING_HXX

#include "bspf.hxx"
#include "Control.hxx"
#include "Event.hxx"

/**
  The standard Atari 2600 Indy 500 driving controller.

  @author  Bradford W. Mott
  @version $Id: Driving.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class Driving : public Controller
{
  public:
    /**
      Create a new Indy 500 driving controller plugged into 
      the specified jack

      @param jack   The jack the controller is plugged into
      @param event  The event object to use for events
      @param system The system using this controller
    */
    Driving(Jack jack, const Event& event, const System& system);

    /**
      Destructor
    */
    virtual ~Driving();

  public:
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
    uInt32 myCounter;

    // Index into the gray code table
    uInt32 myGrayIndex;

    // Y axis value from last yaxis event that was used to generate a new
    // gray code
    int myLastYaxis;

    // Pre-compute the events we care about based on given port
    // This will eliminate test for left or right port in update()
    Event::Type myCWEvent, myCCWEvent, myFireEvent,
                myXAxisValue, myYAxisValue;

    // Controller to emulate in normal mouse axis mode
    int myControlID;

    // Controllers to emulate in 'specific' mouse axis mode
    int myControlIDX, myControlIDY;
};

#endif
