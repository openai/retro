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
// $Id: CartCTY.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <cassert>
#include <cstring>

#include "OSystem.hxx"
#include "Serializer.hxx"
#include "System.hxx"
#include "CartCTYTunes.hxx"
#include "CartCTY.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeCTY::CartridgeCTY(const uInt8* image, uInt32 size, const OSystem& osystem)
  : Cartridge(osystem.settings()),
    myOSystem(osystem),
    myOperationType(0),
    myCounter(0),
    myLDAimmediate(false),
    myRandomNumber(0x2B435044),
    myRamAccessTimeout(0),
    mySystemCycles(0),
    myFractionalClocks(0.0)
{
  // Copy the ROM image into my buffer
  memcpy(myImage, image, MIN(32768u, size));
  createCodeAccessBase(32768);

  // This cart contains 64 bytes extended RAM @ 0x1000
  registerRamArea(0x1000, 64, 0x40, 0x00);

  // Point to the first tune
  myFrequencyImage = CartCTYTunes;

  // Remember startup bank (not bank 0, since that's ARM code)
  myStartBank = 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeCTY::~CartridgeCTY()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::reset()
{
  // Initialize RAM
  if(mySettings.getBool("ramrandom"))
    for(uInt32 i = 0; i < 64; ++i)
      myRAM[i] = mySystem->randGenerator().next();
  else
    memset(myRAM, 0, 64);

  myRAM[0] = myRAM[1] = myRAM[2] = myRAM[3] = 0xFF;

  // Update cycles to the current system cycles
  mySystemCycles = mySystem->cycles();
  myFractionalClocks = 0.0;

  // Upon reset we switch to the startup bank
  bank(myStartBank);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::systemCyclesReset()
{
  // Adjust the cycle counter so that it reflects the new value
  mySystemCycles -= mySystem->cycles();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::install(System& system)
{
  mySystem = &system;
  uInt16 mask = mySystem->pageMask();
  uInt16 shift = mySystem->pageShift();

  // Make sure the system we're being installed in has a page size that'll work
  assert(((0x1000 & mask) == 0) && ((0x1080 & mask) == 0));

  // Map all RAM accesses to call peek and poke
  System::PageAccess access(0, 0, 0, this, System::PA_READ);
  for(uInt32 i = 0x1000; i < 0x1080; i += (1 << shift))
    mySystem->setPageAccess(i >> shift, access);

  // Install pages for the startup bank
  bank(myStartBank);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 CartridgeCTY::peek(uInt16 address)
{
  uInt16 peekAddress = address;
  address &= 0x0FFF;
  uInt8 peekValue = myImage[myCurrentBank + address];

  // In debugger/bank-locked mode, we ignore all hotspots and in general
  // anything that can change the internal state of the cart
  if(bankLocked())
    return peekValue;

  // Check for aliasing to 'LDA #$F2'
  if(myLDAimmediate && peekValue == 0xF2)
  {
    myLDAimmediate = false;

    // Update the music data fetchers (counter & flag)
    updateMusicModeDataFetchers();

#if 0
    // using myDisplayImage[] instead of myProgramImage[] because waveforms
    // can be modified during runtime.
    uInt32 i = myDisplayImage[(myMusicWaveforms[0] << 5) + (myMusicCounters[0] >> 27)] +
               myDisplayImage[(myMusicWaveforms[1] << 5) + (myMusicCounters[1] >> 27)] +
               myDisplayImage[(myMusicWaveforms[2] << 5) + (myMusicCounters[2] >> 27)];
    return = (uInt8)i;
#endif
    return 0xF2;  // FIXME - return frequency value here
  }
  else
    myLDAimmediate = false;

  if(address < 0x0040)  // Write port is at $1000 - $103F (64 bytes)
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
  else if(address < 0x0080)  // Read port is at $1040 - $107F (64 bytes)
  {
    address -= 0x40;
    switch(address)
    {
      case 0x00:  // Error code after operation
        return myRAM[0];
      case 0x01:  // Get next Random Number (8-bit LFSR)
        myRandomNumber = ((myRandomNumber & (1<<10)) ? 0x10adab1e: 0x00) ^
                         ((myRandomNumber >> 11) | (myRandomNumber << 21));
        return myRandomNumber & 0xFF;
      case 0x02:  // Get Tune position (low byte)
        return myCounter & 0xFF;
      case 0x03:  // Get Tune position (high byte)
        return (myCounter >> 8) & 0xFF;
      default:
        return myRAM[address];
    }
  }
  else  // Check hotspots
  {
    switch(address)
    {
      case 0x0FF4:
        // Bank 0 is ARM code and not actually accessed
        return ramReadWrite();
      case 0x0FF5:
      case 0x0FF6:
      case 0x0FF7:
      case 0x0FF8:
      case 0x0FF9:
      case 0x0FFA:
      case 0x0FFB:
        // Banks 1 through 7
        bank(address - 0x0FF4);
        break;
      default:
        break;
    }

    // Is this instruction an immediate mode LDA?
    myLDAimmediate = (peekValue == 0xA9);

    return peekValue;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCTY::poke(uInt16 address, uInt8 value)
{
  address &= 0x0FFF;

//cerr << "POKE: address=" << HEX4 << address << ", value=" << HEX2 << value << endl;
  if(address < 0x0040)  // Write port is at $1000 - $103F (64 bytes)
  {
    switch(address)  // FIXME for functionality
    {
      case 0x00:  // Operation type for $1FF4
        myOperationType = value;
        break;
      case 0x01:  // Set Random seed value (reset)
        myRandomNumber = 0x2B435044;
        break;
      case 0x02:  // Reset fetcher to beginning of tune
        myCounter = 0;
        break;
      case 0x03:  // Advance fetcher to next tune position
        myCounter = (myCounter + 3) & 0x0fff;
        break;
      default:
        myRAM[address] = value;
        break;
    }
  }
  else  // Check hotspots
  {
    switch(address)
    {
      case 0x0FF4:
        // Bank 0 is ARM code and not actually accessed
        ramReadWrite();
        break;
      case 0x0FF5:
      case 0x0FF6:
      case 0x0FF7:
      case 0x0FF8:
      case 0x0FF9:
      case 0x0FFA:
      case 0x0FFB:
        // Banks 1 through 7
        bank(address - 0x0FF4);
        break;
      default:
        break;
    }
  }
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCTY::bank(uInt16 bank)
{ 
  if(bankLocked()) return false;

  // Remember what bank we're in
  myCurrentBank = bank << 12;
  uInt16 shift = mySystem->pageShift();

  // Setup the page access methods for the current bank
  System::PageAccess access(0, 0, 0, this, System::PA_READ);
  for(uInt32 address = 0x1080; address < 0x2000; address += (1 << shift))
  {
    access.codeAccessBase = &myCodeAccessBase[myCurrentBank + (address & 0x0FFF)];
    mySystem->setPageAccess(address >> shift, access);
  }
  return myBankChanged = true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt16 CartridgeCTY::bank() const
{
  return myCurrentBank >> 12;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt16 CartridgeCTY::bankCount() const
{
  return 8;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCTY::patch(uInt16 address, uInt8 value)
{
  address &= 0x0FFF;

  if(address < 0x0080)
  {
    // Normally, a write to the read port won't do anything
    // However, the patch command is special in that ignores such
    // cart restrictions
    myRAM[address & 0x003F] = value;
  }
  else
    myImage[myCurrentBank + address] = value;

  return myBankChanged = true;
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const uInt8* CartridgeCTY::getImage(int& size) const
{
  size = 32768;
  return myImage;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCTY::save(Serializer& out) const
{
   out.putString(name());
   out.putShort(bank());
   out.putByteArray(myRAM, 64);

   out.putByte(myOperationType);
   out.putShort(myCounter);
   out.putBool(myLDAimmediate);
   out.putInt(myRandomNumber);
   out.putInt(mySystemCycles);
   out.putInt((uInt32)(myFractionalClocks * 100000000.0));

   return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeCTY::load(Serializer& in)
{
   if(in.getString() != name())
      return false;

   // Remember what bank we were in
   bank(in.getShort());
   in.getByteArray(myRAM, 64);

   myOperationType = in.getByte();
   myCounter = in.getShort();
   myLDAimmediate = in.getBool();
   myRandomNumber = in.getInt();
   mySystemCycles = (Int32)in.getInt();
   myFractionalClocks = (double)in.getInt() / 100000000.0;

   return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::setRomName(const string& name)
{
  myEEPROMFile = myOSystem.nvramDir() + name + "_eeprom.dat";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 CartridgeCTY::ramReadWrite()
{
  /* The following algorithm implements accessing Harmony cart EEPROM

    1. Wait for an access to hotspot location $1FF4 (return 1 in bit 6
       while busy).

    2. Determine operation from myOperationType.

    3. Save or load relevant EEPROM memory to/from a file.

    4. Set byte 0 of RAM+ memory to zero to indicate success (will
       always happen in emulation).

    5. Return 0 (in bit 6) on the next access to $1FF4, if enough time has
       passed to complete the operation on a real system (0.5 s for read,
       1 s for write).
  */

  if(bankLocked()) return 0xff;

  // First access sets the timer
  if(myRamAccessTimeout == 0)
  {
    // Opcode and value in form of XXXXYYYY (from myOperationType), where:
    //    XXXX = index and YYYY = operation
    uInt8 index = myOperationType >> 4;
    switch(myOperationType & 0xf)
    {
      case 1:  // Load tune (index = tune)
        if(index < 7)
        {
          // Add 0.5 s delay for read
          myRamAccessTimeout = myOSystem.getTicks() + 500000;
          loadTune(index);
        }
        break;
      case 2:  // Load score table (index = table)
        if(index < 4)
        {
          // Add 0.5 s delay for read
          myRamAccessTimeout = myOSystem.getTicks() + 500000;
          loadScore(index);
        }
        break;
      case 3:  // Save score table (index = table)
        if(index < 4)
        {
          // Add 1 s delay for write
          myRamAccessTimeout = myOSystem.getTicks() + 1000000;
          saveScore(index);
        }
        break;
      case 4:  // Wipe all score tables
        // Add 1 s delay for write
        myRamAccessTimeout = myOSystem.getTicks() + 1000000;
        wipeAllScores();
        break;
    }
    // Bit 6 is 1, busy
    return myImage[myCurrentBank + 0xFF4] | 0x40;
  }
  else
  {
    // Have we reached the timeout value yet?
    if(myOSystem.getTicks() >= myRamAccessTimeout)
    {
      myRamAccessTimeout = 0;  // Turn off timer
      myRAM[0] = 0;            // Successful operation

      // Bit 6 is 0, ready/success
      return myImage[myCurrentBank + 0xFF4] & ~0x40;
    }
    else
      // Bit 6 is 1, busy
      return myImage[myCurrentBank + 0xFF4] | 0x40;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::loadTune(uInt8 index)
{
  // Each tune is offset by 4096 bytes
  // Instead of copying non-modifiable data around (as would happen on the
  // Harmony), we simply point to the appropriate tune
  myFrequencyImage = CartCTYTunes + (index << 12);

  // Reset to beginning of tune
  myCounter = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::loadScore(uInt8 index)
{
  Serializer serializer(myEEPROMFile, true);
  if(serializer.isValid())
  {
    uInt8 scoreRAM[256];
    try
    {
      serializer.getByteArray(scoreRAM, 256);
    }
    catch(...)
    {
      memset(scoreRAM, 0, 256);
    }
    // Grab 60B slice @ given index (first 4 bytes are ignored)
    memcpy(myRAM+4, scoreRAM + (index << 6) + 4, 60);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::saveScore(uInt8 index)
{
  Serializer serializer(myEEPROMFile);
  if(serializer.isValid())
  {
    // Load score RAM
    uInt8 scoreRAM[256];
    try
    {
      serializer.getByteArray(scoreRAM, 256);
    }
    catch(...)
    {
      memset(scoreRAM, 0, 256);
    }

    // Add 60B RAM to score table @ given index (first 4 bytes are ignored)
    memcpy(scoreRAM + (index << 6) + 4, myRAM+4, 60);

    // Save score RAM
    serializer.reset();
    try
    {
      serializer.putByteArray(scoreRAM, 256);
    }
    catch(...)
    {
      // Maybe add logging here that save failed?
      cerr << name() << ": ERROR saving score table " << (int)index << endl;
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeCTY::wipeAllScores()
{
  Serializer serializer(myEEPROMFile);
  if(serializer.isValid())
  {
    // Erase score RAM
    uInt8 scoreRAM[256];
    memset(scoreRAM, 0, 256);
    try
    {
      serializer.putByteArray(scoreRAM, 256);
    }
    catch(...)
    {
      // Maybe add logging here that save failed?
      cerr << name() << ": ERROR wiping score tables" << endl;
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
inline void CartridgeCTY::updateMusicModeDataFetchers()
{
  // Calculate the number of cycles since the last update
  Int32 cycles = mySystem->cycles() - mySystemCycles;
  mySystemCycles = mySystem->cycles();

  // Calculate the number of DPC OSC clocks since the last update
  double clocks = ((20000.0 * cycles) / 1193191.66666667) + myFractionalClocks;
  Int32 wholeClocks = (Int32)clocks;
  myFractionalClocks = clocks - (double)wholeClocks;

  if(wholeClocks <= 0)
    return;

  // Let's update counters and flags of the music mode data fetchers
  for(int x = 0; x <= 2; ++x)
  {
//    myMusicCounters[x] += myMusicFrequencies[x];
  }
}
