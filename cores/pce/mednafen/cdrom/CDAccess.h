#ifndef __MDFN_CDROMFILE_H
#define __MDFN_CDROMFILE_H

#include <stdio.h>

#include "CDUtility.h"

class CDAccess
{
 public:

 CDAccess();
 virtual ~CDAccess();

 virtual bool Read_Raw_Sector(uint8_t *buf, int32_t lba) = 0;

 // Returns false if the read wouldn't be "fast"(i.e. reading from a disk),
 // or if the read can't be done in a thread-safe re-entrant manner.
 //
 // Writes 96 bytes into pwbuf, and returns 'true' otherwise.
 virtual bool Fast_Read_Raw_PW_TSRE(uint8_t* pwbuf, int32_t lba) = 0;

 virtual bool Read_TOC(TOC *toc) = 0;

 private:
 CDAccess(const CDAccess&);	// No copy constructor.
 CDAccess& operator=(const CDAccess&); // No assignment operator.
};

CDAccess* CDAccess_Open(const std::string& path, bool image_memcache);

#endif
