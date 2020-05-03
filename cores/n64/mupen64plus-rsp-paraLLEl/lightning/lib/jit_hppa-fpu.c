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

/* FIXME should actually be hw model/version/etc or other constraint
 * that causes a SIGSEGV/SIGILL if using these instructions */
#if 1 //defined(__hpux)
#  define FSTXR				0
#  define FLDXR				0
#else
#  define FSTXR				1
#  define FLDXR				1
#endif

#define f39(o,b,x,t)			_f39(_jit,o,b,x,t)
static void _f39(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f40(o,b,x,r)			_f40(_jit,o,b,x,r)
static void _f40(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t)
    maybe_unused;
#define f41(o,b,x,t)			_f41(_jit,o,b,x,t)
static void _f41(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f42(o,b,i,r)			_f42(_jit,o,b,i,r)
static void _f42(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f43(o,b,t,i)			f1(o,b,t,i)
#define f45(o,r,a,b,fmt,c,d,e,t)	_f45(_jit,o,r,a,b,fmt,c,d,e,t)
static void _f45(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f46(o,r,a,s,df,sf,b,c,d,t)	_f46(_jit,o,r,a,s,df,sf,b,c,d,t)
static void _f46(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f47(o,r1,r2,a,fmt,b,c,d,t)	f47_48(o,r1,r2,a,fmt,b,c,d,t)
#define f48(o,r1,r2,a,fmt,b,c,d,t)	f47_48(o,r1,r2,a,fmt,b,c,d,t)
#define f47_48(o,r1,r2,y,fmt,b,c,d,t)	_f47_48(_jit,o,r1,r2,y,fmt,b,c,d,t)
static void _f47_48(jit_state_t*,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f49(o,r,a,b,c,f,d,e,g,h,i,t)	f49_52(o,r,a,b,c,f,d,e,g,h,i,t)
#define f51(o,r1,r2,y,a,f,b,d,e,g,h,c)	f49_52(o,r1,r2,y,a,f,b,d,e,g,h,c)
#define f52(o,r1,r2,a,b,f,c,d,e,g,h,t)	f49_52(o,r1,r2,a,b,f,c,d,e,g,h,t)
#define f49_52(o,r1,r2,y,v,f,a,b,u,c,d,t) _f49_52(_jit,o,r1,r2,y,v,f,a,b,u,c,d,t)
static void _f49_52(jit_state_t*,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f53(o,r1,r2,ta,ra,f,tm)		_f53(_jit,o,r1,r2,ta,ra,f,tm)
static void _f53(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f54(o,r1,r2,a,b,f,c,d,e,g,t)	_f54(_jit,o,r1,r2,a,b,f,c,d,e,g,t)
static void _f54(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define FABS_(f,r,t)			f49(0xe,r,0,3,0,f,0,0,0,0,0,t)
#define FABS_S(r,t)			FABS_(0,r,t)
#define FABS_D(r,t)			FABS_(1,r,t)
#define FABS_Q(r,t)			f45(0xc,r,0,3,3,0,0,0,t)
#define FADD_(f,r1,r2,t)		f52(0xe,r1,r2,0,0,f,3,0,0,0,0,t)
#define FADD_S(r1,r2,t)			FADD_(0,r1,r2,t)
#define FADD_D(r1,r2,t)			FADD_(1,r1,r2,t)
#define FADD_Q(r1,r2,t)			f48(0xc,r1,r2,0,3,3,0,0,t)
#define FPSR_GT				1
#define FPSR_LT				2
#define FPSR_EQ				4
#define FPSR_UN				8
/*
    Actually these are reversed, but easier for the FTEST after the FCMP
    fcmp,dbl,false? fr4,fr12		0
    fcmp,dbl,false fr4,fr12		1
    fcmp,dbl,? fr4,fr12			2
    fcmp,dbl,!<=> fr4,fr12		3		ORD
    fcmp,dbl,= fr4,fr12			4		NE
    fcmp,dbl,=t fr4,fr12		5
    fcmp,dbl,?= fr4,fr12		6
    fcmp,dbl,!<> fr4,fr12		7		LTGT
    fcmp,dbl,!?>= fr4,fr12		8
    fcmp,dbl,< fr4,fr12			9		UNGE
    fcmp,dbl,?< fr4,fr12		10
    fcmp,dbl,!>= fr4,fr12		11		GE
    fcmp,dbl,!?> fr4,fr12		12
    fcmp,dbl,<= fr4,fr12		13		UNGT
    fcmp,dbl,?<= fr4,fr12		14
    fcmp,dbl,!> fr4,fr12		15		GT
    fcmp,dbl,!?<= fr4,fr12		16
    fcmp,dbl,> fr4,fr12			17		UNLE
    fcmp,dbl,?> fr4,fr12		18
    fcmp,dbl,!<= fr4,fr12		19		LE
    fcmp,dbl,!?< fr4,fr12		20
    fcmp,dbl,>= fr4,fr12		21		UNLT
    fcmp,dbl,?>= fr4,fr12		22
    fcmp,dbl,!< fr4,fr12		23		LT
    fcmp,dbl,!?= fr4,fr12		24
    fcmp,dbl,<> fr4,fr12		25		UNEQ
    fcmp,dbl,!= fr4,fr12		26		EQ
    fcmp,dbl,!=t fr4,fr12		27
    fcmp,dbl,!? fr4,fr12		28
    fcmp,dbl,<=> fr4,fr12		29		UNORD
    fcmp,dbl,true? fr4,fr12		30
    fcmp,dbl,true fr4,fr12		31
 */
#define FCMP_LT				23
#define FCMP_LE				19
#define FCMP_EQ				26
#define FCMP_GE				11
#define FCMP_GT				15
#define FCMP_NE				4
#define FCMP_UNLT			21
#define FCMP_UNLE			17
#define FCMP_UNEQ			25
#define FCMP_UNGE			9
#define FCMP_UNGT			13
#define FCMP_LTGT			7
#define FCMP_ORD			3
#define FCMP_UNORD			29
#define FCMP_(f,r1,r2,c)		f51(0xe,r1,r2,0,0,f,2,0,0,0,0,c)
#define FCMP_S_(r1,r2,c)		FCMP_(0,r1,r2,c)
#define FCMP_D_(r1,r2,c)		FCMP_(1,r1,r2,c)
#define FCMP_Q_(r1,r2,c)		f47(0xc,r1,r2,0,3,2,0,0,c)
#define FCMP_S_LT(r1,r2)		FCMP_S_(r1,r2,FCMP_LT)
#define FCMP_D_LT(r1,r2)		FCMP_D_(r1,r2,FCMP_LT)
#define FCMP_Q_LT(r1,r2)		FCMP_Q_(r1,r2,FCMP_LT)
#define FCMP_S_LE(r1,r2)		FCMP_S_(r1,r2,FCMP_LE)
#define FCMP_D_LE(r1,r2)		FCMP_D_(r1,r2,FCMP_LE)
#define FCMP_Q_LE(r1,r2)		FCMP_Q_(r1,r2,FCMP_LE)
#define FCMP_S_EQ(r1,r2)		FCMP_S_(r1,r2,FCMP_EQ)
#define FCMP_D_EQ(r1,r2)		FCMP_D_(r1,r2,FCMP_EQ)
#define FCMP_Q_EQ(r1,r2)		FCMP_Q_(r1,r2,FCMP_EQ)
#define FCMP_S_GE(r1,r2)		FCMP_S_(r1,r2,FCMP_GE)
#define FCMP_D_GE(r1,r2)		FCMP_D_(r1,r2,FCMP_GE)
#define FCMP_Q_GE(r1,r2)		FCMP_Q_(r1,r2,FCMP_GE)
#define FCMP_S_GT(r1,r2)		FCMP_S_(r1,r2,FCMP_GT)
#define FCMP_D_GT(r1,r2)		FCMP_D_(r1,r2,FCMP_GT)
#define FCMP_Q_GT(r1,r2)		FCMP_Q_(r1,r2,FCMP_GT)
#define FCMP_S_NE(r1,r2)		FCMP_S_(r1,r2,FCMP_NE)
#define FCMP_D_NE(r1,r2)		FCMP_D_(r1,r2,FCMP_NE)
#define FCMP_Q_NE(r1,r2)		FCMP_Q_(r1,r2,FCMP_NE)
#define FCMP_S_UNLT(r1,r2)		FCMP_S_(r1,r2,FCMP_UNLT)
#define FCMP_D_UNLT(r1,r2)		FCMP_D_(r1,r2,FCMP_UNLT)
#define FCMP_Q_UNLT(r1,r2)		FCMP_Q_(r1,r2,FCMP_UNLT)
#define FCMP_S_UNLE(r1,r2)		FCMP_S_(r1,r2,FCMP_UNLE)
#define FCMP_D_UNLE(r1,r2)		FCMP_D_(r1,r2,FCMP_UNLE)
#define FCMP_Q_UNLE(r1,r2)		FCMP_Q_(r1,r2,FCMP_UNLE)
#define FCMP_S_UNEQ(r1,r2)		FCMP_S_(r1,r2,FCMP_UNEQ)
#define FCMP_D_UNEQ(r1,r2)		FCMP_D_(r1,r2,FCMP_UNEQ)
#define FCMP_Q_UNEQ(r1,r2)		FCMP_Q_(r1,r2,FCMP_UNEQ)
#define FCMP_S_UNGE(r1,r2)		FCMP_S_(r1,r2,FCMP_UNGE)
#define FCMP_D_UNGE(r1,r2)		FCMP_D_(r1,r2,FCMP_UNGE)
#define FCMP_Q_UNGE(r1,r2)		FCMP_Q_(r1,r2,FCMP_UNGE)
#define FCMP_S_UNGT(r1,r2)		FCMP_S_(r1,r2,FCMP_UNGT)
#define FCMP_D_UNGT(r1,r2)		FCMP_D_(r1,r2,FCMP_UNGT)
#define FCMP_Q_UNGT(r1,r2)		FCMP_Q_(r1,r2,FCMP_UNGT)
#define FCMP_S_LTGT(r1,r2)		FCMP_S_(r1,r2,FCMP_LTGT)
#define FCMP_D_LTGT(r1,r2)		FCMP_D_(r1,r2,FCMP_LTGT)
#define FCMP_Q_LTGT(r1,r2)		FCMP_Q_(r1,r2,FCMP_LTGT)
#define FCMP_S_ORD(r1,r2)		FCMP_S_(r1,r2,FCMP_ORD)
#define FCMP_D_ORD(r1,r2)		FCMP_D_(r1,r2,FCMP_ORD)
#define FCMP_Q_ORD(r1,r2)		FCMP_Q_(r1,r2,FCMP_ORD)
#define FCMP_S_UNORD(r1,r2)		FCMP_S_(r1,r2,FCMP_UNORD)
#define FCMP_D_UNORD(r1,r2)		FCMP_D_(r1,r2,FCMP_UNORD)
#define FCMP_Q_UNORD(r1,r2)		FCMP_Q_(r1,r2,FCMP_UNORD)
#define XFNVFF(s,d,r,t)			f46(0xc,r,0,0,d,s,1,0,0,t)
#define FCNVFF_Q_S(r,t)			XFNVFF(3,0,r,t)
#define FCNVFF_Q_D(r,t)			XFNVFF(3,1,r,t)
#define FCNVFF_S_Q(r,t)			XFNVFF(0,3,r,t)
#define FCNVFF_D_Q(r,t)			XFNVFF(1,3,r,t)
#define FCNVFF_(s,d,r,t)		f46(0xc,r,0,0,d,s,1,0,0,t)
#define FCNVFF_S_D(r,t)			FCNVFF_(0,1,r,t)
#define FCNVFF_D_S(r,t)			FCNVFF_(1,0,r,t)
#define FCNVXF_(s,d,r,t)		f46(0xc,r,0,1,d,s,1,0,0,t)
#define FCNVXF_S_S(r,t)			FCNVXF_(0,0,r,t)
#define FCNVXF_S_D(r,t)			FCNVXF_(0,1,r,t)
#define FCNVXT_(s,d,r,t)		f46(0xc,r,0,3,d,s,1,0,0,t)
#define FCNVXT_S_S(r,t)			FCNVXT_(0,0,r,t)
#define FCNVXT_D_S(r,t)			FCNVXT_(1,0,r,t)
#define FCPY_(f,r,t)			f49(0xe,r,0,2,0,f,0,0,0,0,0,t)
#define FCPY_S(r,t)			FCPY_(0,r,t)
#define FCPY_D(r,t)			FCPY_(1,r,t)
#define FCPY_Q(r,t)			f45(0xc,r,0,2,2,0,0,0,t)
#define FDIV_(f,r1,r2,t)		f52(0xe,r1,r2,3,0,f,3,0,0,0,0,t)
#define FDIV_S(r1,r2,t)			FDIV_(0,r1,r2,t)
#define FDIV_D(r1,r2,t)			FDIV_(1,r1,r2,t)
#define FDIV_Q(r1,r2,t)			f48(0xc,r1,r2,3,3,3,0,0,t)
#define FID()				f45(0xc,0,0,0,2,0,0,0,0)
#define FLDDL(i,b,t)			f3(0x14,b,t,i,1)
#define FLDD(x,b,t)			f39(0xb,b,x,t)
#define FLDDI(i,b,t)			f41(0xb,b,i,t)
#define FLDWL(i,b,t)			f43(0x17,b,t,i)
#define FLDW(x,b,t)			f39(0x9,b,x,t)
#define FLDWI(i,b,t)			f41(0x9,b,i,t)
#define FMPY_(f,r1,r2,t)		f52(0xe,r1,r2,2,0,f,3,0,0,0,0,t)
#define FMPY_S(r1,r2,t)			FMPY_(0,r1,r2,t)
#define FMPY_D(r1,r2,t)			FMPY_(1,r1,r2,t)
#define FMPY_Q(r1,r2,t)			f48(0xc,r1,r2,2,3,3,0,0,t)
/* FIXME not disassembled */
#define FMPYADD_(f,r1,r2,ta,ra,tm)	f53(0x6,r1,r2,ta,ra,f,tm)
#define FMPYADD_S(r1,r2,ta,ra,tm)	FMPYADD_(0,r1,r2,ta,ra,tm)
#define FMPYADD_D(r1,r2,ta,ra,tm)	FMPYADD_(1,r1,r2,ta,ra,tm)
#define FMPYFADD_(f,r1,r2,ra,t)		f54(0x2e,r1,r2,ra>>3,0,f,(ra)&7,0,0,0,t)
#define FMPYFADD_S(r1,r2,ra,t)		FMPYFADD_(0,r1,r2,ra,t)
#define FMPYFADD_D(r1,r2,ra,t)		FMPYFADD_(1,r1,r2,ra,t)
#define FMPYNFADD_(f,r1,r2,ra,t)	f54(0x2e,r1,r2,ra>>3,0,f,(ra)&7,0,0,1,t)
#define FMPYNFADD_S(r1,r2,ra,t)		FMPYNFADD_(0,r1,r2,ra,t)
#define FMPYNFADD_D(r1,r2,ra,t)		FMPYNFADD_(1,r1,r2,ra,t)
#define FMPYSUB_(f,r1,r2,ta,ra,tm)	f53(0x26,r1,r2,ta,ra,f,tm)
#define FMPYSUB_S(r1,r2,ta,ra,tm)	FMPYSUB_(0,r1,r2,ta,ra,tm)
#define FMPYSUB_D(r1,r2,ta,ra,tm)	FMPYSUB_(1,r1,r2,ta,ra,tm)
#define FNEG_(f,r,t)			f49(0xe,r,0,6,0,f,0,0,0,0,0,t)
#define FNEG_S(r,t)			FNEG_(0,r,t)
#define FNEG_D(r,t)			FNEG_(1,r,t)
/* FIXME not disassembled */
#define FNEG_Q(r,t)			f45(0xc,r,0,6,3,0,0,0,t)
#define FNEGABS_(f,r,t)			f49(0xe,r,0,7,0,f,0,0,0,0,0,t)
#define FNEGABS_S(r,t)			FNEGABS_(0,r,t)
#define FNEGABS_D(r,t)			FNEGABS_(1,r,t)
#define FNEGABS_Q(r,t)			f45(0xc,r,0,7,3,0,0,0,t)
#define FRND_(f,r,t)			f49(0xe,r,0,5,0,f,0,0,0,0,0,t)
#define FRND_S(r,t)			FRND_(0,r,t)
#define FRND_D(r,t)			FRND_(1,r,t)
#define FRND_Q(r,t)			f45(0xc,r,0,5,3,0,0,0,t)
#define FSQRT_(f,r,t)			f49(0xe,r,0,4,0,f,0,0,0,0,0,t)
#define FSQRT_S(r,t)			FSQRT_(0,r,t)
#define FSQRT_D(r,t)			FSQRT_(1,r,t)
#define FSQRT_Q(r,t)			f45(0xc,r,0,4,3,0,0,0,t)
#define FSTDL(r,i,b)			f3(0x1c,b,r,i,1)
#define FSTD(r,x,b)			f40(0xb,b,x,r)
#define FSTDI(r,i,b)			f42(0xb,b,i,r)
#define FSTWL(r,i,b)			f43(0x1f,b,r,i)
#define FSTW(r,x,b)			f40(0x9,b,x,r)
#define FSTWI(r,i,b)			f42(0x9,b,i,r)
#define FSUB_(f,r1,r2,t)		f52(0xe,r1,r2,1,0,f,3,0,0,0,0,t)
#define FSUB_S(r1,r2,t)			FSUB_(0,r1,r2,t)
#define FSUB_D(r1,r2,t)			FSUB_(1,r1,r2,t)
#define FSUB_Q(r1,r2,t)			f48(0xc,r1,r2,1,3,3,0,0,t)
#define FTEST_(c)			f47(0xc,0,0,0,0,2,0,1,c)
#define FTEST()				f47(0xc,0,0,1,0,2,0,1,0)
#define FTEST_LT()			FTEST_(FCMP_LT)
#define FTEST_LE()			FTEST_(FCMP_LE)
#define FTEST_EQ()			FTEST_(FCMP_EQ)
#define FTEST_GE()			FTEST_(FCMP_GE)
#define FTEST_GT()			FTEST_(FCMP_GT)
#define FTEST_NE()			FTEST_(FCMP_NE)
#define FTEST_UNLT()			FTEST_(FCMP_UNLT)
#define FTEST_UNLE()			FTEST_(FCMP_UNLE)
#define FTEST_UNEQ()			FTEST_(FCMP_UNEQ)
#define FTEST_UNGE()			FTEST_(FCMP_UNGE)
#define FTEST_UNGT()			FTEST_(FCMP_UNGT)
#define FTEST_LTGT()			FTEST_(FCMP_LTGT)
#define FTEST_ORD()			FTEST_(FCMP_ORD)
#define FTEST_UNORD()			FTEST_(FCMP_UNORD)
#define XMPYU(r1,r2,t)			f52(0xe,r1,r2,2,0,0,3,1,0,0,0,t)
#define XMPYU_L_R(r1,r2,t)		f52(0xe,r1,r2,2,1,0,3,1,0,0,0,t)
#define XMPYU_R_L(r1,r2,t)		f52(0xe,r1,r2,2,0,0,3,1,1,0,0,t)
#define XMPYU_R_R(r1,r2,t)		f52(0xe,r1,r2,2,1,0,3,1,1,0,0,t)
#define negr_f(r0,r1)			FNEG_S(r1,r0)
#define negr_d(r0,r1)			FNEG_D(r1,r0)
#define sqrtr_f(r0,r1)			FSQRT_S(r1,r0)
#define sqrtr_d(r0,r1)			FSQRT_D(r1,r0)
#define extr_f(r0,r1)			_extr_f(_jit,r0,r1)
static void _extr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#define extr_d(r0,r1)			_extr_d(_jit,r0,r1)
static void _extr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#define extr_f_d(r0,r1)			FCNVFF_S_D(r1,r0)
#define extr_d_f(r0,r1)			FCNVFF_D_S(r1,r0)
#define truncr_f(r0,r1)			truncr_f_i(r0,r1)
#define truncr_f_i(r0,r1)		_truncr_f_i(_jit,r0,r1)
static void _truncr_f_i(jit_state_t*,jit_int32_t,jit_int32_t);
#define truncr_d(r0,r1)			truncr_d_i(r0,r1)
#define truncr_d_i(r0,r1)		_truncr_d_i(_jit,r0,r1)
static void _truncr_d_i(jit_state_t*,jit_int32_t,jit_int32_t);
#define movr_f(r0,r1)			FCPY_S(r1,r0)
#define movi_f(r0,i0)			_movi_f(_jit,r0,i0)
static void _movi_f(jit_state_t*,jit_int32_t,jit_float32_t*);
#define movr_d(r0,r1)			FCPY_D(r1,r0)
#define movi_d(r0,i0)			_movi_d(_jit,r0,i0)
static void _movi_d(jit_state_t*,jit_int32_t,jit_float64_t*);
#define absr_f(r0,r1)			FABS_S(r1,r0)
#define absr_d(r0,r1)			FABS_D(r1,r0)
#define addr_f(r0,r1,r2)		FADD_S(r1,r2,r0)
#define addi_f(r0,r1,i0)		_addi_f(_jit,r0,r1,i0)
static void _addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define addr_d(r0,r1,r2)		FADD_D(r1,r2,r0)
#define addi_d(r0,r1,i0)		_addi_d(_jit,r0,r1,i0)
static void _addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define subr_f(r0,r1,r2)		FSUB_S(r1,r2,r0)
#define subi_f(r0,r1,i0)		_subi_f(_jit,r0,r1,i0)
static void _subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define subr_d(r0,r1,r2)		FSUB_D(r1,r2,r0)
#define subi_d(r0,r1,i0)		_subi_d(_jit,r0,r1,i0)
static void _subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define rsbr_f(r0,r1,r2)		subr_f(r0,r2,r1)
#define rsbi_f(r0,r1,i0)		_rsbi_f(_jit,r0,r1,i0)
static void _rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define rsbr_d(r0,r1,r2)		subr_d(r0,r2,r1)
#define rsbi_d(r0,r1,i0)		_rsbi_d(_jit,r0,r1,i0)
static void _rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define mulr_f(r0,r1,r2)		FMPY_S(r1,r2,r0)
#define muli_f(r0,r1,i0)		_muli_f(_jit,r0,r1,i0)
static void _muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define mulr_d(r0,r1,r2)		FMPY_D(r1,r2,r0)
#define muli_d(r0,r1,i0)		_muli_d(_jit,r0,r1,i0)
static void _muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define divr_f(r0,r1,r2)		FDIV_S(r1,r2,r0)
#define divi_f(r0,r1,i0)		_divi_f(_jit,r0,r1,i0)
static void _divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#define divr_d(r0,r1,r2)		FDIV_D(r1,r2,r0)
#define divi_d(r0,r1,i0)		_divi_d(_jit,r0,r1,i0)
static void _divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#define cmpr_f(c,r0,r1,r2)		_cmpr_f(_jit,c,r0,r1,r2)
static void _cmpr_f(jit_state_t*,jit_word_t,
		    jit_int32_t,jit_int32_t,jit_int32_t);
#define cmpi_f(c,r0,r1,i0)		_cmpi_f(_jit,c,r0,r1,i0)
static void _cmpi_f(jit_state_t*,jit_word_t,
		    jit_int32_t,jit_int32_t,jit_float32_t*);
#define cmpr_d(c,r0,r1,r2)		_cmpr_d(_jit,c,r0,r1,r2)
static void _cmpr_d(jit_state_t*,jit_word_t,
		    jit_int32_t,jit_int32_t,jit_int32_t);
#define cmpi_d(c,r0,r1,i0)		_cmpi_d(_jit,c,r0,r1,i0)
static void _cmpi_d(jit_state_t*,jit_word_t,
		    jit_int32_t,jit_int32_t,jit_float64_t*);
#define ltr_f(r0,r1,r2)			cmpr_f(FCMP_LT,r0,r1,r2)
#define lti_f(r0,r1,i0)			cmpi_f(FCMP_LT,r0,r1,i0)
#define ltr_d(r0,r1,r2)			cmpr_d(FCMP_LT,r0,r1,r2)
#define lti_d(r0,r1,i0)			cmpi_d(FCMP_LT,r0,r1,i0)
#define ler_f(r0,r1,r2)			cmpr_f(FCMP_LE,r0,r1,r2)
#define lei_f(r0,r1,i0)			cmpi_f(FCMP_LE,r0,r1,i0)
#define ler_d(r0,r1,r2)			cmpr_d(FCMP_LE,r0,r1,r2)
#define lei_d(r0,r1,i0)			cmpi_d(FCMP_LE,r0,r1,i0)
#define eqr_f(r0,r1,r2)			cmpr_f(FCMP_EQ,r0,r1,r2)
#define eqi_f(r0,r1,i0)			cmpi_f(FCMP_EQ,r0,r1,i0)
#define eqr_d(r0,r1,r2)			cmpr_d(FCMP_EQ,r0,r1,r2)
#define eqi_d(r0,r1,i0)			cmpi_d(FCMP_EQ,r0,r1,i0)
#define ger_f(r0,r1,r2)			cmpr_f(FCMP_GE,r0,r1,r2)
#define gei_f(r0,r1,i0)			cmpi_f(FCMP_GE,r0,r1,i0)
#define ger_d(r0,r1,r2)			cmpr_d(FCMP_GE,r0,r1,r2)
#define gei_d(r0,r1,i0)			cmpi_d(FCMP_GE,r0,r1,i0)
#define gtr_f(r0,r1,r2)			cmpr_f(FCMP_GT,r0,r1,r2)
#define gti_f(r0,r1,i0)			cmpi_f(FCMP_GT,r0,r1,i0)
#define gtr_d(r0,r1,r2)			cmpr_d(FCMP_GT,r0,r1,r2)
#define gti_d(r0,r1,i0)			cmpi_d(FCMP_GT,r0,r1,i0)
#define ner_f(r0,r1,r2)			cmpr_f(FCMP_NE,r0,r1,r2)
#define nei_f(r0,r1,i0)			cmpi_f(FCMP_NE,r0,r1,i0)
#define ner_d(r0,r1,r2)			cmpr_d(FCMP_NE,r0,r1,r2)
#define nei_d(r0,r1,i0)			cmpi_d(FCMP_NE,r0,r1,i0)
#define unltr_f(r0,r1,r2)		cmpr_f(FCMP_UNLT,r0,r1,r2)
#define unlti_f(r0,r1,i0)		cmpi_f(FCMP_UNLT,r0,r1,i0)
#define unltr_d(r0,r1,r2)		cmpr_d(FCMP_UNLT,r0,r1,r2)
#define unlti_d(r0,r1,i0)		cmpi_d(FCMP_UNLT,r0,r1,i0)
#define unler_f(r0,r1,r2)		cmpr_f(FCMP_UNLE,r0,r1,r2)
#define unlei_f(r0,r1,i0)		cmpi_f(FCMP_UNLE,r0,r1,i0)
#define unler_d(r0,r1,r2)		cmpr_d(FCMP_UNLE,r0,r1,r2)
#define unlei_d(r0,r1,i0)		cmpi_d(FCMP_UNLE,r0,r1,i0)
#define uneqr_f(r0,r1,r2)		cmpr_f(FCMP_UNEQ,r0,r1,r2)
#define uneqi_f(r0,r1,i0)		cmpi_f(FCMP_UNEQ,r0,r1,i0)
#define uneqr_d(r0,r1,r2)		cmpr_d(FCMP_UNEQ,r0,r1,r2)
#define uneqi_d(r0,r1,i0)		cmpi_d(FCMP_UNEQ,r0,r1,i0)
#define unger_f(r0,r1,r2)		cmpr_f(FCMP_UNGE,r0,r1,r2)
#define ungei_f(r0,r1,i0)		cmpi_f(FCMP_UNGE,r0,r1,i0)
#define unger_d(r0,r1,r2)		cmpr_d(FCMP_UNGE,r0,r1,r2)
#define ungei_d(r0,r1,i0)		cmpi_d(FCMP_UNGE,r0,r1,i0)
#define ungtr_f(r0,r1,r2)		cmpr_f(FCMP_UNGT,r0,r1,r2)
#define ungti_f(r0,r1,i0)		cmpi_f(FCMP_UNGT,r0,r1,i0)
#define ungtr_d(r0,r1,r2)		cmpr_d(FCMP_UNGT,r0,r1,r2)
#define ungti_d(r0,r1,i0)		cmpi_d(FCMP_UNGT,r0,r1,i0)
#define ltgtr_f(r0,r1,r2)		cmpr_f(FCMP_LTGT,r0,r1,r2)
#define ltgti_f(r0,r1,i0)		cmpi_f(FCMP_LTGT,r0,r1,i0)
#define ltgtr_d(r0,r1,r2)		cmpr_d(FCMP_LTGT,r0,r1,r2)
#define ltgti_d(r0,r1,i0)		cmpi_d(FCMP_LTGT,r0,r1,i0)
#define ordr_f(r0,r1,r2)		cmpr_f(FCMP_ORD,r0,r1,r2)
#define ordi_f(r0,r1,i0)		cmpi_f(FCMP_ORD,r0,r1,i0)
#define ordr_d(r0,r1,r2)		cmpr_d(FCMP_ORD,r0,r1,r2)
#define ordi_d(r0,r1,i0)		cmpi_d(FCMP_ORD,r0,r1,i0)
#define unordr_f(r0,r1,r2)		cmpr_f(FCMP_UNORD,r0,r1,r2)
#define unordi_f(r0,r1,i0)		cmpi_f(FCMP_UNORD,r0,r1,i0)
#define unordr_d(r0,r1,r2)		cmpr_d(FCMP_UNORD,r0,r1,r2)
#define unordi_d(r0,r1,i0)		cmpi_d(FCMP_UNORD,r0,r1,i0)
#define ldr_f(r0,r1)			FLDWI(0,r1,r0)
#define ldi_f(r0,i0)			_ldi_f(_jit,r0,i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#if FLDXR
#  define ldxr_f(r0,r1,r2)		FLDW(r2,r1,r0)
#  define ldxr_d(r0,r1,r2)		FLDD(r2,r1,r0)
#else
#define ldxr_f(r0,r1,r2)		_ldxr_f(_jit,r0,r1,r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxr_d(r0,r1,r2)		_ldxr_d(_jit,r0,r1,r2)
static void _ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#endif
#define ldxi_f(r0,r1,i0)		_ldxi_f(_jit,r0,r1,i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldr_d(r0,r1)			FLDDI(0,r1,r0)
#define ldi_d(r0,i0)			_ldi_d(_jit,r0,i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxi_d(r0,r1,i0)		_ldxi_d(_jit,r0,r1,i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define str_f(r0,r1)			FSTWI(r1,0,r0)
#define sti_f(i0,r0)			_sti_f(_jit,i0,r0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#if FSTXR
#  define stxr_f(r0,r1,r2)		FSTW(r2,r1,r0)
#  define stxr_d(r0,r1,r2)		FSTD(r2,r1,r0)
#else
#  define stxr_f(r0,r1,r2)		_stxr_f(_jit,r0,r1,r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxr_d(r0,r1,r2)		_stxr_d(_jit,r0,r1,r2)
static void _stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#endif
#define stxi_f(i0,r0,r1)		_stxi_f(_jit,i0,r0,r1)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define str_d(r0,r1)			FSTDI(r1,0,r0)
#define sti_d(i0,r0)			_sti_d(_jit,i0,r0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#define stxi_d(i0,r0,r1)		_stxi_d(_jit,i0,r0,r1)
static void _stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bcmpr_f(c,i0,r0,r1)		_bcmpr_f(_jit,c,i0,r0,r1)
static jit_word_t _bcmpr_f(jit_state_t*,jit_word_t,
			   jit_word_t,jit_int32_t,jit_int32_t);
#define bcmpi_f(c,i0,r0,i1)		_bcmpi_f(_jit,c,i0,r0,i1)
static jit_word_t _bcmpi_f(jit_state_t*,jit_word_t,
			   jit_word_t,jit_int32_t,jit_float32_t*);
#define bcmpr_d(c,i0,r0,r1)		_bcmpr_d(_jit,c,i0,r0,r1)
static jit_word_t _bcmpr_d(jit_state_t*,jit_word_t,
			   jit_word_t,jit_int32_t,jit_int32_t);
#define bcmpi_d(c,i0,r0,i1)		_bcmpi_d(_jit,c,i0,r0,i1)
static jit_word_t _bcmpi_d(jit_state_t*,jit_word_t,
			   jit_word_t,jit_int32_t,jit_float64_t*);
#define bltr_f(i0,r0,r1)		bcmpr_f(FCMP_LT,i0,r0,r1)
#define blti_f(i0,r0,i1)		bcmpi_f(FCMP_LT,i0,r0,i1)
#define bltr_d(i0,r0,r1)		bcmpr_d(FCMP_LT,i0,r0,r1)
#define blti_d(i0,r0,i1)		bcmpi_d(FCMP_LT,i0,r0,i1)
#define bler_f(i0,r0,r1)		bcmpr_f(FCMP_LE,i0,r0,r1)
#define blei_f(i0,r0,i1)		bcmpi_f(FCMP_LE,i0,r0,i1)
#define bler_d(i0,r0,r1)		bcmpr_d(FCMP_LE,i0,r0,r1)
#define blei_d(i0,r0,i1)		bcmpi_d(FCMP_LE,i0,r0,i1)
#define beqr_f(i0,r0,r1)		bcmpr_f(FCMP_EQ,i0,r0,r1)
#define beqi_f(i0,r0,i1)		bcmpi_f(FCMP_EQ,i0,r0,i1)
#define beqr_d(i0,r0,r1)		bcmpr_d(FCMP_EQ,i0,r0,r1)
#define beqi_d(i0,r0,i1)		bcmpi_d(FCMP_EQ,i0,r0,i1)
#define bger_f(i0,r0,r1)		bcmpr_f(FCMP_GE,i0,r0,r1)
#define bgei_f(i0,r0,i1)		bcmpi_f(FCMP_GE,i0,r0,i1)
#define bger_d(i0,r0,r1)		bcmpr_d(FCMP_GE,i0,r0,r1)
#define bgei_d(i0,r0,i1)		bcmpi_d(FCMP_GE,i0,r0,i1)
#define bgtr_f(i0,r0,r1)		bcmpr_f(FCMP_GT,i0,r0,r1)
#define bgti_f(i0,r0,i1)		bcmpi_f(FCMP_GT,i0,r0,i1)
#define bgtr_d(i0,r0,r1)		bcmpr_d(FCMP_GT,i0,r0,r1)
#define bgti_d(i0,r0,i1)		bcmpi_d(FCMP_GT,i0,r0,i1)
#define bner_f(i0,r0,r1)		bcmpr_f(FCMP_NE,i0,r0,r1)
#define bnei_f(i0,r0,i1)		bcmpi_f(FCMP_NE,i0,r0,i1)
#define bner_d(i0,r0,r1)		bcmpr_d(FCMP_NE,i0,r0,r1)
#define bnei_d(i0,r0,i1)		bcmpi_d(FCMP_NE,i0,r0,i1)
#define bunltr_f(i0,r0,r1)		bcmpr_f(FCMP_UNLT,i0,r0,r1)
#define bunlti_f(i0,r0,i1)		bcmpi_f(FCMP_UNLT,i0,r0,i1)
#define bunltr_d(i0,r0,r1)		bcmpr_d(FCMP_UNLT,i0,r0,r1)
#define bunlti_d(i0,r0,i1)		bcmpi_d(FCMP_UNLT,i0,r0,i1)
#define bunler_f(i0,r0,r1)		bcmpr_f(FCMP_UNLE,i0,r0,r1)
#define bunlei_f(i0,r0,i1)		bcmpi_f(FCMP_UNLE,i0,r0,i1)
#define bunler_d(i0,r0,r1)		bcmpr_d(FCMP_UNLE,i0,r0,r1)
#define bunlei_d(i0,r0,i1)		bcmpi_d(FCMP_UNLE,i0,r0,i1)
#define buneqr_f(i0,r0,r1)		bcmpr_f(FCMP_UNEQ,i0,r0,r1)
#define buneqi_f(i0,r0,i1)		bcmpi_f(FCMP_UNEQ,i0,r0,i1)
#define buneqr_d(i0,r0,r1)		bcmpr_d(FCMP_UNEQ,i0,r0,r1)
#define buneqi_d(i0,r0,i1)		bcmpi_d(FCMP_UNEQ,i0,r0,i1)
#define bunger_f(i0,r0,r1)		bcmpr_f(FCMP_UNGE,i0,r0,r1)
#define bungei_f(i0,r0,i1)		bcmpi_f(FCMP_UNGE,i0,r0,i1)
#define bunger_d(i0,r0,r1)		bcmpr_d(FCMP_UNGE,i0,r0,r1)
#define bungei_d(i0,r0,i1)		bcmpi_d(FCMP_UNGE,i0,r0,i1)
#define bungtr_f(i0,r0,r1)		bcmpr_f(FCMP_UNGT,i0,r0,r1)
#define bungti_f(i0,r0,i1)		bcmpi_f(FCMP_UNGT,i0,r0,i1)
#define bungtr_d(i0,r0,r1)		bcmpr_d(FCMP_UNGT,i0,r0,r1)
#define bungti_d(i0,r0,i1)		bcmpi_d(FCMP_UNGT,i0,r0,i1)
#define bltgtr_f(i0,r0,r1)		bcmpr_f(FCMP_LTGT,i0,r0,r1)
#define bltgti_f(i0,r0,i1)		bcmpi_f(FCMP_LTGT,i0,r0,i1)
#define bltgtr_d(i0,r0,r1)		bcmpr_d(FCMP_LTGT,i0,r0,r1)
#define bltgti_d(i0,r0,i1)		bcmpi_d(FCMP_LTGT,i0,r0,i1)
#define bordr_f(i0,r0,r1)		bcmpr_f(FCMP_ORD,i0,r0,r1)
#define bordi_f(i0,r0,i1)		bcmpi_f(FCMP_ORD,i0,r0,i1)
#define bordr_d(i0,r0,r1)		bcmpr_d(FCMP_ORD,i0,r0,r1)
#define bordi_d(i0,r0,i1)		bcmpi_d(FCMP_ORD,i0,r0,i1)
#define bunordr_f(i0,r0,r1)		bcmpr_f(FCMP_UNORD,i0,r0,r1)
#define bunordi_f(i0,r0,i1)		bcmpi_f(FCMP_UNORD,i0,r0,i1)
#define bunordr_d(i0,r0,r1)		bcmpr_d(FCMP_UNORD,i0,r0,r1)
#define bunordi_d(i0,r0,i1)		bcmpi_d(FCMP_UNORD,i0,r0,i1)
#define vaarg_d(r0, r1)			_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
static void
_f39(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t b, jit_int32_t x, jit_int32_t t)
{
    assert(!(o & ~0x3f));
    assert(!(b & ~0x1f));
    assert(!(x & ~0x1f));
    assert(!(t & ~0x1f));
    ii((o<<26)|(b<<21)|(x<<16)|t);
}

static void
_f40(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t b, jit_int32_t x, jit_int32_t r)
{
    assert(!(o & ~0x3f));
    assert(!(b & ~0x1f));
    assert(!(x & ~0x1f));
    assert(!(r & ~0x1f));
    ii((o<<26)|(b<<21)|(x<<16)|(1<<9)|r);
}

static void
_f41(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t b, jit_int32_t x, jit_int32_t t)
{
    assert(!(o & ~0x3f));
    assert(!(b & ~0x1f));
    assert(x >= -16 && x < 15);
    assert(!(t & ~0x1f));
    ii((o<<26)|(b<<21)|(low_sign_unext(x,5)<<16)|(1<<12)|t);
}

static void
_f42(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t b, jit_int32_t i, jit_int32_t r)
{
    assert(!(o & ~0x3f));
    assert(!(b & ~0x1f));
    assert(i >= -16 && i < 15);
    assert(!(r & ~0x1f));
    ii((o<<26)|(b<<21)|(low_sign_unext(i,5)<<16)|(1<<12)|(1<<9)|r);
}

static void
_f45(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t r, jit_int32_t a, jit_int32_t b, jit_int32_t fmt,
     jit_int32_t c, jit_int32_t d, jit_int32_t e, jit_int32_t t)
{
    assert(!(o   & ~0x3f));
    assert(!(r   & ~0x1f));
    assert(!(a   & ~0x1f));
    assert(!(b   &  ~0x7));
    assert(!(fmt &  ~0x3));
    assert(!(c   &  ~0x3));
    assert(!(d   &  ~0x7));
    assert(!(e   &  ~0x1));
    assert(!(t   & ~0x1f));
    ii((o<<26)|(r<<21)|(a<<16)|(fmt<<13)|(b<<11)|(c<<9)|(d<<6)|(e<<5)|t);
}

static void
_f46(jit_state_t *_jit, jit_int32_t o, jit_int32_t r,
     jit_int32_t a, jit_int32_t s, jit_int32_t df, jit_int32_t sf,
     jit_int32_t b, jit_int32_t c, jit_int32_t d, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(r  & ~0x1f));
    assert(!(a  &  ~0x7));
    assert(!(s  &  ~0x7));
    assert(!(df &  ~0x3));
    assert(!(sf &  ~0x3));
    assert(!(b  &  ~0x3));
    assert(!(c  &  ~0x7));
    assert(!(d  &  ~0x1));
    assert(!(t  & ~0x1f));
    ii((o<<26)|(r<<21)|(a<<18)|(s<<15)|
       (df<<13)|(sf<<11)|(b<<9)|(c<<6)|(d<<5)|t);
}

static void
_f47_48(jit_state_t *_jit, jit_int32_t o,
	jit_int32_t r2, jit_int32_t r1, jit_int32_t y, jit_int32_t fmt,
	jit_int32_t a, jit_int32_t b, jit_int32_t c, jit_int32_t t)
{
    assert(!(o   & ~0x3f));
    assert(!(r2  & ~0x1f));
    assert(!(r1  & ~0x1f));
    assert(!(y   &  ~0x7));
    assert(!(fmt &  ~0x3));
    assert(!(a   &  ~0x3));
    assert(!(b   &  ~0x7));
    assert(!(c   &  ~0x1));
    assert(!(t   & ~0x1f));
    ii((o<<26)|(r2<<21)|(r1<<16)|(y<<13)|(fmt<<11)|(a<<9)|(b<<6)|(c<<5)|t);
}

static void
_f49_52(jit_state_t *_jit, jit_int32_t o,
	jit_int32_t r1, jit_int32_t r2, jit_int32_t y,
	jit_int32_t v, jit_int32_t f, jit_int32_t a, jit_int32_t b,
	jit_int32_t u, jit_int32_t c, jit_int32_t d, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(r1 & ~0x1f));
    assert(!(r2 & ~0x3f));
    assert(!(y  &  ~0x7));
    assert(!(v  &  ~0x1));
    assert(!(f  &  ~0x1));
    assert(!(a  &  ~0x3));
    assert(!(b  &  ~0x1));
    assert(!(u  &  ~0x1));
    assert(!(c  &  ~0x1));
    assert(!(d  &  ~0x1));
    assert(!(t  & ~0x1f));
    ii((o<<26)|(r1<<21)|(r2<<16)|(y<<13)|(v<<12)|
       (f<<11)|(a<<9)|(b<<8)|(u<<7)|(c<<6)|(d<<5)|t);
}

static void
_f53(jit_state_t *_jit, jit_int32_t o, jit_int32_t r1, jit_int32_t r2,
     jit_int32_t ta, jit_int32_t ra, jit_int32_t f, jit_int32_t tm)
{
    assert(!(o  & ~0x3f));
    assert(!(r1 & ~0x1f));
    assert(!(r2 & ~0x1f));
    assert(!(ta & ~0x1f));
    assert(!(ra & ~0x1f));
    assert(!(f  &  ~0x1));
    assert(!(tm & ~0x1f));
    assert(ra != tm ||
	   (ta == r1 || ta == r2 || ta == tm) ||
	   (f && ra == 1) || (!f && !ra));
    ii((o<<26)|(r1<<21)|(r2<<16)|(ta<<11)|(ra<<6)|(f<<5)|tm);
}

static void
_f54(jit_state_t *_jit, jit_int32_t o, jit_int32_t r1, jit_int32_t r2,
     jit_int32_t a, jit_int32_t b, jit_int32_t f, jit_int32_t c,
     jit_int32_t d, jit_int32_t e, jit_int32_t g, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(r1 & ~0x1f));
    assert(!(r2 & ~0x1f));
    assert(!(a  &  ~0x7));
    assert(!(b  &  ~0x1));
    assert(!(f  &  ~0x1));
    assert(!(c  &  ~0x7));
    assert(!(e  &  ~0x1));
    assert(!(e  &  ~0x1));
    assert(!(g  &  ~0x1));
    assert(!(t  & ~0x1f));
    ii((o<<26)|(r1<<21)|(r2<<16)|(a<<13)|
       (b<<12)|(f<11)|(c<<8)|(d<<7)|(e<<6)|(g<<5)|t);
}

static void
_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    stxi(alloca_offset - 8, _FP_REGNO, r1);
    ldxi_f(r0, _FP_REGNO, alloca_offset - 8);
    FCNVXF_S_S(r0, r0);
}

static void
_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    stxi(alloca_offset - 8, _FP_REGNO, r1);
    ldxi_f(r0, _FP_REGNO, alloca_offset - 8);
    FCNVXF_S_D(r0, r0);
}

static void
_truncr_f_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    FCNVXT_S_S(r1, rn(reg));
    stxi_f(alloca_offset - 8, _FP_REGNO, rn(reg));
    ldxi(r0, _FP_REGNO, alloca_offset - 8);
    jit_unget_reg(reg);
}

static void
_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    FCNVXT_D_S(r1, rn(reg));
    stxi_d(alloca_offset - 8, _FP_REGNO, rn(reg));
    ldxi(r0, _FP_REGNO, alloca_offset - 8);
    jit_unget_reg(reg);
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
	movi(rn(reg), data.i);
	stxi_i(alloca_offset - 8, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
	ldxi_f(r0, _FP_REGNO, alloca_offset - 8);
    }
    else
	ldi_f(r0, (jit_word_t)i0);
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
	jit_int32_t	 ii[2];
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;

    data.d = *i0;
    if (_jitc->no_data) {
	data.d = *i0;
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.ii[0]);
	stxi_i(alloca_offset - 8, _FP_REGNO, rn(reg));
	movi(rn(reg), data.ii[1]);
	stxi_i(alloca_offset - 4, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
	ldxi_d(r0, _FP_REGNO, alloca_offset - 8);
    }
    else
	ldi_d(r0, (jit_word_t)i0);
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
#define fopi(name)			fpr_opi(name, f, 32)
#define dopi(name)			fpr_opi(name, d, 64)

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
_cmpr_f(jit_state_t *_jit, jit_word_t c,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    LDI(0, r0);
    FCMP_S_(r1, r2, c);
    FTEST();
    LDI(1, r0);
}

static void
_cmpi_f(jit_state_t *_jit, jit_word_t c,
	jit_int32_t r0, jit_int32_t r1, jit_float32_t *i0)
{
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);
    movi_f(rn(reg), i0);
    cmpr_f(c, r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_cmpr_d(jit_state_t *_jit, jit_word_t c,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    LDI(0, r0);
    FCMP_D_(r1, r2, c);
    FTEST();
    LDI(1, r0);
}

static void
_cmpi_d(jit_state_t *_jit, jit_word_t c,
	jit_int32_t r0, jit_int32_t r1, jit_float64_t *i0)
{
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);
    movi_d(rn(reg), i0);
    cmpr_d(c, r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(!(i0 & 3));
    if (i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 6))
	FLDWL(i0, _R0_REGNO, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

#if !FLDXR
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
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_d(r0, rn(reg));
    jit_unget_reg(reg);
}
#endif

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	FLDWI(i0, r1, r0);
    /* |im11a|0|t|i| */
    else if (FLDXR && i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 6))
	FLDWL(i0, r1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_f(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(!(i0 & 7));
    if (i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 14))
	FLDDL(i0, _R0_REGNO, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	FLDDI(i0, r1, r0);
    /* |im10a|m|a|1|i| */
    else if (FLDXR && i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 14))
	FLDDL(i0, r1, r0);
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
    jit_int32_t		reg;
    assert(!(i0 & 3));
    if (i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 6))
	FSTWL(r0, i0, _R0_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_f(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

#if !FSTXR
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
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_d(rn(reg), r2);
    jit_unget_reg(reg);
}
#endif

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	FSTWI(r1, i0, r0);
    /* |im11a|0|t|i| */
    else if (FSTXR && i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 6))
	FSTWL(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
#if FSTXR
	movi(rn(reg), i0);
	stxr_f(rn(reg), r0, r1);
#else
	addi(rn(reg), r0, i0);
	str_f(rn(reg), r1);
#endif
	jit_unget_reg(reg);
    }
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    assert(!(i0 & 7));
    if (i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 14))
	FSTDL(r0, i0, _R0_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_d(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	FSTDI(r1, i0, r0);
    /* |im10a|m|a|1|i| */
    else if (FSTXR && i0 >= -8192 && i0 <= 8191 && !(re_assemble_16(i0) & 14))
	FSTDL(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
#if FSTXR
	movi(rn(reg), i0);
	stxr_d(rn(reg), r0, r1);
#else
	addi(rn(reg), r0, i0);
	str_d(rn(reg), r1);
#endif
	jit_unget_reg(reg);
    }
}

static jit_word_t
_bcmpr_f(jit_state_t *_jit, jit_word_t c,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_S_(r0, r1, c);
    FTEST();
    w = _jit->pc.w;
    B_N(((i0 - w) >> 2) - 2, _R0_REGNO);
    NOP();
    return (w);
}

static jit_word_t
_bcmpi_f(jit_state_t *_jit, jit_word_t c,
	 jit_word_t i0, jit_int32_t r0, jit_float32_t *i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi_f(rn(reg), i1);
    FCMP_S_(r0, rn(reg), c);
    FTEST();
    w = _jit->pc.w;
    B_N(((i0 - w) >> 2) - 2, _R0_REGNO);
    NOP();
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bcmpr_d(jit_state_t *_jit, jit_word_t c,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMP_D_(r0, r1, c);
    FTEST();
    w = _jit->pc.w;
    B_N(((i0 - w) >> 2) - 2, _R0_REGNO);
    NOP();
    return (w);
}

static jit_word_t
_bcmpi_d(jit_state_t *_jit, jit_word_t c,
	 jit_word_t i0, jit_int32_t r0, jit_float64_t *i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi_d(rn(reg), i1);
    FCMP_D_(r0, rn(reg), c);
    FTEST();
    w = _jit->pc.w;
    B_N(((i0 - w) >> 2) - 2, _R0_REGNO);
    NOP();
    jit_unget_reg(reg);
    return (w);
}

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;

    assert(_jitc->function->self.call & jit_call_varargs);

    /* Align pointer if required. */
    reg = jit_get_reg(jit_class_gpr);
    andi(rn(reg), r1, 7);
    subr(r1, r1, rn(reg));
    jit_unget_reg(reg);

    /* Adjust vararg stack pointer. */
    subi(r1, r1, 8);

    /* Load argument. */
    ldr_d(r0, r1);
}
#endif
