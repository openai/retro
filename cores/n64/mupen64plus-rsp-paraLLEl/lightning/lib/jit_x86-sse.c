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

#if PROTO
#  if __X32
#    define sse_address_p(i0)		1
#  else
#    if __X64_32
#      define sse_address_p(i0)		((jit_word_t)(i0) >= 0)
#    else
#      define sse_address_p(i0)		can_sign_extend_int_p(i0)
#    endif
#  endif
#  define _XMM6_REGNO			6
#  define _XMM7_REGNO			7
#  define _XMM8_REGNO			8
#  define _XMM9_REGNO			9
#  define _XMM10_REGNO			10
#  define _XMM11_REGNO			11
#  define _XMM12_REGNO			12
#  define _XMM13_REGNO			13
#  define _XMM14_REGNO			14
#  define _XMM15_REGNO			15
#define X86_SSE_MOV			0x10
#define X86_SSE_MOV1			0x11
#define X86_SSE_MOVLP			0x12
#define X86_SSE_MOVHP			0x16
#define X86_SSE_MOVA			0x28
#define X86_SSE_CVTIS			0x2a
#define X86_SSE_CVTTSI			0x2c
#define X86_SSE_CVTSI			0x2d
#define X86_SSE_UCOMI			0x2e
#define X86_SSE_COMI			0x2f
#define X86_SSE_ROUND			0x3a
#define X86_SSE_SQRT			0x51
#define X86_SSE_RSQRT			0x52
#define X86_SSE_RCP			0x53
#define X86_SSE_AND			0x54
#define X86_SSE_ANDN			0x55
#define X86_SSE_OR			0x56
#define X86_SSE_XOR			0x57
#define X86_SSE_ADD			0x58
#define X86_SSE_MUL			0x59
#define X86_SSE_CVTSD			0x5a
#define X86_SSE_CVTDT			0x5b
#define X86_SSE_SUB			0x5c
#define X86_SSE_MIN			0x5d
#define X86_SSE_DIV			0x5e
#define X86_SSE_MAX			0x5f
#define X86_SSE_X2G			0x6e
#define X86_SSE_EQB			0x74
#define X86_SSE_EQW			0x75
#define X86_SSE_EQD			0x76
#define X86_SSE_G2X			0x7e
#define X86_SSE_MOV2			0xd6
#  define sser(c,r0,r1)			_sser(_jit,c,r0,r1)
static void _sser(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ssexr(p,c,r0,r1)		_ssexr(_jit,p,c,r0,r1)
static void _ssexr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ssexi(c,r0,m,i)		_ssexi(_jit,c,r0,m,i)
static void _ssexi(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addssr(r0, r1)		ssexr(0xf3, X86_SSE_ADD, r0, r1)
#  define addsdr(r0, r1)		ssexr(0xf2, X86_SSE_ADD, r0, r1)
#  define subssr(r0, r1)		ssexr(0xf3, X86_SSE_SUB, r0, r1)
#  define subsdr(r0, r1)		ssexr(0xf2, X86_SSE_SUB, r0, r1)
#  define mulssr(r0, r1)		ssexr(0xf3, X86_SSE_MUL, r0, r1)
#  define mulsdr(r0, r1)		ssexr(0xf2, X86_SSE_MUL, r0, r1)
#  define divssr(r0, r1)		ssexr(0xf3, X86_SSE_DIV, r0, r1)
#  define divsdr(r0, r1)		ssexr(0xf2, X86_SSE_DIV, r0, r1)
#  define andpsr(r0, r1)		sser(       X86_SSE_AND, r0, r1)
#  define andpdr(r0, r1)		ssexr(0x66, X86_SSE_AND, r0, r1)
#  define sse_truncr_f_i(r0, r1)	ssexr(0xf3, X86_SSE_CVTTSI, r0, r1)
#  define sse_truncr_d_i(r0, r1)	ssexr(0xf2, X86_SSE_CVTTSI, r0, r1)
#  if __X64
#    define sse_truncr_f_l(r0, r1)	sselxr(0xf3, X86_SSE_CVTTSI, r0, r1)
#    define sse_truncr_d_l(r0, r1)	sselxr(0xf2, X86_SSE_CVTTSI, r0, r1)
#    define sse_extr_f(r0, r1)		sselxr(0xf3, X86_SSE_CVTIS, r0, r1)
#    define sse_extr_d(r0, r1)		sselxr(0xf2, X86_SSE_CVTIS, r0, r1)
#  else
#    define sse_extr_f(r0, r1)		ssexr(0xf3, X86_SSE_CVTIS, r0, r1)
#    define sse_extr_d(r0, r1)		ssexr(0xf2, X86_SSE_CVTIS, r0, r1)
#  endif
#  define sse_extr_f_d(r0, r1)		ssexr(0xf3, X86_SSE_CVTSD, r0, r1)
#  define sse_extr_d_f(r0, r1)		ssexr(0xf2, X86_SSE_CVTSD, r0, r1)
#  define ucomissr(r0,r1)		sser(X86_SSE_UCOMI,r0,r1)
#  define ucomisdr(r0,r1)		ssexr(0x66,X86_SSE_UCOMI,r0,r1)
#  define xorpsr(r0,r1)			sser(X86_SSE_XOR,r0,r1)
#  define xorpdr(r0,r1)			ssexr(0x66,X86_SSE_XOR,r0,r1)
#  define movdlxr(r0,r1)		ssexr(0x66, X86_SSE_X2G,r0,r1)
#  define pcmpeqlr(r0, r1)		ssexr(0x66, X86_SSE_EQD, r0, r1)
#  define psrl(r0, i0)			ssexi(0x72, r0, 0x02, i0)
#  define psrq(r0, i0)			ssexi(0x73, r0, 0x02, i0)
#  define psll(r0, i0)			ssexi(0x72, r0, 0x06, i0)
#  define pslq(r0, i0)			ssexi(0x73, r0, 0x06, i0)
#  define movdqxr(r0,r1)		sselxr(0x66,X86_SSE_X2G,r0,r1)
#  if __X64 && !__X64_32
#    define sselxr(p,c,r0,r1)		_sselxr(_jit,p,c,r0,r1)
static void
_sselxr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  else
#    define sselxr(p,c,r0,r1)		ssexr(p,c,r0,r1)
#  endif
#  define ssexrx(p,c,md,rb,ri,ms,rd)	_ssexrx(_jit,p,c,md,rb,ri,ms,rd)
#  define movssmr(md,rb,ri,ms,rd)	ssexrx(0xf3,X86_SSE_MOV,md,rb,ri,ms,rd)
#  define movsdmr(md,rb,ri,ms,rd)	ssexrx(0xf2,X86_SSE_MOV,md,rb,ri,ms,rd)
#  define movssrm(rs,md,mb,mi,ms)	ssexrx(0xf3,X86_SSE_MOV1,md,mb,mi,ms,rs)
#  define movsdrm(rs,md,mb,mi,ms)	ssexrx(0xf2,X86_SSE_MOV1,md,mb,mi,ms,rs)
static void
_ssexrx(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t,
	jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_addr_f(r0, r1, r2)	_sse_addr_f(_jit, r0, r1, r2)
static void _sse_addr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_addi_f(r0, r1, i0)	_sse_addi_f(_jit, r0, r1, i0)
static void _sse_addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_addr_d(r0, r1, r2)	_sse_addr_d(_jit, r0, r1, r2)
static void _sse_addr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_addi_d(r0, r1, i0)	_sse_addi_d(_jit, r0, r1, i0)
static void _sse_addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_subr_f(r0, r1, r2)	_sse_subr_f(_jit, r0, r1, r2)
static void _sse_subr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_subi_f(r0, r1, i0)	_sse_subi_f(_jit, r0, r1, i0)
static void _sse_subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_subr_d(r0, r1, r2)	_sse_subr_d(_jit, r0, r1, r2)
static void _sse_subr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_subi_d(r0, r1, i0)	_sse_subi_d(_jit, r0, r1, i0)
static void _sse_subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_rsbr_f(r0, r1, r2)	sse_subr_f(r0, r2, r1)
#  define sse_rsbi_f(r0, r1, i0)	_sse_rsbi_f(_jit, r0, r1, i0)
static void _sse_rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_rsbr_d(r0, r1, r2)	sse_subr_d(r0, r2, r1)
#  define sse_rsbi_d(r0, r1, i0)	_sse_rsbi_d(_jit, r0, r1, i0)
static void _sse_rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_mulr_f(r0, r1, r2)	_sse_mulr_f(_jit, r0, r1, r2)
static void _sse_mulr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_muli_f(r0, r1, i0)	_sse_muli_f(_jit, r0, r1, i0)
static void _sse_muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_mulr_d(r0, r1, r2)	_sse_mulr_d(_jit, r0, r1, r2)
static void _sse_mulr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_muli_d(r0, r1, i0)	_sse_muli_d(_jit, r0, r1, i0)
static void _sse_muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_divr_f(r0, r1, r2)	_sse_divr_f(_jit, r0, r1, r2)
static void _sse_divr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_divi_f(r0, r1, i0)	_sse_divi_f(_jit, r0, r1, i0)
static void _sse_divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_divr_d(r0, r1, r2)	_sse_divr_d(_jit, r0, r1, r2)
static void _sse_divr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_divi_d(r0, r1, i0)	_sse_divi_d(_jit, r0, r1, i0)
static void _sse_divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_absr_f(r0, r1)		_sse_absr_f(_jit, r0, r1)
static void _sse_absr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sse_absr_d(r0, r1)		_sse_absr_d(_jit, r0, r1)
static void _sse_absr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sse_negr_f(r0, r1)		_sse_negr_f(_jit, r0, r1)
static void _sse_negr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sse_negr_d(r0, r1)		_sse_negr_d(_jit, r0, r1)
static void _sse_negr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sse_sqrtr_f(r0, r1)		ssexr(0xf3, X86_SSE_SQRT, r0, r1)
#  define sse_sqrtr_d(r0, r1)		ssexr(0xf2, X86_SSE_SQRT, r0, r1)
#  define ssecmpf(code, r0, r1, r2)	_ssecmp(_jit, 0, code, r0, r1, r2)
#  define ssecmpd(code, r0, r1, r2)	_ssecmp(_jit, 1, code, r0, r1, r2)
static void
_ssecmp(jit_state_t*, jit_bool_t, jit_int32_t,
	jit_int32_t, jit_int32_t, jit_int32_t);
#define sse_movr_f(r0,r1)		_sse_movr_f(_jit,r0,r1)
static void _sse_movr_f(jit_state_t*, jit_int32_t, jit_int32_t);
#define sse_movi_f(r0,i0)		_sse_movi_f(_jit,r0,i0)
static void _sse_movi_f(jit_state_t*, jit_int32_t, jit_float32_t*);
#  define sse_lti_f(r0, r1, i0)		_sse_lti_f(_jit, r0, r1, i0)
static void _sse_lti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_ltr_f(r0, r1, r2)		ssecmpf(X86_CC_A, r0, r1, r2)
#  define sse_lei_f(r0, r1, i0)		_sse_lei_f(_jit, r0, r1, i0)
static void _sse_lei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_ler_f(r0, r1, r2)		ssecmpf(X86_CC_AE, r0, r1, r2)
#  define sse_eqi_f(r0, r1, i0)		_sse_eqi_f(_jit, r0, r1, i0)
static void _sse_eqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_eqr_f(r0, r1, r2)		_sse_eqr_f(_jit, r0, r1, r2)
static void _sse_eqr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_gei_f(r0, r1, i0)		_sse_gei_f(_jit, r0, r1, i0)
static void _sse_gei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_ger_f(r0, r1, r2)		ssecmpf(X86_CC_AE, r0, r2, r1)
#  define sse_gti_f(r0, r1, i0)		_sse_gti_f(_jit, r0, r1, i0)
static void _sse_gti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_gtr_f(r0, r1, r2)		ssecmpf(X86_CC_A, r0, r2, r1)
#  define sse_nei_f(r0, r1, i0)		_sse_nei_f(_jit, r0, r1, i0)
static void _sse_nei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_ner_f(r0, r1, r2)		_sse_ner_f(_jit, r0, r1, r2)
static void _sse_ner_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_unlti_f(r0, r1, i0)	_sse_unlti_f(_jit, r0, r1, i0)
static void _sse_unlti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_unltr_f(r0, r1, r2)	ssecmpf(X86_CC_NAE, r0, r2, r1)
#  define sse_unlei_f(r0, r1, i0)	_sse_unlei_f(_jit, r0, r1, i0)
static void _sse_unlei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_unler_f(r0, r1, r2)	_sse_unler_f(_jit, r0, r1, r2)
#  define sse_uneqi_f(r0, r1, i0)	_sse_uneqi_f(_jit, r0, r1, i0)
static void _sse_uneqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
static void _sse_unler_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_uneqr_f(r0, r1, r2)	_sse_uneqr_f(_jit, r0, r1, r2)
static void _sse_uneqr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_ungei_f(r0, r1, i0)	_sse_ungei_f(_jit, r0, r1, i0)
static void _sse_ungei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_unger_f(r0, r1, r2)	_sse_unger_f(_jit, r0, r1, r2)
static void _sse_unger_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_ungti_f(r0, r1, i0)	_sse_ungti_f(_jit, r0, r1, i0)
static void _sse_ungti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_ungtr_f(r0, r1, r2)	ssecmpf(X86_CC_NAE, r0, r1, r2)
#  define sse_ltgti_f(r0, r1, i0)	_sse_ltgti_f(_jit, r0, r1, i0)
static void _sse_ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_ltgtr_f(r0, r1, r2)	_sse_ltgtr_f(_jit, r0, r1, r2)
static void _sse_ltgtr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_ordi_f(r0, r1, i0)	_sse_ordi_f(_jit, r0, r1, i0)
static void _sse_ordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_ordr_f(r0, r1, r2)	ssecmpf(X86_CC_NP, r0, r2, r1)
#  define sse_unordi_f(r0, r1, i0)	_sse_unordi_f(_jit, r0, r1, i0)
static void _sse_unordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define sse_unordr_f(r0, r1, r2)	ssecmpf(X86_CC_P, r0, r2, r1)
#  define sse_ldr_f(r0, r1)		movssmr(0, r1, _NOREG, _SCL1, r0)
#  define sse_ldi_f(r0, i0)		_sse_ldi_f(_jit, r0, i0)
static void _sse_ldi_f(jit_state_t*, jit_int32_t, jit_word_t);
#  define sse_ldxr_f(r0, r1, r2)	_sse_ldxr_f(_jit, r0, r1, r2)
static void _sse_ldxr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_ldxi_f(r0, r1, i0)	_sse_ldxi_f(_jit, r0, r1, i0)
static void _sse_ldxi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define sse_str_f(r0, r1)		movssrm(r1, 0, r0, _NOREG, _SCL1)
#  define sse_sti_f(i0, r0)		_sse_sti_f(_jit, i0, r0)
static void _sse_sti_f(jit_state_t*, jit_word_t,jit_int32_t);
#  define sse_stxr_f(r0, r1, r2)	_sse_stxr_f(_jit, r0, r1, r2)
static void _sse_stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_stxi_f(i0, r0, r1)	_sse_stxi_f(_jit, i0, r0, r1)
static void _sse_stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bltr_f(i0, r0, r1)	_sse_bltr_f(_jit, i0, r0, r1)
static jit_word_t _sse_bltr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_blti_f(i0, r0, i1)	_sse_blti_f(_jit, i0, r0, i1)
static jit_word_t
_sse_blti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bler_f(i0, r0, r1)	_sse_bler_f(_jit, i0, r0, r1)
static jit_word_t _sse_bler_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_blei_f(i0, r0, i1)	_sse_blei_f(_jit, i0, r0, i1)
static jit_word_t
_sse_blei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_beqr_f(i0, r0, r1)	_sse_beqr_f(_jit, i0, r0, r1)
static jit_word_t _sse_beqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_beqi_f(i0, r0, i1)	_sse_beqi_f(_jit, i0, r0, i1)
static jit_word_t
_sse_beqi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bger_f(i0, r0, r1)	_sse_bger_f(_jit, i0, r0, r1)
static jit_word_t _sse_bger_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bgei_f(i0, r0, i1)	_sse_bgei_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bgei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bgtr_f(i0, r0, r1)	_sse_bgtr_f(_jit, i0, r0, r1)
static jit_word_t _sse_bgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bgti_f(i0, r0, i1)	_sse_bgti_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bgti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bner_f(i0, r0, r1)	_sse_bner_f(_jit, i0, r0, r1)
static jit_word_t _sse_bner_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bnei_f(i0, r0, i1)	_sse_bnei_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bnei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bunltr_f(i0, r0, r1)	_sse_bunltr_f(_jit, i0, r0, r1)
static jit_word_t _sse_bunltr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bunlti_f(i0, r0, i1)	_sse_bunlti_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bunlti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bunler_f(i0, r0, r1)	_sse_bunler_f(_jit, i0, r0, r1)
static jit_word_t _sse_bunler_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bunlei_f(i0, r0, i1)	_sse_bunlei_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bunlei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_buneqr_f(i0, r0, r1)	_sse_buneqr_f(_jit, i0, r0, r1)
static jit_word_t _sse_buneqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_buneqi_f(i0, r0, i1)	_sse_buneqi_f(_jit, i0, r0, i1)
static jit_word_t
_sse_buneqi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bunger_f(i0, r0, r1)	_sse_bunger_f(_jit, i0, r0, r1)
static jit_word_t _sse_bunger_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bungei_f(i0, r0, i1)	_sse_bungei_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bungei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bungtr_f(i0, r0, r1)	_sse_bungtr_f(_jit, i0, r0, r1)
static jit_word_t _sse_bungtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bungti_f(i0, r0, i1)	_sse_bungti_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bungti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bltgtr_f(i0, r0, r1)	_sse_bltgtr_f(_jit, i0, r0, r1)
static jit_word_t _sse_bltgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bltgti_f(i0, r0, i1)	_sse_bltgti_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bltgti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bordr_f(i0, r0, r1)	_sse_bordr_f(_jit, i0, r0, r1)
static jit_word_t _sse_bordr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bordi_f(i0, r0, i1)	_sse_bordi_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bordi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define sse_bunordr_f(i0, r0, r1)	_sse_bunordr_f(_jit, i0, r0, r1)
static jit_word_t _sse_bunordr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bunordi_f(i0, r0, i1)	_sse_bunordi_f(_jit, i0, r0, i1)
static jit_word_t
_sse_bunordi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#define sse_movr_d(r0,r1)		_sse_movr_d(_jit,r0,r1)
static void _sse_movr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#define sse_movi_d(r0,i0)		_sse_movi_d(_jit,r0,i0)
static void _sse_movi_d(jit_state_t*, jit_int32_t, jit_float64_t*);
#  define sse_ltr_d(r0, r1, r2)		ssecmpd(X86_CC_A, r0, r1, r2)
#  define sse_lti_d(r0, r1, i0)		_sse_lti_d(_jit, r0, r1, i0)
static void _sse_lti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_ler_d(r0, r1, r2)		ssecmpd(X86_CC_AE, r0, r1, r2)
#  define sse_lei_d(r0, r1, i0)		_sse_lei_d(_jit, r0, r1, i0)
static void _sse_lei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_eqr_d(r0, r1, r2)		_sse_eqr_d(_jit, r0, r1, r2)
static void _sse_eqr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_eqi_d(r0, r1, i0)		_sse_eqi_d(_jit, r0, r1, i0)
static void _sse_eqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_ger_d(r0, r1, r2)		ssecmpd(X86_CC_AE, r0, r2, r1)
#  define sse_gei_d(r0, r1, i0)		_sse_gei_d(_jit, r0, r1, i0)
static void _sse_gei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_gtr_d(r0, r1, r2)		ssecmpd(X86_CC_A, r0, r2, r1)
#  define sse_gti_d(r0, r1, i0)		_sse_gti_d(_jit, r0, r1, i0)
static void _sse_gti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_ner_d(r0, r1, r2)		_sse_ner_d(_jit, r0, r1, r2)
static void _sse_ner_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_nei_d(r0, r1, i0)		_sse_nei_d(_jit, r0, r1, i0)
static void _sse_nei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_unltr_d(r0, r1, r2)	ssecmpd(X86_CC_NAE, r0, r2, r1)
#  define sse_unlti_d(r0, r1, i0)	_sse_unlti_d(_jit, r0, r1, i0)
static void _sse_unlti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_unler_d(r0, r1, r2)	_sse_unler_d(_jit, r0, r1, r2)
static void _sse_unler_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_unlei_d(r0, r1, i0)	_sse_unlei_d(_jit, r0, r1, i0)
static void _sse_unlei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_uneqr_d(r0, r1, r2)	_sse_uneqr_d(_jit, r0, r1, r2)
static void _sse_uneqr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_uneqi_d(r0, r1, i0)	_sse_uneqi_d(_jit, r0, r1, i0)
static void _sse_uneqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_unger_d(r0, r1, r2)	_sse_unger_d(_jit, r0, r1, r2)
static void _sse_unger_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_ungei_d(r0, r1, i0)	_sse_ungei_d(_jit, r0, r1, i0)
static void _sse_ungei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_ungtr_d(r0, r1, r2)	ssecmpd(X86_CC_NAE, r0, r1, r2)
#  define sse_ungti_d(r0, r1, i0)	_sse_ungti_d(_jit, r0, r1, i0)
static void _sse_ungti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_ltgtr_d(r0, r1, r2)	_sse_ltgtr_d(_jit, r0, r1, r2)
static void _sse_ltgtr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_ltgti_d(r0, r1, i0)	_sse_ltgti_d(_jit, r0, r1, i0)
static void _sse_ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_ordr_d(r0, r1, r2)	ssecmpd(X86_CC_NP, r0, r2, r1)
#  define sse_ordi_d(r0, r1, i0)	_sse_ordi_d(_jit, r0, r1, i0)
static void _sse_ordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_unordr_d(r0, r1, r2)	ssecmpd(X86_CC_P, r0, r2, r1)
#  define sse_unordi_d(r0, r1, i0)	_sse_unordi_d(_jit, r0, r1, i0)
static void _sse_unordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define sse_ldr_d(r0, r1)		movsdmr(0, r1, _NOREG, _SCL1, r0)
#  define sse_ldi_d(r0, i0)		_sse_ldi_d(_jit, r0, i0)
static void _sse_ldi_d(jit_state_t*, jit_int32_t, jit_word_t);
#  define sse_ldxr_d(r0, r1, r2)	_sse_ldxr_d(_jit, r0, r1, r2)
static void _sse_ldxr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define sse_ldxi_d(r0, r1, i0)	_sse_ldxi_d(_jit, r0, r1, i0)
static void _sse_ldxi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define sse_bltr_d(i0, r0, r1)	_sse_bltr_d(_jit, i0, r0, r1)
#  define sse_str_d(r0, r1)		movsdrm(r1, 0, r0, _NOREG, _SCL1)
#  define sse_sti_d(i0, r0)		_sse_sti_d(_jit, i0, r0)
static void _sse_sti_d(jit_state_t*, jit_word_t,jit_int32_t);
#  define sse_stxr_d(r0, r1, r2)	_sse_stxr_d(_jit, r0, r1, r2)
static void _sse_stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define sse_stxi_d(i0, r0, r1)	_sse_stxi_d(_jit, i0, r0, r1)
static void _sse_stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _sse_bltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_blti_d(i0, r0, i1)	_sse_blti_d(_jit, i0, r0, i1)
static jit_word_t
_sse_blti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bler_d(i0, r0, r1)	_sse_bler_d(_jit, i0, r0, r1)
static jit_word_t _sse_bler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_blei_d(i0, r0, i1)	_sse_blei_d(_jit, i0, r0, i1)
static jit_word_t
_sse_blei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_beqr_d(i0, r0, r1)	_sse_beqr_d(_jit, i0, r0, r1)
static jit_word_t _sse_beqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_beqi_d(i0, r0, i1)	_sse_beqi_d(_jit, i0, r0, i1)
static jit_word_t
_sse_beqi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bger_d(i0, r0, r1)	_sse_bger_d(_jit, i0, r0, r1)
static jit_word_t _sse_bger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bgei_d(i0, r0, i1)	_sse_bgei_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bgei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bgtr_d(i0, r0, r1)	_sse_bgtr_d(_jit, i0, r0, r1)
static jit_word_t _sse_bgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bgti_d(i0, r0, i1)	_sse_bgti_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bgti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bner_d(i0, r0, r1)	_sse_bner_d(_jit, i0, r0, r1)
static jit_word_t _sse_bner_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bnei_d(i0, r0, i1)	_sse_bnei_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bnei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bunltr_d(i0, r0, r1)	_sse_bunltr_d(_jit, i0, r0, r1)
static jit_word_t _sse_bunltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bunlti_d(i0, r0, i1)	_sse_bunlti_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bunlti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bunler_d(i0, r0, r1)	_sse_bunler_d(_jit, i0, r0, r1)
static jit_word_t _sse_bunler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bunlei_d(i0, r0, i1)	_sse_bunlei_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bunlei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_buneqr_d(i0, r0, r1)	_sse_buneqr_d(_jit, i0, r0, r1)
static jit_word_t _sse_buneqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_buneqi_d(i0, r0, i1)	_sse_buneqi_d(_jit, i0, r0, i1)
static jit_word_t
_sse_buneqi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bunger_d(i0, r0, r1)	_sse_bunger_d(_jit, i0, r0, r1)
static jit_word_t _sse_bunger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bungei_d(i0, r0, i1)	_sse_bungei_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bungei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bungtr_d(i0, r0, r1)	_sse_bungtr_d(_jit, i0, r0, r1)
static jit_word_t _sse_bungtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bungti_d(i0, r0, i1)	_sse_bungti_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bungti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bltgtr_d(i0, r0, r1)	_sse_bltgtr_d(_jit, i0, r0, r1)
static jit_word_t _sse_bltgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bltgti_d(i0, r0, i1)	_sse_bltgti_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bltgti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bordr_d(i0, r0, r1)	_sse_bordr_d(_jit, i0, r0, r1)
static jit_word_t _sse_bordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bordi_d(i0, r0, i1)	_sse_bordi_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bordi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define sse_bunordr_d(i0, r0, r1)	_sse_bunordr_d(_jit, i0, r0, r1)
static jit_word_t _sse_bunordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define sse_bunordi_d(i0, r0, i1)	_sse_bunordi_d(_jit, i0, r0, i1)
static jit_word_t
_sse_bunordi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#endif

#if CODE
#  define fpr_opi(name, type, size)					\
static void								\
_sse_##name##i_##type(jit_state_t *_jit,				\
		      jit_int32_t r0, jit_int32_t r1,			\
		      jit_float##size##_t *i0)				\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|jit_class_xpr);	\
    assert(jit_sse_reg_p(reg));						\
    sse_movi_##type(rn(reg), i0);					\
    sse_##name##r_##type(r0, r1, rn(reg));				\
    jit_unget_reg(reg);							\
}
#  define fpr_bopi(name, type, size)					\
static jit_word_t							\
_sse_b##name##i_##type(jit_state_t *_jit,				\
		       jit_word_t i0, jit_int32_t r0,			\
		       jit_float##size##_t *i1)				\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|jit_class_xpr|	\
					  jit_class_nospill);		\
    assert(jit_sse_reg_p(reg));						\
    sse_movi_##type(rn(reg), i1);					\
    word = sse_b##name##r_##type(i0, r0, rn(reg));			\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define fopi(name)			fpr_opi(name, f, 32)
#  define fbopi(name)			fpr_bopi(name, f, 32)
#  define dopi(name)			fpr_opi(name, d, 64)
#  define dbopi(name)			fpr_bopi(name, d, 64)
static void
_sser(jit_state_t *_jit, jit_int32_t c, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, 0, r0, 0, r1);
    ic(0x0f);
    ic(c);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_ssexr(jit_state_t *_jit, jit_int32_t p, jit_int32_t c,
       jit_int32_t r0, jit_int32_t r1)
{
    ic(p);
    rex(0, 0, r0, 0, r1);
    ic(0x0f);
    ic(c);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_ssexi(jit_state_t *_jit, jit_int32_t c, jit_int32_t r0,
       jit_int32_t m, jit_int32_t i)
{
    ic(0x66);
    rex(0, 0, 0, 0, r0);
    ic(0x0f);
    ic(c);
    mrm(0x03, r7(m), r7(r0));
    ic(i);
}

#if __X64
static void
_sselxr(jit_state_t *_jit, jit_int32_t p, jit_int32_t c,
	jit_int32_t r0, jit_int32_t r1)
{
    ic(p);
    rex(0, 1, r0, 0, r1);
    ic(0x0f);
    ic(c);
    mrm(0x03, r7(r0), r7(r1));
}
#endif

static void
_ssexrx(jit_state_t *_jit, jit_int32_t px, jit_int32_t code, jit_int32_t md,
       jit_int32_t rb, jit_int32_t ri, jit_int32_t ms, jit_int32_t rd)
{
    ic(px);
    rex(0, 0, rd, ri, rb);
    ic(0x0f);
    ic(code);
    rx(rd, md, rb, ri, ms);
}

static void
_sse_addr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1)
	addssr(r0, r2);
    else if (r0 == r2)
	addssr(r0, r1);
    else {
	sse_movr_f(r0, r1);
	addssr(r0, r2);
    }
}

