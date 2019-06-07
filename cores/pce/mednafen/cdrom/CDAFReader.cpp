/******************************************************************************/
/* Mednafen - Multi-system Emulator                                           */
/******************************************************************************/
/* CDAFReader.cpp:
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

// CDAFR_Open(), and CDAFReader, will NOT take "ownership" of the Stream object(IE it won't ever delete it).  Though it does assume it has exclusive access
// to it for as long as the CDAFReader object exists.

// Don't allow exceptions to propagate into the vorbis/musepack/etc. libraries, as it could easily leave the state of the library's decoder "object" in an
// inconsistent state, which would cause all sorts of unfun when we try to destroy it while handling the exception farther up.

#include <mednafen/mednafen.h>
#include "CDAFReader.h"
#include "CDAFReader_Vorbis.h"
#ifdef HAVE_MPC
#include "CDAFReader_MPC.h"
#endif

CDAFReader::CDAFReader() : LastReadPos(0)
{

}

CDAFReader::~CDAFReader()
{

}

CDAFReader* CDAFR_Open(Stream* fp)
{
#ifdef HAVE_MPC
  return CDAFR_MPC_Open(fp);
#else
  return CDAFR_Vorbis_Open(fp);
#endif
}

