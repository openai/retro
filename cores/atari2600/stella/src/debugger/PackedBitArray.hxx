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
// $Id: PackedBitArray.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef PACKEDBITARRAY_HXX
#define PACKEDBITARRAY_HXX

#include "bspf.hxx"

#define wordSize ( (sizeof(unsigned int)) * 8)

class PackedBitArray
{
  public:
    PackedBitArray(uInt32 length);
    ~PackedBitArray();

    uInt32 isSet(uInt32 bit) const;
    uInt32 isClear(uInt32 bit) const;

    void set(uInt32 bit);
    void clear(uInt32 bit);
    void toggle(uInt32 bit);

  private:
    // number of unsigned ints (size/wordSize):
    uInt32 words;

    // the array itself:
    uInt32* bits;
};

#endif
