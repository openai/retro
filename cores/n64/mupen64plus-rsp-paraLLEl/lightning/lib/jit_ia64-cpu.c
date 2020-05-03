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
#define stack_framesize			144
#define params_offset			16
#define INST_NONE			0	/* should never be generated */
#define INST_STOP			1	/* or'ed if stop is required */
#define INST_A				2	/* M- or I- unit */
#define INST_As				3
#define INST_I				4
#define INST_Is				5
#define INST_M				6
#define INST_Ms				7
#define INST_F				8
#define INST_Fs				9
#define INST_B				10
#define INST_Bs				11
#define INST_L				12
#define INST_Ls				13
#define INST_X				14
#define INST_Xs				15

/* Data and instructions are referenced by 64-bit addresses. Instructions
 * are stored in memory in little endian byte order, in which the least
 * significant byte appears in the lowest addressed byte of a memory
 * location. For data, modes for both big and little endian byte order are
 * supported and can be controlled by a bit in the User Mask Register.
 */
#define il(ii)				*_jit->pc.ul++ = ii
#define set_bundle(p, l, h, tm, s0, s1, s2)				\
    do {								\
	l = tm | ((s0 & 0x1ffffffffffL) << 5L) | (s1 << 46L);		\
	h = ((s1 >> 18L) & 0x7fffffLL) | (s2 << 23L);			\
	p[0] = byte_swap_if_big_endian(l);				\
	p[1] = byte_swap_if_big_endian(h);				\
    } while (0)
#define get_bundle(p, l, h, tm, s0, s1, s2)				\
    do {								\
	l = byte_swap_if_big_endian(p[0]);				\
	h = byte_swap_if_big_endian(p[1]);				\
	tm = l & 0x1f;							\
	s0 = (l >> 5L) & 0x1ffffffffffL;				\
	s1 = ((l >> 46L) & 0x3ffffL) | ((h & 0x7fffffL) << 18L);	\
	s2 = (h >> 23L) & 0x1ffffffffffL;				\
    } while (0)

/*  Need to insert a stop if a modified register would (or could)
 *  be read in the same cycle.
 */
#define TSTREG1(r0)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->regs, r0))			\
	    stop();							\
    } while (0)
#define TSTREG2(r0, r1)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->regs, r0) ||			\
	    jit_regset_tstbit(&_jitc->regs, r1))			\
	    stop();							\
    } while (0)
#define TSTPRED(p0)							\
    do {								\
	if (p0 && (_jitc->pred & (1 << p0)))				\
	    stop();							\
    } while (0)
/* Record register was modified */
#define SETREG(r0)		jit_regset_setbit(&_jitc->regs, r0)

/* Avoid using constants in macros and code */
typedef enum {
    GR_0,		GR_1,		GR_2,		GR_3,
    GR_4,		GR_5,		GR_6,		GR_7,
    GR_8,		GR_9,		GR_10,		GR_11,
    GR_12,		GR_13,		GR_14,		GR_15,
    GR_16,		GR_17,		GR_18,		GR_19,
    GR_20,		GR_21,		GR_22,		GR_23,
    GR_24,		GR_25,		GR_26,		GR_27,
    GR_28,		GR_29,		GR_30,		GR_31,
    GR_32,		GR_33,		GR_34,		GR_35,
    GR_36,		GR_37,		GR_38,		GR_39,
    GR_40,		GR_41,		GR_42,		GR_43,
    /* GR_44...GR_127 */
} gr_reg_t;

typedef enum {
    PR_0,		/* constant - always 1 */
    /* p0-p5		 -  preserved */
    PR_1,		PR_2,		PR_3,		PR_4,
    PR_5,
    /* p6-p15		 - scratch */
    PR_6,		PR_7,		PR_8,		PR_9,
    PR_10,		PR_11,		PR_12,		PR_13,
    PR_14,		PR_15,
    /* p16-...		 - preserved - rotating */
} pr_reg_t;

typedef enum {
    BR_0,		/* scratch - Return link */
    /* b1-b5		 - preserved */
    BR_1,		BR_2,		BR_3,		BR_4,
    BR_5,
    /* b6-b7		- scratch */
    BR_6,		BR_7
} br_reg_t;

typedef enum {
    AR_KR0,		AR_KR1,		AR_KR2,		AR_KR3,
    AR_KR4,		AR_KR5,		AR_KR6,		AR_KR7,
    AR_8,		AR_9,		AR_10,		AR_11,
    AR_12,		AR_13,		AR_14,		AR_15,
    AR_RSC,		AR_BSP,		AR_BSPSTORE,	AR_RNAT,
    AR_20,		AR_FCR,		AR_22,		AR_23,
    AR_EFLAG,		AR_CSD,		AR_SSD,		AR_CFLG,
    AR_FSR,		AR_FIR,		AR_FDR,		AR_31,
    AR_CCV,		AR_33,		AR_34,		AR_35,
    AR_UNAT,		AR_37,		AR_38,		AR_39,
    AR_FPSR,		AR_41,		AR_42,		AR_43,
    AR_ITC,		AR_RUC,		AR_46,		AR_47,
    AR_48,		AR_49,		AR_50,		AR_51,
    AR_52,		AR_53,		AR_54,		AR_55,
    AR_56,		AR_57,		AR_58,		AR_59,
    AR_60,		AR_61,		AR_62,		AR_63,
    AR_PFS,		AR_LC,		AR_EC,
    /* AR_67 ... AR_127 */
} ar_reg_t;

typedef enum {
    TM_M_I_I_,	TM_M_I_Is,	TM_M_IsI_,	TM_M_IsIs,
    TM_M_L_X_,	TM_M_L_Xs,	TM_ILL_06,	TM_ILL_07,
    TM_M_M_I_,	TM_M_M_Is,	TM_MsM_I_,	TM_MsM_Is,
    TM_M_F_I_,	TM_M_F_Is,	TM_M_M_F_,	TM_M_M_Fs,
    TM_M_I_B_,	TM_M_I_Bs,	TM_M_B_B_,	TM_M_B_Bs,
    TM_ILL_14,	TM_ILL_15,	TM_B_B_B_,	TM_B_B_Bs,
    TM_M_M_B_,	TM_M_M_Bs,	TM_ILL_1A,	TM_ILL_1B,
    TM_M_F_B_,	TM_M_F_Bs,	TM_ILL_1E,	TM_ILL_1F,
} template_t;

#define MWH_SPTK			0
#define MWH_NONE			1
#define MWH_DPTK			2

#define IH_NONE				0
#define IH_IMP				1

#define LD_NONE				0
#define LD_NT1				1
#define LD_NTA				3

#define ST_NONE				0
#define ST_NTA				3

#define LF_NONE				0
#define LF_NT1				1
#define LF_NT2				2
#define LF_NTA				3

#define BR_PH_FEW			0
#define BR_PH_MANY			1

#define BR_BWH_SPTK			0
#define BR_BWH_SPNT			1
#define BR_BWH_DPTK			2
#define BR_BWH_DPNT			3

#define BRI_BWH_SPTK			1
#define BRI_BWH_SPNT			3
#define BRI_BWH_DPTK			5
#define BRI_BWH_DPNT			7

#define BR_DH_NONE			0
#define BR_DH_CLR			1

#define BR_IH_NONE			0
#define BR_IH_IMP			1

#define BR_IPWH_SPTK			0
#define BR_IPWH_LOOP			1
#define BR_IPWH_DPTK			2
#define BR_IPWH_EXIT			3

#define BR_INDWH_SPTK			0
#define BR_INDWH_DPTK			2

#define MUX_BRCST			0
#define MUX_REV				11
#define MUX_MIX				8
#define MUX_SHUF			9
#define MUX_ALT				10

#define ldr(r0,r1)			ldr_l(r0,r1)
#define ldi(r0,i0)			ldi_l(r0,i0)
#define str(r0,r1)			str_l(r0,r1)
#define sti(i0,r0)			str_l(i0,r0)
#define ldxr(r0,r1,r2)			ldxr_l(r0,r1,r2)
#define ldxi(r0,r1,i0)			ldxi_l(r0,r1,i0)
#define stxr(r0,r1,r2)			stxr_l(r0,r1,r2)
#define stxi(i0,r0,r1)			stxi_l(i0,r0,r1)

#if !HAVE_FFSL
#  define ffsl(l)			__builtin_ffsl(l)
#endif

