/***************************************************************************
 *   Copyright (C) 2007 by Sindre Aamås                                    *
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
#include "video.h"
#include "savestate.h"
#include <cstring>
#include <algorithm>
#include <string>

namespace gambatte
{

void LCD::reset(const unsigned char *const oamram, unsigned char const *vram, const bool cgb)
{
   ppu_.reset(oamram, vram, cgb);
   lycIrq_.setCgb(cgb);
   refreshPalettes();
}

static unsigned long mode2IrqSchedule(const unsigned statReg, const LyCounter &lyCounter, const unsigned long cycleCounter)
{
   if (!(statReg & 0x20))
      return disabled_time;

   unsigned next = lyCounter.time() - cycleCounter;

   if (lyCounter.ly() >= 143 || (lyCounter.ly() == 142 && next <= 4) || (statReg & 0x08))
      next += (153u - lyCounter.ly()) * lyCounter.lineTime();
   else
   {
      if (next <= 4)
         next += lyCounter.lineTime();

      next -= 4;
   }

   return cycleCounter + next;
}

static inline unsigned long m0IrqTimeFromXpos166Time(
      const unsigned long xpos166Time, const bool cgb, const bool ds)
{
   return xpos166Time + cgb - ds;
}

static inline unsigned long hdmaTimeFromM0Time(
      const unsigned long m0Time, const bool ds)
{
   return m0Time + 1 - ds;
}

static unsigned long nextHdmaTime(const unsigned long lastM0Time,
      const unsigned long nextM0Time, const unsigned long cycleCounter, const bool ds)
{
   return cycleCounter < hdmaTimeFromM0Time(lastM0Time, ds)
      ? hdmaTimeFromM0Time(lastM0Time, ds)
      : hdmaTimeFromM0Time(nextM0Time, ds);
}

void LCD::setStatePtrs(SaveState &state)
{
   state.ppu.bgpData.set(  bgpData_, sizeof  bgpData_);
   state.ppu.objpData.set(objpData_, sizeof objpData_);
   ppu_.setStatePtrs(state);
}

void LCD::saveState(SaveState &state) const
{
   state.mem.hdmaTransfer = hdmaIsEnabled();
   state.ppu.nextM0Irq = eventTimes_(MODE0_IRQ) - ppu_.now();
   state.ppu.pendingLcdstatIrq = eventTimes_(ONESHOT_LCDSTATIRQ) != disabled_time;
   
   if (isCgb())
      std::memcpy(state.ppu.dmgPalette, dmgColorsGBC_, 8 * 3);
   

   lycIrq_.saveState(state);
   m0Irq_.saveState(state);
   ppu_.saveState(state);
}

void LCD::loadState(const SaveState &state, const unsigned char *const oamram)
{
   statReg_ = state.mem.ioamhram.get()[0x141];
   m2IrqStatReg_ = statReg_;
   m1IrqStatReg_ = statReg_;

   ppu_.loadState(state, oamram);
   lycIrq_.loadState(state);
   m0Irq_.loadState(state);

   if (ppu_.lcdc() & 0x80)
   {
      nextM0Time_.predictNextM0Time(ppu_);
      lycIrq_.reschedule(ppu_.lyCounter(), ppu_.now());

      eventTimes_.setm<ONESHOT_LCDSTATIRQ>(state.ppu.pendingLcdstatIrq
            ? ppu_.now() + 1 : static_cast<unsigned long>(disabled_time));
      eventTimes_.setm<ONESHOT_UPDATEWY2>(state.ppu.oldWy != state.mem.ioamhram.get()[0x14A]
            ? ppu_.now() + 1 : static_cast<unsigned long>(disabled_time));
      eventTimes_.set<LY_COUNT>(ppu_.lyCounter().time());
      eventTimes_.setm<SPRITE_MAP>(SpriteMapper::schedule(ppu_.lyCounter(), ppu_.now()));
      eventTimes_.setm<LYC_IRQ>(lycIrq_.time());
      eventTimes_.setm<MODE1_IRQ>(ppu_.lyCounter().nextFrameCycle(144 * 456, ppu_.now()));
      eventTimes_.setm<MODE2_IRQ>(mode2IrqSchedule(statReg_, ppu_.lyCounter(), ppu_.now()));
      eventTimes_.setm<MODE0_IRQ>((statReg_ & 0x08) ? ppu_.now() + state.ppu.nextM0Irq : static_cast<unsigned long>(disabled_time));
      eventTimes_.setm<HDMA_REQ>(state.mem.hdmaTransfer
            ? nextHdmaTime(ppu_.lastM0Time(), nextM0Time_.predictedNextM0Time(), ppu_.now(), isDoubleSpeed())
            : static_cast<unsigned long>(disabled_time));
   }
   else
   {
      for (int i = 0; i < NUM_MEM_EVENTS; ++i)
         eventTimes_.set(static_cast<MemEvent>(i), disabled_time);
   }

   if (isCgb())
      std::memcpy(dmgColorsGBC_, state.ppu.dmgPalette, 8 * 3);
   
   refreshPalettes();
}

void LCD::refreshPalettes()
{
   if (ppu_.cgb() && !ppu_.inDmgMode())
   {
      for (unsigned i = 0; i < 8 * 8; i += 2)
      {
         ppu_.bgPalette()[i >> 1] = gbcToRgb32( bgpData_[i] |  bgpData_[i + 1] << 8);
         ppu_.spPalette()[i >> 1] = gbcToRgb32(objpData_[i] | objpData_[i + 1] << 8);
      }
   }
   else
   {
      if (ppu_.inDmgMode())
      {
         for (unsigned i = 0; i < 8 * 3; i += 2)
             dmgColorsRgb32_[i >> 1] = gbcToRgb32( dmgColorsGBC_[i] |  dmgColorsGBC_[i + 1] << 8);
      }
      setDmgPalette(ppu_.bgPalette()    , dmgColorsRgb32_    ,  bgpData_[0]);
      setDmgPalette(ppu_.spPalette()    , dmgColorsRgb32_ + 4, objpData_[0]);
      setDmgPalette(ppu_.spPalette() + 4, dmgColorsRgb32_ + 8, objpData_[1]);
   }
}

void LCD::resetCc(const unsigned long oldCc, const unsigned long newCc)
{
   update(oldCc);
   ppu_.resetCc(oldCc, newCc);

   if (ppu_.lcdc() & 0x80)
   {
      const unsigned long dec = oldCc - newCc;

      nextM0Time_.invalidatePredictedNextM0Time();
      lycIrq_.reschedule(ppu_.lyCounter(), newCc);

      for (int i = 0; i < NUM_MEM_EVENTS; ++i)
      {
         if (eventTimes_(static_cast<MemEvent>(i)) != disabled_time)
            eventTimes_.set(static_cast<MemEvent>(i), eventTimes_(static_cast<MemEvent>(i)) - dec);
      }

      eventTimes_.set<LY_COUNT>(ppu_.lyCounter().time());
   }
}

void LCD::speedChange(const unsigned long cycleCounter)
{
   update(cycleCounter);
   ppu_.speedChange(cycleCounter);

   if (ppu_.lcdc() & 0x80)
   {
      nextM0Time_.predictNextM0Time(ppu_);
      lycIrq_.reschedule(ppu_.lyCounter(), cycleCounter);

      eventTimes_.set<LY_COUNT>(ppu_.lyCounter().time());
      eventTimes_.setm<SPRITE_MAP>(SpriteMapper::schedule(ppu_.lyCounter(), cycleCounter));
      eventTimes_.setm<LYC_IRQ>(lycIrq_.time());
      eventTimes_.setm<MODE1_IRQ>(ppu_.lyCounter().nextFrameCycle(144 * 456, cycleCounter));
      eventTimes_.setm<MODE2_IRQ>(mode2IrqSchedule(statReg_, ppu_.lyCounter(), cycleCounter));

      if (eventTimes_(MODE0_IRQ) != disabled_time && eventTimes_(MODE0_IRQ) - cycleCounter > 1)
         eventTimes_.setm<MODE0_IRQ>(m0IrqTimeFromXpos166Time(ppu_.predictedNextXposTime(166), ppu_.cgb(), isDoubleSpeed()));

      if (hdmaIsEnabled() && eventTimes_(HDMA_REQ) - cycleCounter > 1)
      {
         eventTimes_.setm<HDMA_REQ>(nextHdmaTime(ppu_.lastM0Time(),
                  nextM0Time_.predictedNextM0Time(), cycleCounter, isDoubleSpeed()));
      }
   }
}

static inline unsigned long m0TimeOfCurrentLine(const unsigned long nextLyTime,
      const unsigned long lastM0Time, const unsigned long nextM0Time)
{
   return nextM0Time < nextLyTime ? nextM0Time : lastM0Time;
}

unsigned long LCD::m0TimeOfCurrentLine(const unsigned long cc)
{
   if (cc >= nextM0Time_.predictedNextM0Time())
   {
      update(cc);
      nextM0Time_.predictNextM0Time(ppu_);
   }

   return gambatte::m0TimeOfCurrentLine(ppu_.lyCounter().time(), ppu_.lastM0Time(), nextM0Time_.predictedNextM0Time());
}

static bool isHdmaPeriod(const LyCounter &lyCounter,
      const unsigned long m0TimeOfCurrentLy, const unsigned long cycleCounter)
{
   const unsigned timeToNextLy = lyCounter.time() - cycleCounter;

   return /*(ppu_.lcdc & 0x80) && */lyCounter.ly() < 144 && timeToNextLy > 4
      && cycleCounter >= hdmaTimeFromM0Time(m0TimeOfCurrentLy, lyCounter.isDoubleSpeed());
}

