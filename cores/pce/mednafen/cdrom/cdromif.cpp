/* Mednafen - Multi-system Emulator
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "../mednafen.h"
#include <string.h>
#include <sys/types.h>
#include "cdromif.h"
#include "CDAccess.h"
#include "../general.h"

#include <algorithm>

#include <boolean.h>
#include <retro_miscellaneous.h>

enum
{
   /* Status/Error messages */
   CDIF_MSG_DONE = 0,		   /* Read -> emu. args: No args. */
   CDIF_MSG_INFO,			      /* Read -> emu. args: str_message */
   CDIF_MSG_FATAL_ERROR,		/* Read -> emu. args: *TODO ARGS* */

   /* Command messages. */
   CDIF_MSG_DIEDIEDIE,		   /* Emu -> read */

   CDIF_MSG_READ_SECTOR		   /* Emu -> read
                              args[0] = lba
                              */
};

class CDIF_Message
{
   public:

      CDIF_Message();
      CDIF_Message(unsigned int message_, uint32_t arg0 = 0, uint32_t arg1 = 0, uint32_t arg2 = 0, uint32_t arg3 = 0);
      CDIF_Message(unsigned int message_, const std::string &str);
      ~CDIF_Message();

      unsigned int message;
      uint32_t args[4];
      void *parg;
      std::string str_message;
};

typedef struct
{
   bool valid;
   bool error;
   int32_t lba;
   uint8_t data[2352 + 96];
} CDIF_Sector_Buffer;

// TODO: prohibit copy constructor
class CDIF_ST : public CDIF
{
   public:

      CDIF_ST(CDAccess *cda);
      virtual ~CDIF_ST();

      virtual void HintReadSector(int32_t lba);
      virtual bool ReadRawSector(uint8_t *buf, int32_t lba);
      virtual bool ReadRawSectorPWOnly(uint8_t* pwbuf, int32_t lba, bool hint_fullread);

   private:
      CDAccess *disc_cdaccess;
};

CDIF::CDIF() : UnrecoverableError(false)
{

}

CDIF::~CDIF()
{

}


CDIF_Message::CDIF_Message()
{
   message = 0;

   memset(args, 0, sizeof(args));
}

CDIF_Message::CDIF_Message(unsigned int message_, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
   message = message_;
   args[0] = arg0;
   args[1] = arg1;
   args[2] = arg2;
   args[3] = arg3;
}

CDIF_Message::CDIF_Message(unsigned int message_, const std::string &str)
{
   message = message_;
   str_message = str;
}

CDIF_Message::~CDIF_Message()
{

}

bool CDIF::ValidateRawSector(uint8_t *buf)
{
   int mode = buf[12 + 3];

   if(mode != 0x1 && mode != 0x2)
      return(false);

   if(!edc_lec_check_and_correct(buf, mode == 2))
      return(false);

   return(true);
}

int CDIF::ReadSector(uint8_t* buf, int32_t lba, uint32_t sector_count, bool suppress_uncorrectable_message)
{
   int ret = 0;

   if(UnrecoverableError)
      return(false);

   while(sector_count--)
   {
      uint8_t tmpbuf[2352 + 96];

      if(!ReadRawSector(tmpbuf, lba))
      {
         puts("CDIF Raw Read error");
         return(FALSE);
      }

      if(!ValidateRawSector(tmpbuf))
      {
         if(!suppress_uncorrectable_message)
         {
            MDFN_DispMessage("Uncorrectable data at sector %d", lba);
            MDFN_PrintError("Uncorrectable data at sector %d", lba);
         }

         return(false);
      }

      const int mode = tmpbuf[12 + 3];

      if(!ret)
         ret = mode;

      if(mode == 1)
      {
         memcpy(buf, &tmpbuf[12 + 4], 2048);
      }
      else if(mode == 2)
      {
         memcpy(buf, &tmpbuf[12 + 4 + 8], 2048);
      }
      else
      {
         printf("CDIF_ReadSector() invalid sector type at LBA=%u\n", (unsigned int)lba);
         return(false);
      }

      buf += 2048;
      lba++;
   }

   return(ret);
}

//
//
// Single-threaded implementation follows.
//
//

CDIF_ST::CDIF_ST(CDAccess *cda) : disc_cdaccess(cda)
{
   //puts("***WARNING USING SINGLE-THREADED CD READER***");

   UnrecoverableError = false;

   disc_cdaccess->Read_TOC(&disc_toc);

   if(disc_toc.first_track < 1 || disc_toc.last_track > 99 || disc_toc.first_track > disc_toc.last_track)
   {
      printf("TOC first(%d)/last(%d) track numbers bad.", disc_toc.first_track, disc_toc.last_track);
   }
}

CDIF_ST::~CDIF_ST()
{

}

void CDIF_ST::HintReadSector(int32_t lba)
{
   // TODO: disc_cdaccess seek hint? (probably not, would require asynchronousitycamel)
}

bool CDIF_ST::ReadRawSector(uint8_t *buf, int32_t lba)
{
   if(UnrecoverableError)
   {
      memset(buf, 0, 2352 + 96);
      return(false);
   }

   if(lba < LBA_Read_Minimum || lba > LBA_Read_Maximum)
   {
      printf("Attempt to read sector out of bounds; LBA=%d\n", lba);
      memset(buf, 0, 2352 + 96);
      return(false);
   }

   disc_cdaccess->Read_Raw_Sector(buf, lba);

   return(true);
}

bool CDIF_ST::ReadRawSectorPWOnly(uint8_t* pwbuf, int32_t lba, bool hint_fullread)
{
   if(UnrecoverableError)
   {
      memset(pwbuf, 0, 96);
      return(false);
   }

   if(lba < LBA_Read_Minimum || lba > LBA_Read_Maximum)
   {
      printf("Attempt to read sector out of bounds; LBA=%d\n", lba);
      memset(pwbuf, 0, 96);
      return(false);
   }

   if(disc_cdaccess->Fast_Read_Raw_PW_TSRE(pwbuf, lba))
      return(true);
   else
   {
      uint8_t tmpbuf[2352 + 96];
      bool ret;

      ret = ReadRawSector(tmpbuf, lba);
      memcpy(pwbuf, tmpbuf + 2352, 96);

      return ret;
   }
}

CDIF *CDIF_Open(const std::string& path, bool image_memcache)
{
   CDAccess *cda = CDAccess_Open(path, image_memcache);

   return new CDIF_ST(cda);
}
