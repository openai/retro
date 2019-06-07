/******************************************************************************/
/* Mednafen - Multi-system Emulator                                           */
/******************************************************************************/
/* CDAFReader_Vorbis.cpp:
**  Copyright (C) 2010-2016 Mednafen Team
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <mednafen/mednafen.h>
#include "CDAFReader.h"
#include "CDAFReader_Vorbis.h"

#include <mednafen/tremor/ivorbisfile.h>

class CDAFReader_Vorbis : public CDAFReader
{
   public:
      CDAFReader_Vorbis(Stream *fp);
      ~CDAFReader_Vorbis();

      uint64_t Read_(int16_t *buffer, uint64_t frames);
      bool Seek_(uint64_t frame_offset);
      uint64_t FrameCount(void);

   private:
      OggVorbis_File ovfile;
      Stream *fw;
};


static size_t iov_read_func(void *ptr, size_t size, size_t nmemb, void *user_data)
{
   Stream *fw = (Stream*)user_data;

   if(!size)
      return(0);

   return fw->read(ptr, size * nmemb, false) / size;
}

static int iov_seek_func(void *user_data, int64_t offset, int whence)
{
   Stream *fw = (Stream*)user_data;

   fw->seek(offset, whence);
   return(0);
}

static int iov_close_func(void *user_data)
{
   Stream *fw = (Stream*)user_data;

   fw->close();
   return(0);
}

static long iov_tell_func(void *user_data)
{
   Stream *fw = (Stream*)user_data;
   return fw->tell();
}

CDAFReader_Vorbis::CDAFReader_Vorbis(Stream *fp) : fw(fp)
{
   ov_callbacks cb;

   memset(&cb, 0, sizeof(cb));
   cb.read_func = iov_read_func;
   cb.seek_func = iov_seek_func;
   cb.close_func = iov_close_func;
   cb.tell_func = iov_tell_func;

   if(ov_open_callbacks(fp, &ovfile, NULL, 0, cb))
      throw(0);
}

CDAFReader_Vorbis::~CDAFReader_Vorbis()
{
   ov_clear(&ovfile);
}

uint64_t CDAFReader_Vorbis::Read_(int16_t *buffer, uint64_t frames)
{
   uint8 *tw_buf = (uint8 *)buffer;
   int cursection = 0;
   long toread = frames * sizeof(int16_t) * 2;

   while(toread > 0)
   {
      long didread = ov_read(&ovfile, (char*)tw_buf, toread, &cursection);

      if(didread == 0)
         break;

      tw_buf = (uint8 *)tw_buf + didread;
      toread -= didread;
   }

   return(frames - toread / sizeof(int16_t) / 2);
}

bool CDAFReader_Vorbis::Seek_(uint64_t frame_offset)
{
   ov_pcm_seek(&ovfile, frame_offset);
   return(true);
}

uint64_t CDAFReader_Vorbis::FrameCount(void)
{
   return(ov_pcm_total(&ovfile, -1));
}

CDAFReader* CDAFR_Vorbis_Open(Stream* fp)
{
   return new CDAFReader_Vorbis(fp);
}
