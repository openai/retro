#include <stdint.h>
#include <cstring>
#include <string>

#include "bootloader.h"

namespace gambatte {

Bootloader::Bootloader() {
   get_raw_bootloader_data = NULL;
}

void Bootloader::patch_gbc_to_gba_mode() {
   /*moves one jump over another and puts ld b,0x01 into the original position*/
   uint16_t patchloc = 0xF2;
   uint8_t patch[0x7] = {0xCD,0xD0,0x05/*<-call systemsetup*/,0x06,0x01/*<-ld b,0x1*/,0x00/*<-nop*/,0x00/*<-nop*/};
   std::memcpy(bootromswapspace + patchloc, patch, 0x7);
}

void Bootloader::load(bool isgbc, bool isgba) {
   if (get_raw_bootloader_data == NULL) {
      using_bootloader = false;
      return;
   }
   
   //the gba only uses the gbc bios
   if (isgba)
      isgbc = true;
   
   bool bootloaderavail = get_raw_bootloader_data((void*)this, isgbc, bootromswapspace, 0x900/*buf_size*/);
   if (!bootloaderavail) {
      using_bootloader = false;
      return;
   }
   
   if (isgbc)
      bootloadersize = 0x900;
   else
      bootloadersize = 0x100;
   
   if (isgba)//patch bootloader to fake gba mode
      patch_gbc_to_gba_mode();
   
   //backup rom segment that is shared with bootloader
   std::memcpy(rombackup, (uint8_t*)addrspace_start, bootloadersize);
   
   //put back cartridge data in a 256 byte window of the bios that is not mapped(GBC only)
   if (isgbc)
      std::memcpy(bootromswapspace + 0x100, rombackup + 0x100, 0x100);
   
   //put bootloader in main memory
   std::memcpy((uint8_t*)addrspace_start, bootromswapspace, bootloadersize);
   
   using_bootloader = true;
}

void Bootloader::reset() {
   bootloadersize = 0;
   has_called_FF50 = false;
   addrspace_start = NULL;
   using_bootloader = false;
}

void Bootloader::set_bootloader_getter(bool (*getter)(void* userdata, bool isgbc, uint8_t* data, uint32_t buf_size)) {
   get_raw_bootloader_data = getter;
}

void Bootloader::set_address_space_start(void* start) {
   addrspace_start = start;
}

void Bootloader::choosebank(bool inbootloader) {
   //inbootloader = (state.mem.ioamhram.get()[0x150] != 0xFF);//do not uncomment this is just for reference
   if (using_bootloader) {
      
      //switching from game to bootloader with savestate
      if (inbootloader && has_called_FF50)
         uncall_FF50();
      
      //switching from bootloader to game with savestate
      else if (!inbootloader && !has_called_FF50)
         call_FF50();
      
      //switching from game to game or bootloader to bootloader needs no changes
      
   }
}

void Bootloader::call_FF50() {
   if (!has_called_FF50 && using_bootloader) {
      //put rom back in main memory when bootloader has finished
      std::memcpy((uint8_t*)addrspace_start, rombackup, bootloadersize);
      has_called_FF50 = true;
   }
}

//this is a developer function only,a real gameboy can never undo calling 0xFF50,this function is for savestate functionality
void Bootloader::uncall_FF50() {
   std::memcpy((uint8_t*)addrspace_start, bootromswapspace, bootloadersize);
   has_called_FF50 = false;
}
   
}
