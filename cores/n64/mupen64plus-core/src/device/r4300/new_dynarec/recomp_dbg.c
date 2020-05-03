/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - recomp_dbg.c                                            *
 *   Copyright (C) 2009-2018 Gillou68310                                   *
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

#define RECOMP_DBG

#if RECOMPILER_DEBUG == 1 //x86
#if NEW_DYNAREC != 1 //x86
#error x86 dynarec debug only available on x86
#endif
#elif RECOMPILER_DEBUG == 2 //x64
#if NEW_DYNAREC != 2 //x64
#error x64 dynarec debug only available on x64
#endif
#elif RECOMPILER_DEBUG == 3 //ARM
#if NEW_DYNAREC != 1 //x86
#error arm dynarec debug only available on x86
#endif
#elif RECOMPILER_DEBUG == 4 //ARM64
#if NEW_DYNAREC != 2 //x64
#error arm64 dynarec debug only available on x64
#endif
#endif

#undef NEW_DYNAREC
#define NEW_DYNAREC RECOMPILER_DEBUG

/* Rename non-static variables */
#define base_addr                               recomp_dbg_base_addr
#define base_addr_rx                            recomp_dbg_base_addr_rx
#define out                                     recomp_dbg_out
#define using_tlb                               recomp_dbg_using_tlb
#define stop_after_jal                          recomp_dbg_stop_after_jal

/* Rename non-static functions */
#define verify_dirty                            recomp_dbg_verify_dirty
#define clean_blocks                            recomp_dbg_clean_blocks
#define get_addr                                recomp_dbg_get_addr
#define get_addr_32                             recomp_dbg_get_addr_32
#define get_addr_ht                             recomp_dbg_get_addr_ht
#define invalidate_all_pages                    recomp_dbg_invalidate_all_pages
#define invalidate_block                        recomp_dbg_invalidate_block
#define invalidate_cached_code_new_dynarec      recomp_dbg_invalidate_cached_code_new_dynarec
#define new_dynarec_cleanup                     recomp_dbg_new_dynarec_cleanup
#define new_dynarec_init                        recomp_dbg_new_dynarec_init
#define new_recompile_block                     recomp_dbg_new_recompile_block
#define new_dynarec_check_interrupt             recomp_dbg_new_dynarec_check_interrupt
#define dynarec_gen_interrupt                   recomp_dbg_dynarec_gen_interrupt

#if RECOMPILER_DEBUG == 3 //ARM
static void jump_vaddr_r0(void){}
static void jump_vaddr_r1(void){}
static void jump_vaddr_r2(void){}
static void jump_vaddr_r3(void){}
static void jump_vaddr_r4(void){}
static void jump_vaddr_r5(void){}
static void jump_vaddr_r6(void){}
static void jump_vaddr_r7(void){}
static void jump_vaddr_r8(void){}
static void jump_vaddr_r9(void){}
static void jump_vaddr_r10(void){}
static void jump_vaddr_r12(void){}
static void invalidate_addr_r0(void){}
static void invalidate_addr_r1(void){}
static void invalidate_addr_r2(void){}
static void invalidate_addr_r3(void){}
static void invalidate_addr_r4(void){}
static void invalidate_addr_r5(void){}
static void invalidate_addr_r6(void){}
static void invalidate_addr_r7(void){}
static void invalidate_addr_r8(void){}
static void invalidate_addr_r9(void){}
static void invalidate_addr_r10(void){}
static void invalidate_addr_r12(void){}
static void __clear_cache(char* begin, char *end){}

#include "arm/arm_cpu_features.h"
/* arm_cpu_features.c */
static arm_cpu_features_t arm_cpu_features;
static void detect_arm_cpu_features(void){}
static void print_arm_cpu_features(void){}
#elif RECOMPILER_DEBUG == 4 //ARM64
void jump_vaddr_x0(void){}
void jump_vaddr_x1(void){}
void jump_vaddr_x2(void){}
void jump_vaddr_x3(void){}
void jump_vaddr_x4(void){}
void jump_vaddr_x5(void){}
void jump_vaddr_x6(void){}
void jump_vaddr_x7(void){}
void jump_vaddr_x8(void){}
void jump_vaddr_x9(void){}
void jump_vaddr_x10(void){}
void jump_vaddr_x11(void){}
void jump_vaddr_x12(void){}
void jump_vaddr_x13(void){}
void jump_vaddr_x14(void){}
void jump_vaddr_x15(void){}
void jump_vaddr_x16(void){}
void jump_vaddr_x17(void){}
void jump_vaddr_x18(void){}
void jump_vaddr_x19(void){}
void jump_vaddr_x20(void){}
void jump_vaddr_x21(void){}
void jump_vaddr_x22(void){}
void jump_vaddr_x23(void){}
void jump_vaddr_x24(void){}
void jump_vaddr_x25(void){}
void jump_vaddr_x26(void){}
void jump_vaddr_x27(void){}
void jump_vaddr_x28(void){}
static void __clear_cache(char* begin, char *end){}
#endif

#include "osal/preproc.h" //for ALIGN
ALIGN(4096, static char recomp_dbg_extra_memory[33554432]);

// Recompile new_dynarec.c with the above redefinitions
#include "new_dynarec.c"

#include <inttypes.h>
#include <capstone.h>
#if RECOMPILER_DEBUG == NEW_DYNAREC_X86
    #define ARCHITECTURE CS_ARCH_X86
    #define MODE CS_MODE_32
    #define INSTRUCTION instr[i].detail->x86
    #define CALL_INST 0x38
    #define BRANCH_INST 0x10a
    #define ARCH_NAME "x86"
#elif RECOMPILER_DEBUG == NEW_DYNAREC_X64
    #define ARCHITECTURE CS_ARCH_X86
    #define MODE CS_MODE_64
    #define INSTRUCTION instr[i].detail->x86
    #define CALL_INST 0x38
    #define BRANCH_INST 0x10a
    #define ARCH_NAME "x64"
#elif RECOMPILER_DEBUG == NEW_DYNAREC_ARM
    #define ARCHITECTURE CS_ARCH_ARM
    #define MODE CS_MODE_LITTLE_ENDIAN
    #define INSTRUCTION instr[i].detail->arm
    #define FP_REGISTER 0x4d
    #define CALL_INST 0xd
    #define BRANCH_INST 0x11
    #define ARCH_NAME "ARM"
