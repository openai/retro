/*
 * Copyright (C) 2012-2019  Free Software Foundation, Inc.
 *
 * This file is part of GNU lightning.
 *
 * GNU lightning is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU lightning is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * Authors:
 *	Paulo Cesar Pereira de Andrade
 */

#ifndef _jit_x86_h
#define _jit_x86_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	2

/*
 * Types
 */
#define jit_sse2_p()		jit_cpu.sse2
#define jit_x87_reg_p(reg)	((reg) >= _ST0 && (reg) <= _ST6)
#if __WORDSIZE == 32
#  if defined(__x86_64__)
#    define __X64_32		1
#    define __X64		1
#  else
#    define __X32		1
#  endif
#else
#  define __X64			1
#endif

#define JIT_FP			_RBP
typedef enum {
#if __X32
#  define jit_r(i)		(_RAX + (i))
#  define jit_r_num()		3
#  define jit_v(i)		(_RBX + (i))
#  define jit_v_num()		3
#  define jit_f(i)		(jit_cpu.sse2 ? _XMM0 + (i) : _ST0 + (i))
#  define jit_f_num()		(jit_cpu.sse2 ? 8 : 6)
#  define JIT_R0		_RAX
#  define JIT_R1		_RCX
#  define JIT_R2		_RDX
    _RAX,	_RCX,	_RDX,
#  define JIT_V0		_RBX
#  define JIT_V1		_RSI
#  define JIT_V2		_RDI
    _RBX,	_RSI,	_RDI,
    _RSP,	_RBP,
#  define JIT_F0		(jit_sse2_p() ? _XMM0 : _ST0)
#  define JIT_F1		(jit_sse2_p() ? _XMM1 : _ST1)
#  define JIT_F2		(jit_sse2_p() ? _XMM2 : _ST2)
#  define JIT_F3		(jit_sse2_p() ? _XMM3 : _ST3)
#  define JIT_F4		(jit_sse2_p() ? _XMM4 : _ST4)
#  define JIT_F5		(jit_sse2_p() ? _XMM5 : _ST5)
#  define JIT_F6		(jit_sse2_p() ? _XMM6 : _ST6)
    _XMM0,	_XMM1,	_XMM2,	_XMM3,	_XMM4,	_XMM5,	_XMM6,	 _XMM7,
#  define jit_sse_reg_p(reg)	((reg) >= _XMM0 && (reg) <= _XMM7)
#else
#  if __CYGWIN__ || _WIN32
#    define jit_r(i)		(_RAX + (i))
#    define jit_r_num()		3
#    define jit_v(i)		(_RBX + (i))
#    define jit_v_num()		7
#    define jit_f(index)	(_XMM4 + (index))
#    define jit_f_num()		12
#    define JIT_R0		_RAX
#    define JIT_R1		_R10
#    define JIT_R2		_R11
#    define JIT_V0		_RBX
#    define JIT_V1		_RDI
#    define JIT_V2		_RSI
#    define JIT_V3		_R12
#    define JIT_V4		_R13
#    define JIT_V5		_R14
#    define JIT_V6		_R15
    /* Volatile - Return value register */
    _RAX,
    /* Volatile */
    _R10,	_R11,
    /* Nonvolatile */
    _RBX,	_RDI,	_RSI,
    _R12,	_R13,	_R14,	_R15,
    /* Volatile - Integer arguments (4 to 1) */
    _R9,	_R8,	_RDX,	_RCX,
    /* Nonvolatile */
    _RSP,	_RBP,
#    define JIT_F0		_XMM4
#    define JIT_F1		_XMM5
#    define JIT_F2		_XMM6
#    define JIT_F3		_XMM7
#    define JIT_F4		_XMM8
#    define JIT_F5		_XMM9
#    define JIT_F6		_XMM10
#    define JIT_F7		_XMM11
#    define JIT_F8		_XMM12
#    define JIT_F9		_XMM13
#    define JIT_F10		_XMM14
#    define JIT_F11		_XMM15
    /* Volatile */
    _XMM4,	_XMM5,
    /* Nonvolatile */
    _XMM6,	_XMM7,	_XMM8,	_XMM9,	_XMM10,
    _XMM11,	_XMM12, _XMM13,	_XMM14,	_XMM15,
    /* Volatile - FP arguments (4 to 1) */
    _XMM3,	_XMM2,	_XMM1,	_XMM0,
#    define jit_sse_reg_p(reg)	((reg) >= _XMM4 && (reg) <= _XMM0)
#  else
#    define jit_r(i)		(_RAX + (i))
#    define jit_r_num()		3
#    define jit_v(i)		(_RBX + (i))
#    define jit_v_num()		5
#    define jit_f(index)	(_XMM8 + (index))
#    define jit_f_num()		8
#    define JIT_R0		_RAX
#    define JIT_R1		_R10
#    define JIT_R2		_R11
    _RAX,	_R10,	_R11,
#    define JIT_V0		_RBX
#    define JIT_V1		_R13
#    define JIT_V2		_R14
#    define JIT_V3		_R15
#    define JIT_V4		_R12
    _RBX,	_R13,	_R14,	_R15,	_R12,
    _R9,	_R8,	_RCX,	_RDX,	_RSI,	_RDI,
    _RSP,	_RBP,
#    define JIT_F0		_XMM8
#    define JIT_F1		_XMM9
#    define JIT_F2		_XMM10
#    define JIT_F3		_XMM11
#    define JIT_F4		_XMM12
#    define JIT_F5		_XMM13
#    define JIT_F6		_XMM14
#    define JIT_F7		_XMM15
    _XMM8,	_XMM9,	_XMM10,	_XMM11,	_XMM12,	_XMM13,	_XMM14,	_XMM15,
    _XMM7,	_XMM6,	_XMM5,	_XMM4,	_XMM3,	_XMM2,	_XMM1,	_XMM0,
#    define jit_sse_reg_p(reg)	((reg) >= _XMM8 && (reg) <= _XMM0)
#  endif
#endif
    _ST0,	_ST1,	_ST2,	_ST3,	_ST4,	_ST5,	_ST6,
#  define JIT_NOREG		_NOREG
    _NOREG,
} jit_reg_t;

