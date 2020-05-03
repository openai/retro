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

#if PROTO
#  define A64_SCVTF			0x1e220000
#  define A64_FMOVWV			0x1e260000
#  define A64_FMOVVW			0x1e270000
#  define A64_FMOVXV			0x9e260000
#  define A64_FMOVVX			0x9e270000
#  define A64_FCVTZS			0x1e380000
#  define A64_FCMPE			0x1e202010
#  define A64_FMOV			0x1e204000
#  define A64_FABS			0x1e20c000
#  define A64_FNEG			0x1e214000
#  define A64_FSQRT			0x1e21c000
#  define A64_FCVTS			0x1e224000
#  define A64_FCVTD			0x1e22c000
#  define A64_FMUL			0x1e200800
#  define A64_FDIV			0x1e201800
#  define A64_FADD			0x1e202800
#  define A64_FSUB			0x1e203800
#  define FCMPES(Rn,Rm)			os_vv(A64_FCMPE,0,Rn,Rm)
#  define FCMPED(Rn,Rm)			os_vv(A64_FCMPE,1,Rn,Rm)
#  define FMOVS(Rd,Rn)			osvv_(A64_FMOV,0,Rd,Rn)
#  define FMOVD(Rd,Rn)			osvv_(A64_FMOV,1,Rd,Rn)
#  define FMOVWS(Rd,Rn)			osvv_(A64_FMOVWV,0,Rd,Rn)
#  define FMOVSW(Rd,Rn)			osvv_(A64_FMOVVW,0,Rd,Rn)
#  define FMOVXD(Rd,Rn)			osvv_(A64_FMOVXV,1,Rd,Rn)
#  define FMOVDX(Rd,Rn)			osvv_(A64_FMOVVX,1,Rd,Rn)
#  define FCVT_SD(Rd,Rn)		osvv_(A64_FCVTS,1,Rd,Rn)
#  define FCVT_DS(Rd,Rn)		osvv_(A64_FCVTD,0,Rd,Rn)
#  define SCVTFS(Rd,Rn)			osvv_(A64_SCVTF|XS,0,Rd,Rn)
#  define SCVTFD(Rd,Rn)			osvv_(A64_SCVTF|XS,1,Rd,Rn)
#  define FCVTSZ_WS(Rd,Rn)		osvv_(A64_FCVTZS,0,Rd,Rn)
#  define FCVTSZ_WD(Rd,Rn)		osvv_(A64_FCVTZS,1,Rd,Rn)
#  define FCVTSZ_XS(Rd,Rn)		osvv_(A64_FCVTZS|XS,0,Rd,Rn)
#  define FCVTSZ_XD(Rd,Rn)		osvv_(A64_FCVTZS|XS,1,Rd,Rn)
#  define FABSS(Rd,Rn)			osvv_(A64_FABS,0,Rd,Rn)
#  define FABSD(Rd,Rn)			osvv_(A64_FABS,1,Rd,Rn)
#  define FNEGS(Rd,Rn)			osvv_(A64_FNEG,0,Rd,Rn)
#  define FNEGD(Rd,Rn)			osvv_(A64_FNEG,1,Rd,Rn)
#  define FSQRTS(Rd,Rn)			osvv_(A64_FSQRT,0,Rd,Rn)
#  define FSQRTD(Rd,Rn)			osvv_(A64_FSQRT,1,Rd,Rn)
#  define FADDS(Rd,Rn,Rm)		osvvv(A64_FADD,0,Rd,Rn,Rm)
#  define FADDD(Rd,Rn,Rm)		osvvv(A64_FADD,1,Rd,Rn,Rm)
#  define FSUBS(Rd,Rn,Rm)		osvvv(A64_FSUB,0,Rd,Rn,Rm)
#  define FSUBD(Rd,Rn,Rm)		osvvv(A64_FSUB,1,Rd,Rn,Rm)
#  define FMULS(Rd,Rn,Rm)		osvvv(A64_FMUL,0,Rd,Rn,Rm)
#  define FMULD(Rd,Rn,Rm)		osvvv(A64_FMUL,1,Rd,Rn,Rm)
#  define FDIVS(Rd,Rn,Rm)		osvvv(A64_FDIV,0,Rd,Rn,Rm)
#  define FDIVD(Rd,Rn,Rm)		osvvv(A64_FDIV,1,Rd,Rn,Rm)
#  define osvvv(Op,Sz,Rd,Rn,Rm)		_osvvv(_jit,Op,Sz,Rd,Rn,Rm)
static void _osvvv(jit_state_t*,jit_int32_t,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define osvv_(Op,Sz,Rd,Rn)		_osvv_(_jit,Op,Sz,Rd,Rn)
static void _osvv_(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define os_vv(Op,Sz,Rn,Rm)		_os_vv(_jit,Op,Sz,Rn,Rm)
static void _os_vv(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define truncr_f_i(r0,r1)		_truncr_f_i(_jit,r0,r1)
static void _truncr_f_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define truncr_f_l(r0,r1)		FCVTSZ_XS(r0,r1)
#  define truncr_d_i(r0,r1)		_truncr_d_i(_jit,r0,r1)
static void _truncr_d_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define truncr_d_l(r0,r1)		FCVTSZ_XD(r0,r1)
#  define addr_f(r0,r1,r2)		FADDS(r0,r1,r2)
#  define addi_f(r0,r1,i0)		_addi_f(_jit,r0,r1,i0)
static void _addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define subr_f(r0,r1,r2)		FSUBS(r0,r1,r2)
#  define subi_f(r0,r1,i0)		_subi_f(_jit,r0,r1,i0)
static void _subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define rsbr_f(r0, r1, r2)		subr_f(r0, r2, r1)
#  define rsbi_f(r0, r1, i0)		_rsbi_f(_jit, r0, r1, i0)
static void _rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define mulr_f(r0,r1,r2)		FMULS(r0,r1,r2)
#  define muli_f(r0,r1,i0)		_muli_f(_jit,r0,r1,i0)
static void _muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define divr_f(r0,r1,r2)		FDIVS(r0,r1,r2)
#  define divi_f(r0,r1,i0)		_divi_f(_jit,r0,r1,i0)
static void _divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define absr_f(r0,r1)			FABSS(r0,r1)
#  define negr_f(r0,r1)			FNEGS(r0,r1)
#  define sqrtr_f(r0,r1)		FSQRTS(r0,r1)
#  define extr_f(r0,r1)			SCVTFS(r0,r1)
#  define ldr_f(r0,r1)			_ldr_f(_jit,r0,r1)
static void _ldr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_f(r0,i0)			_ldi_f(_jit,r0,i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_f(r0,r1,r2)		_ldxr_f(_jit,r0,r1,r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_f(r0,r1,i0)		_ldxi_f(_jit,r0,r1,i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_f(r0,r1)			_str_f(_jit,r0,r1)
static void _str_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sti_f(i0,r0)			_sti_f(_jit,i0,r0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_f(r0,r1,r2)		_stxr_f(_jit,r0,r1,r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_f(i0,r0,r1)		_stxi_f(_jit,i0,r0,r1)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define movr_f(r0,r1)			_movr_f(_jit,r0,r1)
static void _movr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_f(r0,i0)			_movi_f(_jit,r0,i0)
static void _movi_f(jit_state_t*,jit_int32_t,jit_float32_t);
#  define extr_d_f(r0,r1)		FCVT_SD(r0,r1)
#  define fccr(cc,r0,r1,r2)		_fccr(_jit,cc,r0,r1,r2)
static void _fccr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define fcci(cc,r0,r1,i0)		_fcci(_jit,cc,r0,r1,i0)
static void _fcci(jit_state_t*,
		  jit_int32_t,jit_int32_t,jit_int32_t,jit_float32_t);
#  define ltr_f(r0,r1,r2)		fccr(CC_MI,r0,r1,r2)
#  define lti_f(r0,r1,i0)		fcci(CC_MI,r0,r1,i0)
#  define ler_f(r0,r1,r2)		fccr(CC_LS,r0,r1,r2)
#  define lei_f(r0,r1,i0)		fcci(CC_LS,r0,r1,i0)
#  define eqr_f(r0,r1,r2)		fccr(CC_EQ,r0,r1,r2)
#  define eqi_f(r0,r1,i0)		fcci(CC_EQ,r0,r1,i0)
#  define ger_f(r0,r1,r2)		fccr(CC_GE,r0,r1,r2)
#  define gei_f(r0,r1,i0)		fcci(CC_GE,r0,r1,i0)
#  define gtr_f(r0,r1,r2)		fccr(CC_GT,r0,r1,r2)
#  define gti_f(r0,r1,i0)		fcci(CC_GT,r0,r1,i0)
#  define ner_f(r0,r1,r2)		fccr(CC_NE,r0,r1,r2)
#  define nei_f(r0,r1,i0)		fcci(CC_NE,r0,r1,i0)
#  define unltr_f(r0,r1,r2)		fccr(CC_LT,r0,r1,r2)
#  define unlti_f(r0,r1,i0)		fcci(CC_LT,r0,r1,i0)
#  define unler_f(r0,r1,r2)		fccr(CC_LE,r0,r1,r2)
#  define unlei_f(r0,r1,i0)		fcci(CC_LE,r0,r1,i0)
#  define uneqr_f(r0,r1,r2)		_uneqr_f(_jit,r0,r1,r2)
static void _uneqr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define uneqi_f(r0,r1,i0)		_uneqi_f(_jit,r0,r1,i0)
static void _uneqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define unger_f(r0,r1,r2)		fccr(CC_PL,r0,r1,r2)
#  define ungei_f(r0,r1,i0)		fcci(CC_PL,r0,r1,i0)
#  define ungtr_f(r0,r1,r2)		fccr(CC_HI,r0,r1,r2)
#  define ungti_f(r0,r1,i0)		fcci(CC_HI,r0,r1,i0)
#  define ltgtr_f(r0,r1,r2)		_ltgtr_f(_jit,r0,r1,r2)
static void _ltgtr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ltgti_f(r0,r1,i0)		_ltgti_f(_jit,r0,r1,i0)
static void _ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define ordr_f(r0,r1,r2)		fccr(CC_VC,r0,r1,r2)
#  define ordi_f(r0,r1,i0)		fcci(CC_VC,r0,r1,i0)
#  define unordr_f(r0,r1,r2)		fccr(CC_VS,r0,r1,r2)
#  define unordi_f(r0,r1,i0)		fcci(CC_VS,r0,r1,i0)
#define fbccr(cc,i0,r0,r1)		_fbccr(_jit,cc,i0,r0,r1)
static jit_word_t
_fbccr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#define fbcci(cc,i0,r0,i1)		_fbcci(_jit,cc,i0,r0,i1)
static jit_word_t
_fbcci(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_float32_t);
#  define bltr_f(i0,r0,r1)		fbccr(BCC_MI,i0,r0,r1)
#  define blti_f(i0,r0,i1)		fbcci(BCC_MI,i0,r0,i1)
#  define bler_f(i0,r0,r1)		fbccr(BCC_LS,i0,r0,r1)
#  define blei_f(i0,r0,i1)		fbcci(BCC_LS,i0,r0,i1)
#  define beqr_f(i0,r0,r1)		fbccr(BCC_EQ,i0,r0,r1)
#  define beqi_f(i0,r0,i1)		fbcci(BCC_EQ,i0,r0,i1)
#  define bger_f(i0,r0,r1)		fbccr(BCC_GE,i0,r0,r1)
#  define bgei_f(i0,r0,i1)		fbcci(BCC_GE,i0,r0,i1)
#  define bgtr_f(i0,r0,r1)		fbccr(BCC_GT,i0,r0,r1)
#  define bgti_f(i0,r0,i1)		fbcci(BCC_GT,i0,r0,i1)
#  define bner_f(i0,r0,r1)		fbccr(BCC_NE,i0,r0,r1)
#  define bnei_f(i0,r0,i1)		fbcci(BCC_NE,i0,r0,i1)
#  define bunltr_f(i0,r0,r1)		fbccr(BCC_LT,i0,r0,r1)
#  define bunlti_f(i0,r0,i1)		fbcci(BCC_LT,i0,r0,i1)
#  define bunler_f(i0,r0,r1)		fbccr(BCC_LE,i0,r0,r1)
#  define bunlei_f(i0,r0,i1)		fbcci(BCC_LE,i0,r0,i1)
#  define buneqr_f(i0,r0,r1)		_buneqr_f(_jit,i0,r0,r1)
static jit_word_t _buneqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi_f(i0,r0,i1)		_buneqi_f(_jit,i0,r0,i1)
static jit_word_t _buneqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bunger_f(i0,r0,r1)		fbccr(BCC_PL,i0,r0,r1)
#  define bungei_f(i0,r0,i1)		fbcci(BCC_PL,i0,r0,i1)
#  define bungtr_f(i0,r0,r1)		fbccr(BCC_HI,i0,r0,r1)
#  define bungti_f(i0,r0,i1)		fbcci(BCC_HI,i0,r0,i1)
#  define bltgtr_f(i0,r0,r1)		_bltgtr_f(_jit,i0,r0,r1)
static jit_word_t _bltgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti_f(i0,r0,i1)		_bltgti_f(_jit,i0,r0,i1)
static jit_word_t _bltgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bordr_f(i0,r0,r1)		fbccr(BCC_VC,i0,r0,r1)
#  define bordi_f(i0,r0,i1)		fbcci(BCC_VC,i0,r0,i1)
#  define bunordr_f(i0,r0,r1)		fbccr(BCC_VS,i0,r0,r1)
#  define bunordi_f(i0,r0,i1)		fbcci(BCC_VS,i0,r0,i1)
#  define addr_d(r0,r1,r2)		FADDD(r0,r1,r2)
#  define addi_d(r0,r1,i0)		_addi_d(_jit,r0,r1,i0)
static void _addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define subr_d(r0,r1,r2)		FSUBD(r0,r1,r2)
#  define subi_d(r0,r1,i0)		_subi_d(_jit,r0,r1,i0)
static void _subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define rsbr_d(r0, r1, r2)		subr_d(r0, r2, r1)
#  define rsbi_d(r0, r1, i0)		_rsbi_d(_jit, r0, r1, i0)
static void _rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define mulr_d(r0,r1,r2)		FMULD(r0,r1,r2)
#  define muli_d(r0,r1,i0)		_muli_d(_jit,r0,r1,i0)
static void _muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define divr_d(r0,r1,r2)		FDIVD(r0,r1,r2)
#  define divi_d(r0,r1,i0)		_divi_d(_jit,r0,r1,i0)
static void _divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define absr_d(r0,r1)			FABSD(r0,r1)
#  define negr_d(r0,r1)			FNEGD(r0,r1)
#  define sqrtr_d(r0,r1)		FSQRTD(r0,r1)
#  define extr_d(r0,r1)			SCVTFD(r0,r1)
#  define ldr_d(r0,r1)			_ldr_d(_jit,r0,r1)
static void _ldr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_d(r0,i0)			_ldi_d(_jit,r0,i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_d(r0,r1,r2)		_ldxr_d(_jit,r0,r1,r2)
static void _ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_d(r0,r1,i0)		_ldxi_d(_jit,r0,r1,i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_d(r0,r1)			_str_d(_jit,r0,r1)
static void _str_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sti_d(i0,r0)			_sti_d(_jit,i0,r0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_d(r0,r1,r2)		_stxr_d(_jit,r0,r1,r2)
static void _stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_d(i0,r0,r1)		_stxi_d(_jit,i0,r0,r1)
static void _stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define movr_d(r0,r1)			_movr_d(_jit,r0,r1)
static void _movr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_d(r0,i0)			_movi_d(_jit,r0,i0)
static void _movi_d(jit_state_t*,jit_int32_t,jit_float64_t);
#  define extr_f_d(r0,r1)		FCVT_DS(r0,r1)
#  define dccr(cc,r0,r1,r2)		_dccr(_jit,cc,r0,r1,r2)
static void _dccr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define dcci(cc,r0,r1,i0)		_dcci(_jit,cc,r0,r1,i0)
static void _dcci(jit_state_t*,
		  jit_int32_t,jit_int32_t,jit_int32_t,jit_float64_t);
#  define ltr_d(r0,r1,r2)		dccr(CC_MI,r0,r1,r2)
#  define lti_d(r0,r1,i0)		dcci(CC_MI,r0,r1,i0)
#  define ler_d(r0,r1,r2)		dccr(CC_LS,r0,r1,r2)
#  define lei_d(r0,r1,i0)		dcci(CC_LS,r0,r1,i0)
#  define eqr_d(r0,r1,r2)		dccr(CC_EQ,r0,r1,r2)
#  define eqi_d(r0,r1,i0)		dcci(CC_EQ,r0,r1,i0)
#  define ger_d(r0,r1,r2)		dccr(CC_GE,r0,r1,r2)
#  define gei_d(r0,r1,i0)		dcci(CC_GE,r0,r1,i0)
#  define gtr_d(r0,r1,r2)		dccr(CC_GT,r0,r1,r2)
#  define gti_d(r0,r1,i0)		dcci(CC_GT,r0,r1,i0)
#  define ner_d(r0,r1,r2)		dccr(CC_NE,r0,r1,r2)
#  define nei_d(r0,r1,i0)		dcci(CC_NE,r0,r1,i0)
#  define unltr_d(r0,r1,r2)		dccr(CC_LT,r0,r1,r2)
#  define unlti_d(r0,r1,i0)		dcci(CC_LT,r0,r1,i0)
#  define unler_d(r0,r1,r2)		dccr(CC_LE,r0,r1,r2)
#  define unlei_d(r0,r1,i0)		dcci(CC_LE,r0,r1,i0)
#  define uneqr_d(r0,r1,r2)		_uneqr_d(_jit,r0,r1,r2)
static void _uneqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define uneqi_d(r0,r1,i0)		_uneqi_d(_jit,r0,r1,i0)
static void _uneqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define unger_d(r0,r1,r2)		dccr(CC_PL,r0,r1,r2)
#  define ungei_d(r0,r1,i0)		dcci(CC_PL,r0,r1,i0)
#  define ungtr_d(r0,r1,r2)		dccr(CC_HI,r0,r1,r2)
#  define ungti_d(r0,r1,i0)		dcci(CC_HI,r0,r1,i0)
#  define ltgtr_d(r0,r1,r2)		_ltgtr_d(_jit,r0,r1,r2)
static void _ltgtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ltgti_d(r0,r1,i0)		_ltgti_d(_jit,r0,r1,i0)
static void _ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define ordr_d(r0,r1,r2)		dccr(CC_VC,r0,r1,r2)
#  define ordi_d(r0,r1,i0)		dcci(CC_VC,r0,r1,i0)
#  define unordr_d(r0,r1,r2)		dccr(CC_VS,r0,r1,r2)
#  define unordi_d(r0,r1,i0)		dcci(CC_VS,r0,r1,i0)
#define dbccr(cc,i0,r0,r1)		_dbccr(_jit,cc,i0,r0,r1)
static jit_word_t
_dbccr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#define dbcci(cc,i0,r0,i1)		_dbcci(_jit,cc,i0,r0,i1)
static jit_word_t
_dbcci(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_float64_t);
#  define bltr_d(i0,r0,r1)		dbccr(BCC_MI,i0,r0,r1)
#  define blti_d(i0,r0,i1)		dbcci(BCC_MI,i0,r0,i1)
#  define bler_d(i0,r0,r1)		dbccr(BCC_LS,i0,r0,r1)
#  define blei_d(i0,r0,i1)		dbcci(BCC_LS,i0,r0,i1)
#  define beqr_d(i0,r0,r1)		dbccr(BCC_EQ,i0,r0,r1)
#  define beqi_d(i0,r0,i1)		dbcci(BCC_EQ,i0,r0,i1)
#  define bger_d(i0,r0,r1)		dbccr(BCC_GE,i0,r0,r1)
#  define bgei_d(i0,r0,i1)		dbcci(BCC_GE,i0,r0,i1)
#  define bgtr_d(i0,r0,r1)		dbccr(BCC_GT,i0,r0,r1)
#  define bgti_d(i0,r0,i1)		dbcci(BCC_GT,i0,r0,i1)
#  define bner_d(i0,r0,r1)		dbccr(BCC_NE,i0,r0,r1)
#  define bnei_d(i0,r0,i1)		dbcci(BCC_NE,i0,r0,i1)
#  define bunltr_d(i0,r0,r1)		dbccr(BCC_LT,i0,r0,r1)
#  define bunlti_d(i0,r0,i1)		dbcci(BCC_LT,i0,r0,i1)
#  define bunler_d(i0,r0,r1)		dbccr(BCC_LE,i0,r0,r1)
#  define bunlei_d(i0,r0,i1)		dbcci(BCC_LE,i0,r0,i1)
#  define buneqr_d(i0,r0,r1)		_buneqr_d(_jit,i0,r0,r1)
static jit_word_t _buneqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi_d(i0,r0,i1)		_buneqi_d(_jit,i0,r0,i1)
static jit_word_t _buneqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bunger_d(i0,r0,r1)		dbccr(BCC_PL,i0,r0,r1)
#  define bungei_d(i0,r0,i1)		dbcci(BCC_PL,i0,r0,i1)
#  define bungtr_d(i0,r0,r1)		dbccr(BCC_HI,i0,r0,r1)
#  define bungti_d(i0,r0,i1)		dbcci(BCC_HI,i0,r0,i1)
#  define bltgtr_d(i0,r0,r1)		_bltgtr_d(_jit,i0,r0,r1)
static jit_word_t _bltgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti_d(i0,r0,i1)		_bltgti_d(_jit,i0,r0,i1)
static jit_word_t _bltgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bordr_d(i0,r0,r1)		dbccr(BCC_VC,i0,r0,r1)
#  define bordi_d(i0,r0,i1)		dbcci(BCC_VC,i0,r0,i1)
#  define bunordr_d(i0,r0,r1)		dbccr(BCC_VS,i0,r0,r1)
#  define bunordi_d(i0,r0,i1)		dbcci(BCC_VS,i0,r0,i1)
#  define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
static void
_osvvv(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Sz,
       jit_int32_t Rd, jit_int32_t Rn, jit_int32_t Rm)
{
    instr_t	i;
    assert(!(Rd &       ~0x1f));
    assert(!(Rn &       ~0x1f));
    assert(!(Rm &       ~0x1f));
    assert(!(Sz &        ~0x3));
    assert(!(Op & ~0xffe0fc00));
    i.w = Op;
    i.size.b = Sz;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    i.Rm.b = Rm;
    ii(i.w);
}

static void
_osvv_(jit_state_t *_jit, jit_int32_t Op,
       jit_int32_t Sz, jit_int32_t Rd, jit_int32_t Rn)
{
    instr_t	i;
    assert(!(Rd &       ~0x1f));
    assert(!(Rn &       ~0x1f));
    assert(!(Sz &        ~0x3));
    assert(!(Op & ~0xfffffc00));
    i.w = Op;
    i.size.b = Sz;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    ii(i.w);
}

static void
_os_vv(jit_state_t *_jit, jit_int32_t Op,
       jit_int32_t Sz, jit_int32_t Rn, jit_int32_t Rm)
{
    instr_t	i;
    assert(!(Rn &       ~0x1f));
    assert(!(Rm &       ~0x1f));
    assert(!(Sz &        ~0x3));
    assert(!(Op & ~0xff20fc1f));
    i.w = Op;
    i.size.b = Sz;
    i.Rn.b = Rn;
    i.Rm.b = Rm;
    ii(i.w);
}

#define fopi(name)							\
static void								\
_##name##i_f(jit_state_t *_jit,						\
	     jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)		\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    movi_f(rn(reg), i0);						\
    name##r_f(r0, r1, rn(reg));						\
    jit_unget_reg(reg);							\
}
#define dopi(name)							\
static void								\
_##name##i_d(jit_state_t *_jit,						\
	     jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)		\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    movi_d(rn(reg), i0);						\
    name##r_d(r0, r1, rn(reg));						\
    jit_unget_reg(reg);							\
}
#define fbopi(name)							\
static jit_word_t							\
_b##name##i_f(jit_state_t *_jit,					\
	      jit_word_t i0, jit_int32_t r0, jit_float32_t i1)		\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    movi_f(rn(reg), i1);						\
    word = b##name##r_f(i0, r0, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}
#define dbopi(name)							\
static jit_word_t							\
_b##name##i_d(jit_state_t *_jit,					\
	      jit_word_t i0, jit_int32_t r0, jit_float64_t i1)		\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    movi_d(rn(reg), i1);						\
    word = b##name##r_d(i0, r0, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}

static void
_truncr_f_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    FCVTSZ_WS(r0, r1);
    extr_i(r0, r0);
}

static void
_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    FCVTSZ_WD(r0, r1);
    extr_i(r0, r0);
}

fopi(add)
fopi(sub)
fopi(rsb)
fopi(mul)
fopi(div)

static void
_ldr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldr_i(rn(reg), r1);
    FMOVSW(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldi_i(rn(reg), i0);
    FMOVSW(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldxr_i(rn(reg), r1, r2);
    FMOVSW(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldxi_i(rn(reg), r1, i0);
    FMOVSW(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_str_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVWS(rn(reg), r1);
    str_i(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVWS(rn(reg), r0);
    sti_i(i0, rn(reg));
    jit_unget_reg(reg);
}

static void
_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVWS(rn(reg), r2);
    stxr_i(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVWS(rn(reg), r1);
    stxi_i(i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_movr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	FMOVS(r0, r1);
}

static void
_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t i0)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } u;
    jit_int32_t		reg;
    u.f = i0;
    if (u.i == 0)
	FMOVSW(r0, WZR_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	/* prevent generating unused top 32 bits */
	movi(rn(reg), ((jit_word_t)u.i) & 0xffffffff);
	FMOVSW(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_fccr(jit_state_t *_jit, jit_int32_t cc,
      jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPES(r1, r2);
    CSET(r0, cc);
}

static void
_fcci(jit_state_t *_jit, jit_int32_t cc,
      jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    movi_f(rn(reg), i0);
    fccr(cc, r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_uneqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMPES(r1, r2);
    CSET(r0, CC_VS);
    w = _jit->pc.w;
    B_C(BCC_VS, 1);		/* unordered satisfies condition */
    CSET(r0, CC_EQ);		/* equal satisfies condition */
    patch_at(w, _jit->pc.w);
}
fopi(uneq)

static void
_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMPES(r1, r2);
    CSET(r0, CC_VC);		/* set to 1 if ordered */
    w = _jit->pc.w;
    B_C(BCC_VS, 1);		/* unordered does not satisfy condition */
    CSET(r0, CC_NE);		/* set to 1 if not equal */
    patch_at(w, _jit->pc.w);
}
fopi(ltgt)

static jit_word_t
_fbccr(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, d;
    FCMPES(r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) >> 2;
    B_C(cc, d);
    return (w);
}

static jit_word_t
_fbcci(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_float32_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi_f(rn(reg), i1);
    w = fbccr(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_buneqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		u, v, w;
    FCMPES(r0, r1);
    u = _jit->pc.w;
    B_C(BCC_VS, 1);		/* unordered satisfies condition */
    v = _jit->pc.w;
    B_C(BCC_NE, 1);		/* not equal (or unordered) does not satisfy */
    patch_at(u, _jit->pc.w);
    w = _jit->pc.w;
    B((i0 - w) >> 2);
    patch_at(v, _jit->pc.w);
    return (w);
}
fbopi(uneq)

static jit_word_t
_bltgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		u, v, w;
    FCMPES(r0, r1);
    u = _jit->pc.w;
    B_C(BCC_VS, 2);		/* jump over if unordered */
    v = _jit->pc.w;
    B_C(BCC_EQ, 1);		/* jump over if equal */
    w = _jit->pc.w;
    B((i0 - w) >> 2);
    patch_at(u, _jit->pc.w);
    patch_at(v, _jit->pc.w);
    return (w);
}
fbopi(ltgt)

dopi(add)
dopi(sub)
dopi(rsb)
dopi(mul)
dopi(div)

static void
_ldr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldr_l(rn(reg), r1);
    FMOVDX(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldi_l(rn(reg), i0);
    FMOVDX(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldxr_l(rn(reg), r1, r2);
    FMOVDX(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    ldxi_l(rn(reg), r1, i0);
    FMOVDX(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_str_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVXD(rn(reg), r1);
    str_l(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVXD(rn(reg), r0);
    sti_l(i0, rn(reg));
    jit_unget_reg(reg);
}

static void
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVXD(rn(reg), r2);
    stxr_l(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    FMOVXD(rn(reg), r1);
    stxi_l(i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	FMOVD(r0, r1);
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t i0)
{
    union {
	jit_int64_t	l;
	jit_float64_t	d;
    } u;
    jit_int32_t		reg;
    u.d = i0;
    if (u.l == 0)
	FMOVDX(r0, XZR_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), u.l);
	FMOVDX(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_dccr(jit_state_t *_jit, jit_int32_t cc,
      jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPED(r1, r2);
    CSET(r0, cc);
}

static void
_dcci(jit_state_t *_jit, jit_int32_t cc,
      jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    movi_d(rn(reg), i0);
    dccr(cc, r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMPED(r1, r2);
    CSET(r0, CC_VS);
    w = _jit->pc.w;
    B_C(BCC_VS, 1);		/* unordered satisfies condition */
    CSET(r0, CC_EQ);		/* equal satisfies condition */
    patch_at(w, _jit->pc.w);
}
dopi(uneq)

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMPED(r1, r2);
    CSET(r0, CC_VC);		/* set to 1 if ordered */
    w = _jit->pc.w;
    B_C(BCC_VS, 1);		/* unordered does not satisfy condition */
    CSET(r0, CC_NE);		/* set to 1 if not equal */
    patch_at(w, _jit->pc.w);
}
dopi(ltgt)

static jit_word_t
_dbccr(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, d;
    FCMPED(r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) >> 2;
    B_C(cc, d);
    return (w);
}

static jit_word_t
_dbcci(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_float64_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi_d(rn(reg), i1);
    w = dbccr(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		u, v, w;
    FCMPED(r0, r1);
    u = _jit->pc.w;
    B_C(BCC_VS, 1);		/* unordered satisfies condition */
    v = _jit->pc.w;
    B_C(BCC_NE, 1);		/* not equal (or unordered) does not satisfy */
    patch_at(u, _jit->pc.w);
    w = _jit->pc.w;
    B((i0 - w) >> 2);
    patch_at(v, _jit->pc.w);
    return (w);
}
dbopi(uneq)

static jit_word_t
_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		u, v, w;
    FCMPED(r0, r1);
    u = _jit->pc.w;
    B_C(BCC_VS, 2);		/* jump over if unordered */
    v = _jit->pc.w;
    B_C(BCC_EQ, 1);		/* jump over if equal */
    w = _jit->pc.w;
    B((i0 - w) >> 2);
    patch_at(u, _jit->pc.w);
    patch_at(v, _jit->pc.w);
    return (w);
}
dbopi(ltgt)

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		ge_code;
    jit_word_t		lt_code;
    jit_int32_t		rg0, rg1;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg(jit_class_gpr);
    rg1 = jit_get_reg(jit_class_gpr);

    /* Load the fp offset in save area in the first temporary. */
    ldxi_i(rn(rg0), r1, offsetof(jit_va_list_t, fpoff));

    /* Jump over if there are no remaining arguments in the save area. */
    ge_code = bgei(_jit->pc.w, rn(rg0), 0);

    /* Load the gp save pointer in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, fptop));

    /* Load the vararg argument in the first argument. */
    ldxr_d(r0, rn(rg1), rn(rg0));

    /* Update the fp offset. */
    addi(rn(rg0), rn(rg0), 16);
    stxi_i(offsetof(jit_va_list_t, fpoff), r1, rn(rg0));

    /* Will only need one temporary register below. */
    jit_unget_reg(rg1);

    /* Jump over overflow code. */
    lt_code = jmpi_p(_jit->pc.w);

    /* Where to land if argument is in overflow area. */
    patch_at(ge_code, _jit->pc.w);

    /* Load stack pointer. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, stack));

    /* Load argument. */
    ldr_d(r0, rn(rg0));

    /* Update stack pointer. */
    addi(rn(rg0), rn(rg0), 8);
    stxi(offsetof(jit_va_list_t, stack), r1, rn(rg0));

    /* Where to land if argument is in gp save area. */
    patch_at(lt_code, _jit->pc.w);

    jit_unget_reg(rg0);
}
#endif