#elif RECOMPILER_DEBUG == NEW_DYNAREC_ARM64
    #define ARCHITECTURE CS_ARCH_ARM64
    #define MODE CS_MODE_LITTLE_ENDIAN
    #define INSTRUCTION instr[i].detail->arm64
    #define FP_REGISTER 0x1
    #define CALL_INST 0x15
    #define BRANCH_INST 0x10
    #define ARCH_NAME "ARM64"
#endif

typedef struct{
  intptr_t addr;
  int32_t size;
  char * name;
}Variable_t;

static Variable_t var[] = {
  {(intptr_t)NULL /*RDRAM*/, 0, "rdram - 0x80000000"},
  {(intptr_t)g_dev.r4300.cached_interp.invalid_code, sizeof(g_dev.r4300.cached_interp.invalid_code), "invalid_code"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.dynarec_local, sizeof(g_dev.r4300.new_dynarec_hot_state.dynarec_local), "dynarec_local"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.next_interrupt, sizeof(g_dev.r4300.new_dynarec_hot_state.next_interrupt), "next_interrupt"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cycle_count, sizeof(g_dev.r4300.new_dynarec_hot_state.cycle_count), "cycle_count"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.pending_exception, sizeof(g_dev.r4300.new_dynarec_hot_state.pending_exception), "pending_exception"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.pcaddr, sizeof(g_dev.r4300.new_dynarec_hot_state.pcaddr), "pcaddr"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.stop, sizeof(g_dev.r4300.new_dynarec_hot_state.stop), "r4300_stop"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.invc_ptr, sizeof(g_dev.r4300.new_dynarec_hot_state.invc_ptr), "invc_ptr"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.address, sizeof(g_dev.r4300.new_dynarec_hot_state.address), "mem_address"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.rdword, sizeof(g_dev.r4300.new_dynarec_hot_state.rdword), "mem_rdword"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.wdword, sizeof(g_dev.r4300.new_dynarec_hot_state.wdword), "mem_wdword"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.wword, sizeof(g_dev.r4300.new_dynarec_hot_state.wword), "mem_wword"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fcr0, sizeof(g_dev.r4300.new_dynarec_hot_state.fcr0), "cp1_fcr0"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fcr31, sizeof(g_dev.r4300.new_dynarec_hot_state.fcr31), "cp1_fcr31"},
  //{(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs, sizeof(g_dev.r4300.new_dynarec_hot_state.regs), "r4300_regs"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[0], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[0]), "r4300_regs[0]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[1], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[1]), "r4300_regs[1]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[2], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[2]), "r4300_regs[2]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[3], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[3]), "r4300_regs[3]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[4], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[4]), "r4300_regs[4]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[5], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[5]), "r4300_regs[5]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[6], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[6]), "r4300_regs[6]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[7], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[7]), "r4300_regs[7]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[8], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[8]), "r4300_regs[8]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[9], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[9]), "r4300_regs[9]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[10], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[10]), "r4300_regs[10]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[11], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[11]), "r4300_regs[11]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[12], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[12]), "r4300_regs[12]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[13], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[13]), "r4300_regs[13]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[14], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[14]), "r4300_regs[14]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[15], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[15]), "r4300_regs[15]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[16], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[16]), "r4300_regs[16]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[17], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[17]), "r4300_regs[17]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[18], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[18]), "r4300_regs[18]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[19], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[19]), "r4300_regs[19]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[20], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[20]), "r4300_regs[20]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[21], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[21]), "r4300_regs[21]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[22], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[22]), "r4300_regs[22]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[23], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[23]), "r4300_regs[23]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[24], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[24]), "r4300_regs[24]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[25], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[25]), "r4300_regs[25]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[26], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[26]), "r4300_regs[26]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[27], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[27]), "r4300_regs[27]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[28], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[28]), "r4300_regs[28]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[29], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[29]), "r4300_regs[29]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[30], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[30]), "r4300_regs[30]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.regs[31], sizeof(g_dev.r4300.new_dynarec_hot_state.regs[31]), "r4300_regs[31]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.hi, sizeof(g_dev.r4300.new_dynarec_hot_state.hi), "r4300_hi"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.lo, sizeof(g_dev.r4300.new_dynarec_hot_state.lo), "r4300_lo"},
  //{(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs, sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs), "cp0_regs"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[0], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[0]), "cp0_regs[0]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[1], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[1]), "cp0_regs[1]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[2], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[2]), "cp0_regs[2]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[3], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[3]), "cp0_regs[3]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[4], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[4]), "cp0_regs[4]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[5], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[5]), "cp0_regs[5]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[6], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[6]), "cp0_regs[6]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[7], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[7]), "cp0_regs[7]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[8], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[8]), "cp0_regs[8]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[9], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[9]), "cp0_regs[9]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[10], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[10]), "cp0_regs[10]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[11], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[11]), "cp0_regs[11]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[12], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[12]), "cp0_regs[12]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[13], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[13]), "cp0_regs[13]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[14], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[14]), "cp0_regs[14]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[15], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[15]), "cp0_regs[15]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[16], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[16]), "cp0_regs[16]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[17], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[17]), "cp0_regs[17]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[18], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[18]), "cp0_regs[18]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[19], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[19]), "cp0_regs[19]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[20], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[20]), "cp0_regs[20]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[21], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[21]), "cp0_regs[21]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[22], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[22]), "cp0_regs[22]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[23], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[23]), "cp0_regs[23]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[24], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[24]), "cp0_regs[24]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[25], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[25]), "cp0_regs[25]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[26], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[26]), "cp0_regs[26]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[27], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[27]), "cp0_regs[27]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[28], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[28]), "cp0_regs[28]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[29], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[29]), "cp0_regs[29]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[30], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[30]), "cp0_regs[30]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp0_regs[31], sizeof(g_dev.r4300.new_dynarec_hot_state.cp0_regs[31]), "cp0_regs[31]"},
  //{(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple, sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple), "cp1_regs_simple"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[0], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[0]), "cp1_regs_simple[0]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[1], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[1]), "cp1_regs_simple[1]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[2], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[2]), "cp1_regs_simple[2]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[3], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[3]), "cp1_regs_simple[3]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[4], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[4]), "cp1_regs_simple[4]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[5], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[5]), "cp1_regs_simple[5]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[6], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[6]), "cp1_regs_simple[6]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[7], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[7]), "cp1_regs_simple[7]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[8], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[8]), "cp1_regs_simple[8]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[9], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[9]), "cp1_regs_simple[9]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[10], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[10]), "cp1_regs_simple[10]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[11], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[11]), "cp1_regs_simple[11]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[12], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[12]), "cp1_regs_simple[12]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[13], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[13]), "cp1_regs_simple[13]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[14], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[14]), "cp1_regs_simple[14]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[15], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[15]), "cp1_regs_simple[15]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[16], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[16]), "cp1_regs_simple[16]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[17], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[17]), "cp1_regs_simple[17]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[18], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[18]), "cp1_regs_simple[18]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[19], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[19]), "cp1_regs_simple[19]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[20], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[20]), "cp1_regs_simple[20]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[21], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[21]), "cp1_regs_simple[21]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[22], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[22]), "cp1_regs_simple[22]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[23], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[23]), "cp1_regs_simple[23]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[24], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[24]), "cp1_regs_simple[24]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[25], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[25]), "cp1_regs_simple[25]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[26], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[26]), "cp1_regs_simple[26]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[27], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[27]), "cp1_regs_simple[27]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[28], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[28]), "cp1_regs_simple[28]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[29], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[29]), "cp1_regs_simple[29]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[30], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[30]), "cp1_regs_simple[30]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[31], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_simple[31]), "cp1_regs_simple[31]"},
  //{(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double, sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double), "cp1_regs_double"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[0], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[0]), "cp1_regs_double[0]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[1], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[1]), "cp1_regs_double[1]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[2], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[2]), "cp1_regs_double[2]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[3], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[3]), "cp1_regs_double[3]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[4], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[4]), "cp1_regs_double[4]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[5], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[5]), "cp1_regs_double[5]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[6], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[6]), "cp1_regs_double[6]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[7], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[7]), "cp1_regs_double[7]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[8], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[8]), "cp1_regs_double[8]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[9], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[9]), "cp1_regs_double[9]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[10], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[10]), "cp1_regs_double[10]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[11], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[11]), "cp1_regs_double[11]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[12], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[12]), "cp1_regs_double[12]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[13], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[13]), "cp1_regs_double[13]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[14], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[14]), "cp1_regs_double[14]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[15], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[15]), "cp1_regs_double[15]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[16], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[16]), "cp1_regs_double[16]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[17], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[17]), "cp1_regs_double[17]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[18], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[18]), "cp1_regs_double[18]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[19], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[19]), "cp1_regs_double[19]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[20], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[20]), "cp1_regs_double[20]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[21], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[21]), "cp1_regs_double[21]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[22], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[22]), "cp1_regs_double[22]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[23], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[23]), "cp1_regs_double[23]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[24], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[24]), "cp1_regs_double[24]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[25], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[25]), "cp1_regs_double[25]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[26], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[26]), "cp1_regs_double[26]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[27], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[27]), "cp1_regs_double[27]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[28], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[28]), "cp1_regs_double[28]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[29], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[29]), "cp1_regs_double[29]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[30], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[30]), "cp1_regs_double[30]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[31], sizeof(g_dev.r4300.new_dynarec_hot_state.cp1_regs_double[31]), "cp1_regs_double[31]"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.rounding_modes, sizeof(g_dev.r4300.new_dynarec_hot_state.rounding_modes), "rounding_modes"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.branch_target, sizeof(g_dev.r4300.new_dynarec_hot_state.branch_target), "branch_target"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.pc, sizeof(g_dev.r4300.new_dynarec_hot_state.pc), "r4300_pc"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc, /*sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc)*/4, "fake_pc"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.rs, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.rs), "fake_pc.f.r.rs"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.rt, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.rt), "fake_pc.f.r.rt"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.rd, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.rd), "fake_pc.f.r.rd"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.sa, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.sa), "fake_pc.f.r.sa"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.nrd, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.r.nrd), "fake_pc.f.r.nrd"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.i.rs, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.i.rs), "fake_pc.f.i.rs"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.i.rt, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.i.rt), "fake_pc.f.i.rt"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.fake_pc.f.i.immediate, sizeof(g_dev.r4300.new_dynarec_hot_state.fake_pc.f.i.immediate), "fake_pc.f.i.immediate"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.rs, sizeof(g_dev.r4300.new_dynarec_hot_state.rs), "rs"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.rt, sizeof(g_dev.r4300.new_dynarec_hot_state.rt), "rt"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.rd, sizeof(g_dev.r4300.new_dynarec_hot_state.rd), "rd"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.ram_offset, sizeof(g_dev.r4300.new_dynarec_hot_state.ram_offset), "ram_offset"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.mini_ht, sizeof(g_dev.r4300.new_dynarec_hot_state.mini_ht), "mini_ht"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.restore_candidate, sizeof(g_dev.r4300.new_dynarec_hot_state.restore_candidate), "restore_candidate"},
  {(intptr_t)&g_dev.r4300.new_dynarec_hot_state.memory_map, sizeof(g_dev.r4300.new_dynarec_hot_state.memory_map), "memory_map"},
  {-1, -1, NULL}
};

