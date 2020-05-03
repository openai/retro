/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - linkage_arm64.S                                         *
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

#define GLOBAL_FUNCTION(name)  \
    .globl name;               \
    .hidden name;              \
    .type name, %function;     \
    name

#define LOCAL_FUNCTION(name)  \
    .hidden name;             \
    .type name, %function;    \
    name

#define GLOBAL_VARIABLE(name, size_) \
    .global name;                    \
    .hidden name;                    \
    .type   name, %object;           \
    .size   name, size_

.macro movl Wn, imm
    movz    \Wn, (\imm >> 16) & 0xFFFF, lsl 16
    movk    \Wn,  \imm & 0xFFFF
.endm

#define TEXT_SECTION .text
#define END_SECTION

#include "asm_defines_gas.h"

device_r4300_new_dynarec_hot_state_dynarec_local = (offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_dynarec_local)
saved_context = 160

/* Defines offsets for fp addressed variables */
fp_next_interrupt      = offsetof_struct_new_dynarec_hot_state_next_interrupt
fp_cycle_count         = offsetof_struct_new_dynarec_hot_state_cycle_count
fp_pending_exception   = offsetof_struct_new_dynarec_hot_state_pending_exception
fp_pcaddr              = offsetof_struct_new_dynarec_hot_state_pcaddr
fp_stop                = offsetof_struct_new_dynarec_hot_state_stop
fp_invc_ptr            = offsetof_struct_new_dynarec_hot_state_invc_ptr
fp_fcr0                = offsetof_struct_new_dynarec_hot_state_fcr0
fp_fcr31               = offsetof_struct_new_dynarec_hot_state_fcr31
fp_regs                = offsetof_struct_new_dynarec_hot_state_regs
fp_hi                  = offsetof_struct_new_dynarec_hot_state_hi
fp_lo                  = offsetof_struct_new_dynarec_hot_state_lo
fp_cp0_regs            = offsetof_struct_new_dynarec_hot_state_cp0_regs
fp_cp1_regs_simple     = offsetof_struct_new_dynarec_hot_state_cp1_regs_simple
fp_cp1_regs_double     = offsetof_struct_new_dynarec_hot_state_cp1_regs_double
fp_rounding_modes      = offsetof_struct_new_dynarec_hot_state_rounding_modes
fp_branch_target       = offsetof_struct_new_dynarec_hot_state_branch_target
fp_pc                  = offsetof_struct_new_dynarec_hot_state_pc
fp_fake_pc             = offsetof_struct_new_dynarec_hot_state_fake_pc
fp_mini_ht             = offsetof_struct_new_dynarec_hot_state_mini_ht
fp_restore_candidate   = offsetof_struct_new_dynarec_hot_state_restore_candidate
fp_memory_map          = offsetof_struct_new_dynarec_hot_state_memory_map

TEXT_SECTION
    
GLOBAL_FUNCTION(jump_vaddr_x0):
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x1):
    mov    w0, w1
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x2):
    mov    w0, w2
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x3):
    mov    w0, w3
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x4):
    mov    w0, w4
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x5):
    mov    w0, w5
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x6):
    mov    w0, w6
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x8):
    mov    w0, w8
    b      jump_vaddr

GLOBAL_FUNCTION(jump_vaddr_x9):
    mov    w0, w9
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x10):
    mov    w0, w10
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x11):
    mov    w0, w11
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x12):
    mov    w0, w12
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x13):
    mov    w0, w13
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x14):
    mov    w0, w14
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x15):
    mov    w0, w15
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x16):
    mov    w0, w16
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x17):
    mov    w0, w17
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x18):
    brk 0  /*trampoline jumps uses x18*/
    mov    w0, w18
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x19):
    mov    w0, w19
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x20):
    brk    0 /*address in cycle count*/
    mov    w0, w20
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x21):
    mov    w0, w21
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x22):
    mov    w0, w22
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x23):
    mov    w0, w23
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x24):
    mov    w0, w24
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x25):
    mov    w0, w25
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x26):
    mov    w0, w26
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x27):
    mov    w0, w27
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x28):
    mov    w0, w28
    b      jump_vaddr
    
GLOBAL_FUNCTION(jump_vaddr_x7):
    mov    w0, w7
 
