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

#ifndef _jit_s390_h
#define _jit_s390_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	2

/*
 * Types
 */
#define JIT_FP			_R13
typedef enum {
#define jit_r(i)		(_R12 + ((i) << 1))
#define jit_r_num()		3
#define jit_v(i)		(_R11 + ((i) << 1))
#define jit_v_num()		3
#define jit_f(i)		(_F8 + (i))
#define jit_f_num()		6
#define JIT_R0			_R12
#define JIT_R1			_R10
#define JIT_R2			_R8
#define JIT_V0			_R11
#define JIT_V1			_R9
#define JIT_V2			_R7
     _R0,  _R1,			/* Volatile */
    _R12,			/* Saved, GOT */
    _R11,  _R10,  _R9,  _R8,	/* Saved */
     _R7,			/* Saved */
     _R6,			/* Saved, parameter */
     _R5,  _R4,  _R3,		/* Parameter passing */
     _R2,			/* Volatile, parameter and return value */
    _R13,			/* Saved, literal pool pointer */
    _R14,			/* Volatile, return address */
    _R15,			/* Saved, stack pointer */
#define JIT_F0			_F8
#define JIT_F1			_F9
#define JIT_F2			_F10
#define JIT_F3			_F11
#define JIT_F4			_F12
#define JIT_F5			_F13
     _F1,  _F3,  _F5,  _F7,	/* Volatile */
    _F14, _F15,	 _F8,  _F9,	/* Saved */
    _F10, _F11, _F12, _F13,	/* Saved */
     _F6,  _F4,  _F2,		/* Volatile, parameter */
     _F0,			/* Volatile, parameter and return value */
    _NOREG,
#define JIT_NOREG		_NOREG
} jit_reg_t;

#endif /* _jit_s390_h */