typedef struct{
  intptr_t addr;
  char * name;
}Function_t;

static Function_t func[] = {
  {(intptr_t)MFC0_new, "MFC0"},
  {(intptr_t)MTC0_new, "MTC0"},
  {(intptr_t)cached_interp_TLBR, "TLBR"},
  {(intptr_t)cached_interp_TLBP, "TLBP"},
  {(intptr_t)cached_interp_DMULT, "DMULT"},
  {(intptr_t)cached_interp_DMULTU, "DMULTU"},
  {(intptr_t)cached_interp_DDIV, "DDIV"},
  {(intptr_t)cached_interp_DDIVU, "DDIVU"},
#if RECOMPILER_DEBUG == NEW_DYNAREC_X86 || RECOMPILER_DEBUG == NEW_DYNAREC_X64
  {(intptr_t)jump_vaddr_eax, "jump_vaddr_eax"},
  {(intptr_t)jump_vaddr_ecx, "jump_vaddr_ecx"},
  {(intptr_t)jump_vaddr_edx, "jump_vaddr_edx"},
  {(intptr_t)jump_vaddr_ebx, "jump_vaddr_ebx"},
  {(intptr_t)jump_vaddr_ebp, "jump_vaddr_ebp"},
  {(intptr_t)jump_vaddr_edi, "jump_vaddr_edi"},
  {(intptr_t)invalidate_block_eax, "invalidate_block_eax"},
  {(intptr_t)invalidate_block_ecx, "invalidate_block_ecx"},
  {(intptr_t)invalidate_block_edx, "invalidate_block_edx"},
  {(intptr_t)invalidate_block_ebx, "invalidate_block_ebx"},
  {(intptr_t)invalidate_block_ebp, "invalidate_block_ebp"},
  {(intptr_t)invalidate_block_esi, "invalidate_block_esi"},
  {(intptr_t)invalidate_block_edi, "invalidate_block_edi"},
#elif RECOMPILER_DEBUG == NEW_DYNAREC_ARM
  {(intptr_t)invalidate_addr, "invalidate_addr"},
  {(intptr_t)jump_vaddr_r0, "jump_vaddr_r0"},
  {(intptr_t)jump_vaddr_r1, "jump_vaddr_r1"},
  {(intptr_t)jump_vaddr_r2, "jump_vaddr_r2"},
  {(intptr_t)jump_vaddr_r3, "jump_vaddr_r3"},
  {(intptr_t)jump_vaddr_r4, "jump_vaddr_r4"},
  {(intptr_t)jump_vaddr_r5, "jump_vaddr_r5"},
  {(intptr_t)jump_vaddr_r6, "jump_vaddr_r6"},
  {(intptr_t)jump_vaddr_r7, "jump_vaddr_r7"},
  {(intptr_t)jump_vaddr_r8, "jump_vaddr_r8"},
  {(intptr_t)jump_vaddr_r9, "jump_vaddr_r9"},
  {(intptr_t)jump_vaddr_r10, "jump_vaddr_r10"},
  {(intptr_t)jump_vaddr_r12, "jump_vaddr_r12"},
  {(intptr_t)invalidate_addr_r0," invalidate_addr_r0"},
  {(intptr_t)invalidate_addr_r1," invalidate_addr_r1"},
  {(intptr_t)invalidate_addr_r2," invalidate_addr_r2"},
  {(intptr_t)invalidate_addr_r3," invalidate_addr_r3"},
  {(intptr_t)invalidate_addr_r4," invalidate_addr_r4"},
  {(intptr_t)invalidate_addr_r5," invalidate_addr_r5"},
  {(intptr_t)invalidate_addr_r6," invalidate_addr_r6"},
  {(intptr_t)invalidate_addr_r7," invalidate_addr_r7"},
  {(intptr_t)invalidate_addr_r8," invalidate_addr_r8"},
  {(intptr_t)invalidate_addr_r9," invalidate_addr_r9"},
  {(intptr_t)invalidate_addr_r10," invalidate_addr_r10"},
  {(intptr_t)invalidate_addr_r12," invalidate_addr_r12"},
#else //ARM64
  {(intptr_t)invalidate_addr, "invalidate_addr"},
  {(intptr_t)jump_vaddr_x0, "jump_vaddr_x0"},
  {(intptr_t)jump_vaddr_x1, "jump_vaddr_x1"},
  {(intptr_t)jump_vaddr_x2, "jump_vaddr_x2"},
  {(intptr_t)jump_vaddr_x3, "jump_vaddr_x3"},
  {(intptr_t)jump_vaddr_x4, "jump_vaddr_x4"},
  {(intptr_t)jump_vaddr_x5, "jump_vaddr_x5"},
  {(intptr_t)jump_vaddr_x6, "jump_vaddr_x6"},
  {(intptr_t)jump_vaddr_x7, "jump_vaddr_x7"},
  {(intptr_t)jump_vaddr_x8, "jump_vaddr_x8"},
  {(intptr_t)jump_vaddr_x9, "jump_vaddr_x9"},
  {(intptr_t)jump_vaddr_x10, "jump_vaddr_x10"},
  {(intptr_t)jump_vaddr_x11, "jump_vaddr_x11"},
  {(intptr_t)jump_vaddr_x12, "jump_vaddr_x12"},
  {(intptr_t)jump_vaddr_x13, "jump_vaddr_x13"},
  {(intptr_t)jump_vaddr_x14, "jump_vaddr_x14"},
  {(intptr_t)jump_vaddr_x15, "jump_vaddr_x15"},
  {(intptr_t)jump_vaddr_x16, "jump_vaddr_x16"},
  {(intptr_t)jump_vaddr_x17, "jump_vaddr_x17"},
  {(intptr_t)jump_vaddr_x18, "jump_vaddr_x18"},
  {(intptr_t)jump_vaddr_x19, "jump_vaddr_x19"},
  {(intptr_t)jump_vaddr_x20, "jump_vaddr_x20"},
  {(intptr_t)jump_vaddr_x21, "jump_vaddr_x21"},
  {(intptr_t)jump_vaddr_x22, "jump_vaddr_x22"},
  {(intptr_t)jump_vaddr_x23, "jump_vaddr_x23"},
  {(intptr_t)jump_vaddr_x24, "jump_vaddr_x24"},
  {(intptr_t)jump_vaddr_x25, "jump_vaddr_x25"},
  {(intptr_t)jump_vaddr_x26, "jump_vaddr_x26"},
  {(intptr_t)jump_vaddr_x27, "jump_vaddr_x27"},
  {(intptr_t)jump_vaddr_x28, "jump_vaddr_x28"},
#endif
  {(intptr_t)dyna_linker, "dyna_linker"},
  {(intptr_t)dyna_linker_ds, "dyna_linker_ds"},
  {(intptr_t)TLBWI_new, "TLBWI_new"},
  {(intptr_t)TLBWR_new, "TLBWR_new"},
  {(intptr_t)verify_code, "verify_code"},
  {(intptr_t)cc_interrupt, "cc_interrupt"},
  {(intptr_t)fp_exception, "fp_exception"},
  {(intptr_t)fp_exception_ds, "fp_exception_ds"},
  {(intptr_t)jump_syscall, "jump_syscall"},
  {(intptr_t)jump_eret, "jump_eret"},
  {(intptr_t)do_interrupt, "do_interrupt"},
  {(intptr_t)read_byte_new, "read_byte_new "},
  {(intptr_t)read_hword_new, "read_hword_new"},
  {(intptr_t)read_word_new, "read_word_new"},
  {(intptr_t)read_dword_new, "read_dword_new"},
  {(intptr_t)write_byte_new, "write_byte_new"},
  {(intptr_t)write_hword_new, "write_hword_new"},
  {(intptr_t)write_word_new, "write_word_new"},
  {(intptr_t)write_dword_new, "write_dword_new"},
  {(intptr_t)cvt_s_w, "cvt_s_w"},
  {(intptr_t)cvt_d_w, "cvt_d_w"},
  {(intptr_t)cvt_s_l, "cvt_s_l"},
  {(intptr_t)cvt_d_l, "cvt_d_l"},
  {(intptr_t)cvt_w_s, "cvt_w_s"},
  {(intptr_t)cvt_w_d, "cvt_w_d"},
  {(intptr_t)cvt_l_s, "cvt_l_s"},
  {(intptr_t)cvt_l_d, "cvt_l_d"},
  {(intptr_t)cvt_d_s, "cvt_d_s"},
  {(intptr_t)cvt_s_d, "cvt_s_d"},
  {(intptr_t)round_l_s, "round_l_s"},
  {(intptr_t)round_w_s, "round_w_s"},
  {(intptr_t)trunc_l_s, "trunc_l_s"},
  {(intptr_t)trunc_w_s, "trunc_w_s"},
  {(intptr_t)ceil_l_s, "ceil_l_s"},
  {(intptr_t)ceil_w_s, "ceil_w_s"},
  {(intptr_t)floor_l_s, "floor_l_s"},
  {(intptr_t)floor_w_s, "floor_w_s"},
  {(intptr_t)round_l_d, "round_l_d"},
  {(intptr_t)round_w_d, "round_w_d"},
  {(intptr_t)trunc_l_d, "trunc_l_d"},
  {(intptr_t)trunc_w_d, "trunc_w_d"},
  {(intptr_t)ceil_l_d, "ceil_l_d"},
  {(intptr_t)ceil_w_d, "ceil_w_d"},
  {(intptr_t)floor_l_d, "floor_l_d"},
  {(intptr_t)floor_w_d, "floor_w_d"},
  {(intptr_t)c_f_s, "c_f_s"},
  {(intptr_t)c_un_s, "c_un_s"},
  {(intptr_t)c_eq_s, "c_eq_s"},
  {(intptr_t)c_ueq_s, "c_ueq_s"},
  {(intptr_t)c_olt_s, "c_olt_s"},
  {(intptr_t)c_ult_s, "c_ult_s"},
  {(intptr_t)c_ole_s, "c_ole_s"},
  {(intptr_t)c_ule_s, "c_ule_s"},
  {(intptr_t)c_sf_s, "c_sf_s"},
  {(intptr_t)c_ngle_s, "c_ngle_s"},
  {(intptr_t)c_seq_s, "c_seq_s"},
  {(intptr_t)c_ngl_s, "c_ngl_s"},
  {(intptr_t)c_lt_s, "c_lt_s"},
  {(intptr_t)c_nge_s, "c_nge_s"},
  {(intptr_t)c_le_s, "c_le_s"},
  {(intptr_t)c_ngt_s, "c_ngt_s"},
  {(intptr_t)c_f_d, "c_f_d"},
  {(intptr_t)c_un_d, "c_un_d"},
  {(intptr_t)c_eq_d, "c_eq_d"},
  {(intptr_t)c_ueq_d, "c_ueq_d"},
  {(intptr_t)c_olt_d, "c_olt_d"},
  {(intptr_t)c_ult_d, "c_ult_d"},
  {(intptr_t)c_ole_d, "c_ole_d"},
  {(intptr_t)c_ule_d, "c_ule_d"},
  {(intptr_t)c_sf_d, "c_sf_d"},
  {(intptr_t)c_ngle_d, "c_ngle_d"},
  {(intptr_t)c_seq_d, "c_seq_d"},
  {(intptr_t)c_ngl_d, "c_ngl_d"},
  {(intptr_t)c_lt_d, "c_lt_d"},
  {(intptr_t)c_nge_d, "c_nge_d"},
  {(intptr_t)c_le_d, "c_le_d"},
  {(intptr_t)c_ngt_d, "c_ngt_d"},
  {(intptr_t)add_s, "add_s"},
  {(intptr_t)sub_s, "sub_s"},
  {(intptr_t)mul_s, "mul_s"},
  {(intptr_t)div_s, "div_s"},
  {(intptr_t)sqrt_s, "sqrt_s"},
  {(intptr_t)abs_s, "abs_s"},
  {(intptr_t)mov_s, "mov_s"},
  {(intptr_t)neg_s, "neg_s"},
  {(intptr_t)add_d, "add_d"},
  {(intptr_t)sub_d, "sub_d"},
  {(intptr_t)mul_d, "mul_d"},
  {(intptr_t)div_d, "div_d"},
  {(intptr_t)sqrt_d, "sqrt_d"},
  {(intptr_t)abs_d, "abs_d"},
  {(intptr_t)mov_d, "mov_d"},
  {(intptr_t)neg_d, "neg_d"},
  {(intptr_t)breakpoint, "breakpoint"},
  {-1, NULL}
};