typedef struct {
    /* x87 present */
    jit_uint32_t fpu		: 1;
    /* cmpxchg8b instruction */
    jit_uint32_t cmpxchg8b	: 1;
    /* cmov and fcmov branchless conditional mov */
    jit_uint32_t cmov		: 1;
    /* mmx registers/instructions available */
    jit_uint32_t mmx		: 1;
    /* sse registers/instructions available */
    jit_uint32_t sse		: 1;
    /* sse2 registers/instructions available */
    jit_uint32_t sse2		: 1;
    /* sse3 instructions available */
    jit_uint32_t sse3		: 1;
    /* pcmulqdq instruction */
    jit_uint32_t pclmulqdq	: 1;
    /* ssse3 suplemental sse3 instructions available */
    jit_uint32_t ssse3		: 1;
    /* fused multiply/add using ymm state */
    jit_uint32_t fma		: 1;
    /* cmpxchg16b instruction */
    jit_uint32_t cmpxchg16b	: 1;
    /* sse4.1 instructions available */
    jit_uint32_t sse4_1		: 1;
    /* sse4.2 instructions available */
    jit_uint32_t sse4_2		: 1;
    /* movbe instruction available */
    jit_uint32_t movbe		: 1;
    /* popcnt instruction available */
    jit_uint32_t popcnt		: 1;
    /* aes instructions available */
    jit_uint32_t aes		: 1;
    /* avx instructions available */
    jit_uint32_t avx		: 1;
    /* lahf/sahf available in 64 bits mode */
    jit_uint32_t lahf		: 1;
} jit_cpu_t;

/*
 * Initialization
 */
extern jit_cpu_t		jit_cpu;

#endif /* _jit_x86_h */
