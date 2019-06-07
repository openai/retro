#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>
#include <string>

namespace gambatte {

class Bootloader {
   
private:
   uint8_t bootromswapspace[0x900];
   uint8_t rombackup[0x900];
   void* addrspace_start;
   unsigned int bootloadersize;
   bool has_called_FF50;
   
   //Set this to NULL or return false if you want to ignore bootloaders completely
   //(initial value is NULL and so the bootloader is disabled by default)
   bool (*get_raw_bootloader_data)(void* userdata, bool isgbc, uint8_t* data, uint32_t buf_size);

   void patch_gbc_to_gba_mode();
   void uncall_FF50();
   
public:
   bool using_bootloader;
   
   Bootloader();
   void load(bool isgbc,bool isgba);
   void reset();

   void set_bootloader_getter(bool (*getter)(void* userdata, bool isgbc, uint8_t* data, uint32_t buf_size));
   
   void set_address_space_start(void* start);

   void choosebank(bool inbootloader);

   void call_FF50();
};
   
}

#endif