static csh handle;

static void disassemble(int i, FILE * pFile)
{
    if (bt[i]) fprintf(pFile, "*"); else fprintf(pFile, " ");
    switch(itype[i]) {
      case UJUMP:
        fprintf (pFile, " %x: %s %8x\n",start+i*4,insn[i],ba[i]);break;
      case CJUMP:
        fprintf (pFile, " %x: %s r%d,r%d,%8x\n",start+i*4,insn[i],rs1[i],rs2[i],i?start+i*4+4+((signed int)((unsigned int)source[i]<<16)>>14):*ba);break;
      case SJUMP:
        fprintf (pFile, " %x: %s r%d,%8x\n",start+i*4,insn[i],rs1[i],start+i*4+4+((signed int)((unsigned int)source[i]<<16)>>14));break;
      case FJUMP:
        fprintf (pFile, " %x: %s %8x\n",start+i*4,insn[i],ba[i]);break;
      case RJUMP:
        if ((opcode2[i]&1)&&rt1[i]!=31)
          fprintf (pFile, " %x: %s r%d,r%d\n",start+i*4,insn[i],rt1[i],rs1[i]);
        else
          fprintf (pFile, " %x: %s r%d\n",start+i*4,insn[i],rs1[i]);
        break;
      case SPAN:
        fprintf (pFile, " %x: %s (pagespan) r%d,r%d,%8x\n",start+i*4,insn[i],rs1[i],rs2[i],ba[i]);break;
      case IMM16:
        if(opcode[i]==0xf) //LUI
          fprintf (pFile, " %x: %s r%d,%4x0000\n",start+i*4,insn[i],rt1[i],imm[i]&0xffff);
        else
          fprintf (pFile, " %x: %s r%d,r%d,%d\n",start+i*4,insn[i],rt1[i],rs1[i],imm[i]);
        break;
      case LOAD:
      case LOADLR:
        fprintf (pFile, " %x: %s r%d,r%d+%x\n",start+i*4,insn[i],rt1[i],rs1[i],imm[i]);
        break;
      case STORE:
      case STORELR:
        fprintf (pFile, " %x: %s r%d,r%d+%x\n",start+i*4,insn[i],rs2[i],rs1[i],imm[i]);
        break;
      case ALU:
      case SHIFT:
        fprintf (pFile, " %x: %s r%d,r%d,r%d\n",start+i*4,insn[i],rt1[i],rs1[i],rs2[i]);
        break;
      case MULTDIV:
        fprintf (pFile, " %x: %s r%d,r%d\n",start+i*4,insn[i],rs1[i],rs2[i]);
        break;
      case SHIFTIMM:
        fprintf (pFile, " %x: %s r%d,r%d,%d\n",start+i*4,insn[i],rt1[i],rs1[i],imm[i]);
        break;
      case MOV:
        if((opcode2[i]&0x1d)==0x10)
          fprintf (pFile, " %x: %s r%d\n",start+i*4,insn[i],rt1[i]);
        else if((opcode2[i]&0x1d)==0x11)
          fprintf (pFile, " %x: %s r%d\n",start+i*4,insn[i],rs1[i]);
        else
          fprintf (pFile, " %x: %s\n",start+i*4,insn[i]);
        break;
      case COP0:
        if(opcode2[i]==0)
          fprintf (pFile, " %x: %s r%d,cpr0[%d]\n",start+i*4,insn[i],rt1[i],(source[i]>>11)&0x1f); // MFC0
        else if(opcode2[i]==4)
          fprintf (pFile, " %x: %s r%d,cpr0[%d]\n",start+i*4,insn[i],rs1[i],(source[i]>>11)&0x1f); // MTC0
        else fprintf (pFile, " %x: %s",start+i*4,insn[i]);
        break;
      case COP1:
        if(opcode2[i]<3)
          fprintf (pFile, " %x: %s r%d,cpr1[%d]\n",start+i*4,insn[i],rt1[i],(source[i]>>11)&0x1f); // MFC1
        else if(opcode2[i]>3)
          fprintf (pFile, " %x: %s r%d,cpr1[%d]\n",start+i*4,insn[i],rs1[i],(source[i]>>11)&0x1f); // MTC1
        else fprintf (pFile, " %x: %s",start+i*4,insn[i]);
        break;
      case C1LS:
        fprintf (pFile, " %x: %s cpr1[%d],r%d+%x\n",start+i*4,insn[i],(source[i]>>16)&0x1f,rs1[i],imm[i]);
        break;
      default:
        //fprintf (pFile, " %s %8x",insn[i],source[i]);
        fprintf (pFile, " %x: %s\n",start+i*4,insn[i]);
    }
}

