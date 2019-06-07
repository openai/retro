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
// $Id: Control.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <cassert>

#include "System.hxx"
#include "Control.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Controller::Controller(Jack jack, const Event& event, const System& system,
                       Type type)
  : myJack(jack),
    myEvent(event),
    mySystem(system),
    myType(type)
{
  myDigitalPinState[One]   = 
  myDigitalPinState[Two]   = 
  myDigitalPinState[Three] = 
  myDigitalPinState[Four]  = 
  myDigitalPinState[Six]   = true;

  myAnalogPinValue[Five] = 
  myAnalogPinValue[Nine] = maximumResistance;

  switch(myType)
  {
    case Joystick:
      myName = "Joystick";
      break;
    case Paddles:
      myName = "Paddles";
      break;
    case BoosterGrip:
      myName = "BoosterGrip";
      break;
    case Driving:
      myName = "Driving";
      break;
    case Keyboard:
      myName = "Keyboard";
      break;
    case TrackBall22:
      myName = "TrackBall22";
      break;
    case TrackBall80:
      myName = "TrackBall80";
      break;
    case AmigaMouse:
      myName = "AmigaMouse";
      break;
    case AtariVox:
      myName = "AtariVox";
      break;
    case SaveKey:
      myName = "SaveKey";
      break;
    case KidVid:
      myName = "KidVid";
      break;
    case Genesis:
      myName = "Genesis";
      break;
    case MindLink:
      myName = "MindLink";
      break;
    case CompuMate:
      myName = "CompuMate";
      break;
  }
}
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Controller::~Controller()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 Controller::read()
{
  uInt8 ioport = 0x00;
  if(read(One))   ioport |= 0x01;
  if(read(Two))   ioport |= 0x02;
  if(read(Three)) ioport |= 0x04;
  if(read(Four))  ioport |= 0x08;
  return ioport;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Controller::read(DigitalPin pin)
{
  return myDigitalPinState[pin];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Int32 Controller::read(AnalogPin pin)
{
  return myAnalogPinValue[pin];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Controller::set(DigitalPin pin, bool value)
{
  myDigitalPinState[pin] = value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Controller::set(AnalogPin pin, Int32 value)
{
  myAnalogPinValue[pin] = value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Controller::save(Serializer& out) const
{
  try
  {
    // Output the digital pins
    out.putBool(myDigitalPinState[One]);
    out.putBool(myDigitalPinState[Two]);
    out.putBool(myDigitalPinState[Three]);
    out.putBool(myDigitalPinState[Four]);
    out.putBool(myDigitalPinState[Six]);

    // Output the analog pins
    out.putInt(myAnalogPinValue[Five]);
    out.putInt(myAnalogPinValue[Nine]);
  }
  catch(...)
  {
    cerr << "ERROR: Controller::save() exception\n";
    return false;
  }
  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Controller::load(Serializer& in)
{
  try
  {
    // Input the digital pins
    myDigitalPinState[One]   = in.getBool();
    myDigitalPinState[Two]   = in.getBool();
    myDigitalPinState[Three] = in.getBool();
    myDigitalPinState[Four]  = in.getBool();
    myDigitalPinState[Six]   = in.getBool();

    // Input the analog pins
    myAnalogPinValue[Five] = (Int32) in.getInt();
    myAnalogPinValue[Nine] = (Int32) in.getInt();
  }
  catch(...)
  {
    cerr << "ERROR: Controller::load() exception\n";
    return false;
  }
  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Controller::name() const
{
  return myName;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Controller::about() const
{
  return name() + " in " + (myJack == Left ? "left port" : "right port");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const Int32 Controller::maximumResistance = 0x7FFFFFFF;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const Int32 Controller::minimumResistance = 0x00000000;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Controller::Controller(const Controller& c)
  : myJack(c.myJack),
    myEvent(c.myEvent),
    mySystem(c.mySystem),
    myType(c.myType)
{
  assert(false);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Controller& Controller::operator = (const Controller&)
{
  assert(false);
  return *this;
}
