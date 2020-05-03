;Mupen64plus - linkage_x86.asm                                           
;Copyright (C) 2009-2011 Ari64                                         
;                                                                      
;This program is free software; you can redistribute it and/or modify  
;it under the terms of the GNU General Public License as published by  
;the Free Software Foundation; either version 2 of the License, or     
;(at your option) any later version.                                   
;                                                                      
;This program is distributed in the hope that it will be useful,       
;but WITHOUT ANY WARRANTY; without even the implied warranty of        
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
;GNU General Public License for more details.                          
;                                                                      
;You should have received a copy of the GNU General Public License     
;along with this program; if not, write to the                         
;Free Software Foundation, Inc.,                                       
;51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          

%include "asm_defines_nasm.h"

%ifidn __OUTPUT_FORMAT__,elf
section .note.GNU-stack noalloc noexec nowrite progbits
%endif
%ifidn __OUTPUT_FORMAT__,elf32
section .note.GNU-stack noalloc noexec nowrite progbits
%endif
%ifidn __OUTPUT_FORMAT__,elf64
section .note.GNU-stack noalloc noexec nowrite progbits
%endif

%ifdef LEADING_UNDERSCORE
    %macro  cglobal 1
      global  _%1
      %define %1 _%1
    %endmacro

    %macro  cextern 1
      extern  _%1
      %define %1 _%1
    %endmacro
%else
    %macro  cglobal 1
      global  %1
    %endmacro

    %macro  cextern 1
      extern  %1
    %endmacro
%endif

%ifdef WIN64
%define ARG1_REG ecx
%define ARG2_REG edx
%define ARG3_REG r8d
%define ARG4_REG r9d
%define ARG1_REG64 rcx
%define ARG2_REG64 rdx
%define ARG3_REG64 r8
%define ARG4_REG64 r9
%define CCREG esi
%define CCREG64 rsi
%else
%define ARG1_REG edi
%define ARG2_REG esi
%define ARG3_REG edx
%define ARG4_REG ecx
%define ARG1_REG64 rdi
%define ARG2_REG64 rsi
%define ARG3_REG64 rdx
%define ARG4_REG64 rcx
%define CCREG ebx
%define CCREG64 rbx
%endif

%define g_dev_r4300_cached_interp_invalid_code              (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_cached_interp + offsetof_struct_cached_interp_invalid_code)
%define g_dev_r4300_new_dynarec_hot_state_stop              (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_stop)
%define g_dev_r4300_new_dynarec_hot_state_regs              (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_regs)
%define g_dev_r4300_new_dynarec_hot_state_hi                (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_hi)
%define g_dev_r4300_new_dynarec_hot_state_lo                (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_lo)
%define g_dev_r4300_new_dynarec_hot_state_cp0_regs          (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_cp0_regs)
%define g_dev_r4300_new_dynarec_hot_state_next_interrupt    (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_next_interrupt)
%define g_dev_r4300_new_dynarec_hot_state_cycle_count       (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_cycle_count)
%define g_dev_r4300_new_dynarec_hot_state_pending_exception (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_pending_exception)
%define g_dev_r4300_new_dynarec_hot_state_pcaddr            (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_pcaddr)
%define g_dev_r4300_new_dynarec_hot_state_branch_target     (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_branch_target)
%define g_dev_r4300_new_dynarec_hot_state_restore_candidate (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_restore_candidate)
%define g_dev_r4300_new_dynarec_hot_state_memory_map        (g_dev + offsetof_struct_device_r4300 + offsetof_struct_r4300_core_new_dynarec_hot_state + offsetof_struct_new_dynarec_hot_state_memory_map)

cglobal jump_vaddr_eax
cglobal jump_vaddr_ecx
cglobal jump_vaddr_edx
cglobal jump_vaddr_ebx
cglobal jump_vaddr_ebp
cglobal jump_vaddr_esi
cglobal jump_vaddr_edi
cglobal verify_code
cglobal cc_interrupt
cglobal do_interrupt
cglobal fp_exception
cglobal fp_exception_ds
cglobal jump_syscall
cglobal jump_eret
cglobal new_dyna_start
cglobal invalidate_block_eax
cglobal invalidate_block_ecx
cglobal invalidate_block_edx
cglobal invalidate_block_ebx
cglobal invalidate_block_ebp
cglobal invalidate_block_esi
cglobal invalidate_block_edi
cglobal breakpoint