fopi(add)

static void
_sse_addr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1)
	addsdr(r0, r2);
    else if (r0 == r2)
	addsdr(r0, r1);
    else {
	sse_movr_d(r0, r1);
	addsdr(r0, r2);
    }
}

dopi(add)

static void
_sse_subr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r1)
	subssr(r0, r2);
    else if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	sse_movr_f(rn(reg), r0);
	sse_movr_f(r0, r1);
	subssr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	sse_movr_f(r0, r1);
	subssr(r0, r2);
    }
}

fopi(sub)

static void
_sse_subr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r1)
	subsdr(r0, r2);
    else if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	sse_movr_d(rn(reg), r0);
	sse_movr_d(r0, r1);
	subsdr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	sse_movr_d(r0, r1);
	subsdr(r0, r2);
    }
}

dopi(sub)

fopi(rsb)

dopi(rsb)

static void
_sse_mulr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1)
	mulssr(r0, r2);
    else if (r0 == r2)
	mulssr(r0, r1);
    else {
	sse_movr_f(r0, r1);
	mulssr(r0, r2);
    }
}

fopi(mul)

static void
_sse_mulr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1)
	mulsdr(r0, r2);
    else if (r0 == r2)
	mulsdr(r0, r1);
    else {
	sse_movr_d(r0, r1);
	mulsdr(r0, r2);
    }
}