void LCD::enableHdma(const unsigned long cycleCounter)
{
   if (cycleCounter >= nextM0Time_.predictedNextM0Time())
   {
      update(cycleCounter);
      nextM0Time_.predictNextM0Time(ppu_);
   }
   else if (cycleCounter >= eventTimes_.nextEventTime())
      update(cycleCounter);

   if (isHdmaPeriod(ppu_.lyCounter(),
            gambatte::m0TimeOfCurrentLine(ppu_.lyCounter().time(),
               ppu_.lastM0Time(), nextM0Time_.predictedNextM0Time()), cycleCounter))
      eventTimes_.flagHdmaReq();

   eventTimes_.setm<HDMA_REQ>(nextHdmaTime(ppu_.lastM0Time(), nextM0Time_.predictedNextM0Time(), cycleCounter, isDoubleSpeed()));
}

void LCD::disableHdma(const unsigned long cycleCounter)
{
   if (cycleCounter >= eventTimes_.nextEventTime())
      update(cycleCounter);

   eventTimes_.setm<HDMA_REQ>(disabled_time);
}

bool LCD::vramAccessible(const unsigned long cc)
{
   if (cc >= eventTimes_.nextEventTime())
      update(cc);

   return !(ppu_.lcdc() & 0x80) || ppu_.lyCounter().ly() >= 144
      || ppu_.lyCounter().lineCycles(cc) < 80U
      || cc + isDoubleSpeed() - ppu_.cgb() + 2 >= m0TimeOfCurrentLine(cc);
}

