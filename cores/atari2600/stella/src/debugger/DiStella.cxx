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
// $Id: DiStella.cxx 2838 2014-01-17 23:34:03Z stephena $
//============================================================================

#include "bspf.hxx"
#include "Debugger.hxx"
#include "DiStella.hxx"
using namespace Common;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DiStella::DiStella(const CartDebug& dbg, CartDebug::DisassemblyList& list,
                   CartDebug::BankInfo& info, const DiStella::Settings& settings,
                   uInt8* labels, uInt8* directives,
                   CartDebug::ReservedEquates& reserved)
  : myDbg(dbg),
    myList(list),
    mySettings(settings),
    myReserved(reserved),
    myLabels(labels),
    myDirectives(directives)
{
  CartDebug::AddressList& addresses = info.addressList;
  if(addresses.size() == 0)
    return;

  while(!myAddressQueue.empty())
    myAddressQueue.pop();

  bool resolve_code = mySettings.resolve_code;

  CartDebug::AddressList::iterator it = addresses.begin();
  uInt16 start = *it++;

  myOffset = info.offset;
  if(start & 0x1000)
  {
    if(info.size == 4096)  // 4K ROM space
    {
      /*============================================
        The offset is the address where the code segment
        starts.  For a 4K game, it is usually 0xf000.

        Example:
          Start address = $D973, so therefore
          Offset to code = $D000
          Code range = $D000-$DFFF
      =============================================*/
      info.start  = myAppData.start = 0x0000;
      info.end    = myAppData.end   = 0x0FFF;

      // Keep previous offset; it may be different between banks
      if(info.offset == 0)
        info.offset = myOffset = (start - (start % 0x1000));
    }
    else  // 2K ROM space (also includes 'Sub2K' ROMs)
    {
      /*============================================
        The offset is the address where the code segment
        starts.  For a 2K game, it is usually 0xf800,
        but can also be 0xf000.
      =============================================*/
      info.start  = myAppData.start = 0x0000;
      info.end    = myAppData.end   = info.size - 1;
      info.offset = myOffset        = (start - (start % info.size));
    }
  }
  else  // ZP RAM
  {
    // For now, we assume all accesses below $1000 are zero-page 
    info.start  = myAppData.start = 0x0080;
    info.end    = myAppData.end   = 0x00FF;
    info.offset = myOffset        = 0;

    // Resolve code is never used in ZP RAM mode
    resolve_code = false;
  }
  myAppData.length = info.size;

  memset(myLabels, 0, 0x1000);
  memset(myDirectives, 0, 0x1000);
  myAddressQueue.push(start);

  // Process any directives first, as they override automatic code determination
  processDirectives(info.directiveList);

  if(resolve_code)
  {
    // After we've disassembled from all addresses in the address list,
    // use all access points determined by Stella during emulation
    int codeAccessPoint = 0;

    while(!myAddressQueue.empty())
    {
      myPC = myAddressQueue.front();
      myPCBeg = myPC;
      myAddressQueue.pop();
      disasm(myPC, 1);
      if(myPCBeg <= myPCEnd)
      {
        // Tentatively mark all addresses in the range as CODE
        // Note that this is a 'best-effort' approach, since
        // Distella will normally keep going until the end of the
        // range or branch is encountered
        // However, addresses *specifically* marked as DATA/GFX/PGFX
        // in the emulation core indicate that the CODE range has finished
        // Therefore, we stop at the first such address encountered
        for (uInt32 k = myPCBeg; k <= myPCEnd; k++)
        {
          if(Debugger::debugger().getAccessFlags(k) &
             (CartDebug::DATA|CartDebug::GFX|CartDebug::PGFX))
          {
            myPCEnd = k - 1;
            break;
          }
          mark(k, CartDebug::CODE);
        }
      }

      // When we get to this point, all addresses have been processed
      // starting from the initial one in the address list
      // If so, process the next one in the list that hasn't already
      // been marked as CODE
      // If it *has* been marked, it can be removed from consideration
      // in all subsequent passes
      //
      // Once the address list has been exhausted, we process all addresses
      // determined during emulation to represent code, which *haven't* already
      // been considered
      //
      // Note that we can't simply add all addresses right away, since
      // the processing of a single address can cause others to be added in
      // the ::disasm method
      // All of these have to be exhausted before considering a new address
      if(myAddressQueue.empty())
      {
        while(it != addresses.end())
        {
          uInt16 addr = *it;
          if(!check_bit(addr-myOffset, CartDebug::CODE))
          {
            myAddressQueue.push(addr);
            ++it;
            break;
          }
          else   // remove this address, it is redundant
            it = addresses.erase(it);
        }

        // Stella itself can provide hints on whether an address has ever
        // been referenced as CODE
        while(it == addresses.end() && codeAccessPoint <= myAppData.end)
        {
          if((Debugger::debugger().getAccessFlags(codeAccessPoint+myOffset) & CartDebug::CODE)
             && !(myLabels[codeAccessPoint & myAppData.end] & CartDebug::CODE))
          {
            myAddressQueue.push(codeAccessPoint+myOffset);
            ++codeAccessPoint;
            break;
          }
          ++codeAccessPoint;
        }
      }
    }
    for (int k = 0; k <= myAppData.end; k++)
    {
      // Let the emulation core know about tentative code
      if(check_bit(k, CartDebug::CODE) &&
        !(Debugger::debugger().getAccessFlags(k+myOffset) & CartDebug::CODE)
         && myOffset != 0)
      {
        Debugger::debugger().setAccessFlags(k+myOffset, CartDebug::TCODE);
      }

      // Must be ROW / unused bytes
      if (!check_bit(k, CartDebug::CODE | CartDebug::GFX |
                        CartDebug::PGFX | CartDebug::DATA))
        mark(k+myOffset, CartDebug::ROW);
    }
  }

  // Second pass
  disasm(myOffset, 2);

  // Add reserved line equates
  ostringstream reservedLabel;
  for (int k = 0; k <= myAppData.end; k++)
  {
    if ((myLabels[k] & (CartDebug::REFERENCED | CartDebug::VALID_ENTRY)) ==
        CartDebug::REFERENCED)
    {
      // If we have a piece of code referenced somewhere else, but cannot
      // locate the label in code (i.e because the address is inside of a
      // multi-byte instruction, then we make note of that address for reference
      //
      // However, we only do this for labels pointing to ROM (above $1000)
      if(myDbg.addressType(k+myOffset) == CartDebug::ADDR_ROM)
      {
        reservedLabel.str("");
        reservedLabel << "L" << Base::HEX4 << (k+myOffset);
        myReserved.Label.insert(make_pair(k+myOffset, reservedLabel.str()));
      }
    }
  }

  // Third pass
  disasm(myOffset, 3);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DiStella::~DiStella()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DiStella::disasm(uInt32 distart, int pass)
{
#define LABEL_A12_HIGH(address) labelA12High(nextline, op, address, labfound)
#define LABEL_A12_LOW(address)  labelA12Low(nextline, op, address, labfound)

  uInt8 op, d1;
  uInt16 ad;
  AddressingMode addr_mode;
  int bytes=0, labfound=0, addbranch=0;
  stringstream nextline, nextlinebytes;
  myDisasmBuf.str("");

  /* pc=myAppData.start; */
  myPC = distart - myOffset;
  while(myPC <= myAppData.end)
  {
    if(check_bit(myPC, CartDebug::GFX|CartDebug::PGFX) && !check_bit(myPC, CartDebug::CODE))
    {
      mark(myPC+myOffset, CartDebug::VALID_ENTRY);

      if (pass == 3)
      {
        if (check_bit(myPC, CartDebug::REFERENCED))
          myDisasmBuf << Base::HEX4 << myPC+myOffset << "'L" << Base::HEX4 << myPC+myOffset << "'";
        else
          myDisasmBuf << Base::HEX4 << myPC+myOffset << "'     '";

        bool isPGfx = check_bit(myPC, CartDebug::PGFX);
        const string& bit_string = isPGfx ? "\x1f" : "\x1e";
        uInt8 byte = Debugger::debugger().peek(myPC+myOffset);
        myDisasmBuf << ".byte $" << Base::HEX2 << (int)byte << "  |";
        for(uInt8 i = 0, c = byte; i < 8; ++i, c <<= 1)
          myDisasmBuf << ((c > 127) ? bit_string : " ");
        myDisasmBuf << "|  $" << Base::HEX4 << myPC+myOffset << "'";
        if(mySettings.gfx_format == Base::F_2)
          myDisasmBuf << Base::toString(byte, Base::F_2_8);
        else
          myDisasmBuf << Base::HEX2 << (int)byte;
        addEntry(isPGfx ? CartDebug::PGFX : CartDebug::GFX);
      }
      myPC++;
    }
    else if (check_bit(myPC, CartDebug::DATA) &&
             !check_bit(myPC, CartDebug::CODE|CartDebug::GFX|CartDebug::PGFX))
    {
      if (pass == 2)
        mark(myPC+myOffset, CartDebug::VALID_ENTRY);
      else if (pass == 3)
      {
        if (check_bit(myPC, CartDebug::REFERENCED))
          myDisasmBuf << Base::HEX4 << myPC+myOffset << "'L" << Base::HEX4 << myPC+myOffset << "'";
        else
          myDisasmBuf << Base::HEX4 << myPC+myOffset << "'     '";

        uInt8 byte = Debugger::debugger().peek(myPC+myOffset);
        myDisasmBuf << ".byte $" << Base::HEX2 << (int)byte << "              $"
                    << Base::HEX4 << myPC+myOffset << "'"
                    << Base::HEX2 << (int)byte;
        addEntry(CartDebug::DATA);
      }
      myPC++;
    }
    else if (check_bit(myPC, CartDebug::ROW) &&
             !check_bit(myPC, CartDebug::CODE|CartDebug::DATA|CartDebug::GFX|CartDebug::PGFX))
    {
      if (pass == 2)
        mark(myPC+myOffset, CartDebug::VALID_ENTRY);

      if (pass == 3)
      {
        bool row = check_bit(myPC, CartDebug::ROW) &&
                  !check_bit(myPC, CartDebug::CODE | CartDebug::DATA |
                                   CartDebug::GFX | CartDebug::PGFX);
        bool referenced = check_bit(myPC, CartDebug::REFERENCED);
        bool line_empty = true;
        while (row && myPC <= myAppData.end)
        {
          if(referenced)        // start a new line with a label
          {
            if(!line_empty)
            {
              addEntry(CartDebug::ROW);
              line_empty = true;
            }
            myDisasmBuf << Base::HEX4 << myPC+myOffset << "'L" << Base::HEX4
                        << myPC+myOffset << "'.byte " << "$" << Base::HEX2
                        << (int)Debugger::debugger().peek(myPC+myOffset);
            myPC++;
            bytes = 1;
            line_empty = false;
          }
          else if(line_empty)   // start a new line without a label
          {
            myDisasmBuf << "    '     '.byte $" << Base::HEX2 << (int)Debugger::debugger().peek(myPC+myOffset);
            myPC++;
            bytes = 1;
            line_empty = false;
          }
          // Otherwise, append bytes to the current line, up until the maximum
          else if(++bytes == mySettings.bwidth)
          {
            addEntry(CartDebug::ROW);
            line_empty = true;
          }
          else
          {
            myDisasmBuf << ",$" << Base::HEX2 << (int)Debugger::debugger().peek(myPC+myOffset);
            myPC++;
          }

          row = check_bit(myPC, CartDebug::ROW) &&
               !check_bit(myPC, CartDebug::CODE | CartDebug::DATA |
                                CartDebug::GFX | CartDebug::PGFX);
          referenced = check_bit(myPC, CartDebug::REFERENCED);
        }
        if(!line_empty)
          addEntry(CartDebug::ROW);
        myDisasmBuf << "    '     ' ";
        addEntry(CartDebug::NONE);
      }
      else
        myPC++;
    }
    else  // The following sections must be CODE
    {
      // Add label (if any)
      //
      op = Debugger::debugger().peek(myPC+myOffset);
      /* version 2.1 bug fix */
      if (pass == 2)
        mark(myPC+myOffset, CartDebug::VALID_ENTRY);
      else if (pass == 3)
      {
        if (check_bit(myPC, CartDebug::REFERENCED))
          myDisasmBuf << Base::HEX4 << myPC+myOffset << "'L" << Base::HEX4 << myPC+myOffset << "'";
        else
          myDisasmBuf << Base::HEX4 << myPC+myOffset << "'     '";
      }

      // Add opcode mneumonic
      //
      addr_mode = ourLookup[op].addr_mode;
      myPC++;

      // Undefined opcodes start with a '.'
      // These are undefined wrt DASM
      if (ourLookup[op].mnemonic[0] == '.')
      {
        addr_mode = IMPLIED;
        if (pass == 3)
          nextline << ".byte $" << Base::HEX2 << (int)op << " ;";
      }

      if (pass == 1)
      {
        /* M_REL covers BPL, BMI, BVC, BVS, BCC, BCS, BNE, BEQ
           M_ADDR = JMP $NNNN, JSR $NNNN
           M_AIND = JMP Abs, Indirect */
        switch(ourLookup[op].source)
        {
          case M_REL:
          case M_ADDR:
          case M_AIND:
            addbranch = 1;
            break;
          default:
            addbranch = 0;
            break;
        }
      }
      else if (pass == 3)
      {
        nextline << ourLookup[op].mnemonic;
        nextlinebytes << Base::HEX2 << (int)op << " ";
      }

      // Add operand(s) for PC values outside the app data range
      //
      if (myPC >= myAppData.end)
      {
        switch(addr_mode)
        {
          case ABSOLUTE:
          case ABSOLUTE_X:
          case ABSOLUTE_Y:
          case INDIRECT_X:
          case INDIRECT_Y:
          case ABS_INDIRECT:
          {
            if (pass == 3)
            {
              /* Line information is already printed; append .byte since last
                 instruction will put recompilable object larger that original
                 binary file */
              myDisasmBuf << ".byte $" << Base::HEX2 << (int)op << "              $"
                          << Base::HEX4 << myPC+myOffset << "'"
                          << Base::HEX2 << (int)op;
              addEntry(CartDebug::DATA);

              if (myPC == myAppData.end)
              {
                if (check_bit(myPC, CartDebug::REFERENCED))
                  myDisasmBuf << Base::HEX4 << myPC+myOffset << "'L" << Base::HEX4 << myPC+myOffset << "'";
                else
                  myDisasmBuf << Base::HEX4 << myPC+myOffset << "'     '";

                op = Debugger::debugger().peek(myPC+myOffset);  myPC++;
                myDisasmBuf << ".byte $" << Base::HEX2 << (int)op << "              $"
                            << Base::HEX4 << myPC+myOffset << "'"
                            << Base::HEX2 << (int)op;
                addEntry(CartDebug::DATA);
              }
            }
            myPCEnd = myAppData.end + myOffset;
            return;
          }

          case ZERO_PAGE:
          case IMMEDIATE:
          case ZERO_PAGE_X:
          case ZERO_PAGE_Y:
          case RELATIVE:
          {
            if (myPC > myAppData.end)
            {
              if (pass == 3)
              {
                /* Line information is already printed, but we can remove the
                   Instruction (i.e. BMI) by simply clearing the buffer to print */
                myDisasmBuf << ".byte $" << Base::HEX2 << (int)op;
                addEntry(CartDebug::ROW);
                nextline.str("");
                nextlinebytes.str("");
              }
              myPC++;
              myPCEnd = myAppData.end + myOffset;
              return;
            }
          }

          default:
            break;
        }  // end switch(addr_mode)
      }

      // Add operand(s)
      //
      /* Version 2.1 added the extensions to mnemonics */
      switch(addr_mode)
      {
    #if 0
        case IMPLIED:
        {
          if (op == 0x40 || op == 0x60)
            if (pass == 3)
              nextline << "\n";
          break;
        }
    #endif
        case ACCUMULATOR:
        {
          if (pass == 3 && mySettings.aflag)
            nextline << "    A";
          break;
        }

        case ABSOLUTE:
        {
          ad = Debugger::debugger().dpeek(myPC+myOffset);  myPC+=2;
          labfound = mark(ad, CartDebug::REFERENCED);
          if (pass == 1)
          {
            if (addbranch)
            {
              if (!check_bit(ad & myAppData.end, CartDebug::CODE))
              {
                if (ad > 0xfff)
                  myAddressQueue.push((ad & myAppData.end) + myOffset);

                mark(ad, CartDebug::CODE);
              }
            }
            else if(ad > 0xfff)
            {
              mark(ad, CartDebug::DATA);
            }
          }
          else if (pass == 3)
          {
            if (ad < 0x100 && mySettings.fflag)
              nextline << ".w  ";
            else
              nextline << "    ";

            if (labfound == 1)
            {
              LABEL_A12_HIGH(ad);
              nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
            }
            else if (labfound == 4)
            {
              if(mySettings.rflag)
              {
                int tmp = (ad & myAppData.end)+myOffset;
                LABEL_A12_HIGH(tmp);
                nextlinebytes << Base::HEX2 << (int)(tmp&0xff) << " " << Base::HEX2 << (int)(tmp>>8);
              }
              else
              {
                nextline << "$" << Base::HEX4 << ad;
                nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
              }
            }
            else
            {
              LABEL_A12_LOW(ad);
              nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
            }
          }
          break;
        }

        case ZERO_PAGE:
        {
          d1 = Debugger::debugger().peek(myPC+myOffset);  myPC++;
          labfound = mark(d1, CartDebug::REFERENCED);
          if (pass == 3)
          {
            nextline << "    ";
            LABEL_A12_LOW((int)d1);
            nextlinebytes << Base::HEX2 << (int)d1;
          }
          break;
        }

        case IMMEDIATE:
        {
          d1 = Debugger::debugger().peek(myPC+myOffset);  myPC++;
          if (pass == 3)
          {
            nextline << "    #$" << Base::HEX2 << (int)d1 << " ";
            nextlinebytes << Base::HEX2 << (int)d1;
          }
          break;
        }

        case ABSOLUTE_X:
        {
          ad = Debugger::debugger().dpeek(myPC+myOffset);  myPC+=2;
          labfound = mark(ad, CartDebug::REFERENCED);
          if (pass == 2 && !check_bit(ad & myAppData.end, CartDebug::CODE))
          {
            // Since we can't know what address is being accessed unless we also
            // know the current X value, this is marked as ROW instead of DATA
            // The processing is left here, however, in case future versions of
            // the code can somehow track access to CPU registers
            mark(ad, CartDebug::ROW);
          }
          else if (pass == 3)
          {
            if (ad < 0x100 && mySettings.fflag)
              nextline << ".wx ";
            else
              nextline << "    ";

            if (labfound == 1)
            {
              LABEL_A12_HIGH(ad);
              nextline << ",X";
              nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
            }
            else if (labfound == 4)
            {
              if(mySettings.rflag)
              {
                int tmp = (ad & myAppData.end)+myOffset;
                LABEL_A12_HIGH(tmp);
                nextline << ",X";
                nextlinebytes << Base::HEX2 << (int)(tmp&0xff) << " " << Base::HEX2 << (int)(tmp>>8);
              }
              else
              {
                nextline << "$" << Base::HEX4 << ad << ",X";
                nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
              }
            }
            else
            {
              LABEL_A12_LOW(ad);
              nextline << ",X";
              nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
            }
          }
          break;
        }

        case ABSOLUTE_Y:
        {
          ad = Debugger::debugger().dpeek(myPC+myOffset);  myPC+=2;
          labfound = mark(ad, CartDebug::REFERENCED);
          if (pass == 2 && !check_bit(ad & myAppData.end, CartDebug::CODE))
          {
            // Since we can't know what address is being accessed unless we also
            // know the current Y value, this is marked as ROW instead of DATA
            // The processing is left here, however, in case future versions of
            // the code can somehow track access to CPU registers
            mark(ad, CartDebug::ROW);
          }
          else if (pass == 3)
          {
            if (ad < 0x100 && mySettings.fflag)
              nextline << ".wy ";
            else
              nextline << "    ";

            if (labfound == 1)
            {
              LABEL_A12_HIGH(ad);
              nextline << ",Y";
              nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
            }
            else if (labfound == 4)
            {
              if(mySettings.rflag)
              {
                int tmp = (ad & myAppData.end)+myOffset;
                LABEL_A12_HIGH(tmp);
                nextline << ",Y";
                nextlinebytes << Base::HEX2 << (int)(tmp&0xff) << " " << Base::HEX2 << (int)(tmp>>8);
              }
              else
              {
                nextline << "$" << Base::HEX4 << ad << ",Y";
                nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
              }
            }
            else
            {
              LABEL_A12_LOW(ad);
              nextline << ",Y";
              nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
            }
          }
          break;
        }

        case INDIRECT_X:
        {
          d1 = Debugger::debugger().peek(myPC+myOffset);  myPC++;
          if (pass == 3)
          {
            labfound = mark(d1, 0);  // dummy call to get address type
            nextline << "    (";
            LABEL_A12_LOW(d1);
            nextline << ",X)";
            nextlinebytes << Base::HEX2 << (int)d1;
          }
          break;
        }

        case INDIRECT_Y:
        {
          d1 = Debugger::debugger().peek(myPC+myOffset);  myPC++;
          if (pass == 3)
          {
            labfound = mark(d1, 0);  // dummy call to get address type
            nextline << "    (";
            LABEL_A12_LOW(d1);
            nextline << "),Y";
            nextlinebytes << Base::HEX2 << (int)d1;
          }
          break;
        }

        case ZERO_PAGE_X:
        {
          d1 = Debugger::debugger().peek(myPC+myOffset);  myPC++;
          labfound = mark(d1, CartDebug::REFERENCED);
          if (pass == 3)
          {
            nextline << "    ";
            LABEL_A12_LOW(d1);
            nextline << ",X";
          }
          nextlinebytes << Base::HEX2 << (int)d1;
          break;
        }

        case ZERO_PAGE_Y:
        {
          d1 = Debugger::debugger().peek(myPC+myOffset);  myPC++;
          labfound = mark(d1, CartDebug::REFERENCED);
          if (pass == 3)
          {
            nextline << "    ";
            LABEL_A12_LOW(d1);
            nextline << ",Y";
          }
          nextlinebytes << Base::HEX2 << (int)d1;
          break;
        }

        case RELATIVE:
        {
          // SA - 04-06-2010: there seemed to be a bug in distella,
          // where wraparound occurred on a 32-bit int, and subsequent
          // indexing into the labels array caused a crash
          d1 = Debugger::debugger().peek(myPC+myOffset);  myPC++;
          ad = ((myPC + (Int8)d1) & 0xfff) + myOffset;

          labfound = mark(ad, CartDebug::REFERENCED);
          if (pass == 1)
          {
            if ((addbranch) && !check_bit(ad-myOffset, CartDebug::CODE))
            {
              myAddressQueue.push(ad);
              mark(ad, CartDebug::CODE);
            }
          }
          else if (pass == 3)
          {
            if (labfound == 1)
            {
              nextline << "    ";
              LABEL_A12_HIGH(ad);
            }
            else
              nextline << "    $" << Base::HEX4 << ad;

            nextlinebytes << Base::HEX2 << (int)d1;
          }
          break;
        }

        case ABS_INDIRECT:
        {
          ad = Debugger::debugger().dpeek(myPC+myOffset);  myPC+=2;
          labfound = mark(ad, CartDebug::REFERENCED);
          if (pass == 2 && !check_bit(ad & myAppData.end, CartDebug::CODE))
          {
            // Since we can't know what address is being accessed unless we also
            // know the current X value, this is marked as ROW instead of DATA
            // The processing is left here, however, in case future versions of
            // the code can somehow track access to CPU registers
            mark(ad, CartDebug::ROW);
          }
          else if (pass == 3)
          {
            if (ad < 0x100 && mySettings.fflag)
              nextline << ".ind ";
            else
              nextline << "     ";
          }
          if (labfound == 1)
          {
            nextline << "(";
            LABEL_A12_HIGH(ad);
            nextline << ")";
          }
          // TODO - should we consider case 4??
          else
          {
            nextline << "(";
            LABEL_A12_LOW(ad);
            nextline << ")";
          }

          nextlinebytes << Base::HEX2 << (int)(ad&0xff) << " " << Base::HEX2 << (int)(ad>>8);
          break;
        }

        default:
          break;
      } // end switch

      if (pass == 1)
      {
        // RTS/JMP/RTI always indicate the end of a block of CODE
        if (!strcmp(ourLookup[op].mnemonic,"RTS") ||
            !strcmp(ourLookup[op].mnemonic,"JMP") ||
            /* !strcmp(ourLookup[op].mnemonic,"BRK") || */
            !strcmp(ourLookup[op].mnemonic,"RTI"))
        {
          myPCEnd = (myPC-1) + myOffset;
          return;
        }
      }
      else if (pass == 3)
      {
        // A complete line of disassembly (text, cycle count, and bytes)
        myDisasmBuf << nextline.str() << "'"
                    << ";" << dec << (int)ourLookup[op].cycles << "'"
                    << nextlinebytes.str();
        addEntry(CartDebug::CODE);
        if (op == 0x40 || op == 0x60)
        {
          myDisasmBuf << "    '     ' ";
          addEntry(CartDebug::NONE);
        }

        nextline.str("");
        nextlinebytes.str("");
      }
    }
  }  /* while loop */

  /* Just in case we are disassembling outside of the address range, force the myPCEnd to EOF */
  myPCEnd = myAppData.end + myOffset;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int DiStella::mark(uInt32 address, uInt8 mask, bool directive)
{
  /*-----------------------------------------------------------------------
    For any given offset and code range...

    If we're between the offset and the end of the code range, we mark
    the bit in the labels array for that data.  The labels array is an
    array of label info for each code address.  If this is the case,
    return "1", else...

    We sweep for hardware/system equates, which are valid addresses,
    outside the scope of the code/data range.  For these, we mark its
    corresponding hardware/system array element, and return "2" or "3"
    (depending on which system/hardware element was accessed).
    If this was not the case...

    Next we check if it is a code "mirror".  For the 2600, address ranges
    are limited with 13 bits, so other addresses can exist outside of the
    standard code/data range.  For these, we mark the element in the "labels"
    array that corresponds to the mirrored address, and return "4"

    If all else fails, it's not a valid address, so return 0.

    A quick example breakdown for a 2600 4K cart:
    ===========================================================
      $00-$3d     = system equates (WSYNC, etc...); return 2.
      $80-$ff     = zero-page RAM (ram_80, etc...); return 5.
      $0280-$0297 = system equates (INPT0, etc...); mark the array's element
                    with the appropriate bit; return 3.
      $1000-$1FFF = mark the code/data array for the mirrored address
                    with the appropriate bit; return 4.
      $3000-$3FFF = mark the code/data array for the mirrored address
                    with the appropriate bit; return 4.
      $5000-$5FFF = mark the code/data array for the mirrored address
                    with the appropriate bit; return 4.
      $7000-$7FFF = mark the code/data array for the mirrored address
                    with the appropriate bit; return 4.
      $9000-$9FFF = mark the code/data array for the mirrored address
                    with the appropriate bit; return 4.
      $B000-$BFFF = mark the code/data array for the mirrored address
                    with the appropriate bit; return 4.
      $D000-$DFFF = mark the code/data array for the mirrored address
                    with the appropriate bit; return 4.
      $F000-$FFFF = mark the code/data array for the address
                    with the appropriate bit; return 1.
      Anything else = invalid, return 0.
    ===========================================================
  -----------------------------------------------------------------------*/

  // Check for equates before ROM/ZP-RAM accesses, because the original logic
  // of Distella assumed either equates or ROM; it didn't take ZP-RAM into account
  CartDebug::AddrType type = myDbg.addressType(address);
  if (type == CartDebug::ADDR_TIA)
  {
    return 2;
  }
  else if (type == CartDebug::ADDR_IO)
  {
    return 3;
  }
  else if (type == CartDebug::ADDR_ZPRAM && myOffset != 0)
  {
    return 5;
  }
  else if (address >= myOffset && address <= myAppData.end + myOffset)
  {
    myLabels[address-myOffset] = myLabels[address-myOffset] | mask;
    if(directive)  myDirectives[address-myOffset] = mask;
    return 1;
  }
  else if (address > 0x1000 && myOffset != 0)  // Exclude zero-page accesses
  {
    /* 2K & 4K case */
    myLabels[address & myAppData.end] = myLabels[address & myAppData.end] | mask;
    if(directive)  myDirectives[address & myAppData.end] = mask;
    return 4;
  }
  else
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool DiStella::check_bit(uInt16 address, uInt8 mask) const
{
  // The REFERENCED and VALID_ENTRY flags are needed for any inspection of
  // an address
  // Since they're set only in the labels array (as the lower two bits),
  // they must be included in the other bitfields
  uInt8 label     = myLabels[address & myAppData.end],
        lastbits  = label & 0x03,
        directive = myDirectives[address & myAppData.end] & 0xFC,
        debugger  = Debugger::debugger().getAccessFlags(address | myOffset) & 0xFC;

  // Any address marked by a manual directive always takes priority
  if(directive)
    return (directive | lastbits) & mask;
  // Next, the results from a dynamic/runtime analysis are used
  else if((debugger | lastbits) & mask)
    return true;
  // Otherwise, default to static analysis from Distella
  else
    return label & mask;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool DiStella::check_range(uInt16 beg, uInt16 end) const
{
  if(beg > end)
  {
    cerr << "Beginning of range greater than end: start = " << hex << beg
         << ", end = " << hex << end << endl;
    return false;
  }
  else if(beg > myAppData.end + myOffset)
  {
    cerr << "Beginning of range out of range: start = " << hex << beg
         << ", range = " << hex << (myAppData.end + myOffset) << endl;
    return false;
  }
  else if(beg < myOffset)
  {
    cerr << "Beginning of range out of range: start = " << hex << beg
         << ", offset = " << hex << myOffset << endl;
    return false;
  }
  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DiStella::addEntry(CartDebug::DisasmType type)
{
  CartDebug::DisassemblyTag tag;

  // Type
  tag.type = type;

  // Address
  myDisasmBuf.seekg(0, ios::beg);
  if(myDisasmBuf.peek() == ' ')
    tag.address = 0;
  else
    myDisasmBuf >> setw(4) >> hex >> tag.address;

  // Only include addresses within the requested range
  if(tag.address < myAppData.start)
    goto DONE_WITH_ADD;

  // Label (a user-defined label always overrides any auto-generated one)
  myDisasmBuf.seekg(5, ios::beg);
  if(tag.address)
  {
    tag.label = myDbg.getLabel(tag.address, true);
    tag.hllabel = true;
    if(tag.label == EmptyString)
    {
      if(myDisasmBuf.peek() != ' ')
        getline(myDisasmBuf, tag.label, '\'');
      else if(mySettings.show_addresses && tag.type == CartDebug::CODE)
      {
        // Have addresses indented, to differentiate from actual labels
        tag.label = " " + Base::toString(tag.address, Base::F_16_4);
        tag.hllabel = false;
      }
    }
  }

  // Disassembly
  // Up to this point the field sizes are fixed, until we get to
  // variable length labels, cycle counts, etc
  myDisasmBuf.seekg(11, ios::beg);
  switch(tag.type)
  {
    case CartDebug::CODE:
      getline(myDisasmBuf, tag.disasm, '\'');
      getline(myDisasmBuf, tag.ccount, '\'');
      getline(myDisasmBuf, tag.bytes);

      // Make note of when we override CODE sections from the debugger
      // It could mean that the code hasn't been accessed up to this point,
      // but it could also indicate that code will *never* be accessed
      // Since it is impossible to tell the difference, marking the address
      // in the disassembly at least tells the user about it
      if(!(Debugger::debugger().getAccessFlags(tag.address) & CartDebug::CODE)
         && myOffset != 0)
      {
        tag.ccount += " *";
        Debugger::debugger().setAccessFlags(tag.address, CartDebug::TCODE);
      }
      break;
    case CartDebug::GFX:
    case CartDebug::PGFX:
      getline(myDisasmBuf, tag.disasm, '\'');
      getline(myDisasmBuf, tag.bytes);
      break;
    case CartDebug::DATA:
      getline(myDisasmBuf, tag.disasm, '\'');
      getline(myDisasmBuf, tag.bytes);
      break;
    case CartDebug::ROW:
      getline(myDisasmBuf, tag.disasm);
      break;
    default:  // should never happen
      tag.disasm = " ";
      break;
  }
  myList.push_back(tag);

DONE_WITH_ADD:
  myDisasmBuf.clear();
  myDisasmBuf.str("");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void DiStella::processDirectives(const CartDebug::DirectiveList& directives)
{
  for(CartDebug::DirectiveList::const_iterator i = directives.begin();
      i != directives.end(); ++i)
  {
    const CartDebug::DirectiveTag& tag = *i;
    if(check_range(tag.start, tag.end))
      for(uInt32 k = tag.start; k <= tag.end; ++k)
        mark(k, tag.type, true);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DiStella::Settings DiStella::settings = {
  Base::F_2, // gfx_format
  true,      // resolve_code (opposite of -d in Distella)
  true,      // show_addresses (not used externally; always off)
  false,     // aflag (-a in Distella)
  true,      // fflag (-f in Distella)
  false,     // rflag (-r in Distella)
  9          // number of bytes to use with .byte directive
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const DiStella::Instruction_tag DiStella::ourLookup[256] = {
/****  Positive  ****/

  /* 00 */ { "BRK", IMPLIED,     M_NONE, NONE,  7 }, /* Pseudo Absolute */
  /* 01 */ { "ORA", INDIRECT_X,  M_INDX, READ,  6 }, /* (Indirect,X) */
  /* 02 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* 03 */ { "slo", INDIRECT_X,  M_INDX, WRITE, 8 },

  /* 04 */ { "nop", ZERO_PAGE,   M_NONE, NONE,  3 },
  /* 05 */ { "ORA", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* 06 */ { "ASL", ZERO_PAGE,   M_ZERO, WRITE, 5 }, /* Zeropage */
  /* 07 */ { "slo", ZERO_PAGE,   M_ZERO, WRITE, 5 },

  /* 08 */ { "PHP", IMPLIED,     M_SR,   NONE,  3 },
  /* 09 */ { "ORA", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* 0a */ { "ASL", ACCUMULATOR, M_AC,   WRITE, 2 }, /* Accumulator */
  /* 0b */ { "anc", IMMEDIATE,   M_ACIM, READ,  2 },

  /* 0c */ { "nop", ABSOLUTE,    M_NONE, NONE,  4 },
  /* 0d */ { "ORA", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* 0e */ { "ASL", ABSOLUTE,    M_ABS,  WRITE, 6 }, /* Absolute */
  /* 0f */ { "slo", ABSOLUTE,    M_ABS,  WRITE, 6 },

  /* 10 */ { "BPL", RELATIVE,    M_REL,  READ,  2 },
  /* 11 */ { "ORA", INDIRECT_Y,  M_INDY, READ,  5 }, /* (Indirect),Y */
  /* 12 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* 13 */ { "slo", INDIRECT_Y,  M_INDY, WRITE, 8 },

  /* 14 */ { "nop", ZERO_PAGE_X, M_NONE, NONE,  4 },
  /* 15 */ { "ORA", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* 16 */ { "ASL", ZERO_PAGE_X, M_ZERX, WRITE, 6 }, /* Zeropage,X */
  /* 17 */ { "slo", ZERO_PAGE_X, M_ZERX, WRITE, 6 },

  /* 18 */ { "CLC", IMPLIED,     M_NONE, NONE,  2 },
  /* 19 */ { "ORA", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* 1a */ { ".nop",IMPLIED,     M_NONE, NONE,  2 },
  /* 1b */ { "slo", ABSOLUTE_Y,  M_ABSY, WRITE, 7 },

  /* 1c */ { "nop", ABSOLUTE_X,  M_NONE, NONE,  4 },
  /* 1d */ { "ORA", ABSOLUTE_X,  M_ABSX, READ,  4 }, /* Absolute,X */
  /* 1e */ { "ASL", ABSOLUTE_X,  M_ABSX, WRITE, 7 }, /* Absolute,X */
  /* 1f */ { "slo", ABSOLUTE_X,  M_ABSX, WRITE, 7 },

  /* 20 */ { "JSR", ABSOLUTE,    M_ADDR, READ,  6 },
  /* 21 */ { "AND", INDIRECT_X,  M_INDX, READ,  6 }, /* (Indirect ,X) */
  /* 22 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* 23 */ { "rla", INDIRECT_X,  M_INDX, WRITE, 8 },

  /* 24 */ { "BIT", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* 25 */ { "AND", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* 26 */ { "ROL", ZERO_PAGE,   M_ZERO, WRITE, 5 }, /* Zeropage */
  /* 27 */ { "rla", ZERO_PAGE,   M_ZERO, WRITE, 5 },

  /* 28 */ { "PLP", IMPLIED,     M_NONE, NONE,  4 },
  /* 29 */ { "AND", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* 2a */ { "ROL", ACCUMULATOR, M_AC,   WRITE, 2 }, /* Accumulator */
  /* 2b */ { ".anc",IMMEDIATE,   M_ACIM, READ,  2 },

  /* 2c */ { "BIT", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* 2d */ { "AND", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* 2e */ { "ROL", ABSOLUTE,    M_ABS,  WRITE, 6 }, /* Absolute */
  /* 2f */ { "rla", ABSOLUTE,    M_ABS,  WRITE, 6 },

  /* 30 */ { "BMI", RELATIVE,    M_REL,  READ,  2 },
  /* 31 */ { "AND", INDIRECT_Y,  M_INDY, READ,  5 }, /* (Indirect),Y */
  /* 32 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* 33 */ { "rla", INDIRECT_Y,  M_INDY, WRITE, 8 },

  /* 34 */ { ".nop",ZERO_PAGE_X, M_NONE, NONE,  4 },
  /* 35 */ { "AND", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* 36 */ { "ROL", ZERO_PAGE_X, M_ZERX, WRITE, 6 }, /* Zeropage,X */
  /* 37 */ { "rla", ZERO_PAGE_X, M_ZERX, WRITE, 6 },

  /* 38 */ { "SEC", IMPLIED,     M_NONE, NONE,  2 },
  /* 39 */ { "AND", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* 3a */ { ".nop",IMPLIED,     M_NONE, NONE,  2 },
  /* 3b */ { "rla", ABSOLUTE_Y,  M_ABSY, WRITE, 7 },

  /* 3c */ { ".nop",ABSOLUTE_X,  M_NONE, NONE,  4 },
  /* 3d */ { "AND", ABSOLUTE_X,  M_ABSX, READ,  4 }, /* Absolute,X */
  /* 3e */ { "ROL", ABSOLUTE_X,  M_ABSX, WRITE, 7 }, /* Absolute,X */
  /* 3f */ { "rla", ABSOLUTE_X,  M_ABSX, WRITE, 7 },

  /* 40 */ { "RTI", IMPLIED,     M_NONE, NONE,  6 },
  /* 41 */ { "EOR", INDIRECT_X,  M_INDX, READ,  6 }, /* (Indirect,X) */
  /* 42 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* 43 */ { "sre", INDIRECT_X,  M_INDX, WRITE, 8 },

  /* 44 */ { ".nop",ZERO_PAGE,   M_NONE, NONE,  3 },
  /* 45 */ { "EOR", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* 46 */ { "LSR", ZERO_PAGE,   M_ZERO, WRITE, 5 }, /* Zeropage */
  /* 47 */ { "sre", ZERO_PAGE,   M_ZERO, WRITE, 5 },

  /* 48 */ { "PHA", IMPLIED,     M_AC,   NONE,  3 },
  /* 49 */ { "EOR", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* 4a */ { "LSR", ACCUMULATOR, M_AC,   WRITE, 2 }, /* Accumulator */
  /* 4b */ { "asr", IMMEDIATE,   M_ACIM, READ,  2 }, /* (AC & IMM) >>1 */

  /* 4c */ { "JMP", ABSOLUTE,    M_ADDR, READ,  3 }, /* Absolute */
  /* 4d */ { "EOR", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* 4e */ { "LSR", ABSOLUTE,    M_ABS,  WRITE, 6 }, /* Absolute */
  /* 4f */ { "sre", ABSOLUTE,    M_ABS,  WRITE, 6 },

  /* 50 */ { "BVC", RELATIVE,    M_REL,  READ,  2 },
  /* 51 */ { "EOR", INDIRECT_Y,  M_INDY, READ,  5 }, /* (Indirect),Y */
  /* 52 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* 53 */ { "sre", INDIRECT_Y,  M_INDY, WRITE, 8 },

  /* 54 */ { ".nop",ZERO_PAGE_X, M_NONE, NONE,  4 },
  /* 55 */ { "EOR", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* 56 */ { "LSR", ZERO_PAGE_X, M_ZERX, WRITE, 6 }, /* Zeropage,X */
  /* 57 */ { "sre", ZERO_PAGE_X, M_ZERX, WRITE, 6 },

  /* 58 */ { "CLI", IMPLIED,     M_NONE, NONE,  2 },
  /* 59 */ { "EOR", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* 5a */ { ".nop",IMPLIED,     M_NONE, NONE,  2 },
  /* 5b */ { "sre", ABSOLUTE_Y,  M_ABSY, WRITE, 7 },

  /* 5c */ { ".nop",ABSOLUTE_X,  M_NONE, NONE,  4 },
  /* 5d */ { "EOR", ABSOLUTE_X,  M_ABSX, READ,  4 }, /* Absolute,X */
  /* 5e */ { "LSR", ABSOLUTE_X,  M_ABSX, WRITE, 7 }, /* Absolute,X */
  /* 5f */ { "sre", ABSOLUTE_X,  M_ABSX, WRITE, 7 },

  /* 60 */ { "RTS", IMPLIED,     M_NONE, NONE,  6 },
  /* 61 */ { "ADC", INDIRECT_X,  M_INDX, READ,  6 }, /* (Indirect,X) */
  /* 62 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* 63 */ { "rra", INDIRECT_X,  M_INDX, WRITE, 8 },

  /* 64 */ { ".nop",ZERO_PAGE,   M_NONE, NONE,  3 },
  /* 65 */ { "ADC", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* 66 */ { "ROR", ZERO_PAGE,   M_ZERO, WRITE, 5 }, /* Zeropage */
  /* 67 */ { "rra", ZERO_PAGE,   M_ZERO, WRITE, 5 },

  /* 68 */ { "PLA", IMPLIED,     M_NONE, NONE,  4 },
  /* 69 */ { "ADC", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* 6a */ { "ROR", ACCUMULATOR, M_AC,   WRITE, 2 }, /* Accumulator */
  /* 6b */ { "arr", IMMEDIATE,   M_ACIM, READ,  2 }, /* ARR isn't typo */

  /* 6c */ { "JMP", ABS_INDIRECT,M_AIND, READ,  5 }, /* Indirect */
  /* 6d */ { "ADC", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* 6e */ { "ROR", ABSOLUTE,    M_ABS,  WRITE, 6 }, /* Absolute */
  /* 6f */ { "rra", ABSOLUTE,    M_ABS,  WRITE, 6 },

  /* 70 */ { "BVS", RELATIVE,    M_REL,  READ,  2 },
  /* 71 */ { "ADC", INDIRECT_Y,  M_INDY, READ,  5 }, /* (Indirect),Y */
  /* 72 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT relative? */
  /* 73 */ { "rra", INDIRECT_Y,  M_INDY, WRITE, 8 },

  /* 74 */ { ".nop",ZERO_PAGE_X, M_NONE, NONE,  4 },
  /* 75 */ { "ADC", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* 76 */ { "ROR", ZERO_PAGE_X, M_ZERX, WRITE, 6 }, /* Zeropage,X */
  /* 77 */ { "rra", ZERO_PAGE_X, M_ZERX, WRITE, 6 },

  /* 78 */ { "SEI", IMPLIED,     M_NONE, NONE,  2 },
  /* 79 */ { "ADC", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* 7a */ { ".nop",IMPLIED,     M_NONE, NONE,  2 },
  /* 7b */ { "rra", ABSOLUTE_Y,  M_ABSY, WRITE, 7 },

  /* 7c */ { ".nop",ABSOLUTE_X,  M_NONE, NONE,  4 },
  /* 7d */ { "ADC", ABSOLUTE_X,  M_ABSX, READ,  4 },  /* Absolute,X */
  /* 7e */ { "ROR", ABSOLUTE_X,  M_ABSX, WRITE, 7 },  /* Absolute,X */
  /* 7f */ { "rra", ABSOLUTE_X,  M_ABSX, WRITE, 7 },

  /****  Negative  ****/

  /* 80 */ { "nop", IMMEDIATE,   M_NONE, NONE,  2 },
  /* 81 */ { "STA", INDIRECT_X,  M_AC,   WRITE, 6 }, /* (Indirect,X) */
  /* 82 */ { ".nop",IMMEDIATE,   M_NONE, NONE,  2 },
  /* 83 */ { "sax", INDIRECT_X,  M_ANXR, WRITE, 6 },

  /* 84 */ { "STY", ZERO_PAGE,   M_YR,   WRITE, 3 }, /* Zeropage */
  /* 85 */ { "STA", ZERO_PAGE,   M_AC,   WRITE, 3 }, /* Zeropage */
  /* 86 */ { "STX", ZERO_PAGE,   M_XR,   WRITE, 3 }, /* Zeropage */
  /* 87 */ { "sax", ZERO_PAGE,   M_ANXR, WRITE, 3 },

  /* 88 */ { "DEY", IMPLIED,     M_YR,   NONE,  2 },
  /* 89 */ { ".nop",IMMEDIATE,   M_NONE, NONE,  2 },
  /* 8a */ { "TXA", IMPLIED,     M_XR,   NONE,  2 },
  /****  very abnormal: usually AC = AC | #$EE & XR & #$oper  ****/
  /* 8b */ { "ane", IMMEDIATE,   M_AXIM, READ,  2 },

  /* 8c */ { "STY", ABSOLUTE,    M_YR,   WRITE, 4 }, /* Absolute */
  /* 8d */ { "STA", ABSOLUTE,    M_AC,   WRITE, 4 }, /* Absolute */
  /* 8e */ { "STX", ABSOLUTE,    M_XR,   WRITE, 4 }, /* Absolute */
  /* 8f */ { "sax", ABSOLUTE,    M_ANXR, WRITE, 4 },

  /* 90 */ { "BCC", RELATIVE,    M_REL,  READ,  2 },
  /* 91 */ { "STA", INDIRECT_Y,  M_AC,   WRITE, 6 }, /* (Indirect),Y */
  /* 92 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT relative? */
  /* 93 */ { "sha", INDIRECT_Y,  M_ANXR, WRITE, 6 },

  /* 94 */ { "STY", ZERO_PAGE_X, M_YR,   WRITE, 4 }, /* Zeropage,X */
  /* 95 */ { "STA", ZERO_PAGE_X, M_AC,   WRITE, 4 }, /* Zeropage,X */
  /* 96 */ { "STX", ZERO_PAGE_Y, M_XR,   WRITE, 4 }, /* Zeropage,Y */
  /* 97 */ { "sax", ZERO_PAGE_Y, M_ANXR, WRITE, 4 },

  /* 98 */ { "TYA", IMPLIED,     M_YR,   NONE,  2 },
  /* 99 */ { "STA", ABSOLUTE_Y,  M_AC,   WRITE, 5 }, /* Absolute,Y */
  /* 9a */ { "TXS", IMPLIED,     M_XR,   NONE,  2 },
  /*** This is very mysterious comm AND ... */
  /* 9b */ { "shs", ABSOLUTE_Y,  M_ANXR, WRITE, 5 },

  /* 9c */ { "shy", ABSOLUTE_X,  M_YR,   WRITE, 5 },
  /* 9d */ { "STA", ABSOLUTE_X,  M_AC,   WRITE, 5 }, /* Absolute,X */
  /* 9e */ { "shx", ABSOLUTE_Y,  M_XR  , WRITE, 5 },
  /* 9f */ { "sha", ABSOLUTE_Y,  M_ANXR, WRITE, 5 },

  /* a0 */ { "LDY", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* a1 */ { "LDA", INDIRECT_X,  M_INDX, READ,  6 }, /* (indirect,X) */
  /* a2 */ { "LDX", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* a3 */ { "lax", INDIRECT_X,  M_INDX, READ,  6 }, /* (indirect,X) */

  /* a4 */ { "LDY", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* a5 */ { "LDA", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* a6 */ { "LDX", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* a7 */ { "lax", ZERO_PAGE,   M_ZERO, READ,  3 },

  /* a8 */ { "TAY", IMPLIED,     M_AC,   NONE,  2 },
  /* a9 */ { "LDA", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* aa */ { "TAX", IMPLIED,     M_AC,   NONE,  2 },
  /* ab */ { "lxa", IMMEDIATE,   M_ACIM, READ,  2 }, /* LXA isn't a typo */

  /* ac */ { "LDY", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* ad */ { "LDA", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* ae */ { "LDX", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* af */ { "lax", ABSOLUTE,    M_ABS,  READ,  4 },

  /* b0 */ { "BCS", RELATIVE,    M_REL,  READ,  2 },
  /* b1 */ { "LDA", INDIRECT_Y,  M_INDY, READ,  5 }, /* (indirect),Y */
  /* b2 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* b3 */ { "lax", INDIRECT_Y,  M_INDY, READ,  5 },

  /* b4 */ { "LDY", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* b5 */ { "LDA", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* b6 */ { "LDX", ZERO_PAGE_Y, M_ZERY, READ,  4 }, /* Zeropage,Y */
  /* b7 */ { "lax", ZERO_PAGE_Y, M_ZERY, READ,  4 },

  /* b8 */ { "CLV", IMPLIED,     M_NONE, NONE,  2 },
  /* b9 */ { "LDA", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* ba */ { "TSX", IMPLIED,     M_SP,   NONE,  2 },
  /* bb */ { "las", ABSOLUTE_Y,  M_SABY, READ,  4 },

  /* bc */ { "LDY", ABSOLUTE_X,  M_ABSX, READ,  4 }, /* Absolute,X */
  /* bd */ { "LDA", ABSOLUTE_X,  M_ABSX, READ,  4 }, /* Absolute,X */
  /* be */ { "LDX", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* bf */ { "lax", ABSOLUTE_Y,  M_ABSY, READ,  4 },

  /* c0 */ { "CPY", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* c1 */ { "CMP", INDIRECT_X,  M_INDX, READ,  6 }, /* (Indirect,X) */
  /* c2 */ { ".nop",IMMEDIATE,   M_NONE, NONE,  2 }, /* occasional TILT */
  /* c3 */ { "dcp", INDIRECT_X,  M_INDX, WRITE, 8 },

  /* c4 */ { "CPY", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* c5 */ { "CMP", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* c6 */ { "DEC", ZERO_PAGE,   M_ZERO, WRITE, 5 }, /* Zeropage */
  /* c7 */ { "dcp", ZERO_PAGE,   M_ZERO, WRITE, 5 },

  /* c8 */ { "INY", IMPLIED,     M_YR,   NONE,  2 },
  /* c9 */ { "CMP", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* ca */ { "DEX", IMPLIED,     M_XR,   NONE,  2 },
  /* cb */ { "sbx", IMMEDIATE,   M_IMM,  READ,  2 },

  /* cc */ { "CPY", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* cd */ { "CMP", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* ce */ { "DEC", ABSOLUTE,    M_ABS,  WRITE, 6 }, /* Absolute */
  /* cf */ { "dcp", ABSOLUTE,    M_ABS,  WRITE, 6 },

  /* d0 */ { "BNE", RELATIVE,    M_REL,  READ,  2 },
  /* d1 */ { "CMP", INDIRECT_Y,  M_INDY, READ,  5 }, /* (Indirect),Y */
  /* d2 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* d3 */ { "dcp", INDIRECT_Y,  M_INDY, WRITE, 8 },

  /* d4 */ { ".nop",ZERO_PAGE_X, M_NONE, NONE,  4 },
  /* d5 */ { "CMP", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* d6 */ { "DEC", ZERO_PAGE_X, M_ZERX, WRITE, 6 }, /* Zeropage,X */
  /* d7 */ { "dcp", ZERO_PAGE_X, M_ZERX, WRITE, 6 },

  /* d8 */ { "CLD", IMPLIED,     M_NONE, NONE,  2 },
  /* d9 */ { "CMP", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* da */ { ".nop",IMPLIED,     M_NONE, NONE,  2 },
  /* db */ { "dcp", ABSOLUTE_Y,  M_ABSY, WRITE, 7 },

  /* dc */ { ".nop",ABSOLUTE_X,  M_NONE, NONE,  4 },
  /* dd */ { "CMP", ABSOLUTE_X,  M_ABSX, READ,  4 }, /* Absolute,X */
  /* de */ { "DEC", ABSOLUTE_X,  M_ABSX, WRITE, 7 }, /* Absolute,X */
  /* df */ { "dcp", ABSOLUTE_X,  M_ABSX, WRITE, 7 },

  /* e0 */ { "CPX", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* e1 */ { "SBC", INDIRECT_X,  M_INDX, READ,  6 }, /* (Indirect,X) */
  /* e2 */ { ".nop",IMMEDIATE,   M_NONE, NONE,  2 },
  /* e3 */ { "isb", INDIRECT_X,  M_INDX, WRITE, 8 },

  /* e4 */ { "CPX", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* e5 */ { "SBC", ZERO_PAGE,   M_ZERO, READ,  3 }, /* Zeropage */
  /* e6 */ { "INC", ZERO_PAGE,   M_ZERO, WRITE, 5 }, /* Zeropage */
  /* e7 */ { "isb", ZERO_PAGE,   M_ZERO, WRITE, 5 },

  /* e8 */ { "INX", IMPLIED,     M_XR,   NONE,  2 },
  /* e9 */ { "SBC", IMMEDIATE,   M_IMM,  READ,  2 }, /* Immediate */
  /* ea */ { "NOP", IMPLIED,     M_NONE, NONE,  2 },
  /* eb */ { ".sbc",IMMEDIATE,   M_IMM,  READ,  2 }, /* same as e9 */

  /* ec */ { "CPX", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* ed */ { "SBC", ABSOLUTE,    M_ABS,  READ,  4 }, /* Absolute */
  /* ee */ { "INC", ABSOLUTE,    M_ABS,  WRITE, 6 }, /* Absolute */
  /* ef */ { "isb", ABSOLUTE,    M_ABS,  WRITE, 6 },

  /* f0 */ { "BEQ", RELATIVE,    M_REL,  READ,  2 },
  /* f1 */ { "SBC", INDIRECT_Y,  M_INDY, READ,  5 }, /* (Indirect),Y */
  /* f2 */ { ".jam",IMPLIED,     M_NONE, NONE,  0 }, /* TILT */
  /* f3 */ { "isb", INDIRECT_Y,  M_INDY, WRITE, 8 },

  /* f4 */ { ".nop",ZERO_PAGE_X, M_NONE, NONE,  4 },
  /* f5 */ { "SBC", ZERO_PAGE_X, M_ZERX, READ,  4 }, /* Zeropage,X */
  /* f6 */ { "INC", ZERO_PAGE_X, M_ZERX, WRITE, 6 }, /* Zeropage,X */
  /* f7 */ { "isb", ZERO_PAGE_X, M_ZERX, WRITE, 6 },

  /* f8 */ { "SED", IMPLIED,     M_NONE, NONE,  2 },
  /* f9 */ { "SBC", ABSOLUTE_Y,  M_ABSY, READ,  4 }, /* Absolute,Y */
  /* fa */ { ".nop",IMPLIED,     M_NONE, NONE,  2 },
  /* fb */ { "isb", ABSOLUTE_Y,  M_ABSY, WRITE, 7 },

  /* fc */ { ".nop",ABSOLUTE_X,  M_NONE, NONE,  4 },
  /* fd */ { "SBC", ABSOLUTE_X,  M_ABSX, READ,  4 }, /* Absolute,X */
  /* fe */ { "INC", ABSOLUTE_X,  M_ABSX, WRITE, 7 }, /* Absolute,X */
  /* ff */ { "isb", ABSOLUTE_X,  M_ABSX, WRITE, 7 }
};
