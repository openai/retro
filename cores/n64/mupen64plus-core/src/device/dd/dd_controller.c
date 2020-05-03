/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   Mupen64plus - dd_controller.c                                         *
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

#include "dd_controller.h"

#include <assert.h>
#include <string.h>
#include <time.h>

#define M64P_CORE_PROTOTYPES 1
#include "api/m64p_types.h"
#include "api/callbacks.h"
#include "backends/api/clock_backend.h"
#include "backends/api/storage_backend.h"
#include "device/device.h"
#include "device/memory/memory.h"
#include "device/r4300/r4300_core.h"

/* dd commands definition */
#define DD_CMD_NOOP             UINT32_C(0x00000000)
#define DD_CMD_SEEK_READ        UINT32_C(0x00010001)
#define DD_CMD_SEEK_WRITE       UINT32_C(0x00020001)
#define DD_CMD_RECALIBRATE      UINT32_C(0x00030001) // ???
#define DD_CMD_SLEEP            UINT32_C(0x00040000)
#define DD_CMD_START            UINT32_C(0x00050001)
#define DD_CMD_SET_STANDBY      UINT32_C(0x00060000)
#define DD_CMD_SET_SLEEP        UINT32_C(0x00070000)
#define DD_CMD_CLR_DSK_CHNG     UINT32_C(0x00080000)
#define DD_CMD_CLR_RESET        UINT32_C(0x00090000)
#define DD_CMD_READ_VERSION     UINT32_C(0x000A0000)
#define DD_CMD_SET_DISK_TYPE    UINT32_C(0x000B0001)
#define DD_CMD_REQUEST_STATUS   UINT32_C(0x000C0000)
#define DD_CMD_STANDBY          UINT32_C(0x000D0000)
#define DD_CMD_IDX_LOCK_RETRY   UINT32_C(0x000E0000) // ???
#define DD_CMD_SET_YEAR_MONTH   UINT32_C(0x000F0000)
#define DD_CMD_SET_DAY_HOUR     UINT32_C(0x00100000)
#define DD_CMD_SET_MIN_SEC      UINT32_C(0x00110000)
#define DD_CMD_GET_YEAR_MONTH   UINT32_C(0x00120000)
#define DD_CMD_GET_DAY_HOUR     UINT32_C(0x00130000)
#define DD_CMD_GET_MIN_SEC      UINT32_C(0x00140000)
#define DD_CMD_FEATURE_INQ      UINT32_C(0x001B0000)

/* dd status register bitfields definition */
#define DD_STATUS_DATA_RQ       UINT32_C(0x40000000)
#define DD_STATUS_C2_XFER       UINT32_C(0x10000000)
#define DD_STATUS_BM_ERR        UINT32_C(0x08000000)
#define DD_STATUS_BM_INT        UINT32_C(0x04000000)
#define DD_STATUS_MECHA_INT     UINT32_C(0x02000000)
#define DD_STATUS_DISK_PRES     UINT32_C(0x01000000)
#define DD_STATUS_BUSY_STATE    UINT32_C(0x00800000)
#define DD_STATUS_RST_STATE     UINT32_C(0x00400000)
#define DD_STATUS_MTR_N_SPIN    UINT32_C(0x00100000)
#define DD_STATUS_HEAD_RTRCT    UINT32_C(0x00080000)
#define DD_STATUS_WR_PR_ERR     UINT32_C(0x00040000)
#define DD_STATUS_MECHA_ERR     UINT32_C(0x00020000)
#define DD_STATUS_DISK_CHNG     UINT32_C(0x00010000)

/* dd bm status register bitfields definition */
/* read flags */
#define DD_BM_STATUS_RUNNING    UINT32_C(0x80000000)
#define DD_BM_STATUS_ERROR      UINT32_C(0x04000000)
#define DD_BM_STATUS_MICRO      UINT32_C(0x02000000) /* ??? */
#define DD_BM_STATUS_BLOCK      UINT32_C(0x01000000)
#define DD_BM_STATUS_C1CRR      UINT32_C(0x00800000)
#define DD_BM_STATUS_C1DBL      UINT32_C(0x00400000)
#define DD_BM_STATUS_C1SNG      UINT32_C(0x00200000)
#define DD_BM_STATUS_C1ERR      UINT32_C(0x00010000) /* Typo ??? */
/* write flags */
#define DD_BM_CTL_START         UINT32_C(0x80000000)
#define DD_BM_CTL_MNGRMODE      UINT32_C(0x40000000)
#define DD_BM_CTL_INTMASK       UINT32_C(0x20000000)
#define DD_BM_CTL_RESET         UINT32_C(0x10000000)
#define DD_BM_CTL_DIS_OR_CHK    UINT32_C(0x08000000) /* ??? */
#define DD_BM_CTL_DIS_C1_CRR    UINT32_C(0x04000000)
#define DD_BM_CTL_BLK_TRANS     UINT32_C(0x02000000)
#define DD_BM_CTL_MECHA_RST     UINT32_C(0x01000000)

