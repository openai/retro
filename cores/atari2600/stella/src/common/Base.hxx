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
// $Id: Base.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef BASE_HXX
#define BASE_HXX

#include <iostream>
#include <iomanip>

#include "bspf.hxx"

namespace Common {

/**
  This class implements several functions for converting integer data
  into strings in multiple bases, with different formats (# of characters,
  upper/lower-case, etc).

  @author  Stephen Anthony
*/
class Base
{
  public:
    // The base to use for conversion from integers to strings
    // Note that the actual number of places will be determined by
    // the magnitude of the value itself in the general case
    enum Format {
      F_16,      // base 16: 2, 4, 8 bytes (depending on value)
      F_16_1,    // base 16: 1 byte wide
      F_16_2,    // base 16: 2 bytes wide
      F_16_4,    // base 16: 4 bytes wide
      F_16_8,    // base 16: 8 bytes wide
      F_10,      // base 10: 3 or 5 bytes (depending on value)
      F_2,       // base 2:  8 or 16 bits (depending on value)
      F_2_8,     // base 2:  1 byte (8 bits) wide
      F_2_16,    // base 2:  2 bytes (16 bits) wide
      F_DEFAULT
    };

  public:
    /** Get/set the number base when parsing numeric values */
    static void setFormat(Base::Format base) { myDefaultBase = base; }
    static Base::Format format()             { return myDefaultBase; }

    /** Get/set HEX output to be upper/lower case */
    static void setHexUppercase(bool enable);
    static bool hexUppercase() { return myHexflags & std::ios_base::uppercase; }

    /** Output HEX digits in 1/2/4 byte format */
    static inline std::ostream& HEX2(std::ostream& os) {
      os.flags(myHexflags);
      return os << std::setw(2) << std::setfill('0');
    }
    static inline std::ostream& HEX4(std::ostream& os) {
      os.flags(myHexflags);
      return os << std::setw(4) << std::setfill('0');
    }
    static inline std::ostream& HEX8(std::ostream& os) {
      os.flags(myHexflags);
      return os << std::setw(8) << std::setfill('0');
    }

    /** Convert integer to a string in the given base format */
    static string toString(int value,
      Common::Base::Format outputBase = Common::Base::F_DEFAULT);

  private:      // Make sure this class is never instantiated
    Base() { }

  private:
    // Default format to use when none is specified
    static Format myDefaultBase;

    // Upper or lower case for HEX digits
    static std::ios_base::fmtflags myHexflags;

    // Format specifiers to use for sprintf (eventually we may convert
    // to C++ streams
    static const char* myLowerFmt[4];
    static const char* myUpperFmt[4];
    static const char** myFmt;
};

} // Namespace Common

#endif
