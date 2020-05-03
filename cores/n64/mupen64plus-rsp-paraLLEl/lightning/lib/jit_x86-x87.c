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
#    define x87_address_p(i0)		1
#  else
#    if __X64_32
#      define x87_address_p(i0)		((jit_word_t)(i0) >= 0)
#    else
#      define x87_address_p(i0)		can_sign_extend_int_p(i0)
#    endif
#  endif
#  define _ST0_REGNO			0
#  define _ST1_REGNO			1
#  define _ST2_REGNO			2
#  define _ST3_REGNO			3
#  define _ST4_REGNO			4
#  define _ST5_REGNO			5
#  define _ST6_REGNO			6
#  define x87rx(code, md, rb, ri, ms)	_x87rx(_jit, code, md, rb, ri, ms)
#  define fldcwm(md, rb, ri, ms)	x87rx(015, md, rb, ri, ms)
#  define fstcwm(md, rb, ri, ms)	_fstcwm(_jit, md, rb, ri, ms)
static void
_fstcwm(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define fldsm(md, rb, ri, ms)		x87rx(010, md, rb, ri, ms)
#  define fstsm(md, rb, ri, ms)		x87rx(012, md, rb, ri, ms)
#  define fldlm(md, rb, ri, ms)		x87rx(050, md, rb, ri, ms)
#  define fstlm(md, rb, ri, ms)		x87rx(052, md, rb, ri, ms)
#  define fisttplm(md, rb, ri, ms)	x87rx(031, md, rb, ri, ms)
#  define fistlm(md, rb, ri, ms)	x87rx(032, md, rb, ri, ms)
#  define fisttpqm(md, rb, ri, ms)	x87rx(071, md, rb, ri, ms)
#  define fildlm(md, rb, ri, ms)	x87rx(030, md, rb,ri, ms)
#  define fildqm(md, rb, ri, ms)	x87rx(075, md, rb,ri, ms)
static void
_x87rx(jit_state_t*, jit_int32_t, jit_int32_t,
       jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87ri(cc,r0)			_x87ri(_jit,cc,r0)
#  define fchs_()			x87ri(014, 0)
#  define fabs_()			x87ri(014, 1)
#  define fld1()			x87ri(015, 0)
#  define fldl2t()			x87ri(015, 1)
#  define fldl2e()			x87ri(015, 2)
#  define fldpi()			x87ri(015, 3)
#  define fldlg2()			x87ri(015, 4)
#  define fldln2()			x87ri(015, 5)
#  define fldz()			x87ri(015, 6)
#  define fsqrt_()			x87ri(017, 2)
#  define fldr(r0)			x87ri(010, r0)
#  define fxchr(r0)			x87ri(011, r0)
#  define fstr(r0)			x87ri(052, r0)
#  define fstpr(r0)			x87ri(053, r0)
#  define fucomir(r0)			x87ri(035, r0)
#  define fucomipr(r0)			x87ri(075, r0)
static void _x87ri(jit_state_t*, jit_int32_t, jit_int32_t);
#  define faddr(r0, r1)			x87rri(000, r0, r1)
#  define fmulr(r0, r1)			x87rri(001, r0, r1)
#  define fsubr(r0, r1)			x87rri(004, r0, r1)
#  define fsubrr(r0, r1)		x87rri(005, r0, r1)
#  define fdivr(r0, r1)			x87rri(006, r0, r1)
#  define fdivrr(r0, r1)		x87rri(007, r0, r1)
#  define x87rri(cc, r0, r1)		_x87rri(_jit, cc, r0, r1)
static void _x87rri(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_addr_f(r0, r1, r2)	_x87_addr_d(_jit, r0, r1, r2)
#  define x87_addi_f(r0, r1, i0)	_x87_addi_f(_jit, r0, r1, i0)
static void _x87_addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_addr_d(r0, r1, r2)	_x87_addr_d(_jit, r0, r1, r2)
static void _x87_addr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_addi_d(r0, r1, i0)	_x87_addi_d(_jit, r0, r1, i0)
static void _x87_addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_subr_f(r0, r1, r2)	_x87_subr_d(_jit, r0, r1, r2)
#  define x87_subi_f(r0, r1, i0)	_x87_subi_f(_jit, r0, r1, i0)
static void _x87_subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_subr_d(r0, r1, r2)	_x87_subr_d(_jit, r0, r1, r2)
static void _x87_subr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_subi_d(r0, r1, i0)	_x87_subi_d(_jit, r0, r1, i0)
static void _x87_subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_rsbr_f(r0, r1, r2)	x87_subr_f(r0, r2, r1)
#  define x87_rsbi_f(r0, r1, i0)	_x87_rsbi_f(_jit, r0, r1, i0)
static void _x87_rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_rsbr_d(r0, r1, r2)	x87_subr_d(r0, r2, r1)
#  define x87_rsbi_d(r0, r1, i0)	_x87_rsbi_d(_jit, r0, r1, i0)
static void _x87_rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_mulr_f(r0, r1, r2)	_x87_mulr_d(_jit, r0, r1, r2)
#  define x87_muli_f(r0, r1, i0)	_x87_muli_f(_jit, r0, r1, i0)
static void _x87_muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_mulr_d(r0, r1, r2)	_x87_mulr_d(_jit, r0, r1, r2)
static void _x87_mulr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_muli_d(r0, r1, i0)	_x87_muli_d(_jit, r0, r1, i0)
static void _x87_muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_divr_f(r0, r1, r2)	_x87_divr_d(_jit, r0, r1, r2)
#  define x87_divi_f(r0, r1, i0)	_x87_divi_f(_jit, r0, r1, i0)
static void _x87_divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_divr_d(r0, r1, r2)	_x87_divr_d(_jit, r0, r1, r2)
static void _x87_divr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_divi_d(r0, r1, i0)	_x87_divi_d(_jit, r0, r1, i0)
static void _x87_divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_absr_f(r0, r1)		_x87_absr_d(_jit, r0, r1)
#  define x87_absr_d(r0, r1)		_x87_absr_d(_jit, r0, r1)
static void _x87_absr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define x87_negr_f(r0, r1)		_x87_negr_d(_jit, r0, r1)
#  define x87_negr_d(r0, r1)		_x87_negr_d(_jit, r0, r1)
static void _x87_negr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define x87_sqrtr_f(r0, r1)		_x87_sqrtr_d(_jit, r0, r1)
#  define x87_sqrtr_d(r0, r1)		_x87_sqrtr_d(_jit, r0, r1)
static void _x87_sqrtr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define x87_truncr_f_i(r0, r1)	_x87_truncr_d_i(_jit, r0, r1)
#  define x87_truncr_d_i(r0, r1)	_x87_truncr_d_i(_jit, r0, r1)
static void _x87_truncr_d_i(jit_state_t*, jit_int32_t, jit_int32_t);
#  if __X64
#    define x87_truncr_f_l(r0, r1)	_x87_truncr_d_l(_jit, r0, r1)
#    define x87_truncr_d_l(r0, r1)	_x87_truncr_d_l(_jit, r0, r1)
static void _x87_truncr_d_l(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  define x87_extr_f(r0, r1)		_x87_extr_d(_jit, r0, r1)
#  define x87_extr_d(r0, r1)		_x87_extr_d(_jit, r0, r1)
#  define x87_extr_f_d(r0, r1)		x87_movr_d(r0, r1)
#  define x87_extr_d_f(r0, r1)		x87_movr_d(r0, r1)
static void _x87_extr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define x87cmp(code, r0, r1, r2)	_x87cmp(_jit, code, r0, r1, r2)
static void
_x87cmp(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87cmp2(code, r0, r1, r2)	_x87cmp2(_jit, code, r0, r1, r2)
static void
_x87cmp2(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87jcc(code, i0, r0, r1)	_x87jcc(_jit, code, i0, r0, r1)
static jit_word_t
_x87jcc(jit_state_t*, jit_int32_t, jit_word_t, jit_int32_t, jit_int32_t);
#  define x87jcc2(code, i0, r0, r1)	_x87jcc2(_jit, code, i0, r0, r1)
static jit_word_t
_x87jcc2(jit_state_t*, jit_int32_t, jit_word_t, jit_int32_t, jit_int32_t);
#define x87_movi_f(r0,i0)		_x87_movi_f(_jit,r0,i0)
static void _x87_movi_f(jit_state_t*, jit_int32_t, jit_float32_t*);
#  define x87_ldr_f(r0, r1)		_x87_ldr_f(_jit, r0, r1)
static void _x87_ldr_f(jit_state_t*, jit_int32_t, jit_int32_t);
#  define x87_ldi_f(r0, i0)		_x87_ldi_f(_jit, r0, i0)
static void _x87_ldi_f(jit_state_t*, jit_int32_t, jit_word_t);
#  define x87_ldxr_f(r0, r1, r2)	_x87_ldxr_f(_jit, r0, r1, r2)
static void _x87_ldxr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_ldxi_f(r0, r1, i0)	_x87_ldxi_f(_jit, r0, r1, i0)
static void _x87_ldxi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define x87_str_f(r0, r1)		_x87_str_f(_jit, r0, r1)
static void _x87_str_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define x87_sti_f(i0, r0)		_x87_sti_f(_jit, i0, r0)
static void _x87_sti_f(jit_state_t*,jit_word_t, jit_int32_t);
#  define x87_stxr_f(r0, r1, r2)	_x87_stxr_f(_jit, r0, r1, r2)
static void _x87_stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define x87_stxi_f(i0, r0, r1)	_x87_stxi_f(_jit, i0, r0, r1)
static void _x87_stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define x87_ltr_f(r0, r1, r2)		x87cmp(X86_CC_A, r0, r2, r1)
#  define x87_lti_f(r0, r1, i0)		_x87_lti_f(_jit, r0, r1, i0)
static void _x87_lti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_ler_f(r0, r1, r2)		x87cmp(X86_CC_AE, r0, r2, r1)
#  define x87_lei_f(r0, r1, i0)		_x87_lei_f(_jit, r0, r1, i0)
static void _x87_lei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_eqr_f(r0, r1, r2)		x87_eqr_d(r0, r2, r1)
#  define x87_eqi_f(r0, r1, i0)		_x87_eqi_f(_jit, r0, r1, i0)
static void _x87_eqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_ger_f(r0, r1, r2)		x87cmp(X86_CC_AE, r0, r1, r2)
#  define x87_gei_f(r0, r1, i0)		_x87_gei_f(_jit, r0, r1, i0)
static void _x87_gei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_gtr_f(r0, r1, r2)		x87cmp(X86_CC_A, r0, r1, r2)
#  define x87_gti_f(r0, r1, i0)		_x87_gti_f(_jit, r0, r1, i0)
static void _x87_gti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_ner_f(r0, r1, r2)		x87_ner_d(r0, r2, r1)
#  define x87_nei_f(r0, r1, i0)		_x87_nei_f(_jit, r0, r1, i0)
static void _x87_nei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_unltr_f(r0, r1, r2)	x87cmp(X86_CC_NAE, r0, r1, r2)
#  define x87_unlti_f(r0, r1, i0)	_x87_unlti_f(_jit, r0, r1, i0)
static void _x87_unlti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_unler_f(r0, r1, r2)	x87cmp(X86_CC_NA, r0, r1, r2)
#  define x87_unlei_f(r0, r1, i0)	_x87_unlei_f(_jit, r0, r1, i0)
static void _x87_unlei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_uneqr_f(r0, r1, r2)	x87cmp2(X86_CC_E, r0, r1, r2)
#  define x87_uneqi_f(r0, r1, i0)	_x87_uneqi_f(_jit, r0, r1, i0)
static void _x87_uneqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_unger_f(r0, r1, r2)	x87cmp(X86_CC_NA, r0, r2, r1)
#  define x87_ungei_f(r0, r1, i0)	_x87_ungei_f(_jit, r0, r1, i0)
static void _x87_ungei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_ungtr_f(r0, r1, r2)	x87cmp(X86_CC_NAE, r0, r2, r1)
#  define x87_ungti_f(r0, r1, i0)	_x87_ungti_f(_jit, r0, r1, i0)
static void _x87_ungti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_ltgtr_f(r0, r1, r2)	x87_ltgtr_d(r0, r1, r2)
#  define x87_ltgti_f(r0, r1, i0)	_x87_ltgti_f(_jit, r0, r1, i0)
static void _x87_ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_ordr_f(r0, r1, r2)	x87cmp2(X86_CC_NP, r0, r2, r1)
#  define x87_ordi_f(r0, r1, i0)	_x87_ordi_f(_jit, r0, r1, i0)
static void _x87_ordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_unordr_f(r0, r1, r2)	x87cmp2(X86_CC_P, r0, r2, r1)
#  define x87_unordi_f(r0, r1, i0)	_x87_unordi_f(_jit, r0, r1, i0)
static void _x87_unordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define x87_ltr_d(r0, r1, r2)		x87cmp(X86_CC_A, r0, r2, r1)
#  define x87_lti_d(r0, r1, i0)		_x87_lti_d(_jit, r0, r1, i0)
static void _x87_lti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_ler_d(r0, r1, r2)		x87cmp(X86_CC_AE, r0, r2, r1)
#  define x87_lei_d(r0, r1, i0)		_x87_lei_d(_jit, r0, r1, i0)
static void _x87_lei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_eqr_d(r0, r1, r2)		_x87_eqr_d(_jit, r0, r2, r1)
static void _x87_eqr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_eqi_d(r0, r1, i0)		_x87_eqi_d(_jit, r0, r1, i0)
static void _x87_eqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_ger_d(r0, r1, r2)		x87cmp(X86_CC_AE, r0, r1, r2)
#  define x87_gei_d(r0, r1, i0)		_x87_gei_d(_jit, r0, r1, i0)
static void _x87_gei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_gtr_d(r0, r1, r2)		x87cmp(X86_CC_A, r0, r1, r2)
#  define x87_gti_d(r0, r1, i0)		_x87_gti_d(_jit, r0, r1, i0)
static void _x87_gti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_ner_d(r0, r1, r2)		_x87_ner_d(_jit, r0, r2, r1)
static void _x87_ner_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_nei_d(r0, r1, i0)		_x87_nei_d(_jit, r0, r1, i0)
static void _x87_nei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_unltr_d(r0, r1, r2)	x87cmp(X86_CC_NAE, r0, r1, r2)
#  define x87_unlti_d(r0, r1, i0)	_x87_unlti_d(_jit, r0, r1, i0)
static void _x87_unlti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_unler_d(r0, r1, r2)	x87cmp(X86_CC_NA, r0, r1, r2)
#  define x87_unlei_d(r0, r1, i0)	_x87_unlei_d(_jit, r0, r1, i0)
static void _x87_unlei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_uneqr_d(r0, r1, r2)	x87cmp2(X86_CC_E, r0, r1, r2)
#  define x87_uneqi_d(r0, r1, i0)	_x87_uneqi_d(_jit, r0, r1, i0)
static void _x87_uneqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_unger_d(r0, r1, r2)	x87cmp(X86_CC_NA, r0, r2, r1)
#  define x87_ungei_d(r0, r1, i0)	_x87_ungei_d(_jit, r0, r1, i0)
static void _x87_ungei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_ungtr_d(r0, r1, r2)	x87cmp(X86_CC_NAE, r0, r2, r1)
#  define x87_ungti_d(r0, r1, i0)	_x87_ungti_d(_jit, r0, r1, i0)
static void _x87_ungti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_ltgtr_d(r0, r1, r2)	_x87_ltgtr_d(_jit, r0, r1, r2)
static void _x87_ltgtr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_ltgti_d(r0, r1, i0)	_x87_ltgti_d(_jit, r0, r1, i0)
static void _x87_ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_ordr_d(r0, r1, r2)	x87cmp2(X86_CC_NP, r0, r2, r1)
#  define x87_ordi_d(r0, r1, i0)	_x87_ordi_d(_jit, r0, r1, i0)
static void _x87_ordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define x87_unordr_d(r0, r1, r2)	x87cmp2(X86_CC_P, r0, r2, r1)
#  define x87_unordi_d(r0, r1, i0)	_x87_unordi_d(_jit, r0, r1, i0)
static void _x87_unordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define x87_movr_f(r0,r1)		_x87_movr_d(_jit,r0,r1)
#define x87_movr_d(r0,r1)		_x87_movr_d(_jit,r0,r1)
static void _x87_movr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#define x87_movi_d(r0,i0)		_x87_movi_d(_jit,r0,i0)
static void _x87_movi_d(jit_state_t*, jit_int32_t, jit_float64_t*);
#  define x87_ldr_d(r0, r1)		_x87_ldr_d(_jit, r0, r1)
static void _x87_ldr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define x87_ldi_d(r0, i0)		_x87_ldi_d(_jit, r0, i0)
static void _x87_ldi_d(jit_state_t*, jit_int32_t, jit_word_t);
#  define x87_ldxr_d(r0, r1, r2)	_x87_ldxr_d(_jit, r0, r1, r2)
static void _x87_ldxr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define x87_ldxi_d(r0, r1, i0)	_x87_ldxi_d(_jit, r0, r1, i0)
static void _x87_ldxi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define x87_str_d(r0, r1)		_x87_str_d(_jit, r0, r1)
static void _x87_str_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define x87_sti_d(i0, r0)		_x87_sti_d(_jit, i0, r0)
static void _x87_sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define x87_stxr_d(r0, r1, r2)	_x87_stxr_d(_jit, r0, r1, r2)
static void _x87_stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define x87_stxi_d(i0, r0, r1)	_x87_stxi_d(_jit, i0, r0, r1)
static void _x87_stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define x87_bltr_f(i0, r0, r1)	x87jcc(X86_CC_A, i0, r1, r0)
#  define x87_blti_f(i0, r0, i1)	_x87_blti_f(_jit, i0, r0, i1)
static jit_word_t
_x87_blti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bler_f(i0, r0, r1)	x87jcc(X86_CC_AE, i0, r1, r0)
#  define x87_blei_f(i0, r0, i1)	_x87_blei_f(_jit, i0, r0, i1)
static jit_word_t
_x87_blei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_beqr_f(i0, r0, r1)	_x87_beqr_d(_jit, i0, r0, r1)
#  define x87_beqi_f(i0, r0, i1)	_x87_beqi_f(_jit, i0, r0, i1)
static jit_word_t
_x87_beqi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bger_f(i0, r0, r1)	x87jcc(X86_CC_AE, i0, r0, r1)
#  define x87_bgei_f(i0, r0, i1)	_x87_bgei_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bgei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bgtr_f(i0, r0, r1)	x87jcc(X86_CC_A, i0, r0, r1)
#  define x87_bgti_f(i0, r0, i1)	_x87_bgti_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bgti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bner_f(i0, r0, r1)	_x87_bner_d(_jit, i0, r0, r1)
#  define x87_bnei_f(i0, r0, i1)	_x87_bnei_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bnei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bunltr_f(i0, r0, r1)	x87jcc(X86_CC_NAE, i0, r0, r1)
#  define x87_bunlti_f(i0, r0, i1)	_x87_bunlti_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bunlti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bunler_f(i0, r0, r1)	x87jcc(X86_CC_NA, i0, r0, r1)
#  define x87_bunlei_f(i0, r0, i1)	_x87_bunlei_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bunlei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_buneqr_f(i0, r0, r1)	x87jcc2(X86_CC_E, i0, r0, r1)
#  define x87_buneqi_f(i0, r0, i1)	_x87_buneqi_f(_jit, i0, r0, i1)
static jit_word_t
_x87_buneqi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bunger_f(i0, r0, r1)	x87jcc(X86_CC_NA, i0, r1, r0)
#  define x87_bungei_f(i0, r0, i1)	_x87_bungei_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bungei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bungtr_f(i0, r0, r1)	x87jcc(X86_CC_NAE, i0, r1, r0)
#  define x87_bungti_f(i0, r0, i1)	_x87_bungti_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bungti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bltgtr_f(i0, r0, r1)	x87jcc2(X86_CC_NE, i0, r0, r1)
#  define x87_bltgti_f(i0, r0, i1)	_x87_bltgti_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bltgti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bordr_f(i0, r0, r1)	x87jcc2(X86_CC_NP, i0, r0, r1)
#  define x87_bordi_f(i0, r0, i1)	_x87_bordi_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bordi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bunordr_f(i0, r0, r1)	x87jcc2(X86_CC_P, i0, r0, r1)
#  define x87_bunordi_f(i0, r0, i1)	_x87_bunordi_f(_jit, i0, r0, i1)
static jit_word_t
_x87_bunordi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define x87_bltr_d(i0, r0, r1)	x87jcc(X86_CC_A, i0, r1, r0)
#  define x87_blti_d(i0, r0, i1)	_x87_blti_d(_jit, i0, r0, i1)
static jit_word_t
_x87_blti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bler_d(i0, r0, r1)	x87jcc(X86_CC_AE, i0, r1, r0)
#  define x87_blei_d(i0, r0, i1)	_x87_blei_d(_jit, i0, r0, i1)
static jit_word_t
_x87_blei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_beqr_d(i0, r0, r1)	_x87_beqr_d(_jit, i0, r0, r1)
static jit_word_t
_x87_beqr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define x87_beqi_d(i0, r0, i1)	_x87_beqi_d(_jit, i0, r0, i1)
static jit_word_t
_x87_beqi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bger_d(i0, r0, r1)	x87jcc(X86_CC_AE, i0, r0, r1)
#  define x87_bgei_d(i0, r0, i1)	_x87_bgei_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bgei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bgtr_d(i0, r0, r1)	x87jcc(X86_CC_A, i0, r0, r1)
#  define x87_bgti_d(i0, r0, i1)	_x87_bgti_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bgti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bner_d(i0, r0, r1)	_x87_bner_d(_jit, i0, r0, r1)
static jit_word_t
_x87_bner_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define x87_bnei_d(i0, r0, i1)	_x87_bnei_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bnei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bunltr_d(i0, r0, r1)	x87jcc(X86_CC_NAE, i0, r0, r1)
#  define x87_bunlti_d(i0, r0, i1)	_x87_bunlti_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bunlti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bunler_d(i0, r0, r1)	x87jcc(X86_CC_NA, i0, r0, r1)
#  define x87_bunlei_d(i0, r0, i1)	_x87_bunlei_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bunlei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_buneqr_d(i0, r0, r1)	x87jcc2(X86_CC_E, i0, r0, r1)
#  define x87_buneqi_d(i0, r0, i1)	_x87_buneqi_d(_jit, i0, r0, i1)
static jit_word_t
_x87_buneqi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bunger_d(i0, r0, r1)	x87jcc(X86_CC_NA, i0, r1, r0)
#  define x87_bungei_d(i0, r0, i1)	_x87_bungei_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bungei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bungtr_d(i0, r0, r1)	x87jcc(X86_CC_NAE, i0, r1, r0)
#  define x87_bungti_d(i0, r0, i1)	_x87_bungti_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bungti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bltgtr_d(i0, r0, r1)	x87jcc2(X86_CC_NE, i0, r0, r1)
#  define x87_bltgti_d(i0, r0, i1)	_x87_bltgti_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bltgti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bordr_d(i0, r0, r1)	x87jcc2(X86_CC_NP, i0, r0, r1)
#  define x87_bordi_d(i0, r0, i1)	_x87_bordi_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bordi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define x87_bunordr_d(i0, r0, r1)	x87jcc2(X86_CC_P, i0, r0, r1)
#  define x87_bunordi_d(i0, r0, i1)	_x87_bunordi_d(_jit, i0, r0, i1)
static jit_word_t
_x87_bunordi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#endif

#if CODE
#  define fpr_opi(name, type, size)					\
static void								\
_x87_##name##i_##type(jit_state_t *_jit,				\
		      jit_int32_t r0, jit_int32_t r1,			\
		      jit_float##size##_t *i0)				\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    assert(jit_x87_reg_p(reg));						\
    x87_movi_##type(rn(reg), i0);					\
    x87_##name##r_##type(r0, r1, rn(reg));				\
    jit_unget_reg(reg);							\
}
#  define fpr_bopi(name, type, size)					\
static jit_word_t							\
_x87_b##name##i_##type(jit_state_t *_jit,				\
		       jit_word_t i0, jit_int32_t r0,			\
		       jit_float##size##_t *i1)				\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    assert(jit_x87_reg_p(reg));						\
    x87_movi_##type(rn(reg), i1);					\
    word = x87_b##name##r_##type(i0, r0, rn(reg));			\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define fopi(name)			fpr_opi(name, f, 32)
#  define fbopi(name)			fpr_bopi(name, f, 32)
#  define dopi(name)			fpr_opi(name, d, 64)
#  define dbopi(name)			fpr_bopi(name, d, 64)

static void
_fstcwm(jit_state_t *_jit, jit_int32_t md,
	jit_int32_t rb,	jit_int32_t ri, jit_int32_t ms)
{
    ic(0x9b);
    rex(0, 1, rb, ri, _NOREG);
    x87rx(017, md, rb, ri, ms);
}

static void
_x87rx(jit_state_t *_jit, jit_int32_t code, jit_int32_t md,
       jit_int32_t rb, jit_int32_t ri, jit_int32_t ms)
{
    rex(0, 1, rb, ri, _NOREG);
    ic(0xd8 | (code >> 3));
    rx((code & 7), md, rb, ri, ms);
}

static void
_x87ri(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0)
{
    ic(0xd8 | (code >> 3));
    mrm(0x03, (code & 7), r0);
}

static void
_x87rri(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0, jit_int32_t r1)
{
    if (r1 == _ST0_REGNO)
	x87ri(code | 040, r0);
    else {
	assert(r0 == _ST0_REGNO);
	x87ri(code, r1);
    }
}

fopi(add)
fopi(sub)
fopi(rsb)
fopi(mul)
fopi(div)

static void
_x87_addr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1) {
	if (r2 == _ST0_REGNO)
	    faddr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    faddr(_ST0_REGNO, r2);
	else {
	    fxchr(r0);
	    faddr(_ST0_REGNO, r0 == r2 ? _ST0_REGNO : r2);
	    fxchr(r0);
	}
    }
    else if (r0 == r2) {
	if (r1 == _ST0_REGNO)
	    faddr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    faddr(_ST0_REGNO, r1);
	else {
	    fxchr(r0);
	    faddr(_ST0_REGNO, r1);
	    fxchr(r0);
	}
    }
    else {
	fldr(r1);
	faddr(_ST0_REGNO, r2 + 1);
	fstpr(r0 + 1);
    }
}

dopi(add)

static void
_x87_subr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1) {
	if (r2 == _ST0_REGNO)
	    fsubrr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    fsubr(_ST0_REGNO, r2);
	else {
	    fxchr(r0);
	    fsubr(_ST0_REGNO, r0 == r2 ? _ST0_REGNO : r2);
	    fxchr(r0);
	}
    }
    else if (r0 == r2) {
	if (r1 == _ST0_REGNO)
	    fsubr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    fsubrr(_ST0_REGNO, r1);
	else {
	    fxchr(r0);
	    fsubrr(_ST0_REGNO, r1);
	    fxchr(r0);
	}
    }
    else {
	fldr(r1);
	fsubr(_ST0_REGNO, r2 + 1);
	fstpr(r0 + 1);
    }
}

dopi(sub)

dopi(rsb)

static void
_x87_mulr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1) {
	if (r2 == _ST0_REGNO)
	    fmulr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    fmulr(_ST0_REGNO, r2);
	else {
	    fxchr(r0);
	    fmulr(_ST0_REGNO, r0 == r2 ? _ST0_REGNO : r2);
	    fxchr(r0);
	}
    }
    else if (r0 == r2) {
	if (r1 == _ST0_REGNO)
	    fmulr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    fmulr(_ST0_REGNO, r1);
	else {
	    fxchr(r0);
	    fmulr(_ST0_REGNO, r1);
	    fxchr(r0);
	}
    }
    else {
	fldr(r1);
	fmulr(_ST0_REGNO, r2 + 1);
	fstpr(r0 + 1);
    }
}