#define DD_TRACK_LOCK           UINT32_C(0x60000000)

/* disk geometry definitions */
enum { SECTORS_PER_BLOCK = 85 };
enum { BLOCKS_PER_TRACK  = 2  };

enum { DD_DISK_SYSTEM_DATA_SIZE = 0xe8 };

static const unsigned int zone_sec_size[16] = {
    232, 216, 208, 192, 176, 160, 144, 128,
    216, 208, 192, 176, 160, 144, 128, 112
};

static const uint32_t ZoneTracks[16] = {
    158, 158, 149, 149, 149, 149, 149, 114,
    158, 158, 149, 149, 149, 149, 149, 114
};
static const uint32_t DiskTypeZones[7][16] = {
    { 0, 1, 2, 9, 8, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10 },
    { 0, 1, 2, 3, 10, 9, 8, 4, 5, 6, 7, 15, 14, 13, 12, 11 },
    { 0, 1, 2, 3, 4, 11, 10, 9, 8, 5, 6, 7, 15, 14, 13, 12 },
    { 0, 1, 2, 3, 4, 5, 12, 11, 10, 9, 8, 6, 7, 15, 14, 13 },
    { 0, 1, 2, 3, 4, 5, 6, 13, 12, 11, 10, 9, 8, 7, 15, 14 },
    { 0, 1, 2, 3, 4, 5, 6, 7, 14, 13, 12, 11, 10, 9, 8, 15 },
    { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8 }
};
static const uint32_t RevDiskTypeZones[7][16] = {
    { 0, 1, 2, 5, 6, 7, 8, 9, 4, 3, 15, 14, 13, 12, 11, 10 },
    { 0, 1, 2, 3, 7, 8, 9, 10, 6, 5, 4, 15, 14, 13, 12, 11 },
    { 0, 1, 2, 3, 4, 9, 10, 11, 8, 7, 6, 5, 15, 14, 13, 12 },
    { 0, 1, 2, 3, 4, 5, 11, 12, 10, 9, 8, 7, 6, 15, 14, 13 },
    { 0, 1, 2, 3, 4, 5, 6, 13, 12, 11, 10, 9, 8, 7, 15, 14 },
    { 0, 1, 2, 3, 4, 5, 6, 7, 14, 13, 12, 11, 10, 9, 8, 15 },
    { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8 }
};
static const uint32_t StartBlock[7][16] = {
    { 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1 },
    { 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0 },
    { 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1 },
    { 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0 },
    { 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1 },
    { 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0 },
    { 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1 }
};


#define BLOCKSIZE(_zone) zone_sec_size[_zone] * SECTORS_PER_BLOCK
#define TRACKSIZE(_zone) BLOCKSIZE(_zone) * BLOCKS_PER_TRACK
#define ZONESIZE(_zone) TRACKSIZE(_zone) * ZoneTracks[_zone]
#define VZONESIZE(_zone) TRACKSIZE(_zone) * (ZoneTracks[_zone] - 0xC)






static uint8_t byte2bcd(int n)
{
    n %= 100;
    return (uint8_t)(((n / 10) << 4) | (n % 10));
}

static uint32_t time2data(int hi, int lo)
{
    return ((uint32_t)byte2bcd(hi) << 24)
         | ((uint32_t)byte2bcd(lo) << 16);
}

static void update_rtc(struct dd_rtc* rtc)
{
    /* update rtc->now */
    time_t now = rtc->iclock->get_time(rtc->clock);
    rtc->now += now - rtc->last_update_rtc;
    rtc->last_update_rtc = now;
}

static void signal_dd_interrupt(struct dd_controller* dd, uint32_t bm_int)
{
    dd->regs[DD_ASIC_CMD_STATUS] |= bm_int;
    r4300_check_interrupt(dd->r4300, CP0_CAUSE_IP3, 1);
}

static void clear_dd_interrupt(struct dd_controller* dd, uint32_t bm_int)
{
    dd->regs[DD_ASIC_CMD_STATUS] &= ~bm_int;
    r4300_check_interrupt(dd->r4300, CP0_CAUSE_IP3, 0);
}

static void read_C2(struct dd_controller* dd)
{
    size_t i;

    size_t length = zone_sec_size[dd->bm_zone];
    size_t offset = 0x40 * (dd->regs[DD_ASIC_CUR_SECTOR] - SECTORS_PER_BLOCK);

    DebugMessage(M64MSG_VERBOSE, "read C2: length=%08x, offset=%08x",
            (uint32_t)length, (uint32_t)offset);

    for (i = 0; i < length; ++i) {
        dd->c2s_buf[(offset + i) ^ 3] = 0;
    }
}

