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
#define FA(o,d,a,b,c,x)			_FA(_jit,o,d,a,b,c,x,0)
#define FA_(o,d,a,b,c,x)		_FA(_jit,o,d,a,b,c,x,1)
static void _FA(jit_state_t*,int,int,int,int,int,int,int);
#define FXFL(o,m,b,x)			_FXFL(_jit,o,m,b,x,0)
#define FXFL_(o,m,b,x)			_FXFL(_jit,o,m,b,x,1)
static void _FXFL(jit_state_t*,int,int,int,int,int) maybe_unused;
#  define FABS(d,b)			FX(63,d,0,b,264)
#  define FABS_(d,b)			FX_(63,d,0,b,264)
#  define FADD(d,a,b)			FA(63,d,a,b,0,21)
#  define FADD_(d,a,b)			FA_(63,d,a,b,0,21)
#  define FADDS(d,a,b)			FA(59,d,a,b,0,21)
#  define FADDS_(d,a,b)			FA_(59,d,a,b,0,21)
#  define FCFID(d,b)			FX(63,d,0,b,846)
#  define FCMPO(cr,a,b)			FC(63,cr,0,a,b,32)
#  define FCMPU(cr,a,b)			FC(63,cr,0,a,b,0)
#  define FCTIW(d,b)			FX(63,d,0,b,14)
#  define FCTIW_(d,b)			FX_(63,d,0,b,14)
#  define FCTIWZ(d,b)			FX(63,d,0,b,15)
#  define FCTIWZ_(d,b)			FX_(63,d,0,b,15)
#  define FCTID(d,b)			FX(63,d,0,b,814)
#  define FCTID_(d,b)			FX_(63,d,0,b,814)
#  define FCTIDZ(d,b)			FX(63,d,0,b,815)
#  define FCTIDZ_(d,b)			FX_(63,d,0,b,815)
#  define FDIV(d,a,b)			FA(63,d,a,b,0,18)
#  define FDIV_(d,a,b)			FA_(63,d,a,b,0,18)
#  define FDIVS(d,a,b)			FA(59,d,a,b,0,18)
#  define FDIVS_(d,a,b)			FA_(59,d,a,b,0,18)
#  define FMADD(d,a,b,c)		FA(63,d,a,b,c,29)
#  define FMADD_(d,a,b,c)		FA(63,d,a,b,c,29)
#  define FMADDS(d,a,b,c)		FA(59,d,a,b,c,29)
#  define FMADDS_(d,a,b,c)		FA(59,d,a,b,c,29)
#  define FMR(d,b)			FX(63,d,0,b,72)
#  define FMR_(d,b)			FX_(63,d,0,b,72)
#  define FMSUB(d,a,b,c)		FA(63,d,a,b,c,28)
#  define FMSUB_(d,a,b,c)		FA(63,d,a,b,c,28)
#  define FMSUBS(d,a,b,c)		FA(59,d,a,b,c,28)
#  define FMSUBS_(d,a,b,c)		FA(59,d,a,b,c,28)
#  define FMUL(d,a,c)			FA(63,d,a,0,c,25)
#  define FMUL_(d,a,c)			FA_(63,d,a,0,c,25)
#  define FMULS(d,a,c)			FA(59,d,a,0,c,25)
#  define FMULS_(d,a,c)			FA_(59,d,a,0,c,25)
#  define FNABS(d,b)			FX(63,d,0,b,136)
#  define FNABS_(d,b)			FX_(63,d,0,b,136)
#  define FNEG(d,b)			FX(63,d,0,b,40)
#  define FNEG_(d,b)			FX_(63,d,0,b,40)
#  define FNMADD(d,a,b,c)		FA(63,d,a,b,c,31)
#  define FNMADD_(d,a,b,c)		FA_(63,d,a,b,c,31)
#  define FNMADDS(d,a,b,c)		FA(59,d,a,b,c,31)
#  define FNMADDS_(d,a,b,c)		FA_(59,d,a,b,c,31)
#  define FNMSUB(d,a,b,c)		FA(63,d,a,b,c,30)
#  define FNMSUB_(d,a,b,c)		FA_(63,d,a,b,c,30)
#  define FNMSUBS(d,a,b,c)		FA(59,d,a,b,c,30)
#  define FNMSUBS_(d,a,b,c)		FA_(59,d,a,b,c,30)
#  define FRES(d,b)			FA(59,d,0,b,0,24)
#  define FRES_(d,b)			FA_(59,d,0,b,0,24)
#  define FRSP(d,b)			FA(63,d,0,b,0,12)
#  define FRSP_(d,b)			FA_(63,d,0,b,0,12)
#  define FRSQTRE(d,b)			FA(63,d,0,b,0,26)
#  define FRSQTRE_(d,b)			FA_(63,d,0,b,0,26)
#  define FSEL(d,a,b,c)			FA(63,d,a,b,c,23)
#  define FSEL_(d,a,b,c)		FA_(63,d,a,b,c,23)
#  define FSQRT(d,b)			FA(63,d,0,b,0,22)
#  define FSQRT_(d,b)			FA_(63,d,0,b,0,22)
#  define FSQRTS(d,b)			FA(59,d,0,b,0,22)
#  define FSQRTS_(d,b)			FA_(59,d,0,b,0,22)
#  define FSUB(d,a,b)			FA(63,d,a,b,0,20)
#  define FSUB_(d,a,b)			FA(63,d,a,b,0,20)
#  define FSUBS(d,a,b)			FA(59,d,a,b,0,20)
#  define FSUBS_(d,a,b)			FA(59,d,a,b,0,20)
#  define LFD(d,a,s)			FDs(50,d,a,s)
#  define LFDU(d,a,s)			FDs(51,d,a,s)
#  define LFDUX(d,a,b)			FX(31,d,a,b,631)
#  define LFDX(d,a,b)			FX(31,d,a,b,599)
#  define LFS(d,a,s)			FDs(48,d,a,s)
#  define LFSU(d,a,s)			FDs(49,d,a,s)
#  define LFSUX(d,a,b)			FX(31,d,a,b,567)
#  define LFSX(d,a,b)			FX(31,d,a,b,535)
#  define MCRFS(d,s)			FXL(63,d<<2,(s)<<2,64)
#  define MFFS(d)			FX(63,d,0,0,583)
#  define MFFS_(d)			FX_(63,d,0,0,583)
#  define MTFSB0(d)			FX(63,d,0,0,70)
#  define MTFSB0_(d)			FX_(63,d,0,0,70)
#  define MTFSB1(d)			FX(63,d,0,0,38)
#  define MTFSB1_(d)			FX_(63,d,0,0,38)
#  define MTFSF(m,b)			FXFL(63,m,b,711)
#  define MTFSF_(m,b)			FXFL_(63,m,b,711)
#  define MTFSFI(d,i)			FX(63,d<<2,0,i<<1,134)
#  define MTFSFI_(d,i)			FX_(63,d<<2,0,i<<1,134)
#  define STFD(s,a,d)			FDs(54,s,a,d)
#  define STFDU(s,a,d)			FDs(55,s,a,d)
#  define STFDUX(s,a,b)			FX(31,s,a,b,759)
#  define STFDX(s,a,b)			FX(31,s,a,b,727)
#  define STFIWX(s,a,b)			FX(31,s,a,b,983)
#  define STFS(s,a,d)			FDs(52,s,a,d)
#  define STFSU(s,a,d)			FDs(53,s,a,d)
#  define STFSUX(s,a,b)			FX(31,s,a,b,695)
#  define STFSX(s,a,b)			FX(31,s,a,b,663)
#  define movr_f(r0,r1)			movr_d(r0,r1)
#  define movr_d(r0,r1)			_movr_d(_jit,r0,r1)
static void _movr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_f(r0,i0)			_movi_f(_jit,r0,i0)
static void _movi_f(jit_state_t*,jit_int32_t,jit_float32_t*);
#  define movi_d(r0,i0)			_movi_d(_jit,r0,i0)
static void _movi_d(jit_state_t*,jit_int32_t,jit_float64_t*);
#  define extr_f(r0,r1)			extr_d(r0,r1)
#  define extr_d(r0,r1)			_extr_d(_jit,r0,r1)
static void _extr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define truncr_f(r0,r1)		truncr_d(r0,r1)
#  define truncr_f_i(r0,r1)		truncr_d_i(r0,r1)
#  define truncr_d_i(r0,r1)		_truncr_d_i(_jit,r0,r1)
static void _truncr_d_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  if __WORDSIZE == 32
#    define truncr_d(r0,r1)		truncr_d_i(r0,r1)
#  else
#    define truncr_d(r0,r1)		truncr_d_l(r0,r1)
#    define truncr_f_l(r0,r1)		truncr_d_l(r0,r1)
#    define truncr_d_l(r0,r1)		_truncr_d_l(_jit,r0,r1)
static void _truncr_d_l(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#  define extr_d_f(r0,r1)		FRSP(r0,r1)
#  define extr_f_d(r0,r1)		movr_d(r0,r1)
#  define absr_f(r0,r1)			absr_d(r0,r1)
#  define absr_d(r0,r1)			FABS(r0,r1)
#  define negr_f(r0,r1)			negr_d(r0,r1)
#  define negr_d(r0,r1)			FNEG(r0,r1)
#  define sqrtr_f(r0,r1)		FSQRTS(r0,r1)
#  define sqrtr_d(r0,r1)		FSQRT(r0,r1)
#  define addr_f(r0,r1,r2)		FADDS(r0,r1,r2)
#  define addr_d(r0,r1,r2)		FADD(r0,r1,r2)
#  define addi_f(r0,r1,i0)		_addi_f(_jit,r0,r1,i0)
static void _addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define addi_d(r0,r1,i0)		_addi_d(_jit,r0,r1,i0)
static void _addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define subr_f(r0,r1,r2)		FSUBS(r0,r1,r2)
#  define subi_f(r0,r1,i0)		_subi_f(_jit,r0,r1,i0)
static void _subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define subr_d(r0,r1,r2)		FSUB(r0,r1,r2)
#  define subi_d(r0,r1,i0)		_subi_d(_jit,r0,r1,i0)
static void _subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define rsbr_f(r0,r1,r2)		subr_f(r0,r2,r1)
#  define rsbi_f(r0,r1,i0)		_rsbi_f(_jit,r0,r1,i0)
static void _rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define rsbr_d(r0,r1,r2)		subr_d(r0,r2,r1)
#  define rsbi_d(r0,r1,i0)		_rsbi_d(_jit,r0,r1,i0)
static void _rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define mulr_f(r0,r1,r2)		FMULS(r0,r1,r2)
#  define muli_f(r0,r1,i0)		_muli_f(_jit,r0,r1,i0)
static void _muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define mulr_d(r0,r1,r2)		FMUL(r0,r1,r2)
#  define muli_d(r0,r1,i0)		_muli_d(_jit,r0,r1,i0)
static void _muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define divr_f(r0,r1,r2)		FDIVS(r0,r1,r2)
#  define divi_f(r0,r1,i0)		_divi_f(_jit,r0,r1,i0)
static void _divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define divr_d(r0,r1,r2)		FDIV(r0,r1,r2)
#  define divi_d(r0,r1,i0)		_divi_d(_jit,r0,r1,i0)
static void _divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ltr_f(r0,r1,r2)		ltr_d(r0,r1,r2)
#  define ltr_d(r0,r1,r2)		_ltr_d(_jit,r0,r1,r2)
static void _ltr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lti_f(r0,r1,i0)		_lti_f(_jit,r0,r1,i0)
static void _lti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define lti_d(r0,r1,i0)		_lti_d(_jit,r0,r1,i0)
static void _lti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ler_f(r0,r1,r2)		ler_d(r0,r1,r2)
#  define ler_d(r0,r1,r2)		_ler_d(_jit,r0,r1,r2)
static void _ler_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lei_f(r0,r1,i0)		_lei_f(_jit,r0,r1,i0)
static void _lei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define lei_d(r0,r1,i0)		_lei_d(_jit,r0,r1,i0)
static void _lei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define eqr_f(r0,r1,r2)		eqr_d(r0,r1,r2)
#  define eqr_d(r0,r1,r2)		_eqr_d(_jit,r0,r1,r2)
static void _eqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define eqi_f(r0,r1,i0)		_eqi_f(_jit,r0,r1,i0)
static void _eqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define eqi_d(r0,r1,i0)		_eqi_d(_jit,r0,r1,i0)
static void _eqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ger_f(r0,r1,r2)		ger_d(r0,r1,r2)
#  define ger_d(r0,r1,r2)		_ger_d(_jit,r0,r1,r2)
static void _ger_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gei_f(r0,r1,i0)		_gei_f(_jit,r0,r1,i0)
static void _gei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define gei_d(r0,r1,i0)		_gei_d(_jit,r0,r1,i0)
static void _gei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define gtr_f(r0,r1,r2)		gtr_d(r0,r1,r2)
#  define gtr_d(r0,r1,r2)		_gtr_d(_jit,r0,r1,r2)
static void _gtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gti_f(r0,r1,i0)		_gti_f(_jit,r0,r1,i0)
static void _gti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define gti_d(r0,r1,i0)		_gti_d(_jit,r0,r1,i0)
static void _gti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ner_f(r0,r1,r2)		ner_d(r0,r1,r2)
#  define ner_d(r0,r1,r2)		_ner_d(_jit,r0,r1,r2)
static void _ner_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nei_f(r0,r1,i0)		_nei_f(_jit,r0,r1,i0)
static void _nei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define nei_d(r0,r1,i0)		_nei_d(_jit,r0,r1,i0)
static void _nei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unltr_f(r0,r1,r2)		unltr_d(r0,r1,r2)
#  define unltr_d(r0,r1,r2)		_unltr_d(_jit,r0,r1,r2)
static void _unltr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unlti_f(r0,r1,i0)		_unlti_f(_jit,r0,r1,i0)
static void _unlti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define unlti_d(r0,r1,i0)		_unlti_d(_jit,r0,r1,i0)
static void _unlti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unler_f(r0,r1,r2)		unler_d(r0,r1,r2)
#  define unler_d(r0,r1,r2)		_unler_d(_jit,r0,r1,r2)
static void _unler_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unlei_f(r0,r1,i0)		_unlei_f(_jit,r0,r1,i0)
static void _unlei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define unlei_d(r0,r1,i0)		_unlei_d(_jit,r0,r1,i0)
static void _unlei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define uneqr_f(r0,r1,r2)		uneqr_d(r0,r1,r2)
#  define uneqr_d(r0,r1,r2)		_uneqr_d(_jit,r0,r1,r2)
static void _uneqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define uneqi_f(r0,r1,i0)		_uneqi_f(_jit,r0,r1,i0)
static void _uneqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define uneqi_d(r0,r1,i0)		_uneqi_d(_jit,r0,r1,i0)
static void _uneqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unger_f(r0,r1,r2)		unger_d(r0,r1,r2)
#  define unger_d(r0,r1,r2)		_unger_d(_jit,r0,r1,r2)
static void _unger_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ungei_f(r0,r1,i0)		_ungei_f(_jit,r0,r1,i0)
static void _ungei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ungei_d(r0,r1,i0)		_ungei_d(_jit,r0,r1,i0)
static void _ungei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ungtr_f(r0,r1,r2)		ungtr_d(r0,r1,r2)
#  define ungtr_d(r0,r1,r2)		_ungtr_d(_jit,r0,r1,r2)
static void _ungtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ungti_f(r0,r1,i0)		_ungti_f(_jit,r0,r1,i0)
static void _ungti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ungti_d(r0,r1,i0)		_ungti_d(_jit,r0,r1,i0)
static void _ungti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ltgtr_f(r0,r1,r2)		ltgtr_d(r0,r1,r2)
#  define ltgtr_d(r0,r1,r2)		_ltgtr_d(_jit,r0,r1,r2)
static void _ltgtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ltgti_f(r0,r1,i0)		_ltgti_f(_jit,r0,r1,i0)
static void _ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ltgti_d(r0,r1,i0)		_ltgti_d(_jit,r0,r1,i0)
static void _ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ordr_f(r0,r1,r2)		ordr_d(r0,r1,r2)
#  define ordr_d(r0,r1,r2)		_ordr_d(_jit,r0,r1,r2)
static void _ordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ordi_f(r0,r1,i0)		_ordi_f(_jit,r0,r1,i0)
static void _ordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ordi_d(r0,r1,i0)		_ordi_d(_jit,r0,r1,i0)
static void _ordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unordr_f(r0,r1,r2)		unordr_d(r0,r1,r2)
#  define unordr_d(r0,r1,r2)		_unordr_d(_jit,r0,r1,r2)
static void _unordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unordi_f(r0,r1,i0)		_unordi_f(_jit,r0,r1,i0)
static void _unordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define unordi_d(r0,r1,i0)		_unordi_d(_jit,r0,r1,i0)
static void _unordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define bltr_f(i0,r0,r1)		bltr_d(i0,r0,r1)
#  define bltr_d(i0,r0,r1)		_bltr_d(_jit,i0,r0,r1)
static jit_word_t _bltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti_f(i0,r0,i1)		_blti_f(_jit,i0,r0,i1)
static jit_word_t _blti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define blti_d(i0,r0,i1)		_blti_d(_jit,i0,r0,i1)
static jit_word_t _blti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bler_f(i0,r0,r1)		bler_d(i0,r0,r1)
#  define bler_d(i0,r0,r1)		_bler_d(_jit,i0,r0,r1)
static jit_word_t _bler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei_f(i0,r0,i1)		_blei_f(_jit,i0,r0,i1)
static jit_word_t _blei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define blei_d(i0,r0,i1)		_blei_d(_jit,i0,r0,i1)
static jit_word_t _blei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define beqr_f(i0,r0,r1)		beqr_d(i0,r0,r1)
#  define beqr_d(i0,r0,r1)		_beqr_d(_jit,i0,r0,r1)
static jit_word_t _beqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define beqi_f(i0,r0,i1)		_beqi_f(_jit,i0,r0,i1)
static jit_word_t _beqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define beqi_d(i0,r0,i1)		_beqi_d(_jit,i0,r0,i1)
static jit_word_t _beqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bger_f(i0,r0,r1)		bger_d(i0,r0,r1)
#  define bger_d(i0,r0,r1)		_bger_d(_jit,i0,r0,r1)
static jit_word_t _bger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei_f(i0,r0,i1)		_bgei_f(_jit,i0,r0,i1)
static jit_word_t _bgei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bgei_d(i0,r0,i1)		_bgei_d(_jit,i0,r0,i1)
static jit_word_t _bgei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bgtr_f(i0,r0,r1)		bgtr_d(i0,r0,r1)
#  define bgtr_d(i0,r0,r1)		_bgtr_d(_jit,i0,r0,r1)
static jit_word_t _bgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti_f(i0,r0,i1)		_bgti_f(_jit,i0,r0,i1)
static jit_word_t _bgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bgti_d(i0,r0,i1)		_bgti_d(_jit,i0,r0,i1)
static jit_word_t _bgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bner_f(i0,r0,r1)		bner_d(i0,r0,r1)
#  define bner_d(i0,r0,r1)		_bner_d(_jit,i0,r0,r1)
static jit_word_t _bner_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bnei_f(i0,r0,i1)		_bnei_f(_jit,i0,r0,i1)
static jit_word_t _bnei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bnei_d(i0,r0,i1)		_bnei_d(_jit,i0,r0,i1)
static jit_word_t _bnei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bunltr_f(i0,r0,r1)		bunltr_d(i0,r0,r1)
#  define bunltr_d(i0,r0,r1)		_bunltr_d(_jit,i0,r0,r1)
static jit_word_t _bunltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlti_f(i0,r0,i1)		_bunlti_f(_jit,i0,r0,i1)
static jit_word_t _bunlti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bunlti_d(i0,r0,i1)		_bunlti_d(_jit,i0,r0,i1)
static jit_word_t _bunlti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bunler_f(i0,r0,r1)		bunler_d(i0,r0,r1)
#  define bunler_d(i0,r0,r1)		_bunler_d(_jit,i0,r0,r1)
static jit_word_t _bunler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlei_f(i0,r0,i1)		_bunlei_f(_jit,i0,r0,i1)
static jit_word_t _bunlei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bunlei_d(i0,r0,i1)		_bunlei_d(_jit,i0,r0,i1)
static jit_word_t _bunlei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define buneqr_f(i0,r0,r1)		buneqr_d(i0,r0,r1)
#  define buneqr_d(i0,r0,r1)		_buneqr_d(_jit,i0,r0,r1)
static jit_word_t _buneqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi_f(i0,r0,i1)		_buneqi_f(_jit,i0,r0,i1)
static jit_word_t _buneqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define buneqi_d(i0,r0,i1)		_buneqi_d(_jit,i0,r0,i1)
static jit_word_t _buneqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bunger_f(i0,r0,r1)		bunger_d(i0,r0,r1)
#  define bunger_d(i0,r0,r1)		_bunger_d(_jit,i0,r0,r1)
static jit_word_t _bunger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungei_f(i0,r0,i1)		_bungei_f(_jit,i0,r0,i1)
static jit_word_t _bungei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bungei_d(i0,r0,i1)		_bungei_d(_jit,i0,r0,i1)
static jit_word_t _bungei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bungtr_f(i0,r0,r1)		bungtr_d(i0,r0,r1)
#  define bungtr_d(i0,r0,r1)		_bungtr_d(_jit,i0,r0,r1)
static jit_word_t _bungtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungti_f(i0,r0,i1)		_bungti_f(_jit,i0,r0,i1)
static jit_word_t _bungti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bungti_d(i0,r0,i1)		_bungti_d(_jit,i0,r0,i1)
static jit_word_t _bungti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bltgtr_f(i0,r0,r1)		bltgtr_d(i0,r0,r1)
#  define bltgtr_d(i0,r0,r1)		_bltgtr_d(_jit,i0,r0,r1)
static jit_word_t _bltgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti_f(i0,r0,i1)		_bltgti_f(_jit,i0,r0,i1)
static jit_word_t _bltgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bltgti_d(i0,r0,i1)		_bltgti_d(_jit,i0,r0,i1)
static jit_word_t _bltgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bordr_f(i0,r0,r1)		bordr_d(i0,r0,r1)
#  define bordr_d(i0,r0,r1)		_bordr_d(_jit,i0,r0,r1)
static jit_word_t _bordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bordi_f(i0,r0,i1)		_bordi_f(_jit,i0,r0,i1)
static jit_word_t _bordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bordi_d(i0,r0,i1)		_bordi_d(_jit,i0,r0,i1)
static jit_word_t _bordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bunordr_f(i0,r0,r1)		bunordr_d(i0,r0,r1)
#  define bunordr_d(i0,r0,r1)		_bunordr_d(_jit,i0,r0,r1)
static jit_word_t _bunordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunordi_f(i0,r0,i1)		_bunordi_f(_jit,i0,r0,i1)
static jit_word_t _bunordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bunordi_d(i0,r0,i1)		_bunordi_d(_jit,i0,r0,i1)
static jit_word_t _bunordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define ldr_f(r0,r1)			LFSX(r0, _R0_REGNO, r1)
#  define ldi_f(r0,i0)			_ldi_f(_jit,r0,i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_f(r0,r1,r2)		_ldxr_f(_jit,r0,r1,r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_f(r0,r1,i0)		_ldxi_f(_jit,r0,r1,i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_f(r0,r1)			STFSX(r1, _R0_REGNO, r0)
#  define sti_f(i0,r0)			_sti_f(_jit,i0,r0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_f(r0,r1,r2)		_stxr_f(_jit,r0,r1,r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_f(i0,r0,r1)		_stxi_f(_jit,i0,r0,r1)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define ldr_d(r0,r1)			LFDX(r0, _R0_REGNO, r1)
#  define ldi_d(r0,i0)			_ldi_d(_jit,r0,i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_d(r0,r1,r2)		_ldxr_d(_jit,r0,r1,r2)
static void _ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_d(r0,r1,i0)		_ldxi_d(_jit,r0,r1,i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_d(r0,r1)			STFDX(r1, _R0_REGNO, r0)
#  define sti_d(i0,r0)			_sti_d(_jit,i0,r0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_d(r0,r1,r2)		_stxr_d(_jit,r0,r1,r2)
static void _stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_d(i0,r0,r1)		_stxi_d(_jit,i0,r0,r1)
static void _stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#endif

#if CODE
#  define _u16(v)			((v) & 0xffff)
static void
_FA(jit_state_t *_jit, int o, int d, int a, int b, int c, int x, int r)
{
    assert(!(o & ~((1 << 6) - 1)));
    assert(!(d & ~((1 << 5) - 1)));
    assert(!(a & ~((1 << 5) - 1)));
    assert(!(b & ~((1 << 5) - 1)));
    assert(!(c & ~((1 << 5) - 1)));
    assert(!(x & ~((1 << 5) - 1)));
    assert(!(r & ~((1 << 1) - 1)));
    ii((o<<26)|(d<<21)|(a<<16)|(b<<11)|(c<<6)|(x<<1)|r);
}

static void
_FXFL(jit_state_t *_jit, int o, int m, int b, int x, int r)
{
    assert(!(o & ~((1 <<  6) - 1)));
    assert(!(m & ~((1 <<  8) - 1)));
    assert(!(b & ~((1 <<  5) - 1)));
    assert(!(x & ~((1 << 10) - 1)));
    assert(!(r & ~((1 <<  1) - 1)));
    ii((o<<26)|(m<<17)|(b<<11)|(x<<1)|r);
}

static void
_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	FMR(r0,r1);
}

static void
_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t *i0)
{
    union {
	jit_int32_t	 i;
	jit_float32_t	 f;
    } data;
    jit_int32_t		 reg;

    if (_jitc->no_data) {
	data.f = *i0;
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.i & 0xffffffff);
	stxi_i(alloca_offset - 4, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
	ldxi_f(r0, _FP_REGNO, alloca_offset - 4);
    }
    else
	ldi_f(r0, (jit_word_t)i0);
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
	jit_int32_t	 i[2];
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;

    if (_jitc->no_data) {
	data.d = *i0;
	reg = jit_get_reg(jit_class_gpr);
#  if __WORDSIZE == 32
	movi(rn(reg), data.i[0]);
	stxi(alloca_offset - 8, _FP_REGNO, rn(reg));
	movi(rn(reg), data.i[1]);
	stxi(alloca_offset - 4, _FP_REGNO, rn(reg));
#  else
	movi(rn(reg), data.w);
	stxi(alloca_offset - 8, _FP_REGNO, rn(reg));
#  endif
	jit_unget_reg(reg);
	ldxi_d(r0, _FP_REGNO, alloca_offset - 8);
    }
    else
	ldi_d(r0, (jit_word_t)i0);
}

