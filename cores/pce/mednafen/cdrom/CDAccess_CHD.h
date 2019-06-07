/******************************************************************************/
/* Mednafen - Multi-system Emulator                                           */
/******************************************************************************/
/* CDAccess_CHD.h:
**  Copyright (C) 2017 Romain Tisserand
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

#include <mednafen/FileStream.h>
#include <mednafen/MemoryStream.h>

#include "CDAccess.h"
#include "chd.h"

struct CHDFILE_TRACK_INFO
{
   int32_t LBA;

   uint32_t DIFormat;
   uint8_t subq_control;

   int32_t pregap;
   int32_t pregap_dv;

   int32_t postgap;

   int32_t index[100];

   int32_t sectors; // Not including pregap sectors!
   bool FirstFileInstance;
   bool RawAudioMSBFirst;
   long FileOffset;
   unsigned int SubchannelMode;

   uint32_t LastSamplePos;

   uint32_t fileOffset;
};

class CDAccess_CHD : public CDAccess
{
 public:

 CDAccess_CHD(const std::string& path, bool image_memcache);
 virtual ~CDAccess_CHD();

 virtual bool Read_Raw_Sector(uint8 *buf, int32 lba);

 virtual bool Fast_Read_Raw_PW_TSRE(uint8* pwbuf, int32 lba);

 virtual bool Read_TOC(TOC *toc);

 private:

 bool Load(const std::string& path, bool image_memcache);
 void Cleanup(void);

  // MakeSubPQ will OR the simulated P and Q subchannel data into SubPWBuf.
  int32_t MakeSubPQ(int32_t lba, uint8_t *SubPWBuf) const;

  bool Read_CHD_Hunk_RAW(uint8_t *buf, int32_t lba, CHDFILE_TRACK_INFO* track);
  bool Read_CHD_Hunk_M1(uint8_t *buf, int32_t lba, CHDFILE_TRACK_INFO* track);
  bool Read_CHD_Hunk_M2(uint8_t *buf, int32_t lba, CHDFILE_TRACK_INFO* track);

  int32_t NumTracks;
  int32_t FirstTrack;
  int32_t LastTrack;
  int32_t total_sectors;
  uint8_t disc_type;
  TOC toc;
  CHDFILE_TRACK_INFO Tracks[100]; // Track #0(HMM?) through 99

  //struct disc;
  //struct session sessions[DISC_MAX_SESSIONS];
  int num_sessions;
  //struct track tracks[DISC_MAX_TRACKS];
  int num_tracks;

  chd_file *chd;
  /* hunk data cache */
  uint8_t *hunkmem;
  /* last hunknum read */
  int oldhunk;
};