static void read_sector(struct dd_controller* dd)
{
    size_t i;
    const uint8_t* disk_mem = dd->idisk->data(dd->disk);
    size_t offset = dd->bm_track_offset
        + dd->bm_block * BLOCKSIZE(dd->bm_zone)
        + dd->regs[DD_ASIC_CUR_SECTOR] * (dd->regs[DD_ASIC_HOST_SECBYTE] + 1);
    size_t length = dd->regs[DD_ASIC_HOST_SECBYTE] + 1;

    for (i = 0; i < length; ++i) {
        dd->ds_buf[i ^ 3] = disk_mem[offset + i];
    }
}

static void write_sector(struct dd_controller* dd)
{
    size_t i;
    uint8_t* disk_mem = dd->idisk->data(dd->disk);
    size_t offset = dd->bm_track_offset
        + dd->bm_block * BLOCKSIZE(dd->bm_zone)
        + (dd->regs[DD_ASIC_CUR_SECTOR] - 1) * zone_sec_size[dd->bm_zone];
    size_t length = zone_sec_size[dd->bm_zone];

	for (i = 0; i < length; ++i) {
		disk_mem[offset + i] = dd->ds_buf[i ^ 3];
    }

#if 0 /* disabled for now, because it causes too much slowdowns */
    dd->idisk->save(dd->disk);
#endif
}

static void seek_track(struct dd_controller* dd)
{
    static const unsigned int start_offset[] = {
        0x0000000, 0x05f15e0, 0x0b79d00, 0x10801a0,
        0x1523720, 0x1963d80, 0x1d414c0, 0x20bbce0,
        0x23196e0, 0x28a1e00, 0x2df5dc0, 0x3299340,
        0x36d99a0, 0x3ab70e0, 0x3e31900, 0x4149200
    };

    static const unsigned int tracks[] = {
        0x000, 0x09e, 0x13c, 0x1d1, 0x266, 0x2fb, 0x390, 0x425
    };

	unsigned int tr_off;
	unsigned int head_x_8 = ((dd->regs[DD_ASIC_CUR_TK] & 0x1000) >> 9);
	unsigned int track    =  (dd->regs[DD_ASIC_CUR_TK] & 0x0fff);

    /* find track bm_zone */
    for (dd->bm_zone = 7; dd->bm_zone > 0; --dd->bm_zone) {
        if (track >= tracks[dd->bm_zone]) {
            break;
        }
    }

    tr_off = track - tracks[dd->bm_zone];

    /* set zone and track offset */
    dd->bm_zone += head_x_8;
	dd->bm_track_offset = start_offset[dd->bm_zone] + tr_off * TRACKSIZE(dd->bm_zone);

    /* lock track */
    dd->regs[DD_ASIC_CUR_TK] |= DD_TRACK_LOCK;
}

