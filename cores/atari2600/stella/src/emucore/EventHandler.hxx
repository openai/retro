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
// $Id: EventHandler.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef EVENTHANDLER_HXX
#define EVENTHANDLER_HXX

#include <map>

class Console;
class OSystem;
class DialogContainer;
class EventMappingWidget;
class MouseControl;
class StringList;
class VariantList;

#include "Array.hxx"
#include "Event.hxx"
#include "StellaKeys.hxx"
#include "bspf.hxx"

enum MouseButton {
  EVENT_LBUTTONDOWN,
  EVENT_LBUTTONUP,
  EVENT_RBUTTONDOWN,
  EVENT_RBUTTONUP,
  EVENT_WHEELDOWN,
  EVENT_WHEELUP
};

enum JoyHat {
  EVENT_HATUP     = 0,  // make sure these are set correctly,
  EVENT_HATDOWN   = 1,  // since they'll be used as array indices
  EVENT_HATLEFT   = 2,
  EVENT_HATRIGHT  = 3,
  EVENT_HATCENTER = 4
};

enum EventMode {
  kEmulationMode = 0,  // make sure these are set correctly,
  kMenuMode      = 1,  // since they'll be used as array indices
  kNumModes      = 2
};

/**
  This class takes care of event remapping and dispatching for the
  Stella core, as well as keeping track of the current 'mode'.

  The frontend will send translated events here, and the handler will
  check to see what the current 'mode' is.

  If in emulation mode, events received from the frontend are remapped and
  sent to the emulation core.  If in menu mode, the events are sent
  unchanged to the menu class, where (among other things) changing key
  mapping can take place.

  @author  Stephen Anthony
  @version $Id: EventHandler.hxx 2838 2014-01-17 23:34:03Z stephena $
*/
class EventHandler
{
  public:
    /**
      Create a new event handler object
    */
    EventHandler(OSystem* osystem);
 
    /**
      Destructor
    */
    virtual ~EventHandler();

    // Enumeration representing the different states of operation
    enum State {
      S_NONE,
      S_EMULATE,
      S_PAUSE,
      S_LAUNCHER,
      S_MENU,
      S_CMDMENU,
      S_DEBUGGER
    };

    /**
      Returns the event object associated with this handler class.

      @return The event object
    */
    Event& event() { return myEvent; }

    /**
      Initialize state of this eventhandler.
    */
    void initialize();

    /**
      Set up any joysticks on the system.  This must be called *after* the
      framebuffer has been created, since SDL requires the video to be
      intialized before joysticks can be probed.
    */
    void setupJoysticks();

    /**
      Maps the given Stelladaptor/2600-daptor(s) to specified ports on a real 2600.

      @param saport  How to map the ports ('lr' or 'rl')
    */
    void mapStelladaptors(const string& saport);

    /**
      Swaps the ordering of Stelladaptor/2600-daptor(s) devices.
    */
    void toggleSAPortOrder();

    /**
      Collects and dispatches any pending events.  This method should be
      called regularly (at X times per second, where X is the game framerate).

      @param time  The current time in microseconds.
    */
    void poll(uInt64 time);

    /**
      Returns the current state of the EventHandler

      @return The State type
    */
//    State state() const { return myState; }

    /**
      Resets the state machine of the EventHandler to the defaults

      @param state  The current state to set
    */
    void reset(State state);

    /**
      This method indicates that the system should terminate.
    */
    void quit() { handleEvent(Event::Quit, 1); }

    /**
      Sets the mouse axes and buttons to act as the controller specified in
      the ROM properties, otherwise disable mouse control completely

      @param enable  Whether to use the mouse to emulate controllers
                     Currently, this will be one of the following values:
                       'always', 'analog', 'never'
    */
    void setMouseControllerMode(const string& enable);

    /**
      Set the number of seconds between taking a snapshot in
      continuous snapshot mode.  Setting an interval of 0 disables
      continuous snapshots.

      @param interval  Interval in seconds between snapshots
    */
    void setContinuousSnapshots(uInt32 interval);

//    inline bool kbdAlt(int mod) const
//    {
//  #ifndef MAC_OSX
//      return (mod & KMOD_ALT);
//  #else
//      return (mod & KMOD_META);
//  #endif
//    }
//
//    inline bool kbdControl(int mod) const
//    {
//      return (mod & KMOD_CTRL) > 0;
//    }
//
//    inline bool kbdShift(int mod) const
//    {
//      return (mod & KMOD_SHIFT);
//    }

