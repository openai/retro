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

#ifndef _jit_aarch64_h
#define _jit_aarch64_h

#define JIT_HASH_CONSTS		0
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
#define JIT_FP			_R29
typedef enum {
#define jit_r(i)		(_R9 + (i))
#define jit_r_num()		7
#define jit_v(i)		(_R19 + (i))
#define jit_v_num()		10
#define jit_f(i)		(_V8 + (i))
#define jit_f_num()		8
#define JIT_R0			_R9
#define JIT_R1			_R10
#define JIT_R2			_R11
#define JIT_R3			_R12
#define JIT_R4			_R13
#define JIT_R5			_R14
#define JIT_R6			_R15
    _R8,				/* indirect result */
    _R18,				/* platform register */
    _R17,				/* IP1 */
    _R16,				/* IP0 */
    _R9,	_R10,	_R11,	_R12,	/* temporaries */
    _R13,	_R14,	_R15,
#define JIT_V0			_R19
#define JIT_V1			_R20
#define JIT_V2			_R21
#define JIT_V3			_R22
#define JIT_V4			_R23
#define JIT_V5			_R24
#define JIT_V6			_R25
#define JIT_V7			_R26
#define JIT_V8			_R27
#define JIT_V9			_R28
    _R19,	_R20,	_R21,	_R22,	/* callee save */
    _R23,	_R24,	_R25,	_R26,
    _R27,	_R28,
    _SP,				/* stack pointer */
    _R30,				/* link register */
    _R29,				/* frame pointer */
    _R7,	_R6,	_R5,	_R4,
    _R3,	_R2,	_R1,	_R0,
#define JIT_F0			_V8
#define JIT_F1			_V9
#define JIT_F2			_V10
#define JIT_F3			_V11
#define JIT_F4			_V12
#define JIT_F5			_V13
#define JIT_F6			_V14
#define JIT_F7			_V15
    _V31,	_V30,	_V29,	_V28,	/* temporaries */
    _V27,	_V26,	_V25,	_V24,
    _V23,	_V22,	_V21,	_V20,
    _V19,	_V18,	_V17,	_V16,
    /* callee save */
    _V8,	_V9,	_V10,	_V11,
    _V12,	_V13,	_V14,	_V15,
    _V7,	_V6,	_V5,	_V4,	/* arguments */
    _V3,	_V2,	_V1,	_V0,
    _NOREG,
#define JIT_NOREG		_NOREG
} jit_reg_t;

#endif /* _jit_aarch64_h */