static void debugging(int i, FILE * pFile)
{
  fprintf(pFile, "/************************/");

  fprintf(pFile, "\nU:");
  int r;
  for(r=1;r<=CCREG;r++) {
    if((unneeded_reg[i]>>r)&1) {
      if(r==HIREG) fprintf(pFile, " HI");
      else if(r==LOREG) fprintf(pFile, " LO");
      else fprintf(pFile, " r%d",r);
    }
  }

  fprintf(pFile, "\nUU:");
  for(r=1;r<=CCREG;r++) {
    if(((unneeded_reg_upper[i]&~unneeded_reg[i])>>r)&1) {
      if(r==HIREG) fprintf(pFile, " HI");
      else if(r==LOREG) fprintf(pFile, " LO");
      else fprintf(pFile, " r%d",r);
    }
  }

  fprintf(pFile, "\nwas32:");
  for(r=0;r<=CCREG;r++) {
    if((regs[i].was32>>r)&1) {
      if(r==CCREG) fprintf(pFile, " CC");
      else if(r==HIREG) fprintf(pFile, " HI");
      else if(r==LOREG) fprintf(pFile, " LO");
      else fprintf(pFile, " r%d",r);
    }
  }

  fprintf(pFile, "\nreg_pre:");
  for(r=0;r<HOST_REGS;r++)
  {
    if(r!=EXCLUDE_REG) {
      fprintf(pFile, " %s=%d",regname[r],regmap_pre[i][r]);
    }
  }

  fprintf(pFile, "\nreg_needs:");
  for(r=0;r<HOST_REGS;r++)
  {
    if(r!=EXCLUDE_REG) {
      if((needed_reg[i]>>r)&1)
        fprintf(pFile, " %s",regname[r]);
    }
  }
  
  fprintf(pFile, "\nreq32:");
  for(r=0;r<=CCREG;r++) {
    if((requires_32bit[i]>>r)&1) {
      if(r==CCREG) fprintf(pFile, " CC");
      else if(r==HIREG) fprintf(pFile, " HI");
      else if(r==LOREG) fprintf(pFile, " LO");
      else fprintf(pFile, " r%d",r);
    }
  }

  fprintf(pFile, "\nreg_entry:");
  for(r=0;r<HOST_REGS;r++)
  {
    if(r!=EXCLUDE_REG) {
      fprintf(pFile, " %s=%d",regname[r],regs[i].regmap_entry[r]);
    }
  }

  fprintf(pFile, "\nwas_dirty:");
  for(r=0;r<HOST_REGS;r++)
  {
    if(r!=EXCLUDE_REG) {
      if((regs[i].wasdirty>>r)&1)
        fprintf(pFile, " %s",regname[r]);
    }
  }
  
  fprintf(pFile, "\n/*");
  fprintf(pFile, "\n");
  disassemble(i, pFile);
  fprintf(pFile, "*/");
  
  fprintf(pFile, "\nreg_map:");
  for(r=0;r<HOST_REGS;r++)
  {
    if(r!=EXCLUDE_REG) {
      fprintf(pFile, " %s=%d",regname[r],regs[i].regmap[r]);
    }
  }

  fprintf(pFile, "\nisdirty:");
  for(r=0;r<HOST_REGS;r++)
  {
    if(r!=EXCLUDE_REG) {
      if((regs[i].dirty>>r)&1)
        fprintf(pFile, " %s",regname[r]);
    }
  }

  if(regs[i].isconst) {
    fprintf(pFile, "\nconstants:");
    for(r=0;r<HOST_REGS;r++)
    {
      if(r!=EXCLUDE_REG) {
        if((regs[i].isconst>>r)&1)
          fprintf(pFile, " %s=%x ",regname[r],(int)constmap[i][0]);
      }
    }
  }

  fprintf(pFile, "\nis32:");
  for(r=0;r<=CCREG;r++) {
    if((regs[i].is32>>r)&1) {
      if(r==CCREG) fprintf(pFile, " CC");
      else if(r==HIREG) fprintf(pFile, " HI");
      else if(r==LOREG) fprintf(pFile, " LO");
      else fprintf(pFile, " r%d",r);
    }
  }

  if(itype[i]==RJUMP||itype[i]==UJUMP||itype[i]==CJUMP||itype[i]==SJUMP||itype[i]==FJUMP) {

    fprintf(pFile, "\nbranch_reg(%d):",i);
    for(r=0;r<HOST_REGS;r++)
    {
      if(r!=EXCLUDE_REG) {
        fprintf(pFile, " %s=%d",regname[r],branch_regs[i].regmap[r]);
      }
    }


    fprintf(pFile, "\nbranch_isdirty:");
    for(r=0;r<HOST_REGS;r++)
    {
      if(r!=EXCLUDE_REG) {
        if((branch_regs[i].dirty>>r)&1)
          fprintf(pFile, " %s",regname[r]);
      }
    }

    fprintf(pFile, "\nbranch_is32:");
    for(r=0;r<=CCREG;r++) {
      if((branch_regs[i].is32>>r)&1) {
        if(r==CCREG) fprintf(pFile, " CC");
        else if(r==HIREG) fprintf(pFile, " HI");
        else if(r==LOREG) fprintf(pFile, " LO");
        else fprintf(pFile, " r%d",r);
      }
    }
  }

  fprintf(pFile, "\n/************************/\n\n");
}

