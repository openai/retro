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
/* match vfpv3 result */
#define NAN_TO_INT_IS_ZERO		1
extern float	sqrtf(float);
extern double	sqrt(double);
extern float	__addsf3(float, float);
extern double	__adddf3(double, double);
extern float	__aeabi_fsub(float, float);
extern double	__aeabi_dsub(double, double);
extern float	__aeabi_fmul(float, float);
extern double	__aeabi_dmul(double, double);
extern float	__aeabi_fdiv(float, float);
extern double	__aeabi_ddiv(double, double);
extern float	__aeabi_i2f(int);
extern double	__aeabi_i2d(int);
extern float	__aeabi_d2f(double);
extern double	__aeabi_f2d(float);
extern int	__aeabi_f2iz(float);
extern int	__aeabi_d2iz(double);
extern int	__aeabi_fcmplt(float, float);
extern int	__aeabi_dcmplt(double, double);
extern int	__aeabi_fcmple(float, float);
extern int	__aeabi_dcmple(double, double);
extern int	__aeabi_fcmpeq(float, float);
extern int	__aeabi_dcmpeq(double, double);
extern int	__aeabi_fcmpge(float, float);
extern int	__aeabi_dcmpge(double, double);
extern int	__aeabi_fcmpgt(float, float);
extern int	__aeabi_dcmpgt(double, double);
extern int	__aeabi_fcmpun(float, float);
extern int	__aeabi_dcmpun(double, double);
#  define swf_ff(i0,r0,r1)		_swf_ff(_jit,i0,r0,r1)
static void
_swf_ff(jit_state_t*,float(*)(float),jit_int32_t,jit_int32_t) maybe_unused;
#  define swf_dd(i0,r0,r1)		_swf_dd(_jit,i0,r0,r1)
static void
_swf_dd(jit_state_t*,double(*)(double),jit_int32_t,jit_int32_t) maybe_unused;
#  define swf_fff(i0,r0,r1,r2)		_swf_fff(_jit,i0,r0,r1,r2)
static void _swf_fff(jit_state_t*,float(*)(float,float),
		     jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_ddd(i0,r0,r1,r2)		_swf_ddd(_jit,i0,r0,r1,r2)
static void _swf_ddd(jit_state_t*,double(*)(double,double),
		     jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_fff_(i0,r0,r1,i1)		_swf_fff_(_jit,i0,r0,r1,i1)
static void _swf_fff_(jit_state_t*,float(*)(float,float),
		      jit_int32_t,jit_int32_t,jit_float32_t);
#  define swf_ddd_(i0,r0,r1,i1)		_swf_ddd_(_jit,i0,r0,r1,i1)
static void _swf_ddd_(jit_state_t*,double(*)(double,double),
		      jit_int32_t,jit_int32_t,jit_float64_t);
#  define swf_iff(i0,r0,r1,r2)		_swf_iff(_jit,i0,r0,r1,r2)
static void _swf_iff(jit_state_t*,int(*)(float,float),
		     jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_idd(i0,r0,r1,r2)		_swf_idd(_jit,i0,r0,r1,r2)
static void _swf_idd(jit_state_t*,int(*)(double,double),
		     jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_iff_(i0,r0,r1,r2)		_swf_iff_(_jit,i0,r0,r1,r2)
static void _swf_iff_(jit_state_t*,int(*)(float,float),
		      jit_int32_t,jit_int32_t,jit_float32_t);
#  define swf_idd_(i0,r0,r1,r2)		_swf_idd_(_jit,i0,r0,r1,r2)
static void _swf_idd_(jit_state_t*,int(*)(double,double),
		      jit_int32_t,jit_int32_t,jit_float64_t);
#  define swf_iunff(i0,r0,r1,r2)	_swf_iunff(_jit,i0,r0,r1,r2)
static void _swf_iunff(jit_state_t*,int(*)(float,float),
		       jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_iundd(i0,r0,r1,r2)	_swf_iundd(_jit,i0,r0,r1,r2)
static void _swf_iundd(jit_state_t*,int(*)(double,double),
		       jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_iunff_(i0,r0,r1,i1)	_swf_iunff_(_jit,i0,r0,r1,i1)
static void _swf_iunff_(jit_state_t*,int(*)(float,float),
			jit_int32_t,jit_int32_t,jit_float32_t);
#  define swf_iundd_(i0,r0,r1,i1)	_swf_iundd_(_jit,i0,r0,r1,i1)
static void _swf_iundd_(jit_state_t*,int(*)(double,double),
			jit_int32_t,jit_int32_t,jit_float64_t);
#  define swf_bff(i0,cc,i1,r0,r1)	_swf_bff(_jit,i0,cc,i1,r0,r1)
static jit_word_t _swf_bff(jit_state_t*,int(*)(float,float),int,
			   jit_word_t,jit_int32_t,jit_int32_t);
#  define swf_bdd(i0,cc,i1,r0,r1)	_swf_bdd(_jit,i0,cc,i1,r0,r1)
static jit_word_t _swf_bdd(jit_state_t*,int(*)(double,double),int,
			   jit_word_t,jit_int32_t,jit_int32_t);
#  define swf_bff_(i0,cc,i1,r0,i2)	_swf_bff_(_jit,i0,cc,i1,r0,i2)
static jit_word_t _swf_bff_(jit_state_t*,int(*)(float,float),int,
			    jit_word_t,jit_int32_t,jit_float32_t);
#  define swf_bdd_(i0,cc,i1,r0,i2)	_swf_bdd_(_jit,i0,cc,i1,r0,i2)
static jit_word_t _swf_bdd_(jit_state_t*,int(*)(double,double),int,
			    jit_word_t,jit_int32_t,jit_float64_t);
#  define swf_bunff(eq,i0,r0,r1)	_swf_bunff(_jit,eq,i0,r0,r1)
static jit_word_t _swf_bunff(jit_state_t*,int,
			     jit_word_t,jit_int32_t,jit_int32_t);
#  define swf_bundd(eq,i0,r0,r1)	_swf_bundd(_jit,eq,i0,r0,r1)
static jit_word_t _swf_bundd(jit_state_t*,int,
			     jit_word_t,jit_int32_t,jit_int32_t);
#  define swf_bunff_(eq,i0,r0,i1)	_swf_bunff_(_jit,eq,i0,r0,i1)
static jit_word_t _swf_bunff_(jit_state_t*,int,
			      jit_word_t,jit_int32_t,jit_float32_t);
#  define swf_bundd_(eq,i0,r0,i1)	_swf_bundd_(_jit,eq,i0,r0,i1)
static jit_word_t _swf_bundd_(jit_state_t*,int,
			      jit_word_t,jit_int32_t,jit_float64_t);
#  define swf_extr_f(r0,r1)		_swf_extr_f(_jit,r0,r1)
static void _swf_extr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_extr_d(r0,r1)		_swf_extr_d(_jit,r0,r1)
static void _swf_extr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_extr_d_f(r0,r1)		_swf_extr_d_f(_jit,r0,r1)
static void _swf_extr_d_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_extr_f_d(r0,r1)		_swf_extr_f_d(_jit,r0,r1)
static void _swf_extr_f_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_truncr_f_i(r0,r1)		_swf_truncr_f_i(_jit,r0,r1)
static void _swf_truncr_f_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_truncr_d_i(r0,r1)		_swf_truncr_d_i(_jit,r0,r1)
static void _swf_truncr_d_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_movr_f(r0,r1)		_swf_movr_f(_jit,r0,r1)
static void _swf_movr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_movr_d(r0,r1)		_swf_movr_d(_jit,r0,r1)
static void _swf_movr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_movi_f(r0,i0)		_swf_movi_f(_jit,r0,i0)
static void _swf_movi_f(jit_state_t*,jit_int32_t,jit_float32_t);
#  define swf_movi_d(r0,i0)		_swf_movi_d(_jit,r0,i0)
static void _swf_movi_d(jit_state_t*,jit_int32_t,jit_float64_t);
#  define swf_absr_f(r0,r1)		_swf_absr_f(_jit,r0,r1)
static void _swf_absr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_absr_d(r0,r1)		_swf_absr_d(_jit,r0,r1)
static void _swf_absr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_negr_f(r0,r1)		_swf_negr_f(_jit,r0,r1)
static void _swf_negr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_negr_d(r0,r1)		_swf_negr_d(_jit,r0,r1)
static void _swf_negr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_sqrtr_f(r0,r1)		swf_ff(sqrtf,r0,r1)
#  define swf_sqrtr_d(r0,r1)		swf_dd(sqrt,r0,r1)
#  define swf_addr_f(r0,r1,r2)		swf_fff(__addsf3,r0,r1,r2)
#  define swf_addi_f(r0,r1,i0)		swf_fff_(__addsf3,r0,r1,i0)
#  define swf_addr_d(r0,r1,r2)		swf_ddd(__adddf3,r0,r1,r2)
#  define swf_addi_d(r0,r1,i0)		swf_ddd_(__adddf3,r0,r1,i0)
#  define swf_subr_f(r0,r1,r2)		swf_fff(__aeabi_fsub,r0,r1,r2)
#  define swf_subi_f(r0,r1,i0)		swf_fff_(__aeabi_fsub,r0,r1,i0)
#  define swf_subr_d(r0,r1,r2)		swf_ddd(__aeabi_dsub,r0,r1,r2)
#  define swf_subi_d(r0,r1,i0)		swf_ddd_(__aeabi_dsub,r0,r1,i0)
#  define swf_rsbr_f(r0, r1, r2)	swf_subr_f(r0, r2, r1)
#  define swf_rsbi_f(r0, r1, i0)	_swf_rsbi_f(_jit, r0, r1, i0)
static void _swf_rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define swf_rsbr_d(r0, r1, r2)	swf_subr_d(r0, r2, r1)
#  define swf_rsbi_d(r0, r1, i0)	_swf_rsbi_d(_jit, r0, r1, i0)
static void _swf_rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define swf_mulr_f(r0,r1,r2)		swf_fff(__aeabi_fmul,r0,r1,r2)
#  define swf_muli_f(r0,r1,i0)		swf_fff_(__aeabi_fmul,r0,r1,i0)
#  define swf_mulr_d(r0,r1,r2)		swf_ddd(__aeabi_dmul,r0,r1,r2)
#  define swf_muli_d(r0,r1,i0)		swf_ddd_(__aeabi_dmul,r0,r1,i0)
#  define swf_divr_f(r0,r1,r2)		swf_fff(__aeabi_fdiv,r0,r1,r2)
#  define swf_divi_f(r0,r1,i0)		swf_fff_(__aeabi_fdiv,r0,r1,i0)
#  define swf_divr_d(r0,r1,r2)		swf_ddd(__aeabi_ddiv,r0,r1,r2)
#  define swf_divi_d(r0,r1,i0)		swf_ddd_(__aeabi_ddiv,r0,r1,i0)
#  define swf_ltr_f(r0,r1,r2)		swf_iff(__aeabi_fcmplt,r0,r1,r2)
#  define swf_lti_f(r0,r1,i0)		swf_iff_(__aeabi_fcmplt,r0,r1,i0)
#  define swf_ltr_d(r0,r1,r2)		swf_idd(__aeabi_dcmplt,r0,r1,r2)
#  define swf_lti_d(r0,r1,i0)		swf_idd_(__aeabi_dcmplt,r0,r1,i0)
#  define swf_ler_f(r0,r1,r2)		swf_iff(__aeabi_fcmple,r0,r1,r2)
#  define swf_lei_f(r0,r1,i0)		swf_iff_(__aeabi_fcmple,r0,r1,i0)
#  define swf_ler_d(r0,r1,r2)		swf_idd(__aeabi_dcmple,r0,r1,r2)
#  define swf_lei_d(r0,r1,i0)		swf_idd_(__aeabi_dcmple,r0,r1,i0)
#  define swf_eqr_f(r0,r1,r2)		swf_iff(__aeabi_fcmpeq,r0,r1,r2)
#  define swf_eqi_f(r0,r1,i0)		swf_iff_(__aeabi_fcmpeq,r0,r1,i0)
#  define swf_eqr_d(r0,r1,r2)		swf_idd(__aeabi_dcmpeq,r0,r1,r2)
#  define swf_eqi_d(r0,r1,i0)		swf_idd_(__aeabi_dcmpeq,r0,r1,i0)
#  define swf_ger_f(r0,r1,r2)		swf_iff(__aeabi_fcmpge,r0,r1,r2)
#  define swf_gei_f(r0,r1,i0)		swf_iff_(__aeabi_fcmpge,r0,r1,i0)
#  define swf_ger_d(r0,r1,r2)		swf_idd(__aeabi_dcmpge,r0,r1,r2)
#  define swf_gei_d(r0,r1,i0)		swf_idd_(__aeabi_dcmpge,r0,r1,i0)
#  define swf_gtr_f(r0,r1,r2)		swf_iff(__aeabi_fcmpgt,r0,r1,r2)
#  define swf_gti_f(r0,r1,i0)		swf_iff_(__aeabi_fcmpgt,r0,r1,i0)
#  define swf_gtr_d(r0,r1,r2)		swf_idd(__aeabi_dcmpgt,r0,r1,r2)
#  define swf_gti_d(r0,r1,i0)		swf_idd_(__aeabi_dcmpgt,r0,r1,i0)
#  define swf_ner_f(r0,r1,r2)		_swf_ner_f(_jit,r0,r1,r2)
static void _swf_ner_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_nei_f(r0,r1,i0)		_swf_nei_f(_jit,r0,r1,i0)
static void _swf_nei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define swf_ner_d(r0,r1,r2)		_swf_ner_d(_jit,r0,r1,r2)
static void _swf_ner_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_nei_d(r0,r1,i0)		_swf_nei_d(_jit,r0,r1,i0)
static void _swf_nei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define swf_unltr_f(r0,r1,r2)		swf_iunff(__aeabi_fcmplt,r0,r1,r2)
#  define swf_unlti_f(r0,r1,i0)		swf_iunff_(__aeabi_fcmplt,r0,r1,i0)
#  define swf_unltr_d(r0,r1,r2)		swf_iundd(__aeabi_dcmplt,r0,r1,r2)
#  define swf_unlti_d(r0,r1,i0)		swf_iundd_(__aeabi_dcmplt,r0,r1,i0)
#  define swf_unler_f(r0,r1,r2)		swf_iunff(__aeabi_fcmple,r0,r1,r2)
#  define swf_unlei_f(r0,r1,i0)		swf_iunff_(__aeabi_fcmple,r0,r1,i0)
#  define swf_unler_d(r0,r1,r2)		swf_iundd(__aeabi_dcmple,r0,r1,r2)
#  define swf_unlei_d(r0,r1,i0)		swf_iundd_(__aeabi_dcmple,r0,r1,i0)
#  define swf_uneqr_f(r0,r1,r2)		swf_iunff(__aeabi_fcmpeq,r0,r1,r2)
#  define swf_uneqi_f(r0,r1,i0)		swf_iunff_(__aeabi_fcmpeq,r0,r1,i0)
#  define swf_uneqr_d(r0,r1,r2)		swf_iundd(__aeabi_dcmpeq,r0,r1,r2)
#  define swf_uneqi_d(r0,r1,i0)		swf_iundd_(__aeabi_dcmpeq,r0,r1,i0)
#  define swf_unger_f(r0,r1,r2)		swf_iunff(__aeabi_fcmpge,r0,r1,r2)
#  define swf_ungei_f(r0,r1,i0)		swf_iunff_(__aeabi_fcmpge,r0,r1,i0)
#  define swf_unger_d(r0,r1,r2)		swf_iundd(__aeabi_dcmpge,r0,r1,r2)
#  define swf_ungei_d(r0,r1,i0)		swf_iundd_(__aeabi_dcmpge,r0,r1,i0)
#  define swf_ungtr_f(r0,r1,r2)		swf_iunff(__aeabi_fcmpgt,r0,r1,r2)
#  define swf_ungti_f(r0,r1,i0)		swf_iunff_(__aeabi_fcmpgt,r0,r1,i0)
#  define swf_ungtr_d(r0,r1,r2)		swf_iundd(__aeabi_dcmpgt,r0,r1,r2)
#  define swf_ungti_d(r0,r1,i0)		swf_iundd_(__aeabi_dcmpgt,r0,r1,i0)
#  define swf_ltgtr_f(r0,r1,r2)		_swf_ltgtr_f(_jit,r0,r1,r2)
static void _swf_ltgtr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_ltgti_f(r0,r1,i0)		_swf_ltgti_f(_jit,r0,r1,i0)
static void _swf_ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define swf_ltgtr_d(r0,r1,r2)		_swf_ltgtr_d(_jit,r0,r1,r2)
static void _swf_ltgtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_ltgti_d(r0,r1,i0)		_swf_ltgti_d(_jit,r0,r1,i0)
static void _swf_ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define swf_ordr_f(r0,r1,r2)		_swf_ordr_f(_jit,r0,r1,r2)
static void _swf_ordr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_ordi_f(r0,r1,i0)		_swf_ordi_f(_jit,r0,r1,i0)
static void _swf_ordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define swf_ordr_d(r0,r1,r2)		_swf_ordr_d(_jit,r0,r1,r2)
static void _swf_ordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_ordi_d(r0,r1,i0)		_swf_ordi_d(_jit,r0,r1,i0)
static void _swf_ordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define swf_unordr_f(r0,r1,r2)	swf_iunff(__aeabi_fcmpun,r0,r1,r2)
#  define swf_unordi_f(r0,r1,i0)	swf_iunff_(__aeabi_fcmpun,r0,r1,i0)
#  define swf_unordr_d(r0,r1,r2)	swf_iundd(__aeabi_dcmpun,r0,r1,r2)
#  define swf_unordi_d(r0,r1,i0)	swf_iundd_(__aeabi_dcmpun,r0,r1,i0)
#  define swf_bltr_f(i0,r0,r1)		swf_bff(__aeabi_fcmplt,ARM_CC_NE,i0,r0,r1)
#  define swf_blti_f(i0,r0,i1)		swf_bff_(__aeabi_fcmplt,ARM_CC_NE,i0,r0,i1)
#  define swf_bltr_d(i0,r0,r1)		swf_bdd(__aeabi_dcmplt,ARM_CC_NE,i0,r0,r1)
#  define swf_blti_d(i0,r0,i1)		swf_bdd_(__aeabi_dcmplt,ARM_CC_NE,i0,r0,i1)
#  define swf_bler_f(i0,r0,r1)		swf_bff(__aeabi_fcmple,ARM_CC_NE,i0,r0,r1)
#  define swf_blei_f(i0,r0,i1)		swf_bff_(__aeabi_fcmple,ARM_CC_NE,i0,r0,i1)
#  define swf_bler_d(i0,r0,r1)		swf_bdd(__aeabi_dcmple,ARM_CC_NE,i0,r0,r1)
#  define swf_blei_d(i0,r0,i1)		swf_bdd_(__aeabi_dcmple,ARM_CC_NE,i0,r0,i1)
#  define swf_beqr_f(i0,r0,r1)		swf_bff(__aeabi_fcmpeq,ARM_CC_NE,i0,r0,r1)
#  define swf_beqi_f(i0,r0,i1)		swf_bff_(__aeabi_fcmpeq,ARM_CC_NE,i0,r0,i1)
#  define swf_beqr_d(i0,r0,r1)		swf_bdd(__aeabi_dcmpeq,ARM_CC_NE,i0,r0,r1)
#  define swf_beqi_d(i0,r0,i1)		swf_bdd_(__aeabi_dcmpeq,ARM_CC_NE,i0,r0,i1)
#  define swf_bger_f(i0,r0,r1)		swf_bff(__aeabi_fcmpge,ARM_CC_NE,i0,r0,r1)
#  define swf_bgei_f(i0,r0,i1)		swf_bff_(__aeabi_fcmpge,ARM_CC_NE,i0,r0,i1)
#  define swf_bger_d(i0,r0,r1)		swf_bdd(__aeabi_dcmpge,ARM_CC_NE,i0,r0,r1)
#  define swf_bgei_d(i0,r0,i1)		swf_bdd_(__aeabi_dcmpge,ARM_CC_NE,i0,r0,i1)
#  define swf_bgtr_f(i0,r0,r1)		swf_bff(__aeabi_fcmpgt,ARM_CC_NE,i0,r0,r1)
#  define swf_bgti_f(i0,r0,i1)		swf_bff_(__aeabi_fcmpgt,ARM_CC_NE,i0,r0,i1)
#  define swf_bgtr_d(i0,r0,r1)		swf_bdd(__aeabi_dcmpgt,ARM_CC_NE,i0,r0,r1)
#  define swf_bgti_d(i0,r0,i1)		swf_bdd_(__aeabi_dcmpgt,ARM_CC_NE,i0,r0,i1)
#  define swf_bner_f(i0,r0,r1)		swf_bff(__aeabi_fcmpeq,ARM_CC_EQ,i0,r0,r1)
#  define swf_bnei_f(i0,r0,i1)		swf_bff_(__aeabi_fcmpeq,ARM_CC_EQ,i0,r0,i1)
#  define swf_bner_d(i0,r0,r1)		swf_bdd(__aeabi_dcmpeq,ARM_CC_EQ,i0,r0,r1)
#  define swf_bnei_d(i0,r0,i1)		swf_bdd_(__aeabi_dcmpeq,ARM_CC_EQ,i0,r0,i1)
#  define swf_bunltr_f(i0,r0,r1)	swf_bff(__aeabi_fcmpge,ARM_CC_EQ,i0,r0,r1)
#  define swf_bunlti_f(i0,r0,i1)	swf_bff_(__aeabi_fcmpge,ARM_CC_EQ,i0,r0,i1)
#  define swf_bunltr_d(i0,r0,r1)	swf_bdd(__aeabi_dcmpge,ARM_CC_EQ,i0,r0,r1)
#  define swf_bunlti_d(i0,r0,i1)	swf_bdd_(__aeabi_dcmpge,ARM_CC_EQ,i0,r0,i1)
#  define swf_bunler_f(i0,r0,r1)	swf_bff(__aeabi_fcmpgt,ARM_CC_EQ,i0,r0,r1)
#  define swf_bunlei_f(i0,r0,i1)	swf_bff_(__aeabi_fcmpgt,ARM_CC_EQ,i0,r0,i1)
#  define swf_bunler_d(i0,r0,r1)	swf_bdd(__aeabi_dcmpgt,ARM_CC_EQ,i0,r0,r1)
#  define swf_bunlei_d(i0,r0,i1)	swf_bdd_(__aeabi_dcmpgt,ARM_CC_EQ,i0,r0,i1)
#  define swf_buneqr_f(i0,r0,r1)	swf_bunff(1,i0,r0,r1)
#  define swf_buneqi_f(i0,r0,i1)	swf_bunff_(1,i0,r0,i1)
#  define swf_buneqr_d(i0,r0,r1)	swf_bundd(1,i0,r0,r1)
#  define swf_buneqi_d(i0,r0,i1)	swf_bundd_(1,i0,r0,i1)
#  define swf_bunger_f(i0,r0,r1)	swf_bff(__aeabi_fcmplt,ARM_CC_EQ,i0,r0,r1)
#  define swf_bungei_f(i0,r0,i1)	swf_bff_(__aeabi_fcmplt,ARM_CC_EQ,i0,r0,i1)
#  define swf_bunger_d(i0,r0,r1)	swf_bdd(__aeabi_dcmplt,ARM_CC_EQ,i0,r0,r1)
#  define swf_bungei_d(i0,r0,i1)	swf_bdd_(__aeabi_dcmplt,ARM_CC_EQ,i0,r0,i1)
#  define swf_bungtr_f(i0,r0,r1)	swf_bff(__aeabi_fcmple,ARM_CC_EQ,i0,r0,r1)
#  define swf_bungti_f(i0,r0,i1)	swf_bff_(__aeabi_fcmple,ARM_CC_EQ,i0,r0,i1)
#  define swf_bungtr_d(i0,r0,r1)	swf_bdd(__aeabi_dcmple,ARM_CC_EQ,i0,r0,r1)
#  define swf_bungti_d(i0,r0,i1)	swf_bdd_(__aeabi_dcmple,ARM_CC_EQ,i0,r0,i1)
#  define swf_bltgtr_f(i0,r0,r1)	swf_bunff(0,i0,r0,r1)
#  define swf_bltgti_f(i0,r0,i1)	swf_bunff_(0,i0,r0,i1)
#  define swf_bltgtr_d(i0,r0,r1)	swf_bundd(0,i0,r0,r1)
#  define swf_bltgti_d(i0,r0,i1)	swf_bundd_(0,i0,r0,i1)
#  define swf_bordr_f(i0,r0,r1)		swf_bff(__aeabi_fcmpun,ARM_CC_EQ,i0,r0,r1)
#  define swf_bordi_f(i0,r0,i1)		swf_bff_(__aeabi_fcmpun,ARM_CC_EQ,i0,r0,i1)
#  define swf_bordr_d(i0,r0,r1)		swf_bdd(__aeabi_dcmpun,ARM_CC_EQ,i0,r0,r1)
#  define swf_bordi_d(i0,r0,i1)		swf_bdd_(__aeabi_dcmpun,ARM_CC_EQ,i0,r0,i1)
#  define swf_bunordr_f(i0,r0,r1)	swf_bff(__aeabi_fcmpun,ARM_CC_NE,i0,r0,r1)
#  define swf_bunordi_f(i0,r0,i1)	swf_bff_(__aeabi_fcmpun,ARM_CC_NE,i0,r0,i1)
#  define swf_bunordr_d(i0,r0,r1)	swf_bdd(__aeabi_dcmpun,ARM_CC_NE,i0,r0,r1)
#  define swf_bunordi_d(i0,r0,i1)	swf_bdd_(__aeabi_dcmpun,ARM_CC_NE,i0,r0,i1)
#  define swf_ldr_f(r0,r1)		_swf_ldr_f(_jit,r0,r1)
static void _swf_ldr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_ldr_d(r0,r1)		_swf_ldr_d(_jit,r0,r1)
static void _swf_ldr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_ldi_f(r0,i0)		_swf_ldi_f(_jit,r0,i0)
static void _swf_ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  define swf_ldi_d(r0,i0)		_swf_ldi_d(_jit,r0,i0)
static void _swf_ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define swf_ldxr_f(r0,r1,r2)		_swf_ldxr_f(_jit,r0,r1,r2)
static void _swf_ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_ldxr_d(r0,r1,r2)		_swf_ldxr_d(_jit,r0,r1,r2)
static void _swf_ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_ldxi_f(r0,r1,i0)		_swf_ldxi_f(_jit,r0,r1,i0)
static void _swf_ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define swf_ldxi_d(r0,r1,i0)		_swf_ldxi_d(_jit,r0,r1,i0)
static void _swf_ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define swf_str_f(r0,r1)		_swf_str_f(_jit,r0,r1)
static void _swf_str_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_str_d(r0,r1)		_swf_str_d(_jit,r0,r1)
static void _swf_str_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define swf_sti_f(r0,i0)		_swf_sti_f(_jit,r0,i0)
static void _swf_sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  define swf_sti_d(r0,i0)		_swf_sti_d(_jit,r0,i0)
static void _swf_sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define swf_stxr_f(r0,r1,r2)		_swf_stxr_f(_jit,r0,r1,r2)
static void _swf_stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_stxr_d(r0,r1,r2)		_swf_stxr_d(_jit,r0,r1,r2)
static void _swf_stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define swf_stxi_f(r0,r1,i0)		_swf_stxi_f(_jit,r0,r1,i0)
static void _swf_stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define swf_stxi_d(r0,r1,i0)		_swf_stxi_d(_jit,r0,r1,i0)
static void _swf_stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define swf_vaarg_d(r0, r1)		_swf_vaarg_d(_jit, r0, r1)
static void _swf_vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
#define swf_off(rn)			((rn - 16) << 2)

#define swf_call(function, label, regno)				\
    do {								\
	jit_word_t	d;						\
	if (!jit_exchange_p()) {					\
	    if (jit_thumb_p())						\
		d = (((jit_word_t)function - _jit->pc.w) >> 1) - 2;	\
	    else							\
		d = (((jit_word_t)function - _jit->pc.w) >> 2) - 2;	\
	    if (_s24P(d)) {						\
		if (jit_thumb_p())					\
		    T2_BLI(encode_thumb_jump(d));			\
		else							\
		    BLI(d & 0x00ffffff);				\
	    }								\
	    else							\
		goto label;						\
	}								\
	else {								\
	label:								\
	    movi(regno, (jit_word_t)function);				\
	    if (jit_thumb_p())						\
		T1_BLX(regno);						\
	    else							\
		BLX(regno);						\
	}								\
    } while (0)
#define swf_call_with_get_reg(function, label)				\
    do {								\
	jit_word_t	d;						\
	jit_int32_t	reg;						\
	if (!jit_exchange_p()) {					\
	    if (jit_thumb_p())						\
		d = (((jit_word_t)function - _jit->pc.w) >> 1) - 2;	\
	    else							\
		d = (((jit_word_t)function - _jit->pc.w) >> 2) - 2;	\
	    if (_s24P(d)) {						\
		if (jit_thumb_p())					\
		    T2_BLI(encode_thumb_jump(d));			\
		else							\
		    BLI(d & 0x00ffffff);				\
	    }								\
	    else							\
		goto label;						\
	}								\
	else {								\
	label:								\
	    reg = jit_get_reg(jit_class_gpr);				\
	    movi(rn(reg), (jit_word_t)function);			\
	    if (jit_thumb_p())						\
		T1_BLX(rn(reg));					\
	    else							\
		BLX(rn(reg));						\
	    jit_unget_reg(reg);						\
	}								\
    } while (0)
#define swf_ldrin(rt, rn, im)						\
    do {								\
	if (jit_thumb_p())	T2_LDRIN(rt, rn, im);			\
	else			LDRIN(rt, rn, im);			\
    } while (0)
#define swf_strin(rt, rn, im)						\
    do {								\
	if (jit_thumb_p())	T2_STRIN(rt, rn, im);			\
	else			STRIN(rt, rn, im);			\
    } while (0)
#define swf_bici(rt, rn, im)						\
    do {								\
	if (jit_thumb_p())						\
	    T2_BICI(rt, rn, encode_thumb_immediate(im));		\
	else								\
	    BICI(rt, rn, encode_arm_immediate(im));			\
    } while (0)

#if !defined(__GNUC__)
float __addsf3(float u, float v)
{
    return (u + v);
}

double
__adddf3(double u, double v)
{
    return (u + v);
}

float
__aeabi_fsub(float u, float v)
{
    return (u - v);
}

double
__aeabi_dsub(double u, double v)
{
    return (u - v);
}

float
__aeabi_fmul(float u, float v)
{
    return (u * v);
}

double
__aeabi_dmul(double u, double v)
{
    return (u * v);
}

float
__aeabi_fdiv(float u, float v)
{
    return (u / v);
}

double
__aeabi_ddiv(double u, double v)
{
    return (u / v);
}

float
__aeabi_i2f(int u)
{
    return (u);
}

double
__aeabi_i2d(int u)
{
    return (u);
}

float
__aeabi_d2f(double u)
{
    return (u);
}

double
__aeabi_f2d(float u)
{
    return (u);
}

extern int
__aeabi_f2iz(float u)
{
    return (u);
}

int
__aeabi_d2iz(double u)
{
    return (u);
}

int
__aeabi_fcmplt(float u, float v)
{
    return (u < v);
}

int
__aeabi_dcmplt(double u, double v)
{
    return (u < v);
}

int
__aeabi_fcmple(float u, float v)
{
    return (u <= v);
}

int
__aeabi_dcmple(double u, double v)
{
    return (u <= v);
}

int
__aeabi_fcmpeq(float u, float v)
{
    return (u == v);
}

int
__aeabi_dcmpeq(double u, double v)
{
    return (u == v);
}

int
__aeabi_fcmpge(float u, float v)
{
    return (u >= v);
}

int
__aeabi_dcmpge(double u, double v)
{
    return (u >= v);
}

int
__aeabi_fcmpgt(float u, float v)
{
    return (u > v);
}

int
__aeabi_dcmpgt(double u, double v)
{
    return (u > v);
}

int
__aeabi_fcmpun(float u, float v)
{
    return ((u != u) || (v != v));
}

int
__aeabi_dcmpun(double u, double v)
{
    return ((u != u) || (v != v));
}
#endif

static void
_swf_ff(jit_state_t *_jit, float(*i0)(float),
	jit_int32_t r0, jit_int32_t r1)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    swf_call(i0, fallback, _R1_REGNO);
    if (jit_fpr_p(r0))
	swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_dd(jit_state_t *_jit, double (*i0)(double),
	jit_int32_t r0, jit_int32_t r1)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    swf_call(i0, fallback, _R2_REGNO);
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    STRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_strin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(r0, _R0_REGNO);
	movr(r0 + 1, _R1_REGNO);
    }
    jit_unget_reg_args();
}

static void
_swf_fff(jit_state_t *_jit, float (*i0)(float, float),
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    if (jit_fpr_p(r2))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r2) + 8);
    else
	movr(_R1_REGNO, r1);
    swf_call(i0, fallback, _R3_REGNO);
    if (jit_fpr_p(r0))
	swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_ddd(jit_state_t *_jit, double (*i0)(double, double),
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    if (jit_fpr_p(r2)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r2) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r2);
	movr(_R3_REGNO, r2 + 1);
    }
    swf_call_with_get_reg(i0, fallback);
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    STRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_strin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(r0, _R0_REGNO);
	movr(r0 + 1, _R1_REGNO);
    }
    jit_unget_reg_args();
}

