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
// $Id: Control.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef CONTROLLER_HXX
#define CONTROLLER_HXX

class Controller;
class Event;
class System;

#include "Serializable.hxx"
#include "bspf.hxx"

/**
  A controller is a device that plugs into either the left or right 
  controller jack of the Video Computer System (VCS).  The pins of 
  the controller jacks are mapped as follows:

                           -------------
                           \ 1 2 3 4 5 /
                            \ 6 7 8 9 /
                             ---------

            Left Controller             Right Controller

    pin 1   D4  PIA SWCHA               D0  PIA SWCHA
    pin 2   D5  PIA SWCHA               D1  PIA SWCHA
    pin 3   D6  PIA SWCHA               D2  PIA SWCHA
    pin 4   D7  PIA SWCHA               D3  PIA SWCHA
    pin 5   D7  TIA INPT1 (Dumped)      D7  TIA INPT3 (Dumped)
    pin 6   D7  TIA INPT4 (Latched)     D7  TIA INPT5 (Latched)
    pin 7   +5                          +5
    pin 8   GND                         GND
    pin 9   D7  TIA INPT0 (Dumped)      D7  TIA INPT2 (Dumped)

  Each of the pins connected to the PIA can be configured as an
  input or output pin.  The "dumped" TIA pins are used to charge
  a capacitor.  A potentiometer is sometimes connected to these
  pins for analog input.

  This is a base class for all controllers.  It provides a view
  of the controller from the perspective of the controller's jack.

  @author  Bradford W. Mott
  @version $Id: Control.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class Controller : public Serializable
{
  /**
    Riot debug class needs special access to the underlying controller state
  */
  friend class M6532;
  friend class RiotDebug;
  friend class CompuMate;

  public:
    /**
      Enumeration of the controller jacks
    */
    enum Jack { Left = 0, Right = 1 };

    /**
      Enumeration of the digital pins of a controller port
    */
    enum DigitalPin { One, Two, Three, Four, Six };

    /**
      Enumeration of the analog pins of a controller port
    */
    enum AnalogPin { Five, Nine };

    /**
      Enumeration of the controller types
    */
    enum Type
    {
      BoosterGrip, Driving, Keyboard, Paddles, Joystick,
      TrackBall22, TrackBall80, AmigaMouse, AtariVox, SaveKey,
      KidVid, Genesis, MindLink, CompuMate
    };

  public:
    /**
      Create a new controller plugged into the specified jack

      @param jack   The jack the controller is plugged into
      @param event  The event object to use for events
      @param type   The type for this controller
      @param system The system using this controller
    */
    Controller(Jack jack, const Event& event, const System& system,
               Type type);
 
    /**
      Destructor
    */
    virtual ~Controller();

    /**
      Returns the jack that this controller is plugged into.
    */
    Jack jack() const { return myJack; }

    /**
      Returns the type of this controller.
    */
    Type type() const { return myType; }

    /**
      Read the entire state of all digital pins for this controller.
      Note that this method must use the lower 4 bits, and zero the upper bits.

      @return The state of all digital pins
    */
    virtual uInt8 read();

    /**
      Read the value of the specified digital pin for this controller.

      @param pin The pin of the controller jack to read
      @return The state of the pin
    */
    virtual bool read(DigitalPin pin);

    /**
      Read the resistance at the specified analog pin for this controller.  
      The returned value is the resistance measured in ohms.

      @param pin The pin of the controller jack to read
      @return The resistance at the specified pin
    */
    virtual Int32 read(AnalogPin pin);

    /**
      Write the given value to the specified digital pin for this 
      controller.  Writing is only allowed to the pins associated 
      with the PIA.  Therefore you cannot write to pin six.

      @param pin The pin of the controller jack to write to
      @param value The value to write to the pin
    */
    virtual void write(DigitalPin pin, bool value) { };

    /**
      Called after *all* digital pins have been written on Port A.
      Most controllers don't do anything in this case.

      @param value  The entire contents of the SWCHA register
    */
    virtual void controlWrite(uInt8 value) { };

    /**
      Update the entire digital and analog pin state according to the
      events currently set.
    */
    virtual void update() = 0;

    /**
      Notification method invoked by the system right before the
      system resets its cycle counter to zero.  It may be necessary 
      to override this method for controllers that remember cycle counts.
    */
    virtual void systemCyclesReset() { };

    /**
      Determines how this controller will treat values received from the
      X/Y axis and left/right buttons of the mouse.  Since not all controllers
      use the mouse the same way (or at all), it's up to the specific class to
      decide how to use this data.

      In the current implementation, the left button is tied to the X axis,
      and the right one tied to the Y axis.

      @param xtype  The controller to use for x-axis data
      @param xid    The controller ID to use for x-axis data (-1 for no id)
      @param ytype  The controller to use for y-axis data
      @param yid    The controller ID to use for y-axis data (-1 for no id)

      @return  Whether the controller supports using the mouse
    */
    virtual bool setMouseControl(
      Controller::Type xtype, int xid, Controller::Type ytype, int yid)
    { return false; }

    /**
      Returns the name of this controller.
    */
    virtual string name() const;

    /**
      Returns more detailed information about this controller.
    */
    virtual string about() const;

    /**
      The following two functions are used by the debugger to set
      the specified pins to the given value.  Note that this isn't the
      same as a write; the debugger is allowed special access and is
      actually 'below' the controller level.

      @param pin The pin of the controller jack to modify
      @param value The value to set on the pin
    */
    void set(DigitalPin pin, bool value);
    void set(AnalogPin pin, Int32 value);

    /**
      Saves the current state of this controller to the given Serializer.

      @param out The serializer device to save to.
      @return The result of the save.  True on success, false on failure.
    */
    bool save(Serializer& out) const;

    /**
      Loads the current state of this controller from the given Serializer.

      @param in The serializer device to load from.
      @return The result of the load.  True on success, false on failure.
    */
    bool load(Serializer& in);

  public:
    /// Constant which represents maximum resistance for analog pins
    static const Int32 maximumResistance;

    /// Constant which represents minimum resistance for analog pins
    static const Int32 minimumResistance;

  protected:
    /// Specifies which jack the controller is plugged in
    const Jack myJack;

    /// Reference to the event object this controller uses
    const Event& myEvent;

    /// Pointer to the System object (used for timing purposes)
    const System& mySystem;

    /// Specifies which type of controller this is (defined by child classes)
    const Type myType;

    /// Specifies the name of this controller based on type
    string myName;

    /// The boolean value on each digital pin
    bool myDigitalPinState[5];

    /// The analog value on each analog pin
    Int32 myAnalogPinValue[2];

  protected:
    // Copy constructor isn't supported by controllers so make it private
    Controller(const Controller&);

    // Assignment operator isn't supported by controllers so make it private
    Controller& operator = (const Controller&);
};

#endif
