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

#ifndef _jit_ppc_h
#define _jit_ppc_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
typedef enum {
#define jit_r(i)		(_R28 + (i))
#define jit_r_num()		3
#define jit_v(i)		(_R27 - (i))
#define jit_v_num()		14
#define jit_f(i)		(_F14 + (i))
#define jit_f_num()		8
    _R0,
#define JIT_R0			_R28
#define JIT_R1			_R29
#define JIT_R2			_R30
    _R11,	_R12,	_R13,	_R2,
#define JIT_V0			jit_v(0)
#define JIT_V1			jit_v(1)
#define JIT_V2			jit_v(2)
#define JIT_V3			jit_v(3)
#define JIT_V4			jit_v(4)
#define JIT_V5			jit_v(5)
#define JIT_V6			jit_v(6)
#define JIT_V7			jit_v(7)
#define JIT_V8			jit_v(8)
#define JIT_V9			jit_v(9)
#define JIT_V10			jit_v(10)
#define JIT_V11			jit_v(11)
#define JIT_V12			jit_v(12)
#define JIT_V13			jit_v(13)
    _R14,	_R15,	_R16,	_R17,	_R18,	_R19,	_R20,	_R21,
    _R22,	_R23,	_R24,	_R25,	_R26,	_R27,	_R28,	_R29,
    _R30,
    _R1,
#define JIT_FP			_R31
    _R31,
    _R10,	_R9,	_R8,	_R7,	_R6,	_R5,	_R4,	_R3,
    _F0,
    _F14,	_F15,	_F16,	_F17,	_F18,	_F19,	_F20,	_F21,
#define JIT_F0			_F14
#define JIT_F1			_F15
#define JIT_F2			_F16
#define JIT_F3			_F17
#define JIT_F4			_F18
#define JIT_F5			_F19
#define JIT_F6			_F20
#define JIT_F7			_F21
    /* FIXME _F20-_F31 not (easily) accessible and only _F14-_F21
     * saved/restored (if used) */
    _F22,	_F23,	_F24,	_F25,	_F26,	_F27,	_F28,	_F29,
    _F30,	_F31,
    _F13,	_F12,	_F11,	_F10,	_F9,	_F8,	_F7,	_F6,
    _F5,	_F4,	_F3,	_F2,	_F1,
    _NOREG,
#define JIT_NOREG		_NOREG
} jit_reg_t;

#endif /* _jit_ppc_h */