static void
_swf_fff_(jit_state_t *_jit, float (*i0)(float, float),
	  jit_int32_t r0, jit_int32_t r1, jit_float32_t i1)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_get_reg_args();
    data.f = i1;
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    movi(_R1_REGNO, data.i);
    swf_call(i0, fallback, _R3_REGNO);
    if (jit_fpr_p(r0))
	swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_rsbi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_get_reg_args();
    data.f = i0;
    movi(_R0_REGNO, data.i);
    if (jit_fpr_p(r1))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R1_REGNO, r1);
    swf_call(__aeabi_fsub, fallback, _R3_REGNO);
    if (jit_fpr_p(r0))
	swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_ddd_(jit_state_t *_jit, double (*i0)(double, double),
	  jit_int32_t r0, jit_int32_t r1, jit_float64_t i1)
{
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } data;
    jit_get_reg_args();

    data.d = i1;
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    movi(_R2_REGNO, data.i[0]);
    movi(_R3_REGNO, data.i[1]);
    swf_call_with_get_reg(i0, fallback);
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    STRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_strin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(r0, _R0_REGNO);
	movr(r0 + 1, _R1_REGNO);
    }
    jit_unget_reg_args();
}

static void
_swf_rsbi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)
{
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } data;
    jit_get_reg_args();
    data.d = i0;
    movi(_R0_REGNO, data.i[0]);
    movi(_R1_REGNO, data.i[1]);
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r1);
	movr(_R3_REGNO, r1 + 1);
    }
    swf_call_with_get_reg(__aeabi_dsub, fallback);
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    STRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_strin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(r0, _R0_REGNO);
	movr(r0 + 1, _R1_REGNO);
    }
    jit_unget_reg_args();
}

