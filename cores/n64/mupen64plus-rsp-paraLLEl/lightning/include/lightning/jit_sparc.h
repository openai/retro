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

#ifndef _jit_sparc_h
#define _jit_sparc_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
#define JIT_FP			_FP
typedef enum {
#define jit_r(i)		(_G2 + (i))
#define jit_r_num()		3
#define jit_v(i)		(_L0 + (i))
#define jit_v_num()		8
#if __WORDSIZE == 32
#  define jit_f(i)		(_F0 + ((i) << 1))
#  define jit_f_num()		8
#else
#  define jit_f(i)		(_F32 - (i))
#  define jit_f_num()		16
#endif
#define JIT_R0			_G2
#define JIT_R1			_G3
#define JIT_R2			_G4
#define JIT_V0			_L0
#define JIT_V1			_L1
#define JIT_V2			_L2
#define JIT_V3			_L3
#define JIT_V4			_L4
#define JIT_V5			_L5
#define JIT_V6			_L6
#define JIT_V7			_L7
    _G0, _G1, _G2, _G3, _G4, _G5, _G6, _G7,
    _O0, _O1, _O2, _O3, _O4, _O5, _SP, _O7,
    _L0, _L1, _L2, _L3, _L4, _L5, _L6, _L7,
    _I0, _I1, _I2, _I3, _I4, _I5, _FP, _I7,
#if __WORDSIZE == 32
#  define JIT_F0		_F0
#  define JIT_F1		_F2
#  define JIT_F2		_F4
#  define JIT_F3		_F6
#  define JIT_F4		_F8
#  define JIT_F5		_F10
#  define JIT_F6		_F12
#  define JIT_F7		_F14
    _F0, _F1,  _F2,  _F3,  _F4,  _F5,  _F6,  _F7,
    _F8, _F9, _F10, _F11, _F12, _F13, _F14, _F15,
#else
    /* All single precision operations have a high cost due to being
     * stored on registers only encodable as double precision.
     * The cost is due to needing to move values to a register with
     * value <= 31.
     * This is a limitation due to using fixed named registers in
     * lightning. */
#  define JIT_F0		_F32
#  define JIT_F1		_F34
#  define JIT_F2		_F36
#  define JIT_F3		_F38
#  define JIT_F4		_F40
#  define JIT_F5		_F42
#  define JIT_F6		_F44
#  define JIT_F7		_F46
#  define JIT_F8		_F48
#  define JIT_F9		_F50
#  define JIT_F10		_F52
#  define JIT_F11		_F54
#  define JIT_F12		_F56
#  define JIT_F13		_F58
#  define JIT_F14		_F60
#  define JIT_F15		_F62
    _F62, _F60, _F58, _F56, _F54, _F52, _F50, _F48,
    _F46, _F44, _F42, _F40, _F38, _F36, _F34, _F32,
    _F31, _F30, _F29, _F28, _F27, _F26, _F25, _F24,
    _F23, _F22, _F21, _F20, _F19, _F18, _F17, _F16,
    _F15, _F14, _F13, _F12, _F11, _F10,  _F9,  _F8,
     _F7,  _F6,  _F5,  _F4,  _F3,  _F2,  _F1,  _F0,
#endif
#define JIT_NOREG		_NOREG
    _NOREG,
} jit_reg_t;

#endif /* _jit_sparc_h */
