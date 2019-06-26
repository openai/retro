/***************************************************************************
 *   Copyright (C) 2007-2010 by Sindre Aamås                               *
 *   aamas@stud.ntnu.no                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License version 2 for more details.                *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   version 2 along with this program; if not, write to the               *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "cartridge.h"
#include "../savestate.h"
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <algorithm>

namespace gambatte
{

   static unsigned toMulti64Rombank(const unsigned rombank)
   {
      return (rombank >> 1 & 0x30) | (rombank & 0xF);
   }

   static inline unsigned rambanks(MemPtrs const &memptrs)
   {
      return (memptrs.rambankdataend() - memptrs.rambankdata()) / 0x2000;
   }

   static inline unsigned rombanks(MemPtrs const &memptrs)
   {
      return (memptrs.romdataend()     - memptrs.romdata()    ) / 0x4000;
   }

   class DefaultMbc : public Mbc {
      public:
         virtual bool isAddressWithinAreaRombankCanBeMappedTo(unsigned addr, unsigned bank) const {
            return (addr< 0x4000) == (bank == 0);
         }
   };
   class Mbc0 : public DefaultMbc {
      MemPtrs &memptrs;
      bool enableRam;
      public:
      explicit Mbc0(MemPtrs &memptrs)
         : memptrs(memptrs),
         enableRam(false)
      {
      }
      virtual void romWrite(const unsigned P, const unsigned data) {
         if (P < 0x2000) {
            enableRam = (data & 0xF) == 0xA;
            memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0, 0);
         }
      }
      virtual void saveState(SaveState::Mem &ss) const {
         ss.enableRam = enableRam;
      }
      virtual void loadState(const SaveState::Mem &ss) {
         enableRam = ss.enableRam;
         memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0, 0);
      }
   };

   class Mbc1 : public DefaultMbc {
      MemPtrs &memptrs;
      unsigned char rombank;
      unsigned char rambank;
      bool enableRam;
      bool rambankMode;
      static unsigned adjustedRombank(unsigned bank) { return (bank & 0x1F) ? bank : bank | 1; }
      void setRambank() const { memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0, rambank & (rambanks(memptrs) - 1)); }
      void setRombank() const { memptrs.setRombank(adjustedRombank(rombank) & (rombanks(memptrs) - 1)); }
      public:
      explicit Mbc1(MemPtrs &memptrs)
         : memptrs(memptrs),
         rombank(1),
         rambank(0),
         enableRam(false),
         rambankMode(false)
      {
      }
      virtual void romWrite(const unsigned P, const unsigned data) {
         switch (P >> 13 & 3) {
            case 0:
               enableRam = (data & 0xF) == 0xA;
               setRambank();
               break;
            case 1:
               rombank = rambankMode ? data & 0x1F : (rombank & 0x60) | (data & 0x1F);
               setRombank();
               break;
            case 2:
               if (rambankMode) {
                  rambank = data & 3;
                  setRambank();
               } else {
                  rombank = (data << 5 & 0x60) | (rombank & 0x1F);
                  setRombank();
               }
               break;
            case 3:
               // Pretty sure this should take effect immediately, but I have a policy not to change old behavior
               // unless I have something (eg. a verified test or a game) that justifies it.
               rambankMode = data & 1;
               break;
         }
      }
      virtual void saveState(SaveState::Mem &ss) const {
         ss.rombank = rombank;
         ss.rambank = rambank;
         ss.enableRam = enableRam;
         ss.rambankMode = rambankMode;
      }
      virtual void loadState(const SaveState::Mem &ss) {
         rombank = ss.rombank;
         rambank = ss.rambank;
         enableRam = ss.enableRam;
         rambankMode = ss.rambankMode;
         setRambank();
         setRombank();
      }
   };
   class Mbc1Multi64 : public Mbc {
      MemPtrs &memptrs;
      unsigned char rombank;
      bool enableRam;
      bool rombank0Mode;
      static unsigned adjustedRombank(unsigned bank) { return (bank & 0x1F) ? bank : bank | 1; }
      void setRombank() const {
         if (rombank0Mode) {
            const unsigned rb = toMulti64Rombank(rombank);
            memptrs.setRombank0(rb & 0x30);
            memptrs.setRombank(adjustedRombank(rb));
         } else {
            memptrs.setRombank0(0);
            memptrs.setRombank(adjustedRombank(rombank) & (rombanks(memptrs) - 1));
         }
      }
      public:
      explicit Mbc1Multi64(MemPtrs &memptrs)
         : memptrs(memptrs),
         rombank(1),
         enableRam(false),
         rombank0Mode(false)
      {
      }
      virtual void romWrite(const unsigned P, const unsigned data) {
         switch (P >> 13 & 3) {
            case 0:
               enableRam = (data & 0xF) == 0xA;
               memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0, 0);
               break;
            case 1:
               rombank = (rombank & 0x60) | (data & 0x1F);
               memptrs.setRombank(rombank0Mode
                        ? adjustedRombank(toMulti64Rombank(rombank))
                        : adjustedRombank(rombank) & (rombanks(memptrs) - 1));
               break;
            case 2:
               rombank = (data << 5 & 0x60) | (rombank & 0x1F);
               setRombank();
               break;
            case 3:
               rombank0Mode = data & 1;
               setRombank();
               break;
         }
      }
      virtual void saveState(SaveState::Mem &ss) const {
         ss.rombank = rombank;
         ss.enableRam = enableRam;
         ss.rambankMode = rombank0Mode;
      }
      virtual void loadState(const SaveState::Mem &ss) {
         rombank = ss.rombank;
         enableRam = ss.enableRam;
         rombank0Mode = ss.rambankMode;
         memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0, 0);
         setRombank();
      }
      virtual bool isAddressWithinAreaRombankCanBeMappedTo(unsigned addr, unsigned bank) const {
         return (addr < 0x4000) == ((bank & 0xF) == 0);
      }
   };
   class Mbc2 : public DefaultMbc {
      MemPtrs &memptrs;
      unsigned char rombank;
      bool enableRam;
      public:
      explicit Mbc2(MemPtrs &memptrs)
         : memptrs(memptrs),
         rombank(1),
         enableRam(false)
      {
      }
      virtual void romWrite(const unsigned P, const unsigned data) {
         switch (P & 0x6100) {
            case 0x0000:
               enableRam = (data & 0xF) == 0xA;
               memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0, 0);
               break;
            case 0x2100:
               rombank = data & 0xF;
               memptrs.setRombank(rombank & (rombanks(memptrs) - 1));
               break;
         }
      }
      virtual void saveState(SaveState::Mem &ss) const {
         ss.rombank = rombank;
         ss.enableRam = enableRam;
      }
      virtual void loadState(const SaveState::Mem &ss) {
         rombank = ss.rombank;
         enableRam = ss.enableRam;
         memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0, 0);
         memptrs.setRombank(rombank & (rombanks(memptrs) - 1));
      }
   };
   class Mbc3 : public DefaultMbc {
      MemPtrs &memptrs;
      Rtc *const rtc;
      unsigned char rombank;
      unsigned char rambank;
      bool enableRam;
      void setRambank() const {
         unsigned flags = enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0;
         if (rtc) {
            rtc->set(enableRam, rambank);
            if (rtc->getActive())
               flags |= MemPtrs::RTC_EN;
         }
         memptrs.setRambank(flags, rambank & (rambanks(memptrs) - 1));
      }
      public:
      Mbc3(MemPtrs &memptrs, Rtc *const rtc)
         : memptrs(memptrs),
         rtc(rtc),
         rombank(1),
         rambank(0),
         enableRam(false)
      {
      }
      virtual void romWrite(const unsigned P, const unsigned data) {
         switch (P >> 13 & 3) {
            case 0:
               enableRam = (data & 0xF) == 0xA;
               setRambank();
               break;
            case 1:
               rombank = data & 0x7F;
               setRombank();
               break;
            case 2:
               rambank = data;
               setRambank();
               break;
            case 3:
               if (rtc)
                  rtc->latch(data);
               break;
         }
      }
      virtual void saveState(SaveState::Mem &ss) const {
         ss.rombank = rombank;
         ss.rambank = rambank;
         ss.enableRam = enableRam;
      }
      virtual void loadState(const SaveState::Mem &ss) {
         rombank = ss.rombank;
         rambank = ss.rambank;
         enableRam = ss.enableRam;
         setRambank();
         setRombank();
      }

      void setRombank() const {
         memptrs.setRombank(std::max(rombank & (rombanks(memptrs) - 1), 1u));
      }
   };
   class HuC1 : public DefaultMbc {
      MemPtrs &memptrs;
      unsigned char rombank;
      unsigned char rambank;
      bool enableRam;
      bool rambankMode;
      void setRambank() const {
         memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : MemPtrs::READ_EN,
               rambankMode ? rambank & (rambanks(memptrs) - 1) : 0);
      }
      void setRombank() const { memptrs.setRombank((rambankMode ? rombank : rambank << 6 | rombank) & (rombanks(memptrs) - 1)); }
      public:
      explicit HuC1(MemPtrs &memptrs)
         : memptrs(memptrs),
         rombank(1),
         rambank(0),
         enableRam(false),
         rambankMode(false)
      {
      }
      virtual void romWrite(const unsigned P, const unsigned data) {
         switch (P >> 13 & 3) {
            case 0:
               enableRam = (data & 0xF) == 0xA;
               setRambank();
               break;
            case 1:
               rombank = data & 0x3F;
               setRombank();
               break;
            case 2:
               rambank = data & 3;
               rambankMode ? setRambank() : setRombank();
               break;
            case 3:
               rambankMode = data & 1;
               setRambank();
               setRombank();
               break;
         }
      }
      virtual void saveState(SaveState::Mem &ss) const {
         ss.rombank = rombank;
         ss.rambank = rambank;
         ss.enableRam = enableRam;
         ss.rambankMode = rambankMode;
      }
      virtual void loadState(const SaveState::Mem &ss) {
         rombank = ss.rombank;
         rambank = ss.rambank;
         enableRam = ss.enableRam;
         rambankMode = ss.rambankMode;
         setRambank();
         setRombank();
      }
   };
   class Mbc5 : public DefaultMbc {
      MemPtrs &memptrs;
      unsigned short rombank;
      unsigned char rambank;
      bool enableRam;
      static unsigned adjustedRombank(const unsigned bank) { return bank; }
      void setRambank() const {
         memptrs.setRambank(enableRam ? MemPtrs::READ_EN | MemPtrs::WRITE_EN : 0,
               rambank & (rambanks(memptrs) - 1));
      }
      void setRombank() const { memptrs.setRombank(adjustedRombank(rombank) & (rombanks(memptrs) - 1));}
      public:
      explicit Mbc5(MemPtrs &memptrs)
         : memptrs(memptrs),
         rombank(1),
         rambank(0),
         enableRam(false)
      {
      }
      virtual void romWrite(const unsigned P, const unsigned data) {
         switch (P >> 13 & 3) {
            case 0:
               enableRam = (data & 0xF) == 0xA;
               setRambank();
               break;
            case 1:
               rombank = P < 0x3000 ? (rombank & 0x100) | data
                  : (data << 8 & 0x100) | (rombank & 0xFF);
               setRombank();
               break;
            case 2:
               rambank = data & 0xF;
               setRambank();
               break;
            case 3:
               break;
         }
      }
      virtual void saveState(SaveState::Mem &ss) const {
         ss.rombank = rombank;
         ss.rambank = rambank;
         ss.enableRam = enableRam;
      }
      virtual void loadState(const SaveState::Mem &ss) {
         rombank = ss.rombank;
         rambank = ss.rambank;
         enableRam = ss.enableRam;
         setRambank();
         setRombank();
      }
   };

   static bool hasRtc(unsigned headerByte0x147)
   {
      switch (headerByte0x147)
      {
         case 0x0F:
         case 0x10:
            return true;
         default:
            return false;
      }
   }

   void Cartridge::setStatePtrs(SaveState &state)
   {
      state.mem.vram.set(memptrs_.vramdata(), memptrs_.vramdataend() - memptrs_.vramdata());
      state.mem.sram.set(memptrs_.rambankdata(), memptrs_.rambankdataend() - memptrs_.rambankdata());
      state.mem.wram.set(memptrs_.wramdata(0), memptrs_.wramdataend() - memptrs_.wramdata(0));
   }

   void Cartridge::saveState(SaveState &state) const
   {
      mbc->saveState(state.mem);
      rtc_.saveState(state);
   }

   void Cartridge::loadState(const SaveState &state)
   {
      rtc_.loadState(state);
      mbc->loadState(state.mem);
   }

   static void enforce8bit(unsigned char *data, unsigned long sz)
   {
      if (static_cast<unsigned char>(0x100))
         while (sz--)
            *data++ &= 0xFF;
   }

   static unsigned pow2ceil(unsigned n)
   {
      --n;
      n |= n >> 1;
      n |= n >> 2;
      n |= n >> 4;
      n |= n >> 8;
      ++n;

      return n;
   }

   int Cartridge::loadROM(const void *data, unsigned int romsize, unsigned int forceModel, const bool multiCartCompat)
   {
      const uint8_t *romdata = (uint8_t*)data;
      if (romsize < 0x4000 || !romdata)
         return -1;

      unsigned rambanks = 1;
      unsigned rombanks = 2;
      bool cgb = false;
      enum Cartridgetype { PLAIN, MBC1, MBC2, MBC3, MBC5, HUC1 } type = PLAIN;

      {
         unsigned i;
         unsigned char header[0x150];
         for (i = 0; i < 0x150; i++)
            header[i] = romdata[i];

         switch (header[0x0147])
         {
            case 0x00: type = PLAIN; break;
            case 0x01: type = MBC1; break;
            case 0x02: type = MBC1; break;
            case 0x03: type = MBC1; break;
            case 0x05: type = MBC2; break;
            case 0x06: type = MBC2; break;
            case 0x08: type = MBC2; break;
            case 0x09: type = MBC2;break;
            case 0x0B: printf("MM01 ROM not supported.\n"); return -1;
            case 0x0C: printf("MM01 ROM not supported.\n"); return -1;
            case 0x0D: printf("MM01 ROM not supported.\n"); return -1;
            case 0x0F: type = MBC3; break;
            case 0x10: type = MBC3; break;
            case 0x11: type = MBC3; break;
            case 0x12: type = MBC3; break;
            case 0x13: type = MBC3; break;
            case 0x15: printf("MBC4 ROM not supported.\n"); return -1;
            case 0x16: printf("MBC4 ROM not supported.\n"); return -1;
            case 0x17: printf("MBC4 ROM not supported.\n"); return -1;
            case 0x19: type = MBC5; break;
            case 0x1A: type = MBC5; break;
            case 0x1B: type = MBC5; break;
            case 0x1C: type = MBC5; break;
            case 0x1D: type = MBC5; break;
            case 0x1E: type = MBC5; break;
            case 0x20: printf("MBC6 ROM not supported.\n"); return -1;
            case 0x22: printf("MBC7 ROM not supported.\n"); return -1;
            case 0xFC: printf("Pocket Camera ROM not supported.\n"); return -1;
            case 0xFD: printf("Bandai TAMA5 ROM not supported.\n"); return -1;
            case 0xFE: printf("HuC3 ROM+RAM+BATTERY loaded.\n"); return -1;
            case 0xFF: type = HUC1; break;
            default: printf("Wrong data-format, corrupt or unsupported ROM.\n"); return -1;
         }

         switch (header[0x0149])
         {
               case 0x00: /*std::puts("No RAM");*/ rambanks = type == MBC2; break;
               case 0x01: /*std::puts("2kB RAM");*/ /*rambankrom=1; break;*/
               case 0x02: /*std::puts("8kB RAM");*/
                  rambanks = 1;
                  break;
               case 0x03: /*std::puts("32kB RAM");*/
                  rambanks = 4;
                  break;
               case 0x04: /*std::puts("128kB RAM");*/
                  rambanks = 16;
                  break;
               case 0x05: /*std::puts("undocumented kB RAM");*/
                  rambanks = 16;
                  break;
               default: /*std::puts("Wrong data-format, corrupt or unsupported ROM loaded.");*/
                  rambanks = 16;
                  break;
         }

         switch(forceModel){
            case 1://FORCE_DMG
               cgb = false;
               break;
            case 8://FORCE_CGB
               cgb = true;
               break;
            case 0://dont force anything
               cgb = header[0x0143] >> 7;
         }
      }

      rombanks = pow2ceil(romsize / 0x4000);

      ggUndoList_.clear();
      mbc.reset();
      memptrs_.reset(rombanks, rambanks, cgb ? 8 : 2);
      rtc_.set(false, 0);

      memcpy(memptrs_.romdata(), romdata, ((romsize / 0x4000) * 0x4000ul) * sizeof(unsigned char));
      std::memset(memptrs_.romdata() + (romsize / 0x4000) * 0x4000ul, 0xFF, (rombanks - romsize / 0x4000) * 0x4000ul);
      enforce8bit(memptrs_.romdata(), rombanks * 0x4000ul);

      switch (type)
      {
         case PLAIN: mbc.reset(new Mbc0(memptrs_)); break;
         case MBC1:
                     if (!rambanks && rombanks == 64 && multiCartCompat) {
                        std::puts("Multi-ROM \"MBC1\" presumed");
                        mbc.reset(new Mbc1Multi64(memptrs_));
                     } else
                        mbc.reset(new Mbc1(memptrs_));
                     break;
         case MBC2: mbc.reset(new Mbc2(memptrs_)); break;
         case MBC3: mbc.reset(new Mbc3(memptrs_, hasRtc(memptrs_.romdata()[0x147]) ? &rtc_ : 0)); break;
         case MBC5: mbc.reset(new Mbc5(memptrs_)); break;
         case HUC1: mbc.reset(new HuC1(memptrs_)); break;
      }

      return 0;
   }

   static int asHex(const char c)
   {
      return c >= 'A' ? c - 'A' + 0xA : c - '0';
   }

   void Cartridge::applyGameGenie(const std::string &code)
   {
      if (6 < code.length())
      {
         const unsigned val = (asHex(code[0]) << 4 | asHex(code[1])) & 0xFF;
         const unsigned addr = (asHex(code[2]) << 8 | asHex(code[4]) << 4 | asHex(code[5]) | (asHex(code[6]) ^ 0xF) << 12) & 0x7FFF;
         unsigned cmp = 0xFFFF;

         if (10 < code.length())
         {
            cmp = (asHex(code[8]) << 4 | asHex(code[10])) ^ 0xFF;
            cmp = ((cmp >> 2 | cmp << 6) ^ 0x45) & 0xFF;
         }

         for (unsigned bank = 0; bank < static_cast<std::size_t>(memptrs_.romdataend() - memptrs_.romdata()) / 0x4000; ++bank)
         {
            if (mbc->isAddressWithinAreaRombankCanBeMappedTo(addr, bank)
                  && (cmp > 0xFF || memptrs_.romdata()[bank * 0x4000ul + (addr & 0x3FFF)] == cmp))
            {
               ggUndoList_.push_back(AddrData(bank * 0x4000ul + (addr & 0x3FFF), memptrs_.romdata()[bank * 0x4000ul + (addr & 0x3FFF)]));
               memptrs_.romdata()[bank * 0x4000ul + (addr & 0x3FFF)] = val;
            }
         }
      }
   }

   void Cartridge::setGameGenie(const std::string &codes)
   {
#if 0
      if (loaded())
#endif
      {
         for (std::vector<AddrData>::reverse_iterator it = ggUndoList_.rbegin(), end = ggUndoList_.rend(); it != end; ++it)
         {
            if (memptrs_.romdata() + it->addr < memptrs_.romdataend())
               memptrs_.romdata()[it->addr] = it->data;
         }

         ggUndoList_.clear();

         std::string code;
         for (std::size_t pos = 0; pos < codes.length()
               && (code = codes.substr(pos, codes.find(';', pos) - pos), true); pos += code.length() + 1)
            applyGameGenie(code);
      }
   }

}