void dd_update_bm(void* opaque)
{
    struct dd_controller* dd = (struct dd_controller*)opaque;

    /* not running */
	if ((dd->regs[DD_ASIC_BM_STATUS_CTL] & DD_BM_STATUS_RUNNING) == 0) {
		return;
    }

    /* handle writes (BM mode 0) */
    if (dd->bm_write) {
        /* first sector : just issue a BM interrupt to get things going */
        if (dd->regs[DD_ASIC_CUR_SECTOR] == 0) {
            ++dd->regs[DD_ASIC_CUR_SECTOR];
            dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_DATA_RQ;
        }
        /* subsequent sectors: write previous sector */
        else if (dd->regs[DD_ASIC_CUR_SECTOR] < SECTORS_PER_BLOCK) {
            write_sector(dd);
            ++dd->regs[DD_ASIC_CUR_SECTOR];
            dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_DATA_RQ;
        }
        /* otherwise write last sector */
        else if (dd->regs[DD_ASIC_CUR_SECTOR] < SECTORS_PER_BLOCK + 1) {
            /* continue to next block */
            if (dd->regs[DD_ASIC_BM_STATUS_CTL] & DD_BM_STATUS_BLOCK) {
                write_sector(dd);
                dd->bm_block = 1 - dd->bm_block;
                dd->regs[DD_ASIC_CUR_SECTOR] = 1;
                dd->regs[DD_ASIC_BM_STATUS_CTL] &= ~DD_BM_STATUS_BLOCK;
                dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_DATA_RQ;
            /* quit writing after second block */
            } else {
                write_sector(dd);
                ++dd->regs[DD_ASIC_CUR_SECTOR];
                dd->regs[DD_ASIC_BM_STATUS_CTL] &= ~DD_BM_STATUS_RUNNING;
            }
        }
        else {
            DebugMessage(M64MSG_ERROR, "DD Write, sector overrun");
        }
    }
    /* handle reads (BM mode 1) */
    else {
        /* track 6 fails to read on retail units (XXX: retail test) */
        if (((dd->regs[DD_ASIC_CUR_TK] & 0x1fff) == 6) && dd->bm_block == 0) {
            dd->regs[DD_ASIC_CMD_STATUS] &= ~DD_STATUS_DATA_RQ;
            dd->regs[DD_ASIC_BM_STATUS_CTL] |= DD_BM_STATUS_MICRO;
        }
        /* data sectors : read sector and signal BM interrupt */
        else if (dd->regs[DD_ASIC_CUR_SECTOR] < SECTORS_PER_BLOCK) {
            read_sector(dd);
            ++dd->regs[DD_ASIC_CUR_SECTOR];
            dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_DATA_RQ;
        }
        /* C2 sectors: do nothing since they're loaded with zeros */
        else if (dd->regs[DD_ASIC_CUR_SECTOR] < SECTORS_PER_BLOCK + 4) {
            read_C2(dd);
            ++dd->regs[DD_ASIC_CUR_SECTOR];
            if (dd->regs[DD_ASIC_CUR_SECTOR] == SECTORS_PER_BLOCK + 4) {
                dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_C2_XFER;
            }
        }
        /* Gap sector: continue to next block, quit after second block */
        else if (dd->regs[DD_ASIC_CUR_SECTOR] == SECTORS_PER_BLOCK + 4) {
            if (dd->regs[DD_ASIC_BM_STATUS_CTL] & DD_BM_STATUS_BLOCK) {
                dd->bm_block = 1 - dd->bm_block;
                dd->regs[DD_ASIC_CUR_SECTOR] = 0;
                dd->regs[DD_ASIC_BM_STATUS_CTL] &= ~DD_BM_STATUS_BLOCK;
            }
            else {
                dd->regs[DD_ASIC_BM_STATUS_CTL] &= ~DD_BM_STATUS_RUNNING;
            }
        }
        else {
            DebugMessage(M64MSG_ERROR, "DD Read, sector overrun");
        }
    }

    /* Signal a BM interrupt */
    signal_dd_interrupt(dd, DD_STATUS_BM_INT);
}



void init_dd(struct dd_controller* dd,
             void* clock, const struct clock_backend_interface* iclock,
             const uint32_t* rom, size_t rom_size,
             void* disk, const struct storage_backend_interface* idisk,
             struct r4300_core* r4300)
{
    dd->rtc.clock = clock;
    dd->rtc.iclock = iclock;

    dd->rom = rom;
    dd->rom_size = rom_size;

    dd->disk = disk;
    dd->idisk = idisk;

    dd->r4300 = r4300;
}

void poweron_dd(struct dd_controller* dd)
{
    memset(dd->regs, 0, DD_ASIC_REGS_COUNT*sizeof(dd->regs[0]));
    memset(dd->c2s_buf, 0, 0x400);
    memset(dd->ds_buf, 0, 0x100);
    memset(dd->ms_ram, 0, 0x40);

    dd->bm_write = 0;
    dd->bm_reset_held = 0;
    dd->bm_block = 0;
    dd->bm_zone = 0;
    dd->bm_track_offset = 0;

    dd->rtc.now = 0;
    dd->rtc.last_update_rtc = 0;

    dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_RST_STATE;
    if (dd->idisk != NULL) {
        dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_DISK_PRES;
    }

    /* XXX: add non retail support */
    dd->regs[DD_ASIC_ID_REG] = 0x00030000;
}

void read_dd_regs(void* opaque, uint32_t address, uint32_t* value)
{
    struct dd_controller* dd = (struct dd_controller*)opaque;

    if (address < MM_DD_REGS || address >= MM_DD_MS_RAM) {
        DebugMessage(M64MSG_ERROR, "Unknown access in DD registers MMIO space %08x", address);
        *value = 0;
        return;
    }

    uint32_t reg = dd_reg(address);

    /* disk presence test */
    if (reg == DD_ASIC_CMD_STATUS) {
        if (dd->idisk != NULL) {
            dd->regs[reg] |= DD_STATUS_DISK_PRES;
        }
        else {
            dd->regs[reg] &= ~DD_STATUS_DISK_PRES;
        }
    }

    *value = dd->regs[reg];
    DebugMessage(M64MSG_VERBOSE, "DD REG: %08X -> %08x", address, *value);

    /* post read update. Not part of the returned value */
    switch(reg)
    {
    case DD_ASIC_CMD_STATUS: {
            /* clear BM interrupt when reading gap */
            if ((dd->regs[DD_ASIC_CMD_STATUS] & DD_STATUS_BM_INT) && (dd->regs[DD_ASIC_CUR_SECTOR] > SECTORS_PER_BLOCK)) {
                clear_dd_interrupt(dd, DD_STATUS_BM_INT);
                dd_update_bm(dd);
            }
        } break;
    }
}

