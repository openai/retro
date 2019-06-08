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
// $Id: Base.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "Base.hxx"

namespace Common {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Base::setHexUppercase(bool enable)
{
  if(enable)
  {
    myHexflags |= std::ios_base::uppercase;
    myFmt = Base::myUpperFmt;
  }
  else
  {
    myHexflags &= ~std::ios_base::uppercase;
    myFmt = Base::myLowerFmt;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Base::toString(int value, Common::Base::Format outputBase)
{
  static char vToS_buf[32];

  if(outputBase == Base::F_DEFAULT)
    outputBase = myDefaultBase;

  switch(outputBase)
  {
    case Base::F_2:     // base 2:  8 or 16 bits (depending on value)
    case Base::F_2_8:   // base 2:  1 byte (8 bits) wide
    case Base::F_2_16:  // base 2:  2 bytes (16 bits) wide
    {
      int places = (outputBase == Base::F_2_8 ||
               (outputBase == Base::F_2 && value < 0x100)) ? 8 : 16;
      vToS_buf[places] = '\0';
      int bit = 1;
      while(--places >= 0) {
        if(value & bit) vToS_buf[places] = '1';
        else            vToS_buf[places] = '0';
        bit <<= 1;
      }
      break;
    }

    case Base::F_10:    // base 10: 3 or 5 bytes (depending on value)
      if(value < 0x100)
        BSPF_snprintf(vToS_buf, 4, "%3d", value);
      else
        BSPF_snprintf(vToS_buf, 6, "%5d", value);
      break;

    case Base::F_16_1:  // base 16: 1 byte wide
      BSPF_snprintf(vToS_buf, 2, myFmt[0], value);
      break;
    case Base::F_16_2:  // base 16: 2 bytes wide
      BSPF_snprintf(vToS_buf, 3, myFmt[1], value);
      break;
    case Base::F_16_4:  // base 16: 4 bytes wide
      BSPF_snprintf(vToS_buf, 5, myFmt[2], value);
      break;
    case Base::F_16_8:  // base 16: 8 bytes wide
      BSPF_snprintf(vToS_buf, 9, myFmt[3], value);
      break;

    case Base::F_16:    // base 16: 2, 4, 8 bytes (depending on value)
    default:
      if(value < 0x100)
        BSPF_snprintf(vToS_buf, 3, myFmt[1], value);
      else if(value < 0x10000)
        BSPF_snprintf(vToS_buf, 5, myFmt[2], value);
      else
        BSPF_snprintf(vToS_buf, 9, myFmt[3], value);
      break;
  }

  return string(vToS_buf);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Base::Format Base::myDefaultBase = Base::F_16;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::ios_base::fmtflags Base::myHexflags = std::ios_base::hex;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const char* Base::myLowerFmt[4] = {
  "%1x", "%02x", "%04x", "%08x"
};
const char* Base::myUpperFmt[4] = {
  "%1X", "%02X", "%04X", "%08X"
};
const char** Base::myFmt = Base::myLowerFmt;

} // Namespace Common
