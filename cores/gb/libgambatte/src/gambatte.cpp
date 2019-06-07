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
#include "gambatte.h"
#include "cpu.h"
#include "savestate.h"
#include "statesaver.h"
#include "initstate.h"
#include "bootloader.h"
#include <sstream>
#include <cstring>

namespace gambatte {
struct GB::Priv {
	CPU cpu;
	int stateNo;
	bool gbaCgbMode;
	
	Priv() : stateNo(1), gbaCgbMode(false) {}

   void full_init();
};
	
GB::GB() : p_(new Priv) {}

GB::~GB() {
	delete p_;
}

long GB::runFor(gambatte::video_pixel_t *const videoBuf, const int pitch,
			gambatte::uint_least32_t *const soundBuf, unsigned &samples) {
	
	p_->cpu.setVideoBuffer(videoBuf, pitch);
	p_->cpu.setSoundBuffer(soundBuf);
	const long cyclesSinceBlit = p_->cpu.runFor(samples * 2);
	samples = p_->cpu.fillSoundBuffer();
	
	return cyclesSinceBlit < 0 ? cyclesSinceBlit : static_cast<long>(samples) - (cyclesSinceBlit >> 1);
}
   
void GB::Priv::full_init() {
   SaveState state;
   
   cpu.setStatePtrs(state);
   setInitState(state, cpu.isCgb(), gbaCgbMode);
   
   cpu.mem_.bootloader.reset();
   cpu.mem_.bootloader.set_address_space_start((void*)cpu.rombank0_ptr());
   cpu.mem_.bootloader.load(cpu.isCgb(), gbaCgbMode);

   if (cpu.mem_.bootloader.using_bootloader) {
      uint8_t *ioamhram = (uint8_t*)state.mem.ioamhram.get();
      uint8_t serialctrl = (cpu.isCgb() || gbaCgbMode) ? 0x7C : 0x7E;
      state.cpu.pc = 0x0000;
      // the hw registers must be zeroed out to prevent the logo from being garbled
      std::memset((void*)(ioamhram + 0x100), 0x00, 0x100);
      //init values taken from SameBoy
      ioamhram[0x100] = 0xCF;//joypad initial value
      ioamhram[0x102] = serialctrl;//serialctrl
      ioamhram[0x148] = 0xFC;//object palette 0
      ioamhram[0x149] = 0xFC;//object palette 1
   }
   
   cpu.loadState(state);
}

void GB::reset() {
   p_->full_init();
}

void GB::setInputGetter(InputGetter *getInput) {
	p_->cpu.setInputGetter(getInput);
}

void GB::setBootloaderGetter(bool (*getter)(void* userdata, bool isgbc, uint8_t* data, uint32_t max_size)) {
   p_->cpu.mem_.bootloader.set_bootloader_getter(getter);
}

#ifdef HAVE_NETWORK
void GB::setSerialIO(SerialIO *serial_io) {
	p_->cpu.setSerialIO(serial_io);
}
#endif

void *GB::savedata_ptr() { return p_->cpu.savedata_ptr(); }
unsigned GB::savedata_size() { return p_->cpu.savedata_size(); }
void *GB::rtcdata_ptr() { return p_->cpu.rtcdata_ptr(); }
unsigned GB::rtcdata_size() { return p_->cpu.rtcdata_size(); }

int GB::load(const void *romdata, unsigned romsize, const unsigned flags) {
	const int failed = p_->cpu.load(romdata, romsize, flags & (FORCE_DMG | FORCE_CGB), flags & MULTICART_COMPAT);
	
   if (!failed) {
      p_->gbaCgbMode = flags & GBA_CGB;
      p_->full_init();
      p_->stateNo = 1;
   }
	
	return failed;
}

bool GB::isCgb() const {
	return p_->cpu.isCgb();
}

bool GB::isLoaded() const {
	return true;
}

void GB::setDmgPaletteColor(unsigned palNum, unsigned colorNum, unsigned rgb32) {
	p_->cpu.setDmgPaletteColor(palNum, colorNum, rgb32);
}

void GB::loadState(const void *data) {
   SaveState state;
   p_->cpu.setStatePtrs(state);
   
   if (StateSaver::loadState(state, data)) {
      p_->cpu.loadState(state);
      p_->cpu.mem_.bootloader.choosebank(state.mem.ioamhram.get()[0x150] != 0xFF);
   }
}

void GB::saveState(void *data) {
   SaveState state;
   p_->cpu.setStatePtrs(state);
   p_->cpu.saveState(state);
   StateSaver::saveState(state, data);
}

size_t GB::stateSize() const {
   SaveState state;
   p_->cpu.setStatePtrs(state);
   p_->cpu.saveState(state);
   return StateSaver::stateSize(state);
}

void GB::setColorCorrection(bool enable) {
   p_->cpu.mem_.display_setColorCorrection(enable);
}

video_pixel_t GB::gbcToRgb32(const unsigned bgr15) {
   return p_->cpu.mem_.display_gbcToRgb32(bgr15);
}


void GB::setGameGenie(const std::string &codes) {
 p_->cpu.setGameGenie(codes);
}

void GB::setGameShark(const std::string &codes) {
 p_->cpu.setGameShark(codes);
}

void GB::clearCheats() {
 p_->cpu.clearCheats();
}

#ifdef __LIBRETRO__
void *GB::vram_ptr() const {
 return p_->cpu.vram_ptr();
}

void *GB::rambank0_ptr() const {
 return p_->cpu.rambank0_ptr();
}

void *GB::rambank1_ptr() const {
 return p_->cpu.rambank1_ptr();
}

void *GB::rombank0_ptr() const {
 return p_->cpu.rombank0_ptr();
}

void *GB::rombank1_ptr() const {
 return p_->cpu.rombank1_ptr();
}

void *GB::zeropage_ptr() const {
 return p_->cpu.zeropage_ptr();
}

void *GB::oamram_ptr() const {
 return p_->cpu.oamram_ptr();
}
#endif

}