dopi(mul)

static void
_sse_divr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r1)
	divssr(r0, r2);
    else if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	sse_movr_f(rn(reg), r0);
	sse_movr_f(r0, r1);
	divssr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	sse_movr_f(r0, r1);
	divssr(r0, r2);
    }
}

fopi(div)

static void
_sse_divr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r1)
	divsdr(r0, r2);
    else if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	sse_movr_d(rn(reg), r0);
	sse_movr_d(r0, r1);
	divsdr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	sse_movr_d(r0, r1);
	divsdr(r0, r2);
    }
}

dopi(div)

static void
_sse_absr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (r0 == r1) {
	reg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	pcmpeqlr(rn(reg), rn(reg));
	psrl(rn(reg), 1);
	andpsr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	pcmpeqlr(r0, r0);
	psrl(r0, 1);
	andpsr(r0, r1);
    }
}

static void
_sse_absr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (r0 == r1) {
	reg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	pcmpeqlr(rn(reg), rn(reg));
	psrq(rn(reg), 1);
	andpdr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	pcmpeqlr(r0, r0);
	psrq(r0, 1);
	andpdr(r0, r1);
    }
}

static void
_sse_negr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		freg, ireg;
    ireg = jit_get_reg(jit_class_gpr);
    imovi(rn(ireg), 0x80000000);
    if (r0 == r1) {
	freg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	movdlxr(rn(freg), rn(ireg));
	xorpsr(r0, rn(freg));
	jit_unget_reg(freg);
    }
    else {
	movdlxr(r0, rn(ireg));
	xorpsr(r0, r1);
    }
    jit_unget_reg(ireg);
}