/* should only work on newer ppc (fcfid is a ppc64 instruction) */
static void
_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#  if __WORDSIZE == 32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    rshi(rn(reg), r1, 31);
    /* use reserved 8 bytes area */
    stxi(alloca_offset - 4, _FP_REGNO, r1);
    stxi(alloca_offset - 8, _FP_REGNO, rn(reg));
    jit_unget_reg(reg);
#  else
    stxi(alloca_offset - 8, _FP_REGNO, r1);
#  endif
    ldxi_d(r0, _FP_REGNO, alloca_offset - 8);
    FCFID(r0, r0);
}

static void
_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    FCTIWZ(rn(reg), r1);
    /* use reserved 8 bytes area */
    stxi_d(alloca_offset - 8, _FP_REGNO, rn(reg));
    ldxi_i(r0, _FP_REGNO, alloca_offset - 4);
    jit_unget_reg(reg);
}

#  if __WORDSIZE == 64
static void
_truncr_d_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    FCTIDZ(rn(reg), r1);
    /* use reserved 8 bytes area */
    stxi_d(alloca_offset - 8, _FP_REGNO, rn(reg));
    ldxi(r0, _FP_REGNO, alloca_offset - 8);
    jit_unget_reg(reg);
}
#  endif

#  define fpr_opi(name, type, size)					\
static void								\
_##name##i_##type(jit_state_t *_jit,					\
		  jit_int32_t r0, jit_int32_t r1,			\
		  jit_float##size##_t *i0)				\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    movi_##type(rn(reg), i0);						\
    name##r_##type(r0, r1, rn(reg));					\
    jit_unget_reg(reg);							\
}
#  define fpr_bopi(name, type, size)					\
static jit_word_t							\
_b##name##i_##type(jit_state_t *_jit,					\
		  jit_word_t i0, jit_int32_t r0,			\
		  jit_float##size##_t *i1)				\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    movi_##type(rn(reg), i1);						\
    word = b##name##r_##type(i0, r0, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define fopi(name)			fpr_opi(name, f, 32)