bool LCD::cgbpAccessible(const unsigned long cc)
{
   if (cc >= eventTimes_.nextEventTime())
      update(cc);

   return !(ppu_.lcdc() & 0x80) || ppu_.lyCounter().ly() >= 144
      || ppu_.lyCounter().lineCycles(cc) < 80U + isDoubleSpeed()
      || cc >= m0TimeOfCurrentLine(cc) + 3 - isDoubleSpeed();
}

void LCD::doCgbBgColorChange(unsigned index, const unsigned data, const unsigned long cc)
{
   if (cgbpAccessible(cc))
   {
      update(cc);
      doCgbColorChange(bgpData_, ppu_.bgPalette(), index, data);
   }
}

void LCD::doCgbSpColorChange(unsigned index, const unsigned data, const unsigned long cc)
{
   if (cgbpAccessible(cc))
   {
      update(cc);
      doCgbColorChange(objpData_, ppu_.spPalette(), index, data);
   }
}

bool LCD::oamReadable(const unsigned long cc)
{
   if (!(ppu_.lcdc() & 0x80) || ppu_.inactivePeriodAfterDisplayEnable(cc))
      return true;

   if (cc >= eventTimes_.nextEventTime())
      update(cc);

   if (ppu_.lyCounter().lineCycles(cc) + 4 - ppu_.lyCounter().isDoubleSpeed() * 3u >= 456)
      return ppu_.lyCounter().ly() >= 144-1 && ppu_.lyCounter().ly() != 153;

   return ppu_.lyCounter().ly() >= 144 || cc + isDoubleSpeed() - ppu_.cgb() + 2 >= m0TimeOfCurrentLine(cc);
}