static void
_sse_negr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		freg, ireg;
    ireg = jit_get_reg(jit_class_gpr);
    imovi(rn(ireg), 0x80000000);
    if (r0 == r1) {
	freg = jit_get_reg(jit_class_fpr|jit_class_xpr);
	movdlxr(rn(freg), rn(ireg));
	pslq(rn(freg), 32);
	xorpdr(r0, rn(freg));
	jit_unget_reg(freg);
    }
    else {
	movdlxr(r0, rn(ireg));
	pslq(r0, 32);
	xorpdr(r0, r1);
    }
    jit_unget_reg(ireg);
}

static void
_ssecmp(jit_state_t *_jit, jit_bool_t d, jit_int32_t code,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t		rc;
    jit_int32_t		reg;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, reg);
    }
    ixorr(reg, reg);
    if (d)
	ucomisdr(r2, r1);
    else
	ucomissr(r2, r1);
    cc(code, reg);
    if (!rc)
	xchgr(r0, reg);
}

static void
_sse_movr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	ssexr(0xf3, X86_SSE_MOV, r0, r1);
}

static void
_sse_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t *i0)
{
    union {
	jit_int32_t	 i;
	jit_float32_t	 f;
    } data;
    jit_int32_t		 reg;
    jit_bool_t		 ldi;

    data.f = *i0;
    if (data.f == 0.0 && !(data.i & 0x80000000))
	xorpsr(r0, r0);
    else {
	ldi = !_jitc->no_data;
#if __X64
	/* if will allocate a register for offset, just use immediate */
	if (ldi && !sse_address_p(i0))
	    ldi = 0;
#endif
	if (ldi)
	    sse_ldi_f(r0, (jit_word_t)i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), data.i);
	    movdlxr(r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

fopi(lt)
fopi(le)

static void
_sse_eqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t		rc;
    jit_int32_t		reg;
    jit_word_t		jp_code;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, _RAX_REGNO);
    }
    ixorr(reg, reg);
    ucomissr(r2, r1);
    jpes(0);
    jp_code = _jit->pc.w;
    cc(X86_CC_E, reg);
    patch_rel_char(jp_code, _jit->pc.w);
    if (!rc)
	xchgr(r0, reg);
}

