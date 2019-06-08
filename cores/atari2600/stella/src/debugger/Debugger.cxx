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
// $Id: Debugger.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "bspf.hxx"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "Version.hxx"
#include "OSystem.hxx"
#include "FrameBuffer.hxx"
#include "FSNode.hxx"
#include "Settings.hxx"
#include "DebuggerDialog.hxx"
#include "DebuggerParser.hxx"
#include "StateManager.hxx"

#include "Console.hxx"
#include "System.hxx"
#include "M6502.hxx"
#include "Cart.hxx"
#include "TIA.hxx"

#include "CartDebug.hxx"
#include "CartDebugWidget.hxx"
#include "CpuDebug.hxx"
#include "RiotDebug.hxx"
#include "TIADebug.hxx"

#include "TiaInfoWidget.hxx"
#include "TiaOutputWidget.hxx"
#include "TiaZoomWidget.hxx"
#include "EditTextWidget.hxx"

#include "RomWidget.hxx"
#include "Expression.hxx"
#include "PackedBitArray.hxx"
#include "YaccParser.hxx"

#include "Debugger.hxx"

Debugger* Debugger::myStaticDebugger = 0;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static const char* builtin_functions[][3] = {
  // { "name", "definition", "help text" }

  // left joystick:
  { "_joy0left", "!(*SWCHA & $40)", "Left joystick moved left" },
  { "_joy0right", "!(*SWCHA & $80)", "Left joystick moved right" },
  { "_joy0up", "!(*SWCHA & $10)", "Left joystick moved up" },
  { "_joy0down", "!(*SWCHA & $20)", "Left joystick moved down" },
  { "_joy0button", "!(*INPT4 & $80)", "Left joystick button pressed" },

  // right joystick:
  { "_joy1left", "!(*SWCHA & $04)", "Right joystick moved left" },
  { "_joy1right", "!(*SWCHA & $08)", "Right joystick moved right" },
  { "_joy1up", "!(*SWCHA & $01)", "Right joystick moved up" },
  { "_joy1down", "!(*SWCHA & $02)", "Right joystick moved down" },
  { "_joy1button", "!(*INPT5 & $80)", "Right joystick button pressed" },

  // console switches:
  { "_select", "!(*SWCHB & $02)", "Game Select pressed" },
  { "_reset", "!(*SWCHB & $01)", "Game Reset pressed" },
  { "_color", "*SWCHB & $08", "Color/BW set to Color" },
  { "_bw", "!(*SWCHB & $08)", "Color/BW set to BW" },
  { "_diff0b", "!(*SWCHB & $40)", "Left diff. set to B (easy)" },
  { "_diff0a", "*SWCHB & $40", "Left diff. set to A (hard)" },
  { "_diff1b", "!(*SWCHB & $80)", "Right diff. set to B (easy)" },
  { "_diff1a", "*SWCHB & $80", "Right diff. set to A (hard)" },

  // empty string marks end of list, do not remove
  { 0, 0, 0 }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Names are defined here, but processed in YaccParser
static const char* pseudo_registers[][2] = {
  // { "name", "help text" }

  { "_bank", "Currently selected bank" },
  { "_rwport", "Address at which a read from a write port occurred" },
  { "_scan", "Current scanline count" },
  { "_fcount", "Number of frames since emulation started" },
  { "_cclocks", "Color clocks on current scanline" },
  { "_vsync", "Whether vertical sync is enabled (1 or 0)" },
  { "_vblank", "Whether vertical blank is enabled (1 or 0)" },

  // empty string marks end of list, do not remove
  { 0, 0 }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Debugger::Debugger(OSystem& osystem, Console& console)
  : DialogContainer(&osystem),
    myConsole(console),
    mySystem(console.system()),
    myDialog(NULL),
    myParser(NULL),
    myCartDebug(NULL),
    myCpuDebug(NULL),
    myRiotDebug(NULL),
    myTiaDebug(NULL),
    myBreakPoints(NULL),
    myReadTraps(NULL),
    myWriteTraps(NULL),
    myWidth(DebuggerDialog::kSmallFontMinW),
    myHeight(DebuggerDialog::kSmallFontMinH),
    myRewindManager(NULL)
{
  // Init parser
  myParser = new DebuggerParser(*this, osystem.settings());

  // Create debugger subsystems
  myCpuDebug  = new CpuDebug(*this, myConsole);
  myCartDebug = new CartDebug(*this, myConsole, osystem);
  myRiotDebug = new RiotDebug(*this, myConsole);
  myTiaDebug  = new TIADebug(*this, myConsole);

  myBreakPoints = new PackedBitArray(0x10000);
  myReadTraps = new PackedBitArray(0x10000);
  myWriteTraps = new PackedBitArray(0x10000);

  // Allow access to this object from any class
  // Technically this violates pure OO programming, but since I know
  // there will only be ever one instance of debugger in Stella,
  // I don't care :)
  myStaticDebugger = this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Debugger::~Debugger()
{
  delete myParser;
  delete myCartDebug;
  delete myCpuDebug;
  delete myRiotDebug;
  delete myTiaDebug;
  delete myBreakPoints;
  delete myReadTraps;
  delete myWriteTraps;
  delete myRewindManager;

  myStaticDebugger = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::initialize()
{
  // Get the dialog size
  const GUI::Size& size = myOSystem->settings().getSize("dbg.res");
  myWidth = BSPF_max(size.w, 0);
  myHeight = BSPF_max(size.h, 0);
  myWidth = BSPF_max(myWidth, (uInt32)DebuggerDialog::kSmallFontMinW);
  myHeight = BSPF_max(myHeight, (uInt32)DebuggerDialog::kSmallFontMinH);
  myOSystem->settings().setValue("dbg.res", GUI::Size(myWidth, myHeight));

  delete myBaseDialog;  myBaseDialog = myDialog = NULL;
  myDialog = new DebuggerDialog(myOSystem, this, 0, 0, myWidth, myHeight);
  myBaseDialog = myDialog;

  myRewindManager = new RewindManager(*myOSystem, myDialog->rewindButton());
  myCartDebug->setDebugWidget(&(myDialog->cartDebug()));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FBInitStatus Debugger::initializeVideo()
{
  string title = string("Stella ") + STELLA_VERSION + ": Debugger mode";
  return myOSystem->frameBuffer().initialize(title, myWidth, myHeight);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::start(const string& message, int address)
{
  if(myOSystem->eventHandler().enterDebugMode())
  {
    // This must be done *after* we enter debug mode,
    // so the message isn't erased
    ostringstream buf;
    buf << message;
    if(address > -1)
      buf << Common::Base::HEX4 << address;

    myDialog->message().setText(buf.str());
    return true;
  }
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::startWithFatalError(const string& message)
{
  if(myOSystem->eventHandler().enterDebugMode())
  {
    // This must be done *after* we enter debug mode,
    // so the dialog is properly shown
    myDialog->showFatalMessage(message);
    return true;
  }
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::quit(bool exitrom)
{
  if(exitrom)
    myOSystem->eventHandler().handleEvent(Event::LauncherMode, 1);
  else
    myOSystem->eventHandler().leaveDebugMode();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Debugger::autoExec()
{
  ostringstream buf;

  // autoexec.stella is always run
  FilesystemNode autoexec(myOSystem->baseDir() + "autoexec.stella");
  buf << "autoExec():" << endl
      << myParser->exec(autoexec) << endl;

  // Also, "romname.stella" if present
  FilesystemNode romname(myOSystem->romFile().getPathWithExt(".stella"));
  buf << myParser->exec(romname) << endl;

  // Init builtins
  for(int i = 0; builtin_functions[i][0] != 0; i++)
  {
    // TODO - check this for memory leaks
    int res = YaccParser::parse(builtin_functions[i][1]);
    if(res != 0) cerr << "ERROR in builtin function!" << endl;
    Expression* exp = YaccParser::getResult();
    addFunction(builtin_functions[i][0], builtin_functions[i][1], exp, true);
  }
  return buf.str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const string Debugger::run(const string& command)
{
  return myParser->run(command);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const string Debugger::invIfChanged(int reg, int oldReg)
{
  string ret;

  bool changed = reg != oldReg;
  if(changed) ret += "\177";
  ret += Common::Base::toString(reg, Common::Base::F_16_2);
  if(changed) ret += "\177";

  return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::reset()
{
  myCpuDebug->setPC(dpeek(0xfffc));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/* Element 0 of args is the address. The remaining elements are the data
   to poke, starting at the given address.
*/
string Debugger::setRAM(IntArray& args)
{
  ostringstream buf;

  int count = args.size();
  int address = args[0];
  for(int i = 1; i < count; ++i)
    mySystem.poke(address++, args[i]);

  buf << "changed " << (count-1) << " location";
  if(count != 2)
    buf << "s";
  return buf.str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Debugger::step()
{
  saveOldState();
  mySystem.clearDirtyPages();

  int cyc = mySystem.cycles();

  unlockBankswitchState();
  myOSystem->console().tia().updateScanlineByStep();
  lockBankswitchState();

  return mySystem.cycles() - cyc;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// trace is just like step, except it treats a subroutine call as one
// instruction.

// This implementation is not perfect: it just watches the program counter,
// instead of tracking (possibly) nested JSR/RTS pairs. In particular, it
// will fail for recursive subroutine calls. However, with 128 bytes of RAM
// to share between stack and variables, I doubt any 2600 games will ever
// use recursion...

int Debugger::trace()
{
  // 32 is the 6502 JSR instruction:
  if(mySystem.peek(myCpuDebug->pc()) == 32)
  {
    saveOldState();
    mySystem.clearDirtyPages();

    int cyc = mySystem.cycles();
    int targetPC = myCpuDebug->pc() + 3; // return address

    unlockBankswitchState();
    myOSystem->console().tia().updateScanlineByTrace(targetPC);
    lockBankswitchState();

    return mySystem.cycles() - cyc;
  }
  else
    return step();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::toggleBreakPoint(int bp)
{
  mySystem.m6502().setBreakPoints(myBreakPoints);
  if(bp < 0) bp = myCpuDebug->pc();
  myBreakPoints->toggle(bp);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::setBreakPoint(int bp, bool set)
{
  mySystem.m6502().setBreakPoints(myBreakPoints);
  if(bp < 0) bp = myCpuDebug->pc();
  if(set)
    myBreakPoints->set(bp);
  else
    myBreakPoints->clear(bp);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::breakPoint(int bp)
{
  if(bp < 0) bp = myCpuDebug->pc();
  return myBreakPoints->isSet(bp) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::toggleReadTrap(int t)
{
  mySystem.m6502().setTraps(myReadTraps, myWriteTraps);
  myReadTraps->toggle(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::toggleWriteTrap(int t)
{
  mySystem.m6502().setTraps(myReadTraps, myWriteTraps);
  myWriteTraps->toggle(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::toggleTrap(int t)
{
  toggleReadTrap(t);
  toggleWriteTrap(t);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::readTrap(int t)
{
  return myReadTraps->isSet(t) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::writeTrap(int t)
{
  return myWriteTraps->isSet(t) != 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Debugger::cycles()
{
  return mySystem.cycles();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::nextScanline(int lines)
{
  saveOldState();
  mySystem.clearDirtyPages();

  unlockBankswitchState();
  while(lines)
  {
    myOSystem->console().tia().updateScanline();
    --lines;
  }
  lockBankswitchState();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::nextFrame(int frames)
{
  saveOldState();
  mySystem.clearDirtyPages();

  unlockBankswitchState();
  while(frames)
  {
    myOSystem->console().tia().update();
    --frames;
  }
  lockBankswitchState();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::rewindState()
{
  mySystem.clearDirtyPages();

  unlockBankswitchState();
  bool result = myRewindManager->rewindState();
  lockBankswitchState();

  return result;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::clearAllBreakPoints()
{
  delete myBreakPoints;
  myBreakPoints = new PackedBitArray(0x10000);
  mySystem.m6502().setBreakPoints(NULL);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::clearAllTraps() 
{
  delete myReadTraps;
  delete myWriteTraps;
  myReadTraps = new PackedBitArray(0x10000);
  myWriteTraps = new PackedBitArray(0x10000);
  mySystem.m6502().setTraps(NULL, NULL);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Debugger::showWatches()
{
  return myParser->showWatches();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::setBank(int bank)
{
  if(myConsole.cartridge().bankCount() > 1)
  {
    myConsole.cartridge().unlockBank();
    bool status = myConsole.cartridge().bank(bank);
    myConsole.cartridge().lockBank();
    return status;
  }
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::patchROM(int addr, int value)
{
  return myConsole.cartridge().patch(addr, value);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::saveOldState(bool addrewind)
{
  myCartDebug->saveOldState();
  myCpuDebug->saveOldState();
  myRiotDebug->saveOldState();
  myTiaDebug->saveOldState();

  // Add another rewind level to the Undo list
  if(addrewind)  myRewindManager->addState();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::setStartState()
{
  // Lock the bus each time the debugger is entered, so we don't disturb anything
  lockBankswitchState();

  // Start a new rewind list
  myRewindManager->clear();

  // Save initial state, but don't add it to the rewind list
  saveOldState(false);

  // Set the 're-disassemble' flag, but don't do it until the next scheduled time 
  myDialog->rom().invalidate(false);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::setQuitState()
{
  // Bus must be unlocked for normal operation when leaving debugger mode
  unlockBankswitchState();

  // execute one instruction on quit. If we're
  // sitting at a breakpoint/trap, this will get us past it.
  // Somehow this feels like a hack to me, but I don't know why
  //	if(myBreakPoints->isSet(myCpuDebug->pc()))
  mySystem.m6502().execute(1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::addFunction(const string& name, const string& definition,
                           Expression* exp, bool builtin)
{
  functions.insert(make_pair(name, exp));
  if(!builtin)
    functionDefs.insert(make_pair(name, definition));

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::delFunction(const string& name)
{
  FunctionMap::iterator iter = functions.find(name);
  if(iter == functions.end())
    return false;

  functions.erase(name);
  delete iter->second;

  FunctionDefMap::iterator def_iter = functionDefs.find(name);
  if(def_iter == functionDefs.end())
    return false;

  functionDefs.erase(name);
  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const Expression* Debugger::getFunction(const string& name) const
{
  FunctionMap::const_iterator iter = functions.find(name);
  if(iter == functions.end())
    return 0;
  else
    return iter->second;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const string& Debugger::getFunctionDef(const string& name) const
{
  FunctionDefMap::const_iterator iter = functionDefs.find(name);
  if(iter == functionDefs.end())
    return EmptyString;
  else
    return iter->second;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const FunctionDefMap Debugger::getFunctionDefMap() const
{
  return functionDefs;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string Debugger::builtinHelp() const
{
  ostringstream buf;
  uInt16 len, c_maxlen = 0, i_maxlen = 0;

  // Get column widths for aligned output (functions)
  for(int i = 0; builtin_functions[i][0] != 0; ++i)
  {
    len = strlen(builtin_functions[i][0]);
    if(len > c_maxlen)  c_maxlen = len;
    len = strlen(builtin_functions[i][1]);
    if(len > i_maxlen)  i_maxlen = len;
  }

  buf << setfill(' ') << endl << "Built-in functions:" << endl;
  for(int i = 0; builtin_functions[i][0] != 0; ++i)
  {
    buf << setw(c_maxlen) << left << builtin_functions[i][0]
        << setw(2) << right << "{"
        << setw(i_maxlen) << left << builtin_functions[i][1]
        << setw(4) << "}"
        << builtin_functions[i][2]
        << endl;
  }

  // Get column widths for aligned output (pseudo-registers)
  c_maxlen = 0;
  for(int i = 0; pseudo_registers[i][0] != 0; ++i)
  {
    len = strlen(pseudo_registers[i][0]);
    if(len > c_maxlen)  c_maxlen = len;
  }

  buf << endl << "Pseudo-registers:" << endl;
  for(int i = 0; pseudo_registers[i][0] != 0; ++i)
  {
    buf << setw(c_maxlen) << left << pseudo_registers[i][0]
        << setw(2) << " "
        << setw(i_maxlen) << left << pseudo_registers[i][1]
        << endl;
  }

  return buf.str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::getCompletions(const char* in, StringList& list) const
{
  FunctionMap::const_iterator iter;
  for(iter = functions.begin(); iter != functions.end(); ++iter)
  {
    const char* l = iter->first.c_str();
    if(BSPF_equalsIgnoreCase(l, in))
      list.push_back(l);
  }

  for(int i = 0; pseudo_registers[i][0] != 0; ++i)
    if(BSPF_equalsIgnoreCase(pseudo_registers[i][0], in))
      list.push_back(pseudo_registers[i][0]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::lockBankswitchState()
{
  mySystem.lockDataBus();
  myConsole.cartridge().lockBank();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::unlockBankswitchState()
{
  mySystem.unlockDataBus();
  myConsole.cartridge().unlockBank();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Debugger::RewindManager::RewindManager(OSystem& system, ButtonWidget& button)
  : myOSystem(system),
    myRewindButton(button),
    mySize(0),
    myTop(0)
{
  for(int i = 0; i < MAX_SIZE; ++i)
    myStateList[i] = (Serializer*) NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Debugger::RewindManager::~RewindManager()
{
  for(int i = 0; i < MAX_SIZE; ++i)
    delete myStateList[i];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::RewindManager::addState()
{
  // Create a new Serializer object if we need one
  if(myStateList[myTop] == NULL)
    myStateList[myTop] = new Serializer();
  Serializer& s = *(myStateList[myTop]);

  if(s.isValid())
  {
    s.reset();
    if(myOSystem.state().saveState(s) && myOSystem.console().tia().saveDisplay(s))
    {
      // Are we still within the allowable size, or are we overwriting an item?
      mySize++; if(mySize > MAX_SIZE) mySize = MAX_SIZE;

      myTop = (myTop + 1) % MAX_SIZE;
      myRewindButton.setEnabled(true);
      return true;
    }
  }
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::RewindManager::rewindState()
{
  if(mySize > 0)
  {
    mySize--;
    myTop = myTop == 0 ? MAX_SIZE - 1 : myTop - 1;
    Serializer& s = *(myStateList[myTop]);

    s.reset();
    myOSystem.state().loadState(s);
    myOSystem.console().tia().loadDisplay(s);

    if(mySize == 0)
      myRewindButton.setEnabled(false);

    return true;
  }
  else
    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Debugger::RewindManager::isEmpty()
{
  return mySize == 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Debugger::RewindManager::clear()
{
  for(int i = 0; i < MAX_SIZE; ++i)
    if(myStateList[i] != NULL)
      myStateList[i]->reset();

  myTop = mySize = 0;

  // We use Widget::clearFlags here instead of Widget::setEnabled(),
  // since the latter implies an immediate draw/update, but this method
  // might be called before any UI exists
  // TODO - fix this deficiency in the UI core; we shouldn't have to worry
  //        about such things at this level
  myRewindButton.clearFlags(WIDGET_ENABLED);
}