GLOBAL_FUNCTION(jump_vaddr):
    str    w20, [x29, #fp_cycle_count]
    ldr    w18, [x29, #fp_next_interrupt]
    add    w20, w18, w20 /* Count */
    str    w20, [x29, #fp_cp0_regs+36] /* Count */
    bl     get_addr_ht
    ldr    w20, [x29, #fp_cycle_count]
    br     x0
    
GLOBAL_FUNCTION(verify_code):
    /* x0 = head */
    mov    x21, x30 /* Save link register */
    bl     verify_dirty
    tst    x0, x0
    b.ne   .D1
    mov    x30, x21 /* Restore link register */
    ret
.D1:
    bl     get_addr
    br     x0
    
GLOBAL_FUNCTION(cc_interrupt):
    ldr    w0, [x29, #fp_next_interrupt]
    add    w20, w0, w20 /* Count */
    str    wzr, [x29, #fp_pending_exception]
    mov    w1, #0x1fc
    lsr    w0, w20, #19
    and    w1, w1, w0
    add    x0, x29, #fp_restore_candidate
    str    w20, [x29, #fp_cp0_regs+36] /* Count */
    ldr    w22, [x1, x0]
    mov    x20, x30 /* Save link register */
    tst    w22, w22
    b.ne   .E4
.E1:
    bl     dynarec_gen_interrupt
    mov    x30, x20 /* Restore link register */
    ldr    w20, [x29, #fp_cp0_regs+36] /* Count */
    ldr    w0, [x29, #fp_next_interrupt]
    ldr    w1, [x29, #fp_pending_exception]
    ldr    w2, [x29, #fp_stop]
    sub    w20, w20, w0
    tst    w2, w2
    b.ne   .E3
    tst    w1, w1
    b.ne   .E2
    ret
.E2:
    ldr    w0, [x29, #fp_pcaddr]
    bl     get_addr_ht
    br     x0
.E3:
    add    x16, x29, #saved_context
    ldp    x19,x20,[x16,#0]
    ldp    x21,x22,[x16,#16]
    ldp    x23,x24,[x16,#32]
    ldp    x25,x26,[x16,#48]
    ldp    x27,x28,[x16,#64]
    ldp    x29,x30,[x16,#80]
    ret
.E4:
    /* Move 'dirty' blocks to the 'clean' list */
    str    wzr, [x1, x0]
    lsl    w21, w1, #3
    mov    w23, #0
.E5:
    tst    w22, #1
    b.eq   .E6
    add    w0, w21, w23
    bl     clean_blocks
.E6:
    lsr    w22, w22, #1
    add    w23, w23, #1
    tst    w23, #31
    b.ne   .E5
    b      .E1
    
GLOBAL_FUNCTION(do_interrupt):
    ldr    w0, [x29, #fp_pcaddr]
    bl     get_addr_ht
    ldr    w1, [x29, #fp_next_interrupt]
    ldr    w20, [x29, #fp_cp0_regs+36] /* Count */
    sub    w20, w20, w1
    br     x0
    
GLOBAL_FUNCTION(fp_exception):
    mov    w2, #0x10000000
    add    w2, w2, #0x2c
.E7:
    ldr    w1, [x29, #fp_cp0_regs+48] /* Status */
    mov    w3, #0x80000000
    str    w0, [x29, #fp_cp0_regs+56] /* EPC */
    orr    w1, w1, #2
    str    w1, [x29, #fp_cp0_regs+48] /* Status */
    str    w2, [x29, #fp_cp0_regs+52] /* Cause */
    add    w0, w3, #0x180
    bl     get_addr_ht
    br     x0
    
GLOBAL_FUNCTION(fp_exception_ds):
    mov    w2, #0x90000000 /* Set high bit if delay slot */
    add    w2, w2, #0x2c
    b      .E7
    
GLOBAL_FUNCTION(jump_syscall):
    mov    w2, #0x20
    b      .E7
    
GLOBAL_FUNCTION(jump_eret):
    ldr    w1, [x29, #fp_cp0_regs+48] /* Status */
    ldr    w0, [x29, #fp_next_interrupt]
    bic    w1, w1, #2
    add    w20, w0, w20
    str    w1, [x29, #fp_cp0_regs+48] /* Status */
    str    w20, [x29, #fp_cp0_regs+36] /* Count */
    bl     new_dynarec_check_interrupt
    ldr    w1, [x29, #fp_next_interrupt]
    ldr    w0, [x29, #fp_cp0_regs+56] /* EPC */
    subs   w20, w20, w1
    b.pl   .E11
.E8:
    add    x6, x29, #fp_regs+256
    mov    w5, #248
    mov    w1, #0
.E9:
    /*TODO: Optimize for 64bit*/
    sub    x6, x6, #8
    ldr    w2, [x6]
    ldr    w3, [x6, #4]
    eor    w3, w3, w2, asr #31
    subs   w3, w3, #1
    adc    w1, w1, w1
    subs   w5, w5, #8
    b.ne    .E9
    ldr    w2, [x29, #fp_hi]
    ldr    w3, [x29, #fp_hi+4]
    eor    w3, w3, w2, asr #31
    tst    w3, w3
    b.ne   .E10
    ldr    w2, [x29, #fp_lo]
    ldr    w3, [x29, #fp_lo+4]
    eor    w3, w3, w2, asr #31
.E10:
    subs   w3, w3, #1
    adc    w1, w1, w1
    bl     get_addr_32
    br     x0
.E11:
    str    w0, [x29, #fp_pcaddr]
    bl     cc_interrupt
    ldr    w0, [x29, #fp_pcaddr]
    b      .E8
    
GLOBAL_FUNCTION(new_dyna_start):
    adrp   x16, g_dev
    add    x16, x16, :lo12:g_dev
    movl   x1, (device_r4300_new_dynarec_hot_state_dynarec_local + saved_context)
    add    x16, x16, x1
    adrp   x1, base_addr_rx
    add    x1, x1, :lo12:base_addr_rx
    mov    w0, #0xa4000000
    stp    x19,x20,[x16,#0]
    stp    x21,x22,[x16,#16]
    stp    x23,x24,[x16,#32]
    stp    x25,x26,[x16,#48]
    stp    x27,x28,[x16,#64]
    stp    x29,x30,[x16,#80]
    sub    x29, x16, #saved_context
    ldr    x19, [x1]
    add    w0, w0, #0x40
    bl     new_recompile_block
    ldr    w0, [x29, #fp_next_interrupt]
    ldr    w20, [x29, #fp_cp0_regs+36] /* Count */
    sub    w20, w20, w0
    br     x19
    
GLOBAL_FUNCTION(breakpoint):
    brk 0
    ret

END_SECTION