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
// $Id: Event.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef EVENT_HXX
#define EVENT_HXX

#include "bspf.hxx"
#include "StellaKeys.hxx"

/**
  @author  Bradford W. Mott
  @version $Id: Event.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class Event
{
  public:
    /**
      Enumeration of all possible events in Stella, including both
      console and controller event types as well as events that aren't
      technically part of the emulation core
    */
    enum Type
    {
      NoType,
      ConsoleOn, ConsoleOff, ConsoleColor, ConsoleBlackWhite,
      ConsoleLeftDiffA, ConsoleLeftDiffB,
      ConsoleRightDiffA, ConsoleRightDiffB,
      ConsoleSelect, ConsoleReset,

      JoystickZeroUp, JoystickZeroDown, JoystickZeroLeft, JoystickZeroRight,
        JoystickZeroFire, JoystickZeroFire5, JoystickZeroFire9,
      JoystickOneUp, JoystickOneDown, JoystickOneLeft, JoystickOneRight,
        JoystickOneFire, JoystickOneFire5, JoystickOneFire9,

      PaddleZeroDecrease, PaddleZeroIncrease, PaddleZeroAnalog, PaddleZeroFire,
      PaddleOneDecrease, PaddleOneIncrease, PaddleOneAnalog, PaddleOneFire,
      PaddleTwoDecrease, PaddleTwoIncrease, PaddleTwoAnalog, PaddleTwoFire,
      PaddleThreeDecrease, PaddleThreeIncrease, PaddleThreeAnalog, PaddleThreeFire,

      KeyboardZero1, KeyboardZero2, KeyboardZero3,
      KeyboardZero4, KeyboardZero5, KeyboardZero6,
      KeyboardZero7, KeyboardZero8, KeyboardZero9,
      KeyboardZeroStar, KeyboardZero0, KeyboardZeroPound,

      KeyboardOne1, KeyboardOne2, KeyboardOne3,
      KeyboardOne4, KeyboardOne5, KeyboardOne6,
      KeyboardOne7, KeyboardOne8, KeyboardOne9,
      KeyboardOneStar, KeyboardOne0, KeyboardOnePound,

      Combo1, Combo2, Combo3, Combo4, Combo5, Combo6, Combo7, Combo8,
      Combo9, Combo10, Combo11, Combo12, Combo13, Combo14, Combo15, Combo16,
  
      SALeftAxis0Value, SALeftAxis1Value,
      SARightAxis0Value, SARightAxis1Value,

      MouseAxisXValue, MouseAxisYValue,
      MouseButtonLeftValue, MouseButtonRightValue,

      ChangeState, LoadState, SaveState, TakeSnapshot, Quit,
      PauseMode, MenuMode, CmdMenuMode, DebuggerMode, LauncherMode,
      Fry, VolumeDecrease, VolumeIncrease,

      UIUp, UIDown, UILeft, UIRight, UIHome, UIEnd, UIPgUp, UIPgDown,
      UISelect, UINavPrev, UINavNext, UIOK, UICancel, UIPrevDir,

      LastType
    };

  public:
    /**
      Create a new event object
    */
    Event() { clear(); }

  public:
    /**
      Get the value associated with the event of the specified type
    */
    Int32 get(Type type) const { return myValues[type]; }

    /**
      Set the value associated with the event of the specified type
    */
    void set(Type type, Int32 value) { myValues[type] = value; }

    /**
      Clears the event array (resets to initial state)
    */
    void clear()
    {
      for(uInt32 i = 0; i < LastType; ++i)
        myValues[i] = Event::NoType;

      for(uInt32 i = 0; i < KBDK_LAST; ++i)
        myKeyTable[i] = false;
    }

    /**
      Get the keytable associated with this event
    */
    const bool* getKeys() const { return myKeyTable; }

    /**
      Set the value associated with the event of the specified type
    */
    void setKey(StellaKey key, bool state) { myKeyTable[key] = state; }

  private:
    // Array of values associated with each event type
    Int32 myValues[LastType];

    // Array of keyboard key states
    bool myKeyTable[KBDK_LAST];
};

#endif
