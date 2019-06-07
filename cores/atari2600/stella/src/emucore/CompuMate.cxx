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
// $Id: CompuMate.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "Control.hxx"
#include "System.hxx"
#include "StellaKeys.hxx"
#include "CompuMate.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CompuMate::CompuMate(CartridgeCM& cart, const Event& event,
                     const System& system)
  : myCart(cart),
    myEvent(event),
    mySystem(system),
    myLeftController(0),
    myRightController(0),
    myCycleAtLastUpdate(0)
{
  myLeftController = new CMControl(*this, Controller::Left, event, system);
  myRightController = new CMControl(*this, Controller::Right, event, system);

  myLeftController->myAnalogPinValue[Controller::Nine] = Controller::maximumResistance;
  myLeftController->myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
  myRightController->myAnalogPinValue[Controller::Nine] = Controller::minimumResistance;
  myRightController->myAnalogPinValue[Controller::Five] = Controller::maximumResistance;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CompuMate::enableKeyHandling(bool enable)
{
  if(enable)
    myKeyTable = myEvent.getKeys();
  else
  {
    for(uInt32 i = 0; i < KBDK_LAST; ++i)
      myInternalKeyTable[i] = false;

    myKeyTable = myInternalKeyTable;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CompuMate::update()
{
  uInt32 cycle = mySystem.cycles();

  // Only perform update once for both ports in the same cycle
  if(myCycleAtLastUpdate != cycle)
  {
    myCycleAtLastUpdate = cycle;
    return;
  }
  myCycleAtLastUpdate = cycle;

  // Handle SWCHA changes - the following comes almost directly from z26
  Controller& lp = *myLeftController;
  Controller& rp = *myRightController;

  lp.myAnalogPinValue[Controller::Nine] = Controller::maximumResistance;
  lp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
  lp.myDigitalPinState[Controller::Six] = true;
  rp.myAnalogPinValue[Controller::Nine] = Controller::minimumResistance;
  rp.myAnalogPinValue[Controller::Five] = Controller::maximumResistance;
  rp.myDigitalPinState[Controller::Six] = true;

  if (myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT])
    rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
  if (myKeyTable[KBDK_LCTRL] || myKeyTable[KBDK_RCTRL])
    lp.myAnalogPinValue[Controller::Nine] = Controller::minimumResistance;

  rp.myDigitalPinState[Controller::Three] = true;
  rp.myDigitalPinState[Controller::Four] = true;
  switch(myCart.column())
  {
    case 0:
      if (myKeyTable[KBDK_7]) lp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_u]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_j]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_m]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 1:
      if (myKeyTable[KBDK_6]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the '?' character (Shift-6) with the actual question key
      if (myKeyTable[KBDK_SLASH] && (myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_y]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_h]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_n]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 2:
      if (myKeyTable[KBDK_8]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the '[' character (Shift-8) with the actual key
      if (myKeyTable[KBDK_LEFTBRACKET] && !(myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_i]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_k]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_COMMA]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 3:
      if (myKeyTable[KBDK_2]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the '-' character (Shift-2) with the actual minus key
      if (myKeyTable[KBDK_MINUS] && !(myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_w]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_s]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_x]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 4:
      if (myKeyTable[KBDK_3]) lp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_e]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_d]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_c]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 5:
      if (myKeyTable[KBDK_0]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the quote character (Shift-0) with the actual quote key
      if (myKeyTable[KBDK_QUOTE] && (myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_p]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_RETURN] || myKeyTable[KBDK_KP_ENTER])
        rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_SPACE]) rp.myDigitalPinState[Controller::Four] = false;
      // Emulate Ctrl-space (aka backspace) with the actual Backspace key
      if (myKeyTable[KBDK_BACKSPACE])
      {
        lp.myAnalogPinValue[Controller::Nine] = Controller::minimumResistance;
        rp.myDigitalPinState[Controller::Four] = false;
      }
      break;
    case 6:
      if (myKeyTable[KBDK_9]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the ']' character (Shift-9) with the actual key
      if (myKeyTable[KBDK_RIGHTBRACKET] && !(myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_o]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_l]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_PERIOD]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 7:
      if (myKeyTable[KBDK_5]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the '=' character (Shift-5) with the actual equals key
      if (myKeyTable[KBDK_EQUALS] && !(myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_t]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_g]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_b]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 8:
      if (myKeyTable[KBDK_1]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the '+' character (Shift-1) with the actual plus key (Shift-=)
      if (myKeyTable[KBDK_EQUALS] && (myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_q]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_a]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_z]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    case 9:
      if (myKeyTable[KBDK_4]) lp.myDigitalPinState[Controller::Six] = false;
      // Emulate the '/' character (Shift-4) with the actual slash key
      if (myKeyTable[KBDK_SLASH] && !(myKeyTable[KBDK_LSHIFT] || myKeyTable[KBDK_RSHIFT]))
      {
        rp.myAnalogPinValue[Controller::Five] = Controller::minimumResistance;
        lp.myDigitalPinState[Controller::Six] = false;
      }
      if (myKeyTable[KBDK_r]) rp.myDigitalPinState[Controller::Three] = false;
      if (myKeyTable[KBDK_f]) rp.myDigitalPinState[Controller::Six] = false;
      if (myKeyTable[KBDK_v]) rp.myDigitalPinState[Controller::Four] = false;
      break;
    default:
      break;
  }
}