static void
_swf_iff(jit_state_t *_jit, int (*i0)(float, float),
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    if (jit_fpr_p(r2))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r2) + 8);
    else
	movr(_R1_REGNO, r2);
    swf_call(i0, fallback, _R2_REGNO);
    movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_idd(jit_state_t *_jit, int (*i0)(double, double),
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    if (jit_fpr_p(r2)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r2) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r2);
	movr(_R3_REGNO, r2 + 1);
    }
    swf_call_with_get_reg(i0, fallback);
    movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_iff_(jit_state_t *_jit, int (*i0)(float, float),
	  jit_int32_t r0, jit_int32_t r1, jit_float32_t i1)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_get_reg_args();
    data.f = i1;
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    movi(_R1_REGNO, data.i);
    swf_call(i0, fallback, _R2_REGNO);
    movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_idd_(jit_state_t *_jit, int (*i0)(double, double),
	  jit_int32_t r0, jit_int32_t r1, jit_float64_t i1)
{
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } data;
    jit_get_reg_args();
    data.d = i1;
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    movi(_R2_REGNO, data.i[0]);
    movi(_R3_REGNO, data.i[1]);
    swf_call_with_get_reg(i0, fallback);
    movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_iunff(jit_state_t *_jit, int (*i0)(float, float),
	   jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		instr;
    jit_get_reg_args();
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    if (jit_fpr_p(r2))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r2) + 8);
    else
	movr(_R1_REGNO, r2);
    swf_call(__aeabi_fcmpun, fcmpun, _R2_REGNO);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	IT(ARM_CC_NE);
	if (r0 < 8)
	    T1_MOVI(r0, 1);
	else
	    T2_MOVI(r0, 1);
	instr = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	CC_MOVI(ARM_CC_NE, r0, 1);
	instr = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    if (jit_fpr_p(r2))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r2) + 8);
    else
	movr(_R1_REGNO, r2);
    swf_call(i0, fallback, _R2_REGNO);
    movr(r0, _R0_REGNO);
    patch_at(arm_patch_jump, instr, _jit->pc.w);
    jit_unget_reg_args();
}