fopi(eq)
fopi(ge)
fopi(gt)

static void
_sse_ner_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t		rc;
    jit_int32_t		reg;
    jit_word_t		jp_code;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, _RAX_REGNO);
    }
    imovi(reg, 1);
    ucomissr(r2, r1);
    jpes(0);
    jp_code = _jit->pc.w;
    cc(X86_CC_NE, reg);
    patch_rel_char(jp_code, _jit->pc.w);
    if (!rc)
	xchgr(r0, reg);
}

fopi(ne)
fopi(unlt)

static void
_sse_unler_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	ssecmpf(X86_CC_NA, r0, r2, r1);
}

fopi(unle)

static void
_sse_uneqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	ssecmpf(X86_CC_E, r0, r1, r2);
}

fopi(uneq)

static void
_sse_unger_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	ssecmpf(X86_CC_NA, r0, r1, r2);
}

fopi(unge)
fopi(ungt)

static void
_sse_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	ixorr(r0, r0);
    else
	ssecmpf(X86_CC_NE, r0, r1, r2);
}

fopi(ltgt)
fopi(ord)
fopi(unord)

static void
_sse_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (sse_address_p(i0))
	movssmr(i0, _NOREG, _NOREG, _SCL1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	sse_ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sse_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    sse_ldr_f(r0, rn(reg));
    jit_unget_reg(reg);
#else
    movssmr(0, r1, r2, _SCL1, r0);
#endif
}

