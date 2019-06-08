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
// $Id: StateManager.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef STATE_MANAGER_HXX
#define STATE_MANAGER_HXX

class OSystem;

#include "Serializer.hxx"

/**
  This class provides an interface to all things related to emulation state.
  States can be loaded or saved here, as well as recorded, rewound, and later
  played back.

  @author  Stephen Anthony
  @version $Id: StateManager.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class StateManager
{
  public:
    /**
      Create a new statemananger class
    */
    StateManager(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~StateManager();

  public:
    /**
      Updates the state of the system based on the currently active mode
    */
    void update();

    /**
      Load a state into the current system from the given Serializer.
      No messages are printed to the screen.

      @param in  The Serializer object to use

      @return  False on any load errors, else true
    */
    bool loadState(Serializer& in);

    /**
      Save the current state from the system into the given Serializer.
      No messages are printed to the screen.

      @param out  The Serializer object to use

      @return  False on any save errors, else true
    */
    bool saveState(Serializer& out);

    /**
      Resets manager to defaults
    */
    void reset();

  private:
    // Copy constructor isn't supported by this class so make it private
    StateManager(const StateManager&);

    // Assignment operator isn't supported by this class so make it private
    StateManager& operator = (const StateManager&);

  private:
    enum {
      kVersion = 001
    };

    // The parent OSystem object
    OSystem* myOSystem;

    // The current slot for load/save states
    int myCurrentSlot;

    // MD5 of the currently active ROM (either in movie or rewind mode)
    string myMD5;
};

#endif