static void
_swf_iundd(jit_state_t *_jit, int (*i0)(double, double),
	   jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		instr;
    jit_get_reg_args();
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    if (jit_fpr_p(r2)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r2) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r2);
	movr(_R3_REGNO, r2 + 1);
    }
    swf_call_with_get_reg(__aeabi_dcmpun, dcmpun);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	IT(ARM_CC_NE);
	if (r0 < 8)
	    T1_MOVI(r0, 1);
	else
	    T2_MOVI(r0, 1);
	instr = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	CC_MOVI(ARM_CC_NE, r0, 1);
	instr = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    if (jit_fpr_p(r2)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r2) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r2) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r2);
	movr(_R3_REGNO, r2 + 1);
    }
    swf_call_with_get_reg(i0, fallback);
    movr(r0, _R0_REGNO);
    patch_at(arm_patch_jump, instr, _jit->pc.w);
    jit_unget_reg_args();
}

static void
_swf_iunff_(jit_state_t *_jit, int (*i0)(float, float),
	    jit_int32_t r0, jit_int32_t r1, jit_float32_t i1)
{
    jit_word_t		instr;
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_get_reg_args();
    data.f = i1;
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    movi(_R1_REGNO, data.i);
    swf_call(__aeabi_fcmpun, fcmpun, _R2_REGNO);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	IT(ARM_CC_NE);
	if (r0 < 8)
	    T1_MOVI(r0, 1);
	else
	    T2_MOVI(r0, 1);
	instr = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	CC_MOVI(ARM_CC_NE, r0, 1);
	instr = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    movi(_R1_REGNO, data.i);
    swf_call(i0, fallback, _R2_REGNO);
    movr(r0, _R0_REGNO);
    patch_at(arm_patch_jump, instr, _jit->pc.w);
    jit_unget_reg_args();
}