#  define fbopi(name)			fpr_bopi(name, f, 32)
#  define dopi(name)			fpr_opi(name, d, 64)
#  define dbopi(name)			fpr_bopi(name, d, 64)

fopi(add)
dopi(add)
fopi(sub)
dopi(sub)
fopi(rsb)
dopi(rsb)
fopi(mul)
dopi(mul)
fopi(div)
dopi(div)

static void
_ltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPO(CR_0, r1, r2);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_LT);
}
fopi(lt)
dopi(lt)

static void
_ler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPO(CR_0, r1, r2);
    CREQV(CR_GT, CR_GT, CR_UN);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_GT);
}
fopi(le)
dopi(le)

static void
_eqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPO(CR_0, r1, r2);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_EQ);
}
fopi(eq)
dopi(eq)

static void
_ger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPO(CR_0, r1, r2);
    CREQV(CR_LT, CR_LT, CR_UN);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_LT);
}
fopi(ge)
dopi(ge)

static void
_gtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPO(CR_0, r1, r2);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_GT);
}
fopi(gt)
dopi(gt)

static void
_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPO(CR_0, r1, r2);
    CRNOT(CR_EQ, CR_EQ);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_EQ);
}
fopi(ne)
dopi(ne)

static void
_unltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    CROR(CR_LT, CR_LT, CR_UN);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_LT);
}
fopi(unlt)
dopi(unlt)

