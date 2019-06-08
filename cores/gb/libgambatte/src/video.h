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
#ifndef VIDEO_H
#define VIDEO_H

#include "interruptrequester.h"
#include "video/lyc_irq.h"
#include "video/m0_irq.h"
#include "video/next_m0_time.h"
#include "video/ppu.h"
#include <memory>

namespace gambatte {

class VideoInterruptRequester
{
   public:
      explicit VideoInterruptRequester(InterruptRequester &intreq) : intreq_(intreq) {}
      void flagHdmaReq() const { gambatte::flagHdmaReq(intreq_); }
      void flagIrq(const unsigned bit) const { intreq_.flagIrq(bit); }
      void setNextEventTime(const unsigned long time) const { intreq_.setEventTime<intevent_video>(time); }

   private:
      InterruptRequester &intreq_;
};

class LCD
{
   public:
      LCD(const unsigned char *oamram, const unsigned char *vram_in, VideoInterruptRequester memEventRequester);
      void reset(const unsigned char *oamram, unsigned char const *vram, bool cgb);
      void setStatePtrs(SaveState &state);
      void saveState(SaveState &state) const;
      void loadState(const SaveState &state, const unsigned char *oamram);
      void setDmgPaletteColor(unsigned palNum, unsigned colorNum, video_pixel_t rgb32);
      void setVideoBuffer(video_pixel_t *videoBuf, int pitch);
      void setDmgMode(bool mode) { ppu_.setDmgMode(mode); }
   
      void swapToDMG() {
         ppu_.setDmgMode(true);
         refreshPalettes();
      }

      void dmgBgPaletteChange(const unsigned data, const unsigned long cycleCounter) {
         update(cycleCounter);
         bgpData_[0] = data;
         setDmgPalette(ppu_.bgPalette(), dmgColorsRgb32_, data);
      }

      void dmgSpPalette1Change(const unsigned data, const unsigned long cycleCounter) {
         update(cycleCounter);
         objpData_[0] = data;
         setDmgPalette(ppu_.spPalette(), dmgColorsRgb32_ + 4, data);
      }

      void dmgSpPalette2Change(const unsigned data, const unsigned long cycleCounter) {
         update(cycleCounter);
         objpData_[1] = data;
         setDmgPalette(ppu_.spPalette() + 4, dmgColorsRgb32_ + 8, data);
      }

      void cgbBgColorChange(unsigned index, const unsigned data, const unsigned long cycleCounter) {
         if (bgpData_[index] != data) {
            doCgbBgColorChange(index, data, cycleCounter);
            if(index < 8)
               doCgbColorChange(dmgColorsGBC_, dmgColorsRgb32_, index, data);
         }
      }

      void cgbSpColorChange(unsigned index, const unsigned data, const unsigned long cycleCounter) {
         if (objpData_[index] != data) {
            doCgbSpColorChange(index, data, cycleCounter);
            if(index < 8 * 2/*dmg has 2 sprite banks*/)
               doCgbColorChange(dmgColorsGBC_ + 8, dmgColorsRgb32_ + 4, index, data);
         }
      }

      unsigned cgbBgColorRead(const unsigned index, const unsigned long cycleCounter) {
         return (ppu_.cgb() & cgbpAccessible(cycleCounter)) ? bgpData_[index] : 0xFF;
      }

      unsigned cgbSpColorRead(const unsigned index, const unsigned long cycleCounter) {
         return (ppu_.cgb() & cgbpAccessible(cycleCounter)) ? objpData_[index] : 0xFF;
      }

      void updateScreen(bool blanklcd, unsigned long cc);
      void resetCc(unsigned long oldCC, unsigned long newCc);
      void speedChange(unsigned long cycleCounter);
      bool vramAccessible(unsigned long cycleCounter);
      bool oamReadable(unsigned long cycleCounter);
      bool oamWritable(unsigned long cycleCounter);
      void wxChange(unsigned newValue, unsigned long cycleCounter);
      void wyChange(unsigned newValue, unsigned long cycleCounter);
      void oamChange(unsigned long cycleCounter);
      void oamChange(const unsigned char *oamram, unsigned long cycleCounter);
      void scxChange(unsigned newScx, unsigned long cycleCounter);
      void scyChange(unsigned newValue, unsigned long cycleCounter);

      void vramChange(const unsigned long cycleCounter) { update(cycleCounter); }

      unsigned getStat(unsigned lycReg, unsigned long cycleCounter);

