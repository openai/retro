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

#ifndef _jit_arm_h
#define _jit_arm_h

#define JIT_HASH_CONSTS		0
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
#define jit_swf_p()		(jit_cpu.vfp == 0)
#define jit_hardfp_p()		jit_cpu.abi
#define jit_ldrt_strt_p()	jit_cpu.ldrt_strt

#define JIT_FP			_R11
typedef enum {
#define jit_r(i)		(_R4 + (i))
#define jit_r_num()		3
#define jit_v(i)		(_R7 + (i))
#define jit_v_num()		3
#define jit_f(i)		(jit_cpu.abi ? _D8 + ((i)<<1) : _D0 - ((i)<<1))
#define jit_f_num()		8
    _R12,			/* ip - temporary */
#define JIT_R0			_R4
#define JIT_R1			_R5
#define JIT_R2			_R6
    _R4,			/* r4 - variable */
    _R5,			/* r5 - variable */
    _R6,			/* r6 - variable */
#define JIT_V0			_R7
#define JIT_V1			_R8
#define JIT_V2			_R9
    _R7,			/* r7 - variable */
    _R8,			/* r8 - variable */
    _R9,			/* r9 - variable */
    _R10,			/* sl - stack limit */
    _R11,			/* fp - frame pointer */
    _R13,			/* sp - stack pointer */
    _R14,			/* lr - link register */
    _R15,			/* pc - program counter */
    _R3,			/* r3 - argument/result */
    _R2,			/* r2 - argument/result */
    _R1,			/* r1 - argument/result */
    _R0,			/* r0 - argument/result */
#define JIT_F0			(jit_hardfp_p() ? _D8 : _D0)
#define JIT_F1			(jit_hardfp_p() ? _D9 : _D1)
#define JIT_F2			(jit_hardfp_p() ? _D10 : _D2)
#define JIT_F3			(jit_hardfp_p() ? _D11 : _D3)
#define JIT_F4			(jit_hardfp_p() ? _D12 : _D4)
#define JIT_F5			(jit_hardfp_p() ? _D13 : _D5)
#define JIT_F6			(jit_hardfp_p() ? _D14 : _D6)
#define JIT_F7			(jit_hardfp_p() ? _D15 : _D7)
    _S16,	_D8 = _S16,	_Q4 = _D8,
    _S17,
    _S18,	_D9 = _S18,
    _S19,
    _S20,	_D10 = _S20,	_Q5 = _D10,
    _S21,
    _S22,	_D11 = _S22,
    _S23,
    _S24,	_D12 = _S24,	_Q6 = _D12,
    _S25,
    _S26,	_D13 = _S26,
    _S27,
    _S28,	_D14 = _S28,	_Q7 = _D14,
    _S29,
    _S30,	_D15 = _S30,
    _S31,
    _S15,
    _S14,	_D7 = _S14,
    _S13,
    _S12,	_D6 = _S12,	_Q3 = _D6,
    _S11,
    _S10,	_D5 = _S10,
    _S9,
    _S8,	_D4 = _S8,	_Q2 = _D4,
    _S7,
    _S6,	_D3 = _S6,
    _S5,
    _S4,	_D2 = _S4,	_Q1 = _D2,
    _S3,
    _S2,	_D1 = _S2,
    _S1,
    _S0,	_D0 = _S0,	_Q0 = _D0,
    _NOREG,
#define JIT_NOREG		_NOREG
} jit_reg_t;

typedef struct {
    jit_uint32_t version	: 4;
    jit_uint32_t extend		: 1;
    /* only generate thumb instructions for thumb2 */
    jit_uint32_t thumb		: 1;
    jit_uint32_t vfp		: 3;
    jit_uint32_t neon		: 1;
    jit_uint32_t abi		: 2;
    /* use strt+offset instead of str.w?
     * on special cases it causes a SIGILL at least on qemu, probably
     * due to some memory ordering constraint not being respected, so,
     * disable by default */
    jit_uint32_t ldrt_strt	: 1;
} jit_cpu_t;

/*
 * Initialization
 */
extern jit_cpu_t		jit_cpu;

#endif /* _jit_arm_h */