bool LCD::oamWritable(const unsigned long cc)
{
   if (!(ppu_.lcdc() & 0x80) || ppu_.inactivePeriodAfterDisplayEnable(cc))
      return true;

   if (cc >= eventTimes_.nextEventTime())
      update(cc);

   if (ppu_.lyCounter().lineCycles(cc) + 3 + ppu_.cgb() - ppu_.lyCounter().isDoubleSpeed() * 2u >= 456)
      return ppu_.lyCounter().ly() >= 144-1 && ppu_.lyCounter().ly() != 153;

   return ppu_.lyCounter().ly() >= 144 || cc + isDoubleSpeed() - ppu_.cgb() + 2 >= m0TimeOfCurrentLine(cc);
}

void LCD::mode3CyclesChange()
{
   nextM0Time_.invalidatePredictedNextM0Time();

   if (eventTimes_(MODE0_IRQ) != disabled_time
         && eventTimes_(MODE0_IRQ) > m0IrqTimeFromXpos166Time(ppu_.now(), ppu_.cgb(), isDoubleSpeed())) {
      eventTimes_.setm<MODE0_IRQ>(m0IrqTimeFromXpos166Time(ppu_.predictedNextXposTime(166), ppu_.cgb(), isDoubleSpeed()));
   }

   if (eventTimes_(HDMA_REQ) != disabled_time
         && eventTimes_(HDMA_REQ) > hdmaTimeFromM0Time(ppu_.lastM0Time(), isDoubleSpeed())) {
      nextM0Time_.predictNextM0Time(ppu_);
      eventTimes_.setm<HDMA_REQ>(hdmaTimeFromM0Time(nextM0Time_.predictedNextM0Time(), isDoubleSpeed()));
   }
}

void LCD::wxChange(const unsigned newValue, const unsigned long cycleCounter)
{
   update(cycleCounter + isDoubleSpeed() + 1);
   ppu_.setWx(newValue);
   mode3CyclesChange();
}

void LCD::wyChange(const unsigned newValue, const unsigned long cc)
{
   update(cc + 1);
   ppu_.setWy(newValue);
   // 	mode3CyclesChange(); // should be safe to wait until after wy2 delay, because no mode3 events are close to when wy1 is read.

   // wy2 is a delayed version of wy. really just slowness of ly == wy comparison.
   if (ppu_.cgb() && (ppu_.lcdc() & 0x80)) {
      eventTimes_.setm<ONESHOT_UPDATEWY2>(cc + 5);
   } else {
      update(cc + 2);
      ppu_.updateWy2();
      mode3CyclesChange();
   }
}

void LCD::scxChange(const unsigned newScx, const unsigned long cycleCounter) {
   update(cycleCounter + ppu_.cgb() + isDoubleSpeed());
   ppu_.setScx(newScx);
   mode3CyclesChange();
}