void write_dd_regs(void* opaque, uint32_t address, uint32_t value, uint32_t mask)
{
    uint8_t start_sector;
    struct dd_controller* dd = (struct dd_controller*)opaque;

    if (address < MM_DD_REGS || address >= MM_DD_MS_RAM) {
        DebugMessage(M64MSG_ERROR, "Unknown access in DD registers MMIO space %08x", address);
        return;
    }

    uint32_t reg = dd_reg(address);

    assert(mask == ~UINT32_C(0));

    DebugMessage(M64MSG_VERBOSE, "DD REG: %08X <- %08x", address, value);

    switch (reg)
    {
    case DD_ASIC_DATA:
        dd->regs[DD_ASIC_DATA] = value;
        break;

    case DD_ASIC_CMD_STATUS:
        update_rtc(&dd->rtc);
        const struct tm* tm = localtime(&dd->rtc.now);

        switch ((value >> 16) & 0xff)
        {
        /* No-op */
        case 0x00:
            break;

        /* Seek track */
        case 0x01:
        case 0x02:
            dd->regs[DD_ASIC_CUR_TK] = dd->regs[DD_ASIC_DATA] >> 16;
            seek_track(dd);
            dd->bm_write = (value >> 17) & 0x1;
            break;

        /* Clear Disk change flag */
        case 0x08:
            dd->regs[DD_ASIC_CMD_STATUS] &= ~DD_STATUS_DISK_CHNG;
            break;

        /* Clear reset flag */
        case 0x09:
            dd->regs[DD_ASIC_CMD_STATUS] &= ~DD_STATUS_RST_STATE;
            dd->regs[DD_ASIC_CMD_STATUS] &= ~DD_STATUS_DISK_CHNG;
            break;

        /* Set Disk type */
        case 0x0b:
            DebugMessage(M64MSG_VERBOSE, "Setting disk type %u", (dd->regs[DD_ASIC_DATA] >> 16) & 0xf);
            break;

        /* Read RTC in ASIC_DATA (BCD format) */
        case 0x12:
            dd->regs[DD_ASIC_DATA] = time2data(tm->tm_year, tm->tm_mon + 1);
            break;
        case 0x13:
            dd->regs[DD_ASIC_DATA] = time2data(tm->tm_mday, tm->tm_hour);
            break;
        case 0x14:
            dd->regs[DD_ASIC_DATA] = time2data(tm->tm_min, tm->tm_sec);
            break;

        /* Feature inquiry */
        case 0x1b:
            dd->regs[DD_ASIC_DATA] = 0x00000000;
            break;

        default:
            DebugMessage(M64MSG_WARNING, "DD ASIC CMD not yet implemented (%08x)", value);
        }

        /* Signal a MECHA interrupt */
        signal_dd_interrupt(dd, DD_STATUS_MECHA_INT);
        break;

    case DD_ASIC_BM_STATUS_CTL:
        /* set sector */
        start_sector = (value >> 16) & 0xff;
        if (start_sector == 0x00) {
            dd->bm_block = 0;
            dd->regs[DD_ASIC_CUR_SECTOR] = 0;
        } else if (start_sector == 0x5a) {
            dd->bm_block = 1;
            dd->regs[DD_ASIC_CUR_SECTOR] = 0;
        }
        else {
            DebugMessage(M64MSG_ERROR, "Start sector not aligned");
        }

        /* clear MECHA interrupt */
        if (value & DD_BM_CTL_MECHA_RST) {
            dd->regs[DD_ASIC_CMD_STATUS] &= ~DD_STATUS_MECHA_INT;
        }
        /* start block transfer */
        if (value & DD_BM_CTL_BLK_TRANS) {
            dd->regs[DD_ASIC_BM_STATUS_CTL] |= DD_BM_STATUS_BLOCK;
        }
        /* handle reset */
        if (value & DD_BM_CTL_RESET) {
            dd->bm_reset_held = 1;
        }
        if (!(value & DD_BM_CTL_RESET) && dd->bm_reset_held) {
            dd->bm_reset_held = 0;
            dd->regs[DD_ASIC_CMD_STATUS] &= ~(DD_STATUS_DATA_RQ
                                            | DD_STATUS_C2_XFER
                                            | DD_STATUS_BM_ERR
                                            | DD_STATUS_BM_INT);
            dd->regs[DD_ASIC_BM_STATUS_CTL] = 0;
            dd->regs[DD_ASIC_CUR_SECTOR] = 0;
            dd->bm_block = 0;
        }

        /* clear DD interrupt if both MECHA and BM are cleared */
        if ((dd->regs[DD_ASIC_CMD_STATUS] & (DD_STATUS_BM_INT | DD_STATUS_MECHA_INT)) == 0) {
            clear_dd_interrupt(dd, DD_STATUS_BM_INT);
        }

        /* start transfer */
        if (value & DD_BM_CTL_START) {
            if (dd->bm_write && (value & DD_BM_CTL_MNGRMODE)) {
                DebugMessage(M64MSG_WARNING, "Attempt to write disk with BM mode 1");
            }
            if (!dd->bm_write && !(value & DD_BM_CTL_MNGRMODE)) {
                DebugMessage(M64MSG_WARNING, "Attempt to read disk with BM mode 0");
            }
            dd->regs[DD_ASIC_BM_STATUS_CTL] |= DD_BM_STATUS_RUNNING;
            dd_update_bm(dd);
        }
        break;

    case DD_ASIC_HARD_RESET:
        if (value != 0xaaaa0000) {
            DebugMessage(M64MSG_WARNING, "Unexpected hard reset value %08x", value);
        }
        dd->regs[DD_ASIC_CMD_STATUS] |= DD_STATUS_RST_STATE;
        break;

    case DD_ASIC_HOST_SECBYTE:
        dd->regs[DD_ASIC_HOST_SECBYTE] = (value >> 16) & 0xff;
        if ((dd->regs[DD_ASIC_HOST_SECBYTE] + 1) != zone_sec_size[dd->bm_zone]) {
            DebugMessage(M64MSG_WARNING, "Sector size %u set different than expected %u",
                    dd->regs[DD_ASIC_HOST_SECBYTE] + 1, zone_sec_size[dd->bm_zone]);
        }
        break;

    case DD_ASIC_SEC_BYTE:
        dd->regs[DD_ASIC_SEC_BYTE] = (value >> 24) & 0xff;
        if (dd->regs[DD_ASIC_SEC_BYTE] != SECTORS_PER_BLOCK + 4) {
            DebugMessage(M64MSG_WARNING, "Sectors per block %u set different than expected %u",
                    dd->regs[DD_ASIC_SEC_BYTE] + 1, SECTORS_PER_BLOCK + 4);
        }
        break;

    default:
        dd->regs[reg] = value;
    }
}