/* libgcc */
#if defined(__GNUC__)
extern long __divdi3(long,long);
extern unsigned long __udivdi3(unsigned long,unsigned long);
extern long __moddi3(long,long);
extern unsigned long __umoddi3(unsigned long,unsigned long);
#else
static long __divdi3(long,long);
static unsigned long __udivdi3(unsigned long,unsigned long);
static long __moddi3(long,long);
static unsigned long __umoddi3(unsigned long,unsigned long);
#endif
#define out(n,tm,s0,s1,s2)		_out(_jit,n,tm,s0,s1,s2)
static void _out(jit_state_t*,int,int,jit_word_t,jit_word_t,jit_word_t);
#define stop()				_stop(_jit)
static void _stop(jit_state_t*);
#define sync()				_sync(_jit)
static void _sync(jit_state_t*);
#define flush()				_flush(_jit)
static void _flush(jit_state_t*);
#define inst(w, t)			_inst(_jit, w, t)
static void _inst(jit_state_t*, jit_word_t, jit_uint8_t);
#define A1(x4,x2,r3,r2,r1)		_A1(_jit,0,x4,x2,r3,r2,r1)
static void _A1(jit_state_t*, jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define A2(x4,ct,r3,r2,r1)		A1(x4,ct,r3,r2,r1)
#define A3(x4,x2,r3,im,r1)		_A3(_jit,0,x4,x2,r3,im,r1)
static void _A3(jit_state_t*, jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define A4(x2a,r3,im,r1)		_A4(_jit,0,x2a,r3,im,r1)
static void _A4(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define A5(r3,im,r1)			_A5(_jit,0,r3,im,r1)
static void _A5(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t);
#define A6(o,x2,ta,p2,r3,r2,c,p1)	_A6(_jit,0,o,x2,ta,p2,r3,r2,c,p1)
static void _A6(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define A7(o,x2,ta,p2,r3,c,p1)		_A7(_jit,0,o,x2,ta,p2,r3,c,p1)
static void _A7(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define A8(o,x2,ta,p2,r3,im,c,p1)	_A8(_jit,0,o,x2,ta,p2,r3,im,c,p1)
static void _A8(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define A9(za,zb,x4,x2,r3,r2,r1)	_A9(_jit,0,za,zb,x4,x2,r3,r2,r1)
static void _A9(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define A10(x4,ct,r3,r2,r1)		A9(0,1,x4,ct,r3,r2,r1)
#define I1(ct,x2,r3,r2,r1)		_I1(_jit,0,ct,x2,r3,r2,r1)
static void _I1(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I2(za,x2a,zb,x2c,x2b,r3,r2,r1)	_I2(_jit,0,za,x2a,zb,x2c,x2b,r3,r2,r1)
static void _I2(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I3(mbt,r2,r1)			_I3(_jit,0,mbt,r2,r1)
static void _I3(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t)
#if __BYTE_ORDER == __BIG_ENDIAN
    maybe_unused
#endif
    ;
#define I4(mht,r2,r1)			_I4(_jit,0,mht,r2,r1)
static void _I4(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I5(za,zb,x2b,r3,r2,r1)		_I5(_jit,0,za,zb,x2b,r3,r2,r1)
static void _I5(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t);
#define I6(za,zb,x2b,r3,ct,r1)		_I6(_jit,0,za,zb,x2b,r3,ct,r1)
static void _I6(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I7(za,zb,r3,r2,r1)		_I7(_jit,0,za,zb,r3,r2,r1)
static void _I7(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define I8(za,zb,ct,r2,r1)		_I8(_jit,0,za,zb,ct,r2,r1)
static void _I8(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I9(x2c,r3,r1)			_I9(_jit,0,x2c,r3,r1)
static void _I9(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I10(ct,r3,r2,r1)		_I10(_jit,0,ct,r3,r2,r1)
static void _I10(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I11(len,r3,pos,y,r1)		_I11(_jit,0,len,r3,pos,y,r1)
static void _I11(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I12(len,pos,r2,r1)		_I12(_jit,0,len,pos,r2,r1)
static void _I12(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I13(len,pos,im,r1)		_I13(_jit,0,len,pos,im,r1)
static void _I13(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I14(s,len,r3,pos,r1)		_I14(_jit,0,s,len,r3,pos,r1)
static void _I14(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I15(pos,len,r3,r2,r1)		_I15(_jit,0,pos,len,r3,r2,r1)
static void _I15(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I16(tb,ta,p2,r3,pos,c,p1)	_I16(_jit,0,tb,ta,p2,r3,pos,c,p1)
static void _I16(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I17(tb,ta,p2,r3,c,p1)		_I17(_jit,0,tb,ta,p2,r3,c,p1)
static void _I17(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define I18(im,y)			_I18(_jit,0,im,y)
static void _I18(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define I19(im)				_I19(_jit,0,im)
static void _I19(jit_state_t*,jit_word_t,
		 jit_word_t)
    maybe_unused;
#define I20(r2,im)			_I20(_jit,0,r2,im)
static void _I20(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define I21(im,ih,x,wh,r2,b1)		_I21(_jit,0,im,ih,x,wh,r2,b1)
static void _I21(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define I22(b2,r1)			_I22(_jit,0,b2,r1)
static void _I22(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t);
#define I23(r2,im)			_I23(_jit,0,r2,im)
static void _I23(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define I24(im)				_I24(_jit,0,im)
static void _I24(jit_state_t*,jit_word_t,
		 jit_word_t)
    maybe_unused;
#define I25(x6,r1)			_I25(_jit,0,x6,r1)
static void _I25(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define I26(ar,r2)			_I26(_jit,0,ar,r2)
static void _I26(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t);
#define I27(ar,im)			_I27(_jit,0,ar,im)
static void _I27(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define I28(ar,r1)			_I28(_jit,0,ar,r1)
static void _I28(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define I29(x6,r3,r1)			_I29(_jit,0,x6,r3,r1)
static void _I29(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t);
#define I30(tb,ta,p2,im,c,p1)		_I30(_jit,0,ta,tb,p2,im,c,p1)
static void _I30(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M1(x6,ht,x,r3,r1)		_M1(_jit,0,x6,ht,x,r3,r1)
static void _M1(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define M2(x6,ht,r3,r2,r1)		_M2(_jit,0,x6,ht,r3,r2,r1)
static void _M2(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M3(x6,ht,r3,im,r1)		_M3(_jit,0,x6,ht,r3,im,r1)
static void _M3(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define M5(x6,ht,r3,r2,im)		_M5(_jit,0,x6,ht,r3,r2,im)
static void _M5(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M6(x6,ht,x,r3,r2)		_M6(_jit,0,x6,ht,x,r3,r2)
static void _M6(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define M13(x6,ht,r3,f2)		_M13(_jit,0,x6,ht,r3,f2)
static void _M13(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define M14(x6,ht,r3,r2)		_M14(_jit,0,x6,ht,r3,r2)
static void _M14(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M15(x6,ht,r3,im)		_M15(_jit,0,x6,ht,r3,im)
static void _M15(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M16(x6,ht,r3,r2,r1)		_M16(_jit,0,x6,ht,r3,r2,r1)
static void _M16(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M17(x6,ht,r3,im,r1)		_M17(_jit,0,x6,ht,r3,im,r1)
static void _M17(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M20(r2,im)			M20x(0x1,r2,im)
#define M20x(x3,r2,im)			_M20x(_jit,0,x3,r2,im)
static void _M20x(jit_state_t*,jit_word_t,
		  jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M22(x3,im,r1)			M22x(x3,im,r1)
#define M22x(x3,im,r1)			_M22x(_jit,0,x3,im,r1)
static void _M22x(jit_state_t*,jit_word_t,
		  jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M24(x2,x4)			_M24(_jit,0,x2,x4)
#define M25(x4)				M24(0,x4)
static void _M24(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M26(r1)				M26x(2,r1)
#define M26x(x4,r1)			_M26x(_jit,0,x4,r1)
static void _M26x(jit_state_t*,jit_word_t,
		  jit_word_t,jit_word_t)
    maybe_unused;
#define M28(x,r3)			_M28(_jit,0,x,r3)
static void _M28(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M29(ar,r2)			_M29(_jit,0,ar,r2)
static void _M29(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M30(ar,im)			_M30(_jit,0,ar,im)
static void _M30(jit_state_t*,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M31(ar,r1)			_M31(_jit,0,ar,r1)
static void _M31(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M32(cr,r2)			_M32(_jit,0,cr,r2)
static void _M32(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M33(cr,r1)			_M33(_jit,0,cr,r1)
static void _M33(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M34(sor,sol,sof,r1)		_M34(_jit,0,sor,sol,sof,r1)
static void _M34(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define M35(x6,r2)			_M35(_jit,0,x6,r2)
static void _M35(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M36(x6,r1)			_M36(_jit,0,x6,r1)
static void _M36(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M37(im)				_M37(_jit,0,im)
static void _M37(jit_state_t*,jit_word_t,
		 jit_word_t)
    maybe_unused;
#define M38(x6,r3,r2,r1)		_M38(_jit,0,x6,r3,r2,r1)
static void _M38(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M39(x6,r3,im,r1)		_M39(_jit,0,x6,r3,im,r1)
static void _M39(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M40(x6,r3,im)			_M40(_jit,0,x6,r3,im)
static void _M40(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M41(x6,r2)			_M41(_jit,0,x6,r2)
static void _M41(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M42(x6,r3,r2)			_M42(_jit,0,x6,r3,r2)
static void _M42(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M43(x6,r3,r1)			_M43(_jit,0,x6,r3,r1)
static void _M43(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M44(x4,im)			_M44(_jit,0,x4,im)
static void _M44(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define M45(x6,r3,r2)			_M45(_jit,0,x6,r3,r2)
static void _M45(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M46(x6,r3,r1)			_M46(_jit,0,x6,r3,r1)
#define M47(x6,r3)			M46(x6,r3,0)
static void _M46(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define M48(y,im)			_M48(_jit,0,y,im)
static void _M48(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t)
    maybe_unused;
#define B1(d,wh,im,p,tp)		_B1(_jit,0,d,wh,im,p,tp)
#define B2(d,wh,im,p,tp)		B1(d,wh,im,p,tp)
static void _B1(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define B3(d,wh,im,p,b)			_B3(_jit,0,d,wh,im,p,b)
static void _B3(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define B4(d,wh,x6,b,p,tp)		_B4(_jit,0,d,wh,x6,b,p,tp)
static void _B4(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t);
#define B5(d,wh,b2,p,b1)		_B5(_jit,0,d,wh,b2,p,b1)
static void _B5(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t);
#define B6(ih,im,tag,wh)		_B6(_jit,0,ih,im,tag,wh)
static void _B6(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define B7(ih,x6,b2,tag,wh)		_B7(_jit,0,ih,x6,b2,tag,wh)
static void _B7(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define B8(x6)				_B8(_jit,0,x6)
static void _B8(jit_state_t*,jit_word_t,
		jit_word_t)
    maybe_unused;
#define B9(op,x6,im)			_B9(_jit,0,op,x6,im)
static void _B9(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t,jit_word_t)
    maybe_unused;
#define X1(im)				_X1(_jit,0,im)
static void _X1(jit_state_t*,jit_word_t,
		jit_word_t)
    maybe_unused;
#define X2(r1,im)			_X2(_jit,0,r1,im)
static void _X2(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t);
#define X3x(o,d,wh,p,tp,im)		_X3x(_jit,0,o,d,wh,p,tp,im)
#define X3(d,wh,p,tp,im)		X3x(0xc,d,wh,p,tp,im)
#define X4(d,wh,p,tp,im)		X3x(0xd,d,wh,p,tp,im)
static void _X3x(jit_state_t*,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t,
		 jit_word_t,jit_word_t,jit_word_t);
#define X5(y,im)			_X5(_jit,0,y,im)
static void _X5(jit_state_t*,jit_word_t,
		jit_word_t,jit_word_t)
    maybe_unused;

/* add */
#define ADD(r1,r2,r3)			A1(0,0,r3,r2,r1)
#define ADD1(r1,r2,r3)			A1(0,1,r3,r2,r1)
#define ADDS(r1,im,r3)			A4(2,r3,im,r1)
#define ADDS_p(r1,im,r3,_p)		_A4(_jit,_p,2,r3,im,r1)
#define ADDL(r1,im,r3)			A5(r3,im,r1)
#define ADDL_p(r1,im,r3,_p)		_A5(_jit,_p,r3,im,r1)
/* addp4 */
#define ADDP4(r1,r2,r3)			A1(2,0,r3,r2,r1)
#define ADDIP4(r1,im,r3)		A4(3,r3,im,r1)
/* alloc */
#define ALLOCR(r1,i,l,o,r)		M34((r)>>3,(i)+(l),(i)+(l)+(o),r1)
#define ALLOC(r1,i,o)			ALLOCR(r1,i,0,o,0)
/* and */
#define AND(r1,r2,r3)			A1(3,0,r3,r2,r1)
#define ANDI(r1,im,r3)			A3(0xb,0,r3,im,r1)
/* andcm */
#define ANDCM(r1,r2,r3)			A1(3,1,r3,r2,r1)
#define ANDCMI(r1,im,r3)		A3(0xb,1,r3,im,r1)
/* br */
#define BRI(im)				B1(BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_FEW,0)
#define BRI_COND(im,_p)			_B1(_jit,_p,BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_FEW,0)
#define BRI_WEXIT(im)			B1(BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_MANY,2)
#define BRI_WTOP(im)			B1(BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_MANY,3)
#define BRI_CALL(b,im)			B3(BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_MANY,b)
#define BRI_CLOOP(im)			B2(BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_MANY,5)
#define BRI_CEXIT(im)			B2(BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_MANY,6)
#define BRI_CTOP(im)			B2(BR_DH_NONE,BR_BWH_SPTK,im,BR_PH_MANY,7)
#define BR_COND(b,_p)			_B4(_jit,_p,BR_DH_NONE,BR_BWH_SPTK,0x20,b,BR_PH_FEW,0)
#define BR(b)				B4(BR_DH_NONE,BR_BWH_SPTK,0x20,b,BR_PH_FEW,0)
#define BR_IA(b)			B4(BR_DH_NONE,BR_BWH_SPTK,0x20,b,BR_PH_MANY,1)
#define BR_RET(b)			B4(BR_DH_NONE,BR_BWH_SPTK,0x21,b,BR_PH_MANY,4)
#define BR_CALL(b1,b2)			B5(BR_DH_NONE,BRI_BWH_SPTK,b2,BR_PH_MANY,b1)
/* break */
#define BREAK_I(im)			I19(im)
#define BREAK_M(im)			M37(im)
#define BREAK_B(im)			B9(0,0,im)
#define BREAK_X(im)			X1(im)
/* brl */
#define BRL(im)				X3(BR_DH_NONE,BR_BWH_SPTK,BR_PH_MANY,0,im)
#define BRL_COND(im,_p)			_X3(_jit,_p,BR_DH_NONE,BR_BWH_SPTK,BR_PH_MANY,0,im)
#define BRL_CALL(b1,im)			X4(BR_DH_NONE,BR_BWH_SPTK,BR_PH_MANY,b1,im)
/* brp */
#define BRP(im,tag)			B6(BR_IH_NONE,im,tag,BR_IPWH_SPTK)
#define BRPI(b2,tag)			B7(BR_IH_NONE,0x10,b2,tag,BR_INDWH_SPTK)
#define BRPI_RET(b2,tag)		B7(BR_IH_NONE,0x11,b2,tag,BR_INDWH_SPTK)
/* bsw */
#define BSW_0()				B8(0x0c)
#define BSW_1()				B8(0x0d)
/* chk */
#define CHK_S_I(r2,im)			I20(r2,im)
#define CHK_S_M(r2,im)			M20(r2,im)
#define CHK_A_NC(r1,im)			M22(0x4,im,r1)
#define CHK_A_CLR(r1,im)		M22(0x5,im,r1)
/* clrrrb */
#define CLRRRB()			B8(0x04)
#define CLRRRB_PR()			B8(0x05)
/* clz */
#define CLZ(r1,r3)			I9(3,r3,r1)
/* cmp */
#define CMP_LT(p1,p2,r2,r3)		A6(0xc,0,0,p2,r3,r2,0,p1)
#define CMP_LT_p(p1,p2,r2,r3,_p)	A6(_jit,_p,0xc,0,0,p2,r3,r2,0,p1)
#define CMP_LTU(p1,p2,r2,r3)		A6(0xd,0,0,p2,r3,r2,0,p1)
#define CMP_EQ(p1,p2,r2,r3)		A6(0xe,0,0,p2,r3,r2,0,p1)
#define CMP_LT_UNC(p1,p2,r2,r3)		A6(0xc,0,0,p2,r3,r2,1,p1)
#define CMP_LTU_UNC(p1,p2,r2,r3)	A6(0xd,0,0,p2,r3,r2,1,p1)
#define CMP_EQ_UNC(p1,p2,r2,r3)		A6(0xe,0,0,p2,r3,r2,1,p1)
#define CMP_EQ_AND(p1,p2,r2,r3)		A6(0xc,0,1,p2,r3,r2,0,p1)
#define CMP_EQ_OR(p1,p2,r2,r3)		A6(0xd,0,1,p2,r3,r2,0,p1)
#define CMP_EQ_OR_ANDCM(p1,p2,r2,r3)	A6(0xe,0,1,p2,r3,r2,0,p1)
#define CMP_NE_AND(p1,p2,r2,r3)		A6(0xc,0,1,p2,r3,r2,1,p1)
#define CMP_NE_OR(p1,p2,r2,r3)		A6(0xd,0,1,p2,r3,r2,1,p1)
#define CMP_NE_OR_ANDCM(p1,p2,r2,r3)	A6(0xe,0,1,p2,r3,r2,1,p1)
#define CMPI_LT(p1,p2,im,r3)		A8(0xc,2,0,p2,r3,im,0,p1)
#define CMPI_LTU(p1,p2,im,r3)		A8(0xd,2,0,p2,r3,im,0,p1)
#define CMPI_EQ(p1,p2,im,r3)		A8(0xe,2,0,p2,r3,im,0,p1)
#define CMPI_EQ_p(p1,p2,im,r3,_p)	_A8(_jit,_p,0xe,2,0,p2,r3,im,0,p1)
#define CMPI_LT_UNC(p1,p2,im,r3)	A8(0xc,2,0,p2,r3,im,1,p1)
#define CMPI_LTU_UNC(p1,p2,im,r3)	A8(0xd,2,0,p2,r3,im,1,p1)
#define CMPI_EQ_UNC(p1,p2,im,r3)	A8(0xe,2,0,p2,r3,im,1,p1)
#define CMPI_EQ_AND(p1,p2,im,r3)	A8(0xc,2,1,p2,r3,im,0,p1)
#define CMPI_EQ_OR(p1,p2,im,r3)		A8(0xd,2,1,p2,r3,im,0,p1)
#define CMPI_EQ_ANDCM(p1,p2,im,r3)	A8(0xe,2,1,p2,r3,im,0,p1)
#define CMPI_NE_AND(p1,p2,im,r3)	A8(0xc,2,1,p2,r3,im,1,p1)
#define CMPI_NE_OR(p1,p2,im,r3)		A8(0xd,2,1,p2,r3,im,1,p1)
#define CMPI_NE_ANDCM(p1,p2,im,r3)	A8(0xe,2,1,p2,r3,im,1,p1)
#define ZCMP_GT_AND(p1,p2,r3)		A7(0xc,0,0,p2,r3,0,p1)
#define ZCMP_GT_OR(p1,p2,r3)		A7(0xd,0,0,p2,r3,0,p1)
#define ZCMP_GT_ANDCM(p1,p2,r3)		A7(0xe,0,0,p2,r3,0,p1)
#define ZCMP_LE_AND(p1,p2,r3)		A7(0xc,0,0,p2,r3,1,p1)
#define ZCMP_LE_OR(p1,p2,r3)		A7(0xd,0,0,p2,r3,1,p1)
#define ZCMP_LE_ANDCM(p1,p2,r3)		A7(0xe,0,0,p2,r3,1,p1)
#define ZCMP_GE_AND(p1,p2,r3)		A7(0xc,0,1,p2,r3,0,p1)
#define ZCMP_GE_OR(p1,p2,r3)		A7(0xd,0,1,p2,r3,0,p1)
#define ZCMP_GE_ANDCM(p1,p2,r3)		A7(0xe,0,1,p2,r3,0,p1)
#define ZCMP_LT_AND(p1,p2,r3)		A7(0xc,0,1,p2,r3,1,p1)
#define ZCMP_LT_OR(p1,p2,r3)		A7(0xd,0,1,p2,r3,1,p1)
#define ZCMP_LT_ANDCM(p1,p2,r3)		A7(0xe,0,1,p2,r3,1,p1)
/* cmp4 */
#define CMP4_LT(p1,p2,r2,r3)		A6(0xc,1,0,p2,r3,r2,0,p1)
#define CMP4_LTU(p1,p2,r2,r3)		A6(0xd,1,0,p2,r3,r2,0,p1)
#define CMP4_EQ(p1,p2,r2,r3)		A6(0xe,1,0,p2,r3,r2,0,p1)
#define CMP4_LT_UNC(p1,p2,r2,r3)	A6(0xc,1,0,p2,r3,r2,1,p1)
#define CMP4_LTU_UNC(p1,p2,r2,r3)	A6(0xd,1,0,p2,r3,r2,1,p1)
#define CMP4_EQ_UNC(p1,p2,r2,r3)	A6(0xe,1,0,p2,r3,r2,1,p1)
#define CMP4_EQ_AND(p1,p2,r2,r3)	A6(0xc,1,1,p2,r3,r2,0,p1)
#define CMP4_EQ_OR(p1,p2,r2,r3)		A6(0xd,1,1,p2,r3,r2,0,p1)
#define CMP4_EQ_XOR(p1,p2,r2,r3)	A6(0xe,1,1,p2,r3,r2,0,p1)
#define CMP4_NE_AND(p1,p2,r2,r3)	A6(0xc,1,1,p2,r3,r2,1,p1)
#define CMP4_NE_OR(p1,p2,r2,r3)		A6(0xd,1,1,p2,r3,r2,1,p1)
#define CMP4_NE_XOR(p1,p2,r2,r3)	A6(0xe,1,1,p2,r3,r2,1,p1)
#define CMP4I_LT(p1,p2,im,r3)		A8(0xc,3,0,p2,r3,im,0,p1)
#define CMP4I_LTU(p1,p2,im,r3)		A8(0xd,3,0,p2,r3,im,0,p1)
#define CMP4I_EQ(p1,p2,im,r3)		A8(0xe,3,0,p2,r3,im,0,p1)
#define CMP4I_LT_UNC(p1,p2,im,r3)	A8(0xc,3,0,p2,r3,im,1,p1)
#define CMP4I_LTU_UNC(p1,p2,im,r3)	A8(0xd,3,0,p2,r3,im,1,p1)
#define CMP4I_EQ_UNC(p1,p2,im,r3)	A8(0xe,3,0,p2,r3,im,1,p1)
#define CMP4I_EQ_AND(p1,p2,im,r3)	A8(0xc,3,1,p2,r3,im,0,p1)
#define CMP4I_EQ_OR(p1,p2,im,r3)	A8(0xd,3,1,p2,r3,im,0,p1)
#define CMP4I_EQ_ANDCM(p1,p2,im,r3)	A8(0xe,3,1,p2,r3,im,0,p1)
#define CMP4I_NE_AND(p1,p2,im,r3)	A8(0xc,3,1,p2,r3,im,1,p1)
#define CMP4I_NE_OR(p1,p2,im,r3)	A8(0xd,3,1,p2,r3,im,1,p1)
#define CMP4I_NE_ANDCM(p1,p2,im,r3)	A8(0xe,3,1,p2,r3,im,1,p1)
#define ZCMP4_GT_AND(p1,p2,r3)		A7(0xc,1,0,p2,r3,0,p1)
#define ZCMP4_GT_OR(p1,p2,r3)		A7(0xd,1,0,p2,r3,0,p1)
#define ZCMP4_GT_ANDCM(p1,p2,r3)	A7(0xe,1,0,p2,r3,0,p1)
#define ZCMP4_LE_AND(p1,p2,r3)		A7(0xc,1,0,p2,r3,1,p1)
#define ZCMP4_LE_OR(p1,p2,r3)		A7(0xd,1,0,p2,r3,1,p1)
#define ZCMP4_LE_ANDCM(p1,p2,r3)	A7(0xe,1,0,p2,r3,1,p1)
#define ZCMP4_GE_AND(p1,p2,r3)		A7(0xc,1,1,p2,r3,0,p1)
#define ZCMP4_GE_OR(p1,p2,r3)		A7(0xd,1,1,p2,r3,0,p1)
#define ZCMP4_GE_ANDCM(p1,p2,r3)	A7(0xe,1,1,p2,r3,0,p1)
#define ZCMP4_LT_AND(p1,p2,r3)		A7(0xc,1,1,p2,r3,1,p1)
#define ZCMP4_LT_OR(p1,p2,r3)		A7(0xd,1,1,p2,r3,1,p1)
#define ZCMP4_LT_ANDCM(p1,p2,r3)	A7(0xe,1,1,p2,r3,1,p1)
/* cmpxchg */
#define CMPXCHG1_ACQ(r1,r3,r2)		M16(0x00,LD_NONE,r3,r2,r1)
#define CMPXCHG2_ACQ(r1,r3,r2)		M16(0x01,LD_NONE,r3,r2,r1)
#define CMPXCHG4_ACQ(r1,r3,r2)		M16(0x02,LD_NONE,r3,r2,r1)
#define CMPXCHG8_ACQ(r1,r3,r2)		M16(0x03,LD_NONE,r3,r2,r1)
#define CMPXCHG1_REL(r1,r3,r2)		M16(0x04,LD_NONE,r3,r2,r1)
#define CMPXCHG2_REL(r1,r3,r2)		M16(0x05,LD_NONE,r3,r2,r1)
#define CMPXCHG4_REL(r1,r3,r2)		M16(0x06,LD_NONE,r3,r2,r1)
#define CMPXCHG8_REL(r1,r3,r2)		M16(0x07,LD_NONE,r3,r2,r1)
#define CMP8XCHG16_ACQ(r1,r3,r2)	M16(0x20,LD_NONE,r3,r2,r1)
#define CMP8XCHG16_REL(r1,r3,r2)	M16(0x24,LD_NONE,r3,r2,r1)
/* cover */
#define COVER()				B8(0x02)
/* cxz */
#define CZX1_L(r1,r3)			I29(0x18,r3,r1)
#define CZX2_L(r1,r3)			I29(0x19,r3,r1)
#define CZX1_R(r1,r3)			I29(0x1c,r3,r1)
#define CZX2_R(r1,r3)			I29(0x1d,r3,r1)
/* dep */
#define DEP_Z(r1,r2,pos,len)		I12(len,pos,r2,r1)
#define DEPI_Z(r1,im,pos,len)		I13(len,pos,im,r1)
#define DEPs(r1,r2,r3,pos,len)		I14(1,len,r3,pos,r1)
#define DEPu(r1,r2,r3,pos,len)		I14(0,len,r3,pos,r1)
#define DEP(r1,r2,r3,pos,len)		I15(pos,len,r3,r2,r1)
/* epc */
#define EPC()				B8(0x10)
/* extr */
#define EXTR(r1,r3,pos,len)		I11(len,r3,pos,1,r1)
#define EXTR_U(r1,r3,pos,len)		I11(len,r3,pos,0,r1)
/* fc */
#define FC(r3)				M28(0,r3)
#define FC_I(r3)			M28(1,r3)
/* fetchadd */
#define FETCHADD4_ACQ(r1,r3,im)		M17(0x12,LD_NONE,r3,im,r1)
#define FETCHADD8_ACQ(r1,r3,im)		M17(0x13,LD_NONE,r3,im,r1)
#define FETCHADD4_REL(r1,r3,im)		M17(0x16,LD_NONE,r3,im,r1)
#define FETCHADD8_REL(r1,r3,im)		M17(0x17,LD_NONE,r3,im,r1)
/* flushrs */
#define FLUSHRS()			M25(0xc)
/* fwb */
#define FWB()				M24(2,0)
/* hint */
#define HINT_I(im)			I18(im,1)
#define HINT_M(im)			M48(1,im)
#define HINT_B(im)			B9(2,1,im)
#define HINT_X(im)			X5(1,im)
/* invala */
#define INVALA()			M24(1,0)
#define INVALA_E(r1)			M26(r1)
/* itc */
#define ITC_I(r2)			M41(0x2f,r2)
#define ITC_D(r2)			M41(0x2e,r2)
/* itr */
#define ITR_I(r3,r2)			M42(0x0f,r3,r2)
#define ITR_D(r3,r2)			M42(0x0e,r3,r2)
/* ld */
#define LD1(r1,r3)			M1(0x00,LD_NONE,0,r3,r1)
#define LD2(r1,r3)			M1(0x01,LD_NONE,0,r3,r1)
#define LD4(r1,r3)			M1(0x02,LD_NONE,0,r3,r1)
#define LD8(r1,r3)			M1(0x03,LD_NONE,0,r3,r1)
#define LD1_S(r1,r3)			M1(0x04,LD_NONE,0,r3,r1)
#define LD2_S(r1,r3)			M1(0x05,LD_NONE,0,r3,r1)
#define LD4_S(r1,r3)			M1(0x06,LD_NONE,0,r3,r1)
#define LD8_S(r1,r3)			M1(0x07,LD_NONE,0,r3,r1)
#define LD1_A(r1,r3)			M1(0x08,LD_NONE,0,r3,r1)
#define LD2_A(r1,r3)			M1(0x09,LD_NONE,0,r3,r1)
#define LD4_A(r1,r3)			M1(0x0a,LD_NONE,0,r3,r1)
#define LD8_A(r1,r3)			M1(0x0b,LD_NONE,0,r3,r1)
#define LD1_SA(r1,r3)			M1(0x0c,LD_NONE,0,r3,r1)
#define LD2_SA(r1,r3)			M1(0x0d,LD_NONE,0,r3,r1)
#define LD4_SA(r1,r3)			M1(0x0e,LD_NONE,0,r3,r1)
#define LD8_SA(r1,r3)			M1(0x0f,LD_NONE,0,r3,r1)
#define LD1_BIAS(r1,r3)			M1(0x10,LD_NONE,0,r3,r1)
#define LD2_BIAS(r1,r3)			M1(0x11,LD_NONE,0,r3,r1)
#define LD4_BIAS(r1,r3)			M1(0x12,LD_NONE,0,r3,r1)
#define LD8_BIAS(r1,r3)			M1(0x13,LD_NONE,0,r3,r1)
#define LD1_ACQ(r1,r3)			M1(0x14,LD_NONE,0,r3,r1)
#define LD2_ACQ(r1,r3)			M1(0x15,LD_NONE,0,r3,r1)
#define LD4_ACQ(r1,r3)			M1(0x16,LD_NONE,0,r3,r1)
#define LD8_ACQ(r1,r3)			M1(0x17,LD_NONE,0,r3,r1)
#define LD8_FILL(r1,r3)			M1(0x1b,LD_NONE,0,r3,r1)
#define LD1_C_CLR(r1,r3)		M1(0x20,LD_NONE,0,r3,r1)
#define LD2_C_CLR(r1,r3)		M1(0x21,LD_NONE,0,r3,r1)
#define LD4_C_CLR(r1,r3)		M1(0x22,LD_NONE,0,r3,r1)
#define LD8_C_CLR(r1,r3)		M1(0x23,LD_NONE,0,r3,r1)
#define LD1_C_NC(r1,r3)			M1(0x24,LD_NONE,0,r3,r1)
#define LD2_C_NC(r1,r3)			M1(0x25,LD_NONE,0,r3,r1)
#define LD4_C_NC(r1,r3)			M1(0x26,LD_NONE,0,r3,r1)
#define LD8_C_NC(r1,r3)			M1(0x27,LD_NONE,0,r3,r1)
#define LD1_C_CLR_ACQ(r1,r3)		M1(0x28,LD_NONE,0,r3,r1)
#define LD2_C_CLR_ACQ(r1,r3)		M1(0x29,LD_NONE,0,r3,r1)
#define LD4_C_CLR_ACQ(r1,r3)		M1(0x2a,LD_NONE,0,r3,r1)
#define LD8_C_CLR_ACQ(r1,r3)		M1(0x2b,LD_NONE,0,r3,r1)
#define LD16(r1,r3)			M1(0x28,LD_NONE,1,r3,r1)
#define LD16_ACQ(r1,r3)			M1(0x2c,LD_NONE,1,r3,r1)
#define LD1_inc(r1,r3,im)		M3(0x00,LD_NONE,r3,im,r1)
#define LD2_inc(r1,r3,im)		M3(0x01,LD_NONE,r3,im,r1)
#define LD4_inc(r1,r3,im)		M3(0x02,LD_NONE,r3,im,r1)
#define LD8_inc(r1,r3,im)		M3(0x03,LD_NONE,r3,im,r1)
#define LD1_S_inc(r1,r3,im)		M3(0x04,LD_NONE,r3,im,r1)
#define LD2_S_inc(r1,r3,im)		M3(0x05,LD_NONE,r3,im,r1)
#define LD4_S_inc(r1,r3,im)		M3(0x06,LD_NONE,r3,im,r1)
#define LD8_S_inc(r1,r3,im)		M3(0x07,LD_NONE,r3,im,r1)
#define LD1_A_inc(r1,r3,im)		M3(0x08,LD_NONE,r3,im,r1)
#define LD2_A_inc(r1,r3,im)		M3(0x09,LD_NONE,r3,im,r1)
#define LD4_A_inc(r1,r3,im)		M3(0x0a,LD_NONE,r3,im,r1)
#define LD8_A_inc(r1,r3,im)		M3(0x0b,LD_NONE,r3,im,r1)
#define LD1_SA_inc(r1,r3,im)		M3(0x0c,LD_NONE,r3,im,r1)
#define LD2_SA_inc(r1,r3,im)		M3(0x0d,LD_NONE,r3,im,r1)
#define LD4_SA_inc(r1,r3,im)		M3(0x0e,LD_NONE,r3,im,r1)
#define LD8_SA_inc(r1,r3,im)		M3(0x0f,LD_NONE,r3,im,r1)
#define LD1_BIAS_inc(r1,r3,im)		M3(0x10,LD_NONE,r3,im,r1)
#define LD2_BIAS_inc(r1,r3,im)		M3(0x11,LD_NONE,r3,im,r1)
#define LD4_BIAS_inc(r1,r3,im)		M3(0x12,LD_NONE,r3,im,r1)
#define LD8_BIAS_inc(r1,r3,im)		M3(0x13,LD_NONE,r3,im,r1)
#define LD1_ACQ_inc(r1,r3,im)		M3(0x14,LD_NONE,r3,im,r1)
#define LD2_ACQ_inc(r1,r3,im)		M3(0x15,LD_NONE,r3,im,r1)
#define LD4_ACQ_inc(r1,r3,im)		M3(0x16,LD_NONE,r3,im,r1)
#define LD8_AVQ_inc(r1,r3,im)		M3(0x17,LD_NONE,r3,im,r1)
#define LD8_FILL_inc(r1,r3,im)		M3(0x1b,LD_NONE,r3,im,r1)
#define LD1_C_CLR_inc(r1,r3,im)		M3(0x20,LD_NONE,r3,im,r1)
#define LD2_C_CLR_inc(r1,r3,im)		M3(0x21,LD_NONE,r3,im,r1)
#define LD4_C_CLR_inc(r1,r3,im)		M3(0x22,LD_NONE,r3,im,r1)
#define LD8_C_CLR_inc(r1,r3,im)		M3(0x23,LD_NONE,r3,im,r1)
#define LD1_C_NC_inc(r1,r3,im)		M3(0x24,LD_NONE,r3,im,r1)
#define LD2_C_NC_inc(r1,r3,im)		M3(0x25,LD_NONE,r3,im,r1)
#define LD4_C_NC_inc(r1,r3,im)		M3(0x26,LD_NONE,r3,im,r1)
#define LD8_C_NC_inc(r1,r3,im)		M3(0x27,LD_NONE,r3,im,r1)
#define LD1_C_CLR_ACQ_inc(r1,r3,im)	M3(0x28,LD_NONE,r3,im,r1)
#define LD2_C_CLR_ACQ_inc(r1,r3,im)	M3(0x29,LD_NONE,r3,im,r1)
#define LD4_C_CLR_ACQ_inc(r1,r3,im)	M3(0x2a,LD_NONE,r3,im,r1)
#define LD8_C_CLR_ACQ_inc(r1,r3,im)	M3(0x2b,LD_NONE,r3,im,r1)
#define LDX1(r1,r3,r2)			M2(0x00,LD_NONE,r3,r2,r1)
#define LDX2(r1,r3,r2)			M2(0x01,LD_NONE,r3,r2,r1)
#define LDX4(r1,r3,r2)			M2(0x02,LD_NONE,r3,r2,r1)
#define LDX8(r1,r3,r2)			M2(0x03,LD_NONE,r3,r2,r1)
#define LDX1_S(r1,r3,r2)		M2(0x04,LD_NONE,r3,r2,r1)
#define LDX2_S(r1,r3,r2)		M2(0x05,LD_NONE,r3,r2,r1)
#define LDX4_S(r1,r3,r2)		M2(0x06,LD_NONE,r3,r2,r1)
#define LDX8_S(r1,r3,r2)		M2(0x07,LD_NONE,r3,r2,r1)
#define LDX1_A(r1,r3,r2)		M2(0x08,LD_NONE,r3,r2,r1)
#define LDX2_A(r1,r3,r2)		M2(0x09,LD_NONE,r3,r2,r1)
#define LDX4_A(r1,r3,r2)		M2(0x0a,LD_NONE,r3,r2,r1)
#define LDX8_A(r1,r3,r2)		M2(0x0b,LD_NONE,r3,r2,r1)
#define LDX1_SA(r1,r3,r2)		M2(0x0c,LD_NONE,r3,r2,r1)
#define LDX2_SA(r1,r3,r2)		M2(0x0d,LD_NONE,r3,r2,r1)
#define LDX4_SA(r1,r3,r2)		M2(0x0e,LD_NONE,r3,r2,r1)
#define LDX8_SA(r1,r3,r2)		M2(0x0f,LD_NONE,r3,r2,r1)
#define LDX1_BIAS(r1,r3,r2)		M2(0x10,LD_NONE,r3,r2,r1)
#define LDX2_BIAS(r1,r3,r2)		M2(0x11,LD_NONE,r3,r2,r1)
#define LDX4_BIAS(r1,r3,r2)		M2(0x12,LD_NONE,r3,r2,r1)
#define LDX8_BIAS(r1,r3,r2)		M2(0x13,LD_NONE,r3,r2,r1)
#define LDX1_ACQ(r1,r3,r2)		M2(0x14,LD_NONE,r3,r2,r1)
#define LDX2_ACQ(r1,r3,r2)		M2(0x15,LD_NONE,r3,r2,r1)
#define LDX4_ACQ(r1,r3,r2)		M2(0x16,LD_NONE,r3,r2,r1)
#define LDX8_ACQ(r1,r3,r2)		M2(0x17,LD_NONE,r3,r2,r1)
#define LDX8_FILL(r1,r3,r2)		M2(0x1b,LD_NONE,r3,r2,r1)
#define LDX1_C_CLR(r1,r3,r2)		M2(0x20,LD_NONE,r3,r2,r1)
#define LDX2_C_CLR(r1,r3,r2)		M2(0x21,LD_NONE,r3,r2,r1)
#define LDX4_C_CLR(r1,r3,r2)		M2(0x22,LD_NONE,r3,r2,r1)
#define LDX8_C_CLR(r1,r3,r2)		M2(0x23,LD_NONE,r3,r2,r1)
#define LDX1_C_NC(r1,r3,r2)		M2(0x24,LD_NONE,r3,r2,r1)
#define LDX2_C_NC(r1,r3,r2)		M2(0x25,LD_NONE,r3,r2,r1)
#define LDX4_C_NC(r1,r3,r2)		M2(0x26,LD_NONE,r3,r2,r1)
#define LDX8_C_NC(r1,r3,r2)		M2(0x27,LD_NONE,r3,r2,r1)
#define LDX1_C_CLR_ACQ(r1,r3,r2)	M2(0x28,LD_NONE,r3,r2,r1)
#define LDX2_C_CLR_ACQ(r1,r3,r2)	M2(0x29,LD_NONE,r3,r2,r1)
#define LDX4_C_CLR_ACQ(r1,r3,r2)	M2(0x2a,LD_NONE,r3,r2,r1)
#define LDX8_C_CLR_ACQ(r1,r3,r2)	M2(0x2b,LD_NONE,r3,r2,r1)
/* lfetch */
#define LFETCH_EXCL(r3)			M13(0x2d,LF_NONE,r3,GR_0)
#define LFETCH_FAULT(r3)		M13(0x2e,LF_NONE,r3,GR_0)
#define LFETCH_FAULT_EXCL(r3)		M13(0x2f,LF_NONE,r3,GR_0)
#define LXFETCH(r3,r2)			M14(0x2c,LF_NONE,r3,r2)
#define LXFETCH_EXCL(r3,r2)		M14(0x2d,LF_NONE,r3,r2)
#define LXFETCH_FAULT(r3,r2)		M14(0x2e,LF_NONE,r3,r2)
#define LXFETCH_FAULT_EXCL(r3,r2)	M14(0x2f,LF_NONE,r3,r2)
#define LFETCHI(r3,im)			M15(0x2c,LF_NONE,r3,im)
#define LFETCHI_EXCL(r3,im)		M15(0x2d,LF_NONE,r3,im)
#define LFETCHI_FAULT(r3,im)		M15(0x2e,LF_NONE,r3,im)
#define LFETCHI_FAULT_EXCL(r3,im)	M15(0x2f,LF_NONE,r3,im)
/* loadrs */
#define LOADRS()			M25(0xa)
/* mf */
#define MF()				M24(2,2)
#define MF_A()				M24(2,3)
/* mix */
#define MIX1_R(r1,r2,r3)		I2(0,2,0,2,0,r3,r2,r1)
#define MIX2_R(r1,r2,r3)		I2(0,2,1,2,0,r3,r2,r1)
#define MIX4_R(r1,r2,r3)		I2(1,2,0,2,0,r3,r2,r1)
#define MIX1_L(r1,r2,r3)		I2(0,2,0,2,2,r3,r2,r1)
#define MIX2_L(r1,r2,r3)		I2(0,2,1,2,2,r3,r2,r1)
#define MIX4_L(r1,r2,r3)		I2(1,2,0,2,2,r3,r2,r1)
/* mov - Move Application Register */
#define MOV_I_rn_ar(r1,ar)		I28(ar,r1)
#define MOV_I_ar_rn(ar,r2)		I26(ar,r2)
#define MOV_I_ar_im(ar,im)		I27(ar,im)
#define MOV_M_rn_a(r1,ar)		M31(r1,ar)
#define MOV_M_ar_rn(ar,r2)		M29(ar,r2)
#define MOV_M_ar_im(ar,im)		M30(ar,im)
/* mov - Move Branch Register */
#define MOV_rn_br(r1,b2)		I22(b2,r1)
#define MOV_br_rn_tg(b1,r2,tag)		I21(tag,IH_NONE,0,MWH_NONE,r2,b1)
#define MOV_br_rn(b1,r2)		MOV_br_rn_tg(b1,r2,0)
#define MOV_RET_br_rn_tg(b1,r2,tag)	I21(tag,IH_NONE,1,MWH_NONE,r2,b1)
/* mov - Move Control Register */
#define MOV_rn_cr(cr,r1)		M33(cr,r1)
#define MOV_cr_rr(cr,r2)		M32(cr,r2)
/* mov - Move General Register */
#define MOV(r0,r1)			ADDS(r0,0,r1)
#define MOV_p(r0,r1,_p)			ADDS_p(r0,0,r1,_p)
/* mov - Move Immediate */
#define MOVI(r1,im)			ADDL(r1,im,GR_0)
#define MOVI_p(r1,im,_p)		ADDL_p(r1,im,GR_0,_p)
/* mov - Move Indirect Register */
#define MOV_rn_RR(r1,r3)		M43(0x10,r3,r1)
#define MOV_rn_DBR(r1,r3)		M43(0x11,r3,r1)
#define MOV_rn_IBR(r1,r3)		M43(0x012,r3,r1)
#define MOV_rn_PKR(r1,r3)		M43(0x13,r3,r1)
#define MOV_rn_PMC(r1,r3)		M43(0x14,r3,r1)
#define MOV_rn_PMD(r1,r3)		M43(0x15,r3,r1)
#define MOV_rn_CPUID(r1,r3)		M43(0x17,r3,r1)
#define MOV_RR_rn(r3,r2)		M42(0x00,r3,r2)
#define MOV_DBR_rn(r3,r2)		M42(0x01,r3,r2)
#define MOV_IBR_rn(r3,r2)		M42(0x02,r3,r2)
#define MOV_PKR_rn(r3,r2)		M42(0x03,r3,r2)
#define MOV_PMC_rn(r3,r2)		M42(0x04,r3,r2)
#define MOV_PMD_rn(r3,r2)		M42(0x05,r3,r2)
/* mov - Move Instruction Pointer */
#define MOV_rn_ip(r1)			I25(0x30,r1)
/* mov - Move Predicates */
#define MOV_rn_pr(r1)			I25(0x33,r1)
#define MOV_pr_rn(r2,im)		I23(r2,im)
#define MOVI_pr(im)			I24(im)
/* mov - Move Processor Status Register */
#define MOV_rn_psr(r1)			M36(0x25,r1)
#define MOV_psr_l_rn(r2)		M35(0x2d,r2)
/* mov - Move User Mask */
#define MOV_rn_psr_um(r1)		M36(0x21,r1)
#define MOV_psr_um_rn(r2)		M35(0x29,r2)
/* movl */
#define MOVL(r1,im)			X2(r1,im)
/* mpy4 */
#define MPY4(r1,r2,r3)			I2(1,0,0,3,1,r3,r2,r1)
/* mpyshl4 */
#define MPYSHL4(r1,r2,r3)		I2(1,0,0,3,3,r3,r2,r1)
/* mux */
#define MUX1(r1,r2,mbt)			I3(mbt,r2,r1)
#define MUX2(r1,r2,mht)			I4(mht,r2,r1)
/* nop */
#define NOP_I(im)			I18(im,0)
#define NOP_M(im)			M48(0,im)
#define NOP_B(im)			B9(2,0,im)
#define NOP_X(im)			X5(0,im)
/* or */
#define OR(r1,r2,r3)			A1(3,2,r3,r2,r1)
#define ORI(r1,im,r3)			A3(0xb,2,r3,im,r1)
/* pack */
#define PACK2_USS(r1,r2,r3)		I2(0,2,1,0,0,r3,r2,r1)
#define PACK2_SSS(r1,r2,r3)		I2(0,2,1,0,2,r3,r2,r1)
#define PACK4_SSS(r1,r2,r3)		I2(1,2,0,0,2,r3,r2,r1)
/* padd */
#define PADD1(r1,r2,r3)			A9(0,0,0,0,r3,r2,r1)
#define PADD1_SSS(r1,r2,r3)		A9(0,0,0,1,r3,r2,r1)
#define PADD1_UUU(r1,r2,r3)		A9(0,0,0,2,r3,r2,r1)
#define PADD1_UUS(r1,r2,r3)		A9(0,0,0,3,r3,r2,r1)
#define PADD2(r1,r2,r3)			A9(0,1,0,0,r3,r2,r1)
#define PADD2_SSS(r1,r2,r3)		A9(0,1,0,1,r3,r2,r1)
#define PADD2_UUU(r1,r2,r3)		A9(0,1,0,2,r3,r2,r1)
#define PADD2_UUS(r1,r2,r3)		A9(0,1,0,3,r3,r2,r1)
#define PADD4(r1,r2,r3)			A9(1,0,0,0,r3,r2,r1)
/* pavg */
#define PAVG1(r1,r2,r3)			A9(0,0,2,2,r3,r2,r1)
#define PAVG2(r1,r2,r3)			A9(0,1,2,2,r3,r2,r1)
#define PAVG1_RAZ(r1,r2,r3)		A9(0,0,2,3,r3,r2,r1)
#define PAVG2_RAZ(r1,r2,r3)		A9(0,1,2,3,r3,r2,r1)
/* pavgsub */
#define PAVGSUB1(r1,r2,r3)		A9(0,0,3,2,r3,r2,r1)
#define PAVGSUB2(r1,r2,r3)		A9(0,1,3,2,r3,r2,r1)
/* pcmp */
#define PCMP1_EQ(r1,r2,r3)		A9(0,0,9,0,r3,r2,r1)
#define PCMP2_EQ(r1,r2,r3)		A9(0,1,9,0,r3,r2,r1)
#define PCMP4_EQ(r1,r2,r3)		A9(1,0,9,0,r3,r2,r1)
#define PCMP1_GT(r1,r2,r3)		A9(0,0,9,1,r3,r2,r1)
#define PCMP2_GT(r1,r2,r3)		A9(0,1,9,1,r3,r2,r1)
#define PCMP4_GT(r1,r2,r3)		A9(1,0,9,1,r3,r2,r1)
/* pmax */
#define PMAX1_U(r1,r2,r3)		I2(0,2,0,1,1,r3,r2,r1)
#define PMAX2(r1,r2,r3)			I2(0,2,1,1,3,r3,r2,r1)
/* pmin */
#define PMIN1_U(r1,r2,r3)		I2(0,2,0,0,1,r3,r2,r1)
#define PMIN2(r1,r2,r3)			I2(0,2,1,0,3,r3,r2,r1)
/* pmpy */
#define PMPY2_R(r1,r2,r3)		I2(0,2,1,3,1,r3,r2,r1)
#define PMPY2_L(r1,r2,r3)		I2(0,2,1,3,3,r3,r2,r1)
/* pmpyshr */
#define PMPYSHR2(r1,r2,r3,im)		I1(im,3,r3,r2,r1)
#define PMPYSHR2_U(r1,r2,r3,im)		I1(im,1,r3,r2,r1)
/* popcnt */
#define POPCNT(r1,r3)			I9(2,r3,r1)
/* probe */
#define PROBE_R(r1,r3,r2)		M38(0x38,r3,r2,r1)
#define PROBE_W(r1,r3,r2)		M38(0x39,r3,r2,r1)
#define PROBEI_R(r1,r3,im)		M39(0x18,r3,im,r1)
#define PROBEI_W(r1,r3,im)		M39(0x19,r3,im,r1)
#define PROBE_RW_FAULT(r3,im)		M40(0x31,r3,im)
#define PROBE_R_FAULT(r3,im)		M40(0x32,r3,im)
#define PROBE_W_FAULT(r3,im)		M40(0x33,r3,im)
/* psad */
#define PSAD1(r1,r2,r3)			I2(0,2,0,2,3,r3,r2,r1)
/* pshl */
#define PSHL2(r1,r2,r3)			I7(0,1,r3,r2,r1)
#define PSHL4(r1,r2,r3)			I7(1,0,r3,r2,r1)
#define PSHL2I(r1,r2,im)		I8(0,1,im,r2,r1)
#define PSHL4I(r1,r2,im)		I8(1,0,im,r2,r1)
/* pshladd */
#define PSHLADD2(r1,r2,im,r3)		A10(4,im,r3,r2,r1)
/* pshr */
#define PSHR2(r1,r3,r2)			I5(0,1,2,r3,r2,r1)
#define PSHR2I(r1,r3,im)		I6(0,1,3,r3,im,r1)
#define PSHR2_U(r1,r3,r2)		I5(0,1,0,r3,r2,r1)
#define PSHR2I_U(r1,r3,im)		I6(0,1,1,r3,im,r1)
#define PSHR4(r1,r3,r2)			I5(1,0,2,r3,r2,r1)
#define PSHR4I(r1,r3,im)		I6(1,0,3,r3,im,r1)
#define PSHR4_U(r1,r3,r2)		I5(1,0,0,r3,r2,r1)
#define PSHR4I_U(r1,r3,im)		I6(1,0,1,r3,im,r1)
/* pshradd */
#define PSHRADD2(r1,r2,im,r3)		A10(6,im,r3,r2,r1)
/* psub */
#define PSUB1(r1,r2,r3)			A9(0,0,1,0,r3,r2,r1)
#define PSUB1_SSS(r1,r2,r3)		A9(0,0,1,1,r3,r2,r1)
#define PSUB1_UUU(r1,r2,r3)		A9(0,0,1,2,r3,r2,r1)
#define PSUB1_UUS(r1,r2,r3)		A9(0,0,1,3,r3,r2,r1)
#define PSUB2(r1,r2,r3)			A9(0,1,1,0,r3,r2,r1)
#define PSUB2_SSS(r1,r2,r3)		A9(0,1,1,1,r3,r2,r1)
#define PSUB2_UUU(r1,r2,r3)		A9(0,1,1,2,r3,r2,r1)
#define PSUB2_UUS(r1,r2,r3)		A9(0,1,1,3,r3,r2,r1)
#define PSUB4(r1,r2,r3)			A9(1,0,1,0,r3,r2,r1)
/* ptc.e */
#define PTC_E(r3)			M47(0x34,r3)
/* ptc.g, ptc.ga */
#define PTC_G(r3,r2)			M45(0xa,r3,r2)
#define PTC_GA(r3,r2)			M45(0xb,r3,r2)
/* ptc.l */
#define PTC_L(r3,r2)			M45(0x9,r3,r2)
/* ptr */
#define PTR_D(r3,r2)			M45(0xc,r3,r2)
#define PTR_I(r3,r2)			M45(0xd,r3,r2)
/* rfi */
#define RFI()				B8(0x08)
/* rsm */
#define RSM(im)				M44(7,im)
/* rum */
#define RUM(im)				M44(5,im)
/* shl */
#define SHL(r1,r2,r3)			I7(1,1,r3,r2,r1)
/* shladd */
#define SHLADD(r1,r2,im,r3)		A2(4,im,r3,r2,r1)
/* shladdp4 */
#define SHLADDP4(r1,r2,im,r3)		A2(6,im,r3,r2,r1)
/* shr */
#define SHR(r1,r3,r2)			I5(1,1,2,r3,r2,r1)
#define SHR_U(r1,r3,r2)			I5(1,1,0,r3,r2,r1)
/* shrp */
#define SHRP(r1,r2,r3,im)		I10(im,r3,r2,r1)
/* srlz */
#define SRLZ_I()			M24(3,1)
#define SRLZ_D()			M24(3,0)
/* ssm */
#define SSM(im)				M44(6,im)
/* st */
#define ST1(r3,r2)			M6(0x30,ST_NONE,0,r3,r2)
#define ST2(r3,r2)			M6(0x31,ST_NONE,0,r3,r2)
#define ST4(r3,r2)			M6(0x32,ST_NONE,0,r3,r2)
#define ST8(r3,r2)			M6(0x33,ST_NONE,0,r3,r2)
#define ST1_REL(r3,r2)			M6(0x34,ST_NONE,0,r3,r2)
#define ST2_REL(r3,r2)			M6(0x35,ST_NONE,0,r3,r2)
#define ST4_REL(r3,r2)			M6(0x36,ST_NONE,0,r3,r2)
#define ST8_REL(r3,r2)			M6(0x37,ST_NONE,0,r3,r2)
#define ST8_SPILL(r3,r2)		M6(0x3b,ST_NONE,0,r3,r2)
#define ST16(r3,r2)			M6(0x30,ST_NONE,1,r3,r2)
#define ST16_REL(r3,r2)			M6(0x34,ST_NONE,1,r3,r2)
#define ST1_inc(r3,r2,im)		M5(0x30,ST_NONE,r3,r2,im)
#define ST2_inc(r3,r2,im)		M5(0x31,ST_NONE,r3,r2,im)
#define ST4_inc(r3,r2,im)		M5(0x32,ST_NONE,r3,r2,im)
#define ST8_inc(r3,r2,im)		M5(0x33,ST_NONE,r3,r2,im)
#define ST1_REL_inc(r3,r2,im)		M5(0x34,ST_NONE,r3,r2,im)
#define ST2_REL_inc(r3,r2,im)		M5(0x35,ST_NONE,r3,r2,im)
#define ST4_REL_inc(r3,r2,im)		M5(0x36,ST_NONE,r3,r2,im)
#define ST8_REL_inc(r3,r2,im)		M5(0x37,ST_NONE,r3,r2,im)
#define ST8_SPILL_inc(r3,r2,im)		M5(0x3b,ST_NONE,r3,r2,im)
/* sub */
#define SUB(r1,r2,r3)			A1(1,1,r3,r2,r1)
#define SUB1(r1,r2,r3)			A1(1,0,r3,r2,r1)
#define SUBI(r1,im,r3)			A3(9,1,r3,im,r1)
/* sum */
#define SUM(im)				M44(4,im)
/* sxt */
#define SXT1(r1,r3)			I29(0x14,r3,r1)
#define SXT2(r1,r3)			I29(0x15,r3,r1)
#define SXT4(r1,r3)			I29(0x16,r3,r1)
/* sync */
#define SYNC_I()			M24(3,3)
/* tak */
#define TAK(r1,r3)			M46(0x1f,r3,r1)
/* tbit */
#define TBIT_Z(p1,p2,r3,pos)		I16(0,0,p2,r3,pos,0,p1)
#define TBIT_Z_UNC(p1,p2,r3,pos)	I16(0,0,p2,r3,pos,1,p1)
#define TBIT_Z_AND(p1,p2,r3,pos)	I16(1,0,p2,r3,pos,0,p1)
#define TBIT_NZ_AND(p1,p2,r3,pos)	I16(1,0,p2,r3,pos,1,p1)
#define TBIT_Z_OR(p1,p2,r3,pos)		I16(0,1,p2,r3,pos,0,p1)
#define TBIT_NZ_OR(p1,p2,r3,pos)	I16(0,1,p2,r3,pos,1,p1)
#define TBIT_Z_ANDCM(p1,p2,r3,pos)	I16(1,1,p2,r3,pos,0,p1)
#define TBIT_NZ_ANDCM(p1,p2,r3,pos)	I16(1,1,p2,r3,pos,1,p1)
/* tf */
#define TF_Z(p1,p2,im)			I30(0,0,p2,im,0,p1)
#define TF_Z_UNC(p1,p2,im)		I30(0,0,p2,im,1,p1)
#define TF_Z_AND(p1,p2,im)		I30(1,0,p2,im,0,p1)
#define TF_NZ_AND(p1,p2,im)		I30(1,0,p2,im,1,p1)
#define TF_Z_OR(p1,p2,im)		I30(0,1,p2,im,0,p1)
#define TF_NZ_OR(p1,p2,im)		I30(0,1,p2,im,1,p1)
#define TF_Z_ANDCM(p1,p2,im)		I30(1,1,p2,im,0,p1)
#define TF_NZ_ANDCM(p1,p2,im)		I30(1,1,p2,im,1,p1)
/* thash */
#define THASH(r1,r3)			M46(0x1a,r3,r1)
/* tnat */
#define TNAT_Z(p1,p2,r3)		I17(0,0,p2,r3,0,p1)
#define TNAT_Z_UNC(p1,p2,r3)		I17(0,0,p2,r3,1,p1)
#define TNAT_Z_AND(p1,p2,r3)		I17(1,0,p2,r3,0,p1)
#define TNAT_NZ_AND(p1,p2,r3)		I17(1,0,p2,r3,1,p1)
#define TNAT_Z_OR(p1,p2,r3)		I17(0,1,p2,r3,0,p1)
#define TNAT_NZ_OR(p1,p2,r3)		I17(0,1,p2,r3,1,p1)
#define TNAT_Z_ANDCM(p1,p2,r3)		I17(1,1,p2,r3,0,p1)
#define TNAT_NZ_ANDCM(p1,p2,r3)		I17(1,1,p2,r3,1,p1)
/* tpa */
#define TPA(r1,r3)			M46(0x1e,r3,r1)
/* ttag */
#define TTAG(r1,r3)			M46(0x1b,r3,r1)
/* unpack */
#define UNPACK1_H(r1,r2,r3)		I2(0,2,0,1,0,r3,r2,r1)
#define UNPACK2_H(r1,r2,r3)		I2(0,2,1,1,0,r3,r2,r1)
#define UNPACK4_H(r1,r2,r3)		I2(1,2,0,1,0,r3,r2,r1)
#define UNPACK1_L(r1,r2,r3)		I2(0,2,0,1,2,r3,r2,r1)
#define UNPACK2_L(r1,r2,r3)		I2(0,2,1,1,2,r3,r2,r1)
#define UNPACK4_L(r1,r2,r3)		I2(1,2,0,1,2,r3,r2,r1)
/* vmsw */
#define VMSW_0()			B8(0x18)
#define VMSW_1()			B8(0x19)
/* xchg */
#define XCHG1_ACQ(r1,r3,r2)		M16(0x08,LD_NONE,r3,r2,r1)
#define XCHG2_ACQ(r1,r3,r2)		M16(0x09,LD_NONE,r3,r2,r1)
#define XCHG4_ACQ(r1,r3,r2)		M16(0x0a,LD_NONE,r3,r2,r1)
#define XCHG8_ACQ(r1,r3,r2)		M16(0x0b,LD_NONE,r3,r2,r1)
/* xor */
#define XOR(r1,r2,r3)			A1(3,3,r3,r2,r1)
#define XORI(r1,im,r3)			A3(0xb,3,r3,im,r1)
/* zxt */
#define ZXT1(r1,r3)			I29(0x10,r3,r1)
#define ZXT2(r1,r3)			I29(0x11,r3,r1)
#define ZXT4(r1,r3)			I29(0x12,r3,r1)

#define addr(r0,r1,r2)			ADD(r0,r1,r2)
#define addi(r0,r1,i0)			_addi(_jit,r0,r1,i0)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define addcr(r0, r1, r2)		_addcr(_jit, r0, r1, r2)
static void _addcr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#define addci(r0, r1, i0)		_addci(_jit, r0, r1, i0)
static void _addci(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define addxr(r0, r1, r2)		_addxr(_jit, r0, r1, r2)
static void _addxr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define addxi(r0, r1, i0)		_addxi(_jit, r0, r1, i0)
static void _addxi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#define subr(r0,r1,r2)			SUB(r0,r1,r2)
#define subi(r0,r1,i0)			_subi(_jit,r0,r1,i0)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subcr(r0, r1, r2)		_subcr(_jit, r0, r1, r2)
static void _subcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subci(r0, r1, i0)		_subci(_jit, r0, r1, i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subxr(r0, r1, r2)		_subxr(_jit, r0, r1, r2)
static void _subxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subxi(r0, r1, i0)		_subxi(_jit, r0, r1, i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define mulr(r0,r1,r2)			_mulr(_jit,r0,r1,r2)
static void _mulr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define muli(r0,r1,i0)			_muli(_jit,r0,r1,i0)
static void _muli(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define divr(r0,r1,r2)			_divr(_jit,r0,r1,r2)
static void _divr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define divi(r0,r1,i0)			_divi(_jit,r0,r1,i0)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define divr_u(r0,r1,r2)		_divr_u(_jit,r0,r1,r2)
static void _divr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define divi_u(r0,r1,i0)		_divi_u(_jit,r0,r1,i0)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define remr(r0,r1,r2)			_remr(_jit,r0,r1,r2)
static void _remr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define remi(r0,r1,i0)			_remi(_jit,r0,r1,i0)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define remr_u(r0,r1,r2)		_remr_u(_jit,r0,r1,r2)
static void _remr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define remi_u(r0,r1,i0)		_remi_u(_jit,r0,r1,i0)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define mulh(r0,r1,r2,sign)		_mulh(_jit,r0,r1,r2,sign)
static void _mulh(jit_state_t*,jit_bool_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define qmulr(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,1)
#define qmulr_u(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,0)
#define iqmulr(r0,r1,r2,r3,sign)	_iqmulr(_jit,r0,r1,r2,r3,sign)
static void _iqmulr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#define qmuli(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,1)
#define qmuli_u(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,0)
#define iqmuli(r0,r1,r2,i0,sign)	_iqmuli(_jit,r0,r1,r2,i0,sign)
static void _iqmuli(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#define qdivr(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,1)
#define qdivr_u(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,0)
#define iqdivr(r0,r1,r2,r3,sign)	_iqdivr(_jit,r0,r1,r2,r3,sign)
static void _iqdivr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#define qdivi(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,1)
#define qdivi_u(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,0)
#define iqdivi(r0,r1,r2,i0,sign)	_iqdivi(_jit,r0,r1,r2,i0,sign)
static void _iqdivi(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#define andr(r0,r1,r2)			AND(r0,r1,r2)
#define andi(r0,r1,i0)			_andi(_jit,r0,r1,i0)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define orr(r0,r1,r2)			OR(r0,r1,r2)
#define ori(r0,r1,i0)			_ori(_jit,r0,r1,i0)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define xorr(r0,r1,r2)			XOR(r0,r1,r2)
#define xori(r0,r1,i0)			_xori(_jit,r0,r1,i0)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define lshr(r0,r1,r2)			SHL(r0,r1,r2)
#define lshi(r0,r1,i0)			_lshi(_jit,r0,r1,i0)
static void _lshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define rshr(r0,r1,r2)			SHR(r0,r1,r2)
#define rshi(r0,r1,i0)			_rshi(_jit,r0,r1,i0)
static void _rshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define rshr_u(r0,r1,r2)		SHR_U(r0,r1,r2)
#define rshi_u(r0,r1,i0)		_rshi_u(_jit,r0,r1,i0)
static void _rshi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ltr(r0,r1,r2)			_ltr(_jit,r0,r1,r2)
static void _ltr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lti(r0,r1,i0)			_lti(_jit,r0,r1,i0)
static void _lti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ltr_u(r0,r1,r2)			_ltr_u(_jit,r0,r1,r2)
static void _ltr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lti_u(r0,r1,i0)			_lti_u(_jit,r0,r1,i0)
static void _lti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ler(r0,r1,r2)			_ler(_jit,r0,r1,r2)
static void _ler(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lei(r0,r1,i0)			_lei(_jit,r0,r1,i0)
static void _lei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ler_u(r0,r1,r2)			_ler_u(_jit,r0,r1,r2)
static void _ler_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lei_u(r0,r1,i0)			_lei_u(_jit,r0,r1,i0)
static void _lei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define eqr(r0,r1,r2)			_eqr(_jit,r0,r1,r2)
static void _eqr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define eqi(r0,r1,i0)			_eqi(_jit,r0,r1,i0)
static void _eqi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ger(r0,r1,r2)			_ger(_jit,r0,r1,r2)
static void _ger(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gei(r0,r1,i0)			_gei(_jit,r0,r1,i0)
static void _gei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ger_u(r0,r1,r2)			_ger_u(_jit,r0,r1,r2)
static void _ger_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gei_u(r0,r1,i0)			_gei_u(_jit,r0,r1,i0)
static void _gei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define gtr(r0,r1,r2)			_gtr(_jit,r0,r1,r2)
static void _gtr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gti(r0,r1,i0)			_gti(_jit,r0,r1,i0)
static void _gti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define gtr_u(r0,r1,r2)			_gtr_u(_jit,r0,r1,r2)
static void _gtr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gti_u(r0,r1,i0)			_gti_u(_jit,r0,r1,i0)
static void _gti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ner(r0,r1,r2)			_ner(_jit,r0,r1,r2)
static void _ner(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define nei(r0,r1,i0)			_nei(_jit,r0,r1,i0)
static void _nei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define negr(r0,r1)			subr(r0,0,r1)
#define comr(r0,r1)			ANDCMI(r0,-1,r1)
#define movr(r0,r1)			_movr(_jit,r0,r1)
static void _movr(jit_state_t*,jit_int32_t,jit_int32_t);
#define movi(r0,i0)			_movi(_jit,r0,i0)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#define movi_p(r0,i0)			_movi_p(_jit,r0,i0)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#if __BYTE_ORDER == __LITTLE_ENDIAN
#  define htonr_us(r0,r1)		_htonr_us(_jit,r0,r1)
static void _htonr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define htonr_ui(r0,r1)		_htonr_ui(_jit,r0,r1)
static void _htonr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  define htonr_ul(r0,r1)		MUX1(r0,r1,MUX_REV)
#else
#  define htonr_us(r0,r1)		extr_us(r0,r1)
#  define htonr_ui(r0,r1)		extr_ui(r0,r1)
#  define htonr_ul(r0,r1)		movr(r0,r1)
#endif
#define extr_c(r0,r1)			SXT1(r0,r1)
#define extr_uc(r0,r1)			ZXT1(r0,r1)
#define extr_s(r0,r1)			SXT2(r0,r1)
#define extr_us(r0,r1)			ZXT2(r0,r1)
#define extr_i(r0,r1)			SXT4(r0,r1)
#define extr_ui(r0,r1)			ZXT4(r0,r1)
#define bltr(i0,r0,r1)			_bltr(_jit,i0,r0,r1)
static jit_word_t _bltr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define blti(i0,r0,i1)			_blti(_jit,i0,r0,i1)
static jit_word_t _blti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bltr_u(i0,r0,r1)		_bltr_u(_jit,i0,r0,r1)
static jit_word_t _bltr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define blti_u(i0,r0,i1)		_blti_u(_jit,i0,r0,i1)
static jit_word_t _blti_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bler(i0,r0,r1)			_bler(_jit,i0,r0,r1)
static jit_word_t _bler(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define blei(i0,r0,i1)			_blei(_jit,i0,r0,i1)
static jit_word_t _blei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bler_u(i0,r0,r1)		_bler_u(_jit,i0,r0,r1)
static jit_word_t _bler_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define blei_u(i0,r0,i1)		_blei_u(_jit,i0,r0,i1)
static jit_word_t _blei_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define beqr(i0,r0,r1)			_beqr(_jit,i0,r0,r1)
static jit_word_t _beqr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define beqi(i0,r0,i1)			_beqi(_jit,i0,r0,i1)
static jit_word_t _beqi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bger(i0,r0,r1)			_bger(_jit,i0,r0,r1)
static jit_word_t _bger(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bgei(i0,r0,i1)			_bgei(_jit,i0,r0,i1)
static jit_word_t _bgei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bger_u(i0,r0,r1)		_bger_u(_jit,i0,r0,r1)
static jit_word_t _bger_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bgei_u(i0,r0,i1)		_bgei_u(_jit,i0,r0,i1)
static jit_word_t _bgei_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bgtr(i0,r0,r1)			_bgtr(_jit,i0,r0,r1)
static jit_word_t _bgtr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bgti(i0,r0,i1)			_bgti(_jit,i0,r0,i1)
static jit_word_t _bgti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bgtr_u(i0,r0,r1)		_bgtr_u(_jit,i0,r0,r1)
static jit_word_t _bgtr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bgti_u(i0,r0,i1)		_bgti_u(_jit,i0,r0,i1)
static jit_word_t _bgti_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bner(i0,r0,r1)			_bner(_jit,i0,r0,r1)
static jit_word_t _bner(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bnei(i0,r0,i1)			_bnei(_jit,i0,r0,i1)
static jit_word_t _bnei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bmsr(i0,r0,r1)			_bmsr(_jit,i0,r0,r1)
static jit_word_t _bmsr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bmsi(i0,r0,i1)			_bmsi(_jit,i0,r0,i1)
static jit_word_t _bmsi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bmcr(i0,r0,r1)			_bmcr(_jit,i0,r0,r1)
static jit_word_t _bmcr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bmci(i0,r0,i1)			_bmci(_jit,i0,r0,i1)
static jit_word_t _bmci(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define baddr(i0,r0,r1,cc)		_baddr(_jit,i0,r0,r1,cc)
static jit_word_t _baddr(jit_state_t*,jit_word_t,
			 jit_int32_t,jit_int32_t,jit_bool_t);
#define baddi(i0,r0,i1,cc)		_baddi(_jit,i0,r0,i1,cc)
static jit_word_t _baddi(jit_state_t*,jit_word_t,
			 jit_int32_t,jit_word_t,jit_bool_t);
#define baddr_u(i0,r0,r1,cc)		_baddr_u(_jit,i0,r0,r1,cc)
static jit_word_t _baddr_u(jit_state_t*,jit_word_t,
			   jit_int32_t,jit_int32_t,jit_bool_t);
#define baddi_u(i0,r0,i1,cc)		_baddi_u(_jit,i0,r0,i1,cc)
static jit_word_t _baddi_u(jit_state_t*,jit_word_t,
			   jit_int32_t,jit_word_t,jit_bool_t);
#define bsubr(i0,r0,r1,cc)		_bsubr(_jit,i0,r0,r1,cc)
static jit_word_t _bsubr(jit_state_t*,jit_word_t,
			 jit_int32_t,jit_int32_t,jit_bool_t);
#define bsubi(i0,r0,i1,cc)		_bsubi(_jit,i0,r0,i1,cc)
static jit_word_t _bsubi(jit_state_t*,jit_word_t,
			 jit_int32_t,jit_word_t,jit_bool_t);
#define bsubr_u(i0,r0,r1,cc)		_bsubr_u(_jit,i0,r0,r1,cc)
static jit_word_t _bsubr_u(jit_state_t*,jit_word_t,
			   jit_int32_t,jit_int32_t,jit_bool_t);
#define bsubi_u(i0,r0,i1,cc)		_bsubi_u(_jit,i0,r0,i1,cc)
static jit_word_t _bsubi_u(jit_state_t*,jit_word_t,
			   jit_int32_t,jit_word_t,jit_bool_t);
#define boaddr(i0,r0,r1)		baddr(i0,r0,r1,1)
#define boaddi(i0,r0,i1)		baddi(i0,r0,i1,1)
#define boaddr_u(i0,r0,r1)		baddr_u(i0,r0,r1,1)
#define boaddi_u(i0,r0,i1)		baddi_u(i0,r0,i1,1)
#define bxaddr(i0,r0,r1)		baddr(i0,r0,r1,0)
#define bxaddi(i0,r0,i1)		baddi(i0,r0,i1,0)
#define bxaddr_u(i0,r0,r1)		baddr_u(i0,r0,r1,0)
#define bxaddi_u(i0,r0,i1)		baddi_u(i0,r0,i1,0)
#define bosubr(i0,r0,r1)		bsubr(i0,r0,r1,1)
#define bosubi(i0,r0,i1)		bsubi(i0,r0,i1,1)
#define bosubr_u(i0,r0,r1)		bsubr_u(i0,r0,r1,1)
#define bosubi_u(i0,r0,i1)		bsubi_u(i0,r0,i1,1)
#define bxsubr(i0,r0,r1)		bsubr(i0,r0,r1,0)
#define bxsubi(i0,r0,i1)		bsubi(i0,r0,i1,0)
#define bxsubr_u(i0,r0,r1)		bsubr_u(i0,r0,r1,0)
#define bxsubi_u(i0,r0,i1)		bsubi_u(i0,r0,i1,0)
#define ldr_c(r0,r1)			_ldr_c(_jit,r0,r1)
static void _ldr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#define ldi_c(r0,i0)			_ldi_c(_jit,r0,i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#define ldr_uc(r0,r1)			LD1(r0,r1)
#define ldi_uc(r0,i0)			_ldi_uc(_jit,r0,i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#define ldr_s(r0,r1)			_ldr_s(_jit,r0,r1)
static void _ldr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#define ldi_s(r0,i0)			_ldi_s(_jit,r0,i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#define ldr_us(r0,r1)			LD2(r0,r1)
#define ldi_us(r0,i0)			_ldi_us(_jit,r0,i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#define ldr_i(r0,r1)			_ldr_i(_jit,r0,r1)
static void _ldr_i(jit_state_t*,jit_int32_t,jit_int32_t);
#define ldi_i(r0,i0)			_ldi_i(_jit,r0,i0)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#define ldr_ui(r0,r1)			LD4(r0,r1)
#define ldi_ui(r0,i0)			_ldi_ui(_jit,r0,i0)
static void _ldi_ui(jit_state_t*,jit_int32_t,jit_word_t);
#define ldr_l(r0,r1)			LD8(r0,r1)
#define ldi_l(r0,i0)			_ldi_l(_jit,r0,i0)
static void _ldi_l(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_c(r0,r1,r2)		_ldxr_c(_jit,r0,r1,r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_c(r0,r1,i0)		_ldxi_c(_jit,r0,r1,i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldxr_uc(r0,r1,r2)		_ldxr_uc(_jit,r0,r1,r2)
static void _ldxr_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_uc(r0,r1,i0)		_ldxi_uc(_jit,r0,r1,i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldxr_s(r0,r1,r2)		_ldxr_s(_jit,r0,r1,r2)
static void _ldxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_s(r0,r1,i0)		_ldxi_s(_jit,r0,r1,i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldxr_us(r0,r1,r2)		_ldxr_us(_jit,r0,r1,r2)
static void _ldxr_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_us(r0,r1,i0)		_ldxi_us(_jit,r0,r1,i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldxr_i(r0,r1,r2)		_ldxr_i(_jit,r0,r1,r2)
static void _ldxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_i(r0,r1,i0)		_ldxi_i(_jit,r0,r1,i0)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldxr_ui(r0,r1,r2)		_ldxr_ui(_jit,r0,r1,r2)
static void _ldxr_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_ui(r0,r1,i0)		_ldxi_ui(_jit,r0,r1,i0)
static void _ldxi_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldxr_l(r0,r1,r2)		_ldxr_l(_jit,r0,r1,r2)
static void _ldxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_l(r0,r1,i0)		_ldxi_l(_jit,r0,r1,i0)
static void _ldxi_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define str_c(r0,r1)			ST1(r0,r1)
#define sti_c(i0,r0)			_sti_c(_jit,i0,r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#define str_s(r0,r1)			ST2(r0,r1)
#define sti_s(i0,r0)			_sti_s(_jit,i0,r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#define str_i(r0,r1)			ST4(r0,r1)
#define sti_i(i0,r0)			_sti_i(_jit,i0,r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#define str_l(r0,r1)			ST8(r0,r1)
#define sti_l(i0,r0)			_sti_l(_jit,i0,r0)
static void _sti_l(jit_state_t*,jit_word_t,jit_int32_t);
#define stxr_c(r0,r1,r2)		_stxr_c(_jit,r0,r1,r2)
static void _stxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_c(i0,r0,r1)		_stxi_c(_jit,i0,r0,r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define stxr_s(r0,r1,r2)		_stxr_s(_jit,r0,r1,r2)
static void _stxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_s(i0,r0,r1)		_stxi_s(_jit,i0,r0,r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define stxr_i(r0,r1,r2)		_stxr_i(_jit,r0,r1,r2)
static void _stxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_i(i0,r0,r1)		_stxi_i(_jit,i0,r0,r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define stxr_l(r0,r1,r2)		_stxr_l(_jit,r0,r1,r2)
static void _stxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_l(i0,r0,r1)		_stxi_l(_jit,i0,r0,r1)
static void _stxi_l(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define jmpr(r0)			_jmpr(_jit,r0)
static void _jmpr(jit_state_t*,jit_int32_t);
#define jmpi(i0)			_jmpi(_jit,i0)
static void _jmpi(jit_state_t*,jit_word_t);
#define jmpi_p(i0)			_jmpi_p(_jit,i0)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t);
#define callr(r0)			_callr(_jit,r0)
static void _callr(jit_state_t*,jit_int32_t);
#define calli(i0)			_calli(_jit,i0)
static void _calli(jit_state_t*,jit_word_t);
#define calli_p(i0)			_calli_p(_jit,i0)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#define prolog(node)			_prolog(_jit,node)
static void _prolog(jit_state_t*,jit_node_t*);
#define epilog(node)			_epilog(_jit,node)
static void _epilog(jit_state_t*,jit_node_t*);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#define patch_at(node,instr,label)	_patch_at(_jit,node,instr,label)
static void _patch_at(jit_state_t*,jit_code_t,jit_word_t,jit_word_t);
#endif

#if CODE
#if __BYTE_ORDER == __BIG_ENDIAN
static jit_word_t
byte_swap_if_big_endian(jit_word_t w)
{
    union {
	char	c[8];
	long	w;
    } l, h;
    l.w = w;
    h.c[0] = l.c[7];
    h.c[1] = l.c[6];
    h.c[2] = l.c[5];
    h.c[3] = l.c[4];
    h.c[4] = l.c[3];
    h.c[5] = l.c[2];
    h.c[6] = l.c[1];
    h.c[7] = l.c[0];
    return (h.w);
}
#else
#define byte_swap_if_big_endian(w)		(w)
#endif

static void
_out(jit_state_t *_jit, int n, int tm,
     jit_word_t s0, jit_word_t s1, jit_word_t s2)
{
    int			 i;
    jit_word_t		 l, h, *w;
    set_bundle(_jit->pc.ul, l, h, tm, s0, s1, s2);
    _jit->pc.ul += 2;
    w = (jit_word_t *)_jitc->inst;
    for (i = n; i < _jitc->ioff; i++)
	w[i - n] = w[i];
    _jitc->ioff -= n;
}

#define nop_m		0x0008000000L
#define nop_i		0x0008000000L
#define nop_b		0x4000000000L
static void
_stop(jit_state_t *_jit)
{
    /* Clear set of live registers */
    jit_regset_set_ui(&_jitc->regs, 0);
    _jitc->pred = 0;
    /* Flag a stop is required */
    if (_jitc->ioff)
	_jitc->inst[_jitc->ioff - 1].t |= INST_STOP;
    else
	inst(nop_m, INST_Ms);
}

static void
_sync(jit_state_t *_jit)
{
    /* Taken branches are supposed to not need a stop, so, it
     * should not be required to stop if no registers live in
     * sequential code */
    if (jit_regset_cmp_ui(&_jitc->regs, 0) != 0 || _jitc->pred)
	stop();
    do
	flush();
    while (_jitc->ioff);
}

#define A_0		INST_A
#define As0		INST_As
#define I_0		INST_I
#define Is0		INST_Is
#define M_0		INST_M
#define Ms0		INST_Ms
#define F_0		INST_F
#define Fs0		INST_Fs
#define B_0		INST_B
#define Bs0		INST_Bs
#define L_0		INST_L
#define Ls0		INST_Ls
#define X_0		INST_X
#define Xs0		INST_Xs
#define A_1		(INST_A<<4)
#define As1		(INST_As<<4)
#define I_1		(INST_I<<4)
#define Is1		(INST_Is<<4)
#define M_1		(INST_M<<4)
#define Ms1		(INST_Ms<<4)
#define F_1		(INST_F<<4)
#define Fs1		(INST_Fs<<4)
#define B_1		(INST_B<<4)
#define Bs1		(INST_Bs<<4)
#define L_1		(INST_L<<4)
#define Ls1		(INST_Ls<<4)
#define X_1		(INST_X<<4)
#define Xs1		(INST_Xs<<4)
#define A_2		(INST_A<<8)
#define As2		(INST_As<<8)
#define I_2		(INST_I<<8)
#define Is2		(INST_Is<<8)
#define M_2		(INST_M<<8)
#define Ms2		(INST_Ms<<8)
#define F_2		(INST_F<<8)
#define Fs2		(INST_Fs<<8)
#define B_2		(INST_B<<8)
#define Bs2		(INST_Bs<<8)
#define L_2		(INST_L<<8)
#define Ls2		(INST_Ls<<8)
#define X_2		(INST_X<<8)
#define Xs2		(INST_Xs<<8)

#define I_		I_0
#define I_I_		I_0|I_1
#define I_Is		I_0|Is1
#define I_B_		I_0|B_1
#define I_Bs		I_0|Bs1
#define Is		Is0
#define IsI_		Is0|I_1
#define IsIs		Is0|Is1
#define M_		M_0
#define M_I_		M_0|I_1
#define M_Is		M_0|Is1
#define M_M_		M_0|M_1
#define M_Ms		M_0|Ms1
#define M_F_		M_0|F_1
#define M_Fs		M_0|Fs1
#define M_B_		M_0|B_1
#define M_Bs		M_0|Bs1
#define M_I_I_		M_0|I_1|I_2
#define M_I_Is		M_0|I_1|Is2
#define M_I_B_		M_0|I_1|B_2
#define M_I_Bs		M_0|I_1|Bs2
#define M_IsI_		M_0|Is1|I_2
#define M_IsIs		M_0|Is1|Is2
#define M_M_I_		M_0|M_1|I_2
#define M_M_Is		M_0|M_1|Is2
#define M_M_F_		M_0|M_1|F_2
#define M_M_Fs		M_0|M_1|Fs2
#define M_M_B_		M_0|M_1|B_2
#define M_M_Bs		M_0|M_1|Bs2
#define M_F_I_		M_0|F_1|I_2
#define M_F_Is		M_0|F_1|Is2
#define M_F_B_		M_0|F_1|B_2
#define M_F_Bs		M_0|F_1|Bs2
#define M_B_B_		M_0|B_1|B_2
#define M_B_Bs		M_0|B_1|Bs2
#define M_L_X_		M_0|L_1|X_2
#define M_L_Xs		M_0|L_1|Xs2
#define Ms		Ms0
#define MsI_		Ms0|I_1
#define MsIs		Ms0|Is1
#define MsM_		Ms0|M_1
#define MsMs		Ms0|Ms1
#define MsM_I_		Ms0|M_1|I_2
#define MsM_Is		Ms0|M_1|Is2
#define F_		F_0
#define F_I_		F_0|I_1
#define F_Is		F_0|Is1
#define F_B_		F_0|B_1
#define F_Bs		F_0|Bs1
#define Fs		Fs0
#define B_		B_0
#define B_B_		B_0|B_1
#define B_Bs		B_0|Bs1
#define B_B_B_		B_0|B_1|B_2
#define B_B_Bs		B_0|B_1|Bs2
#define Bs		Bs0
#define L_X_		L_0|X_1
#define L_Xs		L_0|Xs1

static jit_word_t
templat(jit_word_t cc)
{
    switch (cc) {
	case I_:
	case I_I_:	case I_Is:
	case I_B_:	case I_Bs:
	case Is:
	case IsI_:	case IsIs:
	case M_:
	case M_I_:	case M_Is:
	case M_M_:	case M_Ms:
	case M_F_:	case M_Fs:
	case M_B_:	case M_Bs:
	case M_I_I_:	case M_I_Is:
	case M_I_B_:	case M_I_Bs:
	case M_IsI_:	case M_IsIs:
	case M_M_I_:	case M_M_Is:
	case M_M_F_:	case M_M_Fs:
	case M_M_B_:	case M_M_Bs:
	case M_F_I_:	case M_F_Is:
	case M_F_B_:	case M_F_Bs:
	case M_B_B_:	case M_B_Bs:
	case M_L_X_:	case M_L_Xs:
	case Ms:
	case MsI_:	case MsIs:
	case MsM_:	case MsMs:
	case MsM_I_:	case MsM_Is:
	case F_:
	case F_I_:	case F_Is:
	case F_B_:	case F_Bs:
	case Fs:
	case B_:
	case B_B_:	case B_Bs:
	case B_B_B_:	case B_B_Bs:
	case Bs:
	case L_X_:	case L_Xs:
	    return (cc);
	default:
	    return (0);
    }
}

/* match* functions recurse attempting to find a template for A-
 * instructions, that may be executed in M- or I- unit.
 * It also uses an heuristic of trying first M- for slot 0 and 2,
 * and I- for slot 1, but tries all possible matches.
 */
static jit_word_t
match_2(jit_word_t cc)
{
    jit_word_t		t;
    if ((t = templat(cc | M_2)))
	return (t);
    if ((t = templat(cc | I_2)))
	return (t);
    return (0);
}

static jit_word_t
matchs2(jit_word_t cc)
{
    jit_word_t		t;
    if ((t = templat(cc | Ms2)))
	return (t);
    if ((t = templat(cc | Is2)))
	return (t);
    return (0);
}

static jit_word_t
match2(jit_word_t cc)
{
    if ((cc & 0xf00) == A_2)
	return (match_2(cc & ~0xf00));
    if ((cc & 0xf00) == As2)
	return (matchs2(cc & ~0xf00));
    return (0);
}

static jit_word_t
match_1(jit_word_t cc)
{
    jit_word_t		t;
    if ((t = templat(cc | I_1)))
	return (t);
    if ((t = templat(cc | M_1)))
	return (t);
    if ((t = match2(cc | I_1)))
	return (t);
    if ((t = match2(cc | M_1)))
	return (t);
    return (0);
}

static jit_word_t
matchs1(jit_word_t cc)
{
    jit_word_t		t;
    if ((t = templat(cc | Is1)))
	return (t);
    if ((t = templat(cc | Ms1)))
	return (t);
    if ((t = match2(cc | Is1)))
	return (t);
    if ((t = match2(cc | Ms1)))
	return (t);
    return (0);
}

static jit_word_t
match1(jit_word_t cc)
{
    if ((cc & 0x0f0) == A_1)
	return (match_1(cc & ~0x0f0));
    if ((cc & 0x0f0) == As1)
	return (matchs1(cc & ~0x0f0));
    return (0);
}

static jit_word_t
match_0(jit_word_t cc)
{
    jit_word_t		t;
    if ((t = templat(cc | M_0)))
	return (t);
    if ((t = templat(cc | I_0)))
	return (t);
    if ((t = match1(cc | M_0)))
	return (t);
    if ((t = match1(cc | I_0)))
	return (t);
    return (0);
}

static jit_word_t
matchs0(jit_word_t cc)
{
    jit_word_t		t;
    if ((t = templat(cc | Ms0)))
	return (t);
    if ((t = templat(cc | Is0)))
	return (t);
    if ((t = match1(cc | Ms0)))
	return (t);
    if ((t = match1(cc | Is0)))
	return (t);
    return (0);
}

static jit_word_t
match0(jit_word_t cc)
{
    if ((cc & 0x00f) == A_0)
	return (match_0(cc & ~0x00f));
    if ((cc & 0x00f) == As0)
	return (matchs0(cc & ~0x00f));
    return (0);
}

static void
_flush(jit_state_t *_jit)
{
    int			n, soff;
    jit_word_t		t, cc, tm, s0, s1, s2;

    if (!_jitc->ioff)
	return;
    for (cc = 0, n = soff = 0; n < _jitc->ioff; n++, soff += 4)
	cc |= (jit_uword_t)(_jitc->inst[n].t) << soff;

    soff = 0xf00;
    while (soff) {
	/* Try to find a template, or reduce down
	 * to one instruction if no template match */
	if ((t = templat(cc))) {
	    cc = t;
	    break;
	}
	/* A- instructions may be execute in M- or I- unit */
	if ((t = match0(cc))) {
	    cc = t;
	    break;
	}
	cc &= ~soff;
	soff >>= 4;
    }
    assert(soff);

    /* Prefer tail nop if need to add some nop, so that patching is easier */
#define ii(n)		_jitc->inst[n].i
    switch (cc) {
	case I_:
	    n = 1;		tm = TM_M_I_I_;
	    s0 = nop_m;		s1 = ii(0);		s2 = nop_i;
	    break;
	case I_I_:
	    n = 2;		tm = TM_M_I_I_;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case I_Is:
	    n = 2;		tm = TM_M_I_Is;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case I_B_:
	    n = 2;		tm = TM_M_I_B_;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case I_Bs:
	    n = 2;		tm = TM_M_I_Bs;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case Is:
	    n = 1;		tm = TM_M_IsI_;
	    s0 = nop_m;		s1 = ii(0);		s2 = nop_i;
	    break;
	case IsI_:
	    n = 2;		tm = TM_M_IsI_;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case IsIs:
	    n = 2;		tm = TM_M_IsIs;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case M_:
	    n = 1;		tm = TM_M_I_I_;
	    s0 = ii(0);		s1 = nop_i;		s2 = nop_i;
	    break;
	case M_I_:
	    n = 2;		tm = TM_M_I_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case M_Is:
	    n = 2;		tm = TM_M_IsI_;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case M_M_:
	    n = 2;		tm = TM_M_M_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case M_Ms:
	    n = 2;		tm = TM_M_M_Is;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case M_F_:
	    n = 2;		tm = TM_M_F_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case M_Fs:
	    n = 2;		tm = TM_M_F_Is;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case M_B_:
	    n = 2;		tm = TM_M_B_B_;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_b;
	    break;
	case M_Bs:
	    n = 2;		tm = TM_M_B_Bs;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_b;
	    break;
	case M_I_I_:
	    n = 3;		tm = TM_M_I_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_I_Is:
	    n = 3;		tm = TM_M_I_Is;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_I_B_:
	    n = 3;		tm = TM_M_I_B_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_I_Bs:
	    n = 3;		tm = TM_M_I_Bs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_IsI_:
	    n = 3;		tm = TM_M_IsI_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_IsIs:
	    n = 3;		tm = TM_M_IsIs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_M_I_:
	    n = 3;		tm = TM_M_M_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_M_Is:
	    n = 3;		tm = TM_M_M_Is;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_M_F_:
	    n = 3;		tm = TM_M_M_F_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_M_Fs:
	    n = 3;		tm = TM_M_M_Fs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_M_B_:
	    n = 3;		tm = TM_M_M_B_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_M_Bs:
	    n = 3;		tm = TM_M_M_Bs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_F_I_:
	    n = 3;		tm = TM_M_F_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_F_Is:
	    n = 3;		tm = TM_M_F_Is;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_F_B_:
	    n = 3;		tm = TM_M_F_B_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_F_Bs:
	    n = 3;		tm = TM_M_F_Bs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_B_B_:
	    n = 3;		tm = TM_M_B_B_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_B_Bs:
	    n = 3;		tm = TM_M_B_Bs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_L_X_:
	    n = 3;		tm = TM_M_L_X_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case M_L_Xs:
	    n = 3;		tm = TM_M_L_Xs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case Ms:
	    n = 1;		tm = TM_MsM_I_;
	    s0 = ii(0);		s1 = nop_m;		s2 = nop_i;
	    break;
	case MsI_:
	    n = 2;		tm = TM_MsM_I_;
	    s0 = ii(0);		s1 = nop_m;		s2 = ii(1);
	    break;
	case MsIs:
	    n = 2;		tm = TM_MsM_Is;
	    s0 = ii(0);		s1 = nop_m;		s2 = ii(1);
	    break;
	case MsM_:
	    n = 2;		tm = TM_MsM_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case MsMs:
	    n = 2;		tm = TM_MsM_Is;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_i;
	    break;
	case MsM_I_:
	    n = 3;		tm = TM_MsM_I_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case MsM_Is:
	    n = 3;		tm = TM_MsM_Is;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case F_:
	    n = 1;		tm = TM_M_F_I_;
	    s0 = nop_m;		s1 = ii(0);		s2 = nop_i;
	    break;
	case F_I_:
	    n = 2;		tm = TM_M_F_I_;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case F_Is:
	    n = 2;		tm = TM_M_F_Is;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case F_B_:
	    n = 2;		tm = TM_M_F_B_;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case F_Bs:
	    n = 2;		tm = TM_M_F_Bs;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case Fs:
	    n = 1;		tm = TM_M_F_Is;
	    s0 = nop_m;		s1 = ii(0);		s2 = nop_i;
	    break;
	case B_:
	    n = 1;		tm = TM_B_B_B_;
	    s0 = ii(0);		s1 = nop_b;		s2 = nop_b;
	    break;
	case B_B_:
	    n = 2;		tm = TM_B_B_B_;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_b;
	    break;
	case B_Bs:
	    n = 2;		tm = TM_B_B_Bs;
	    s0 = ii(0);		s1 = ii(1);		s2 = nop_b;
	    break;
	case B_B_B_:
	    n = 3;		tm = TM_B_B_B_;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case B_B_Bs:
	    n = 3;		tm = TM_B_B_Bs;
	    s0 = ii(0);		s1 = ii(1);		s2 = ii(2);
	    break;
	case Bs:
	    n = 1;		tm = TM_B_B_Bs;
	    s0 = ii(0);		s1 = nop_b;		s2 = nop_b;
	    break;
	case L_X_:
	    n = 2;		tm = TM_M_L_X_;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	case L_Xs:
	    n = 2;		tm = TM_M_L_Xs;
	    s0 = nop_m;		s1 = ii(0);		s2 = ii(1);
	    break;
	default:
	    abort();
    }
    out(n, tm, s0, s1, s2);
}

static void
_inst(jit_state_t *_jit, jit_word_t i, jit_uint8_t t)
{
    if (_jitc->ioff > 2)
	flush();
    assert(!(i & 0x11111e0000000000L));
    _jitc->inst[_jitc->ioff].i = i;
    _jitc->inst[_jitc->ioff].t = t;
    ++_jitc->ioff;
}

static void
_A1(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x4, jit_word_t x2, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x4 &  ~0xfL));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((8L<<37)|(x4<<29)|(x2<<27)|(r3<<20)|(r2<<13)|(r1<<6)|_p, INST_A);
    SETREG(r1);
}

static void
_A3(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x4, jit_word_t x2, jit_word_t r3, jit_word_t im, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x4 &  ~0xfL));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(im >= -128 && im <= 127);
    assert(!(r1 & ~0x7f));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((8L<<37)|(((im>>7)&1L)<<36)|(x4<<29)|(x2<<27)|
	 (r3<<20)|((im&0x7fL)<<13)|(r1<<6)|_p, INST_A);
    SETREG(r1);
}

static void
_A4(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x2, jit_word_t r3, jit_word_t im, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(im >= -8192 && im <= 8191);
    assert(!(r1  & ~0x7f));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((8L<<37)|(((im>>13)&1L)<<36)|(x2<<34)|(((im>>7)&0x3fL)<<27)|
	 (r3<<20)|((im&0x7fL)<<13)|(r1<<6)|_p, INST_A);
    SETREG(r1);
}

static void
_A5(jit_state_t *_jit, jit_word_t _p,
    jit_word_t r3, jit_word_t im, jit_word_t r1)
{
    jit_word_t		s, i5, i9, i7;
    assert(!(_p & ~0x3fL));
    assert(!(r3  & ~0x3L));
    assert(im >= -2097152 && im <= 2097151);
    assert(!(r1  & ~0x7fL));
    /* imm22 = sign_ext(s << 21 | imm5c << 16 | imm9d << 7 | imm7b, 22) */
    s  = (im & 0x200000) >> 21;
    i5 = (im & 0x1f0000) >> 16;
    i9 = (im &   0xff80) >>  7;
    i7 =  im &     0x7f;
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((9L<<37)|(s<<36)|(i9<<27)|(i5<<22)|
	 (r3<<20)|(i7<<13)|(r1<<6)|_p, INST_A);
    SETREG(r1);
}

static void
_A6(jit_state_t *_jit, jit_word_t _p,
    jit_word_t o, jit_word_t x2, jit_word_t ta, jit_word_t p2,
    jit_word_t r3, jit_word_t r2, jit_word_t c, jit_word_t p1)
{
    assert(!(_p & ~0x3fL));
    assert(!(o  &  ~0xfL));
    assert(!(x2 &  ~0x3L));
    assert(!(ta &  ~0x1L));
    assert(!(p2 & ~0x7fL));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(c  &  ~0x1L));
    assert(!(p1 & ~0x3fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    inst((o<<37)|(x2<<34)|(ta<<33)|(p2<<27)|(r3<<20)|
	 (r2<<13)|(c<<12)|(p1<<6)|_p, INST_A);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
_A7(jit_state_t *_jit, jit_word_t _p,
    jit_word_t o, jit_word_t x2, jit_word_t ta,
    jit_word_t p2, jit_word_t r3, jit_word_t c, jit_word_t p1)
{
    assert(!(_p & ~0x3fL));
    assert(!(o  &  ~0xfL));
    assert(!(x2 &  ~0x3L));
    assert(!(ta &  ~0x1L));
    assert(!(p2 & ~0x7fL));
    assert(!(r3 & ~0x7fL));
    assert(!(c  &  ~0x1L));
    assert(!(p1 & ~0x3fL));
    TSTREG1(r3);
    TSTPRED(_p);
    inst((o<<37)|(1L<<36)|(x2<<34)|(ta<<33)|
	 (p2<<27)|(r3<<20)|(c<<12)|(p1<<6)|_p, INST_A);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
_A8(jit_state_t *_jit, jit_word_t _p,
    jit_word_t o, jit_word_t x2, jit_word_t ta, jit_word_t p2,
    jit_word_t r3, jit_word_t im, jit_word_t c, jit_word_t p1)
{
    assert(!(_p & ~0x3fL));
    assert(!(o  &  ~0xfL));
    assert(!(x2 &  ~0x3L));
    assert(!(ta &  ~0x1L));
    assert(!(p2 & ~0x7fL));
    assert(!(r3 & ~0x7fL));
    assert(im >= -128 && im <= 127);
    assert(!(c  &  ~0x1L));
    assert(!(p1 & ~0x3fL));
    TSTREG1(r3);
    TSTPRED(_p);
    inst((o<<37)|(((im>>7)&1L)<<36)|(x2<<34)|(ta<<33)|(p2<<27)|(r3<<20)|
	 ((im&0x7fL)<<13)|(c<<12)|(p1<<6)|_p, INST_A);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
_A9(jit_state_t *_jit, jit_word_t _p,
    jit_word_t za, jit_word_t zb, jit_word_t x4,
    jit_word_t x2, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(za &  ~0x1L));
    assert(!(zb &  ~0x1L));
    assert(!(x4 &  ~0xfL));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((8L<<37)|(za<<36)|(1L<<34)|(zb<<33)|(x4<<29)|(x2<<27)|
	 (r3<<20)|(r2<<13)|(r1<<6)|_p, INST_A);
    SETREG(r1);
}

static void
_I1(jit_state_t *_jit, jit_word_t _p,
    jit_word_t ct, jit_word_t x2, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(ct &  ~0x3L));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(1L<<33)|(ct<<30)|(x2<<28)|
	 (r3<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I2(jit_state_t *_jit, jit_word_t _p,
    jit_word_t za, jit_word_t xa, jit_word_t zb, jit_word_t xc,
    jit_word_t xb ,jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(za &  ~0x1L));
    assert(!(xa &  ~0x3L));
    assert(!(zb &  ~0x1L));
    assert(!(xc &  ~0x3L));
    assert(!(xb &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(za<<36)|(xa<<34)|(zb<<33)|(xc<<30)|
	 (xb<<28)|(r3<<20)|(r2<<13)|(r1<<6), INST_I);
    SETREG(r1);
}

static void
_I3(jit_state_t *_jit, jit_word_t _p,
    jit_word_t mb, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(mb &  ~0xfL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(3L<<34)|(2L<<30)|(2L<<28)|
	 (mb<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I4(jit_state_t *_jit, jit_word_t _p,
    jit_word_t mh, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(mh & ~0xffL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(3L<<34)|(1L<<33)|(2L<<30)|
	 (2L<<28)|(mh<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I5(jit_state_t *_jit, jit_word_t _p,
    jit_word_t za, jit_word_t zb, jit_word_t x2,
    jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(za &  ~0x1L));
    assert(!(zb &  ~0x1L));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(za<<36)|(zb<<33)|(x2<<28)|
	 (r3<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I6(jit_state_t *_jit, jit_word_t _p,
    jit_word_t za, jit_word_t zb, jit_word_t x2,
    jit_word_t r3, jit_word_t ct, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(za &  ~0x1L));
    assert(!(zb &  ~0x1L));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(ct & ~0x1fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(za<<36)|(1L<<34)|(zb<<33)|
	 (x2<<28)|(r3<<20)|(ct<<14)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I7(jit_state_t *_jit, jit_word_t _p,
    jit_word_t za, jit_word_t zb, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(za &  ~0x1L));
    assert(!(zb &  ~0x1L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(za<<36)|(zb<<33)|(1L<<30)|
	 (r3<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I8(jit_state_t *_jit, jit_word_t _p,
    jit_word_t za, jit_word_t zb, jit_word_t im, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(za &  ~0x1L));
    assert(!(zb &  ~0x1L));
    assert(!(im & ~0x1fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(za<<36)|(3L<<34)|(zb<<33)|(1L<<30)|(1L<<28)|
	 (im<<20)|(r2<<13)|(r1<<6), INST_I);
    SETREG(r1);
}

static void
_I9(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x2, jit_word_t r3, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x2 &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((7L<<37)|(1L<<34)|(1L<<34)|(1L<<33)|
	 (x2<<30)|(1L<<28)|(r3<<20)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I10(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ct, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(ct & ~0x3fL));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((5L<<37)|(3L<<34)|(ct<<27)|(r3<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I11(jit_state_t *_jit, jit_word_t _p,
     jit_word_t len, jit_word_t r3,
     jit_word_t pos, jit_word_t y, jit_word_t r1)
{
    assert(!(_p  & ~0x3fL));
    assert(!(len & ~0x3fL));
    assert(!(r3  & ~0x7fL));
    assert(!(pos & ~0x1fL));
    assert(!(y   &  ~0x1L));
    assert(!(r1  & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((5L<<37)|(1L<<34)|(len<<27)|(r3<<20)|
	 (pos<<14)|(y<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I12(jit_state_t *_jit, jit_word_t _p,
     jit_word_t len, jit_word_t pos, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p  & ~0x3fL));
    assert(!(len & ~0x3fL));
    assert(!(pos & ~0x3fL));
    assert(!(r2  & ~0x7fL));
    assert(!(r1  & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((5L<<37)|(1L<<34)|(1L<<33)|(len<<27)|
	 (pos<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I13(jit_state_t *_jit, jit_word_t _p,
     jit_word_t len, jit_word_t pos, jit_word_t im, jit_word_t r1)
{
    assert(!(_p  & ~0x3fL));
    assert(!(len & ~0x3fL));
    assert(!(pos & ~0x3fL));
    assert(!(im  & ~0x7fL));
    assert(!(r1  & ~0x7fL));
    TSTPRED(_p);
    TSTREG1(r1);
    inst((5L<<37)|(((im>>7)&1L)<<36)|(1L<<34)|(1L<<33)|(len<<27)|
	 (1L<<26)|(pos<<20)|((im&0x7fL)<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I14(jit_state_t *_jit, jit_word_t _p,
     jit_word_t s, jit_word_t len, jit_word_t r3, jit_word_t pos, jit_word_t r1)
{
    assert(!(_p  & ~0x3fL));
    assert(!(s   &  ~0x1L));
    assert(!(len & ~0x3fL));
    assert(!(r3  & ~0x7fL));
    assert(!(pos & ~0x1fL));
    assert(!(r1  & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((5L<<37)|(s<<36)|(3L<<34)|(1L<<33)|
	 (len<<27)|(r3<<20)|(pos<<14)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I15(jit_state_t *_jit, jit_word_t _p,
     jit_word_t pos, jit_word_t len,
     jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p  & ~0x3fL));
    assert(!(pos & ~0x3fL));
    assert(!(len &  ~0xfL));
    assert(!(r3  & ~0x7fL));
    assert(!(r2  & ~0x7fL));
    assert(!(r1  & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((4L<<37)|(pos<<31)|(len<<27)|(r3<<20)|(r2<<13)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I16(jit_state_t *_jit, jit_word_t _p,
     jit_word_t tb, jit_word_t ta, jit_word_t p2,
     jit_word_t r3, jit_word_t ps, jit_word_t c, jit_word_t p1)
{
    assert(!(_p & ~0x3fL));
    assert(!(tb &  ~0x1L));
    assert(!(ta &  ~0x1L));
    assert(!(p2 & ~0x7fL));
    assert(!(r3 & ~0x7fL));
    assert(!(ps & ~0x3fL));
    assert(!(c  &  ~0x1L));
    assert(!(p1 & ~0x3fL));
    TSTREG1(r3);
    TSTPRED(_p);
    inst((5L<<37)|(tb<<36)|(ta<<33)|(p2<<27)|
	 (r3<<20)|(ps<<14)|(c<<12)|(p1<<6), INST_I);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
_I17(jit_state_t *_jit, jit_word_t _p,
     jit_word_t tb, jit_word_t ta, jit_word_t p2,
     jit_word_t r3, jit_word_t c, jit_word_t p1)
{
    assert(!(_p & ~0x3fL));
    assert(!(tb &  ~0x1L));
    assert(!(ta &  ~0x1L));
    assert(!(p2 & ~0x7fL));
    assert(!(r3 & ~0x7fL));
    assert(!(c  &  ~0x1L));
    assert(!(p1 & ~0x3fL));
    TSTREG1(r3);
    TSTPRED(_p);
    inst((5L<<37)|(tb<<36)|(ta<<33)|(p2<<27)|
	 (r3<<20)|(1L<<13)|(c<<12)|(p1<<6)|_p, INST_I);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
_I18(jit_state_t *_jit, jit_word_t _p,
     jit_word_t im, jit_word_t y)
{
    assert(!(_p &     ~0x3fL));
    assert(!(im & ~0x1fffffL));
    assert(!(y  &      ~0x1L));
    TSTPRED(_p);
    inst((((im>>20)&1L)<<26)|(1L<<27)|(y<<26)|((im&0xffffL)<<6)|_p, INST_I);
}

static void
_I19(jit_state_t *_jit, jit_word_t _p,
     jit_word_t im)
{
    assert(!(_p &     ~0x3fL));
    assert(!(im & ~0x1fffffL));
    TSTPRED(_p);
    inst(((im>>20)&1L)|((im&0xffffL)<<6)|_p, INST_I);
}

static void
_I20(jit_state_t *_jit, jit_word_t _p,
     jit_word_t r2, jit_word_t im)
{
    assert(!(_p &     ~0x3fL));
    assert(!(r2 &     ~0x7fL));
    assert(!(im & ~0x1fffffL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst(((im>>20)&1L)|(1L<<33)|(((im>>7)&0x1fffL)<<20)|
	 (r2<<13)|((im&0x7fL)<<6)|_p, INST_I);
}

static void
_I21(jit_state_t *_jit, jit_word_t _p,
     jit_word_t im, jit_word_t ih, jit_word_t x,
     jit_word_t wh, jit_word_t r2, jit_word_t b1)
{
    assert(!(_p &     ~0x3fL));
    assert(!(im &    ~0x1ffL));
    assert(!(ih &      ~0x1L));
    assert(!(x  &      ~0x1L));
    assert(!(wh &      ~0x3L));
    assert(!(r2 &     ~0x7fL));
    assert(!(b1 &      ~0x7L));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((7L<<33)|(im<<24)|(ih<<23)|(x<<22)|(wh<<20)|
	 (r2<<13)|(b1<<6), INST_I);
}

static void
_I22(jit_state_t *_jit, jit_word_t _p,
     jit_word_t b2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(b2 &  ~0x7L));
    assert(!(r1 & ~0x7fL));
    TSTPRED(_p);
    inst((0x31L<<27)|(b2<<13)|(r1<<6)|_p, INST_I);
}

static void
_I23(jit_state_t *_jit, jit_word_t _p,
     jit_word_t r2, jit_word_t im)
{
    assert(!(_p &   ~0x3fL));
    assert(!(r2 &   ~0x7fL));
    assert(!(im & ~0xffffL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((((im>>15)&1L)<<36)|(3L<<33)|(((im>>7)&0xffL)<<24)|
	 (r2<<13)|(im&0x7fL)|_p, INST_I);
}

static void
_I24(jit_state_t *_jit, jit_word_t _p,
     jit_word_t im)
{
    jit_uint8_t		cc = INST_I;
    assert(!(_p &      ~0x3fL));
    assert(!(im & ~0xfffffffL));
    TSTPRED(_p);
    inst((((im>>27)&1L)<<36)|(2L<<33)|((im&0x7ffffffL)<<6)|_p, cc);
}

static void
_I25(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(r1 & ~0x7fL));
    TSTPRED(_p);
    TSTREG1(r1);
    inst((x6<<27)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I26(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ar,jit_word_t r2)
{
    assert(!(_p & ~0x3fL));
    assert(!(ar & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((0x2aL<<27)|(ar<<20)|(r2<<13)|_p, INST_I);
}

static void
_I27(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ar,jit_word_t im)
{
    assert(!(_p & ~0x3fL));
    assert(!(ar & ~0x7fL));
    assert(!(im & ~0xffL));
    TSTPRED(_p);
    inst((((im>>7)&1L)<<36)|(0xaL<<27)|(ar<<20)|((im&0x7fL)<<13)|_p, INST_I);
}

static void
_I28(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ar, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(ar & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTPRED(_p);
    TSTREG1(r1);
    inst((0x32L<<27)|(ar<<20)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I29(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3,jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(r3 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((x6<<27)|(r3<<20)|(r1<<6)|_p, INST_I);
    SETREG(r1);
}

static void
_I30(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ta, jit_word_t tb, jit_word_t p2,
     jit_word_t im, jit_word_t c, jit_word_t p1)
{
    assert(!(_p & ~0x3fL));
    assert(!(ta &  ~0x1L));
    assert(!(tb &  ~0x1L));
    assert(!(p2 & ~0x3fL));
    assert(!(im & ~0x1fL));
    assert(!(c  &  ~0x1L));
    assert(!(p1 & ~0x1fL));
    TSTPRED(_p);
    inst((5L<<37)|(tb<<36)|(ta<<33)|(1L<<19)|(im<<14)|
	 (1L<<13)|(c<<12)|(p1<<6)|_p, INST_I);
    if (p1)
	_jitc->pred |= 1 << p1;
    if (p2)
	_jitc->pred |= 1 << p2;
}

static void
_M1(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t x, jit_word_t r3, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(x  &  ~0x1L));
    assert(!(r3 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((4L<<37)|(x6<<30)|(ht<<28)|(x<<27)|(r3<<20)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M2(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((4L<<37)|(1L<<36)|(x6<<30)|(ht<<28)|
	 (r3<<20)|(r2<<13)|(r1<<6)|_p, INST_M);
    SETREG(r1);
    SETREG(r3);
}

static void
_M3(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t im, jit_word_t r1)
{
    jit_uint8_t		cc = INST_M;
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(ht &   ~0x3L));
    assert(!(r3 &  ~0x7fL));
    assert(im > -256 && im <= 255);
    assert(!(r1 &  ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((5L<<37)|(((im>>8)&1L)<<36)|(x6<<30)|(ht<<28)|
	 (((im>>7)&1L)<<27)|(r3<<20)|((im&0x7fL)<<13)|(r1<<6)|_p, cc);
    SETREG(r1);
    SETREG(r3);
}

static void
_M5(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t r2, jit_word_t im)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(ht &   ~0x3L));
    assert(!(r3 &  ~0x7fL));
    assert(!(r2 &  ~0x7fL));
    assert(im > -256 && im <= 255);
    TSTREG2(r2, r3);
    TSTPRED(_p);
    inst((5L<<37)|(((im>>8)&1L)<<36)|(x6<<30)|(ht<<28)|
	 (((im>>7)&1L)<<27)|(r3<<20)|(r2<<13)|((im&0x7fL)<<6)|_p, INST_M);
    SETREG(r3);
}

static void
_M6(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6, jit_word_t ht, jit_word_t x, jit_word_t r3, jit_word_t r2)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(x  &  ~0x1L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    inst((4L<<37)|(x6<<30)|(ht<<28)|(x<<27)|(r3<<20)|(r2<<13)|_p, INST_M);
}

static void
_M13(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t r2)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    TSTREG1(r3);
    if (r2)
	TSTFREG1(r2);
    TSTPRED(_p);
    inst((6L<<37)|(x6<<30)|(ht<<28)|(r3<<20)|(r2<<13)|_p, INST_M);
}

static void
_M14(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t r2)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    inst((6L<<37)|(1L<<36)|(x6<<30)|(ht<<28)|(r3<<20)|(r2<<13)|_p, INST_M);
}

static void
_M15(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t im)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(ht &   ~0x3L));
    assert(!(r3 &  ~0x7fL));
    assert(!(im & ~0x1ffL));
    TSTREG1(r3);
    TSTPRED(_p);
    inst((7L<<37)|(((im>>8)&1L)<<36)|(x6<<30)|(ht<<28)|
	 (((im>>7)&1L)<<27)|(r3<<20)|((im&0x7fL)<<13)|_p, INST_M);
}

static void
_M16(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(r2 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((4L<<37)|(x6<<30)|(ht<<28)|(1L<<27)|
	 (r3<<20)|(r2<<13)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M17(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t ht, jit_word_t r3, jit_word_t im, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(ht &  ~0x3L));
    assert(!(r3 & ~0x7fL));
    assert(!(im &  ~0x7L));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((4L<<37)|(x6<<30)|(ht<<28)|(1L<<27)|
	 (r3<<20)|(im<<13)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M20x(jit_state_t *_jit, jit_word_t _p,
      jit_word_t x3, jit_word_t r2, jit_word_t im)
{
    assert(!(_p &     ~0x3fL));
    assert(!(x3 &      ~0x7L));
    assert(!(r2 &     ~0x7fL));
    assert(!(im & ~0x1fffffL));
    if (x3 == 1)
	TSTREG1(r2);
    else
	TSTFREG1(r2);
    TSTPRED(_p);
    inst((1L<<37)|(((im>>20)&1L)<<36)|(x3<<33)|
	 (((im>>7)&0x1fffL)<<20)|(r2<<13)|((im&0x7fL)<<6)|_p, INST_M);
}

static void
_M22x(jit_state_t *_jit, jit_word_t _p,
      jit_word_t x3, jit_word_t im, jit_word_t r1)
{
    assert(!(_p &     ~0x3fL));
    assert(!(x3 &      ~0x7L));
    assert(!(im & ~0x1fffffL));
    assert(!(r1 &     ~0x7fL));
    TSTPRED(_p);
    if (x3 < 6)
	TSTREG1(r1);
    else
	TSTFREG1(r1);
    inst((((im>>20)&1L)<<36)|(x3<<33)|((im&0xffffL)<<13)|(r1<<6)|_p, INST_M);
    if (x3 < 6)
	SETREG(r1);
    else
	SETFREG(r1);
}

static void
_M24(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x2, jit_word_t x4)
{
    assert(!(_p & ~0x3fL));
    assert(!(x2 &  ~0x3L));
    assert(!(x4 &  ~0xfL));
    TSTPRED(_p);
    inst((x2<<31)|(x4<<27)|_p, INST_M);
}

static void
_M26x(jit_state_t *_jit, jit_word_t _p,
      jit_word_t x4, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x4 &  ~0xfL));
    assert(!(r1 & ~0x7fL));
    if (x4 == 2)
	TSTREG1(r1);
    else
	TSTFREG1(r1);
    TSTPRED(_p);
    inst((1L<<31)|(x4<<27)|(r1<<6)|_p, INST_M);
}

static void
_M28(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x, jit_word_t r3)
{
    assert(!(_p & ~0x3fL));
    assert(!(x  &  ~0x1L));
    assert(!(r3 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    inst((1L<<37)|(x<<36)|(0x30L<<27)|(r3<<20)|_p, INST_M);
}

static void
_M29(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ar, jit_word_t r2)
{
    assert(!(_p & ~0x3fL));
    assert(!(ar  & ~0x7L));
    assert(!(r2 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((1L<<37)|(0x2aL<<27)|(ar<<20)|(r2<<13)|_p, INST_M);
}

static void
_M30(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ar, jit_word_t im)
{
    assert(!(_p & ~0x3fL));
    assert(!(ar  & ~0x7L));
    assert(!(im & ~0xffL));
    TSTPRED(_p);
    inst((((im>>7)&1L)<<36)|(2L<<31)|(0x8L<<27)|
	 (ar<<20)|((im&0x7fL)<<13)|_p, INST_M);
}

static void
_M31(jit_state_t *_jit, jit_word_t _p,
     jit_word_t ar, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(ar  & ~0x7L));
    assert(!(r1 & ~0x7fL));
    TSTPRED(_p);
    TSTREG1(r1);
    inst((1L<<37)|(0x22L<<27)|(ar<<20)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M32(jit_state_t *_jit, jit_word_t _p,
     jit_word_t cr, jit_word_t r2)
{
    assert(!(_p & ~0x3fL));
    assert(!(cr  & ~0x7L));
    assert(!(r2 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((1L<<37)|(0x2cL<<27)|(cr<<20)|(r2<<13)|_p, INST_M);
}

static void
_M33(jit_state_t *_jit, jit_word_t _p,
     jit_word_t cr, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(cr  & ~0x7L));
    assert(!(r1 & ~0x7fL));
    TSTPRED(_p);
    TSTREG1(r1);
    inst((1L<<37)|(0x24L<<27)|(cr<<20)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M34(jit_state_t *_jit, jit_word_t _p,
     jit_word_t sor, jit_word_t sol, jit_word_t sof, jit_word_t r1)
{
    assert(!(_p  & ~0x3fL));
    assert(!(sor &  ~0xfL));
    assert(!(sol & ~0x7fL));
    assert(!(sof & ~0x7fL));
    assert(!(r1  & ~0x7fL));
    TSTPRED(_p);
    inst((1L<<37)|(6L<<33)|(sor<<27)|(sol<<20)|(sof<<13)|(r1<<6)|_p, INST_M);
}

static void
_M35(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r2)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(r2 & ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((1L<<37)|(x6<<27)|(r2<<13)|_p, INST_M);
}

static void
_M36(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(r1 & ~0x7fL));
    TSTPRED(_p);
    TSTREG1(r1);
    inst((1L<<37)|(x6<<27)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M37(jit_state_t *_jit, jit_word_t _p,
     jit_word_t im)
{
    assert(!(_p &    ~0x3fL));
    assert(!(im & ~0x1ffffL));
    TSTPRED(_p);
    inst((((im>>20)&1L)<<36)|((im&0xffffL)<<6)|_p, INST_M);
}

static void
_M38(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3, jit_word_t r2, jit_word_t r1)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(r3 &  ~0x7fL));
    assert(!(r2 &  ~0x7fL));
    assert(!(r1 &  ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((1L<<37)|(x6<<27)|(r3<<20)|(r2<<13)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M39(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3, jit_word_t im, jit_word_t r1)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(r3 &  ~0x7fL));
    assert(!(im &   ~0x7L));
    assert(!(r1 &  ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    TSTREG1(r1);
    inst((1L<<37)|(x6<<27)|(r3<<20)|(im<<13)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M40(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3, jit_word_t im)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(r3 &  ~0x7fL));
    assert(!(im &   ~0x7L));
    TSTREG1(r3);
    TSTPRED(_p);
    inst((1L<<37)|(x6<<27)|(r3<<20)|(im<<13)|_p, INST_M);
}

static void
_M41(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r2)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(r2 &  ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((1L<<37)|(x6<<27)|(r2<<13)|_p, INST_M);
}

static void
_M42(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3, jit_word_t r2)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(r3 &  ~0x7fL));
    assert(!(r2 &  ~0x7fL));
    TSTREG1(r2);
    TSTPRED(_p);
    inst((1L<<37)|(x6<<27)|(r3<<20)|(r2<<13)|_p, INST_M);
}

static void
_M43(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3, jit_word_t r1)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(r3 &  ~0x7fL));
    assert(!(r1 &  ~0x7fL));
    TSTPRED(_p);
    TSTREG1(r1);
    inst((1L<<37)|(x6<<27)|(r3<<20)|(r1<<6)|_p, INST_M);
    SETREG(r1);
}

static void
_M44(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x4, jit_word_t im)
{
    assert(!(_p &    ~0x3fL));
    assert(!(x4 &     ~0xfL));
    assert(!(im & ~0xfffffL));
    TSTPRED(_p);
    inst((((im>>23)&1L)<<36)|(((im>>21)&3L)<<31)|
	 (x4<<27)|((im&0x1ffffL)<<6)|_p, INST_M);
}

static void
_M45(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3, jit_word_t r2)
{
    assert(!(_p &  ~0x3fL));
    assert(!(x6 &  ~0x3fL));
    assert(!(r3 &  ~0x7fL));
    assert(!(r2 &  ~0x7fL));
    TSTREG2(r2, r3);
    TSTPRED(_p);
    inst((1L<<37)|(x6<<27)|(r3<<20)|(r2<<13)|_p, INST_M);
}

static void
_M46(jit_state_t *_jit, jit_word_t _p,
     jit_word_t x6, jit_word_t r3, jit_word_t r1)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    assert(!(r3 & ~0x7fL));
    assert(!(r1 & ~0x7fL));
    TSTREG1(r3);
    TSTPRED(_p);
    if (r1)	TSTREG1(r1);
    inst((1L<<37)|(x6<<27)|(r3<<20)|(r1<<6)|_p, INST_M);
    if (r1)	SETREG(r1);
}

static void
_M48(jit_state_t *_jit, jit_word_t _p,
     jit_word_t y, jit_word_t im)
{
    assert(!(_p &    ~0x3fL));
    assert(!(y  &     ~0x1L));
    assert(!(im & ~0x1ffffL));
    TSTPRED(_p);
    inst((((im>>20)&1L)<<36)|(1L<<27)|(y<<26)|((im&0xffffL)<<6)|_p, INST_M);
}

static void
_B1(jit_state_t *_jit, jit_word_t _p,
    jit_word_t d, jit_word_t wh, jit_word_t im, jit_word_t p, jit_word_t tp)
{
    assert(!(_p &    ~0x3fL));
    assert(!(d  &     ~0x1L));
    assert(!(wh &     ~0x3L));
    assert(im >= -1048576 && im <= 1048575);
    assert(!(p  &     ~0x1L));
    assert(!(tp &     ~0x7L));
    TSTPRED(_p);
    inst((4L<<37)|(((im>>20)&1L)<<36)|(d<<35)|(wh<<33)|
	 ((im&0xfffffL)<<13)|(p<<12)|(tp<<6)|_p, INST_B);
}

static void
_B3(jit_state_t *_jit, jit_word_t _p,
    jit_word_t d, jit_word_t wh, jit_word_t im, jit_word_t p, jit_word_t b)
{
    assert(!(_p &    ~0x3fL));
    assert(!(d  &     ~0x1L));
    assert(!(wh &     ~0x3L));
    assert(im >= -1048576 && im <= 1048575);
    assert(!(p  &     ~0x1L));
    assert(!(b  &     ~0x3L));
    TSTPRED(_p);
    inst((5L<<37)|(((im>>20)&1L)<<36)|(d<<35)|(wh<<33)|
	 ((im&0xfffffL)<<13)|(p<<12)|(b<<6)|_p, INST_B);
}

static void
_B4(jit_state_t *_jit, jit_word_t _p,
    jit_word_t d, jit_word_t wh, jit_word_t x6,
    jit_word_t b, jit_word_t p, jit_word_t tp)
{
    assert(!(_p & ~0x3fL));
    assert(!(d  &  ~0x1L));
    assert(!(wh &  ~0x3L));
    assert(!(x6 & ~0x3fL));
    assert(!(b  &  ~0x7L));
    assert(!(p  &  ~0x1L));
    assert(!(tp &  ~0x7L));
    TSTPRED(_p);
    inst((d<<35)|(wh<<33)|(x6<<27)|(b<<13)|(p<<12)|(tp<<6)|_p, INST_B);
}

static void
_B5(jit_state_t *_jit, jit_word_t _p,
    jit_word_t d, jit_word_t wh, jit_word_t b2, jit_word_t p, jit_word_t b1)
{
    assert(!(_p & ~0x3fL));
    assert(!(d  &  ~0x1L));
    assert(!(wh &  ~0x3L));
    assert(!(b2 &  ~0x7L));
    assert(!(p  &  ~0x1L));
    assert(!(b1 &  ~0x7L));
    TSTPRED(_p);
    inst((1L<<37)|(d<<35)|(wh<<32)|(b2<<13)|(p<<12)|(b1<<6)|_p, INST_B);
}

static void
_B6(jit_state_t *_jit, jit_word_t _p,
    jit_word_t ih, jit_word_t im, jit_word_t tag, jit_word_t wh)
{
    assert(!(_p &     ~0x3fL));
    assert(!(ih  &     ~0x1L));
    assert(!(im  & ~0x1ffffL));
    assert(!(tag &   ~0x1ffL));
    assert(!(wh  &     ~0x3L));
    TSTPRED(_p);
    inst((7L<<37)|(((im>>20)&1L)<<36)|(ih<<35)|(((tag>>7)&3L)<<33)|
	 ((im&0xfffffL)<<13)|((tag&0x7fL)<<6)|(wh<<3)|_p, INST_B);
}

static void
_B7(jit_state_t *_jit, jit_word_t _p,
    jit_word_t ih, jit_word_t x6, jit_word_t b2, jit_word_t tag, jit_word_t wh)
{
    assert(!(_p &   ~0x3fL));
    assert(!(ih  &   ~0x1L));
    assert(!(x6  &  ~0x3fL));
    assert(!(b2  &   ~0x7L));
    assert(!(tag & ~0x1ffL));
    assert(!(wh  &   ~0x3L));
    TSTPRED(_p);
    inst((2L<<37)|(ih<<35)|(((tag>>7)&3L)<<33)|(x6<<27)|
	 (b2<<13)|((tag&0x7fL)<<6)|(wh<<3)|_p, INST_B);
}

static void
_B8(jit_state_t *_jit, jit_word_t _p,
    jit_word_t x6)
{
    assert(!(_p & ~0x3fL));
    assert(!(x6 & ~0x3fL));
    TSTPRED(_p);
    inst((x6<<27)|_p, INST_B);
}

static void
_B9(jit_state_t *_jit, jit_word_t _p,
    jit_word_t op, jit_word_t x6, jit_word_t im)
{
    assert(!(_p &    ~0x3fL));
    assert(!(op &     ~0xfL));
    assert(!(x6 &    ~0x3fL));
    assert(!(im & ~0x1ffffL));
    TSTPRED(_p);
    inst((op<<37)|(((im>>20)&1L)<<36)|(x6<<27)|((im&0xffffL)<<6)|_p, INST_B);
}

static void
_X1(jit_state_t *_jit, jit_word_t _p,
    jit_word_t im)
{
    jit_word_t		i41, i1, i20;
    assert(!(_p &               ~0x3fL));
    assert(im > -0x2000000000000000 && im <= 0x1fffffffffffffff);
    i41 = (im >> 22) & 0x1ffffffffffL;
    i1  = (im >> 21) &           0x1L;
    i20 =  im        &       0xfffffL;
    TSTPRED(_p);
    inst(i41, INST_L);
    inst((i1<<36)|(i20<<6)|_p, INST_X);
}

static void
_X2(jit_state_t *_jit, jit_word_t _p,
    jit_word_t r1, jit_word_t im)
{
    jit_word_t		i1, i41, ic, i5, i9, i7;
    assert(!(_p & ~0x3fL));
    assert(!(r1 & ~0x7fL));
    i1  = (im >> 63) &           0x1L;
    i41 = (im >> 22) & 0x1ffffffffffL;
    ic  = (im >> 21) &           0x1L;
    i5  = (im >> 16) &          0x1fL;
    i9  = (im >>  7) &         0x1ffL;
    i7  =  im        &          0x7fL;
    TSTPRED(_p);
    TSTREG1(r1);
    inst(i41, INST_L);
    inst((6L<<37)|(i1<<36)|(i9<<27)|(i5<<22)|
	 (ic<<21)|(i7<<13)|(r1<<6)|_p, INST_X);
    SETREG(r1);
}

static void
_X3x(jit_state_t *_jit, jit_word_t _p,
     jit_word_t op, jit_word_t d, jit_word_t wh,
     jit_word_t p, jit_word_t tp, jit_word_t im)
{
    /* target64 = IP + ((i1 << 59 | imm39 << 20 | imm20b) << 4) */
    jit_word_t		i1, i41, i20;
    assert(!(_p &               ~0x3fL));
    assert(!(op &                ~0xfL));
    assert(!(d  &                ~0x1L));
    assert(!(wh &                ~0x3L));
    assert(!(p  &                ~0x1L));
    assert(!(tp &                ~0x7L));
    i1  = (im >> 61) &           0x1L;
    i41 = (im >> 22) & 0x1ffffffffffL;
    i20 =  im        &       0xfffffL;
    TSTPRED(_p);
    inst(i41, INST_L);
    inst((op<<37)|(i1<<36)|(d<<35)|(wh<<33)|
	 (i20<<13)|(p<<12)|(tp<<6)|_p, INST_X);
}

static void
_X5(jit_state_t *_jit, jit_word_t _p,
    jit_word_t y, jit_word_t im)
{
    jit_word_t		i41, i1, i20;
    assert(!(_p &               ~0x3fL));
    assert(im > -0x2000000000000000 && im <= 0x1fffffffffffffff);
    i41 = (im >> 22) & 0x1ffffffffffL;
    i1  = (im >> 21) &           0x1L;
    i20 =  im        &       0xfffffL;
    TSTPRED(_p);
    inst(i41, INST_L);
    inst((i1<<36)|(1L<<27)|(y<<26)|(i20<<6)|_p, INST_X);
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* patch pushargr */
    if (r0 >= 120)
	r0 = _jitc->rout + (r0 - 120);

    MOV(r0, r1);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    /* patch pushargi */
    if (r0 >= 120)
	r0 = _jitc->rout + (r0 - 120);

    if (i0 >= -2097152 && i0 <= 2097151)
	MOVI(r0, i0);
    else
	MOVL(r0, i0);
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    sync();
    w = _jit->pc.w;
    MOVL(r0, i0);
    return (w);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -8192 && i0 <= 8191)
	ADDS(r0, i0, r1);
    else if (!(r1 & ~3) && i0 >= -2097152 && i0 <= 2097151)
	ADDL(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	reg = jit_get_reg(jit_class_gpr);
	addr(rn(reg), r1, r2);
	ltr_u(rn(jit_carry), rn(reg), r1);
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	addr(r0, r1, r2);
	ltr_u(rn(jit_carry), r0, r1);
    }
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ltr_u(rn(jit_carry), rn(reg), r1);
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	addi(r0, r1, i0);
	ltr_u(rn(jit_carry), r0, r1);
    }
}

static void
_addxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    assert(jit_carry != _NOREG);
    reg = jit_get_reg(jit_class_gpr);
    movr(rn(reg), rn(jit_carry));
    addcr(r0, r1, r2);
    addcr(r0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(jit_carry != _NOREG);
    reg = jit_get_reg(jit_class_gpr);
    movr(rn(reg), rn(jit_carry));
    addci(r0, r1, i0);
    addcr(r0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    subr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_subcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	reg = jit_get_reg(jit_class_gpr);
	subr(rn(reg), r1, r2);
	ltr_u(rn(jit_carry), r1, rn(reg));
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	subr(r0, r1, r2);
	ltr_u(rn(jit_carry), r1, r0);
    }
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, -i0);
	ltr_u(rn(jit_carry), r1, rn(reg));
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	addi(r0, r1, -i0);
	ltr_u(rn(jit_carry), r1, r0);
    }
}

static void
_subxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    assert(jit_carry != _NOREG);
    reg = jit_get_reg(jit_class_gpr);
    movr(rn(reg), rn(jit_carry));
    subcr(r0, r1, r2);
    subcr(r0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(jit_carry != _NOREG);
    reg = jit_get_reg(jit_class_gpr);
    movr(rn(reg), rn(jit_carry));
    subci(r0, r1, i0);
    subcr(r0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_rsbi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -128 && i0 <= 127)
	SUBI(r0, i0, r1);
    else if (!(r1 & ~3) && i0 >= -2097151 && i0 <= 2097152)
	ADDL(r1, -i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subr(r0, rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_mulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		f0, f1;
    f0 = jit_get_reg(jit_class_fpr);
    f1 = jit_get_reg(jit_class_fpr);
    SETF_SIG(rn(f0), r1);
    SETF_SIG(rn(f1), r2);
    XMPY_L(rn(f0), rn(f0), rn(f1));
    GETF_SIG(r0, rn(f0));
    jit_unget_reg(f0);
    jit_unget_reg(f1);
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    mulr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

#if !defined(__GNUC__)
static long
__divdi3(long u, long v)
{
    return (u / v);
}

static unsigned long
__udivdi3(unsigned long u, unsigned long v)
{
    return (u / v);
}

static long
__moddi3(long u, long v)
{
    return (u % v);
}

static unsigned long
__umoddi3(unsigned long u, unsigned long v)
{
    return (u % v);
}
#endif

static void
_divr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MOV(_jitc->rout, r1);
    MOV(_jitc->rout + 1, r2);
    calli((jit_word_t)__divdi3);
    MOV(r0, GR_8);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    switch (i0) {
	case 1:
	    movr(r0, r1);
	    return;
	case -1:
	    negr(r0, r1);
	    return;
	default:
	    if (i0 > 0 && !(i0 & (i0 - 1))) {
		movr(r0, r1);
		rshi(r0, r0, ffsl(i0) - 1);
		return;
	    }
	    break;
    }
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    divr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_divr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MOV(_jitc->rout, r1);
    MOV(_jitc->rout + 1, r2);
    calli((jit_word_t)__udivdi3);
    MOV(r0, GR_8);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 1) {
	movr(r0, r1);
	return;
    }
    else if (i0 > 0 && !(i0 & (i0 - 1))) {
	movr(r0, r1);
	rshi_u(r0, r0, ffsl(i0) - 1);
	return;
    }
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    divr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MOV(_jitc->rout, r1);
    MOV(_jitc->rout + 1, r2);
    calli((jit_word_t)__moddi3);
    MOV(r0, GR_8);
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 1 || i0 == -1) {
	MOV(r0, GR_0);
	return;
    }
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_remr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MOV(_jitc->rout, r1);
    MOV(_jitc->rout + 1, r2);
    calli((jit_word_t)__umoddi3);
    MOV(r0, GR_8);
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 1) {
	MOV(r0, GR_0);
	return;
    }
    else if (i0 > 0 && !(i0 & (i0 - 1))) {
	andi(r0, r1, i0 - 1);
	return;
    }
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_mulh(jit_state_t *_jit,
      jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_bool_t sign)
{
    jit_int32_t		f0, f1;
    f0 = jit_get_reg(jit_class_fpr);
    f1 = jit_get_reg(jit_class_fpr);
    SETF_SIG(rn(f0), r1);
    SETF_SIG(rn(f1), r2);
    if (sign)
	XMPY_H(rn(f0), rn(f0), rn(f1));
    else
	XMPY_HU(rn(f0), rn(f0), rn(f1));
    GETF_SIG(r0, rn(f0));
    jit_unget_reg(f0);
    jit_unget_reg(f1);
}

static void
_iqmulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		reg;
    if (r0 == r2 || r0 == r3) {
	reg = jit_get_reg(jit_class_gpr);
	mulr(rn(reg), r2, r3);
    }
    else
	mulr(r0, r2, r3);
    mulh(r1, r2, r3, sign);
    if (r0 == r2 || r0 == r3) {
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_iqmuli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    iqmulr(r0, r1, r2, rn(reg), sign);
    jit_unget_reg(reg);
}

static void
_iqdivr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		sv0, rg0;
    jit_int32_t		sv1, rg1;

    if (r0 == r2 || r0 == r3) {
	sv0 = jit_get_reg(jit_class_gpr);
	rg0 = rn(sv0);
    }
    else
	rg0 = r0;
    if (r1 == r2 || r1 == r3) {
	sv1 = jit_get_reg(jit_class_gpr);
	rg1 = rn(sv1);
    }
    else
	rg1 = r1;

    if (sign)
	divr(rg0, r2, r3);
    else
	divr_u(rg0, r2, r3);
    mulr(rg1, r3, rg0);
    subr(rg1, r2, rg1);
    if (rg0 != r0) {
	movr(r0, rg0);
	jit_unget_reg(sv0);
    }
    if (rg1 != r1) {
	movr(r1, rg1);
	jit_unget_reg(sv1);
    }
}

static void
_iqdivi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    iqdivr(r0, r1, r2, rn(reg), sign);
    jit_unget_reg(reg);
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -128 && i0 <= 127)
	ANDI(r0, i0, r1);
    else if (~i0 >= -128 && ~i0 <= 127)
	ANDCMI(r0, ~i0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	andr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -128 && i0 <= 127)
	ORI(r0, i0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	orr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -128 && i0 <= 127)
	XORI(r0, i0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	xorr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

#if __BYTE_ORDER == __LITTLE_ENDIAN
static void
_htonr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    rshi(rn(t0), r1, 8);
    andi(r0, r1, 0xff);
    andi(rn(t0), rn(t0), 0xff);
    lshi(r0, r0, 8);
    orr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_htonr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr);
    t2 = jit_get_reg(jit_class_gpr);
    rshi(rn(t0), r1, 24);
    rshi(rn(t1), r1, 16);
    rshi(rn(t2), r1,  8);
    andi(rn(t0), rn(t0), 0xff);
    andi(rn(t1), rn(t1), 0xff);
    andi(rn(t2), rn(t2), 0xff);
    andi(r0, r1, 0xff);
    lshi(r0, r0, 24);
    lshi(rn(t1), rn(t1), 8);
    orr(r0, r0, rn(t0));
    lshi(rn(t2), rn(t2), 16);
    orr(r0, r0, rn(t1));
    orr(r0, r0, rn(t2));
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}
#endif

static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    lshr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_rshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    rshr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_rshi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    rshr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_ltr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LT(PR_6, PR_7, r1, r2);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_lti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -127 && i0 <= 128)
	CMPI_LT(PR_7, PR_6, i0 - 1, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMP_LT(PR_6, PR_7, r1, rn(reg));
	jit_unget_reg(reg);
    }
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_ltr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LTU(PR_6, PR_7, r1, r2);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_lti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -127 && i0 <= 128)
	CMPI_LTU(PR_7, PR_6, i0 - 1, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMP_LTU(PR_6, PR_7, r1, rn(reg));
	jit_unget_reg(reg);
    }
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_ler(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LT(PR_6, PR_7, r2, r1);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_lei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ler(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_ler_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LTU(PR_6, PR_7, r2, r1);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_lei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ler_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_eqr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_EQ(PR_6, PR_7, r1, r2);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_eqi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -128 && i0 <= 127)
	CMPI_EQ(PR_6, PR_7, i0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMP_EQ(PR_6, PR_7, r1, rn(reg));
	jit_unget_reg(reg);
    }
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_ger(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LT(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_gei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -127 && i0 <= 128)
	CMPI_LT(PR_7, PR_6, i0 - 1, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMP_LT(PR_6, PR_7, r1, rn(reg));
	jit_unget_reg(reg);
    }
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_ger_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LTU(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_gei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -127 && i0 <= 128)
	CMPI_LTU(PR_7, PR_6, i0 - 1, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMP_LTU(PR_6, PR_7, r1, rn(reg));
	jit_unget_reg(reg);
    }
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_gtr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LT(PR_6, PR_7, r2, r1);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_gti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    gtr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_gtr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_LTU(PR_6, PR_7, r2, r1);
    MOVI_p(r0, 1, PR_6);
    MOV_p(r0, GR_0, PR_7);
}

static void
_gti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    gtr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_ner(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP_EQ(PR_6, PR_7, r1, r2);
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_nei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -128 && i0 <= 127)
	CMPI_EQ(PR_6, PR_7, i0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMP_EQ(PR_6, PR_7, r1, rn(reg));
	jit_unget_reg(reg);
    }
    MOV_p(r0, GR_0, PR_6);
    MOVI_p(r0, 1, PR_7);
}

static void
_ldr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    ldr_uc(r0, r1);
    extr_c(r0, r0);
}

static void
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_c(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_uc(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    ldr_us(r0, r1);
    extr_s(r0, r0);
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_s(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_us(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    ldr_ui(r0, r1);
    extr_i(r0, r0);
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_i(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_ui(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_l(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_l(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_c(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_c(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_c(r0, r1);
}

static void
_ldxr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_uc(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_uc(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_uc(r0, r1);
}

static void
_ldxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_s(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_s(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_s(r0, r1);
}

static void
_ldxr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_us(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_us(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_us(r0, r1);
}

static void
_ldxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_i(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_i(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_i(r0, r1);
}

static void
_ldxr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_ui(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_ui(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_ui(r0, r1);
}

static void
_ldxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_l(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_l(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else
	ldr_l(r0, r1);
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_c(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_s(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_i(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_sti_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_l(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_stxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_c(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_c(rn(reg), r1);
	jit_unget_reg(reg);
    }
    else
	str_c(r0, r1);
}

static void
_stxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_s(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_s(rn(reg), r1);
	jit_unget_reg(reg);
    }
    else
	str_s(r0, r1);
}

static void
_stxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_i(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_i(rn(reg), r1);
	jit_unget_reg(reg);
    }
    else
	str_i(r0, r1);
}

static void
_stxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_l(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0) {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_l(rn(reg), r1);
	jit_unget_reg(reg);
    }
    else
	str_l(r0, r1);
}

static jit_word_t
_bltr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LT(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_blti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -127 && i1 <= 128)
	CMPI_LT(PR_7, PR_6, i1 - 1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	CMP_LT(PR_6, PR_7, r0, rn(reg));
	jit_unget_reg(reg);
    }
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_bltr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LTU(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_blti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -127 && i1 <= 128)
	CMPI_LTU(PR_7, PR_6, i1 - 1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	CMP_LTU(PR_6, PR_7, r0, rn(reg));
	jit_unget_reg(reg);
    }
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_bler(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LT(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_blei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bler(i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bler_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LTU(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_blei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bler_u(i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_beqr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_EQ(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_beqi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -128 && i1 <= 127)
	CMPI_EQ(PR_6, PR_7, i1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	CMP_EQ(PR_6, PR_7, r0, rn(reg));
	jit_unget_reg(reg);
    }
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_bger(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LT(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bgei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -127 && i1 <= 128)
	CMPI_LT(PR_7, PR_6, i1 - 1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	CMP_LT(PR_6, PR_7, r0, rn(reg));
	jit_unget_reg(reg);
    }
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bger_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LTU(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bgei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -127 && i1 <= 128)
	CMPI_LTU(PR_7, PR_6, i1 - 1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	CMP_LTU(PR_6, PR_7, r0, rn(reg));
	jit_unget_reg(reg);
    }
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bgtr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LT(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_bgti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bgtr(i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgtr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_LTU(PR_6, PR_7, r1, r0);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_bgti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bgtr_u(i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bner(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_EQ(PR_6, PR_7, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bnei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -128 && i1 <= 127)
	CMPI_EQ(PR_6, PR_7, i1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	CMP_EQ(PR_6, PR_7, r0, rn(reg));
	jit_unget_reg(reg);
    }
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bmsr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    andr(rn(reg), r0, r1);
    CMPI_EQ(PR_6, PR_7, 0, rn(reg));
    jit_unget_reg(reg);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bmsi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i1);
    andr(rn(reg), r0, rn(reg));
    CMPI_EQ(PR_6, PR_7, 0, rn(reg));
    jit_unget_reg(reg);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_7);
    return (w);
}

static jit_word_t
_bmcr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    andr(rn(reg), r0, r1);
    CMPI_EQ(PR_6, PR_7, 0, rn(reg));
    jit_unget_reg(reg);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_bmci(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i1);
    andr(rn(reg), r0, rn(reg));
    CMPI_EQ(PR_6, PR_7, 0, rn(reg));
    jit_unget_reg(reg);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, PR_6);
    return (w);
}

static jit_word_t
_baddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1,
       jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;
    /* t1 = r0 + r1;	overflow = r1 < 0 ? r0 < t1 : t1 < r0 */
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    lti(rn(t0), r1, 0);			/* t0 = r1 < 0 */
    addr(rn(t1), r0, r1);		/* t1 = r0 + r1 */
    ltr(rn(t2), rn(t1), r0);		/* t2 = t1 < r0 */
    ltr(rn(t1), r0, rn(t1));		/* t1 = r0 < t1 */
    CMPI_EQ(PR_6, PR_7, 0, rn(t0));
    CMPI_EQ_p(PR_8, PR_9, 0, rn(t2), PR_6);/* if (t0==0) p8=t2==0,p9=t2!=0; */
    CMPI_EQ_p(PR_8, PR_9, 0, rn(t1), PR_7);/* if (t0!=0) p8=t1==0,p9=t1!=0; */
    addr(r0, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, carry ? PR_9 : PR_8);
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_baddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
       jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = baddr(i0, r0, rn(reg), carry);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_baddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1,
	 jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    ltr_u(rn(t1), rn(t0), r0);
    CMPI_EQ(PR_6, PR_7, 0, rn(t1));
    MOV(r0, rn(t0));
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, carry ? PR_7 : PR_6);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_baddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
	 jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = baddr_u(i0, r0, rn(reg), carry);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bsubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1,
       jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;
    /* t1 = r0 - r1;	overflow = 0 < r1 ? r0 < t1 : t1 < r0 */
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    gti(rn(t0), r1, 0);			/* t0 = r1 > 0 */
    subr(rn(t1), r0, r1);		/* t1 = r0 - r1 */
    ltr(rn(t2), rn(t1), r0);		/* t2 = t1 < r0 */
    ltr(rn(t1), r0, rn(t1));		/* t1 = r0 < t1 */
    CMPI_EQ(PR_6, PR_7, 0, rn(t0));
    CMPI_EQ_p(PR_8, PR_9, 0, rn(t2), PR_6);/* if (t0==0) p4=t2==0,p5=t2!=0; */
    CMPI_EQ_p(PR_8, PR_9, 0, rn(t1), PR_7);/* if (t0!=0) p4=t1==0,p5=t1!=0; */
    subr(r0, r0, r1);
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, carry ? PR_9 : PR_8);
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bsubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
       jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bsubr(i0, r0, rn(reg), carry);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bsubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1,
	 jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    ltr_u(rn(t1), r0, rn(t0));
    CMPI_EQ(PR_6, PR_7, 0, rn(t1));
    MOV(r0, rn(t0));
    sync();
    w = _jit->pc.w;
    BRI_COND((i0 - w) >> 4, carry ? PR_7 : PR_6);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bsubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
	 jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bsubr_u(i0, r0, rn(reg), carry);
    jit_unget_reg(reg);
    return (w);
}

static void
_jmpr(jit_state_t *_jit, jit_int32_t r0)
{
    MOV_br_rn(BR_6, r0);
    BR(BR_6);
}

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d;
    sync();
    d = ((jit_word_t)i0 - _jit->pc.w) >> 4;
    if (d >= -16777216 && d <= 16777215)
	BRI(d);
    else
	BRL(d);
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d, w;
    sync();
    w = _jit->pc.w;
    d = ((jit_word_t)i0 - w) >> 4;
    BRL(d);
    return (w);
}

static void
_callr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    LD8_inc(rn(reg), r0, 8);
    MOV_br_rn(BR_6, rn(reg));
    jit_unget_reg(reg);
    LD8(GR_1, r0);
    BR_CALL(BR_0, BR_6);
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    callr(rn(reg));
    jit_unget_reg(reg);
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    w = movi_p(rn(reg), i0);
    callr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_prolog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg, ruse, rout;

    if (_jitc->function->define_frame || _jitc->function->assume_frame) {
	jit_int32_t	frame = -_jitc->function->frame;
	assert(_jitc->function->self.aoff >= frame);
	if (_jitc->function->assume_frame)
	    return;
	_jitc->function->self.aoff = frame;
    }
    if (_jitc->function->allocar)
	_jitc->function->self.aoff &= -16;
    _jitc->function->stack = ((_jitc->function->self.alen -
			       _jitc->function->self.aoff) + 15) & -16;

    /* First lowest unused register is first output register */
    for (reg = _R115; reg >= _R40; reg--) {
	if (jit_regset_tstbit(&_jitc->function->regset, reg))
	    break;
    }
    _jitc->breg = rn(reg) + 1;
    _jitc->rout = _jitc->breg + 5;
    ruse = _jitc->rout - GR_32;

    /* How many out argument registers required? */
    if (!_jitc->function->define_frame) {
	for (reg = _OUT7; reg >= _OUT0; --reg) {
	    if (jit_regset_tstbit(&_jitc->function->regset, reg))
		break;
	}
	rout = (reg + 1) - _OUT0;
    }
    else
	rout = 8;

    /* Do not know if will call div/mod functions (sqrt* needs one) */
    if (rout < 2)
	rout = 2;

    /* Match gcc prolog */
    ALLOC(_jitc->breg + 1, ruse, rout);
    MOV(_jitc->breg + 2, GR_12);
    MOV_rn_br(_jitc->breg, BR_0);
    MOV(_jitc->breg + 3, GR_1);

    /* lightning specific, use r4 as frame pointer */
    MOV(_jitc->breg + 4, GR_4);
    addi(GR_4, GR_12, -(stack_framesize + params_offset));

    /* adjust stack pointer */
    addi(GR_12, GR_12, -(stack_framesize +
			 (params_offset << 1) + _jitc->function->stack));

    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F0))
	STF_SPILL(GR_4, rn(JIT_F0));
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F1)) {
	addi(GR_2, GR_4, 16);
	STF_SPILL(GR_2, rn(JIT_F1));
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F2)) {
	addi(GR_2, GR_4, 32);
	STF_SPILL(GR_2, rn(JIT_F2));
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F3)) {
	addi(GR_2, GR_4, 48);
	STF_SPILL(GR_2, rn(JIT_F3));
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F4)) {
	addi(GR_2, GR_4, 64);
	STF_SPILL(GR_2, rn(JIT_F4));
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F5)) {
	addi(GR_2, GR_4, 80);
	STF_SPILL(GR_2, rn(JIT_F5));
    }

    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, GR_4, rn(reg));
	jit_unget_reg(reg);
    }

    if (_jitc->function->self.call & jit_call_varargs) {
	for (reg = _jitc->function->vagp; reg < 8; ++reg)
	    stxi(112 + reg * 8, GR_4, GR_32 + reg);
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    if (_jitc->function->assume_frame)
	return;
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F0))
	LDF_FILL(rn(JIT_F0), GR_4);
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F1)) {
	addi(GR_2, GR_4, 16);
	LDF_FILL(rn(JIT_F1), GR_2);
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F2)) {
	addi(GR_2, GR_4, 32);
	LDF_FILL(rn(JIT_F2), GR_2);
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F3)) {
	addi(GR_2, GR_4, 48);
	LDF_FILL(rn(JIT_F3), GR_2);
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F4)) {
	addi(GR_2, GR_4, 64);
	LDF_FILL(rn(JIT_F4), GR_2);
    }
    if (jit_regset_tstbit(&_jitc->function->regset, JIT_F5)) {
	addi(GR_2, GR_4, 80);
	LDF_FILL(rn(JIT_F5), GR_2);
    }
    /* Match gcc epilog */
    MOV(GR_1, _jitc->breg + 3);
    MOV_I_ar_rn(AR_PFS, _jitc->breg + 1);
    MOV_br_rn(BR_0, _jitc->breg);
    MOV(GR_12, _jitc->breg + 2);
    /* Restore lightning specific r4 as frame pointer */
    MOV(GR_4, _jitc->breg + 4);
    BR_RET(BR_0);
    flush();
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Initialize va_list to the first stack argument. */
    if (_jitc->function->vagp < 8)
	addi(r0, GR_4, 112 + _jitc->function->vagp * 8);
    else
	addi(r0, GR_4, _jitc->function->self.size);
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Load argument. */
    ldr(r0, r1);
    /* Update va_list. */
    addi(r1, r1, 8);
}

static void
_patch_at(jit_state_t *_jit, jit_code_t code,
	  jit_word_t instr, jit_word_t label)
{
    jit_word_t		 l, h, tm, s0, s1, s2;
    union {
	jit_word_t	 w;
	jit_word_t	*p;
    } c;
    jit_word_t		 i1, i41, i20, ic, i5, i9, i7;
    c.w = instr;
    get_bundle(c.p, l, h, tm, s0, s1, s2);
    switch (code) {
	case jit_code_movi:
	    /* Handle jit functions as C function, so that jit function
	     * pointers can be passed to C code, and jit code does not
	     * need to try to differentiate them. */
	case jit_code_calli:
	    i1  = (label >> 63) &           0x1L;
	    i41 = (label >> 22) & 0x1ffffffffffL;
	    ic  = (label >> 21) &           0x1L;
	    i5  = (label >> 16) &          0x1fL;
	    i9  = (label >>  7) &         0x1ffL;
	    i7  =  label        &          0x7fL;
	    s1 = i41;
	    assert((tm & ~1) == TM_M_L_X_ &&
		   (s2 & 0xfL<<37) == (6L<<37) &&
		   s0 == nop_m);
	    s2 &= (6L<<37)|(0x7fL<<6);
	    s2 |= (i1<<36)|(i9<<27)|(i5<<22)|(ic<<21)|(i7<<13);
	    break;
	case jit_code_jmpi:
	    if (_jitc->jump) {
		/* kludge to hide function descriptors; check that gp
		 * is zero, what is done for all jit functions */
		if (((long *)label)[1] == 0) {
		    for (ic = 0; ic < _jitc->prolog.offset; ic++) {
			if (_jitc->prolog.ptr[ic] == label) {
			    label += 16;
			    break;
			}
		    }
		}
	    }
	    ic = (label - instr) >> 4;
	    i1  = (ic >> 61) &           0x1L;
	    i41 = (ic >> 22) & 0x1ffffffffffL;
	    i20 =  ic        &       0xfffffL;
	    assert((tm & ~1) == TM_M_L_X_ &&
		   (s2 & 0xfL<<37) == (0xcL<<37) &&
		   s0 == nop_m);
	    s1 = i41;
	    s2 &= (0xcL<<37)|(0x7L<<33)|(1L<<12);
	    s2 |= (i1<<36)|(i20<<13);
	    break;
	default:
	    /* Only B1 in slot 0 expected due to need to either
	     * a stop to update predicates, or a sync before
	     * unconditional short branch */
	    ic = (label - instr) >> 4;
	    assert((s0 >> 37) == 4 && (s0 & (7 << 6)) == 0);
	    s0 &= (4L<<37)|(7L<<33)|(1L<<12)|0x1f;
	    s0 |= (((ic>>20)&1L)<<36)|((ic&0xfffffL)<<13);
	    break;
    }
    set_bundle(c.p, l, h, tm, s0, s1, s2);
}
#endif