void LCD::scyChange(const unsigned newValue, const unsigned long cycleCounter) {
   update(cycleCounter + ppu_.cgb() + isDoubleSpeed());
   ppu_.setScy(newValue);
}

void LCD::oamChange(const unsigned long cc) {
   if (ppu_.lcdc() & 0x80) {
      update(cc);
      ppu_.oamChange(cc);
      eventTimes_.setm<SPRITE_MAP>(SpriteMapper::schedule(ppu_.lyCounter(), cc));
   }
}

void LCD::oamChange(const unsigned char *const oamram, const unsigned long cc) {
   update(cc);
   ppu_.oamChange(oamram, cc);

   if (ppu_.lcdc() & 0x80)
      eventTimes_.setm<SPRITE_MAP>(SpriteMapper::schedule(ppu_.lyCounter(), cc));
}

void LCD::lcdcChange(const unsigned data, const unsigned long cc) {
   const unsigned oldLcdc = ppu_.lcdc();
   update(cc);

   if ((oldLcdc ^ data) & 0x80)
   {
      ppu_.setLcdc(data, cc);

      if (data & 0x80) {
         lycIrq_.lcdReset();
         m0Irq_.lcdReset(statReg_, lycIrq_.lycReg());

         if (lycIrq_.lycReg() == 0 && (statReg_ & 0x40))
            eventTimes_.flagIrq(2);

         nextM0Time_.predictNextM0Time(ppu_);
         lycIrq_.reschedule(ppu_.lyCounter(), cc);

         eventTimes_.set<LY_COUNT>(ppu_.lyCounter().time());
         eventTimes_.setm<SPRITE_MAP>(SpriteMapper::schedule(ppu_.lyCounter(), cc));
         eventTimes_.setm<LYC_IRQ>(lycIrq_.time());
         eventTimes_.setm<MODE1_IRQ>(ppu_.lyCounter().nextFrameCycle(144 * 456, cc));
         eventTimes_.setm<MODE2_IRQ>(mode2IrqSchedule(statReg_, ppu_.lyCounter(), cc));

         if (statReg_ & 0x08)
            eventTimes_.setm<MODE0_IRQ>(m0IrqTimeFromXpos166Time(ppu_.predictedNextXposTime(166), ppu_.cgb(), isDoubleSpeed()));

         if (hdmaIsEnabled())
            eventTimes_.setm<HDMA_REQ>(nextHdmaTime(ppu_.lastM0Time(),
                     nextM0Time_.predictedNextM0Time(), cc, isDoubleSpeed()));
      }
      else
      {
         for (int i = 0; i < NUM_MEM_EVENTS; ++i)
            eventTimes_.set(static_cast<MemEvent>(i), disabled_time);
      }
   }
   else if (data & 0x80)
   {
      if (ppu_.cgb())
      {
         ppu_.setLcdc((oldLcdc & ~0x14) | (data & 0x14), cc);

         if ((oldLcdc ^ data) & 0x04)
            eventTimes_.setm<SPRITE_MAP>(SpriteMapper::schedule(ppu_.lyCounter(), cc));

         update(cc + isDoubleSpeed() + 1);
         ppu_.setLcdc(data, cc + isDoubleSpeed() + 1);

         if ((oldLcdc ^ data) & 0x20)
            mode3CyclesChange();
      }
      else
      {
         ppu_.setLcdc(data, cc);

         if ((oldLcdc ^ data) & 0x04)
            eventTimes_.setm<SPRITE_MAP>(SpriteMapper::schedule(ppu_.lyCounter(), cc));

         if ((oldLcdc ^ data) & 0x22)
            mode3CyclesChange();
      }
   }
   else
      ppu_.setLcdc(data, cc);
}

namespace {
   struct LyCnt {
      unsigned ly; int timeToNextLy;
      LyCnt(unsigned ly, int timeToNextLy) : ly(ly), timeToNextLy(timeToNextLy) {}
   };