    void enterMenuMode(State state);
    void leaveMenuMode();
    bool enterDebugMode();
    void leaveDebugMode();

    /**
      Send an event directly to the event handler.
      These events cannot be remapped.

      @param type  The event
      @param value The value for the event
    */
    void handleEvent(Event::Type type, Int32 value);

//    bool frying() const { return myFryingFlag; }

    void getActionList(EventMode mode, StringList& list) const;
    void getComboList(EventMode mode, VariantList& map) const;

    /** Used to access the list of events assigned to a specific combo event. */
    void getComboListForEvent(Event::Type event, StringList& list) const;
    void setComboListForEvent(Event::Type event, const StringList& events);

//    Event::Type eventForKey(StellaKey key, EventMode mode) const
//      { return myKeyTable[key][mode]; }
//    Event::Type eventForJoyAxis(int stick, int axis, int value, EventMode mode) const
//      { return myJoysticks[stick].axisTable[axis][(value > 0)][mode]; }
//    Event::Type eventForJoyButton(int stick, int button, EventMode mode) const
//      { return myJoysticks[stick].btnTable[button][mode]; }
//    Event::Type eventForJoyHat(int stick, int hat, int value, EventMode mode) const
//      { return myJoysticks[stick].hatTable[hat][value][mode]; }

    Event::Type eventAtIndex(int idx, EventMode mode) const;
    string actionAtIndex(int idx, EventMode mode) const;
    string keyAtIndex(int idx, EventMode mode) const;

    /**
      Bind a key to an event/action and regenerate the mapping array(s)

      @param event  The event we are remapping
      @param mode   The mode where this event is active
      @param key    The key to bind to this event
    */
    bool addKeyMapping(Event::Type event, EventMode mode, StellaKey key);

    /**
      Bind a joystick axis direction to an event/action and regenerate
      the mapping array(s)

      @param event  The event we are remapping
      @param mode   The mode where this event is active
      @param stick  The joystick number
      @param axis   The joystick axis
      @param value  The value on the given axis
      @param updateMenus  Whether to update the action mappings (normally
                          we want to do this, unless there are a batch of
                          'adds', in which case it's delayed until the end
    */
    bool addJoyAxisMapping(Event::Type event, EventMode mode,
                           int stick, int axis, int value,
                           bool updateMenus = true);

    /**
      Bind a joystick button to an event/action and regenerate the
      mapping array(s)

      @param event  The event we are remapping
      @param mode   The mode where this event is active
      @param stick  The joystick number
      @param button The joystick button
      @param updateMenus  Whether to update the action mappings (normally
                          we want to do this, unless there are a batch of
                          'adds', in which case it's delayed until the end
    */
    bool addJoyButtonMapping(Event::Type event, EventMode mode, int stick, int button,
                             bool updateMenus = true);

    /**
      Bind a joystick hat direction to an event/action and regenerate
      the mapping array(s)

      @param event  The event we are remapping
      @param mode   The mode where this event is active
      @param stick  The joystick number
      @param axis   The joystick hat
      @param value  The value on the given hat
      @param updateMenus  Whether to update the action mappings (normally
                          we want to do this, unless there are a batch of
                          'adds', in which case it's delayed until the end
    */
    bool addJoyHatMapping(Event::Type event, EventMode mode,
                          int stick, int hat, int value,
                          bool updateMenus = true);

    /**
      Erase the specified mapping

      @param event  The event for which we erase all mappings
      @param mode   The mode where this event is active
    */
    void eraseMapping(Event::Type event, EventMode mode);

    /**
      Resets the event mappings to default values

      @param event  The event which to (re)set (Event::NoType resets all)
      @param mode   The mode for which the defaults are set
    */
    void setDefaultMapping(Event::Type event, EventMode mode);

    /**
      Sets the combo event mappings to those in the 'combomap' setting
    */
    void setComboMap();

    /**
      Joystick emulates 'impossible' directions (ie, left & right
      at the same time)

      @param allow  Whether or not to allow impossible directions
    */
    void allowAllDirections(bool allow) { myAllowAllDirectionsFlag = allow; }

  private:
//    enum {
//      kComboSize          = 16,
//      kEventsPerCombo     = 8,
//      kEmulActionListSize = 75 + kComboSize,
//      kMenuActionListSize = 14
//    };

    /**
      Detects and changes the eventhandler state

      @param type  The event
      @return      True if the state changed, else false
    */
    bool eventStateChange(Event::Type type);