cextern base_addr
cextern new_recompile_block
cextern get_addr_ht
cextern get_addr
cextern dynarec_gen_interrupt
cextern clean_blocks
cextern invalidate_block
cextern new_dynarec_check_interrupt
cextern get_addr_32
cextern g_dev
cextern verify_dirty

section .bss
align 4

section .rodata
section .text

jump_vaddr_eax:
    mov     ARG1_REG,    eax
    jmp     jump_vaddr

jump_vaddr_edx:
    mov     ARG1_REG,    edx
    jmp     jump_vaddr

jump_vaddr_ebx:
%ifdef WIN64
    mov     ARG1_REG,    ebx
    jmp     jump_vaddr
%else
    int     3
%endif

jump_vaddr_edi:
    mov     ARG1_REG,    edi
    jmp     jump_vaddr

jump_vaddr_ebp:
    mov     ARG1_REG,    ebp
    jmp     jump_vaddr

jump_vaddr_esi:
%ifdef WIN64
    int     3
%else
    mov     ARG1_REG,    esi
    jmp     jump_vaddr
%endif

jump_vaddr_ecx:
    mov     ARG1_REG,    ecx

jump_vaddr:
    mov     DWORD[rel g_dev_r4300_new_dynarec_hot_state_cycle_count],    CCREG
    add     CCREG,    [rel g_dev_r4300_new_dynarec_hot_state_next_interrupt]
    mov     [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+36],    CCREG
    call    get_addr_ht
    mov     CCREG,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_cycle_count]
    jmp     rax


verify_code:
    ;ARG1_REG64 = head
    add     rsp,    -8
    call    verify_dirty
    test    eax,eax
    jne     _D1
    add     rsp,    8
    ret
_D1:
    mov     ARG1_REG,    eax
    call    get_addr
    add     rsp,    16
    jmp     rax

cc_interrupt:
    add     CCREG,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_next_interrupt]
    add     rsp,    -56 ;Align stack
    mov     DWORD[rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+36],    CCREG
    shr     CCREG,    19
    mov     DWORD [rel g_dev_r4300_new_dynarec_hot_state_pending_exception],    0
    and     CCREG,    01fch
    lea     r10,    [rel g_dev_r4300_new_dynarec_hot_state_restore_candidate]
    cmp     DWORD [r10+CCREG64],    0
    jne     _E4
_E1:
    call    dynarec_gen_interrupt
    mov     CCREG,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+36]
    mov     eax,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_next_interrupt]
    mov     ecx,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_pending_exception]
    mov     edx,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_stop]
    add     rsp,    56
    sub     CCREG,    eax
    test    edx,    edx
    jne     _E3
    test    ecx,    ecx
    jne     _E2
    ret
_E2:
    add     rsp,    -8
    mov     ARG1_REG,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_pcaddr]
    mov     DWORD[rel g_dev_r4300_new_dynarec_hot_state_cycle_count],    CCREG
    call    get_addr_ht
    mov     CCREG,    DWORD[rel g_dev_r4300_new_dynarec_hot_state_cycle_count]
    add     rsp,    16
    jmp     rax
_E3:
    add     rsp,    64    ;pop return address
    ;restore callee-save registers
    pop     rbp
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
%ifdef WIN64
    pop     rsi
    pop     rdi
%endif
    ret             ;exit dynarec
_E4:
    ;Move 'dirty' blocks to the 'clean' list
    mov     edx,    DWORD[r10+CCREG64]
    mov     DWORD [r10+CCREG64],    0
    shl     CCREG,    3
    mov     ebp,    0
_E5:
    shr     edx,    1
    jnc     _E6
    mov     ARG1_REG,    CCREG
    add     ARG1_REG,    ebp
    call    clean_blocks
_E6:
    inc     ebp
    test    ebp,    31
    jne     _E5
    jmp     _E1

do_interrupt:
    mov     ARG1_REG,    [rel g_dev_r4300_new_dynarec_hot_state_pcaddr]
    call    get_addr_ht
    mov     CCREG,    [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+36]
    mov     edx,    [rel g_dev_r4300_new_dynarec_hot_state_next_interrupt]
    sub     CCREG,    edx
    jmp     rax

