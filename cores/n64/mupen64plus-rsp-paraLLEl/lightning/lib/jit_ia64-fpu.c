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
#define SF_S0				0
#define SF_S1				1
#define SF_S2				2
#define SF_S3				3

#define TSTFREG1(r0)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->regs, r0 + 128))			\
	    stop();							\
    } while (0)
#define TSTFREG2(r0, r1)						\
    do {								\
	if (jit_regset_tstbit(&_jitc->regs, r0 + 128) ||		\
	    jit_regset_tstbit(&_jitc->regs, r1 + 128))			\
	    stop();							\
    } while (0)
#define TSTFREG3(r0, r1, r2)						\
    do {								\
	if (jit_regset_tstbit(&_jitc->regs, r0 + 128) ||		\
	    jit_regset_tstbit(&_jitc->regs, r1 + 128) ||		\
	    jit_regset_tstbit(&_jitc->regs, r2 + 128))			\
	    stop();							\
    } while (0)
#define SETFREG(r0)		jit_regset_setbit(&_jitc->regs, r0 + 128)

/* libm */
extern float sqrtf(float);
extern double sqrt(double);
#define M7(x6,ht,r3,r2,f1)		_M7(_jit,0,x6,ht,r3,r2,f1)
static void _M7(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M8(x6,ht,r3,im,f1)		_M8(_jit,0,x6,ht,r3,im,f1)
static void _M8(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M9(x6,ht,r3,f1)			_M9(_jit,0,x6,ht,r3,f1)
static void _M9(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define M10(x6,ht,r3,r2,im)		_M10(_jit,0,x6,ht,r3,r2,im)
static void _M10(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M11(x6,ht,r3,f2,f1)		_M11(_jit,0,x6,ht,r3,f2,f1)
static void _M11(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M12(x6,ht,r3,f2,f1)		_M12(_jit,0,x6,ht,r3,f2,f1)
static void _M12(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M18(x6,r2,f1)			_M18(_jit,0,x6,r2,f1)
static void _M18(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t);
#define M19(x6,f2,r1)			_M19(_jit,0,x6,f2,r1)
static void _M19(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t);
#define M21(f2,im)			M20x(0x3,f2,im)
#define M23(x3,im,f1)			M22x(x3,im,f1)
#define M27(f1)				M26x(3,f1)
#define F1(op,x,sf,f4,f3,f2,f1)		F1_(_jit,0,op,x,sf,f4,f3,f2,f1)
#define F2(x2,f4,f3,f2,f1)		F1(0xe,1,x2,f4,f3,f2,f1)
#define F3(f4,f3,f2,f1)			F1(0xe,0,0,f4,f3,f2,f1)
static void F1_(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define F4(rb,sf,ra,p2,f3,f2,ta,p1)	F4_(_jit,0,rb,sf,ra,p2,f3,f2,ta,p1)
static void F4_(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define F5(p2,fc,f2,ta,p1)		F5_(_jit,0,p2,fc,f2,ta,p1)
static void F5_(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define F6x(op,q,sf,p2,f3,f2,f1)	F6x_(_jit,0,op,q,sf,p2,f3,f2,f1)
#define F6(op,sf,p2,f3,f2,f1)		F6x(op,0,sf,p2,f3,f2,f1)
#define F7(op,sf,p2,f3,f1)		F6x(op,1,sf,p2,f3,0,f1)
static void F6x_(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define F8(op,sf,x6,f3,f2,f1)		F8_(_jit,0,op,sf,x6,f3,f2,f1)
#define F9(op,x6,f3,f2,f1)		F8(op,0,x6,f3,f2,f1)
#define F10(op,sf,x6,f2,f1)		F8(op,sf,x6,0,f2,f1)
#define F11(x6,f2,f1)			F8(0,0,x6,0,f2,f1)
static void F8_(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t);
#define F12(sf,x6,omsk,amsk)		F12_(_jit,0,sf,x6,omsk,amsk)
#define F13(sf,x6)			F12(sf,x6,0,0)
static void F12_(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define F14x(sf,x,x6,im)		F14x_(_jit,0,sf,x,x6,im)
#define F14(sf,im)			F14x(sf,0,8,im)
#define F15(im)				F14x(0,0,0,im)
static void F14x_(jit_state_t*,jit_word_t,
		  jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define F16(y,im)			F16_(_jit,0,y,im)
static void F16_(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)maybe_unused;

/* break */
#define BREAK_F(im)			F15(im)
/* chk */
#define CHK_S(f2,im)			M21(f2,im)
#define CHKF_A_NC(f1,im)		M23(0x6,im,f1)
#define CHKF_A_CLR(f1,im)		M23(0x7,im,f1)
/* fabs */
#define FABS(f1,f3)			FMERGE_S(f1,0,f3)
/* fadd */
#define FADD(f1,f3,f2)			FMA(f1,f3,1,f2)
#define FADD_S(f1,f3,f2)		FMA_S(f1,f3,1,f2)
#define FADD_D(f1,f3,f2)		FMA_D(f1,f3,1,f2)
/* famax */
#define FAMAX(f1,f2,f3)			F8(0,SF_S0,0x17,f3,f2,f1)
/* famin */
#define FAMIN(f1,f2,f3)			F8(0,SF_S0,0x16,f3,f2,f1)
/* fand */
#define FAND(f1,f2,f3)			F9(0,0x2c,f3,f2,f1)
/* fandcm */
#define FANDCM(f1,f2,f3)		F9(0,0x2d,f3,f2,f1)
/* fchkf */
#define FCHKF(im)			F14(SF_S0,im)
/* fclass */
#define FCLASS_M(p1,p2,f2,fc)		F5(p2,fc,f2,0,p1)
#define FCLASS_M_UNC(p1,p2,f2,fc)	F5(p2,fc,f2,1,p1)
/* fclrf */
#define FCLRF()				F13(SF_S0,5)
/* fcmp */
#define FCMP_EQ(p1,p2,f2,f3)		F4(0,SF_S0,0,p2,f3,f2,0,p1)
#define FCMP_LT(p1,p2,f2,f3)		F4(1,SF_S0,0,p2,f3,f2,0,p1)
#define FCMP_LE(p1,p2,f2,f3)		F4(0,SF_S0,1,p2,f3,f2,0,p1)
#define FCMP_UNORD(p1,p2,f2,f3)		F4(1,SF_S0,1,p2,f3,f2,0,p1)
#define FCMP_EQ_UNC(p1,p2,f2,f3)	F4(0,SF_S0,0,p2,f3,f2,1,p1)
#define FCMP_LT_UNC(p1,p2,f2,f3)	F4(1,SF_S0,0,p2,f3,f2,1,p1)
#define FCMP_LE_UNC(p1,p2,f2,f3)	F4(0,SF_S0,1,p2,f3,f2,1,p1)
#define FCMP_UNORD_UNC(p1,p2,f2,f3)	F4(1,SF_S0,1,p2,f3,f2,1,p1)
/* fcvt.fx */
#define FCVT_FX(f1,f2)			F10(0,SF_S0,0x18,f2,f1)
#define FCVT_FXU(f1,f2)			F10(0,SF_S0,0x19,f2,f1)
#define FCVT_FX_TRUNC(f1,f2)		F10(0,SF_S0,0x1a,f2,f1)
#define FCVT_FXU_TRUNC(f1,f2)		F10(0,SF_S0,0x1b,f2,f1)
/* fcvt.xf */
#define FCVT_XF(f1,f2)			F11(0x1c,f2,f1)
/* fcvt.fxuf */
#define FCVT_XUF(f1,f3)			FMA(f1,f3,1,0)
/* fma */
#define FMA(f1,f3,f4,f2)		F1(0x8,0,SF_S0,f4,f3,f2,f1)
#define FMA_p(f1,f3,f4,f2,sf,_p)	F1_(_jit,_p,0x8,0,sf,f4,f3,f2,f1)
#define FMA_S(f1,f3,f4,f2)		F1(0x8,1,SF_S0,f4,f3,f2,f1)
#define FMA_S_p(f1,f3,f4,f2,sf,_p)	F1_(_jit,_p,0x8,1,sf,f4,f3,f2,f1)
#define FMA_D(f1,f3,f4,f2)		F1(0x9,0,SF_S0,f4,f3,f2,f1)
#define FMA_D_p(f1,f3,f4,f2,sf,_p)	F1_(_jit,_p,0x9,0,sf,f4,f3,f2,f1)
/* fmax */
#define FMAX(f1,f2,f3)			F8(0,SF_S0,0x15,f3,f2,f1)
/* fmerge */
#define FMERGE_S(f1,f2,f3)		F9(0,0x10,f3,f2,f1)
#define FMERGE_NS(f1,f2,f3)		F9(0,0x11,f3,f2,f1)
#define FMERGE_SE(f1,f2,f3)		F9(0,0x12,f3,f2,f1)
/* fmin */
#define FMIN(f1,f2,f3)			F8(0,SF_S0,0x14,f3,f2,f1)
/* fmix */
#define FMIX_LR(f1,f2,f3)		F9(0,0x39,f3,f2,f1)
#define FMIX_R(f1,f2,f3)		F9(0,0x3a,f3,f2,f1)
#define FMIX_L(f1,f2,f3)		F9(0,0x3b,f3,f2,f1)
/* fmpy */
#define FMPY(f1,f3,f4)			FMA(f1,f3,f4,0)
#define FMPY_p(f1,f3,f4,sf,_p)		FMA_p(f1,f3,f4,0,sf,_p)
#define FMPY_S(f1,f3,f4)		FMA_S(f1,f3,f4,0)
#define FMPY_S_p(f1,f3,f4,sf,_p)	FMA_S_p(f1,f3,f4,0,sf,_p)
#define FMPY_D(f1,f3,f4)		FMA_D(f1,f3,f4,0)
#define FMPY_D_p(f1,f3,f4,sf,_p)	FMA_D_p(f1,f3,f4,0,sf,_p)
/* fms */
#define FMS(f1,f3,f4,f2)		F1(0xa,0,SF_S0,f4,f3,f2,f1)
#define FMS_S(f1,f3,f4,f2)		F1(0xa,1,SF_S0,f4,f3,f2,f1)
#define FMS_D(f1,f3,f4,f2)		F1(0xb,0,SF_S0,f4,f3,f2,f1)
/* fneg */
#define FNEG(f1,f3)			FMERGE_NS(f1,f3,f3)
/* fnegabs */
#define FNEGABS(f1,f3)			FMERGE_NS(f1,0,f3)
/* fnma */
#define FNMA(f1,f3,f4,f2)		F1(0xc,0,SF_S0,f4,f3,f2,f1)
#define FNMA_p(f1,f3,f4,f2,sf,_p)	F1_(_jit,_p,0xc,0,sf,f4,f3,f2,f1)
#define FNMA_S(f1,f3,f4,f2)		F1(0xc,1,SF_S0,f4,f3,f2,f1)
#define FNMA_D(f1,f3,f4,f2)		F1(0xd,0,SF_S0,f4,f3,f2,f1)
/* fnmpy */
#define FNMPY(f1,f3,f4)			FNMA(f1,f3,f4,0)
/* fnorm */
#define FNORM(f1,f3)			FMA(f1,f3,1,0)
#define FNORM_S(f1,f3)			FMA_S(f1,f3,1,0)
#define FNORM_D(f1,f3)			FMA_D(f1,f3,1,0)
/* for */
#define FOR(f1,f2,f3)			F9(0,0x2e,f3,f2,f1)
/* fpabs */
#define FPABS(f1,f3)			FPMERGE_S(f1,0,f3)
/* fpack */
#define FPACK(f1,f2,f3)			F9(0,0x28,f3,f2,f1)
/* fpamax */
#define FPAMAX(f1,f2,f3)		F8(1,SF_S0,0x17,f3,f2,f1)
/* fpamin */
#define FPAMIN(f1,f2,f3)		F8(1,SF_S0,0x16,f3,f2,f1)
/* fpcmp */
#define FPCMP_EQ(f1,f2,f3)		F8(1,SF_S0,0x30,f3,f2,f1)
#define FPCMP_LT(f1,f2,f3)		F8(1,SF_S0,0x31,f3,f2,f1)
#define FPCMP_LE(f1,f2,f3)		F8(1,SF_S0,0x32,f3,f2,f1)
#define FPCMP_UNORD(f1,f2,f3)		F8(1,SF_S0,0x33,f3,f2,f1)
#define FPCMP_NEQ(f1,f2,f3)		F8(1,SF_S0,0x34,f3,f2,f1)
#define FPCMP_NLT(f1,f2,f3)		F8(1,SF_S0,0x35,f3,f2,f1)
#define FPCMP_NLE(f1,f2,f3)		F8(1,SF_S0,0x36,f3,f2,f1)
#define FPCMP_ORD(f1,f2,f3)		F8(1,SF_S0,0x37,f3,f2,f1)
/* fpcvt.fx */
#define FPCVT_FX(f1,f2)			F10(1,SF_S0,0x18,f2,f1)
#define FPCVT_FXU(f1,f2)		F10(1,SF_S0,0x19,f2,f1)
#define FPCVT_FX_TRUNC(f1,f2)		F10(1,SF_S0,0x1a,f2,f1)
#define FPCVT_FXU_TRUNC(f1,f2)		F10(1,SF_S0,0x1b,f2,f1)
/* fpma */
#define FPMA(f1,f3,f4,f2)		F1(0x9,1,SF_S0,f4,f3,f3,f1)
/* fpmax */
#define FPMAX(f1,f2,f3)			F8(1,SF_S0,0x15,f3,f2,f1)
/* fpmerge */
#define FPMERGE_S(f1,f2,f3)		F9(1,0x10,f3,f2,f1)
#define FPMERGE_NS(f1,f2,f3)		F9(1,0x11,f3,f2,f1)
#define FPMERGE_SE(f1,f2,f3)		F9(1,0x12,f3,f2,f1)
/* fpmin */
#define FPMIN(f1,f2,f3)			F8(1,SF_S0,0x14,f3,f2,f1)
/* fpmpy */
#define FPMPY(f1,f3,f4)			FPMA(f1,f3,f4,0)
/* fpms */
#define FPMS(f1,f3,f4,f2)		F1(0xb,1,SF_S0,f4,f3,f3,f1)
/* fpneg */
#define FPNEG(f1,f3)			FPMERGE_NS(f1,f3,f3)
/* fpnegabs */
#define FPNEGABS(f1,f3)			FPMERGE_NS(f1,0,f3)
/* fpnma */
#define FPNMA(f1,f3,f4,f2)		F1(0xd,1,SF_S0,f4,f3,f3,f1)
/* fpnmpy */
#define FPNMPY(f1,f3,f4)		FPNMA(f1,f3,f4,0)
/* fprcpa */
#define FPRCPA(f1,p2,f2,f3)		F6(1,SF_S0,p2,f3,f2,f1)
/* fprsqrta */
#define FPRSQRTA(f1,p2,f3)		F7(1,SF_S0,p2,f3,f1)
/* frcpa */
#define FRCPA(f1,p2,f2,f3)		F6(0,SF_S0,p2,f3,f2,f1)
/* frsqrta */
#define FRSQRTA(f1,p2,f3)		F7(0,SF_S0,p2,f3,f1)
/* fselect */
#define FSELECT(f1,f3,f4,f2)		F3(f4,f3,f2,f1)
#define FSETC(amsk,omsk)		F12(SF_S0,4,omsk,amsk)
/* fsub */
#define FSUB(f1,f3,f2)			FMS(f1,f3,1,f2)
#define FSUB_S(f1,f3,f2)		FMS_S(f1,f3,1,f2)
#define FSUB_D(f1,f3,f2)		FMS_D(f1,f3,1,f2)
/* fswap */
#define FSWAP(f1,f2,f3)			F9(0,0x34,f3,f2,f1)
#define FSWAP_NL(f1,f2,f3)		F9(0,0x35,f3,f2,f1)
#define FSWAP_NR(f1,f2,f3)		F9(0,0x36,f3,f2,f1)
/* fsxt */
#define FSXT_R(f1,f2,f3)		F9(0,0x3c,f3,f2,f1)
#define FSXT_L(f1,f2,f3)		F9(0,0x3d,f3,f2,f1)
/* fxor */
#define FXOR(f1,f2,f3)			F9(0,0x2f,f3,f2,f1)
/* getf */
#define GETF_S(r1,f2)			M19(0x1e,f2,r1)
#define GETF_D(r1,f2)			M19(0x1f,f2,r1)
#define GETF_EXP(r1,f2)			M19(0x1d,f2,r1)
#define GETF_SIG(r1,f2)			M19(0x1c,f2,r1)
/* hint */
#define HINT_F(im)			F16(1,im)
/* invala */
#define INVALAF_E(f1)			M27(f1)
/* ldf */
#define LDFS(f1,r3)			M9(0x02,LD_NONE,r3,f1)
#define LDFD(f1,r3)			M9(0x03,LD_NONE,r3,f1)
#define LDF8(f1,r3)			M9(0x01,LD_NONE,r3,f1)
#define LDFE(f1,r3)			M9(0x00,LD_NONE,r3,f1)
#define LDFS_S(f1,r3)			M9(0x06,LD_NONE,r3,f1)
#define LDFD_S(f1,r3)			M9(0x07,LD_NONE,r3,f1)
#define LDF8_S(f1,r3)			M9(0x05,LD_NONE,r3,f1)
#define LDFE_S(f1,r3)			M9(0x04,LD_NONE,r3,f1)
#define LDFS_A(f1,r3)			M9(0x0a,LD_NONE,r3,f1)
#define LDFD_A(f1,r3)			M9(0x0b,LD_NONE,r3,f1)
#define LDF8_A(f1,r3)			M9(0x09,LD_NONE,r3,f1)
#define LDFE_A(f1,r3)			M9(0x08,LD_NONE,r3,f1)
#define LDFS_SA(f1,r3)			M9(0x0e,LD_NONE,r3,f1)
#define LDFD_SA(f1,r3)			M9(0x0f,LD_NONE,r3,f1)
#define LDF8_SA(f1,r3)			M9(0x0d,LD_NONE,r3,f1)
#define LDFE_SA(f1,r3)			M9(0x0c,LD_NONE,r3,f1)
#define LDF_FILL(f1,r3)			M9(0x1b,LD_NONE,r3,f1)
#define LDFS_C_CLR(f1,r3)		M9(0x22,LD_NONE,r3,f1)
#define LDFD_C_CLR(f1,r3)		M9(0x23,LD_NONE,r3,f1)
#define LDF8_C_CLR(f1,r3)		M9(0x21,LD_NONE,r3,f1)
#define LDFE_C_CLR(f1,r3)		M9(0x20,LD_NONE,r3,f1)
#define LDFS_C_NC(f1,r3)		M9(0x26,LD_NONE,r3,f1)
#define LDFD_C_NC(f1,r3)		M9(0x27,LD_NONE,r3,f1)
#define LDF8_C_NC(f1,r3)		M9(0x25,LD_NONE,r3,f1)
#define LDFE_C_NC(f1,r3)		M9(0x24,LD_NONE,r3,f1)
#define LDFS_inc(f1,r3,r2)		M7(0x02,LD_NONE,r3,r2,f1)
#define LDFD_inc(f1,r3,r2)		M7(0x03,LD_NONE,r3,r2,f1)
#define LDF8_inc(f1,r3,r2)		M7(0x01,LD_NONE,r3,r2,f1)
#define LDFE_inc(f1,r3,r2)		M7(0x00,LD_NONE,r3,r2,f1)
#define LDFS_S_inc(f1,r3,r2)		M7(0x06,LD_NONE,r3,r2,f1)
#define LDFD_S_inc(f1,r3,r2)		M7(0x07,LD_NONE,r3,r2,f1)
#define LDF8_S_inc(f1,r3,r2)		M7(0x05,LD_NONE,r3,r2,f1)
#define LDFE_S_inc(f1,r3,r2)		M7(0x04,LD_NONE,r3,r2,f1)
#define LDFS_A_inc(f1,r3,r2)		M7(0x0a,LD_NONE,r3,r2,f1)
#define LDXFD_A_inc(f1,r3,r2)		M7(0x0b,LD_NONE,r3,r2,f1)
#define LDXF8_A_inc(f1,r3,r2)		M7(0x09,LD_NONE,r3,r2,f1)
#define LDXFE_A_inc(f1,r3,r2)		M7(0x08,LD_NONE,r3,r2,f1)
#define LDXFS_SA_inc(f1,r3,r2)		M7(0x0e,LD_NONE,r3,r2,f1)
#define LDXFD_SA_inc(f1,r3,r2)		M7(0x0f,LD_NONE,r3,r2,f1)
#define LDXF8_SA_inc(f1,r3,r2)		M7(0x0d,LD_NONE,r3,r2,f1)
#define LDXFE_SA_inc(f1,r3,r2)		M7(0x0c,LD_NONE,r3,r2,f1)
#define LDXFS_FILL_inc(f1,r3,r2)	M7(0x1b,LD_NONE,r3,r2,f1)
#define LDXFS_C_CLR_inc(f1,r3,r2)	M7(0x22,LD_NONE,r3,r2,f1)
#define LDXFD_C_CLR_inc(f1,r3,r2)	M7(0x23,LD_NONE,r3,r2,f1)
#define LDXF8_C_CLR_inc(f1,r3,r2)	M7(0x21,LD_NONE,r3,r2,f1)
#define LDXFE_C_CLR_inc(f1,r3,r2)	M7(0x20,LD_NONE,r3,r2,f1)
#define LDXFS_C_NC_inc(f1,r3,r2)	M7(0x26,LD_NONE,r3,r2,f1)
#define LDXFD_C_NC_inc(f1,r3,r2)	M7(0x27,LD_NONE,r3,r2,f1)
#define LDXF8_C_NC_inc(f1,r3,r2)	M7(0x25,LD_NONE,r3,r2,f1)
#define LDXFE_C_NC_inc(f1,r3,r2)	M7(0x24,LD_NONE,r3,r2,f1)
#define LDIFS_inc(f1,f3,im)		M8(0x02,LD_NONE,f3,im,f1)
#define LDIFD_inc(f1,f3,im)		M8(0x03,LD_NONE,f3,im,f1)
#define LDIF8_inc(f1,f3,im)		M8(0x01,LD_NONE,f3,im,f1)
#define LDIFE_inc(f1,f3,im)		M8(0x00,LD_NONE,f3,im,f1)
#define LDIFS_S_inc(f1,f3,im)		M8(0x06,LD_NONE,f3,im,f1)
#define LDIFD_S_inc(f1,f3,im)		M8(0x07,LD_NONE,f3,im,f1)
#define LDIF8_S_inc(f1,f3,im)		M8(0x05,LD_NONE,f3,im,f1)
#define LDIFE_S_inc(f1,f3,im)		M8(0x04,LD_NONE,f3,im,f1)
#define LDIFS_A_inc(f1,f3,im)		M8(0x0a,LD_NONE,f3,im,f1)
#define LDIFD_A_inc(f1,f3,im)		M8(0x0b,LD_NONE,f3,im,f1)
#define LDIF8_A_inc(f1,f3,im)		M8(0x09,LD_NONE,f3,im,f1)
#define LDIFE_A_inc(f1,f3,im)		M8(0x08,LD_NONE,f3,im,f1)
#define LDIF_FILL_inc(f1,f3,im)		M8(0x1b,LD_NONE,f3,im,f1)
#define LDIFS_C_CLR_inc(f1,f3,im)	M8(0x22,LD_NONE,f3,im,f1)
#define LDIFD_C_CLR_inc(f1,f3,im)	M8(0x23,LD_NONE,f3,im,f1)
#define LDIF8_C_CLR_inc(f1,f3,im)	M8(0x21,LD_NONE,f3,im,f1)
#define LDIFE_C_CLR_inc(f1,f3,im)	M8(0x20,LD_NONE,f3,im,f1)
#define LDIFS_C_NC_inc(f1,f3,im)	M8(0x26,LD_NONE,f3,im,f1)
#define LDIFD_C_NC_inc(f1,f3,im)	M8(0x27,LD_NONE,f3,im,f1)
#define LDIF8_C_NC_inc(f1,f3,im)	M8(0x25,LD_NONE,f3,im,f1)
#define LDIFE_C_NC_inc(f1,f3,im)	M8(0x24,LD_NONE,f3,im,f1)
/* ldpf */
#define LDFPS(f1,f2,r3)			M11(0x02,LD_NONE,r3,f2,f1)
#define LDFPD(f1,f2,r3)			M11(0x03,LD_NONE,r3,f2,f1)
#define LDFP8(f1,f2,r3)			M11(0x01,LD_NONE,r3,f2,f1)
#define LDFPS_S(f1,f2,r3)		M11(0x06,LD_NONE,r3,f2,f1)
#define LDFPD_S(f1,f2,r3)		M11(0x07,LD_NONE,r3,f2,f1)
#define LDFP8_S(f1,f2,r3)		M11(0x05,LD_NONE,r3,f2,f1)
#define LDFPS_A(f1,f2,r3)		M11(0x0a,LD_NONE,r3,f2,f1)
#define LDFPD_A(f1,f2,r3)		M11(0x0b,LD_NONE,r3,f2,f1)
#define LDFP8_A(f1,f2,r3)		M11(0x09,LD_NONE,r3,f2,f1)
#define LDFPS_SA(f1,f2,r3)		M11(0x0e,LD_NONE,r3,f2,f1)
#define LDFPD_SA(f1,f2,r3)		M11(0x0f,LD_NONE,r3,f2,f1)
#define LDFP8_SA(f1,f2,r3)		M11(0x0d,LD_NONE,r3,f2,f1)
#define LDFPS_C_CLR(f1,f2,r3)		M11(0x22,LD_NONE,r3,f2,f1)
#define LDFPD_C_CLR(f1,f2,r3)		M11(0x23,LD_NONE,r3,f2,f1)
#define LDFP8_C_CLR(f1,f2,r3)		M11(0x21,LD_NONE,r3,f2,f1)
#define LDFPS_C_NC(f1,f2,r3)		M11(0x26,LD_NONE,r3,f2,f1)
#define LDFPD_C_NC(f1,f2,r3)		M11(0x27,LD_NONE,r3,f2,f1)
#define LDFP8_C_NC(f1,f2,r3)		M11(0x25,LD_NONE,r3,f2,f1)
#define LDIFPS(f1,f2,r3)		M12(0x02,LD_NONE,r3,f2,f1)
#define LDIFPD(f1,f2,r3)		M12(0x03,LD_NONE,r3,f2,f1)
#define LDIFP8(f1,f2,r3)		M12(0x01,LD_NONE,r3,f2,f1)
#define LDIFPS_S(f1,f2,r3)		M12(0x06,LD_NONE,r3,f2,f1)
#define LDIFPD_S(f1,f2,r3)		M12(0x07,LD_NONE,r3,f2,f1)
#define LDIFP8_S(f1,f2,r3)		M12(0x05,LD_NONE,r3,f2,f1)
#define LDIFPS_A(f1,f2,r3)		M12(0x0a,LD_NONE,r3,f2,f1)
#define LDIFPD_A(f1,f2,r3)		M12(0x0b,LD_NONE,r3,f2,f1)
#define LDIFP8_A(f1,f2,r3)		M12(0x09,LD_NONE,r3,f2,f1)
#define LDIFPS_SA(f1,f2,r3)		M12(0x0e,LD_NONE,r3,f2,f1)
#define LDIFPD_SA(f1,f2,r3)		M12(0x0f,LD_NONE,r3,f2,f1)
#define LDIFP8_SA(f1,f2,r3)		M12(0x0d,LD_NONE,r3,f2,f1)
#define LDIFPS_C_CLR(f1,f2,r3)		M12(0x22,LD_NONE,r3,f2,f1)
#define LDIFPD_C_CLR(f1,f2,r3)		M12(0x23,LD_NONE,r3,f2,f1)
#define LDIFP8_C_CLR(f1,f2,r3)		M12(0x21,LD_NONE,r3,f2,f1)
#define LDIFPS_C_NC(f1,f2,r3)		M12(0x26,LD_NONE,r3,f2,f1)
#define LDIFPD_C_NC(f1,f2,r3)		M12(0x27,LD_NONE,r3,f2,f1)
#define LDIFP8_C_NC(f1,f2,r3)		M12(0x25,LD_NONE,r3,f2,f1)
/* mov - Move Floating-point Register */
#define MOVF(f1,f3)			FMERGE_S(f1,f3,f3)
/* nop */
#define NOP_F(im)			F16(0,im)
/* setf */
#define SETF_S(f1,r2)			M18(0x1e,r2,f1)
#define SETF_D(f1,r2)			M18(0x1f,r2,f1)
#define SETF_EXP(f1,r2)			M18(0x1d,r2,f1)
#define SETF_SIG(f1,r2)			M18(0x1c,r2,f1)
/* stf */
#define STFS(r3,f2)			M13(0x32,ST_NONE,r3,f2)
#define STFD(r3,f2)			M13(0x33,ST_NONE,r3,f2)
#define STF8(r3,f2)			M13(0x31,ST_NONE,r3,f2)
#define STFE(r3,f2)			M13(0x30,ST_NONE,r3,f2)
#define STF_SPILL(r3,f2)		M13(0x3b,ST_NONE,r3,f2)
#define STFS_inc(r3,f2,im)		M10(0x32,ST_NONE,r3,f2,im)
#define STFD_inc(r3,f2,im)		M10(0x33,ST_NONE,r3,f2,im)
#define STF8_inc(r3,f2,im)		M10(0x31,ST_NONE,r3,f2,im)
#define STFE_inc(r3,f2,im)		M10(0x30,ST_NONE,r3,f2,im)
#define STF_SPILL_inc(r3,f2,im)		M10(0x3b,ST_NONE,r3,f2,im)
/* xma */
#define XMA_L(f1,f3,f4,f2)		F2(0,f4,f3,f2,f1)
#define XMA_LU(f1,f3,f4,f2)		XMA_L(f1,f3,f4,f2)
#define XMA_H(f1,f3,f4,f2)		F2(3,f4,f3,f2,f1)
#define XMA_HU(f1,f3,f4,f2)		F2(2,f4,f3,f2,f1)
/* xmpy */
#define XMPY_L(f1,f3,f4)		XMA_L(f1,f3,f4,0)
#define XMPY_LU(f1,f3,f4)		XMA_L(f1,f3,f4,0)
#define XMPY_H(f1,f3,f4)		XMA_H(f1,f3,f4,0)
#define XMPY_HU(f1,f3,f4)		XMA_HU(f1,f3,f4,0)

#define movr_f(r0,r1)			movr_d(r0,r1)
#define movr_d(r0,r1)			MOVF(r0,r1)
#define movi_f(r0,i0)			_movi_f(_jit,r0,i0)
static void _movi_f(jit_state_t*,jit_int32_t,jit_float32_t*);
#define movi_d(r0,i0)			_movi_d(_jit,r0,i0)
static void _movi_d(jit_state_t*,jit_int32_t,jit_float64_t*);
#define movr_w_f(r0,r1)			_movr_w_f(_jit,r0,r1)
static void _movr_w_f(jit_state_t*,jit_int32_t,jit_int32_t);
#define movr_f_w(r0,r1)			_movr_f_w(_jit,r0,r1)
static void _movr_f_w(jit_state_t*,jit_int32_t,jit_int32_t);
#define movr_w_d(r0,r1)			_movr_w_d(_jit,r0,r1)
static void _movr_w_d(jit_state_t*,jit_int32_t,jit_int32_t);
#define movr_d_w(r0,r1)			_movr_d_w(_jit,r0,r1)
static void _movr_d_w(jit_state_t*,jit_int32_t,jit_int32_t);
#define movi_f_w(r0,i0)			_movi_f_w(_jit,r0,i0)
static void _movi_f_w(jit_state_t*,jit_int32_t,jit_float32_t*);
#define movi_d_w(r0,i0)			_movi_d_w(_jit,r0,i0)
static void _movi_d_w(jit_state_t*,jit_int32_t,jit_float64_t*);
#define absr_f(r0,r1)			absr_d(r0,r1)
#define absr_d(r0,r1)			FABS(r0,r1)
#define negr_f(r0,r1)			negr_d(r0,r1)
#define negr_d(r0,r1)			FNEG(r0,r1)
#define sqrtr_f(r0,r1)			_sqrtr_f(_jit,r0,r1)
static void _sqrtr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#define sqrtr_d(r0,r1)			_sqrtr_d(_jit,r0,r1)
static void _sqrtr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#define extr_f_d(r0,r1)			FNORM_D(r0,r1)
#define extr_d_f(r0,r1)			FNORM_S(r0,r1)
#define extr_f(r0,r1)			_extr_f(_jit,r0,r1)
static void _extr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#define extr_d(r0,r1)			_extr_d(_jit,r0,r1)
static void _extr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#define truncr_f_i(r0,r1)		truncr_d_l(r0,r1)
#define truncr_d_i(r0,r1)		truncr_d_l(r0,r1)
#define truncr_f_l(r0,r1)		truncr_d_l(r0,r1)
#define truncr_d_l(r0,r1)		_truncr_d_l(_jit,r0,r1)
static void _truncr_d_l(jit_state_t*,jit_int32_t,jit_int32_t);
#define addr_f(r0,r1,r2)		FADD_S(r0,r1,r2)
#define addi_f(r0,r1,i0)		_addi_f(_jit,r0,r1,i0)
static void _addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define addr_d(r0,r1,r2)		FADD_D(r0,r1,r2)
#define addi_d(r0,r1,i0)		_addi_d(_jit,r0,r1,i0)
static void _addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define subr_f(r0,r1,r2)		FSUB_S(r0,r1,r2)
#define subi_f(r0,r1,i0)		_subi_f(_jit,r0,r1,i0)
static void _subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define subr_d(r0,r1,r2)		FSUB_D(r0,r1,r2)
#define subi_d(r0,r1,i0)		_subi_d(_jit,r0,r1,i0)
static void _subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define rsbr_f(r0,r1,r2)		subr_f(r0,r2,r1)
#define rsbi_f(r0,r1,i0)		_rsbi_f(_jit,r0,r1,i0)
static void _rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define rsbr_d(r0,r1,r2)		subr_d(r0,r2,r1)
#define rsbi_d(r0,r1,i0)		_rsbi_d(_jit,r0,r1,i0)
static void _rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define mulr_f(r0,r1,r2)		FMPY_S(r0,r1,r2)
#define muli_f(r0,r1,i0)		_muli_f(_jit,r0,r1,i0)
static void _muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define mulr_d(r0,r1,r2)		FMPY_D(r0,r1,r2)
#define muli_d(r0,r1,i0)		_muli_d(_jit,r0,r1,i0)
static void _muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define divr_f(r0,r1,r2)		_divr_f(_jit,r0,r1,r2)
static void _divr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define divi_f(r0,r1,i0)		_divi_f(_jit,r0,r1,i0)
static void _divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define divr_d(r0,r1,r2)		_divr_d(_jit,r0,r1,r2)
static void _divr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define divi_d(r0,r1,i0)		_divi_d(_jit,r0,r1,i0)
static void _divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ltr_f(r0,r1,r2)			ltr_d(r0,r1,r2)
#define ltr_d(r0,r1,r2)			_ltr_d(_jit,r0,r1,r2)
static void _ltr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lti_f(r0,r1,i0)			_lti_f(_jit,r0,r1,i0)
static void _lti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define lti_d(r0,r1,i0)			_lti_d(_jit,r0,r1,i0)
static void _lti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ler_f(r0,r1,r2)			ler_d(r0,r1,r2)
#define ler_d(r0,r1,r2)			_ler_d(_jit,r0,r1,r2)
static void _ler_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lei_f(r0,r1,i0)			_lei_f(_jit,r0,r1,i0)
static void _lei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define lei_d(r0,r1,i0)			_lei_d(_jit,r0,r1,i0)
static void _lei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define eqr_f(r0,r1,r2)			eqr_d(r0,r1,r2)
#define eqr_d(r0,r1,r2)			_eqr_d(_jit,r0,r1,r2)
static void _eqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define eqi_f(r0,r1,i0)			_eqi_f(_jit,r0,r1,i0)
static void _eqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define eqi_d(r0,r1,i0)			_eqi_d(_jit,r0,r1,i0)
static void _eqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ger_f(r0,r1,r2)			ger_d(r0,r1,r2)
#define ger_d(r0,r1,r2)			_ger_d(_jit,r0,r1,r2)
static void _ger_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gei_f(r0,r1,i0)			_gei_f(_jit,r0,r1,i0)
static void _gei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define gei_d(r0,r1,i0)			_gei_d(_jit,r0,r1,i0)
static void _gei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define gtr_f(r0,r1,r2)			gtr_d(r0,r1,r2)
#define gtr_d(r0,r1,r2)			_gtr_d(_jit,r0,r1,r2)
static void _gtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gti_f(r0,r1,i0)			_gti_f(_jit,r0,r1,i0)
static void _gti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define gti_d(r0,r1,i0)			_gti_d(_jit,r0,r1,i0)
static void _gti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ner_f(r0,r1,r2)			ner_d(r0,r1,r2)
#define ner_d(r0,r1,r2)			_ner_d(_jit,r0,r1,r2)
static void _ner_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define nei_f(r0,r1,i0)			_nei_f(_jit,r0,r1,i0)
static void _nei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define nei_d(r0,r1,i0)			_nei_d(_jit,r0,r1,i0)
static void _nei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define unltr_f(r0,r1,r2)		unltr_d(r0,r1,r2)
#define unltr_d(r0,r1,r2)		_unltr_d(_jit,r0,r1,r2)
static void _unltr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define unlti_f(r0,r1,i0)		_unlti_f(_jit,r0,r1,i0)
static void _unlti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define unlti_d(r0,r1,i0)		_unlti_d(_jit,r0,r1,i0)
static void _unlti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define unler_f(r0,r1,r2)		unler_d(r0,r1,r2)
#define unler_d(r0,r1,r2)		_unler_d(_jit,r0,r1,r2)
static void _unler_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define unlei_f(r0,r1,i0)		_unlei_f(_jit,r0,r1,i0)
static void _unlei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define unlei_d(r0,r1,i0)		_unlei_d(_jit,r0,r1,i0)
static void _unlei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define uneqr_f(r0,r1,r2)		uneqr_d(r0,r1,r2)
#define uneqr_d(r0,r1,r2)		_uneqr_d(_jit,r0,r1,r2)
static void _uneqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define uneqi_f(r0,r1,i0)		_uneqi_f(_jit,r0,r1,i0)
static void _uneqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define uneqi_d(r0,r1,i0)		_uneqi_d(_jit,r0,r1,i0)
static void _uneqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define unger_f(r0,r1,r2)		unger_d(r0,r1,r2)
#define unger_d(r0,r1,r2)		_unger_d(_jit,r0,r1,r2)
static void _unger_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ungei_f(r0,r1,i0)		_ungei_f(_jit,r0,r1,i0)
static void _ungei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define ungei_d(r0,r1,i0)		_ungei_d(_jit,r0,r1,i0)
static void _ungei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ungtr_f(r0,r1,r2)		ungtr_d(r0,r1,r2)
#define ungtr_d(r0,r1,r2)		_ungtr_d(_jit,r0,r1,r2)
static void _ungtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ungti_f(r0,r1,i0)		_ungti_f(_jit,r0,r1,i0)
static void _ungti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define ungti_d(r0,r1,i0)		_ungti_d(_jit,r0,r1,i0)
static void _ungti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ltgtr_f(r0,r1,r2)		ltgtr_d(r0,r1,r2)
#define ltgtr_d(r0,r1,r2)		_ltgtr_d(_jit,r0,r1,r2)
static void _ltgtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ltgti_f(r0,r1,i0)		_ltgti_f(_jit,r0,r1,i0)
static void _ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define ltgti_d(r0,r1,i0)		_ltgti_d(_jit,r0,r1,i0)
static void _ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ordr_f(r0,r1,r2)		ordr_d(r0,r1,r2)
#define ordr_d(r0,r1,r2)		_ordr_d(_jit,r0,r1,r2)
static void _ordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ordi_f(r0,r1,i0)		_ordi_f(_jit,r0,r1,i0)
static void _ordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define ordi_d(r0,r1,i0)		_ordi_d(_jit,r0,r1,i0)
static void _ordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define unordr_f(r0,r1,r2)		unordr_d(r0,r1,r2)
#define unordr_d(r0,r1,r2)		_unordr_d(_jit,r0,r1,r2)
static void _unordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define unordi_f(r0,r1,i0)		_unordi_f(_jit,r0,r1,i0)
static void _unordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define unordi_d(r0,r1,i0)		_unordi_d(_jit,r0,r1,i0)
static void _unordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define ldr_f(r0,r1)			LDFS(r0,r1)
#define ldi_f(r0,i0)			_ldi_f(_jit,r0,i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_f(r0,r1,r2)		_ldxr_f(_jit,r0,r1,r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_f(r0,r1,i0)		_ldxi_f(_jit,r0,r1,i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldr_d(r0,r1)			LDFD(r0,r1)
#define ldi_d(r0,i0)			_ldi_d(_jit,r0,i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_d(r0,r1,r2)		_ldxr_d(_jit,r0,r1,r2)
static void _ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_d(r0,r1,i0)		_ldxi_d(_jit,r0,r1,i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define str_f(r0,r1)			STFS(r0,r1)
#define sti_f(i0,r0)			_sti_f(_jit,i0,r0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#define stxr_f(r0,r1,r2)		_stxr_f(_jit,r0,r1,r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_f(i0,r0,r1)		_stxi_f(_jit,i0,r0,r1)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define str_d(r0,r1)			STFD(r0,r1)
#define sti_d(i0,r0)			_sti_d(_jit,i0,r0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#define stxr_d(r0,r1,r2)		_stxr_d(_jit,r0,r1,r2)
static void _stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_d(i0,r0,r1)		_stxi_d(_jit,i0,r0,r1)
static void _stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bltr_f(i0,r0,r1)		bltr_d(i0,r0,r1)
#define bltr_d(i0,r0,r1)		_bltr_d(_jit,i0,r0,r1)
static jit_word_t _bltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define blti_f(i0,r0,i1)		_blti_f(_jit,i0,r0,i1)
static jit_word_t _blti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define blti_d(i0,r0,i1)		_blti_d(_jit,i0,r0,i1)
static jit_word_t _blti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bler_f(i0,r0,r1)		bler_d(i0,r0,r1)
#define bler_d(i0,r0,r1)		_bler_d(_jit,i0,r0,r1)
static jit_word_t _bler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define blei_f(i0,r0,i1)		_blei_f(_jit,i0,r0,i1)
static jit_word_t _blei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define blei_d(i0,r0,i1)		_blei_d(_jit,i0,r0,i1)
static jit_word_t _blei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define beqr_f(i0,r0,r1)		beqr_d(i0,r0,r1)
#define beqr_d(i0,r0,r1)		_beqr_d(_jit,i0,r0,r1)
static jit_word_t _beqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define beqi_f(i0,r0,i1)		_beqi_f(_jit,i0,r0,i1)
static jit_word_t _beqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define beqi_d(i0,r0,i1)		_beqi_d(_jit,i0,r0,i1)
static jit_word_t _beqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bger_f(i0,r0,r1)		bger_d(i0,r0,r1)
#define bger_d(i0,r0,r1)		_bger_d(_jit,i0,r0,r1)
static jit_word_t _bger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bgei_f(i0,r0,i1)		_bgei_f(_jit,i0,r0,i1)
static jit_word_t _bgei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bgei_d(i0,r0,i1)		_bgei_d(_jit,i0,r0,i1)
static jit_word_t _bgei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bgtr_f(i0,r0,r1)		bgtr_d(i0,r0,r1)
#define bgtr_d(i0,r0,r1)		_bgtr_d(_jit,i0,r0,r1)
static jit_word_t _bgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bgti_f(i0,r0,i1)		_bgti_f(_jit,i0,r0,i1)
static jit_word_t _bgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bgti_d(i0,r0,i1)		_bgti_d(_jit,i0,r0,i1)
static jit_word_t _bgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bner_f(i0,r0,r1)		bner_d(i0,r0,r1)
#define bner_d(i0,r0,r1)		_bner_d(_jit,i0,r0,r1)
static jit_word_t _bner_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bnei_f(i0,r0,i1)		_bnei_f(_jit,i0,r0,i1)
static jit_word_t _bnei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bnei_d(i0,r0,i1)		_bnei_d(_jit,i0,r0,i1)
static jit_word_t _bnei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bunltr_f(i0,r0,r1)		bunltr_d(i0,r0,r1)
#define bunltr_d(i0,r0,r1)		_bunltr_d(_jit,i0,r0,r1)
static jit_word_t _bunltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bunlti_f(i0,r0,i1)		_bunlti_f(_jit,i0,r0,i1)
static jit_word_t _bunlti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bunlti_d(i0,r0,i1)		_bunlti_d(_jit,i0,r0,i1)
static jit_word_t _bunlti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bunler_f(i0,r0,r1)		bunler_d(i0,r0,r1)
#define bunler_d(i0,r0,r1)		_bunler_d(_jit,i0,r0,r1)
static jit_word_t _bunler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bunlei_f(i0,r0,i1)		_bunlei_f(_jit,i0,r0,i1)
static jit_word_t _bunlei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bunlei_d(i0,r0,i1)		_bunlei_d(_jit,i0,r0,i1)
static jit_word_t _bunlei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define buneqr_f(i0,r0,r1)		buneqr_d(i0,r0,r1)
#define buneqr_d(i0,r0,r1)		_buneqr_d(_jit,i0,r0,r1)
static jit_word_t _buneqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define buneqi_f(i0,r0,i1)		_buneqi_f(_jit,i0,r0,i1)
static jit_word_t _buneqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define buneqi_d(i0,r0,i1)		_buneqi_d(_jit,i0,r0,i1)
static jit_word_t _buneqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bunger_f(i0,r0,r1)		bunger_d(i0,r0,r1)
#define bunger_d(i0,r0,r1)		_bunger_d(_jit,i0,r0,r1)
static jit_word_t _bunger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bungei_f(i0,r0,i1)		_bungei_f(_jit,i0,r0,i1)
static jit_word_t _bungei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bungei_d(i0,r0,i1)		_bungei_d(_jit,i0,r0,i1)
static jit_word_t _bungei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bungtr_f(i0,r0,r1)		bungtr_d(i0,r0,r1)
#define bungtr_d(i0,r0,r1)		_bungtr_d(_jit,i0,r0,r1)
static jit_word_t _bungtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bungti_f(i0,r0,i1)		_bungti_f(_jit,i0,r0,i1)
static jit_word_t _bungti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bungti_d(i0,r0,i1)		_bungti_d(_jit,i0,r0,i1)
static jit_word_t _bungti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bltgtr_f(i0,r0,r1)		bltgtr_d(i0,r0,r1)
#define bltgtr_d(i0,r0,r1)		_bltgtr_d(_jit,i0,r0,r1)
static jit_word_t _bltgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bltgti_f(i0,r0,i1)		_bltgti_f(_jit,i0,r0,i1)
static jit_word_t _bltgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bltgti_d(i0,r0,i1)		_bltgti_d(_jit,i0,r0,i1)
static jit_word_t _bltgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bordr_f(i0,r0,r1)		bordr_d(i0,r0,r1)
#define bordr_d(i0,r0,r1)		_bordr_d(_jit,i0,r0,r1)
static jit_word_t _bordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bordi_f(i0,r0,i1)		_bordi_f(_jit,i0,r0,i1)
static jit_word_t _bordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bordi_d(i0,r0,i1)		_bordi_d(_jit,i0,r0,i1)
static jit_word_t _bordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#define bunordr_f(i0,r0,r1)		bunordr_d(i0,r0,r1)
#define bunordr_d(i0,r0,r1)		_bunordr_d(_jit,i0,r0,r1)
static jit_word_t _bunordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bunordi_f(i0,r0,i1)		_bunordi_f(_jit,i0,r0,i1)
static jit_word_t _bunordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t*);
#define bunordi_d(i0,r0,i1)		_bunordi_d(_jit,i0,r0,i1)
static jit_word_t _bunordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t*);
#  define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
static void
_M7(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t r2, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTFREG1(f1);
    inst((6L<<37)|(1L<<36)|(x6<<30)|(ht<<28)|
	 (r3<<20)|(r2<<13)|(f1<<6)|_p, INST_M);
    SETFREG(f1);
    SETREG(r3);
}

static void
_M8(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t im, jit_word_t f1)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(ht &   ~0x3L));
    assert(!(r3 &  ~0x7fL));
    assert(im > -256 && im <= 255);
    assert(!(f1 &  ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTFREG1(f1);
    inst((7L<<37)|(((im>>8)&1L)<<36)|(x6<<30)|(ht<<28)|
	 (((im>>8)&1L)<<27)|(r3<<20)|((im&0x7fLL)<<13)|(f1<<6)|_p, INST_M);
    SETFREG(f1);
    SETREG(r3);
}

static void
_M9(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTFREG1(f1);
    inst((6L<<37)|(x6<<30)|(ht<<28)|(r3<<20)|(f1<<6)|_p, INST_M);
    SETFREG(f1);
}

static void
_M10(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t f2, jit_word_t im)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(ht &   ~0x3L));
    assert(!(r3 &  ~0x7fL));
    assert(!(f2 &  ~0x7fL));
    assert(im > -256 && im <= 255);
    TSTREG1(r3);
    TSTFREG1(f2);
    TSTPRED(_p);
    inst((7L<<37)|(((im>>8)&1L)<<36)|(x6<<30)|(ht<<28)|
	 (((im>>8)&1L)<<27)|(r3<<20)|(f2<<13)|((im&0x7fL)<<6)|_p, INST_M);
    SETREG(r3);
}

static void
_M11(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t f2, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(f2 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTFREG2(f1, f2);
    inst((6L<<37)|(x6<<30)|(ht<<28)|(1L<<27)|
	 (r3<<20)|(f2<<13)|(f1<<6)|_p, INST_M);
    SETFREG(f1);
    SETFREG(f2);
}

static void
_M12(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t f2, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(f2 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTFREG2(f1, f2);
    inst((6L<<37)|(1L<<36)|(x6<<30)|(ht<<28)|
	 (1L<<27)|(r3<<20)|(f2<<13)|(f1<<6)|_p, INST_M);
    SETFREG(f1);
    SETFREG(f2);
    SETREG(r3);
}

static void
_M18(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r2, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(r2 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    TSTFREG1(f1);
    inst((6L<<37)|(x6<<30)|(1L<<27)|(r2<<13)|(f1<<6)|_p, INST_M);
    SETFREG(f1);
}

static void
_M19(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t f2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(f2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTFREG1(f2);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((4L<<37)|(x6<<30)|(1L<<27)|(f2<<13)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
F1_(jit_state_t *_jit, jit_word_t _p,
    jit_word_t op, jit_word_t x, jit_word_t sf,
    jit_word_t f4, jit_word_t f3, jit_word_t f2, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(op &  ~0xfL));
    assert(!(x  &  ~0x1L));
    assert(!(sf &  ~0x3L));
    assert(!(f4 & ~0x7fL));
    assert(!(f3 & ~0x7fL));
    assert(!(f2 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTFREG3(f2, f3, f4);
    TSTPRED(_p);
    TSTFREG1(f1);
    inst((op<<37)|(x<<36)|(sf<<34)|(f4<<27)|
	 (f3<<20)|(f2<<13)|(f1<<6)|_p, INST_F);
    SETFREG(f1);
}

static void
F4_(jit_state_t *_jit, jit_word_t _p,
    jit_word_t rb, jit_word_t sf, jit_word_t ra, jit_word_t p2,
    jit_word_t f3, jit_word_t f2, jit_word_t ta, jit_word_t p1)
{
    assert(!(_p & ~0x3fL));
    assert(!(rb &  ~0x1L));
    assert(!(sf &  ~0x3L));
    assert(!(ra &  ~0x1L));
    assert(!(p2 & ~0x3fL));
    assert(!(f3 & ~0x7fL));
    assert(!(f2 & ~0x7fL));
    assert(!(ta &  ~0x1L));
    assert(!(p1 & ~0x3fL));
    TSTFREG2(f2, f3);
    TSTPRED(_p);
    inst((4L<<37)|(rb<<36)|(sf<<34)|(ra<<33)|(p2<<27)|
	 (f3<<20)|(f2<<13)|(ta<<12)|(p1<<6)|_p, INST_F);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
F5_(jit_state_t *_jit, jit_word_t _p,
    jit_word_t p2, jit_word_t fc, jit_word_t f2, jit_word_t ta, jit_word_t p1)
{
    assert(!(_p &  ~0x3fL));
    assert(!(p2 &  ~0x3fL));
    assert(!(fc & ~0x1ffL));
    assert(!(f2 &  ~0x7fL));
    assert(!(ta &   ~0x1L));
    assert(!(p1 &  ~0x3fL));
    TSTFREG1(f2);
    TSTPRED(_p);
    inst((5L<<37)|(((fc>>7)&3L)<<33)|(p2<<27)|
	 ((fc&0x7fL)<<20)|(f2<<13)|(ta<<12)|(p1<<6)|_p, INST_F);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
F6x_(jit_state_t *_jit, jit_word_t _p,
     jit_word_t op, jit_word_t q, jit_word_t sf,
     jit_word_t p2,  jit_word_t f3, jit_word_t f2, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(op &  ~0xfL));
    assert(!(q  &  ~0x1L));
    assert(!(sf &  ~0x3L));
    assert(!(p2 & ~0x3fL));
    assert(!(f3 & ~0x7fL));
    assert(!(f2 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTFREG2(f2, f3);
    TSTPRED(_p);
    TSTFREG1(f1);
    inst((op<<37)|(q<<36)|(sf<<34)|(1L<<33)|
	 (p2<<27)|(f3<<20)|(f2<<13)|(f1<<6)|_p, INST_F);
    SETFREG(f1);
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
F8_(jit_state_t *_jit, jit_word_t _p,
    jit_word_t op, jit_word_t sf, jit_word_t x6,
    jit_word_t f3, jit_word_t f2, jit_word_t f1)
{
    assert(!(_p & ~0x3fL));
    assert(!(op &  ~0xfL));
    assert(!(sf &  ~0x3L));
    assert(!(x6 & ~0x3fL));
    assert(!(f3 & ~0x7fL));
    assert(!(f2 & ~0x7fL));
    assert(!(f1 & ~0x7fL));
    TSTFREG2(f2, f3);
    TSTPRED(_p);
    TSTFREG1(f1);
    inst((op<<37)|(sf<<34)|(x6<<27)|(f3<<20)|(f2<<13)|(f1<<6)|_p, INST_F);
    SETFREG(f1);
}

static void
F12_(jit_state_t *_jit, jit_word_t _p,
     jit_word_t sf, jit_word_t x6, jit_word_t omsk, jit_word_t amsk)
{
    assert(!(_p   & ~0x3fL));
    assert(!(sf   &  ~0x3L));
    assert(!(x6   & ~0x3fL));
    assert(!(omsk & ~0x7fL));
    assert(!(amsk & ~0x7fL));
    TSTPRED(_p);
    inst((sf<<34)|(x6<<27)|(omsk<<20)|(amsk<<13), INST_F);
}

static void
F14x_(jit_state_t* _jit, jit_word_t _p,
      jit_word_t sf,  jit_word_t x, jit_word_t x6, jit_word_t im)
{
    assert(!(_p &    ~0x3fL));
    assert(!(sf &     ~0x3L));
    assert(!(x  &     ~0x1L));
    assert(!(x6 &    ~0x3fL));
    assert(!(im & ~0x1ffffL));
    TSTPRED(_p);
    inst((((im>>20)&1L)<<36)|(sf<<34)|(x<<33)|
	 (x6<<27)|((im&0xffffL)<<6)|_p, INST_F);
}

static void
F16_(jit_state_t* _jit, jit_word_t _p,
     jit_word_t y, jit_word_t im)
{
    assert(!(_p &    ~0x3fL));
    assert(!(y  &     ~0x1L));
    assert(!(im & ~0x1ffffL));
    TSTPRED(_p);
    inst((((im>>20)&1L)<<36)|(y<<27)|(1L<<26)|((im&0xffffL)<<6)|_p, INST_F);
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
	SETF_S(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldi_f(r0, (jit_word_t)i0);
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;

    if (_jitc->no_data) {
	data.d = *i0;
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.w);
	SETF_D(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldi_d(r0, (jit_word_t)i0);
}

static void
_movr_w_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* Should be used only in this case (with out0 == 120) */
    if (r1 >= 120)
	r1 = _jitc->rout + (r1 - 120);
    SETF_S(r0, r1);
}

static void
_movr_f_w(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* Should actually be used only in this case (with out0 == 120) */
    if (r0 >= 120)
	r0 = _jitc->rout + (r0 - 120);
    GETF_S(r0, r1);
}

static void
_movi_f_w(jit_state_t *_jit, jit_int32_t r0, jit_float32_t *i0)
{
    jit_data_t		data;

    /* Should be used only in this case (with out0 == 120) */
    if (r0 >= 120)
	r0 = _jitc->rout + (r0 - 120);
    if (_jitc->no_data) {
	data.f = *i0;
	movi(r0, data.q.l);
    }
    else
	ldi_i(r0, (jit_word_t)i0);
}

static void
_movr_w_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* Should be used only in this case (with out0 == 120) */
    if (r1 >= 120)
	r1 = _jitc->rout + (r1 - 120);
    SETF_D(r0, r1);
}

static void
_movr_d_w(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* Should be used only in this case (with out0 == 120) */
    if (r0 >= 120)
	r0 = _jitc->rout + (r0 - 120);
    GETF_D(r0, r1);
}

static void
_movi_d_w(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;

    /* Should be used only in this case (with out0 == 120) */
    if (r0 >= 120)
	r0 = _jitc->rout + (r0 - 120);
    if (_jitc->no_data) {
	data.d = *i0;
	movi(r0, data.w);
    }
    else
	ldi_l(r0, (jit_word_t)i0);
}

#define fpr_opi(name, type, size)					\
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
#define fpr_bopi(name, type, size)					\
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
#define fopi(name)			fpr_opi(name, f, 32)
#define fbopi(name)			fpr_bopi(name, f, 32)
#define dopi(name)			fpr_opi(name, d, 64)
#define dbopi(name)			fpr_bopi(name, d, 64)

fopi(add)
fopi(sub)
fopi(rsb)
fopi(mul)
fopi(div)
dopi(add)
dopi(sub)
dopi(rsb)
dopi(mul)
dopi(div)

/* translation from gcc -O0 */
static void
_divr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0, t1, t2;
    t0 = jit_get_reg(jit_class_fpr);
    t1 = jit_get_reg(jit_class_fpr);
    t2 = jit_get_reg(jit_class_fpr);
    FRCPA(rn(t0), PR_6, r1, r2);
    FNMA_p(rn(t1), r2, rn(t0), GR_1, SF_S1, PR_6);
    FMA_p(rn(t2), rn(t0), rn(t1), rn(t0), SF_S1, PR_6);
    FMPY_p(rn(t1), rn(t1), rn(t1), SF_S1, PR_6);
    FMA_p(rn(t2), rn(t2), rn(t1), rn(t2), SF_S1, PR_6);
    FMPY_p(rn(t1), rn(t1), rn(t1), SF_S1, PR_6);
    FMA_p(rn(t1), rn(t2), rn(t1), rn(t2), SF_S1, PR_6);
    FMPY_S_p(rn(t2), r1, rn(t1), SF_S1, PR_6);
    FNMA_p(rn(t0), r2, rn(t2), r1, SF_S1, PR_6);
    FMA_S_p(r0, rn(t0), rn(t1), rn(t2), SF_S0, PR_6);
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}

static void
_divr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0, t1, t2;
    t0 = jit_get_reg(jit_class_fpr);
    t1 = jit_get_reg(jit_class_fpr);
    t2 = jit_get_reg(jit_class_fpr);
    FRCPA(rn(t0), PR_6, r1, r2);
    FNMA_p(rn(t1), r2, rn(t0), GR_1, SF_S1, PR_6);
    FMA_p(rn(t2), rn(t0), rn(t1), rn(t0), SF_S1, PR_6);
    FMPY_p(rn(t1), rn(t1), rn(t1), SF_S1, PR_6);
    FMA_p(rn(t2), rn(t2), rn(t1), rn(t2), SF_S1, PR_6);
    FMPY_p(rn(t1), rn(t1), rn(t1), SF_S1, PR_6);
    FMA_p(rn(t1), rn(t2), rn(t1), rn(t2), SF_S1, PR_6);
    FMPY_D_p(rn(t2), r1, rn(t1), SF_S1, PR_6);
    FNMA_p(rn(t0), r2, rn(t2), r1, SF_S1, PR_6);
    FMA_D_p(r0, rn(t0), rn(t1), rn(t2), SF_S0, PR_6);
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}

static void
_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    SETF_SIG(rn(reg), r1);
    FCVT_XF(r0, rn(reg));
    FNORM_S(r0, r0);
    jit_unget_reg(reg);
}

static void
_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    SETF_SIG(rn(reg), r1);
    FCVT_XF(r0, rn(reg));
    FNORM_D(r0, r0);
    jit_unget_reg(reg);
}

static void
_truncr_d_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    FCVT_FX_TRUNC(rn(reg), r1);
    GETF_SIG(r0, rn(reg));
    FNORM(r0, r0);
    jit_unget_reg(reg);
}

static void
_ltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LT(PR_6, PR_7, r1, r2);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}
fopi(lt)
dopi(lt)

static void
_ler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LE(PR_6, PR_7, r1, r2);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}
fopi(le)
dopi(le)

static void
_eqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_EQ(PR_6, PR_7, r1, r2);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}
fopi(eq)
dopi(eq)

static void
_ger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LE(PR_6, PR_7, r2, r1);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}
fopi(ge)
dopi(ge)

static void
_gtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LT(PR_6, PR_7, r2, r1);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}
fopi(gt)
dopi(gt)

static void
_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_EQ(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}
fopi(ne)
dopi(ne)

static void
_unltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LE(PR_6, PR_7, r2, r1);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}
fopi(unlt)
dopi(unlt)

static void
_unler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LT(PR_6, PR_7, r2, r1);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}
fopi(unle)
dopi(unle)

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MOVI(r0, 1);
    FCMP_LT(PR_8, PR_9, r1, r2);
    FCMP_LT(PR_6, PR_7, r2, r1);
    MOV_p(r0, GR_0, PR_8);		/* !(r1 < r2) && !(r2 < r1) */
    MOV_p(r0, GR_0, PR_6);
}
fopi(uneq)
dopi(uneq)

static void
_unger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LT(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}
fopi(unge)
dopi(unge)

static void
_ungtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_LE(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}
fopi(ungt)
dopi(ungt)

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MOVI(r0, 1);
    FCMP_EQ(PR_8, PR_9, r1, r2);
    FCMP_UNORD(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_8);
    MOV_p(r0, GR_0, PR_6);
}
fopi(ltgt)
dopi(ltgt)

static void
_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_UNORD(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}
fopi(ord)
dopi(ord)

static void
_unordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_UNORD(PR_6, PR_7, r1, r2);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}
fopi(unord)
dopi(unord)

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_f(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_f(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (r0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_f(r0, r1);
}

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_d(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_d(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (r0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_d(r0, r1);
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_f(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_f(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_f(rn(reg), r1);
	jit_unget_reg(reg);
    }
    else
	str_f(r0, r1);
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_d(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_d(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_d(rn(reg), r1);
	jit_unget_reg(reg);
    }
    else
	str_d(r0, r1);
}

static void
_sqrtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    movr_f(GR_8, r1);
    calli((jit_word_t)sqrtf);
    MOVF(r0, GR_8);
}

static void
_sqrtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    movr_d(GR_8, r1);
    calli((jit_word_t)sqrt);
    MOVF(r0, GR_8);
}

static jit_word_t
_bltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LT(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}
fbopi(lt)
dbopi(lt)

static jit_word_t
_bler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LE(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}
fbopi(le)
dbopi(le)

static jit_word_t
_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_EQ(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}
fbopi(eq)
dbopi(eq)

static jit_word_t
_bger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LE(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}
fbopi(ge)
dbopi(ge)

static jit_word_t
_bgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LT(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}
fbopi(gt)
dbopi(gt)

static jit_word_t
_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_EQ(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}
fbopi(ne)
dbopi(ne)

static jit_word_t
_bunltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LE(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}
fbopi(unlt)
dbopi(unlt)

static jit_word_t
_bunler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LT(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}
fbopi(unle)
dbopi(unle)

static jit_word_t
_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_word_t		junord, jne;
    FCMP_UNORD(PR_8, PR_9, r0, r1);
    sync();
    /* junord L1 */
    junord = _jit->pc.w;
    BRI_COND(3, PR_8);
    FCMP_EQ(PR_6, PR_7, r0, r1);
    sync();
    /* jne L2 */
    jne = _jit->pc.w;
    BRI_COND(2, PR_7);
    sync();
    w = _jit->pc.w;
    /* L1: */
    patch_at(jit_code_bunordr_d, junord, _jit->pc.w);
    BRI((i0 - w) >> 4);		/* unconditional jump to patch */
    sync();
    /* L2: */
    patch_at(jit_code_bner_d, jne, _jit->pc.w);
    return (w);
}
fbopi(uneq)
dbopi(uneq)

static jit_word_t
_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LT(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}
fbopi(unge)
dbopi(unge)

static jit_word_t
_bungtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_LE(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}
fbopi(ungt)
dbopi(ungt)

static jit_word_t
_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_word_t		jeq, junord;
    FCMP_EQ(PR_8, PR_9, r0, r1);
    /* jeq L1 */
    sync();
    jeq = _jit->pc.w;
    BRI_COND(4, PR_8);
    FCMP_UNORD(PR_6, PR_7, r0, r1);
    /* junord L1 */
    sync();
    junord = _jit->pc.w;
    BRI_COND(2, PR_6);
    sync();
    w = _jit->pc.w;
    BRI((i0 - w) >> 4);		/* unconditional jump to patch */
    /* L1 */
    sync();
    patch_at(jit_code_beqr_d, jeq, _jit->pc.w);
    patch_at(jit_code_bordr_d, junord, _jit->pc.w);
    return (w);
}
fbopi(ltgt)
dbopi(ltgt)

static jit_word_t
_bordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_UNORD(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}
fbopi(ord)
dbopi(ord)

static jit_word_t
_bunordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_UNORD(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}
fbopi(unord)
dbopi(unord)

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Load argument. */
    ldr_d(r0, r1);
    /* Update va_list. */
    addi(r1, r1, 8);
}
#endif