void read_dd_rom(void* opaque, uint32_t address, uint32_t* value)
{
    struct dd_controller* dd = (struct dd_controller*)opaque;
	uint32_t addr = dd_rom_address(address);

    *value = dd->rom[addr];

    DebugMessage(M64MSG_VERBOSE, "DD ROM: %08X -> %08x", address, *value);
}

void write_dd_rom(void* opaque, uint32_t address, uint32_t value, uint32_t mask)
{
    DebugMessage(M64MSG_VERBOSE, "DD ROM: %08X <- %08x & %08x", address, value, mask);
}

unsigned int dd_dom_dma_read(void* opaque, const uint8_t* dram, uint32_t dram_addr, uint32_t cart_addr, uint32_t length)
{
    struct dd_controller* dd = (struct dd_controller*)opaque;
    uint8_t* mem;
    size_t i;

    DebugMessage(M64MSG_VERBOSE, "DD DMA read dram=%08x  cart=%08x length=%08x",
            dram_addr, cart_addr, length);

    if (cart_addr == MM_DD_DS_BUFFER) {
        cart_addr = (cart_addr - MM_DD_DS_BUFFER) & 0x3fffff;
        mem = dd->ds_buf;
    }
    else {
        DebugMessage(M64MSG_ERROR, "Unknown DD dma read dram=%08x  cart=%08x length=%08x",
            dram_addr, cart_addr, length);
        return (length * 63) / 25;
    }

    for (i = 0; i < length; ++i) {
        mem[(cart_addr + i) ^ S8] = dram[(dram_addr + i) ^ S8];
    }

    return (length * 63) / 25;
}