static int disasm_block[] = {0xa4000040};

static void replace_addr(intptr_t real_addr, intptr_t addr, size_t addr_size, Variable_t * var, char * op_str, size_t op_size)
{
  char right[256];
  char addr_str[32];
  char * ptr = NULL;
  char * ptr2 = NULL;
  
  if(addr_size == 4)
    sprintf(addr_str, "0x%x", addr);
  else if(addr_size == 8)
    sprintf(addr_str, "0x%llx", addr);
  else
    assert(0);

  ptr = strstr(op_str, addr_str);
  
  if(ptr == NULL) {
    sprintf(addr_str, "0x%x", -addr);
    ptr = strstr(op_str, addr_str);
  
    assert(ptr != NULL);
    assert(*(ptr-2) == '-');
    *(ptr-2) = '+';
  }

  ptr2 = strstr(op_str, "rip");
  if(ptr2 == NULL)
    *ptr = '\0';
  else
    *ptr2 = '\0';

  memcpy(right, (ptr + strlen(addr_str)), op_size - (ptr - op_str)); /* copy right part after address */

  if((real_addr - var->addr) == 0)
    snprintf(op_str, op_size, "%s%s%s", op_str, var->name, right);
  else
    snprintf(op_str, op_size, "%s%s+%d%s", op_str, var->name, (real_addr - var->addr), right);
}

