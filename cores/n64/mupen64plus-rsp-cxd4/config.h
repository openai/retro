/******************************************************************************\
* Authors:  Iconoclast                                                         *
* Release:  2013.12.04                                                         *
* License:  CC0 Public Domain Dedication                                       *
*                                                                              *
* To the extent possible under law, the author(s) have dedicated all copyright *
* and related and neighboring rights to this software to the public domain     *
* worldwide. This software is distributed without any warranty.                *
*                                                                              *
* You should have received a copy of the CC0 Public Domain Dedication along    *
* with this software.                                                          *
* If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.             *
\******************************************************************************/
#ifndef _CXD4_CONFIG_H
#define _CXD4_CONFIG_H

extern unsigned char rsp_conf[32];

#define MINIMUM_MESSAGE_PRIORITY    1
#define EXTERN_COMMAND_LIST_GBI
#define EXTERN_COMMAND_LIST_ABI
#define SEMAPHORE_LOCK_CORRECTIONS
#define WAIT_FOR_CPU_HOST
#define EMULATE_STATIC_PC

/*
 * The config file used to be a 32-byte EEPROM with binary settings storage.
 * It was found necessary for user and contributor convenience to replace.
 *
 * The current configuration system now uses Garteal's CFG text definitions.
 */

#define CFG_HLE_GFX     (rsp_conf[0x00])
#define CFG_HLE_AUD     (rsp_conf[0x01])
#define CFG_HLE_VID     (rsp_conf[0x02]) /* reserved/unused */
#define CFG_HLE_JPG     (rsp_conf[0x03]) /* unused */
/*
 * Most of the point behind this config system is to let users use HLE video
 * or audio plug-ins.  The other task types are used less than 1% of the time
 * and only in a few games.  They require simulation from within the RSP
 * internally, which I have no intention to ever support.  Some good research
 * on a few of these special task types was done by Hacktarux in the MUPEN64
 * HLE RSP plug-in, so consider using that instead for complete HLE.
 */

/*
 * Schedule binary dump exports to the DllConfig schedule delay queue.
 */
#define CFG_QUEUE_E_DRAM    (*(int *)(rsp_conf + 0x04))
#define CFG_QUEUE_E_DMEM    (*(int *)(rsp_conf + 0x08))
#define CFG_QUEUE_E_IMEM    (*(int *)(rsp_conf + 0x0C))
/*
 * Note:  This never actually made it into the configuration system.
 * Instead, DMEM and IMEM are always exported on every call to DllConfig().
 */

/*
 * Special switches.
 * (generally for correcting RSP clock behavior on Project64 2.x)
 * Also includes RSP register states debugger.
 */
#define CFG_WAIT_FOR_CPU_HOST       (*(int *)(rsp_conf + 0x10))
#define CFG_MEND_SEMAPHORE_LOCK     (*(int *)(rsp_conf + 0x14))
#define CFG_TRACE_RSP_REGISTERS     (*(int *)(rsp_conf + 0x18))

#endif
