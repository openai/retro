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
// $Id: StateManager.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include <sstream>

#include "OSystem.hxx"
#include "Settings.hxx"
#include "Console.hxx"
#include "Cart.hxx"
#include "Control.hxx"
#include "Switches.hxx"
#include "System.hxx"
#include "Serializable.hxx"

#include "StateManager.hxx"

#define STATE_HEADER "03090100state"
#define MOVIE_HEADER "03030000movie"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
StateManager::StateManager(OSystem* osystem)
  : myOSystem(osystem),
    myCurrentSlot(0)
{
  reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
StateManager::~StateManager()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void StateManager::update()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool StateManager::loadState(Serializer& in)
{
  if(&myOSystem->console())
  {
    // Make sure the file can be opened for reading
    if(in.isValid())
    {
      // First test if we have a valid header and cart type
      // If so, do a complete state load using the Console
      return in.getString() == STATE_HEADER &&
             in.getString() == myOSystem->console().cartridge().name() &&
             myOSystem->console().load(in);
    }
  }
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool StateManager::saveState(Serializer& out)
{
  try
  {
    if(&myOSystem->console())
    {
      // Make sure the file can be opened for writing
      if(out.isValid())
      {
        // Add header so that if the state format changes in the future,
        // we'll know right away, without having to parse the rest of the file
        out.putString(STATE_HEADER);

        // Sanity check; prepend the cart type/name
        out.putString(myOSystem->console().cartridge().name());

        // Do a complete state save using the Console
        if(myOSystem->console().save(out))
          return true;
      }
    }
  }
  catch(...)
  {
    cerr << "ERROR: StateManager::saveState(Serializer&)" << endl;
  }
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void StateManager::reset()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
StateManager::StateManager(const StateManager&)
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
StateManager& StateManager::operator = (const StateManager&)
{
  assert(false);
  return *this;
}