unsigned int dd_dom_dma_write(void* opaque, uint8_t* dram, uint32_t dram_addr, uint32_t cart_addr, uint32_t length)
{
    struct dd_controller* dd = (struct dd_controller*)opaque;
    unsigned int cycles;
    const uint8_t* mem;
    size_t i;

    DebugMessage(M64MSG_VERBOSE, "DD DMA write dram=%08x  cart=%08x length=%08x",
            dram_addr, cart_addr, length);

    if (cart_addr < MM_DD_ROM) {
        if (cart_addr == MM_DD_C2S_BUFFER) {
            /* C2 sector buffer */
            cart_addr = (cart_addr - MM_DD_C2S_BUFFER);
            mem = (const uint8_t*)&dd->c2s_buf;
        }
        else if (cart_addr == MM_DD_DS_BUFFER) {
            /* Data sector buffer */
            cart_addr = (cart_addr - MM_DD_DS_BUFFER);
            mem = (const uint8_t*)&dd->ds_buf;
        }
        else {
            DebugMessage(M64MSG_ERROR, "Unknown DD dma write dram=%08x  cart=%08x length=%08x",
                dram_addr, cart_addr, length);

            return (length * 63) / 25;
        }

        cycles = (length * 63) / 25;
    }
    else {
        /* DD ROM */
        cart_addr = (cart_addr - MM_DD_ROM);
        mem = (const uint8_t*)dd->rom;
        cycles = (length * 63) / 25;
    }

    for (i = 0; i < length; ++i) {
        dram[(dram_addr + i) ^ S8] = mem[(cart_addr + i) ^ S8];
    }

    invalidate_r4300_cached_code(dd->r4300, R4300_KSEG0 + dram_addr, length);
    invalidate_r4300_cached_code(dd->r4300, R4300_KSEG1 + dram_addr, length);

    return cycles;
}

void dd_on_pi_cart_addr_write(struct dd_controller* dd, uint32_t address)
{
    /* clear C2 xfer */
    if (address == MM_DD_C2S_BUFFER) {
        dd->regs[DD_ASIC_CMD_STATUS] &= ~(DD_STATUS_C2_XFER | DD_STATUS_BM_ERR);
        clear_dd_interrupt(dd, DD_STATUS_BM_INT);
    }
    /* clear data RQ */
    else if (address == MM_DD_DS_BUFFER) {
        dd->regs[DD_ASIC_CMD_STATUS] &= ~(DD_STATUS_DATA_RQ | DD_STATUS_BM_ERR);
        clear_dd_interrupt(dd, DD_STATUS_BM_INT);
    }
}


