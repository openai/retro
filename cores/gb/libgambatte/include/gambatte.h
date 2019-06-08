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
#ifndef GAMBATTE_H
#define GAMBATTE_H

#include "inputgetter.h"
#ifdef HAVE_NETWORK
#include "serial_io.h"
#endif
#include "gbint.h"
#include <string>
#include <cstddef>

namespace gambatte {
#ifdef VIDEO_RGB565
typedef uint16_t video_pixel_t;
#else
typedef uint_least32_t video_pixel_t;
#endif
enum { BG_PALETTE = 0, SP1_PALETTE = 1, SP2_PALETTE = 2 };

class GB {
public:
	GB();
	~GB();
	
	enum LoadFlag {
      FORCE_DMG        = 1, /**< Treat the ROM as not having CGB support regardless of what its header advertises. */
      GBA_CGB          = 2, /**< Use GBA intial CPU register values when in CGB mode. */
      MULTICART_COMPAT = 4,  /**< Use heuristics to detect and support some multicart MBCs disguised as MBC1. */
      FORCE_CGB        = 8
	};
	
   int load(const void *romdata, unsigned size, unsigned flags = 0);
	
	/** Emulates until at least 'samples' stereo sound samples are produced in the supplied buffer,
	  * or until a video frame has been drawn.
	  *
	  * There are 35112 stereo sound samples in a video frame.
	  * May run for uptil 2064 stereo samples too long.
	  * A stereo sample consists of two native endian 2s complement 16-bit PCM samples,
	  * with the left sample preceding the right one. Usually casting soundBuf to/from
	  * short* is OK and recommended. The reason for not using a short* in the interface
	  * is to avoid implementation-defined behaviour without compromising performance.
	  *
	  * Returns early when a new video frame has finished drawing in the video buffer,
	  * such that the caller may update the video output before the frame is overwritten.
	  * The return value indicates whether a new video frame has been drawn, and the
	  * exact time (in number of samples) at which it was drawn.
	  *
	  * @param videoBuf 160x144 RGB32 (native endian) video frame buffer or 0
	  * @param pitch distance in number of pixels (not bytes) from the start of one line to the next in videoBuf.
	  * @param soundBuf buffer with space >= samples + 2064
	  * @param samples in: number of stereo samples to produce, out: actual number of samples produced
	  * @return sample number at which the video frame was produced. -1 means no frame was produced.
	  */
	long runFor(gambatte::video_pixel_t *videoBuf, int pitch,
			gambatte::uint_least32_t *soundBuf, unsigned &samples);
	
	/** Reset to initial state.
	  * Equivalent to reloading a ROM image, or turning a Game Boy Color off and on again.
	  */
	void reset();
	
	/** @param palNum 0 <= palNum < 3. One of BG_PALETTE, SP1_PALETTE and SP2_PALETTE.
	  * @param colorNum 0 <= colorNum < 4
	  */
	void setDmgPaletteColor(unsigned palNum, unsigned colorNum, unsigned rgb32);

	/** Sets the callback used for getting input state. */
	void setInputGetter(InputGetter *getInput);
   
   /** Sets the callback used for getting the bootloader data. */
   void setBootloaderGetter(bool (*getter)(void *userdata, bool isgbc, uint8_t *data, uint32_t buf_size));

#ifdef HAVE_NETWORK
	/** Sets the callback used for transferring serial data. */
	void setSerialIO(SerialIO *serial_io);
#endif
	
	/** Sets the directory used for storing save data. The default is the same directory as the ROM Image file. */
	void setSaveDir(const std::string &sdir);

   void *savedata_ptr();
   unsigned savedata_size();
   void *rtcdata_ptr();
   unsigned rtcdata_size();
	
	/** Returns true if the currently loaded ROM image is treated as having CGB support. */
	bool isCgb() const;
	
	/** Returns true if a ROM image is loaded. */
	bool isLoaded() const;
	
   void saveState(void *data);
   void loadState(const void *data);
   size_t stateSize() const;

   void setColorCorrection(bool enable);
   video_pixel_t gbcToRgb32(const unsigned bgr15);

   /** Set Game Genie codes to apply to currently loaded ROM image. Cleared on ROM load.
    * @param codes Game Genie codes in format HHH-HHH-HHH;HHH-HHH-HHH;... where H is [0-9]|[A-F]
    */
   void setGameGenie(const std::string &codes);

   /** Set Game Shark codes to apply to currently loaded ROM image. Cleared on ROM load.
    * @param codes Game Shark codes in format 01HHHHHH;01HHHHHH;... where H is [0-9]|[A-F]
    */
   void setGameShark(const std::string &codes);

   void clearCheats();
   
#ifdef __LIBRETRO__
   void *vram_ptr() const;
   void *rambank0_ptr() const;
   void *rambank1_ptr() const;
   void *rombank0_ptr() const;
   void *rombank1_ptr() const;
   void *zeropage_ptr() const;
   void *oamram_ptr() const;
#endif

private:
	struct Priv;
	Priv *const p_;

	void loadState(const std::string &filepath, bool osdMessage);
	GB(const GB &);
	GB & operator=(const GB &);
};
}

#endif
