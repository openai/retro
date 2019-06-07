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
// $Id: CartCTY.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef CARTRIDGECHETIRY_HXX
#define CARTRIDGECHETIRY_HXX

class System;

#include "bspf.hxx"
#include "Cart.hxx"
#ifdef DEBUGGER_SUPPORT
  #include "CartCTYWidget.hxx"
#endif

/**
  The 'Chetiry' bankswitch scheme was developed by Chris D. Walton for a
  Tetris clone game by the same name.  It makes use of a Harmony cart,
  whereby ARM code in bank 0 is executed to implement the bankswitch scheme.
  The implementation here does not execute this ARM code, and instead
  implements the bankswitching directly.  Its functionality is similar to
  several other schemes, as follows:

  F4SC:
    The scheme contains 8 4K banks, with the first bank being inaccessible
    (due to containing ARM code).  The remaining banks (1 - 7) are accessed
    at hotspots $FF5 - $FFB, exactly the same as F4SC.

    There is 64 bytes of RAM (vs. 128 bytes in F4SC) at $1000 - $107F
    ($1000 - $103F is write port, $1040 - $107F is read port).

  FA2:
    The first four bytes of RAM are actually a kind of hotspot, with the
    following functionality.  Data is accessed from Harmony EEPROM in
    the same fashion as the FA2 scheme.

    Write Addresses:
      $1000 = Operation Type (see discussion of hotspot $1FF4 below)
      $1001 = Set Random Seed Value
      $1002 = Reset Fetcher To Beginning Of Tune
      $1003 = Advance Fetcher To Next Tune Position

    Read Addresses:
      $1040 = Error Code after operation
      $1041 = Get Next Random Number (8-bit LFSR)
      $1042 = Get Tune Position (Low Byte)
      $1043 = Get Tune Position (High Byte)

    RAM Load And Save Operations:

      Address $1FF4 is used as a special hotspot to trigger loading and saving
      of the RAM, similar to FA2 bankswitching. The operation to perform is
      given using the first byte of the extra RAM. The format of this byte is
      XXXXYYYY, where XXXX is an index and YYYY is the operation to perform.
      There are 4 different operation types:

        1 = Load Tune (index = tune)
        2 = Load Score Table (index = table)
        3 = Save Score Table (index = table)
        4 = Wipe All Score Tables (set all 256 bytes of EEPROM to $00)

      The score table functionality is based on 256 bytes from Harmony
      EEPROM, of which there are 4 64-byte 'tables'.  The 'index' for
      operations 2 and 3 can therefore be in the range 0 - 3, indicating
      which table to use.  For this implementation, the 256 byte EEPROM
      is serialized to a file.

      The tune table functionality is also based on Harmony EEPROM, where
      7 4K tunes are stored (28K total).  The 'index' for operation 1 can
      therefore be in the range 0 - 6, indicating which tune to load.
      For this implementation, the 28K tune data is in the 'CartCTYTunes'
      header file.

  DPC+:
    The music functionality is quite similar to the DPC+ scheme.
    
    Fast Fetcher
      The music frequency value is fetched using a fast fetcher operation.
      This operation is aliased to the instruction "LDA #$F2". Whenever this
      instruction is executed, the $F2 value is replaced with the frequency
      value calculated from the tune data. The pointer to the tune data does
      not advance until address $1003 is written. When a new tune is loaded,
      the pointer is reset to the beginning of the tune. This also happens
      when the end of the tune is reached or when address $1002 is written to.

      The calculation of the frequency value is essentially the same as DPC.
      There are 3 different channels that are combined together, and only a
      square waveform is used.  The data is formatted so that the three notes
      for each position appear consecutively (note0, note1, note2).  Moving
      to the next tune position means incrementing by 3 bytes. The end of the
      tune is marked by a note value of 1. A note value of 0 means that the
      current value should not be updated, i.e continue with the previous
      non-zero value.

  @author  Stephen Anthony and Chris D. Walton
  @version $Id: CartCTY.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class CartridgeCTY : public Cartridge
{
  friend class CartridgeCTYWidget;

  public:
    /**
      Create a new cartridge using the specified image

      @param image     Pointer to the ROM image
      @param size      The size of the ROM image
      @param osystem   A reference to the OSystem currently in use
    */
    CartridgeCTY(const uInt8* image, uInt32 size, const OSystem& osystem);
 
    /**
      Destructor
    */
    virtual ~CartridgeCTY();

  public:
    /**
      Reset device to its power-on state
    */
    void reset();

    /**
      Notification method invoked by the system right before the
      system resets its cycle counter to zero.  It may be necessary
      to override this method for devices that remember cycle counts.
    */
    void systemCyclesReset();

    /**
      Install cartridge in the specified system.  Invoked by the system
      when the cartridge is attached to it.

      @param system The system the device should install itself in
    */
    void install(System& system);

    /**
      Install pages for the specified bank in the system.

      @param bank The bank that should be installed in the system
    */
    bool bank(uInt16 bank);

    /**
      Get the current bank.
    */
    uInt16 bank() const;

    /**
      Query the number of banks supported by the cartridge.
    */
    uInt16 bankCount() const;

    /**
      Patch the cartridge ROM.

      @param address  The ROM address to patch
      @param value    The value to place into the address
      @return    Success or failure of the patch operation
    */
    bool patch(uInt16 address, uInt8 value);

    /**
      Access the internal ROM image for this cartridge.

      @param size  Set to the size of the internal ROM image data
      @return  A pointer to the internal ROM image data
    */
    const uInt8* getImage(int& size) const;

    /**
      Save the current state of this cart to the given Serializer.

      @param out  The Serializer object to use
      @return  False on any errors, else true
    */
    bool save(Serializer& out) const;

    /**
      Load the current state of this cart from the given Serializer.

      @param in  The Serializer object to use
      @return  False on any errors, else true
    */
    bool load(Serializer& in);

    /**
      Get a descriptor for the device name (used in error checking).

      @return The name of the object
    */
    string name() const { return "CartridgeCTY"; }

    /**
      Informs the cartridge about the name of the ROM file used when
      creating this cart.

      @param name  The properties file name of the ROM
    */
    void setRomName(const string& name);

  #ifdef DEBUGGER_SUPPORT
    /**
      Get debugger widget responsible for accessing the inner workings
      of the cart.
    */
    CartDebugWidget* debugWidget(GuiObject* boss, const GUI::Font& lfont,
        const GUI::Font& nfont, int x, int y, int w, int h)
    {
      return new CartridgeCTYWidget(boss, lfont, nfont, x, y, w, h, *this);
    }
  #endif

  public:
    /**
      Get the byte at the specified address.

      @return The byte at the specified address
    */
    uInt8 peek(uInt16 address);

    /**
      Change the byte at the specified address to the given value

      @param address The address where the value should be stored
      @param value The value to be stored at the address
      @return  True if the poke changed the device address space, else false
    */
    bool poke(uInt16 address, uInt8 value);

  private:
    /**
      Either load or save internal RAM to Harmony EEPROM (represented by
      a file in emulation).

      @return  The value at $FF4 with bit 6 set or cleared (depending on
               whether the RAM access was busy or successful)
    */
    uInt8 ramReadWrite();

    /**
      Actions initiated by accessing $FF4 hotspot.
    */
    void loadTune(uInt8 index);
    void loadScore(uInt8 index);
    void saveScore(uInt8 index);
    void wipeAllScores();

    /** 
      Updates any data fetchers in music mode based on the number of
      CPU cycles which have passed since the last update.
    */
    void updateMusicModeDataFetchers();

  private:
    // OSsytem currently in use
    const OSystem& myOSystem;

    // Indicates which bank is currently active
    uInt16 myCurrentBank;

    // The 32K ROM image of the cartridge
    uInt8 myImage[32768];

    // The 64 bytes of RAM accessible at $1000 - $1080
    uInt8 myRAM[64];

    // Operation type (written to $1000, used by hotspot $1FF4)
    uInt8 myOperationType;

    // Pointer to the 28K frequency table (points to the start of one
    // of seven 4K tunes in CartCTYTunes)
    const uInt8* myFrequencyImage;

    // The counter register for the data fetcher
    uInt16 myCounter;

    // Flags that last byte peeked was A9 (LDA #)
    bool myLDAimmediate;

    // The random number generator register
    uInt32 myRandomNumber;

    // The time after which the first request of a load/save operation
    // will actually be completed
    // Due to Harmony EEPROM constraints, a read/write isn't instantaneous,
    // so we need to emulate the delay as well
    uInt64 myRamAccessTimeout;

    // Full pathname of the file to use when emulating load/save
    // of internal RAM to Harmony cart EEPROM
    string myEEPROMFile;

    // System cycle count when the last update to music data fetchers occurred
    Int32 mySystemCycles;

    // Fractional DPC music OSC clocks unused during the last update
    double myFractionalClocks;
};

#endif