void recomp_dbg_init(void)
{
  var[0].addr = (uintptr_t)g_dev.rdram.dram - 0x80000000;
  var[0].size = g_dev.rdram.dram_size;

  recomp_dbg_base_addr = recomp_dbg_base_addr_rx = (void*)recomp_dbg_extra_memory;

  /* New dynarec init */
  recomp_dbg_out=(u_char *)recomp_dbg_base_addr;

  for(int n=0;n<65536;n++)
    hash_table[n][0]=hash_table[n][1]=NULL;

  copy_size=0;
  expirep=16384; // Expiry pointer, +2 blocks
  literalcount=0;

  arch_init();

  /* Capstone init */
  if(cs_open(ARCHITECTURE, MODE, &handle) != CS_ERR_OK) return;
  cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

 #if RECOMPILER_DEBUG >= NEW_DYNAREC_ARM
  FILE * pFile = fopen ("jump_table.txt","w");
  uintptr_t * src = (uintptr_t *)((char *)base_addr+(1<<TARGET_SIZE_2)-JUMP_TABLE_SIZE);

  while((char *)src<(char *)base_addr+(1<<TARGET_SIZE_2))
  {
    cs_insn *instr;
    size_t count = cs_disasm(handle, (uint8_t*)src, sizeof(uintptr_t), (uintptr_t)src, 0, &instr);

    for(uint32_t i = 0; i < count; i++)
      fprintf(pFile, "0x%" PRIxPTR ": %s %s\n", (uintptr_t)instr[i].address, instr[i].mnemonic, instr[i].op_str);

    cs_free(instr, count);
    src++;

    uint32_t j = 0;
    while(func[j].addr != -1) {
      if(*src>>2 == func[j].addr>>2)
        break;
      j++;
    }
    
    fprintf(pFile, "0x%" PRIxPTR ": 0x%" PRIxPTR " (%s)\n", (uintptr_t)src, (uintptr_t)*src, func[j].name);
    src++;
  }
  fflush(pFile);
  fclose(pFile);
#endif
}

void recomp_dbg_cleanup(void)
{
  /* New dynarec cleanup */
  for(int n=0;n<4096;n++) ll_clear(jump_in+n);
  for(int n=0;n<4096;n++) ll_clear(jump_out+n);
  for(int n=0;n<4096;n++) ll_clear(jump_dirty+n);
  assert(copy_size==0);

  VirtualFree(recomp_dbg_base_addr, 0, MEM_RELEASE);

  /* Capstone cleanup */
  if(handle == 0) return;
  cs_close(&handle);
}