static void
_swf_iundd_(jit_state_t *_jit, int (*i0)(double, double),
	    jit_int32_t r0, jit_int32_t r1, jit_float64_t i1)
{
    jit_word_t		instr;
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } data;
    jit_get_reg_args();
    data.d = i1;
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    movi(_R2_REGNO, data.i[0]);
    movi(_R3_REGNO, data.i[1]);
    swf_call_with_get_reg(__aeabi_dcmpun, dcmpun);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	IT(ARM_CC_NE);
	if (r0 < 8)
	    T1_MOVI(r0, 1);
	else
	    T2_MOVI(r0, 1);
	instr = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	CC_MOVI(ARM_CC_NE, r0, 1);
	instr = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    movi(_R2_REGNO, data.i[0]);
    movi(_R3_REGNO, data.i[1]);
    swf_call_with_get_reg(i0, fallback);
    movr(r0, _R0_REGNO);
    patch_at(arm_patch_jump, instr, _jit->pc.w);
    jit_unget_reg_args();
}

static jit_word_t
_swf_bff(jit_state_t *_jit, int (*i0)(float, float), int cc,
	 jit_word_t i1, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, d;
    jit_get_reg_args();
    if (jit_fpr_p(r0))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(_R0_REGNO, r0);
    if (jit_fpr_p(r1))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R1_REGNO, r1);
    swf_call(i0, fallback, _R2_REGNO);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    jit_unget_reg_args();
    return (w);
}

static jit_word_t
_swf_bdd(jit_state_t *_jit, int (*i0)(double, double), int cc,
	 jit_word_t i1, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, d;
    jit_get_reg_args();
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r0);
	movr(_R1_REGNO, r0 + 1);
    }
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r1);
	movr(_R3_REGNO, r1 + 1);
    }
    swf_call_with_get_reg(i0, fallback);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    jit_unget_reg_args();
    return (w);
}

static jit_word_t
_swf_bff_(jit_state_t *_jit, int (*i0)(float, float), int cc,
	  jit_word_t i1, jit_int32_t r0, jit_float32_t i2)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_word_t		w, d;
    jit_get_reg_args();
    data.f = i2;
    if (jit_fpr_p(r0))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(_R0_REGNO, r0);
    movi(_R1_REGNO, data.i);
    swf_call(i0, fallback, _R2_REGNO);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    jit_unget_reg_args();
    return (w);
}

static jit_word_t
_swf_bdd_(jit_state_t *_jit, int (*i0)(double, double), int cc,
	  jit_word_t i1, jit_int32_t r0, jit_float64_t i2)
{
    jit_word_t		w, d;
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } data;
    jit_get_reg_args();
    data.d = i2;
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r0);
	movr(_R1_REGNO, r0 + 1);
    }
    movi(_R2_REGNO, data.i[0]);
    movi(_R3_REGNO, data.i[1]);
    swf_call_with_get_reg(i0, fallback);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	w = _jit->pc.w;
	d = ((i1 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    jit_unget_reg_args();
    return (w);
}

static jit_word_t
_swf_bunff(jit_state_t *_jit, int eq,
	   jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, d, j0, j1;
    jit_get_reg_args();
    if (jit_fpr_p(r0))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(_R0_REGNO, r0);
    if (jit_fpr_p(r1))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R1_REGNO, r1);
    swf_call(__aeabi_fcmpun, fcmpun, _R2_REGNO);
    /* if unordered */
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r0))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(_R0_REGNO, r0);
    if (jit_fpr_p(r1))
	swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R1_REGNO, r1);
    swf_call(__aeabi_fcmpeq, fcmpeq, _R2_REGNO);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    T2_CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    T2_CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s24P(d));
	T2_B(encode_thumb_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	B(d & 0x00ffffff);
    }
    if (!eq)
	patch_at(arm_patch_jump, j0, _jit->pc.w);
    patch_at(arm_patch_jump, j1, _jit->pc.w);
    jit_unget_reg_args();
    return (w);
}

static jit_word_t
_swf_bundd(jit_state_t *_jit, int eq,
	   jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, d, j0, j1;
    jit_get_reg_args();
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r0);
	movr(_R1_REGNO, r0 + 1);
    }
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r0);
	movr(_R3_REGNO, r0 + 1);
    }
    swf_call_with_get_reg(__aeabi_dcmpun, dcmpun);
    /* if unordered */
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r0);
	movr(_R1_REGNO, r0 + 1);
    }
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R2_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R3_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R2_REGNO, r0);
	movr(_R3_REGNO, r0 + 1);
    }
    swf_call_with_get_reg(__aeabi_dcmpeq, dcmpeq);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    T2_CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    T2_CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s24P(d));
	T2_B(encode_thumb_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	B(d & 0x00ffffff);
    }
    if (!eq)
	patch_at(arm_patch_jump, j0, _jit->pc.w);
    patch_at(arm_patch_jump, j1, _jit->pc.w);
    jit_unget_reg_args();
    return (w);
}

static jit_word_t
_swf_bunff_(jit_state_t *_jit, int eq,
	    jit_word_t i0, jit_int32_t r0, jit_float32_t i1)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_word_t		w, d, j0, j1;
    data.f = i1;
    jit_get_reg_args();
    if (jit_fpr_p(r0))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(_R0_REGNO, r0);
    movi(_R1_REGNO, data.i);
    swf_call(__aeabi_fcmpun, fcmpun, _R2_REGNO);
    /* if unordered */
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r0))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(_R0_REGNO, r0);
    movi(_R1_REGNO, data.i);
    swf_call(__aeabi_fcmpeq, fcmpeq, _R2_REGNO);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    T2_CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    T2_CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s24P(d));
	T2_B(encode_thumb_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	B(d & 0x00ffffff);
    }
    if (!eq)
	patch_at(arm_patch_jump, j0, _jit->pc.w);
    patch_at(arm_patch_jump, j1, _jit->pc.w);
    jit_unget_reg_args();
    return (w);
}

