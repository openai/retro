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
// $Id: M6532.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef M6532_HXX
#define M6532_HXX

class Console;
class RiotDebug;
class Settings;

#include "bspf.hxx"
#include "Device.hxx"
#include "System.hxx"

/**
  This class models the M6532 RAM-I/O-Timer (aka RIOT) chip in the 2600
  console.  Note that since the M6507 CPU doesn't contain an interrupt line,
  the following functionality relating to the RIOT IRQ line is not emulated:

    - A3 to enable/disable interrupt from timer to IRQ
    - A1 to enable/disable interrupt from PA7 to IRQ

  @author  Bradford W. Mott and Stephen Anthony
  @version $Id: M6532.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class M6532 : public Device
{
  public:
    /**
      The RIOT debugger class is a friend who needs special access
    */
    friend class RiotDebug;

  public:
    /**
      Create a new 6532 for the specified console

      @param console  The console the 6532 is associated with
      @param settings The settings used by the system
    */
    M6532(const Console& console, const Settings& settings);
 
    /**
      Destructor
    */
    virtual ~M6532();

   public:
    /**
      Reset cartridge to its power-on state
    */
    void reset();

    /**
      Notification method invoked by the system right before the
      system resets its cycle counter to zero.  It may be necessary
      to override this method for devices that remember cycle counts.
    */
    void systemCyclesReset();

    /**
      Update the entire digital and analog pin state of ports A and B.
    */
    void update();

    /**
      Install 6532 in the specified system.  Invoked by the system
      when the 6532 is attached to it.

      @param system The system the device should install itself in
    */
    void install(System& system);

    /**
      Install 6532 in the specified system and device.  Invoked by
      the system when the 6532 is attached to it.  All devices
      which invoke this method take responsibility for chaining
      requests back to *this* device.

      @param system The system the device should install itself in
      @param device The device responsible for this address space
    */
    void install(System& system, Device& device);

    /**
      Save the current state of this device to the given Serializer.

      @param out  The Serializer object to use
      @return  False on any errors, else true
    */
    bool save(Serializer& out) const;

    /**
      Load the current state of this device from the given Serializer.

      @param in  The Serializer object to use
      @return  False on any errors, else true
    */
    bool load(Serializer& in);

    /**
      Get a descriptor for the device name (used in error checking).

      @return The name of the object
    */
    string name() const { return "M6532"; }

   public:
    /**
      Get the byte at the specified address

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
    Int32 timerClocks() const
      { return myTimer - (mySystem->cycles() - myCyclesWhenTimerSet); }

    void setTimerRegister(uInt8 data, uInt8 interval);
    void setPinState(bool shcha);

    // The following are used by the debugger to read INTIM/TIMINT
    // We need separate methods to do this, so the state of the system
    // isn't changed
    uInt8 intim() const;
    uInt8 timint() const;
    Int32 intimClocks() const;

  private:
    // Accessible bits in the interrupt flag register
    // All other bits are always zeroed
    enum {
      TimerBit = 0x80,
      PA7Bit = 0x40
    };

    // Reference to the console
    const Console& myConsole;

    // Reference to the settings
    const Settings& mySettings;

    // An amazing 128 bytes of RAM
    uInt8 myRAM[128];

    // Current value of the timer
    uInt32 myTimer;

    // Log base 2 of the number of cycles in a timer interval
    uInt32 myIntervalShift;

    // Indicates the number of cycles when the timer was last set
    Int32 myCyclesWhenTimerSet;

    // Data Direction Register for Port A
    uInt8 myDDRA;

    // Data Direction Register for Port B
    uInt8 myDDRB;

    // Last value written to Port A
    uInt8 myOutA;

    // Last value written to Port B
    uInt8 myOutB;

    // Interrupt Flag Register
    uInt8 myInterruptFlag;

    // Whether the timer flag (as currently set) can be used
    // If it isn't valid, it will be updated as required
    bool myTimerFlagValid;

    // Used to determine whether an active transition on PA7 has occurred
    // True is positive edge-detect, false is negative edge-detect
    bool myEdgeDetectPositive;

    // Last value written to the timer registers
    uInt8 myOutTimer[4];

  private:
    // Copy constructor isn't supported by this class so make it private
    M6532(const M6532&);
 
    // Assignment operator isn't supported by this class so make it private
    M6532& operator = (const M6532&);

  public: uInt8* getRAM() { return myRAM; }
};

#endif
