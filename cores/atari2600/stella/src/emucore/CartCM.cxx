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
// $Id: CartCM.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <cassert>
#include <cstring>

#include "System.hxx"
#include "M6532.hxx"
#include "CartCM.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeCM::CartridgeCM(const uInt8* image, uInt32 size, const Settings& settings)
  : Cartridge(settings)
{
  // Copy the ROM image into my buffer
  memcpy(myImage, image, MIN(16384u, size));
  createCodeAccessBase(16384);

  // This cart contains 2048 bytes extended RAM @ 0x1800
  // This RAM scheme is unique in that it doesn't require separate read/write ports
  registerRamArea(0x1800, 2048, 0x00, 0x00);

  // On powerup, portA is all 1's, so the last bank of ROM is enabled and
  // RAM is disabled
  mySWCHA = 0xff;
  myStartBank = mySWCHA & 0x3;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeCM::~CartridgeCM()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCM::reset()
{
  // Initialize RAM
  if(mySettings.getBool("ramrandom"))
    for(uInt32 i = 0; i < 2048; ++i)
      myRAM[i] = mySystem->randGenerator().next();
  else
    memset(myRAM, 0, 2048);

  // Upon reset we switch to the startup bank
  bank(myStartBank);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCM::install(System& system)
{
  mySystem = &system;
  uInt16 mask = mySystem->pageMask();

  // Make sure the system we're being installed in has a page size that'll work
  assert((0x1000 & mask) == 0);

  // Mirror all access in RIOT; by doing so we're taking responsibility
  // for that address space in peek and poke below.
  mySystem->m6532().install(system, *this);

  // Install pages for the startup bank
  bank(myStartBank);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 CartridgeCM::peek(uInt16 address)
{
  // NOTE: This does not handle accessing cart ROM/RAM, however, this function
  // should never be called for ROM/RAM because of the way page accessing
  // has been setup (it will only ever be called for RIOT reads)
  return mySystem->m6532().peek(address);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCM::poke(uInt16 address, uInt8 value)
{
  // NOTE: This could be called for RIOT writes or cart ROM writes
  // In the latter case, the write is ignored
  if(!(address & 0x1000))
  {
    // RIOT mirroring, check bankswitch
    if(address == 0x280)
    {
      mySWCHA = value;
      bank(mySWCHA & 0x3);
      if(value & 0x20) myColumn = 0;
      if(value & 0x40) myColumn = (myColumn + 1) % 10;
    }
    mySystem->m6532().poke(address, value);
  }
  return myBankChanged;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCM::bank(uInt16 bank)
{
  if(bankLocked()) return false;

  // Remember what bank we're in
  myCurrentBank = bank;
  uInt16 offset = myCurrentBank << 12;
  uInt16 shift = mySystem->pageShift();

  // Although this scheme contains four 4K ROM banks and one 2K RAM bank,
  // it's easier to think of things in terms of 2K slices, as follows:
  //
  // The lower 2K of cart address space always points to the lower 2K of the
  // current ROM bank
  // The upper 2K of cart address space can point to either the 2K of RAM or
  // the upper 2K of the current ROM bank

  System::PageAccess access(0, 0, 0, this, System::PA_READ);

  // Lower 2K (always ROM)
  for(uInt32 address = 0x1000; address < 0x1800; address += (1 << shift))
  {
    access.directPeekBase = &myImage[offset + (address & 0x0FFF)];
    access.codeAccessBase = &myCodeAccessBase[offset + (address & 0x0FFF)];
    mySystem->setPageAccess(address >> shift, access);
  }

  // Upper 2K (RAM or ROM)
  for(uInt32 address = 0x1800; address < 0x2000; address += (1 << shift))
  {
    access.type = System::PA_READWRITE;

    if(mySWCHA & 0x10)
    {
      access.directPeekBase = &myImage[offset + (address & 0x0FFF)];
      access.codeAccessBase = &myCodeAccessBase[offset + (address & 0x0FFF)];
    }
    else
    {
      access.directPeekBase = &myRAM[address & 0x7FF];
      access.codeAccessBase = &myCodeAccessBase[offset + (address & 0x07FF)];
    }

    if((mySWCHA & 0x30) == 0x20)
      access.directPokeBase = &myRAM[address & 0x7FF];
    else
      access.directPokeBase = 0;

    mySystem->setPageAccess(address >> shift, access);
  }

  return myBankChanged = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt16 CartridgeCM::bank() const
{
  return myCurrentBank;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt16 CartridgeCM::bankCount() const
{
  // We report 4 banks (of ROM), even though RAM can overlap the upper 2K
  // of cart address space at some times
  // However, this RAM isn't enabled in the normal way that bankswitching
  // works, so it is ignored here
  return 4;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCM::patch(uInt16 address, uInt8 value)
{
  if((mySWCHA & 0x30) == 0x20)
    myRAM[address & 0x7FF] = value;
  else
    myImage[(myCurrentBank << 12) + address] = value;

  return myBankChanged = true;
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const uInt8* CartridgeCM::getImage(int& size) const
{
  size = 16384;
  return myImage;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCM::save(Serializer& out) const
{
   out.putString(name());
   out.putShort(myCurrentBank);
   out.putByte(mySWCHA);
   out.putByte(myColumn);
   out.putByteArray(myRAM, 2048);

   return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCM::load(Serializer& in)
{
   if(in.getString() != name())
      return false;

   myCurrentBank = in.getShort();
   mySWCHA = in.getByte();
   myColumn = in.getByte();
   in.getByteArray(myRAM, 2048);

   // Remember what bank we were in
   bank(myCurrentBank);

   return true;
}