dopi(mul)

static void
_x87_divr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1) {
	if (r2 == _ST0_REGNO)
	    fdivrr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    fdivr(_ST0_REGNO, r2);
	else {
	    fxchr(r0);
	    fdivr(_ST0_REGNO, r0 == r2 ? _ST0_REGNO : r2);
	    fxchr(r0);
	}
    }
    else if (r0 == r2) {
	if (r1 == _ST0_REGNO)
	    fdivr(r0, _ST0_REGNO);
	else if (r0 == _ST0_REGNO)
	    fsubrr(_ST0_REGNO, r1);
	else {
	    fxchr(r0);
	    fdivrr(_ST0_REGNO, r1);
	    fxchr(r0);
	}
    }
    else {
	fldr(r1);
	fdivr(_ST0_REGNO, r2 + 1);
	fstpr(r0 + 1);
    }
}

dopi(div)

static void
_x87_absr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1) {
	if (r1 == _ST0_REGNO)
	    fabs_();
	else {
	    fxchr(r0);
	    fabs_();
	    fxchr(r0);
	}
    }
    else {
	fldr(r1);
	fabs_();
	fstpr(r0 + 1);
    }
}

static void
_x87_negr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1) {
	if (r1 == _ST0_REGNO)
	    fchs_();
	else {
	    fxchr(r0);
	    fchs_();
	    fxchr(r0);
	}
    }
    else {
	fldr(r1);
	fchs_();
	fstpr(r0 + 1);
    }
}

