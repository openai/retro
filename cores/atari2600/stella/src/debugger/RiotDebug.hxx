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
// $Id: RiotDebug.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef RIOT_DEBUG_HXX
#define RIOT_DEBUG_HXX

class Debugger;
class RiotDebug;

#include "Array.hxx"
#include "M6532.hxx"
#include "DebuggerSystem.hxx"

class RiotState : public DebuggerState
{
  public:
    uInt8 SWCHA_R, SWCHA_W, SWACNT, SWCHB_R, SWCHB_W, SWBCNT;
    BoolArray swchaReadBits;
    BoolArray swchaWriteBits;
    BoolArray swacntBits;
    BoolArray swchbReadBits;
    BoolArray swchbWriteBits;
    BoolArray swbcntBits;

    uInt8 TIM1T, TIM8T, TIM64T, T1024T, INTIM, TIMINT;
    Int32 TIMCLKS, INTIMCLKS;

    // These are actually from the TIA, but are I/O related
    uInt8 INPT0, INPT1, INPT2, INPT3, INPT4, INPT5;
};

class RiotDebug : public DebuggerSystem
{
  public:
    RiotDebug(Debugger& dbg, Console& console);

    const DebuggerState& getState();
    const DebuggerState& getOldState() { return myOldState; }

    void saveOldState();
    string toString();

    /* Port A and B registers */
    uInt8 swcha(int newVal = -1);
    uInt8 swacnt(int newVal = -1);
    uInt8 swchb(int newVal = -1);
    uInt8 swbcnt(int newVal = -1);

    /* TIA INPTx and VBLANK registers
       Techically not part of the RIOT, but more appropriately placed here */
    uInt8 inpt(int x);
    bool vblank(int bit);

    /* Timer registers & associated clock */
    uInt8 tim1T(int newVal = -1);
    uInt8 tim8T(int newVal = -1);
    uInt8 tim64T(int newVal = -1);
    uInt8 tim1024T(int newVal = -1);
    uInt8 intim() const       { return mySystem.m6532().intim();       }
    uInt8 timint() const      { return mySystem.m6532().timint();      }
    Int32 timClocks() const   { return mySystem.m6532().timerClocks(); }
    Int32 intimClocks() const { return mySystem.m6532().intimClocks(); }

    /* Controller ports */
    Controller& controller(Controller::Jack jack) const;

    /* Console switches */
    bool diffP0(int newVal = -1);
    bool diffP1(int newVal = -1);
    bool tvType(int newVal = -1);
    bool select(int newVal = -1);
    bool reset(int newVal = -1);

    /* Port A description */
    string dirP0String();
    string dirP1String();

    /* Port B description */
    string diffP0String();
    string diffP1String();
    string tvTypeString();
    string switchesString();
    
  private:
    RiotState myState;
    RiotState myOldState;
};

#endif
