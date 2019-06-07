#ifndef __MDFN_CDACCESS_IMAGE_H
#define __MDFN_CDACCESS_IMAGE_H

#include <map>

#include "CDUtility.h"

class Stream;
class CDAFReader;

struct CDRFILE_TRACK_INFO
{
   int32_t LBA;

   uint32_t DIFormat;
   uint8_t subq_control;

   int32_t pregap;
   int32_t pregap_dv;

   int32_t postgap;

   int32_t index[100];

   int32_t sectors;	// Not including pregap sectors!
   Stream *fp;
   bool FirstFileInstance;
   bool RawAudioMSBFirst;
   long FileOffset;
   unsigned int SubchannelMode;

   uint32_t LastSamplePos;

   CDAFReader *AReader;
};

template<typename T, size_t N>
class stl_array {
   T m_data[N];
   public:
   T* begin()              { return m_data; }
   T* end()                { return m_data + N; }
   T& operator[](size_t i) { return m_data[i]; }
   size_t size()           { return N; }
   bool empty()            { return N == 0; }
   T* data()               { return m_data; }

   const T* begin() const  { return m_data; }
   const T*   end() const  { return m_data + N; }
   const T& operator[](size_t i) const { return m_data[i]; }
   const T*  data() const  { return m_data; }
};

class CDAccess_Image : public CDAccess
{
   public:

      CDAccess_Image(const std::string& path, bool image_memcache);
      virtual ~CDAccess_Image();

      virtual bool Read_Raw_Sector(uint8_t *buf, int32_t lba);

      virtual bool Fast_Read_Raw_PW_TSRE(uint8_t* pwbuf, int32_t lba);

      virtual bool Read_TOC(TOC *toc);

   private:

      int32_t NumTracks;
      int32_t FirstTrack;
      int32_t LastTrack;
      int32_t total_sectors;
      uint8_t disc_type;
      CDRFILE_TRACK_INFO Tracks[100]; // Track #0(HMM?) through 99
      TOC toc;

      std::map<uint32_t, stl_array<uint8_t, 12> > SubQReplaceMap;

      std::string base_dir;

      bool ImageOpen(const std::string& path, bool image_memcache);
      bool LoadSBI(const std::string& sbi_path);
      void GenerateTOC(void);
      void Cleanup(void);

      // MakeSubPQ will OR the simulated P and Q subchannel data into SubPWBuf.
      int32_t MakeSubPQ(int32_t lba, uint8_t *SubPWBuf) const;

      bool ParseTOCFileLineInfo(CDRFILE_TRACK_INFO *track, const int tracknum, const std::string &filename, const char *binoffset, const char *msfoffset, const char *length, bool image_memcache, std::map<std::string, Stream*> &toc_streamcache);
      uint32_t GetSectorCount(CDRFILE_TRACK_INFO *track);
};


#endif