static void
_sse_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0))
	movssmr(i0, r1, _NOREG, _SCL1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r1, i0);
	sse_ldr_f(r0, rn(reg));
#else
	movi(rn(reg), i0);
	sse_ldxr_f(r0, r1, rn(reg));
#endif
	jit_unget_reg(reg);
    }
}

static void
_sse_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (sse_address_p(i0))
	movssrm(r0, i0, _NOREG, _NOREG, _SCL1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	sse_str_f(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sse_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    sse_str_f(rn(reg), r2);
    jit_unget_reg(reg);
#else
    movssrm(r2, 0, r0, r1, _SCL1);
#endif
}

static void
_sse_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0))
	movssrm(r1, i0, r0, _NOREG, _SCL1);
    else {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r0, i0);
	sse_str_f(rn(reg), r1);
#else
	movi(rn(reg), i0);
	sse_stxr_f(rn(reg), r0, r1);
#endif
	jit_unget_reg(reg);
    }
}

static jit_word_t
_sse_bltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r1, r0);
    ja(i0);
    return (_jit->pc.w);
}
fbopi(lt)

static jit_word_t
_sse_bler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r1, r0);
    jae(i0);
    return (_jit->pc.w);
}
fbopi(le)

static jit_word_t
_sse_beqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		jp_code;
    ucomissr(r0, r1);
    jps(0);
    jp_code = _jit->pc.w;
    je(i0);
    patch_rel_char(jp_code, _jit->pc.w);
    return (_jit->pc.w);
}
fbopi(eq)

