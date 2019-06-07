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
// $Id: CheetahCheat.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef CHEETAH_CHEAT_HXX
#define CHEETAH_CHEAT_HXX

#include "Cheat.hxx"

class CheetahCheat : public Cheat
{
  public:
    CheetahCheat(OSystem* os, const string& name, const string& code);
    ~CheetahCheat();

    virtual bool enable();
    virtual bool disable();

    virtual void evaluate();

  private:
    uInt8  savedRom[16];
    uInt16 address;
    uInt8  value;
    uInt8  count;
};

#endif