static void
_unler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    CRNOT(CR_GT, CR_GT);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_GT);
}
fopi(unle)
dopi(unle)

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    CROR(CR_EQ, CR_EQ, CR_UN);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_EQ);
}
fopi(uneq)
dopi(uneq)

static void
_unger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    CRNOT(CR_LT, CR_LT);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_LT);
}
fopi(unge)
dopi(unge)

static void
_ungtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    CROR(CR_GT, CR_GT, CR_UN);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_GT);
}
fopi(ungt)
dopi(ungt)

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    CROR(CR_GT, CR_GT, CR_LT);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_GT);
}
fopi(ltgt)
dopi(ltgt)

static void
_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    CRNOT(CR_UN, CR_UN);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_UN);
}
fopi(ord)
dopi(ord)

static void
_unordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPU(CR_0, r1, r2);
    MFCR(r0);
    EXTRWI(r0, r0, 1, CR_UN);
}
fopi(unord)
dopi(unord)

static jit_word_t
_bltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPO(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BLT(d);
    return (w);
}
fbopi(lt)
dbopi(lt)

static jit_word_t
_bler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPO(CR_0, r0, r1);
    CREQV(CR_GT, CR_GT, CR_UN);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BGT(d);
    return (w);
}
fbopi(le)
dbopi(le)

