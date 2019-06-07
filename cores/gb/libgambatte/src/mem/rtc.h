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
#ifndef RTC_H
#define RTC_H

#include <ctime>
#include <stdint.h>

namespace gambatte
{

   struct SaveState;

   class Rtc
   {
      public:
         Rtc();

         const unsigned char* getActive() const
         {
            return activeData_;
         }

         uint64_t& getBaseTime()
         {
            return baseTime_;
         }

         void setBaseTime(const uint64_t baseTime)
         {
            baseTime_ = baseTime;
         }

         void latch(const unsigned data)
         {
            if (!lastLatchData_ && data == 1)
               doLatch();

            lastLatchData_ = data;
         }

         void saveState(SaveState &state) const;
         void loadState(const SaveState &state);

         void set(const bool enabled, unsigned bank)
         {
            bank &= 0xF;
            bank -= 8;

            this->enabled_ = enabled;
            this->index_   = bank;

            doSwapActive();
         }

         void write(const unsigned data)
         {
            (this->*activeSet_)(data);
            *activeData_ = data;
         }

      private:
         unsigned char *activeData_;
         void (Rtc::*activeSet_)(unsigned);
         uint64_t baseTime_;
         uint64_t haltTime_;
         unsigned char index_;
         unsigned char dataDh_;
         unsigned char dataDl_;
         unsigned char dataH_;
         unsigned char dataM_;
         unsigned char dataS_;
         bool enabled_;
         bool lastLatchData_;

         void doLatch();
         void doSwapActive();
         void setDh(unsigned new_dh);
         void setDl(unsigned new_lowdays);
         void setH(unsigned new_hours);
         void setM(unsigned new_minutes);
         void setS(unsigned new_seconds);

   };

}

#endif
