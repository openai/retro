/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   Mupen64plus - dd_controller.h                                         *
*   Mupen64Plus homepage: https://mupen64plus.org/                        *
*   Copyright (C) 2015 LuigiBlood                                         *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef M64P_DEVICE_DD_CONTROLLER_H
#define M64P_DEVICE_DD_CONTROLLER_H

struct r4300_core;

#include <stdint.h>
#include <time.h>

#include "osal/preproc.h"

struct clock_backend_interface;
struct storage_backend_interface;

/* Disk format sizes */
#define MAME_FORMAT_DUMP_SIZE 0x0435b0c0
#define SDK_FORMAT_DUMP_SIZE  0x03dec800

#define DD_REGION_JP UINT32_C(0xe848d316)
#define DD_REGION_US UINT32_C(0x2263ee56)

enum dd_registers {
    DD_ASIC_DATA,
    DD_ASIC_MISC_REG,
    DD_ASIC_CMD_STATUS,
    DD_ASIC_CUR_TK,
    DD_ASIC_BM_STATUS_CTL,
    DD_ASIC_ERR_SECTOR,
    DD_ASIC_SEQ_STATUS_CTL,
    DD_ASIC_CUR_SECTOR,
    DD_ASIC_HARD_RESET,
    DD_ASIC_C1_S0,
    DD_ASIC_HOST_SECBYTE,
    DD_ASIC_C1_S2,
    DD_ASIC_SEC_BYTE,
    DD_ASIC_C1_S4,
    DD_ASIC_C1_S6,
    DD_ASIC_CUR_ADDR,
    DD_ASIC_ID_REG,
    DD_ASIC_TEST_REG,
    DD_ASIC_TEST_PIN_SEL,
    DD_ASIC_REGS_COUNT
};

struct dd_rtc
{
    time_t now;
    time_t last_update_rtc;

    void* clock;
    const struct clock_backend_interface* iclock;
};

struct dd_controller
{
    uint32_t regs[DD_ASIC_REGS_COUNT];  /* 0x500-0x54f: registers */

    uint8_t c2s_buf[0x400];             /* 0x000-0x3ff: c2s buffer */
    uint8_t ds_buf[0x100];              /* 0x400-0x4ff: data buffer */
    uint8_t ms_ram[0x40];               /* 0x580-0x5bf: micro sequencer */

    /* buffer manager */
    unsigned char bm_write;         /* [0-1] */
    unsigned char bm_reset_held;    /* [0-1] */
    unsigned char bm_block;         /* [0-1] */
    unsigned int bm_zone;           /* [0-15] */
    unsigned int bm_track_offset;   /* */

    /* DD RTC */
    struct dd_rtc rtc;

    /* DD ROM */
    const uint32_t* rom;
    size_t rom_size;

    /* DD Disk */
    void* disk;
    const struct storage_backend_interface* idisk;

    struct r4300_core* r4300;
};

static osal_inline uint32_t dd_reg(uint32_t address)
{
    return (address & 0xff) >> 2;
}

static osal_inline uint32_t dd_rom_address(uint32_t address)
{
    return (address & 0x3fffff) >> 2;
}


void init_dd(struct dd_controller* dd,
             void* clock, const struct clock_backend_interface* iclock,
             const uint32_t* rom, size_t rom_size,
             void* disk, const struct storage_backend_interface* idisk,
             struct r4300_core* r4300);

void poweron_dd(struct dd_controller* dd);

void read_dd_regs(void* opaque, uint32_t address, uint32_t* value);
void write_dd_regs(void* opaque, uint32_t address, uint32_t value, uint32_t mask);

void read_dd_rom(void* opaque, uint32_t address, uint32_t* value);
void write_dd_rom(void* opaque, uint32_t address, uint32_t value, uint32_t mask);

unsigned int dd_dom_dma_read(void* opaque, const uint8_t* dram, uint32_t dram_addr, uint32_t cart_addr, uint32_t length);
unsigned int dd_dom_dma_write(void* opaque, uint8_t* dram, uint32_t dram_addr, uint32_t cart_addr, uint32_t length);

void dd_on_pi_cart_addr_write(struct dd_controller* dd, uint32_t address);
void dd_update_bm(void* opaque);

/* Disk conversion routines */
void dd_convert_to_mame(unsigned char* mame_disk, const unsigned char* sdk_disk);
void dd_convert_to_sdk (const unsigned char* mame_disk, unsigned char* sdk_disk);

#endif
