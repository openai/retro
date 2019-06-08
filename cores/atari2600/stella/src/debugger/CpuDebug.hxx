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
// $Id: CpuDebug.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#ifndef CPU_DEBUG_HXX
#define CPU_DEBUG_HXX

class EquateList;

#include "Array.hxx"
#include "M6502.hxx"
#include "System.hxx"
#include "DebuggerSystem.hxx"

// pointer types for CpuDebug instance methods
typedef int (CpuDebug::*CPUDEBUG_INT_METHOD)();

// call the pointed-to method on the (global) CPU debugger object.
#define CALL_CPUDEBUG_METHOD(method) ( ( Debugger::debugger().cpuDebug().*method)() )

class CpuState : public DebuggerState
{
  public:
    int PC, SP, PS, A, X, Y;
    int srcS, srcA, srcX, srcY;
    BoolArray PSbits;
};

class CpuDebug : public DebuggerSystem
{
  public:
    CpuDebug(Debugger& dbg, Console& console);

    const DebuggerState& getState();
    const DebuggerState& getOldState() { return myOldState; }

    void saveOldState();
    string toString() { return ""; } // Not needed, since CPU stuff is always visible

    // I know, we ain't supposed to do this...
    M6502& m6502() { return mySystem.m6502(); }

    int pc() { return mySystem.m6502().PC; }
    int sp() { return mySystem.m6502().SP; }
    int a()  { return mySystem.m6502().A;  }
    int x()  { return mySystem.m6502().X;  }
    int y()  { return mySystem.m6502().Y;  }

    // These return int, not boolean!
    int n() { return mySystem.m6502().N;     }
    int v() { return mySystem.m6502().V;     }
    int b() { return mySystem.m6502().B;     }
    int d() { return mySystem.m6502().D;     }
    int i() { return mySystem.m6502().I;     }
    int z() { return !mySystem.m6502().notZ; }
    int c() { return mySystem.m6502().C;     }

    void setPC(int pc);
    void setSP(int sp);
    void setPS(int ps);
    void setA(int a);
    void setX(int x);
    void setY(int y);

    void setN(bool on);
    void setV(bool on);
    void setB(bool on);
    void setD(bool on);
    void setI(bool on);
    void setZ(bool on);
    void setC(bool on);

    void toggleN();
    void toggleV();
    void toggleB();
    void toggleD();
    void toggleI();
    void toggleZ();
    void toggleC();

  private:
    M6502& my6502;

    CpuState myState;
    CpuState myOldState;
};

#endif