static jit_word_t
_sse_bger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r0, r1);
    jae(i0);
    return (_jit->pc.w);
}
fbopi(ge)

static jit_word_t
_sse_bgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r0, r1);
    ja(i0);
    return (_jit->pc.w);
}
fbopi(gt)

static jit_word_t
_sse_bner_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		jp_code;
    jit_word_t		jz_code;
    ucomissr(r0, r1);
    jps(0);
    jp_code = _jit->pc.w;
    jzs(0);
    jz_code = _jit->pc.w;
    patch_rel_char(jp_code, _jit->pc.w);
    jmpi(i0);
    patch_rel_char(jz_code, _jit->pc.w);
    return (_jit->pc.w);
}
fbopi(ne)

static jit_word_t
_sse_bunltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r0, r1);
    jnae(i0);
    return (_jit->pc.w);
}
fbopi(unlt)

static jit_word_t
_sse_bunler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1)
	jmpi(i0);
    else {
	ucomissr(r0, r1);
	jna(i0);
    }
    return (_jit->pc.w);
}
fbopi(unle)

static jit_word_t
_sse_buneqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1)
	jmpi(i0);
    else {
	ucomissr(r0, r1);
	je(i0);
    }
    return (_jit->pc.w);
}
fbopi(uneq)

static jit_word_t
_sse_bunger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1)
	jmpi(i0);
    else {
	ucomissr(r1, r0);
	jna(i0);
    }
    return (_jit->pc.w);
}
fbopi(unge)

static jit_word_t
_sse_bungtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r1, r0);
    jnae(i0);
    return (_jit->pc.w);
}
fbopi(ungt)

static jit_word_t
_sse_bltgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r0, r1);
    jne(i0);
    return (_jit->pc.w);
}
fbopi(ltgt)

static jit_word_t
_sse_bordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r0, r1);
    jnp(i0);
    return (_jit->pc.w);
}
fbopi(ord)

static jit_word_t
_sse_bunordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomissr(r0, r1);
    jp(i0);
    return (_jit->pc.w);
}
fbopi(unord)

dopi(lt)
dopi(le)

static void
_sse_eqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t		rc;
    jit_int32_t		reg;
    jit_word_t		jp_code;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, _RAX_REGNO);
    }
    ixorr(reg, reg);
    ucomisdr(r2, r1);
    jpes(0);
    jp_code = _jit->pc.w;
    cc(X86_CC_E, reg);
    patch_rel_char(jp_code, _jit->pc.w);
    if (!rc)
	xchgr(r0, reg);
}

dopi(eq)
dopi(ge)
dopi(gt)

static void
_sse_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t		rc;
    jit_int32_t		reg;
    jit_word_t		jp_code;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, _RAX_REGNO);
    }
    imovi(reg, 1);
    ucomisdr(r2, r1);
    jpes(0);
    jp_code = _jit->pc.w;
    cc(X86_CC_NE, reg);
    patch_rel_char(jp_code, _jit->pc.w);
    if (!rc)
	xchgr(r0, reg);
}

