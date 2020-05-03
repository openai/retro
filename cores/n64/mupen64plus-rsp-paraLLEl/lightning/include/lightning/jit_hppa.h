/*
 * Copyright (C) 2013-2019  Free Software Foundation, Inc.
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

#ifndef _jit_hppa_h
#define _jit_hppa_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
typedef enum {
    _R0,			/* Always zero */
    _R1,			/* ADDIL implicit target */
    _R2,			/* B,L implicit target */
#define jit_r_num()		4
#define jit_r(n)		((n) < 3 ? _R4 + (n) : _R10 + (n) - 3)
#define jit_v_num()		11
#define jit_v(n)		((n) < 3 ? _R7 + (n) : _R11 + (n) - 3)
#define jit_f_num()		8
#define jit_f(n)		(_F12 - (n))
#define JIT_FP			_R3
#define JIT_R0			_R4
#define JIT_R1			_R5
#define JIT_R2			_R6
#define JIT_R3			_R10
#define JIT_V0			_R7
#define JIT_V1			_R8
#define JIT_V2			_R9
#define JIT_V3			_R11
#define JIT_V4			_R12
#define JIT_V5			_R13
#define JIT_V6			_R14
#define JIT_V7			_R15
#define JIT_V8			_R16
#define JIT_V9			_R17
#define JIT_V10			_R18
    _R3,
    _R19,
    _R20,
    _R21,
    _R22,
    _R29,			/* ret1 */
    _R28,			/* ret0 */
    _R4,
    _R5,
    _R6,
    _R7,
    _R8,
    _R9,
    _R10,
    _R11,
    _R12,
    _R13,
    _R14,
    _R15,
    _R16,
    _R17,
    _R18,
    _R23,			/* arg3 */
    _R24,			/* arg2 */
    _R25,			/* arg1 */
    _R26,			/* arg0 */
    _R27,			/* Data Pointer */
    _R30,			/* Stack Pointer */
    _R31,			/* Link register */
#define JIT_F0			_F12
#define JIT_F1			_F13
#define JIT_F2			_F14
#define JIT_F3			_F15
#define JIT_F4			_F16
#define JIT_F5			_F17
#define JIT_F6			_F18
#define JIT_F7			_F19
#define JIT_F8			_F20
#define JIT_F9			_F21
    /* Caller Saves */
    _F31,
    _F30,
    _F29,
    _F28,
    _F27,
    _F26,
    _F25,
    _F24,
    _F23,
    _F22,
    _F11,
    _F10,
    _F9,
    _F8,
    /* Arguments */
    _F7,			/* farg3 */
    _F6,			/* farg2 */
    _F5,			/* farg1 */
    _F4,			/* farg0 */
    /* Callee Saves */
    _F21,
    _F20,
    _F19,
    _F18,
    _F17,
    _F16,
    _F15,
    _F14,
    _F13,
    _F12,
    /* Floating-Pointer Status and Exception */
    _F0,
    _F1,
    _F2,
    _F3,
#define JIT_NOREG		_NOREG
    _NOREG,
} jit_reg_t;

#endif /* _jit_hppa */
