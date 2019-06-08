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
// $Id: CompuMate.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef COMPUMATE_HXX
#define COMPUMATE_HXX

#include "bspf.hxx"
#include "CartCM.hxx"
#include "Control.hxx"
#include "Event.hxx"

/**
  Handler for SpectraVideo CompuMate bankswitched games.

  The specifics of the CompuMate format can be found in both the Cart side
  (CartCM) and the Controller side (CMControl).  The CompuMate device is
  unique for the 2600 in that it requires close co-operation between the
  cartridge and the left and right controllers.

  This class acts as a 'parent' for cartridge and both the left and right
  CMControl's, taking care of their creation and communication between them.
  It also allows to enable/disable the users actual keyboard when required.

  @author  Stephen Anthony
  @version $Id: CompuMate.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class CompuMate
{
  public:
    /**
      Create a new CompuMate handler for both left and right ports.
      Note that this class creates CMControl controllers for both ports,
      but does not take responsibility for their deletion.

      @param cart   The CompuMate cartridge
      @param event  The event object to use for events
      @param system The system using this controller
    */
    CompuMate(CartridgeCM& cart, const Event& event, const System& system);

    /**
      Destructor
      Controllers are deleted outside this class
    */
    virtual ~CompuMate() { }

    /**
      Return the left and right CompuMate controllers
    */
    Controller* leftController() { return myLeftController; }
    Controller* rightController() { return myRightController; }

    /**
      In normal key-handling mode, the update handler receives key events
      from the keyboard.  This is meant to be used during emulation.

      Otherwise, the update handler ignores keys from the keyboard and uses
      its own internal buffer, which essentially can only be set directly
      within the class itself (by the debugger).

      This is necessary since Stella is otherwise event-based, whereas
      reading from the keyboard (in the current code) bypasses the event
      system.  This leads to issues where typing commands in the debugger
      would then be processed by the update handler as if they were
      entered on the CompuMate keyboard.
    */
    void enableKeyHandling(bool enable);

  private:
    /**
      Called by the controller(s) when all pins have been written
      This method keeps track of consecutive calls, and only updates once
    */
    void update();

    // The actual CompuMate controller
    // More information about these scheme can be found in CartCM.hxx
    class CMControl : public Controller
    {
      public:
        /**
          Create a new CMControl controller plugged into the specified jack

          @param handler  Class which coordinates between left & right controllers
          @param jack     The jack the controller is plugged into
          @param event    The event object to use for events
          @param system   The system using this controller
        */
        CMControl(class CompuMate& handler, Controller::Jack jack, const Event& event,
                  const System& system)
          : Controller(jack, event, system, Controller::CompuMate),
            myHandler(handler)
        { }

        /**
          Destructor
        */
        virtual ~CMControl() { }

      public:
        /**
          Called after *all* digital pins have been written on Port A.

          @param value  The entire contents of the SWCHA register
        */
        void controlWrite(uInt8) { myHandler.update(); }

        /**
          Update the entire digital and analog pin state according to the
          events currently set.
        */
        void update() { }

      private:
        class CompuMate& myHandler;
    };

  private:
    // Cart, Event and System objects
    CartridgeCM& myCart;
    const Event& myEvent;
    const System& mySystem;

    // Left and right controllers
    CMControl *myLeftController, *myRightController;

    // The keyboard state array (tells us the current state of the keyboard)
    const bool* myKeyTable;

    // Array of keyboard key states when in the debugger (the normal keyboard
    // keys are ignored in such a case)
    bool myInternalKeyTable[KBDK_LAST];

    // System cycle at which the update() method is called
    // Multiple calls at the same cycle should be ignored
    uInt32 myCycleAtLastUpdate;
};

#endif
