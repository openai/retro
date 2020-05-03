/*
 * Copyright (C) 2019  Free Software Foundation, Inc.
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

#ifndef _jit_riscv_h
#define _jit_riscv_h

#define JIT_HASH_CONSTS		0
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
#define JIT_FP			_FP
typedef enum {
#define jit_r(i)		(JIT_R0 + (i))
#define jit_r_num()		7
#define jit_v(i)		(JIT_V0 + (i))
#define jit_v_num()		11
#define jit_f(i)		(JIT_F0 + (i))
#define jit_f_num()		12
    _ZERO,	/*  x0 - Hard-wired zero	---		*/
    _RA,	/*  x1 - Return address		(CalleR save)	*/
    _SP,	/*  x2 - Stack pointer		(CalleE save)	*/
    _GP,	/*  x3 - Global pointer		---		*/

#if 0		/* Pretend it does not exist, so _NOREG can be used in
		 * a 64 bit bitmask */
    _TP,	/*  x4 - Thread pointer		---		*/
#endif
    
#define JIT_R0		_T0
#define JIT_R1		_T1
#define JIT_R2		_T2
#define JIT_R3		_T3
#define JIT_R4		_T4
#define JIT_R5		_T5
#define JIT_R6		_T6
    _T0,	/*  x5 - Temporary/alternate
			 link register		(CalleR save)	*/
    _T1,	/*  x6 - Temporary		(CalleR save)	*/
    _T2,	/*  x7 - Temporary		(CalleR save)	*/
    _T3,	/* x28 - Temporary		(CalleR save)	*/
    _T4,	/* x28 - Temporary		(CalleR save)	*/
    _T5,	/* x30 - Temporary		(CalleR save)	*/
    _T6,	/* x31 - Temporary		(CalleR save)	*/
    _FP,	/*  x8 - Saved register/frame
			 pointer		(CalleE save)	*/
    _S0 = _FP,
#define JIT_V0		_S1
#define JIT_V1		_S2
#define JIT_V2		_S3
#define JIT_V3		_S4
#define JIT_V4		_S5
#define JIT_V5		_S6
#define JIT_V6		_S7
#define JIT_V7		_S8
#define JIT_V8		_S9
#define JIT_V9		_S10
#define JIT_V10		_S11
    _S1,	/*  x9 - Saved register		(CalleE save)	*/
    _S2,	/* x18 - Saved register		(CalleE save)	*/
    _S3,	/* x19 - Saved register		(CalleE save)	*/
    _S4,	/* x20 - Saved register		(CalleE save)	*/
    _S5,	/* x21 - Saved register		(CalleE save)	*/
    _S6,	/* x22 - Saved register		(CalleE save)	*/
    _S7,	/* x23 - Saved register		(CalleE save)	*/
    _S8,	/* x24 - Saved register		(CalleE save)	*/
    _S9,	/* x25 - Saved register		(CalleE save)	*/
    _S10,	/* x26 - Saved register		(CalleE save)	*/
    _S11,	/* x27 - Saved register		(CalleE save)	*/
    _A7,	/* x17 - Function argument	(CalleR save)	*/
    _A6,	/* x16 - Function argument	(CalleR save)	*/
    _A5,	/* x15 - Function argument	(CalleR save)	*/
    _A4,	/* x14 - Function argument	(CalleR save)	*/
    _A3,	/* x13 - Function argument	(CalleR save)	*/
    _A2,	/* x12 - Function argument	(CalleR save)	*/
    _A1,	/* x11 - Function argument/
			 return value		(CalleR save)	*/
    _A0,	/* x10 - Function argument/
			 return value		(CalleR save)	*/
    _FT0,	/*  f0 - FP temporary		(CalleR save)	*/
    _FT1,	/*  f1 - FP temporary		(CalleR save)	*/
    _FT2,	/*  f2 - FP temporary		(CalleR save)	*/
    _FT3,	/*  f3 - FP temporary		(CalleR save)	*/
    _FT4,	/*  f4 - FP temporary		(CalleR save)	*/
    _FT5,	/*  f5 - FP temporary		(CalleR save)	*/
    _FT6,	/*  f6 - FP temporary		(CalleR save)	*/
    _FT7,	/*  f7 - FP temporary		(CalleR save)	*/
    _FT8,	/* f28 - FP temporary		(CalleR save)	*/
    _FT9,	/* f29 - FP temporary		(CalleR save)	*/
    _FT10,	/* f30 - FP temporary		(CalleR save)	*/
    _FT11,	/* f31 - FP temporary		(CalleR save)	*/
#define JIT_F0		_FS0
#define JIT_F1		_FS1
#define JIT_F2		_FS2
#define JIT_F3		_FS3
#define JIT_F4		_FS4
#define JIT_F5		_FS5
#define JIT_F6		_FS6
#define JIT_F7		_FS7
#define JIT_F8		_FS8
#define JIT_F9		_FS9
#define JIT_F10		_FS10
#define JIT_F11		_FS11
    _FS0,	/*  f8 - FP saved register	(CalleE save)	*/
    _FS1,	/*  f9 - FP saved register	(CalleE save)	*/
    _FS2,	/* f18 - FP saved register	(CalleE save)	*/
    _FS3,	/* f19 - FP saved register	(CalleE save)	*/
    _FS4,	/* f20 - FP saved register	(CalleE save)	*/
    _FS5,	/* f21 - FP saved register	(CalleE save)	*/
    _FS6,	/* f22 - FP saved register	(CalleE save)	*/
    _FS7,	/* f23 - FP saved register	(CalleE save)	*/
    _FS8,	/* f24 - FP saved register	(CalleE save)	*/
    _FS9,	/* f25 - FP saved register	(CalleE save)	*/
    _FS10,	/* f26 - FP saved register	(CalleE save)	*/
    _FS11,	/* f27 - FP saved register	(CalleE save)	*/
    _FA7,	/* f17 - FP Function argument	(CalleR save)	*/
    _FA6,	/* f16 - FP Function argument	(CalleR save)	*/
    _FA5,	/* f15 - FP Function argument	(CalleR save)	*/
    _FA4,	/* f14 - FP Function argument	(CalleR save)	*/
    _FA3,	/* f13 - FP Function argument	(CalleR save)	*/
    _FA2,	/* f12 - FP Function argument	(CalleR save)	*/
    _FA1,	/* f11 - FP function argument/
			 return value		(CalleR save)	*/
    _FA0,	/* f10 - FP function argument/
			 return value		(CalleR save)	*/
    _NOREG,
#define JIT_NOREG		_NOREG
} jit_reg_t;

#endif /* _jit_riscv_h */
