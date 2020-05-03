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

#ifndef _jit_mips_h
#define _jit_mips_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	3

#if _MIPS_SIM != _ABIO32
#    define NEW_ABI		1
#endif

/*
 * Types
 */
#define JIT_FP			_FP
typedef enum {
#define jit_r(i)		(_V0 + (i))
#if NEW_ABI
#  define jit_r_num()		7
#else
#  define jit_r_num()		11
#endif
#define jit_v(i)		(_S0 + (i))
#define jit_v_num()		8
#define jit_f(i)		(_F0 + (i))
#if NEW_ABI
#  define jit_f_num()		6
#else
#  define jit_f_num()		8
#endif
    _AT,
#define JIT_R0			_V0
#define JIT_R1			_V1
#if NEW_ABI
#  define JIT_R2		_T4
#  define JIT_R3		_T5
#  define JIT_R4		_T6
#  define JIT_R5		_T7
#  define JIT_R6		_T8
#else
#  define JIT_R2		_T0
#  define JIT_R3		_T1
#  define JIT_R4		_T2
#  define JIT_R5		_T3
#  define JIT_R6		_T4
#  define JIT_R7		_T5
#  define JIT_R8		_T6
#  define JIT_R9		_T7
#  define JIT_R10		_T8
#endif
    _V0, _V1,
#if !NEW_ABI
    _T0, _T1, _T2, _T3,
#endif
    _T4, _T5, _T6, _T7, _T8, _T9,
#define JIT_V0			_S0
#define JIT_V1			_S1
#define JIT_V2			_S2
#define JIT_V3			_S3
#define JIT_V4			_S4
#define JIT_V5			_S5
#define JIT_V6			_S6
#define JIT_V7			_S7
    _S0, _S1, _S2, _S3, _S4, _S5, _S6, _S7,
    _ZERO, _K0, _K1, _RA,
    _GP,
    _SP, _FP,
#if NEW_ABI
    _A7, _A6, _A5, _A4,
#endif
    _A3, _A2, _A1, _A0,
#define JIT_F0			_F0
#define JIT_F1			_F2
#define JIT_F2			_F4
#define JIT_F3			_F6
#define JIT_F4			_F8
#define JIT_F5			_F10
#if !NEW_ABI
#  define JIT_F6		_F16
#  define JIT_F7		_F18
#endif
    _F0, _F2, _F4, _F6, _F8, _F10,
    /* callee save float registers */
#if !NEW_ABI
    _F16, _F18,
#endif
    _F20, _F22, _F24, _F26, _F28, _F30,
#if NEW_ABI
    _F19, _F18, _F17, _F16, _F15, _F14, _F13, _F12,
#else
    _F14, _F12,
#endif
#define JIT_NOREG		_NOREG
    _NOREG,
} jit_reg_t;

#endif /* _jit_mips_h */