   static LyCnt const getLycCmpLy(LyCounter const &lyCounter, unsigned long cc) {
      unsigned ly = lyCounter.ly();
      int timeToNextLy = lyCounter.time() - cc;

      if (ly == 153) {
         if (timeToNextLy -  (448 << lyCounter.isDoubleSpeed()) > 0) {
            timeToNextLy -= (448 << lyCounter.isDoubleSpeed());
         } else {
            ly = 0;
            timeToNextLy += lyCounter.lineTime();
         }
      }
      return LyCnt(ly, timeToNextLy);
   }
}

void LCD::lcdstatChange(const unsigned data, const unsigned long cc)
{
   if (cc >= eventTimes_.nextEventTime())
      update(cc);

   const unsigned old = statReg_;
   statReg_ = data;
   lycIrq_.statRegChange(data, ppu_.lyCounter(), cc);

   if (ppu_.lcdc() & 0x80)
   {
      int const timeToNextLy = ppu_.lyCounter().time() - cc;
      LyCnt const lycCmp = getLycCmpLy(ppu_.lyCounter(), cc);

      if (!ppu_.cgb())
      {
         if (ppu_.lyCounter().ly() < 144)
         {
            if (cc + 1 < m0TimeOfCurrentLine(cc))
            {
               if (lycCmp.ly == lycIrq_.lycReg() && !(old & 0x40))
                  eventTimes_.flagIrq(2);
            }
            else
            {
               if (!(old & 0x08) && !(lycCmp.ly == lycIrq_.lycReg() && (old & 0x40)))
                  eventTimes_.flagIrq(2);
            }
         }
         else
         {
            if (!(old & 0x10) && !(lycCmp.ly == lycIrq_.lycReg() && (old & 0x40)))
               eventTimes_.flagIrq(2);
         }
      }
      else if (data & ~old & 0x78)
      {
         bool const lycperiod = lycCmp.ly == lycIrq_.lycReg() && lycCmp.timeToNextLy > 4 - isDoubleSpeed() * 4;

         if (!(lycperiod && (old & 0x40)))
         {
            if (ppu_.lyCounter().ly() < 144)
            {
               if (cc + isDoubleSpeed() * 2 < m0TimeOfCurrentLine(cc) || timeToNextLy <= 4)
               {
                  if (lycperiod && (data & 0x40))
                     eventTimes_.flagIrq(2);
               }
               else if (!(old & 0x08))
               {
                  if ((data & 0x08) || (lycperiod && (data & 0x40)))
                     eventTimes_.flagIrq(2);
               }
            }
            else if (!(old & 0x10))
            {
               if ((data & 0x10) && (ppu_.lyCounter().ly() < 153 || timeToNextLy > 4 - isDoubleSpeed() * 4))
               {
                  eventTimes_.flagIrq(2);
               }
               else if (lycperiod && (data & 0x40))
                  eventTimes_.flagIrq(2);
            }
         }

         if ((data & 0x28) == 0x20 && !(old & 0x20)
               && ((timeToNextLy <= 4 && ppu_.lyCounter().ly() < 143)
                  || (timeToNextLy == 456*2 && ppu_.lyCounter().ly() < 144))) 
         {
            eventTimes_.flagIrq(2);
         }
      }

      if ((data & 0x08) && eventTimes_(MODE0_IRQ) == disabled_time)
      {
         update(cc);
         eventTimes_.setm<MODE0_IRQ>(m0IrqTimeFromXpos166Time(ppu_.predictedNextXposTime(166), ppu_.cgb(), isDoubleSpeed()));
      }

      eventTimes_.setm<MODE2_IRQ>(mode2IrqSchedule(data, ppu_.lyCounter(), cc));
      eventTimes_.setm<LYC_IRQ>(lycIrq_.time());
   }

   m2IrqStatReg_ = eventTimes_(MODE2_IRQ) - cc > (ppu_.cgb() - isDoubleSpeed()) * 4U
      ? data : (m2IrqStatReg_ & 0x10) | (statReg_ & ~0x10);
   m1IrqStatReg_ = eventTimes_(MODE1_IRQ) - cc > (ppu_.cgb() - isDoubleSpeed()) * 4U
      ? data : (m1IrqStatReg_ & 0x08) | (statReg_ & ~0x08);

   m0Irq_.statRegChange(data, eventTimes_(MODE0_IRQ), cc, ppu_.cgb());
}