    /**
      The following methods take care of assigning action mappings.
    */
    void setActionMappings(EventMode mode);
    void setSDLMappings();
    void setKeymap();
    void setJoymap();
    void setDefaultKeymap(Event::Type, EventMode mode);
    void setDefaultJoymap(Event::Type, EventMode mode);
    void saveKeyMapping();
    void saveJoyMapping();
    void saveComboMapping();
    void setMouseAsPaddle(int paddle, const string& message);

    /**
      Tests if a given event should use continuous/analog values.

      @param event  The event to test for analog processing
      @return       True if analog, else false
    */
    bool eventIsAnalog(Event::Type event) const;

    void setEventState(State state);

    // Callback function invoked by the event-reset SDL Timer
    static uInt32 resetEventsCallback(uInt32 interval, void* param);

  private:
    // Structure used for action menu items
//    struct ActionList {
//      Event::Type event;
//      const char* action;
//      char* key;
//      bool allow_combo;
//    };
//
//    struct JoyMouse {   // Used for joystick to mouse emulation
//      bool active;
//      int x, y, x_amt, y_amt, amt, val, old_val;
//    };

    // Global OSystem object
//    OSystem* myOSystem;

    // Global Event object
    Event myEvent;

    // Indicates current overlay object
//    DialogContainer* myOverlay;
//
//    // MouseControl object, which takes care of switching the mouse between
//    // all possible controller modes
//    MouseControl* myMouseControl;
//
//    // Array of key events, indexed by StellaKey
//    Event::Type myKeyTable[KBDK_LAST][kNumModes];
//
//    // The event(s) assigned to each combination event
//    Event::Type myComboTable[kComboSize][kEventsPerCombo];
//
//    // Array of strings which correspond to the given StellaKey
//    string ourKBDKMapping[KBDK_LAST];
//
//    // Indicates the current state of the system (ie, which mode is current)
//    State myState;

    // Indicates whether the joystick emulates 'impossible' directions
    bool myAllowAllDirectionsFlag;;

    // Indicates whether or not we're in frying mode
/*     bool myFryingFlag;

    // Indicates whether the key-combos tied to the Control key are
    // being used or not (since Ctrl by default is the fire button,
    // pressing it with a movement key could inadvertantly activate
    // a Ctrl combo when it isn't wanted)
    bool myUseCtrlKeyFlag;

    // A bug in the SDL video handler creates an extraneous mouse motion
    // event after a video state change
    // We detect when this happens and discard the event
    bool mySkipMouseMotion;

    // Used for continuous snapshot mode
    uInt32 myContSnapshotInterval;
    uInt32 myContSnapshotCounter;

    // Holds static strings for the remap menu (emulation and menu events)
    static ActionList ourEmulActionList[kEmulActionListSize];
    static ActionList ourMenuActionList[kMenuActionListSize];

    // Static lookup tables for Stelladaptor/2600-daptor axis/button support
    static const Event::Type SA_Axis[2][2];
    static const Event::Type SA_Button[2][4];
    static const Event::Type SA_Key[2][12];

    // Thin wrapper holding all information about an SDL joystick in Stella.
    // A StellaJoystick holds its own event mapping information, space for
    // which is dynamically allocated based on the actual number of buttons,
    // axes, etc that the device contains.
    class StellaJoystick
    {
      public:
        StellaJoystick();
        virtual ~StellaJoystick();

        string setStick(int i);
        string getMap() const;
        bool setMap(const string& map);
        void eraseMap(EventMode mode);
        void eraseEvent(Event::Type event, EventMode mode);
        string about() const;

      public:
        enum JoyType {
          JT_NONE               = 0,
          JT_REGULAR            = 1,
          JT_STELLADAPTOR_LEFT  = 2,
          JT_STELLADAPTOR_RIGHT = 3,
          JT_2600DAPTOR_LEFT    = 4,
          JT_2600DAPTOR_RIGHT   = 5
        };

        JoyType type;
        string name;
        SDL_Joystick* stick;
        int numAxes, numButtons, numHats;
        Event::Type (*axisTable)[2][kNumModes];
        Event::Type (*btnTable)[kNumModes];
        Event::Type (*hatTable)[4][kNumModes];
        int* axisLastValue;

      private:
        void getValues(const string& list, IntArray& map);
    };
    StellaJoystick* myJoysticks;
    uInt32 myNumJoysticks;
    map<string,string> myJoystickMap;*/
};

#endif