static jit_word_t
_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPO(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BEQ(d);
    return (w);
}
fbopi(eq)
dbopi(eq)

static jit_word_t
_bger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPO(CR_0, r0, r1);
    CREQV(CR_LT, CR_LT, CR_UN);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BLT(d);
    return (w);
}
fbopi(ge)
dbopi(ge)

static jit_word_t
_bgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPO(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BGT(d);
    return (w);
}
fbopi(gt)
dbopi(gt)

static jit_word_t
_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPO(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BNE(d);
    return (w);
}
fbopi(ne)
dbopi(ne)

static jit_word_t
_bunltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    CROR(CR_LT, CR_LT, CR_UN);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BLT(d);
    return (w);
}
fbopi(unlt)
dbopi(unlt)

static jit_word_t
_bunler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BLE(d);
    return (w);
}
fbopi(unle)
dbopi(unle)

static jit_word_t
_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    CROR(CR_EQ, CR_EQ, CR_UN);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BEQ(d);
    return (w);
}
fbopi(uneq)
dbopi(uneq)

static jit_word_t
_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BGE(d);
    return (w);
}
fbopi(unge)
dbopi(unge)

static jit_word_t
_bungtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    CROR(CR_GT, CR_GT, CR_UN);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BGT(d);
    return (w);
}
fbopi(ungt)
dbopi(ungt)