static void
_x87_sqrtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1) {
	if (r1 == _ST0_REGNO)
	    fsqrt_();
	else {
	    fxchr(r0);
	    fsqrt_();
	    fxchr(r0);
	}
    }
    else {
	fldr(r1);
	fsqrt_();
	fstpr(r0 + 1);
    }
}

static void
_x87_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if defined(sun)
    /* for the sake of passing test cases in x87 mode, otherwise only sse
     * is supported */
    fstcwm(-4, _RBP_REGNO, _NOREG, _SCL1);
    ldxi_s(r0, _RBP_REGNO, -4);
    extr_uc(r0, r0);
#  define FPCW_CHOP	0xc00
    ori(r0, r0, FPCW_CHOP);
    stxi_s(-8, _RBP_REGNO, r0);
    fldcwm(-8, _RBP_REGNO, _NOREG, _SCL1);
    if (r1 == _ST0_REGNO)
	fistlm(CVT_OFFSET, _RBP_REGNO, _NOREG, _SCL1);
    else {
	fxchr(r1);
	fistlm(CVT_OFFSET, _RBP_REGNO, _NOREG, _SCL1);
	fxchr(r1);
    }
    fldcwm(-4, _RBP_REGNO, _NOREG, _SCL1);
    ldxi(r0, _RBP_REGNO, CVT_OFFSET);
