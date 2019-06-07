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
// $Id: Genesis.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "Event.hxx"
#include "Genesis.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Genesis::Genesis(Jack jack, const Event& event, const System& system)
  : Controller(jack, event, system, Controller::Genesis),
    myControlID(-1)
{
  if(myJack == Left)
  {
    myUpEvent      = Event::JoystickZeroUp;
    myDownEvent    = Event::JoystickZeroDown;
    myLeftEvent    = Event::JoystickZeroLeft;
    myRightEvent   = Event::JoystickZeroRight;
    myFire1Event   = Event::JoystickZeroFire;
    myFire2Event   = Event::JoystickZeroFire5;
  }
  else
  {
    myUpEvent      = Event::JoystickOneUp;
    myDownEvent    = Event::JoystickOneDown;
    myLeftEvent    = Event::JoystickOneLeft;
    myRightEvent   = Event::JoystickOneRight;
    myFire1Event   = Event::JoystickOneFire;
    myFire2Event   = Event::JoystickOneFire5;
  }

  // Analog pin 9 is not connected to this controller at all
  // Analog pin 5 corresponds to button 'C' on the gamepad, and corresponds
  // to the 'booster' button on a BoosterGrip controller
  myAnalogPinValue[Five] = myAnalogPinValue[Nine] = maximumResistance;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Genesis::~Genesis()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Genesis::update()
{
  // Digital events (from keyboard or joystick hats & buttons)
  myDigitalPinState[One]   = (myEvent.get(myUpEvent) == 0);
  myDigitalPinState[Two]   = (myEvent.get(myDownEvent) == 0);
  myDigitalPinState[Three] = (myEvent.get(myLeftEvent) == 0);
  myDigitalPinState[Four]  = (myEvent.get(myRightEvent) == 0);
  myDigitalPinState[Six]   = (myEvent.get(myFire1Event) == 0);

  // The Genesis has one more button (C) that can be read by the 2600
  // However, it seems to work opposite to the BoosterGrip controller,
  // in that the logic is inverted
  myAnalogPinValue[Five] = (myEvent.get(myFire2Event) == 0) ?
                            minimumResistance : maximumResistance;

  // Mouse motion and button events
  if(myControlID > -1)
  {
    // The following code was taken from z26
    #define MJ_Threshold 2
    int mousex = myEvent.get(Event::MouseAxisXValue),
        mousey = myEvent.get(Event::MouseAxisYValue);
    if(mousex || mousey)
    {
      if((!(abs(mousey) > abs(mousex) << 1)) && (abs(mousex) >= MJ_Threshold))
      {
        if(mousex < 0)
          myDigitalPinState[Three] = false;
        else if (mousex > 0)
          myDigitalPinState[Four] = false;
      }

      if((!(abs(mousex) > abs(mousey) << 1)) && (abs(mousey) >= MJ_Threshold))
      {
        if(mousey < 0)
          myDigitalPinState[One] = false;
        else if(mousey > 0)
          myDigitalPinState[Two] = false;
      }
    }
    // Get mouse button state
    if(myEvent.get(Event::MouseButtonLeftValue))
      myDigitalPinState[Six] = false;
    if(myEvent.get(Event::MouseButtonRightValue))
      myAnalogPinValue[Five] = maximumResistance;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Genesis::setMouseControl(
    Controller::Type xtype, int xid, Controller::Type ytype, int yid)
{
  // Currently, the Genesis controller takes full control of the mouse, using
  // both axes for its two degrees of movement, and the left/right buttons for
  // 'B' and 'C', respectively
  if(xtype == Controller::Genesis && ytype == Controller::Genesis && xid == yid)
  {
    myControlID = ((myJack == Left && xid == 0) ||
                   (myJack == Right && xid == 1)
                  ) ? xid : -1;
  }
  else
    myControlID = -1;

  return true;
}