static jit_word_t
_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    CROR(CR_EQ, CR_LT, CR_GT);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BEQ(d);
    return (w);
}
fbopi(ltgt)
dbopi(ltgt)

static jit_word_t
_bordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BNU(d);
    return (w);
}
fbopi(ord)
dbopi(ord)

static jit_word_t
_bunordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    FCMPU(CR_0, r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) & ~3;
    BUN(d);
    return (w);
}
fbopi(unord)
dbopi(unord)

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_bool_t		inv;
    jit_int32_t		reg;
    jit_word_t		lo, hi;
    if (can_sign_extend_short_p(i0))
	LFS(r0, _R0_REGNO, i0);
    else if (can_sign_extend_int_p(i0)) {
	hi = (jit_int16_t)((i0 >> 16) + ((jit_uint16_t)i0 >> 15));
	lo = (jit_int16_t)(i0 - (hi << 16));
	reg = jit_get_reg(jit_class_gpr);
	if ((inv = reg == _R0))		reg = jit_get_reg(jit_class_gpr);
	LIS(rn(reg), hi);
	LFS(r0, rn(reg), lo);
	jit_unget_reg(reg);
	if (inv)			jit_unget_reg(_R0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_bool_t		inv;
    jit_int32_t		reg;
    jit_word_t		lo, hi;
    if (can_sign_extend_short_p(i0))
	LFD(r0, _R0_REGNO, i0);
    else if (can_sign_extend_int_p(i0)) {
	hi = (jit_int16_t)((i0 >> 16) + ((jit_uint16_t)i0 >> 15));
	lo = (jit_int16_t)(i0 - (hi << 16));
	reg = jit_get_reg(jit_class_gpr);
	if ((inv = reg == _R0))		reg = jit_get_reg(jit_class_gpr);
	LIS(rn(reg), hi);
	LFD(r0, rn(reg), lo);
	jit_unget_reg(reg);
	if (inv)			jit_unget_reg(_R0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r1 == _R0_REGNO) {
	if (r2 != _R0_REGNO)
	    LFSX(r0, r2, r1);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r1);
	    LFSX(r0, rn(reg), r2);
	    jit_unget_reg(reg);
	}
    }
    else
	LFSX(r0, r1, r2);
}