#else
    fldr(r1);
    fisttplm(CVT_OFFSET, _RBP_REGNO, _NOREG, _SCL1);
    ldxi_i(r0, _RBP_REGNO, CVT_OFFSET);
#endif
}

#  if __X64
static void
_x87_truncr_d_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    fldr(r1);
    fisttpqm(CVT_OFFSET, _RBP_REGNO, _NOREG, _SCL1);
    ldxi(r0, _RBP_REGNO, CVT_OFFSET);
}
#  endif

static void
_x87_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    stxi(CVT_OFFSET, _RBP_REGNO, r1);
#  if __X32
    fildlm(CVT_OFFSET, _RBP_REGNO, _NOREG, _SCL1);
#  else
    fildqm(CVT_OFFSET, _RBP_REGNO, _NOREG, _SCL1);
#  endif
    fstpr(r0 + 1);
}

static void
_x87cmp(jit_state_t *_jit, jit_int32_t code,
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
    if (r1 == _ST0_REGNO)
	fucomir(r2);
    else {
	fldr(r1);
	fucomipr(r2 + 1);
    }
    cc(code, reg);
    if (!rc)
	xchgr(r0, reg);
}

static void
_x87cmp2(jit_state_t *_jit, jit_int32_t code,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t			rc;
    jit_int32_t			reg;
    jit_int32_t			f1, f2;
    if (r2 == _ST0_REGNO)	f1 = r2, f2 = r1;
    else			f1 = r1, f2 = r2;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, reg);
    }
    ixorr(reg, reg);
    if (f1 == _ST0_REGNO)
	fucomir(f2);
    else {
	fldr(f1);
	fucomipr(f2 + 1);
    }
    cc(code, reg);
    if (!rc)
	xchgr(r0, reg);
}