static jit_word_t
_swf_bundd_(jit_state_t *_jit, int eq,
	    jit_word_t i0, jit_int32_t r0, jit_float64_t i1)
{
    jit_word_t		w, d, j0, j1;
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } data;
    jit_get_reg_args();
    data.d = i1;
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r0);
	movr(_R1_REGNO, r0 + 1);
    }
    movi(_R2_REGNO, data.i[0]);
    movi(_R3_REGNO, data.i[1]);
    swf_call_with_get_reg(__aeabi_dcmpun, fcmpun);
    /* if unordered */
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
    }
    else {
	CMPI(_R0_REGNO, 0);
	j0 = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
    }
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r0);
	movr(_R1_REGNO, r0 + 1);
    }
    movi(_R2_REGNO, data.i[0]);
    movi(_R3_REGNO, data.i[1]);
    swf_call_with_get_reg(__aeabi_dcmpeq, fcmpeq);
    if (jit_thumb_p()) {
	T1_CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    T2_CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    T2_CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s24P(d));
	T2_B(encode_thumb_jump(d));
    }
    else {
	CMPI(_R0_REGNO, 0);
	j1 = _jit->pc.w;
	if (eq) {
	    CC_B(ARM_CC_EQ, 0);
	    patch_at(arm_patch_jump, j0, _jit->pc.w);
	}
	else
	    CC_B(ARM_CC_NE, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	B(d & 0x00ffffff);
    }
    if (!eq)
	patch_at(arm_patch_jump, j0, _jit->pc.w);
    patch_at(arm_patch_jump, j1, _jit->pc.w);
    jit_unget_reg_args();
    return (w);
}

static void
_swf_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_get_reg_args();
    movr(_R0_REGNO, r1);
    swf_call(__aeabi_i2f, i2f, _R1_REGNO);
    if (jit_fpr_p(r0))
	swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_get_reg_args();
    movr(_R0_REGNO, r1);
    swf_call(__aeabi_i2d, i2d, _R2_REGNO);
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    STRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_strin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(r0, _R0_REGNO);
	movr(r0 + 1, _R1_REGNO);
    }
    jit_unget_reg_args();
}

static void
_swf_extr_d_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
    swf_call(__aeabi_d2f, d2f, _R2_REGNO);
    if (jit_fpr_p(r0))
	swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
    else
	movr(r0, _R0_REGNO);
    jit_unget_reg_args();
}

static void
_swf_extr_f_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_get_reg_args();
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
    swf_call(__aeabi_f2d, f2d, _R1_REGNO);
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    STRDIN(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	else {
	    swf_strin(_R0_REGNO, _FP_REGNO, swf_off(r0) + 8);
	    swf_strin(_R1_REGNO, _FP_REGNO, swf_off(r0) + 4);
	}
    }
    else {
	movr(r0, _R0_REGNO);
	movr(r0 + 1, _R1_REGNO);
    }
    jit_unget_reg_args();
}

static void
_swf_truncr_f_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if !NAN_TO_INT_IS_ZERO
    jit_word_t		is_nan;
    jit_word_t		fast_not_nan;
    jit_word_t		slow_not_nan;
#endif
    jit_get_reg_args();
    if (jit_fpr_p(r1))
	swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
    else
	movr(_R0_REGNO, r1);
#if !NAN_TO_INT_IS_ZERO
    /* >> based on fragment of __aeabi_fcmpun */
    lshi(_R2_REGNO, _R0_REGNO, 1);
    if (jit_thumb_p())
	torrrs(THUMB2_MVN|ARM_S, _R0_REGNO, _R3_REGNO, _R2_REGNO,
	       encode_thumb_shift(24, ARM_ASR));
    else
	corrrs(ARM_CC_AL, ARM_MVN|ARM_S|ARM_ASR,
	       _R0_REGNO, _R3_REGNO, _R2_REGNO, 24);
    fast_not_nan = _jit->pc.w;
    if (jit_thumb_p()) {
	T2_CC_B(ARM_CC_NE, 0);
	tshift(THUMB2_LSLI|ARM_S, _R0_REGNO, _R3_REGNO, 9);
    }
    else {
	CC_B(ARM_CC_NE, 0);
	cshift(ARM_CC_AL, ARM_S|ARM_LSL, _R0_REGNO, _R3_REGNO, _R0_REGNO, 9);
    }
    slow_not_nan = _jit->pc.w;
    if (jit_thumb_p())
	T2_CC_B(ARM_CC_EQ, 0);
    else
	CC_B(ARM_CC_EQ, 0);
    movi(r0, 0x80000000);
    is_nan = _jit->pc.w;
    if (jit_thumb_p())
	T2_B(0);
    else
	B(0);
    patch_at(arm_patch_jump, fast_not_nan, _jit->pc.w);
    patch_at(arm_patch_jump, slow_not_nan, _jit->pc.w);
    /* << based on fragment of __aeabi_fcmpun */
#endif
    swf_call(__aeabi_f2iz, f2iz, _R2_REGNO);
    movr(r0, _R0_REGNO);
#if !NAN_TO_INT_IS_ZERO
    patch_at(arm_patch_jump, is_nan, _jit->pc.w);
#endif
    jit_unget_reg_args();
}

static void
_swf_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if !NAN_TO_INT_IS_ZERO
    jit_word_t		is_nan;
    jit_word_t		fast_not_nan;
    jit_word_t		slow_not_nan;
#endif
    jit_get_reg_args();
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p())
	    LDRDIN(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	else {
	    swf_ldrin(_R0_REGNO, _FP_REGNO, swf_off(r1) + 8);
	    swf_ldrin(_R1_REGNO, _FP_REGNO, swf_off(r1) + 4);
	}
    }
    else {
	movr(_R0_REGNO, r1);
	movr(_R1_REGNO, r1 + 1);
    }
#if !NAN_TO_INT_IS_ZERO
    /* >> based on fragment of __aeabi_dcmpun */
    lshi(_R3_REGNO, _R1_REGNO, 1);
    if (jit_thumb_p())
	torrrs(THUMB2_MVN|ARM_S, _R0_REGNO, _R3_REGNO, _R3_REGNO,
	       encode_thumb_shift(21, ARM_ASR));
    else
	corrrs(ARM_CC_AL, ARM_MVN|ARM_S|ARM_ASR,
	       _R0_REGNO, _R3_REGNO, _R3_REGNO, 21);
    fast_not_nan = _jit->pc.w;
    if (jit_thumb_p()) {
	T2_CC_B(ARM_CC_NE, 0);
	torrrs(THUMB2_ORR|ARM_S, _R0_REGNO, _R3_REGNO, _R1_REGNO,
	       encode_thumb_shift(12, ARM_LSL));
    }
    else {
	CC_B(ARM_CC_NE, 0);
	corrrs(ARM_CC_AL, ARM_ORR|ARM_S|ARM_LSL,
	       _R0_REGNO, _R3_REGNO, _R1_REGNO, 12);
    }
    slow_not_nan = _jit->pc.w;
    if (jit_thumb_p())
	T2_CC_B(ARM_CC_EQ, 0);
    else
	CC_B(ARM_CC_EQ, 0);
    movi(r0, 0x80000000);
    is_nan = _jit->pc.w;
    if (jit_thumb_p())
	T2_B(0);
    else
	B(0);
    patch_at(arm_patch_jump, fast_not_nan, _jit->pc.w);
    patch_at(arm_patch_jump, slow_not_nan, _jit->pc.w);
    /* << based on fragment of __aeabi_dcmpun */
#endif
    swf_call(__aeabi_d2iz, d2iz, _R3_REGNO);
    movr(r0, _R0_REGNO);
#if !NAN_TO_INT_IS_ZERO
    patch_at(arm_patch_jump, is_nan, _jit->pc.w);
#endif
    jit_unget_reg_args();
}

static void
_swf_movr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (r0 != r1) {
	if (jit_fpr_p(r1)) {
	    reg = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	    if (jit_fpr_p(r0))
		swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	    else
		movr(r0, rn(reg));
	    jit_unget_reg(reg);
	}
	else if (jit_fpr_p(r0))
	    swf_strin(r1, _FP_REGNO, swf_off(r0) + 8);
	else
	    movr(r0, r1);
    }
}

static void
_swf_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (r0 != r1) {
	if (jit_fpr_p(r1)) {
	    if (!jit_thumb_p() && jit_armv5e_p() &&
		(reg = jit_get_reg_pair()) != JIT_NOREG) {
		LDRDIN(rn(reg), _FP_REGNO, swf_off(r1) + 8);
		if (jit_fpr_p(r0))
		    STRDIN(rn(reg), _FP_REGNO, swf_off(r0) + 8);
		else {
		    movr(r0, rn(reg));
		    movr(r0 + 1, rn(reg) + 1);
		}
		jit_unget_reg_pair(reg);
	    }
	    else {
		reg = jit_get_reg(jit_class_gpr);
		swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
		if (jit_fpr_p(r0))
		    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
		else
		    movr(r0, rn(reg));
		swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 4);
		if (jit_fpr_p(r0))
		    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 4);
		else
		    movr(r0 + 1, rn(reg));
		jit_unget_reg(reg);
	    }
	}
	else if (jit_fpr_p(r0)) {
	    if (!jit_thumb_p() && jit_armv5e_p() && !(r1 & 1))
		STRDIN(r1, _FP_REGNO, swf_off(r0) + 8);
	    else {
		swf_strin(r1, _FP_REGNO, swf_off(r0) + 8);
		swf_strin(r1 + 1, _FP_REGNO, swf_off(r0) + 4);
	    }
	}
	else {
	    movr(r0, r1);
	    movr(r0 + 1, r1 + 1);
	}
    }
}

