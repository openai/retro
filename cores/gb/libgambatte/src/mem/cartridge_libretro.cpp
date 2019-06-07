#include "cartridge.h"
#include "../savestate.h"
#include <cstring>
#include <fstream>

#include "../../libretro/libretro.h"
extern retro_log_printf_t log_cb;

namespace gambatte
{
   static bool hasBattery(unsigned char headerByte0x147)
   {
      switch (headerByte0x147)
      {
         case 0x03:
         case 0x06:
         case 0x09:
         case 0x0F:
         case 0x10:
         case 0x13:
         case 0x1B:
         case 0x1E:
         case 0xFF:
            return true;
         default:
            return false;
      }
   }

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

   void *Cartridge::savedata_ptr()
   {
      // Check ROM header for battery.
      if (hasBattery(memptrs_.romdata()[0x147]))
         return memptrs_.rambankdata();
      return 0;
   }

   unsigned Cartridge::savedata_size()
   {
      if (hasBattery(memptrs_.romdata()[0x147]))
         return memptrs_.rambankdataend() - memptrs_.rambankdata();
      return 0;
   }

   void *Cartridge::rtcdata_ptr()
   {
      if (hasRtc(memptrs_.romdata()[0x147]))
         return &rtc_.getBaseTime();
      return 0;
   }

   unsigned Cartridge::rtcdata_size()
   { 
      if (hasRtc(memptrs_.romdata()[0x147]))
         return sizeof(rtc_.getBaseTime());
      return 0;
   }

   void Cartridge::clearCheats()
   {
      ggUndoList_.clear();
   }

}
