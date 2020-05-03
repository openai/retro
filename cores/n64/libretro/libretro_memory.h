#ifndef M64P_LIBRETRO_MEMORY_H
#define M64P_LIBRETRO_MEMORY_H

#include <stdint.h>
#define EEPROM_MAX_SIZE 0x800
#include <device/controllers/paks/mempak.h>
#include <device/cart/flashram.h>
#include <device/cart/sram.h>

typedef struct _save_memory_data
{
   uint8_t eeprom[EEPROM_MAX_SIZE];
   uint8_t mempack[MEMPAK_SIZE * 4];
   uint8_t sram[SRAM_SIZE];
   uint8_t flashram[FLASHRAM_SIZE];
} save_memory_data;

extern save_memory_data saved_memory;

#endif