static jit_word_t
_x87jcc(jit_state_t *_jit, jit_int32_t code,
	jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == _ST0_REGNO)
	fucomir(r1);
    else {
	fldr(r0);
	fucomipr(r1 + 1);
    }
    jcc(code, i0);
    return (_jit->pc.w);
}

static jit_word_t
_x87jcc2(jit_state_t *_jit, jit_int32_t code,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t			f0, f1;
    if (r1 == _ST0_REGNO)	f0 = r1, f1 = r0;
    else			f0 = r0, f1 = r1;
    if (f0 == _ST0_REGNO)
	fucomir(f1);
    else {
	fldr(f0);
	fucomipr(f1 + 1);
    }
    jcc(code, i0);
    return (_jit->pc.w);
}

fopi(lt)
fopi(le)
fopi(eq)
fopi(ge)
fopi(gt)
fopi(ne)
fopi(unlt)
fopi(unle)
fopi(uneq)
fopi(unge)
fopi(ungt)
fopi(ltgt)
fopi(ord)
fopi(unord)
fbopi(lt)
fbopi(le)
fbopi(eq)
fbopi(ge)
fbopi(gt)
fbopi(ne)
fbopi(unlt)
fbopi(unle)
fbopi(uneq)
fbopi(unge)
fbopi(ungt)
fbopi(ltgt)
fbopi(ord)
fbopi(unord)

