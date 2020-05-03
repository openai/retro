/*
 * Copyright (C) 2014-2019  Free Software Foundation, Inc.
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

#ifndef _jit_alpha_h
#define _jit_alpha_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
#define JIT_FP			_FP
typedef enum {
#define jit_r(i)		(_S0 + (i))
#define jit_r_num()		3
#define jit_v(i)		(_S3 + (i))
#define jit_v_num()		3
#define jit_f(i)		(_F2 + (i))
#define jit_f_num()		8
    /* Volatile - Assembly temporary register */
    _AT,
    /* Volatile - Return value register */
    _V0,
    /* Volatile - Temporary registers */
    _T0,		_T1,		_T2,		_T3,
    _T4,		_T5,		_T6,		_T7,
    _T8,		_T9,		_T10,		_T11,
    /* FIXME Use callee save register for r0-r2 (instead of 12+ JIT_RN
     * and 6 JIT_VN because division must call a function)
     * FIX would be to create proper functions that do not clobber
     * registers and inject/inline them in the jit */
#define JIT_R0			_S0
#define JIT_R1			_S1
#define JIT_R2			_S2
#define JIT_V0			_S3
#define JIT_V1			_S4
#define JIT_V2			_S5
    /* Nonvolatile - Saved registers */
    _S0,		_S1,		_S2,		_S3,
    _S4,		_S5,
    /* Nonvolatile - Frame pointer */
    _FP,
    /* Volatile - Argument registers */
    _A5, 		_A4,		_A3,		_A2,
    _A1,		_A0,
    /* Volatile - Return address register */
    _RA,
    /* Volatile - Temporary register */
    _PV,
    /* Nonvolatile - Global pointer */
    _GP,
    /* Nonvolatile - Stack pointer */
    _SP,
    /* Constant  RAZ / writes ignored */
    _ZERO,
#define JIT_F0			_F2
#define JIT_F1			_F3
#define JIT_F2			_F4
#define JIT_F3			_F5
#define JIT_F4			_F6
#define JIT_F5			_F7
#define JIT_F6			_F8
#define JIT_F7			_F9
    /* Volatile - Return value register (real part) */
    _F0,
    /* Volatile - Return value register (imaginary part) */
    _F1,
    /* Nonvolatile - Saved registers */
    _F2,		_F3,		_F4,		_F5,
    _F6,		_F7,		_F8,		_F9,
    /* Volatile - Temporary registers */
    _F10,		_F11,		_F12,		_F13,
    _F14,		_F15,
    /* Volatile - Argument registers */
    _F21,		_F20,		_F19,		_F18,
    _F17,		_F16,
    /* Volatile - Temporary registers */
    _F22,		_F23,		_R24,		_F25,
    _F26,		_F27,		_F28,		_F29,
    _F30,
    /* Constant - RAZ / writes ignored */
    _F31,
    /* Lightning internal invalid register identifier */
    _NOREG,
#define JIT_NOREG		_NOREG
} jit_reg_t;

#endif /* _jit_alpha_h */