fp_exception:
    mov     edx,    01000002ch
_E7:
    ;EAX = pcaddr
    mov     ecx,    [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+48]
    or      ecx,    2
    mov     [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+48],    ecx     ;Status
    mov     [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+52],    edx     ;Cause
    mov     [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+56],    eax     ;EPC
    mov     ARG1_REG,    080000180h
    call    get_addr_ht
    jmp     rax

fp_exception_ds:
    mov     edx,    09000002ch    ;Set high bit if delay slot
    jmp     _E7

jump_syscall:
    mov     edx,    020h
    jmp     _E7

jump_eret:
    mov     ecx,    [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+48]        ;Status
    add     CCREG,    [rel g_dev_r4300_new_dynarec_hot_state_next_interrupt]
    and     ecx,    0FFFFFFFDh
    mov     [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+36],    CCREG        ;Count
    mov     [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+48],    ecx        ;Status
    call    new_dynarec_check_interrupt
    mov     eax,    [rel g_dev_r4300_new_dynarec_hot_state_next_interrupt]
    mov     CCREG,    [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+36]
    sub     CCREG,    eax
    mov     [rel g_dev_r4300_new_dynarec_hot_state_cycle_count],    CCREG
    mov     eax,    [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+56]        ;EPC
    jns     _E11
_E8:
    mov     r9,     248
    xor     r10d,    r10d
_E9:
    lea     r8,     [rel g_dev_r4300_new_dynarec_hot_state_regs]
    mov     ecx,    [r8+r9]
    mov     edx,    [r8+r9+4]
    sar     ecx,    31
    xor     edx,    ecx
    neg     edx
    adc     r10d,    r10d
    sub     r9,     8
    jne     _E9
    lea     r8,     [rel g_dev_r4300_new_dynarec_hot_state_hi]
    mov     ecx,    [r8+r9]
    mov     edx,    [r8+r9+4]
    sar     ecx,    31
    xor     edx,    ecx
    jne     _E10
    lea     r8,     [rel g_dev_r4300_new_dynarec_hot_state_lo]
    mov     ecx,    [r8+r9]
    mov     edx,    [r8+r9+4]
    sar     ecx,    31
    xor     edx,    ecx
_E10:
    neg     edx
    adc     ARG2_REG,    r10d
    mov     ARG1_REG,    eax
    call    get_addr_32
    mov     CCREG,    [rel g_dev_r4300_new_dynarec_hot_state_cycle_count]
    jmp     rax
_E11:
    mov     [rel g_dev_r4300_new_dynarec_hot_state_pcaddr],    eax
    call    cc_interrupt
    mov     eax,    [rel g_dev_r4300_new_dynarec_hot_state_pcaddr]
    jmp     _E8

new_dyna_start:
    ;we must push an even # of registers to keep stack 16-byte aligned
%ifdef WIN64
    push rdi
    push rsi
%endif
    push rbx
    push r12
    push r13
    push r14
    push r15
    push rbp
    add     rsp,    -56
    mov     ARG1_REG,    0a4000040h
    call    new_recompile_block
    mov     eax,    DWORD [rel g_dev_r4300_new_dynarec_hot_state_next_interrupt]
    mov     CCREG,    DWORD [rel g_dev_r4300_new_dynarec_hot_state_cp0_regs+36]
    sub     CCREG,    eax
    mov     rax,    QWORD[rel base_addr]
    jmp     rax
    
invalidate_block_eax:
    mov     ARG1_REG,    eax
    jmp     invalidate_block_call

invalidate_block_edi:
    mov     ARG1_REG,    edi
    jmp     invalidate_block_call

invalidate_block_edx:
    mov     ARG1_REG,    edx
    jmp     invalidate_block_call

invalidate_block_ebx:
    mov     ARG1_REG,    ebx
    jmp     invalidate_block_call

invalidate_block_ebp:
    mov     ARG1_REG,    ebp
    jmp     invalidate_block_call

invalidate_block_esi:
    mov     ARG1_REG,    esi
    jmp     invalidate_block_call

invalidate_block_ecx:
    mov     ARG1_REG,    ecx

invalidate_block_call:
    add     rsp,    -8
    call    invalidate_block
    add     rsp,    8
    ret

breakpoint:
    int    3
    ret
