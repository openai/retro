#pragma once

#include <stdint.h>

void plugin_init(void);
void plugin_sync_dp(void);
uint32_t** plugin_get_dp_registers(void);
uint32_t** plugin_get_vi_registers(void);
uint8_t* plugin_get_rdram(void);
uint32_t plugin_get_rdram_size(void);
uint8_t* plugin_get_dmem(void);
uint8_t* plugin_get_rom_header(void);
uint32_t plugin_get_rom_name(char* name, uint32_t name_size);
void plugin_close(void);