void LCD::lycRegChange(const unsigned data, const unsigned long cc)
{
   unsigned const old = lycIrq_.lycReg();

   if (data == old)
      return;

   if (cc >= eventTimes_.nextEventTime())
      update(cc);

   m0Irq_.lycRegChange(data, eventTimes_(MODE0_IRQ), cc, isDoubleSpeed(), ppu_.cgb());
   lycIrq_.lycRegChange(data, ppu_.lyCounter(), cc);

   if (!(ppu_.lcdc() & 0x80))
      return;

   eventTimes_.setm<LYC_IRQ>(lycIrq_.time());

   int const timeToNextLy = ppu_.lyCounter().time() - cc;

   if ((statReg_ & 0x40) && data < 154
         && (ppu_.lyCounter().ly() < 144
             ? !(statReg_ & 0x08) || cc < m0TimeOfCurrentLine(cc) || timeToNextLy <= 4 << ppu_.cgb()
             : !(statReg_ & 0x10) || (ppu_.lyCounter().ly() == 153 && timeToNextLy <= 4 && ppu_.cgb() && !isDoubleSpeed())))
   {
      LyCnt lycCmp = getLycCmpLy(ppu_.lyCounter(), cc);

      if (lycCmp.timeToNextLy <= 4 << ppu_.cgb())
      {
         lycCmp.ly = old != lycCmp.ly || (lycCmp.timeToNextLy <= 4 && ppu_.cgb() && !isDoubleSpeed())
               ? (lycCmp.ly == 153 ? 0 : lycCmp.ly + 1)
               : 0xFF; // simultaneous ly/lyc inc. lyc flag never goes low -> no trigger.
      }

      if (data == lycCmp.ly)
      {
         if (ppu_.cgb() && !isDoubleSpeed())
         {
            eventTimes_.setm<ONESHOT_LCDSTATIRQ>(cc + 5);
         }
         else
            eventTimes_.flagIrq(2);
      }
   }
}

unsigned LCD::getStat(const unsigned lycReg, const unsigned long cc)
{
   unsigned stat = 0;

   if (ppu_.lcdc() & 0x80)
   {
      if (cc >= eventTimes_.nextEventTime())
         update(cc);

      int const timeToNextLy = ppu_.lyCounter().time() - cc;

      if (ppu_.lyCounter().ly() > 143)
      {
         if (ppu_.lyCounter().ly() < 153 || timeToNextLy > 4 - isDoubleSpeed() * 4)
            stat = 1;
      }
      else
      {
         unsigned const lineCycles = 456 - (timeToNextLy >> isDoubleSpeed());

         if (lineCycles < 80)
         {
            if (!ppu_.inactivePeriodAfterDisplayEnable(cc))
               stat = 2;
         }
         else if (cc + isDoubleSpeed() - ppu_.cgb() + 2 < m0TimeOfCurrentLine(cc))
            stat = 3;
      }

      LyCnt const lycCmp = getLycCmpLy(ppu_.lyCounter(), cc);

      if (lycReg == lycCmp.ly && lycCmp.timeToNextLy > 4 - isDoubleSpeed() * 4)
         stat |= 4;
   }

   return stat;
}

