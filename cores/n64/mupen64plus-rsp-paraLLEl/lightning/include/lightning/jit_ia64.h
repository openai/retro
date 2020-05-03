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

#ifndef _jit_ia64_h
#define _jit_ia64_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
#define JIT_FP			_R4	/* Not abi specific */
typedef enum {
#define JIT_R0		_R40
#define JIT_R1		_R41
#define JIT_R2		_R42
#define JIT_R3		_R43
#define JIT_V0		_R44
#define JIT_V1		_R45
#define JIT_V2		_R46
#define JIT_V3		_R47
#define jit_r_num()	4
#define jit_r(n)	(_R40 + (n))
#define jit_v_num()	4
#define jit_v(n)	(_R44 + (n))
    _R0,	/* constant - Always 0 */
    _R1,	/* special - Global Data pointer (gp) */
    /* r2-r3	 - scratch - Use with 22-bit immediate add - scratch */
    _R2,	_R3,
    /* r4-r7 - preserved */
    _R4,	_R5,	_R6,	_R7,
    _R8,	/* scratch - Return value; structure/union return pointer */
    /* r9-r11	 - scratch - Return values */
    _R9,	_R10,	_R11,
    _R12,	/* special - Memory stack pointer (sp) */
    _R13,	/* special - Reserved as a thread pointer (tp)*/
    /* r14-r31	 - scratch */
    _R31,	_R30,
    _R29,	_R28,	_R27,	_R26,	_R25,	_R24,	_R23,	_R22,
    _R21,	_R20,	_R19,	_R18,	_R17,	_R16,	_R15,	_R14,
    /* r32-r39	 - aka in0-in7 - Incoming register arguments */
    _R32,	_R33,	_R34,	_R35,	_R36,	_R37,	_R38,	_R39,
    /* r40-r127	 - loc0...locn,out0...outn */
    _R40,	_R41,	_R42,	_R43,	_R44,	_R45,	_R46,	_R47,
    _R48,	_R49,	_R50,	_R51,	_R52,	_R53,	_R54,	_R55,
    _R56,	_R57,	_R58,	_R59,	_R60,	_R61,	_R62,	_R63,
    _R64,	_R65,	_R66,	_R67,	_R68,	_R69,	_R70,	_R71,
    _R72,	_R73,	_R74,	_R75,	_R76,	_R77,	_R78,	_R79,
    _R80,	_R81,	_R82,	_R83,	_R84,	_R85,	_R86,	_R87,
    _R88,	_R89,	_R90,	_R91,	_R92,	_R93,	_R94,	_R95,
    _R96,	_R97,	_R98,	_R99,	_R100,	_R101,	_R102,	_R103,
    _R104,	_R105,	_R106,	_R107,	_R108,	_R109,	_R110,	_R111,
    _R112,	_R113,	_R114,	_R115,	_R116,	_R117,	_R118,	_R119,
    _R120,	_R121,	_R122,	_R123,	_R124,	_R125,	_R126,	_R127,
#define JIT_F0		_F16
#define JIT_F1		_F17
#define JIT_F2		_F18
#define JIT_F3		_F19
#define JIT_F4		_F20
#define JIT_F5		_F21
#define jit_f_num()	6
#define jit_f(n)	(_F16 + (n))
    _F0,	/* constant - Always 0.0 */
    _F1,	/* constant - Always 1.0 */
    /* f2-f5	 - preserved */
    _F2,	_F3,	_F4,	_F5,
    /* f6-f7	 - scratch */
    _F6,	_F7,
    /* f8-f15	 - scratch - Argument/return registers */
    _F8,	_F9,	_F10,	_F11,	_F12,	_F13,	_F14,	_F15,
    /* f16-f31	 - preserved */
    _F16,	_F17,	_F18,	_F19,	_F20,	_F21,	_F22,	_F23,
    _F24,	_F25,	_F26,	_F27,	_F28,	_F29,	_F30,	_F31,
    /* f32-f127	 - scratch - Rotating registers or scratch */
    _F32,	_F33,	_F34,	_F35,	_F36,	_F37,	_F38,	_F39,
    _F40,	_F41,	_F42,	_F43,	_F44,	_F45,	_F46,	_F47,
    _F48,	_F49,	_F50,	_F51,	_F52,	_F53,	_F54,	_F55,
    _F56,	_F57,	_F58,	_F59,	_F60,	_F61,	_F62,	_F63,
    _F64,	_F65,	_F66,	_F67,	_F68,	_F69,	_F70,	_F71,
    _F72,	_F73,	_F74,	_F75,	_F76,	_F77,	_F78,	_F79,
    _F80,	_F81,	_F82,	_F83,	_F84,	_F85,	_F86,	_F87,
    _F88,	_F89,	_F90,	_F91,	_F92,	_F93,	_F94,	_F95,
    _F96,	_F97,	_F98,	_F99,	_F100,	_F101,	_F102,	_F103,
    _F104,	_F105,	_F106,	_F107,	_F108,	_F109,	_F110,	_F111,
    _F112,	_F113,	_F114,	_F115,	_F116,	_F117,	_F118,	_F119,

#if 0
    /* Do not list these to not need an unique identifier larger
     * than 255 for jit_regset_t */
    _F120,	_F121,	_F122,	_F123,	_F124,	_F125,	_F126,	_F127,
#endif

    /* Fake registers. Required because while "in" parameters start at r32,
     * "out" parameters start *after* registers allocated for temporaries,
     * and that are supposed to kept alive (what is desirable, that is, to
      * not spill/reload them in memory) */
    _OUT0,	_OUT1,	_OUT2,	_OUT3,	_OUT4,	_OUT5,	_OUT6,	_OUT7,

#define JIT_NOREG		_NOREG
    _NOREG,
} jit_reg_t;

#endif /* _jit_ia64_h */