static void
_x87_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t *i0)
{
    union {
	jit_int32_t	 i;
	jit_float32_t	 f;
    } data;
    jit_int32_t		 reg;

    data.f = *i0;
    if (data.f == 0.0 && !(data.i & 0x80000000))
	fldz();
    else if (data.f == 1.0)
	fld1();
    else if (data.f == 3.3219280948873623478703195458468f)
	fldl2t();
    else if (data.f == 1.4426950408889634073599246886656f)
	fldl2e();
    else if (data.f == 3.1415926535897932384626421096161f)
	fldpi();
    else if (data.f == 0.3010299956639811952137387498515f)
	fldlg2();
    else if (data.f == 0.6931471805599453094172323683399f)
	fldln2();
    else {
	if (_jitc->no_data) {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), data.i);
	    stxi_i(CVT_OFFSET, _RBP_REGNO, rn(reg));
	    jit_unget_reg(reg);
	    x87_ldxi_f(r0, _RBP_REGNO, CVT_OFFSET);
	}
	else
	    x87_ldi_f(r0, (jit_word_t)i0);
	return;
    }
    fstpr(r0 + 1);
}

static void
_x87_ldr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    fldsm(0, r1, _NOREG, _SCL1);
    fstpr(r0 + 1);
}

static void
_x87_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (x87_address_p(i0)) {
	fldsm(i0, _NOREG, _NOREG, _SCL1);
	fstpr(r0 + 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	x87_ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_x87_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    x87_ldr_f(r0, rn(reg));
    jit_unget_reg(reg);
#else
    fldsm(0, r1, r2, _SCL1);
    fstpr(r0 + 1);
#endif
}

static void
_x87_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	fldsm(i0, r1, _NOREG, _SCL1);
	fstpr(r0 + 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r1, i0);
	x87_ldr_f(r0, rn(reg));
#else
	movi(rn(reg), i0);
	x87_ldxr_f(r0, r1, rn(reg));
#endif
	jit_unget_reg(reg);
    }
}