inline void LCD::doMode2IrqEvent()
{
   const unsigned ly = eventTimes_(LY_COUNT) - eventTimes_(MODE2_IRQ) < 8
      ? (ppu_.lyCounter().ly() == 153 ? 0 : ppu_.lyCounter().ly() + 1)
      : ppu_.lyCounter().ly();

   if ((ly != 0 || !(m2IrqStatReg_ & 0x10)) &&
         (!(m2IrqStatReg_ & 0x40) || (lycIrq_.lycReg() != 0 ? ly != (lycIrq_.lycReg() + 1U) : ly > 1)))
      eventTimes_.flagIrq(2);

   m2IrqStatReg_ = statReg_;

   if (!(statReg_ & 0x08))
   {
      unsigned long nextTime = eventTimes_(MODE2_IRQ) + ppu_.lyCounter().lineTime();

      if (ly == 0)
         nextTime -= 4;
      else if (ly == 143)
         nextTime += ppu_.lyCounter().lineTime() * 10 + 4;

      eventTimes_.setm<MODE2_IRQ>(nextTime);
   }
   else
      eventTimes_.setm<MODE2_IRQ>(eventTimes_(MODE2_IRQ) + (70224 << isDoubleSpeed()));
}

inline void LCD::event()
{
   switch (eventTimes_.nextEvent())
   {
      case MEM_EVENT:
         switch (eventTimes_.nextMemEvent())
         {
            case MODE1_IRQ:
               eventTimes_.flagIrq((m1IrqStatReg_ & 0x18) == 0x10 ? 3 : 1);
               m1IrqStatReg_ = statReg_;
               eventTimes_.setm<MODE1_IRQ>(eventTimes_(MODE1_IRQ) + (70224 << isDoubleSpeed()));
               break;

            case LYC_IRQ:
               {
                  unsigned char ifreg = 0;
                  lycIrq_.doEvent(&ifreg, ppu_.lyCounter());
                  eventTimes_.flagIrq(ifreg);
                  eventTimes_.setm<LYC_IRQ>(lycIrq_.time());
               }
               break;
            case SPRITE_MAP:
               eventTimes_.setm<SPRITE_MAP>(ppu_.doSpriteMapEvent(eventTimes_(SPRITE_MAP)));
               mode3CyclesChange();
               break;
            case HDMA_REQ:
               eventTimes_.flagHdmaReq();
               nextM0Time_.predictNextM0Time(ppu_);
               eventTimes_.setm<HDMA_REQ>(hdmaTimeFromM0Time(nextM0Time_.predictedNextM0Time(), isDoubleSpeed()));
               break;
            case MODE2_IRQ:
               doMode2IrqEvent();
               break;
            case MODE0_IRQ:
               {
                  unsigned char ifreg = 0;
                  m0Irq_.doEvent(&ifreg, ppu_.lyCounter().ly(), statReg_, lycIrq_.lycReg());
                  eventTimes_.flagIrq(ifreg);
               }

               eventTimes_.setm<MODE0_IRQ>((statReg_ & 0x08)
                     ? m0IrqTimeFromXpos166Time(ppu_.predictedNextXposTime(166), ppu_.cgb(), isDoubleSpeed())
                     : static_cast<unsigned long>(disabled_time));
               break;
            case ONESHOT_LCDSTATIRQ:
               eventTimes_.flagIrq(2);
               eventTimes_.setm<ONESHOT_LCDSTATIRQ>(disabled_time);
               break;
            case ONESHOT_UPDATEWY2:
               ppu_.updateWy2();
               mode3CyclesChange();
               eventTimes_.setm<ONESHOT_UPDATEWY2>(disabled_time);
               break;
         }
         break;
      case LY_COUNT:
         ppu_.doLyCountEvent();
         eventTimes_.set<LY_COUNT>(ppu_.lyCounter().time());
         break;
   }
}

void LCD::update(const unsigned long cycleCounter)
{
   if (!(ppu_.lcdc() & 0x80))
      return;

   while (cycleCounter >= eventTimes_.nextEventTime())
   {
      ppu_.update(eventTimes_.nextEventTime());
      event();
   }

   ppu_.update(cycleCounter);
}

}