static void
_swf_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t i0)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_int32_t		reg;
    data.f = i0;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.i);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else
	movi(r0, data.i);
}

static void
_swf_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t i0)
{
    jit_int32_t		reg;
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } data;
    data.d = i0;
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    (reg = jit_get_reg_pair()) != JIT_NOREG) {
	    movi(rn(reg), data.i[0]);
	    movi(rn(reg) + 1, data.i[1]);
	    STRDIN(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	    jit_unget_reg_pair(reg);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), data.i[0]);
	    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	    movi(rn(reg), data.i[1]);
	    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 4);
	    jit_unget_reg(reg);
	}
    }
    else {
	movi(r0, data.i[0]);
	movi(r0 + 1, data.i[1]);
    }
}

static void
_swf_absr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	reg = jit_get_reg(jit_class_gpr);
	swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	swf_bici(rn(reg), rn(reg), 0x80000000);
	if (jit_fpr_p(r0))
	    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	else
	    movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r1);
	swf_bici(rn(reg), rn(reg), 0x80000000);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else
	swf_bici(r0, r1, 0x80000000);
}

static void
_swf_absr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	if (jit_fpr_p(r0) && !jit_thumb_p() && jit_armv5e_p() &&
	    r0 != r1 && (reg = jit_get_reg_pair()) != JIT_NOREG) {
	    LDRDIN(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	    swf_bici(rn(reg) + 1, rn(reg) + 1, 0x80000000);
	    STRDIN(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	    jit_unget_reg_pair(reg);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 4);
	    swf_bici(rn(reg), rn(reg), 0x80000000);
	    if (jit_fpr_p(r0)) {
		swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 4);
		if (r0 != r1) {
		    swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
		    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
		}
	    }
	    else {
		movr(r0, rn(reg));
		swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
		movr(r0 + 1, rn(reg));
	    }
	    jit_unget_reg(reg);
	}
    }
    else if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r1);
	swf_bici(rn(reg), rn(reg), 0x80000000);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 4);
	movr(rn(reg), r1 + 1);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else {
	swf_bici(r0, r1, 0x80000000);
	if (r0 != r1)
	    movr(r0 + 1, r1 + 1);
    }
}

static void
_swf_negr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	reg = jit_get_reg(jit_class_gpr);
	swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	xori(rn(reg), rn(reg), 0x80000000);
	if (jit_fpr_p(r0))
	    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	else
	    movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r1);
	xori(rn(reg), rn(reg), 0x80000000);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else
	xori(r0, r1, 0x80000000);
}

static void
_swf_negr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	if (jit_fpr_p(r0) && !jit_thumb_p() && jit_armv5e_p() &&
	    r0 != r1 && (reg = jit_get_reg_pair()) != JIT_NOREG) {
	    LDRDIN(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	    EORI(rn(reg) + 1, rn(reg) + 1, encode_arm_immediate(0x80000000));
	    STRDIN(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	    jit_unget_reg_pair(reg);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 4);
	    xori(rn(reg), rn(reg), 0x80000000);
	    if (jit_fpr_p(r0)) {
		swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 4);
		if (r0 != r1) {
		    swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
		    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
		}
	    }
	    else {
		movr(r0, rn(reg));
		swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
		movr(r0 + 1, rn(reg));
	    }
	    jit_unget_reg(reg);
	}
    }
    else if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r1);
	xori(rn(reg), rn(reg), 0x80000000);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 4);
	movr(rn(reg), r1 + 1);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else {
	xori(r0, r1, 0x80000000);
	if (r0 != r1)
	    movr(r0 + 1, r1 + 1);
    }
}

static void
_swf_ner_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    swf_iff(__aeabi_fcmpeq, r0, r1, r2);
    xori(r0, r0, 1);
}

static void
_swf_nei_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)
{
    swf_iff_(__aeabi_fcmpeq, r0, r1, i0);
    xori(r0, r0, 1);
}

static void
_swf_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    swf_idd(__aeabi_dcmpeq, r0, r1, r2);
    xori(r0, r0, 1);
}

static void
_swf_nei_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)
{
    swf_idd_(__aeabi_dcmpeq, r0, r1, i0);
    xori(r0, r0, 1);
}

static void
_swf_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    swf_iunff(__aeabi_fcmpeq, r0, r1, r2);
    xori(r0, r0, 1);
}

static void
_swf_ltgti_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)
{
    swf_iunff_(__aeabi_fcmpeq, r0, r1, i0);
    xori(r0, r0, 1);
}

static void
_swf_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    swf_iundd(__aeabi_dcmpeq, r0, r1, r2);
    xori(r0, r0, 1);
}

static void
_swf_ltgti_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)
{
    swf_iundd_(__aeabi_dcmpeq, r0, r1, i0);
    xori(r0, r0, 1);
}

static void
_swf_ordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    swf_iff(__aeabi_fcmpun, r0, r1, r2);
    xori(r0, r0, 1);
}

static void
_swf_ordi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)
{
    swf_iff_(__aeabi_fcmpun, r0, r1, i0);
    xori(r0, r0, 1);
}

static void
_swf_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    swf_idd(__aeabi_dcmpun, r0, r1, r2);
    xori(r0, r0, 1);
}

static void
_swf_ordi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)
{
    swf_idd_(__aeabi_dcmpun, r0, r1, i0);
    xori(r0, r0, 1);
}

static void
_swf_ldr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	ldxi_i(rn(reg), r1, 0);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else
	ldxi_i(r0, r1, 0);
}

static void
_swf_ldr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    (reg = jit_get_reg_pair()) != JIT_NOREG) {
	    LDRDI(rn(reg), r1, 0);
	    STRDIN(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	    jit_unget_reg_pair(reg);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    ldxi_i(rn(reg), r1, 0);
	    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	    ldxi_i(rn(reg), r1, 4);
	    swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 4);
	    jit_unget_reg(reg);
	}
    }
    else if (!jit_thumb_p() && jit_armv5e_p() && !(r0 & 1))
	LDRDI(r0, r1, 0);
    else {
	ldxi_i(r0, r1, 0);
	ldxi_i(r0 + 1, r1, 4);
    }
}

static void
_swf_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	ldi_i(rn(reg), i0);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else
	ldi_i(r0, i0);
}

static void
_swf_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r0) && !jit_thumb_p() && jit_armv5e_p() &&
	(rg0 = jit_get_reg_pair()) != JIT_NOREG) {
	movi(rn(rg0), i0);
	LDRDI(rn(rg0), rn(rg0), 0);
	STRDIN(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg_pair(rg0);
    }
    else {
	rg1 = jit_get_reg(jit_class_gpr);
	movi(rn(rg1), i0);
	if (jit_fpr_p(r0)) {
	    rg0 = jit_get_reg(jit_class_gpr);
	    ldxi_i(rn(rg0), rn(rg1), 0);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    ldxi_i(rn(rg0), rn(rg1), 4);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 4);
	    jit_unget_reg(rg0);
	}
	else if (!jit_thumb_p() && jit_armv5e_p() && !(r0 & 1))
	    LDRDI(r0, rn(rg1), 0);
	else {
	    ldxi_i(r0, rn(rg1), 0);
	    ldxi_i(r0 + 1, rn(rg1), 0);
	}
	jit_unget_reg(rg1);
    }
}

static void
_swf_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	ldxr_i(rn(reg), r1, r2);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else
	ldxr_i(r0, r1, r2);
}

static void
_swf_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    (rg0 = jit_get_reg_pair()) != JIT_NOREG) {
	    LDRD(rn(rg0), r1, r2);
	    STRDIN(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    jit_unget_reg_pair(rg0);
	}
	else {
	    rg1 = jit_get_reg(jit_class_gpr);
	    addr(rn(rg1), r1, r2);
	    rg0 = jit_get_reg(jit_class_gpr);
	    ldxi_i(rn(rg0), rn(rg1), 0);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    ldxi_i(rn(rg0), rn(rg1), 4);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 4);
	    jit_unget_reg(rg0);
	    jit_unget_reg(rg1);
	}
    }
    else {
	if (!jit_thumb_p() && jit_armv5e_p() && !(r0 & 1))
	    LDRD(r0, r1, r2);
	else {
	    rg1 = jit_get_reg(jit_class_gpr);
	    addr(rn(rg1), r1, r2);
	    ldxi_i(r0, rn(rg1), 0);
	    ldxi_i(r0 + 1, rn(rg1), 4);
	    jit_unget_reg(rg1);
	}
    }
}

static void
_swf_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	ldxi_i(rn(reg), r1, i0);
	swf_strin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	jit_unget_reg(reg);
    }
    else
	ldxi_i(r0, r1, i0);
}