/* Disk conversion routines */
void dd_convert_to_mame(unsigned char* mame_disk, const unsigned char* sdk_disk)
{
    /* Original code by Happy_ */
    uint8_t system_data[DD_DISK_SYSTEM_DATA_SIZE];
    uint8_t block_data[2][0x100 * SECTORS_PER_BLOCK];

    uint32_t disktype = 0;
    uint32_t zone, track = 0;
    int32_t atrack = 0;
    int32_t block = 0;
    uint32_t InOffset, OutOffset = 0;
    uint32_t InStart[16];
    uint32_t OutStart[16];

    int cur_offset = 0;


    /* Read System Area */
    memcpy(system_data, sdk_disk, DD_DISK_SYSTEM_DATA_SIZE);
    disktype = system_data[5] & 0xf;

    /* Prepare Input Offsets */
    InStart[0] = 0;
    for (zone = 1; zone < 16; ++zone) {
        InStart[zone] = InStart[zone - 1] + VZONESIZE(DiskTypeZones[disktype][zone - 1]);
    }

    /* Prepare Output Offsets */
    OutStart[0] = 0;
    for (zone = 1; zone < 16; ++zone) {
        OutStart[zone] = OutStart[zone - 1] + ZONESIZE(zone - 1);
    }

    /* Copy Head 0 */
    for (zone = 0; zone < 8; zone++)
    {
        OutOffset = OutStart[zone];
        InOffset = InStart[RevDiskTypeZones[disktype][zone]];
        cur_offset = InOffset;

        block = StartBlock[disktype][zone];
        atrack = 0;
        for (track = 0; track < ZoneTracks[zone]; track++)
        {
            if (atrack < 0xC && track == system_data[0x20 + zone * 0xC + atrack])
            {
                memset((void *)(&block_data[0]), 0, BLOCKSIZE(zone));
                memset((void *)(&block_data[1]), 0, BLOCKSIZE(zone));
                atrack += 1;
            }
            else
            {
                if ((block % 2) == 1)
                {
                    memcpy(block_data[1], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                    memcpy(block_data[0], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                }
                else
                {
                    memcpy(block_data[0], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                    memcpy(block_data[1], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                }
                block = 1 - block;
            }
            memcpy(mame_disk + OutOffset, &block_data[0], BLOCKSIZE(zone));
            OutOffset += BLOCKSIZE(zone);
            memcpy(mame_disk + OutOffset, &block_data[1], BLOCKSIZE(zone));
            OutOffset += BLOCKSIZE(zone);
        }
    }

    /* Copy Head 1 */
    for (zone = 8; zone < 16; zone++)
    {
        InOffset = InStart[RevDiskTypeZones[disktype][zone]];
        cur_offset = InOffset;

        block = StartBlock[disktype][zone];
        atrack = 0xB;
        for (track = 1; track < ZoneTracks[zone] + 1; track++)
        {
            if (atrack > -1 && (ZoneTracks[zone] - track) == system_data[0x20 + (zone)* 0xC + atrack])
            {
                memset((void *)(&block_data[0]), 0, BLOCKSIZE(zone));
                memset((void *)(&block_data[1]), 0, BLOCKSIZE(zone));
                atrack -= 1;
            }
            else
            {
                if ((block % 2) == 1)
                {
                    memcpy(block_data[1], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                    memcpy(block_data[0], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                }
                else
                {
                    memcpy(block_data[0], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                    memcpy(block_data[1], sdk_disk + cur_offset, BLOCKSIZE(zone));
                    cur_offset += BLOCKSIZE(zone);
                }
                block = 1 - block;
            }
            OutOffset = OutStart[zone] + (ZoneTracks[zone] - track) * TRACKSIZE(zone);
            memcpy(mame_disk + OutOffset, &block_data[0], BLOCKSIZE(zone));
            OutOffset += BLOCKSIZE(zone);
            memcpy(mame_disk + OutOffset, &block_data[1], BLOCKSIZE(zone));
            OutOffset += BLOCKSIZE(zone);
        }
    }
}

void dd_convert_to_sdk(const unsigned char* mame_disk, unsigned char* sdk_disk)
{
    /* Original code by Happy_ */
    uint8_t system_data[DD_DISK_SYSTEM_DATA_SIZE];
    uint8_t block_data[2][0x100 * SECTORS_PER_BLOCK];

    uint32_t disktype = 0;
    uint32_t zone, track = 0;
    int32_t atrack = 0;
    int32_t block = 0;
    uint32_t InOffset, OutOffset = 0;
    uint32_t InStart[16];
    uint32_t OutStart[16];


    /* Read System Area */
    memcpy(system_data, mame_disk, DD_DISK_SYSTEM_DATA_SIZE);
    disktype = system_data[5] & 0xf;

    /* Prepare Input Offsets */
    InStart[0] = 0;
    for (zone = 1; zone < 16; ++zone) {
        InStart[zone] = InStart[zone - 1] + VZONESIZE(DiskTypeZones[disktype][zone - 1]);
    }

    /* Prepare Output Offsets */
    OutStart[0] = 0;
    for (zone = 1; zone < 16; ++zone) {
        OutStart[zone] = OutStart[zone - 1] + ZONESIZE(zone - 1);
    }

    /* Copy Head 0 */
    for (zone = 0; zone < 8; zone++)
    {
        block = StartBlock[disktype][zone];
        atrack = 0;
        for (track = 0; track < ZoneTracks[zone]; track++)
        {
            InOffset = OutStart[zone] + (track)* TRACKSIZE(zone);
            OutOffset = InStart[RevDiskTypeZones[disktype][zone]] + (track - atrack) * TRACKSIZE(zone);

            if (atrack < 0xC && track == system_data[0x20 + zone * 0xC + atrack])
            {
                atrack += 1;
            }
            else
            {
                if ((block % 2) == 1)
                {
                    memcpy(&block_data[1], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                    memcpy(&block_data[0], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                }
                else
                {
                    memcpy(&block_data[0], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                    memcpy(&block_data[1], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                }
                block = 1 - block;
                memcpy(sdk_disk + OutOffset, &block_data[0], BLOCKSIZE(zone));
                OutOffset += BLOCKSIZE(zone);
                memcpy(sdk_disk + OutOffset, &block_data[1], BLOCKSIZE(zone));
                OutOffset += BLOCKSIZE(zone);
            }
        }
    }

    /* Copy Head 1 */
    for (zone = 8; zone < 16; zone++)
    {
        block = StartBlock[disktype][zone];
        atrack = 0xB;
        for (track = 1; track < ZoneTracks[zone] + 1; track++)
        {
            InOffset = OutStart[zone] + (ZoneTracks[zone] - track) * TRACKSIZE(zone);
            OutOffset = InStart[RevDiskTypeZones[disktype][zone]] + (track - (0xB - atrack) - 1) * TRACKSIZE(zone);

            if (atrack > -1 && (ZoneTracks[zone] - track) == system_data[0x20 + (zone)* 0xC + atrack])
            {
                atrack -= 1;
            }
            else
            {
                if ((block % 2) == 1)
                {
                    memcpy(&block_data[1], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                    memcpy(&block_data[0], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                }
                else
                {
                    memcpy(&block_data[0], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                    memcpy(&block_data[1], mame_disk + InOffset, BLOCKSIZE(zone));
                    InOffset += BLOCKSIZE(zone);
                }
                block = 1 - block;
                memcpy(sdk_disk + OutOffset, &block_data[0], BLOCKSIZE(zone));
                OutOffset += BLOCKSIZE(zone);
                memcpy(sdk_disk + OutOffset, &block_data[1], BLOCKSIZE(zone));
                OutOffset += BLOCKSIZE(zone);
            }
        }
    }
}
