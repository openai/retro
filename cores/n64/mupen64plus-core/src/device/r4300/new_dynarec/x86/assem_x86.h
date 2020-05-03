#ifndef M64P_DEVICE_R4300_NEW_DYNAREC_X86_ASSEM_X86_H
#define M64P_DEVICE_R4300_NEW_DYNAREC_X86_ASSEM_X86_H

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

#define HOST_REGS 8
#define HOST_CCREG ESI
#define HOST_BTREG EBP
#define EXCLUDE_REG ESP

//#define IMM_PREFETCH 1
#define HOST_IMM_ADDR32 1
#define INVERTED_CARRY 1
#define DESTRUCTIVE_WRITEBACK 1
#define DESTRUCTIVE_SHIFT 1

#define USE_MINI_HT 1

#define TARGET_SIZE_2 25 // 2^25 = 32 megabytes
#define JUMP_TABLE_SIZE 0 // Not needed for 32-bit x86

/* x86 calling convention:
   caller-save: %eax %ecx %edx
   callee-save: %ebp %ebx %esi %edi */

#endif /* M64P_DEVICE_R4300_NEW_DYNAREC_X86_ASSEM_X86_H */