      unsigned getLyReg(const unsigned long cycleCounter) {
         unsigned lyReg = 0;

         if (ppu_.lcdc() & 0x80) {
            if (cycleCounter >= ppu_.lyCounter().time())
               update(cycleCounter);

            lyReg = ppu_.lyCounter().ly();

            if (lyReg == 153) {
               if (isDoubleSpeed()) {
                  if (ppu_.lyCounter().time() - cycleCounter <= 456 * 2 - 8)
                     lyReg = 0;
               } else
                  lyReg = 0;
            } else if (ppu_.lyCounter().time() - cycleCounter <= 4)
               ++lyReg;
         }

         return lyReg;
      }

      unsigned long nextMode1IrqTime() const { return eventTimes_(MODE1_IRQ); }

      void lcdcChange(unsigned data, unsigned long cycleCounter);
      void lcdstatChange(unsigned data, unsigned long cycleCounter);
      void lycRegChange(unsigned data, unsigned long cycleCounter);

      void enableHdma(unsigned long cycleCounter);
      void disableHdma(unsigned long cycleCounter);
      bool hdmaIsEnabled() const { return eventTimes_(HDMA_REQ) != disabled_time; }

      void update(unsigned long cycleCounter);

      bool isCgb() const { return ppu_.cgb(); }
      bool isDoubleSpeed() const { return ppu_.lyCounter().isDoubleSpeed(); }

      void setColorCorrection(bool colorCorrection);
      video_pixel_t gbcToRgb32(const unsigned bgr15);
   private:
      enum Event { MEM_EVENT, LY_COUNT }; enum { NUM_EVENTS = LY_COUNT + 1 };
      enum MemEvent { ONESHOT_LCDSTATIRQ, ONESHOT_UPDATEWY2, MODE1_IRQ, LYC_IRQ, SPRITE_MAP,
         HDMA_REQ, MODE2_IRQ, MODE0_IRQ }; enum { NUM_MEM_EVENTS = MODE0_IRQ + 1 };

      class EventTimes
      {
         public:
            explicit EventTimes(const VideoInterruptRequester memEventRequester) : memEventRequester_(memEventRequester) {}

            Event nextEvent() const { return static_cast<Event>(eventMin_.min()); }
            unsigned long nextEventTime() const { return eventMin_.minValue(); }
            unsigned long operator()(const Event e) const { return eventMin_.value(e); }
            template<Event e> void set(const unsigned long time) { eventMin_.setValue<e>(time); }
            void set(const Event e, const unsigned long time) { eventMin_.setValue(e, time); }

            MemEvent nextMemEvent() const { return static_cast<MemEvent>(memEventMin_.min()); }
            unsigned long nextMemEventTime() const { return memEventMin_.minValue(); }
            unsigned long operator()(const MemEvent e) const { return memEventMin_.value(e); }
            template<MemEvent e> void setm(const unsigned long time) { memEventMin_.setValue<e>(time); setMemEvent(); }
            void set(const MemEvent e, const unsigned long time) { memEventMin_.setValue(e, time); setMemEvent(); }

            void flagIrq(const unsigned bit) { memEventRequester_.flagIrq(bit); }
            void flagHdmaReq() { memEventRequester_.flagHdmaReq(); }

         private:
            MinKeeper<NUM_EVENTS> eventMin_;
            MinKeeper<NUM_MEM_EVENTS> memEventMin_;
            VideoInterruptRequester memEventRequester_;

            void setMemEvent() {
               const unsigned long nmet = nextMemEventTime();
               eventMin_.setValue<MEM_EVENT>(nmet);
               memEventRequester_.setNextEventTime(nmet);
            }

      };

      PPU ppu_;
      video_pixel_t dmgColorsRgb32_[3 * 4];
      unsigned char dmgColorsGBC_[3 * 8];
      unsigned char  bgpData_[8 * 8];
      unsigned char objpData_[8 * 8];

      EventTimes eventTimes_;
      M0Irq m0Irq_;
      LycIrq lycIrq_;
      NextM0Time nextM0Time_;

      unsigned char statReg_;
      unsigned char m2IrqStatReg_;
      unsigned char m1IrqStatReg_;

      static void setDmgPalette(video_pixel_t *palette, const video_pixel_t *dmgColors, unsigned data);
      void setDmgPaletteColor(unsigned index, video_pixel_t rgb32);

      void setDBuffer();
      void refreshPalettes();

      void doMode2IrqEvent();
      void event();

      unsigned long m0TimeOfCurrentLine(unsigned long cc);
      bool cgbpAccessible(unsigned long cycleCounter);

      void mode3CyclesChange();
      void doCgbBgColorChange(unsigned index, unsigned data, unsigned long cycleCounter);
      void doCgbSpColorChange(unsigned index, unsigned data, unsigned long cycleCounter);

      bool colorCorrection;
      void doCgbColorChange(unsigned char *const pdata,
            video_pixel_t *const palette, unsigned index, const unsigned data);

};

}

#endif