static void
_x87_str_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r1 == _ST0_REGNO)
	fstsm(0, r0, _NOREG, _SCL1);
    else {
	fxchr(r1);
	fstsm(0, r0, _NOREG, _SCL1);
	fxchr(r1);
    }
}

static void
_x87_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (!x87_address_p(i0)) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	x87_str_f(rn(reg), r0);
	jit_unget_reg(reg);
    }
    else if (r0 == _ST0_REGNO)
	fstsm(i0, _NOREG, _NOREG, _SCL1);
    else {
	fxchr(r0);
	fstsm(i0, _NOREG, _NOREG, _SCL1);
	fxchr(r0);
    }
}

static void
_x87_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    x87_str_f(rn(reg), r2);
    jit_unget_reg(reg);
#else
    if (r2 == _ST0_REGNO)
	fstsm(0, r0, r1, _SCL1);
    else {
	fxchr(r2);
	fstsm(0, r0, r1, _SCL1);
	fxchr(r2);
    }
#endif
}

static void
_x87_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (!can_sign_extend_int_p(i0)) {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r0, i0);
	x87_str_f(rn(reg), r1);
#else
	movi(rn(reg), i0);
	x87_stxr_f(rn(reg), r0, r1);
#endif
	jit_unget_reg(reg);
    }
    else if (r1 == _ST0_REGNO)
	fstsm(i0, r0, _NOREG, _SCL1);
    else {
	fxchr(r1);
	fstsm(i0, r0, _NOREG, _SCL1);
	fxchr(r1);
    }
}

static void
_x87_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1) {
	if (r1 == _ST0)
	    fstr(r0);
	else if (r0 == _ST0) {
	    fxchr(r1);
	    fstr(r1);
	}
	else {
	    fldr(r1);
	    fstpr(r0 + 1);
	}
    }
}

static void
_x87_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
	jit_int32_t	 ii[2];
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;

    data.d = *i0;
    if (data.d == 0.0 && !(data.ii[1] & 0x80000000))
	fldz();
    else if (data.d == 1.0)
	fld1();
    else if (data.d == 3.3219280948873623478703195458468)
	fldl2t();
    else if (data.d == 1.4426950408889634073599246886656)
	fldl2e();
    else if (data.d == 3.1415926535897932384626421096161)
	fldpi();
    else if (data.d == 0.3010299956639811952137387498515)
	fldlg2();
    else if (data.d == 0.6931471805599453094172323683399)
	fldln2();
    else {
	if (_jitc->no_data) {
	    reg = jit_get_reg(jit_class_gpr);
#if __X32 || __X64_32
	    movi(rn(reg), data.ii[0]);
	    stxi_i(CVT_OFFSET, _RBP_REGNO, rn(reg));
	    movi(rn(reg), data.ii[1]);
	    stxi_i(CVT_OFFSET + 4, _RBP_REGNO, rn(reg));
#else
	    movi(rn(reg), data.w);
	    stxi_l(CVT_OFFSET, _RBP_REGNO, rn(reg));
#endif
	    jit_unget_reg(reg);
	    x87_ldxi_d(r0, _RBP_REGNO, CVT_OFFSET);
	}
	else
	    x87_ldi_d(r0, (jit_word_t)i0);
	return;
    }
    fstpr(r0 + 1);
}

dopi(lt)
dopi(le)