dopi(ne)
dopi(unlt)

static void
_sse_unler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	ssecmpd(X86_CC_NA, r0, r2, r1);
}

dopi(unle)

static void
_sse_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	ssecmpd(X86_CC_E, r0, r1, r2);
}

dopi(uneq)

static void
_sse_unger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	ssecmpd(X86_CC_NA, r0, r1, r2);
}

dopi(unge)
dopi(ungt)

static void
_sse_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	ixorr(r0, r0);
    else
	ssecmpd(X86_CC_NE, r0, r1, r2);
}

dopi(ltgt)
dopi(ord)
dopi(unord)

static void
_sse_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	ssexr(0xf2, X86_SSE_MOV, r0, r1);
}

static void
_sse_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
	jit_int32_t	 ii[2];
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;
    jit_bool_t		 ldi;

    data.d = *i0;
    if (data.d == 0.0 && !(data.ii[1] & 0x80000000))
	xorpdr(r0, r0);
    else {
	ldi = !_jitc->no_data;
#if __X64
	/* if will allocate a register for offset, just use immediate */
	if (ldi && !sse_address_p(i0))
	    ldi = 0;
#endif
	if (ldi)
	    sse_ldi_d(r0, (jit_word_t)i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
#if __X64 && !__X64_32
	    movi(rn(reg), data.w);
	    movdqxr(r0, rn(reg));
	    jit_unget_reg(reg);
#else
	    movi(rn(reg), data.ii[0]);
	    stxi_i(CVT_OFFSET, _RBP_REGNO, rn(reg));
	    movi(rn(reg), data.ii[1]);
	    stxi_i(CVT_OFFSET + 4, _RBP_REGNO, rn(reg));
	    jit_unget_reg(reg);
	    sse_ldxi_d(r0, _RBP_REGNO, CVT_OFFSET);
#endif
	}
    }
}

static void
_sse_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (sse_address_p(i0))
	movsdmr(i0, _NOREG, _NOREG, _SCL1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	sse_ldr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sse_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    sse_ldr_d(r0, rn(reg));
    jit_unget_reg(reg);
#else
    movsdmr(0, r1, r2, _SCL1, r0);
#endif
}

static void
_sse_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0))
	movsdmr(i0, r1, _NOREG, _SCL1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r1, i0);
	sse_ldr_d(r0, rn(reg));
#else
	movi(rn(reg), i0);
	sse_ldxr_d(r0, r1, rn(reg));
#endif
	jit_unget_reg(reg);
    }
}

static void
_sse_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (sse_address_p(i0))
	movsdrm(r0, i0, _NOREG, _NOREG, _SCL1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	sse_str_d(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sse_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    sse_str_d(rn(reg), r2);
    jit_unget_reg(reg);
#else
    movsdrm(r2, 0, r0, r1, _SCL1);
#endif
}

static void
_sse_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0))
	movsdrm(r1, i0, r0, _NOREG, _SCL1);
    else {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r0, i0);
	sse_str_d(rn(reg), r1);
#else
	movi(rn(reg), i0);
	sse_stxr_f(rn(reg), r0, r1);
#endif
	jit_unget_reg(reg);
    }
}

static jit_word_t
_sse_bltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r1, r0);
    ja(i0);
    return (_jit->pc.w);
}
dbopi(lt)

static jit_word_t
_sse_bler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r1, r0);
    jae(i0);
    return (_jit->pc.w);
}
dbopi(le)

static jit_word_t
_sse_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		jp_code;
    ucomisdr(r0, r1);
    jps(0);
    jp_code = _jit->pc.w;
    je(i0);
    patch_rel_char(jp_code, _jit->pc.w);
    return (_jit->pc.w);
}
dbopi(eq)

static jit_word_t
_sse_bger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r0, r1);
    jae(i0);
    return (_jit->pc.w);
}
dbopi(ge)

static jit_word_t
_sse_bgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r0, r1);
    ja(i0);
    return (_jit->pc.w);
}
dbopi(gt)

static jit_word_t
_sse_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		jp_code;
    jit_word_t		jz_code;
    ucomisdr(r0, r1);
    jps(0);
    jp_code = _jit->pc.w;
    jzs(0);
    jz_code = _jit->pc.w;
    patch_rel_char(jp_code, _jit->pc.w);
    jmpi(i0);
    patch_rel_char(jz_code, _jit->pc.w);
    return (_jit->pc.w);
}
dbopi(ne)

static jit_word_t
_sse_bunltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r0, r1);
    jnae(i0);
    return (_jit->pc.w);
}
dbopi(unlt)

static jit_word_t
_sse_bunler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1)
	jmpi(i0);
    else {
	ucomisdr(r0, r1);
	jna(i0);
    }
    return (_jit->pc.w);
}
dbopi(unle)

static jit_word_t
_sse_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1)
	jmpi(i0);
    else {
	ucomisdr(r0, r1);
	je(i0);
    }
    return (_jit->pc.w);
}
dbopi(uneq)

static jit_word_t
_sse_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1)
	jmpi(i0);
    else {
	ucomisdr(r1, r0);
	jna(i0);
    }
    return (_jit->pc.w);
}
dbopi(unge)

static jit_word_t
_sse_bungtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r1, r0);
    jnae(i0);
    return (_jit->pc.w);
}
dbopi(ungt)

static jit_word_t
_sse_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r0, r1);
    jne(i0);
    return (_jit->pc.w);
}
dbopi(ltgt)

static jit_word_t
_sse_bordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r0, r1);
    jnp(i0);
    return (_jit->pc.w);
}
dbopi(ord)

static jit_word_t
_sse_bunordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    ucomisdr(r0, r1);
    jp(i0);
    return (_jit->pc.w);
}
dbopi(unord)
#  undef fopi
#  undef fbopi
#  undef bopi
#  undef dbopi
#  undef fpr_bopi
#  undef fpr_opi
#endif