static void
_swf_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    ((i0 >= 0 && i0 <= 255) || (i0 < 0 && i0 >= -255)) &&
	    (rg0 = jit_get_reg_pair()) != JIT_NOREG) {
	    if (i0 >= 0)
		LDRDI(rn(rg0), r1, i0);
	    else
		LDRDIN(rn(rg0), r1, -i0);
	    STRDIN(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    jit_unget_reg_pair(rg0);
	}
	else if (i0 >= 0 && i0 + 4 <= 4095) {
	    rg0 = jit_get_reg(jit_class_gpr);
	    ldxi_i(rn(rg0), r1, i0);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    ldxi_i(rn(rg0), r1, i0 + 4);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 4);
	    jit_unget_reg(rg0);
	}
	else if (i0 < 0 && ((jit_thumb_p() && i0 >= -255) ||
			    (!jit_thumb_p() && i0 >= -4095))) {
	    rg0 = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(rg0), r1, -i0);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    swf_ldrin(rn(rg0), r1, -(i0 + 4));
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 4);
	    jit_unget_reg(rg0);
	}
	else {
	    rg0 = jit_get_reg(jit_class_gpr);
	    rg1 = jit_get_reg(jit_class_gpr);
	    addi(rn(rg1), r1, i0);
	    ldxi_i(rn(rg0), rn(rg1), 0);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    ldxi_i(rn(rg0), rn(rg1), 4);
	    swf_strin(rn(rg0), _FP_REGNO, swf_off(r0) + 4);
	    jit_unget_reg(rg1);
	    jit_unget_reg(rg0);
	}
    }
    else {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    i0 >= 0 && i0 <= 255 && !(r0 & 1))
	    LDRDI(r0, r1, i0);
	else if (!jit_thumb_p() && jit_armv5e_p() &&
		 i0 < 0 && i0 >= -255 && !(r0 & 1))
	    LDRDIN(r0, r1, -i0);
	else if (i0 >= 0 && i0 + 4 <= 4095) {
	    ldxi_i(r0, r1, i0);
	    ldxi_i(r0 + 1, r1, i0 + 4);
	}
	else if (i0 < 0 && i0 >= -4095) {
	    swf_ldrin(r0, r1, -i0);
	    swf_ldrin(r0 + 1, r1, -(i0 + 4));
	}
	else {
	    rg0 = jit_get_reg(jit_class_gpr);
	    addi(rn(rg0), r1, i0);
	    ldxi_i(r0, rn(rg0), 0);
	    ldxi_i(r0 + 1, rn(rg0), 4);
	    jit_unget_reg(rg0);
	}
    }
}

static void
_swf_str_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	reg = jit_get_reg(jit_class_gpr);
	swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	stxi_i(0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	str_i(r0, r1);
}

static void
_swf_str_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    (reg = jit_get_reg_pair()) != JIT_NOREG) {
	    LDRDIN(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	    STRDI(rn(reg), r0, 0);
	    jit_unget_reg_pair(reg);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	    stxi_i(0, r0, rn(reg));
	    swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 4);
	    stxi_i(4, r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (!jit_thumb_p() && jit_armv5e_p() && !(r1 & 1))
	    STRDI(r1, r0, 0);
	else {
	    stxi_i(0, r0, r1);
	    stxi_i(4, r0, r1 + 1);
	}
    }
}

static void
_swf_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	swf_ldrin(rn(reg), _FP_REGNO, swf_off(r0) + 8);
	sti_i(i0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	sti_i(i0, r0);
}

static void
_swf_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r0)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    (rg0 = jit_get_reg_pair()) != JIT_NOREG) {
	    rg1 = jit_get_reg(jit_class_gpr);
	    movi(rn(rg1), i0);
	    LDRDIN(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    STRDI(rn(rg0), rn(rg1), 0);
	    jit_unget_reg(rg1);
	    jit_unget_reg_pair(rg0);
	}
	else {
	    rg1 = jit_get_reg(jit_class_gpr);
	    movi(rn(rg1), i0);
	    rg0 = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r0) + 8);
	    stxi_i(0, rn(rg1), rn(rg0));
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r0) + 4);
	    stxi_i(4, rn(rg1), rn(rg0));
	    jit_unget_reg(rg1);
	    jit_unget_reg(rg0);
	}
    }
    else {
	rg1 = jit_get_reg(jit_class_gpr);
	movi(rn(rg1), i0);
	if (!jit_thumb_p() && jit_armv5e_p() && !(r0 & 1))
	    STRDI(r0, rn(rg1), 0);
	else {
	    stxi_i(0, rn(rg1), r0);
	    stxi_i(4, rn(rg1), r0 + 1);
	}
	jit_unget_reg(rg1);
    }
}

static void
_swf_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r2)) {
	reg = jit_get_reg(jit_class_gpr);
	swf_ldrin(rn(reg), _FP_REGNO, swf_off(r2) + 8);
	stxr_i(r1, r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	stxr_i(r0, r1, r2);
}

static void
_swf_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r2)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    (rg0 = jit_get_reg_pair()) != JIT_NOREG) {
	    LDRDIN(rn(rg0), _FP_REGNO, swf_off(r2) + 8);
	    STRD(rn(rg0), r0, r1);
	    jit_unget_reg_pair(rg0);
	}
	else {
	    rg1 = jit_get_reg(jit_class_gpr);
	    addr(rn(rg1), r0, r1);
	    rg0 = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r2) + 8);
	    stxi_i(0, rn(rg1), rn(rg0));
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r2) + 4);
	    stxi_i(4, rn(rg1), rn(rg0));
	    jit_unget_reg(rg0);
	    jit_unget_reg(rg1);
	}
    }
    else {
	if (!jit_thumb_p() && jit_armv5e_p() && !(r2 & 1))
	    STRD(r0, r1, r2);
	else {
	    rg1 = jit_get_reg(jit_class_gpr);
	    addr(rn(rg1), r0, r1);
	    stxi_i(0, rn(rg1), r2);
	    stxi_i(4, rn(rg1), r2 + 1);
	    jit_unget_reg(rg1);
	}
    }
}

static void
_swf_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	reg = jit_get_reg(jit_class_gpr);
	swf_ldrin(rn(reg), _FP_REGNO, swf_off(r1) + 8);
	stxi_i(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	stxi_i(i0, r0, r1);
}

static void
_swf_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r1)) {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    ((i0 >= 0 && i0 <= 255) || (i0 < 0 && i0 >= -255)) &&
	    (rg0 = jit_get_reg_pair()) != JIT_NOREG) {
	    LDRDIN(rn(rg0), _FP_REGNO, swf_off(r1) + 8);
	    if (i0 >= 0 && i0 <= 255)
		STRDI(rn(rg0), r0, i0);
	    else
		STRDIN(rn(rg0), r0, -i0);
	    jit_unget_reg_pair(rg0);
	}
	else if (i0 >= 0 && i0 + 4 <= 4095) {
	    rg0 = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r1) + 8);
	    stxi_i(i0, r0, rn(rg0));
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r1) + 4);
	    stxi_i(i0 + 4, r0, rn(rg0));
	    jit_unget_reg(rg0);
	}
	else if (i0 < 0 && ((jit_thumb_p() && i0 >= -255) ||
			    (!jit_thumb_p() && i0 >= -4095))) {
	    rg0 = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r1) + 8);
	    swf_strin(rn(rg0), r0, -i0);
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r1) + 4);
	    swf_strin(rn(rg0), r0, -(i0 + 4));
	    jit_unget_reg(rg0);
	}
	else {
	    rg1 = jit_get_reg(jit_class_gpr);
	    addi(rn(rg1), r0, i0);
	    rg0 = jit_get_reg(jit_class_gpr);
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r1) + 8);
	    stxi_i(0, rn(rg1), rn(rg0));
	    swf_ldrin(rn(rg0), _FP_REGNO, swf_off(r1) + 4);
	    stxi_i(4, rn(rg1), rn(rg0));
	    jit_unget_reg(rg0);
	    jit_unget_reg(rg1);
	}
    }
    else {
	if (!jit_thumb_p() && jit_armv5e_p() &&
	    i0 >= 0 && i0 <= 255 && !(r1 & 1))
	    STRDI(r1, r0, i0);
	else if (!jit_thumb_p() && jit_armv5e_p() &&
		 i0 < 0 && i0 >= -255 && !(r1 & 1))
	    STRDIN(r1, r0, -i0);
	else if (i0 >= 0 && i0 + 4 <= 4095) {
	    stxi_i(i0, r0, r1);
	    stxi_i(i0 + 4, r0, r1 + 1);
	}
	else if (i0 < 0 && ((jit_thumb_p() && i0 >= 255) ||
			    (!jit_thumb_p() && i0 >= -4095))) {
	    swf_strin(r1, r0, -i0);
	    swf_strin(r1 + 1, r0, -(i0 + 4));
	}
	else {
	    rg1 = jit_get_reg(jit_class_gpr);
	    addi(rn(rg1), r0, i0);
	    stxi_i(0, rn(rg1), r1);
	    stxi_i(4, rn(rg1), r1 + 1);
	    jit_unget_reg(rg1);
	}
    }
}

static void
_swf_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;

    assert(_jitc->function->self.call & jit_call_varargs);

    /* Adjust pointer. */
    reg = jit_get_reg(jit_class_gpr);
    andi(rn(reg), r1, 7);
    addr(r1, r1, rn(reg));
    jit_unget_reg(reg);

    /* Load argument. */
    swf_ldr_d(r0, r1);

    /* Update stack pointer. */
    addi(r1, r1, sizeof(jit_float64_t));
}

#endif