void recomp_dbg_block(int addr)
{
  uint32_t * beginning;
  uint32_t * end;
  cs_insn *instr;
  size_t count;
  int32_t size = 0;
  int32_t sum = 0;
  char filename[32];
  FILE * pFile;

  /* Copy data from running dynarec */
#undef using_tlb
#undef stop_after_jal
extern unsigned int stop_after_jal;
extern unsigned int using_tlb;

  recomp_dbg_using_tlb = using_tlb;
  recomp_dbg_stop_after_jal = stop_after_jal;

  /* Warning: invalid_code is shared between both running and debugged recompiler*/
  beginning=(uint32_t *)recomp_dbg_out;
  recomp_dbg_new_recompile_block(addr);
  end=(uint32_t *)recomp_dbg_out;

  int disasm=0;
  int block, inst;
  for(block=0;block<(sizeof(disasm_block)>>2);block++) {
    for(inst=0;inst<slen;inst++) {
      if((start+inst*4)==disasm_block[block]) {
        disasm=1;
        break;
      }
    }
  }

  if((disasm == 0) || (handle == 0)) return;

  sprintf(filename, "0x%.8x.txt",addr);
  pFile = fopen (filename,"w");

  for(int i=0;i<slen;i++)
    disassemble(i, pFile);

  fflush(pFile);
  fclose(pFile);

  sprintf(filename, "%s_0x%.8x.txt","debug",addr);
  pFile = fopen (filename,"w");

  for(int i=0;i<slen;i++)
    debugging(i, pFile);

  fflush(pFile);
  fclose(pFile);

  sprintf(filename, "%s_0x%.8x.txt",ARCH_NAME,addr);
  pFile = fopen (filename,"w");
  size = (intptr_t)end - (intptr_t)beginning;
  size = (size < 0) ? (-size) : size;

  count = cs_disasm(handle, (uint8_t*)beginning, size, (uintptr_t)beginning, 0, &instr);
  if(count <= 0) return;

  for (uint32_t i = 0; i < count; i++) {
    sum += instr[i].size;
 #if RECOMPILER_DEBUG >= NEW_DYNAREC_ARM
    if(INSTRUCTION.operands[1].reg == FP_REGISTER) {
      uint32_t j = 0;
      uint32_t imm;

      if(INSTRUCTION.op_count > 2)
        imm = INSTRUCTION.operands[2].mem.base;
      else
        imm = INSTRUCTION.operands[1].mem.disp;

      assert(imm>=0 && imm<4096);

      while(var[j].addr != -1) {
        uint32_t offset = var[j].addr - (uintptr_t)&g_dev.r4300.new_dynarec_hot_state.dynarec_local;
        if(imm >= offset && imm < (offset + var[j].size))
          break;
        j++;
      }
      if(var[j].addr != -1) {
        if((imm - (var[j].addr - (uintptr_t)&g_dev.r4300.new_dynarec_hot_state.dynarec_local)) == 0)
          fprintf(pFile, "0x%" PRIxPTR ": %s %s (%s)\n", (uintptr_t)instr[i].address, instr[i].mnemonic, instr[i].op_str, var[j].name);
        else
          fprintf(pFile, "0x%" PRIxPTR ": %s %s (%s+%d)\n", (uintptr_t)instr[i].address, instr[i].mnemonic, instr[i].op_str, var[j].name, imm - (var[j].addr - (uintptr_t)&g_dev.r4300.new_dynarec_hot_state.dynarec_local));
        continue;
      }
    }
#else
    if(INSTRUCTION.disp /*|| (INSTRUCTION.operands[1].imm && INSTRUCTION.operands[1].type == X86_OP_IMM)*/) {
      char op_str[256];
      uint32_t j = 0;
      uint32_t off = 0;
      int64_t addr = INSTRUCTION.disp;

      strcpy(op_str, instr[i].op_str);

      char *ptr = strstr(op_str, "rip");
      if(ptr != NULL) // rip relative
        addr = addr + instr[i+1].address;

      while(var[j].addr != -1) {
        if(addr >= var[j].addr && addr < (var[j].addr + var[j].size))
          break;
        j++;
      }

      //TODO: remove replace_addr?
      if(var[j].addr != -1)
        replace_addr(addr, INSTRUCTION.disp, sizeof(INSTRUCTION.disp), &var[j], op_str, sizeof(op_str));

      //uint32_t k = 0;
      //while(var[k].addr != -1) {
      //  if(INSTRUCTION.operands[1].imm >= var[k].addr && INSTRUCTION.operands[1].imm < (var[k].addr + var[k].size))
      //    break;
      //  k++;
      //}
      //
      //if(var[k].addr != -1)
      //  replace_addr(addr, INSTRUCTION.operands[1].imm, sizeof(INSTRUCTION.operands[1].imm), &var[k], op_str, sizeof(op_str));

      if((var[j].addr != -1) /*|| (var[k].addr != -1)*/)
      {
        fprintf(pFile, "0x%" PRIxPTR ": %s %s\n", (uintptr_t)instr[i].address, instr[i].mnemonic, op_str);
        continue;
      }
    }
#endif

    if(instr[i].id == CALL_INST || instr[i].id == BRANCH_INST) {
      uint32_t j = 0;
      intptr_t addr = (intptr_t)INSTRUCTION.operands[0].imm;
      intptr_t * paddr = (intptr_t*)addr;

      while(func[j].addr != -1) {
#if RECOMPILER_DEBUG >= NEW_DYNAREC_ARM
        if((addr>>2 == func[j].addr>>2) || ((*(paddr+1))>>2 == func[j].addr>>2)) // check jump_table_symbols on ARM
#else
        if(addr>>2 == func[j].addr>>2)
#endif
          break;
        j++;
      }

      if(func[j].addr != -1) {
        fprintf(pFile, "0x%" PRIxPTR ": %s %s\n", (uintptr_t)instr[i].address, instr[i].mnemonic, func[j].name);
        continue;
      }
    }
    fprintf(pFile, "0x%" PRIxPTR ": %s %s\n", (uintptr_t)instr[i].address, instr[i].mnemonic, instr[i].op_str);
  }

  if(size != sum)
    fprintf(pFile, "Failed to disassemble code at: 0x%.8x\n", (uintptr_t)beginning + sum);

  cs_free(instr, count);
  fflush(pFile);
  fclose(pFile);
}
