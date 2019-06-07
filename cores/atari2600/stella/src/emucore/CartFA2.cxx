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
// $Id: CartFA2.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <cassert>
#include <cstring>

#include "OSystem.hxx"
#include "Serializer.hxx"
#include "System.hxx"
#include "CartFA2.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeFA2::CartridgeFA2(const uInt8* image, uInt32 size, const OSystem& osystem)
  : Cartridge(osystem.settings()),
    myOSystem(osystem),
    myRamAccessTimeout(0),
    mySize(size)
{
  // 29/32K version of FA2 has valid data @ 1K - 29K
  if(size >= 29 * 1024)
  {
    image += 1024;
    mySize = 28 * 1024; 
  }

  // Allocate array for the ROM image
  myImage = new uInt8[mySize];

  // Copy the ROM image into my buffer
  memcpy(myImage, image, mySize);
  createCodeAccessBase(mySize);

  // This cart contains 256 bytes extended RAM @ 0x1000
  registerRamArea(0x1000, 256, 0x100, 0x00);

  // Remember startup bank
  myStartBank = 0;
}
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeFA2::~CartridgeFA2()
{
  delete[] myImage;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeFA2::reset()
{
  // Initialize RAM
  if(mySettings.getBool("ramrandom"))
    for(uInt32 i = 0; i < 256; ++i)
      myRAM[i] = mySystem->randGenerator().next();
  else
    memset(myRAM, 0, 256);

  // Upon reset we switch to the startup bank
  bank(myStartBank);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeFA2::install(System& system)
{
  mySystem = &system;
  uInt16 shift = mySystem->pageShift();
  uInt16 mask = mySystem->pageMask();

  // Make sure the system we're being installed in has a page size that'll work
  assert(((0x1100 & mask) == 0) && ((0x1200 & mask) == 0));

  System::PageAccess access(0, 0, 0, this, System::PA_READ);

  // Set the page accessing method for the RAM writing pages
  access.type = System::PA_WRITE;
  for(uInt32 j = 0x1000; j < 0x1100; j += (1 << shift))
  {
    access.directPokeBase = &myRAM[j & 0x00FF];
    access.codeAccessBase = &myCodeAccessBase[j & 0x00FF];
    mySystem->setPageAccess(j >> shift, access);
  }
 
  // Set the page accessing method for the RAM reading pages
  access.directPokeBase = 0;
  access.type = System::PA_READ;
  for(uInt32 k = 0x1100; k < 0x1200; k += (1 << shift))
  {
    access.directPeekBase = &myRAM[k & 0x00FF];
    access.codeAccessBase = &myCodeAccessBase[0x100 + (k & 0x00FF)];
    mySystem->setPageAccess(k >> shift, access);
  }

  // Install pages for the startup bank
  bank(myStartBank);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 CartridgeFA2::peek(uInt16 address)
{
  uInt16 peekAddress = address;
  address &= 0x0FFF;

  // Switch banks if necessary
  switch(address)
  {
    case 0x0FF4:
      // Load/save RAM to/from Harmony cart flash
      if(mySize == 28*1024 && !bankLocked())
        return ramReadWrite();
      break;

    case 0x0FF5:
      // Set the current bank to the first 4k bank
      bank(0);
      break;

    case 0x0FF6:
      // Set the current bank to the second 4k bank
      bank(1);
      break;

    case 0x0FF7:
      // Set the current bank to the third 4k bank
      bank(2);
      break;
  
    case 0x0FF8:
      // Set the current bank to the fourth 4k bank
      bank(3);
      break;

    case 0x0FF9:
      // Set the current bank to the fifth 4k bank
      bank(4);
      break;

    case 0x0FFA:
      // Set the current bank to the sixth 4k bank
      bank(5);
      break;

    case 0x0FFB:
      // Set the current bank to the seventh 4k bank
      // This is only available on 28K ROMs
      if(mySize == 28*1024)  bank(6);
      break;

    default:
      break;
  }

  if(address < 0x0100)  // Write port is at 0xF000 - 0xF100 (256 bytes)
  {
    // Reading from the write port triggers an unwanted write
    uInt8 value = mySystem->getDataBusState(0xFF);

    if(bankLocked())
      return value;
    else
    {
      triggerReadFromWritePort(peekAddress);
      return myRAM[address] = value;
    }
  }  
  else
    return myImage[(myCurrentBank << 12) + address];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeFA2::poke(uInt16 address, uInt8)
{
  address &= 0x0FFF;

  // Switch banks if necessary
  switch(address)
  {
    case 0x0FF4:
      // Load/save RAM to/from Harmony cart flash
      if(mySize == 28*1024 && !bankLocked())
        ramReadWrite();
      break;

    case 0x0FF5:
      // Set the current bank to the first 4k bank
      bank(0);
      break;

    case 0x0FF6:
      // Set the current bank to the second 4k bank
      bank(1);
      break;

    case 0x0FF7:
      // Set the current bank to the third 4k bank
      bank(2);
      break;
  
    case 0x0FF8:
      // Set the current bank to the fourth 4k bank
      bank(3);
      break;

    case 0x0FF9:
      // Set the current bank to the fifth 4k bank
      bank(4);
      break;

    case 0x0FFA:
      // Set the current bank to the sixth 4k bank
      bank(5);
      break;

    case 0x0FFB:
      // Set the current bank to the seventh 4k bank
      // This is only available on 28K ROMs
      if(mySize == 28*1024)  bank(6);
      break;

    default:
      break;
  }

  // NOTE: This does not handle accessing RAM, however, this function
  // should never be called for RAM because of the way page accessing
  // has been setup
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeFA2::bank(uInt16 bank)
{
  if(bankLocked()) return false;

  // Remember what bank we're in
  myCurrentBank = bank;
  uInt16 offset = myCurrentBank << 12;
  uInt16 shift = mySystem->pageShift();
  uInt16 mask = mySystem->pageMask();

  System::PageAccess access(0, 0, 0, this, System::PA_READ);

  // Set the page accessing methods for the hot spots
  for(uInt32 i = (0x1FF4 & ~mask); i < 0x2000; i += (1 << shift))
  {
    access.codeAccessBase = &myCodeAccessBase[offset + (i & 0x0FFF)];
    mySystem->setPageAccess(i >> shift, access);
  }

  // Setup the page access methods for the current bank
  for(uInt32 address = 0x1200; address < (0x1FF4U & ~mask);
      address += (1 << shift))
  {
    access.directPeekBase = &myImage[offset + (address & 0x0FFF)];
    access.codeAccessBase = &myCodeAccessBase[offset + (address & 0x0FFF)];
    mySystem->setPageAccess(address >> shift, access);
  }
  return myBankChanged = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt16 CartridgeFA2::bank() const
{
  return myCurrentBank;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt16 CartridgeFA2::bankCount() const
{
  return (mySize / 4096);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeFA2::patch(uInt16 address, uInt8 value)
{
  address &= 0x0FFF;

  if(address < 0x0200)
  {
    // Normally, a write to the read port won't do anything
    // However, the patch command is special in that ignores such
    // cart restrictions
    myRAM[address & 0x00FF] = value;
  }
  else
    myImage[(myCurrentBank << 12) + address] = value;

  return myBankChanged = true;
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const uInt8* CartridgeFA2::getImage(int& size) const
{
  size = mySize;
  return myImage;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeFA2::save(Serializer& out) const
{
  try
  {
    out.putString(name());
    out.putShort(myCurrentBank);
    out.putByteArray(myRAM, 256);
  }
  catch(...)
  {
    cerr << "ERROR: CartridgeFA2::save" << endl;
    return false;
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeFA2::load(Serializer& in)
{
  try
  {
    if(in.getString() != name())
      return false;

    myCurrentBank = in.getShort();
    in.getByteArray(myRAM, 256);
  }
  catch(...)
  {
    cerr << "ERROR: CartridgeFA2::load" << endl;
    return false;
  }

  // Remember what bank we were in
  bank(myCurrentBank);

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeFA2::setRomName(const string& name)
{
  myFlashFile = myOSystem.nvramDir() + name + "_flash.dat";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 CartridgeFA2::ramReadWrite()
{
  /* The following algorithm implements accessing Harmony cart flash

    1. Wait for an access to hotspot location $1FF4 (return 1 in bit 6
       while busy).

    2. Read byte 256 of RAM+ memory to determine the operation requested
       (1 = read, 2 = write).

    3. Save or load the entire 256 bytes of RAM+ memory to a file.

    4. Set byte 256 of RAM+ memory to zero to indicate success (will
       always happen in emulation).

    5. Return 0 (in bit 6) on the next access to $1FF4, if enough time has
       passed to complete the operation on a real system (0.5 ms for read,
       101 ms for write).
  */

  // First access sets the timer
  if(myRamAccessTimeout == 0)
  {
    // Remember when the first access was made
    myRamAccessTimeout = myOSystem.getTicks();

    // We go ahead and do the access now, and only return when a sufficient
    // amount of time has passed
    Serializer serializer(myFlashFile);
    if(serializer.isValid())
    {
      if(myRAM[255] == 1)       // read
      {
        try
        {
          serializer.getByteArray(myRAM, 256);
        }
        catch(...)
        {
          memset(myRAM, 0, 256);
        }
        myRamAccessTimeout += 500;  // Add 0.5 ms delay for read
      }
      else if(myRAM[255] == 2)  // write
      {
        try
        {
          serializer.putByteArray(myRAM, 256);
        }
        catch(...)
        {
          // Maybe add logging here that save failed?
          cerr << name() << ": ERROR saving score table" << endl;
        }
        myRamAccessTimeout += 101000;  // Add 101 ms delay for write
      }
    }
    // Bit 6 is 1, busy
    return myImage[(myCurrentBank << 12) + 0xFF4] | 0x40;
  }
  else
  {
    // Have we reached the timeout value yet?
    if(myOSystem.getTicks() >= myRamAccessTimeout)
    {
      myRamAccessTimeout = 0;  // Turn off timer
      myRAM[255] = 0;          // Successful operation

      // Bit 6 is 0, ready/success
      return myImage[(myCurrentBank << 12) + 0xFF4] & ~0x40;
    }
    else
      // Bit 6 is 1, busy
      return myImage[(myCurrentBank << 12) + 0xFF4] | 0x40;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeFA2::flash(uInt8 operation)
{
  Serializer serializer(myFlashFile);
  if(serializer.isValid())
  {
    if(operation == 0)       // erase
    {
      try
      {
        uInt8 buf[256];
        memset(buf, 0, 256);
        serializer.putByteArray(buf, 256);
      }
      catch(...)
      {
      }
    }
    else if(operation == 1)  // read
    {
      try
      {
        serializer.getByteArray(myRAM, 256);
      }
      catch(...)
      {
        memset(myRAM, 0, 256);
      }
    }
    else if(operation == 2)  // write
    {
      try
      {
        serializer.putByteArray(myRAM, 256);
      }
      catch(...)
      {
        // Maybe add logging here that save failed?
        cerr << name() << ": ERROR saving score table" << endl;
      }
    }
  }
}
