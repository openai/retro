/******************************************************************************/
/* Mednafen - Multi-system Emulator                                           */
/******************************************************************************/
/* CDAFReader_MPC.cpp:
**  Copyright (C) 2006-2016 Mednafen Team
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
#include "CDAFReader_MPC.h"

#include <mednafen/mpcdec/mpcdec.h>

class CDAFReader_MPC : public CDAFReader
{
   public:
      CDAFReader_MPC(Stream *fp);
      ~CDAFReader_MPC();

      uint64_t Read_(int16 *buffer, uint64_t frames);
      bool Seek_(uint64_t frame_offset);
      uint64_t FrameCount(void);

   private:
      mpc_reader reader;
      mpc_demux *demux;
      mpc_streaminfo si;

      MPC_SAMPLE_FORMAT MPCBuffer[MPC_DECODER_BUFFER_LENGTH];

      uint32_t MPCBufferIn;
      uint32_t MPCBufferOffs;
      Stream *fw;
};


/// Reads size bytes of data into buffer at ptr.
static mpc_int32_t_t impc_read(mpc_reader *p_reader, void *ptr, mpc_int32_t_t size)
{
   Stream *fw = (Stream*)(p_reader->data);

   return fw->read(ptr, size, false);
}

/// Seeks to byte position offset.
static mpc_bool_t impc_seek(mpc_reader *p_reader, mpc_int32_t_t offset)
{
   Stream *fw = (Stream*)(p_reader->data);

   fw->seek(offset, SEEK_SET);
   return(MPC_TRUE);
}

/// Returns the current byte offset in the stream.
static mpc_int32_t_t impc_tell(mpc_reader *p_reader)
{
   Stream *fw = (Stream*)(p_reader->data);
   return fw->tell();
}

/// Returns the total length of the source stream, in bytes.
static mpc_int32_t_t impc_get_size(mpc_reader *p_reader)
{
   Stream *fw = (Stream*)(p_reader->data);
   return fw->size();
}

/// True if the stream is a seekable stream.
static mpc_bool_t impc_canseek(mpc_reader *p_reader)
{
   return(MPC_TRUE);
}

CDAFReader_MPC::CDAFReader_MPC(Stream *fp) : fw(fp)
{
   demux = NULL;
   memset(&si, 0, sizeof(si));
   memset(MPCBuffer, 0, sizeof(MPCBuffer));
   MPCBufferOffs = 0;
   MPCBufferIn = 0;

   memset(&reader, 0, sizeof(reader));
   reader.read = impc_read;
   reader.seek = impc_seek;
   reader.tell = impc_tell;
   reader.get_size = impc_get_size;
   reader.canseek = impc_canseek;
   reader.data = (void*)fp;

   if(!(demux = mpc_demux_init(&reader)))
   {
      throw(0);
   }
   mpc_demux_get_info(demux, &si);

   if(si.channels != 2)
   {
      mpc_demux_exit(demux);
      demux = NULL;
      throw MDFN_Error(0, _("MusePack stream has wrong number of channels(%u); the correct number is 2."), si.channels);
   }

   if(si.sample_freq != 44100)
   {
      mpc_demux_exit(demux);
      demux = NULL;
      throw MDFN_Error(0, _("MusePack stream has wrong samplerate(%u Hz); the correct samplerate is 44100 Hz."), si.sample_freq);
   }
}

CDAFReader_MPC::~CDAFReader_MPC()
{
   if(demux)
   {
      mpc_demux_exit(demux);
      demux = NULL;
   }
}

uint64_t CDAFReader_MPC::Read_(int16 *buffer, uint64_t frames)
{
   mpc_status err;
   int16 *cowbuf = (int16 *)buffer;
   int32_t toread = frames * 2;

   while(toread > 0)
   {
      int32_t tmplen;

      if(!MPCBufferIn)
      {
         mpc_frame_info fi;
         memset(&fi, 0, sizeof(fi));

         fi.buffer = MPCBuffer;
         if((err = mpc_demux_decode(demux, &fi)) < 0 || fi.bits == -1)
            return(frames - toread / 2);

         MPCBufferIn = fi.samples * 2;
         MPCBufferOffs = 0;
      }

      tmplen = MPCBufferIn;

      if(tmplen >= toread)
         tmplen = toread;

      for(int x = 0; x < tmplen; x++)
      {
         int32_t samp = MPCBuffer[MPCBufferOffs + x] >> MPC_FIXED_POINT_FRACTPART;
         if(samp < -32768)
            samp = -32768;

         if(samp > 32767)
            samp = 32767;

         *cowbuf = (int16)samp;
         cowbuf++;
      }

      MPCBufferOffs += tmplen;
      toread -= tmplen;
      MPCBufferIn -= tmplen;
   }

   return(frames - toread / 2);
}

bool CDAFReader_MPC::Seek_(uint64_t frame_offset)
{
   MPCBufferOffs = 0;
   MPCBufferIn = 0;

   if(mpc_demux_seek_sample(demux, frame_offset) < 0)
      return(false);

   return(true);
}

uint64_t CDAFReader_MPC::FrameCount(void)
{
   return(mpc_streaminfo_get_length_samples(&si));
}


CDAFReader* CDAFR_MPC_Open(Stream* fp)
{
   return new CDAFReader_MPC(fp);
}