static void
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r1 == _R0_REGNO) {
	if (r2 != _R0_REGNO)
	    LFDX(r0, r2, r1);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r1);
	    LFDX(r0, rn(reg), r2);
	    jit_unget_reg(reg);
	}
    }
    else
	LFDX(r0, r1, r2);
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	ldr_f(r0, r1);
    else if (can_sign_extend_short_p(i0)) {
	if (r1 == _R0_REGNO) {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r1);
	    LFS(r0, rn(reg), i0);
	    jit_unget_reg(reg);
	}
	else
	    LFS(r0, r1, i0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_f(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	ldr_d(r0, r1);
    else if (can_sign_extend_short_p(i0)) {
	if (r1 == _R0_REGNO) {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r1);
	    LFD(r0, rn(reg), i0);
	    jit_unget_reg(reg);
	}
	else
	    LFD(r0, r1, i0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_d(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_bool_t		inv;
    jit_int32_t		reg;
    jit_word_t		lo, hi;
    if (can_sign_extend_short_p(i0))
	STFS(r0, _R0_REGNO, i0);
    else if (can_sign_extend_int_p(i0)) {
	hi = (jit_int16_t)((i0 >> 16) + ((jit_uint16_t)i0 >> 15));
	lo = (jit_int16_t)(i0 - (hi << 16));
	reg = jit_get_reg(jit_class_gpr);
	if ((inv = reg == _R0))		reg = jit_get_reg(jit_class_gpr);
	LIS(rn(reg), hi);
	STFS(r0, rn(reg), lo);
	jit_unget_reg(reg);
	if (inv)			jit_unget_reg(_R0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_f(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_bool_t		inv;
    jit_int32_t		reg;
    jit_word_t		lo, hi;
    if (can_sign_extend_short_p(i0))
	STFD(r0, _R0_REGNO, i0);
    else if (can_sign_extend_int_p(i0)) {
	hi = (jit_int16_t)((i0 >> 16) + ((jit_uint16_t)i0 >> 15));
	lo = (jit_int16_t)(i0 - (hi << 16));
	reg = jit_get_reg(jit_class_gpr);
	if ((inv = reg == _R0))		reg = jit_get_reg(jit_class_gpr);
	LIS(rn(reg), hi);
	STFD(r0, rn(reg), lo);
	jit_unget_reg(reg);
	if (inv)			jit_unget_reg(_R0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_d(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == _R0_REGNO) {
	if (r1 != _R0_REGNO)
	    STFSX(r2, r1, r0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r1);
	    STFSX(r2, rn(reg), r0);
	    jit_unget_reg(reg);
	}
    }
    else
	STFSX(r2, r0, r1);
}

static void
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == _R0_REGNO) {
	if (r1 != _R0_REGNO)
	    STFDX(r2, r1, r0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r0);
	    STFDX(r2, rn(reg), r1);
	    jit_unget_reg(reg);
	}
    }
    else
	STFDX(r2, r0, r1);
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0 == 0)
	str_f(r0, r1);
    else if (can_sign_extend_short_p(i0)) {
	if (r0 == _R0_REGNO) {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), i0);
	    STFS(r1, rn(reg), i0);
	    jit_unget_reg(reg);
	}
	else
	    STFS(r1, r0, i0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_f(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0 == 0)
	str_d(r0, r1);
    else if (can_sign_extend_short_p(i0)) {
	if (r0 == _R0_REGNO) {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), i0);
	    STFD(r1, rn(reg), i0);
	    jit_unget_reg(reg);
	}
	else
	    STFD(r1, r0, i0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_d(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}
#endif
