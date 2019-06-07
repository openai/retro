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
// $Id: Thumbulator.hxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

//============================================================================
// This class provides Thumb emulation code ("Thumbulator")
//    by David Welch (dwelch@dwelch.com)
// Modified by Fred Quimby
// Code is public domain and used with the author's consent
//============================================================================

#ifdef THUMB_SUPPORT

#include "bspf.hxx"

#define ROMADDMASK 0x7FFF
#define RAMADDMASK 0x1FFF

#define ROMSIZE (ROMADDMASK+1)
#define RAMSIZE (RAMADDMASK+1)

//0b10000 User       PC, R14 to R0, CPSR
//0b10001 FIQ        PC, R14_fiq to R8_fiq, R7 to R0, CPSR, SPSR_fiq
//0b10010 IRQ        PC, R14_irq, R13_irq, R12 to R0, CPSR, SPSR_irq
//0b10011 Supervisor PC, R14_svc, R13_svc, R12 to R0, CPSR, SPSR_svc
//0b10111 Abort      PC, R14_abt, R13_abt, R12 to R0, CPSR, SPSR_abt
//0b11011 Undefined  PC, R14_und, R13_und, R12 to R0, CPSR, SPSR_und
//0b11111 System

#define MODE_USR 0x10
#define MODE_FIQ 0x11
#define MODE_IRQ 0x12
#define MODE_SVC 0x13
#define MODE_ABT 0x17
#define MODE_UND 0x1B
#define MODE_SYS 0x1F

#define CPSR_T (1<<5)
#define CPSR_F (1<<6)
#define CPSR_I (1<<7)
#define CPSR_N (1<<31)
#define CPSR_Z (1<<30)
#define CPSR_C (1<<29)
#define CPSR_V (1<<28)
#define CPSR_Q (1<<27)

class Thumbulator
{
  public:
    Thumbulator(const uInt16* rom, uInt16* ram, bool traponfatal);
    ~Thumbulator();

    /**
      Run the ARM code, and return when finished.  A string exception is
      thrown in case of any fatal errors/aborts (if enabled), containing the
      actual error, and the contents of the registers at that point in time.

      @return  The results of any debugging output (if enabled),
               otherwise an empty string
    */
    string run();

    /**
      Normally when a fatal error is encountered, the ARM emulation
      immediately throws an exception and exits.  This method allows execution
      to continue, and simply log the error.

      Note that this is meant for developers only, and should normally be
      always enabled.  It can be used to temporarily ignore illegal reads
      and writes, but a ROM which consistently performs these operations
      should be fixed, as it can cause crashes on real hardware.

      @param enable  Enable (the default) or disable exceptions on fatal errors
    */
    static void trapFatalErrors(bool enable) { trapOnFatal = enable; }

  private:
    uInt32 read_register ( uInt32 reg );
    uInt32 write_register ( uInt32 reg, uInt32 data );
    uInt32 fetch16 ( uInt32 addr );
    uInt32 fetch32 ( uInt32 addr );
    uInt32 read16 ( uInt32 addr );
    uInt32 read32 ( uInt32 );
    void write16 ( uInt32 addr, uInt32 data );
    void write32 ( uInt32 addr, uInt32 data );

    void do_zflag ( uInt32 x );
    void do_nflag ( uInt32 x );
    void do_cflag ( uInt32 a, uInt32 b, uInt32 c );
    void do_sub_vflag ( uInt32 a, uInt32 b, uInt32 c );
    void do_add_vflag ( uInt32 a, uInt32 b, uInt32 c );
    void do_cflag_bit ( uInt32 x );
    void do_vflag_bit ( uInt32 x );

    // Throw a string exception containing an error referencing the given
    // message and variables
    // Note that the return value is never used in these methods
    int fatalError(const char* opcode, uInt32 v1, const char* msg);
    int fatalError(const char* opcode, uInt32 v1, uInt32 v2, const char* msg);

    void dump_counters ( void );
    void dump_regs( void );
    int execute ( void );
    int reset ( void );

  private:
    const uInt16* rom;
    uInt16* ram;
    //Int32 copydata;

    uInt32 halfadd;
    uInt32 cpsr;
    //uInt32 reg_usr[16]; //User mode
    uInt32 reg_sys[16]; //System mode
    uInt32 reg_svc[16]; //Supervisor mode
    //uInt32 reg_abt[16]; //Abort mode
    //uInt32 reg_und[16]; //Undefined mode
    //uInt32 reg_irq[16]; //Interrupt mode
    //uInt32 reg_fiq[16]; //Fast Interrupt mode
    uInt32 mamcr;

    uInt64 instructions;
    uInt64 fetches;
    uInt64 reads;
    uInt64 writes;

    ostringstream statusMsg;

    static bool trapOnFatal;
};

#endif