static void
_x87_eqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t			rc;
    jit_word_t			jp_code;
    jit_int32_t			reg, f1, f2;
    if (r2 == _ST0_REGNO)	f1 = r2, f2 = r1;
    else			f1 = r1, f2 = r2;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, reg);
    }
    ixorr(reg, reg);
    if (f1 == _ST0_REGNO)
	fucomir(f2);
    else {
	fldr(f1);
	fucomipr(f2 + 1);
    }
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
_x87_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_bool_t			rc;
    jit_word_t			jp_code;
    jit_int32_t			reg, f1, f2;
    if (r2 == _ST0_REGNO)	f1 = r2, f2 = r1;
    else			f1 = r1, f2 = r2;
    if ((rc = reg8_p(r0)))
	reg = r0;
    else {
	reg = _RAX_REGNO;
	movr(r0, reg);
    }
    imovi(reg, 1);
    if (f1 == _ST0_REGNO)
	fucomir(f2);
    else {
	fldr(f1);
	fucomipr(f2 + 1);
    }
    jpes(0);
    jp_code = _jit->pc.w;
    cc(X86_CC_NE, reg);
    patch_rel_char(jp_code, _jit->pc.w);
    if (!rc)
	xchgr(r0, reg);
}

dopi(ne)
dopi(unlt)
dopi(unle)
dopi(uneq)
dopi(unge)
dopi(ungt)

static void
_x87_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	x87cmp2(X86_CC_NE, r0, r1, r2);
}

dopi(ltgt)
dopi(ord)
dopi(unord)

static void
_x87_ldr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    fldlm(0, r1, _NOREG, _SCL1);
    fstpr(r0 + 1);
}

static void
_x87_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (x87_address_p(i0)) {
	fldlm(i0, _NOREG, _NOREG, _SCL1);
	fstpr(r0 + 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	x87_ldr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_x87_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    x87_ldr_d(r0, rn(reg));
    jit_unget_reg(reg);
#else
    fldlm(0, r1, r2, _SCL1);
    fstpr(r0 + 1);
#endif
}

static void
_x87_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	fldlm(i0, r1, _NOREG, _SCL1);
	fstpr(r0 + 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r1, i0);
	x87_ldr_d(r0, rn(reg));
#else
	movi(rn(reg), i0);
	x87_ldxr_d(r0, r1, rn(reg));
#endif
	jit_unget_reg(reg);
    }
}

static void
_x87_str_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r1 == _ST0_REGNO)
	fstlm(0, r0, _NOREG, _SCL1);
    else {
	fxchr(r1);
	fstlm(0, r0, _NOREG, _SCL1);
	fxchr(r1);
    }
}

static void
_x87_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (!x87_address_p(i0)) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	x87_str_d(rn(reg), r0);
	jit_unget_reg(reg);
    }
    else if (r0 == _ST0_REGNO)
	fstlm(i0, _NOREG, _NOREG, _SCL1);
    else {
	fxchr(r0);
	fstlm(i0, _NOREG, _NOREG, _SCL1);
	fxchr(r0);
    }
}

static void
_x87_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    x87_str_d(rn(reg), r2);
    jit_unget_reg(reg);
#else
    if (r2 == _ST0_REGNO)
	fstlm(0, r0, r1, _SCL1);
    else {
	fxchr(r2);
	fstlm(0, r0, r1, _SCL1);
	fxchr(r2);
    }
#endif
}

static void
_x87_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (!can_sign_extend_int_p(i0)) {
	reg = jit_get_reg(jit_class_gpr);
#if __X64_32
	addi(rn(reg), r0, i0);
	x87_str_d(rn(reg), r1);
#else
	movi(rn(reg), i0);
	x87_stxr_d(rn(reg), r0, r1);
#endif
	jit_unget_reg(reg);
    }
    else if (r1 == _ST0_REGNO)
	fstlm(i0, r0, _NOREG, _SCL1);
    else {
	fxchr(r1);
	fstlm(i0, r0, _NOREG, _SCL1);
	fxchr(r1);
    }
}

dbopi(lt)
dbopi(le)

static jit_word_t
_x87_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t			f0, f1;
    jit_word_t			jp_code;
    if (r1 == _ST0_REGNO)	f0 = r1, f1 = r0;
    else			f0 = r0, f1 = r1;
    if (f0 == _ST0_REGNO)
	fucomir(f1);
    else {
	fldr(f0);
	fucomipr(f1 + 1);
    }
    jpes(0);
    jp_code = _jit->pc.w;
    jcc(X86_CC_E, i0);
    patch_rel_char(jp_code, _jit->pc.w);
    return (_jit->pc.w);
}
dbopi(eq)
dbopi(ge)
dbopi(gt)

static jit_word_t
_x87_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t			f0, f1;
    jit_word_t			jp_code;
    jit_word_t			jz_code;
    if (r1 == _ST0_REGNO)	f0 = r1, f1 = r0;
    else			f0 = r0, f1 = r1;
    if (f0 == _ST0_REGNO)
	fucomir(f1);
    else {
	fldr(f0);
	fucomipr(f1 + 1);
    }
    jpes(0);
    jp_code = _jit->pc.w;
    jzs(0);
    jz_code = _jit->pc.w;
    patch_rel_char(jp_code, _jit->pc.w);
    jmpi(i0);
    patch_rel_char(jz_code, _jit->pc.w);
    return (_jit->pc.w);
}
dbopi(ne)
dbopi(unlt)
dbopi(unle)
dbopi(uneq)
dbopi(unge)
dbopi(ungt)
dbopi(ltgt)
dbopi(ord)
dbopi(unord)
#  undef fopi
#  undef fbopi
#  undef dopi
#  undef dbopi
#  undef fpr_bopi
#  undef fpr_opi
#endif
