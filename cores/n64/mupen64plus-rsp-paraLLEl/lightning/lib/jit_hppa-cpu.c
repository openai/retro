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
typedef struct idiv {
    int			quo;
    int			rem;
} idiv_t;

typedef struct udiv {
    unsigned int	quo;
    unsigned int	rem;
} udiv_t;

/* 16 spill bytes; -52 for first actual stack argument */
#define params_offset		-32
/* Assume all callee save registers may need to be spilled */
#define alloca_offset		192
#define _R0_REGNO		0
#define _R1_REGNO		1
#define _RP_REGNO		2
#define _FP_REGNO		3
#define _R19_REGNO		19
#define _R23_REGNO		23
#define _R24_REGNO		24
#define _R25_REGNO		25
#define _R26_REGNO		26
#define _R28_REGNO		28
#define _R29_REGNO		29
#define _SP_REGNO		30
#define _R31_REGNO		31
#define _CR11_REGNO		11
#define ii(v)			*_jit->pc.ui++ = v
#define f1(o,b,t,i)			_f1(_jit,o,b,t,i)
static void _f1(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#define f2(o,b,r,i,j)			_f2(_jit,o,b,r,i,j)
static void _f2(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f3(o,b,t,i,j)			_f3(_jit,o,b,t,i,j)
static void _f3(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#define f4(o,b,x,s,u,y,c,z,m,t)		_f4(_jit,o,b,x,s,u,y,c,z,m,t)
static void _f4(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f5(o,b,i,s,a,y,c,z,m,t)		_f5(_jit,o,b,i,s,a,y,c,z,m,t)
static void _f5(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f6(o,b,r,s,a,x,c,y,m,i)		_f6(_jit,o,b,r,s,a,x,c,y,m,i)
static void _f6(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f7(o,r,i)			_f7(_jit,o,r,i)
static void _f7(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define f8(o,r2,r1,cf,e1,x,e2,y,d,t)	_f8(_jit,o,r2,r1,cf,e1,x,e2,y,d,t)
static void _f8(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#define f9(o,r,t,cf,e1,im)		_f9(_jit,o,r,t,cf,e1,im)
static void _f9(jit_state_t*,
		jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#define f10(o,r2,r1,u,v,w,x,sa,y,t)	_f10(_jit,o,r2,r1,u,v,w,x,sa,y,t)
static void _f10(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f11(o,r2,r1,c,x,y,z,u,t)	_f11(_jit,o,r2,r1,c,x,y,z,u,t)
static void _f11(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f12(o,r,t,c,x,se,y,c1,z,clen)	_f12(_jit,o,r,t,c,x,se,y,c1,z,clen)
static void _f12(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f13(o,t,r,c,x,nz,c1,clen)	_f13(_jit,o,t,r,c,x,nz,c1,clen)
static void _f13(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t);
#define f13x(o,t,i,c,x,nz,c1,clen)	_f13x(_jit,o,t,i,c,x,nz,c1,clen)
static void _f13x(jit_state_t*,jit_int32_t,jit_int32_t,
		  jit_int32_t,jit_int32_t,jit_int32_t,
		  jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f14(o,r2,r1,c,x,cp,y,cpos,t)	_f14(_jit,o,r2,r1,c,x,cp,y,cpos,t)
static void _f14(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f15(o,r,t,c,c1,p,se,pos,clen)	_f15(_jit,o,r,t,c,c1,p,se,pos,clen)
static void _f15(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f16(o,t,r,c,c1,cp,nz,cpos,clen)	_f16(_jit,o,t,r,c,c1,cp,nz,cpos,clen)
static void _f16(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f16x(o,t,i,c,c1,cp,nz,cpos,clen) _f16x(_jit,o,t,i,c,c1,cp,nz,cpos,clen)
static void _f16x(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f17(o,r2,r1,c,i,n)		_f17(_jit,o,r2,r1,c,i,n)
static void _f17(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f17x(o,r2,r1,c,i,n)		_f17x(_jit,o,r2,r1,c,i,n)
static void _f17x(jit_state_t*,jit_int32_t,jit_int32_t,
		  jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f18(o,p,r,c,i,n)		_f18(_jit,o,p,r,c,i,n)
static void _f18(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f19(o,b,s,i,n)			_f19(_jit,o,b,s,i,n)
static void _f19(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f20(o,t,i,g,n)			_f20(_jit,o,t,i,g,n)
static void _f20(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#define f21(o,t,x,y,n)			_f21(_jit,o,t,x,y,n)
static void _f21(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#define f22(o,b,x,r,n,p)		_f22(_jit,o,b,x,r,n,p)
static void _f22(jit_state_t*,jit_int32_t,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f23(o,a,b,c,d,e,f,g,h)		_f23(_jit,o,a,b,c,d,e,f,g,h)
static void _f23(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f24(o,b,x,s,y,m,r)		_f24(_jit,o,b,x,s,y,m,r)
static void _f24(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f25(o,b,i,s,y,m,r)		_f25(_jit,o,b,i,s,y,m,r)
static void _f25(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f26(o,b,x,s,y,m,r)		_f26(_jit,o,b,x,s,y,m,r)
static void _f26(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f27(o,i,j)			_f27(_jit,o,i,j)
static void _f27(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f28(o,i)			_f28(_jit,o,i)
static void _f28(jit_state_t*,jit_int32_t,jit_int32_t) maybe_unused;
#define f29(o,r,x,s,y,t)		_f29(_jit,o,r,x,s,y,t)
static void _f29(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f30(o,b,r,s,x,y,t)		_f30(_jit,o,b,r,s,x,y,t)
static void _f30(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f31(o,t,r,v,x,y)		_f31(_jit,o,t,r,v,x,y)
static void _f31(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define f33(o,x,r,y,z,u)		_f33(_jit,o,x,r,y,z,u)
static void _f33(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f34(o,o1,x,sf,n,o2)		_f34(_jit,o,o1,x,sf,n,o2)
static void _f34(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f35(o,op,x,sf,n,t)		_f35(_jit,o,op,x,sf,n,t)
static void _f35(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f36(o,r,o1,x,sf,n,o2)		_f36(_jit,o,r,o1,x,sf,n,o2)
static void _f36(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f37(o,r2,r1,o1,x,sf,n,o2)	_f37(_jit,o,r2,r1,o1,x,sf,n,o2)
static void _f37(jit_state_t*,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
#define f38(o,s,u,n)			_f38(_jit,o,s,u,n)
static void _f38(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t) maybe_unused;
/* nulify next instruction if condition is met with addition */
#define ADD_CF_NV		0	/* never */
#define ADD_CF_EQ		2	/* O1 == -O2 (word) */
#define ADD_CF_LT		4	/* O1 <  -O2 (signed)*/
#define ADD_CF_LE		6	/* O1 <= -O2 (signed) */
#define ADD_CF_NUV		8	/* O1 +   O2 does not overflow (unsigned) */
#define ADD_CF_ZNV		10	/* O1 +   O2 is zero or no overflow (unsigned) */
#define ADD_CF_SV		12	/* O1 +   O2 overflows (signed) */
#define ADD_CF_OD		14	/* O1 +   O2 is odd */
#define ADD_CF_TR		1	/* always */
#define ADD_CF_NE		3	/* O1 != -O2 */
#define ADD_CF_GE		5	/* O1 >= -O2 (signed) */
#define ADD_CF_GT		7	/* O1 >  -O2 (signed) */
#define ADD_CF_UV		9	/* O1 +   O2 overflows (unsigned) */
#define ADD_CF_VNZ		11	/* O1 +   O2 is nonzero and overflows (unsigned) */
#define ADD_CF_NSV		13	/* O1 +   O2 does not overflow (signed) */
#define ADD_CF_EV		15	/* O1 +   O2 is even */
#define ADD_EN_NONE		6	/* none */
#define ADD_EN_C		7	/* with carry */
#define ADD_EN_L		10	/* logical */
#define ADD_EN_TSV		12	/* trap on signed overflow */
#define ADD_EN_C_TSV		13	/* with carry and trap on signed overflow */
#define ADDI_OE_TC		0	/* trap on condition */
#define ADDI_OE_TSV_TC		1	/* trap on signed overflow or condition */
#define ADDI_OE_NONE		2	/* none */
#define ADDI_OE_TSV		3	/* trap on signed overflow */
#define ADD_(en,cf,r1,r2,t)	f8(0x2,r2,r1,cf,en>>2,1,en&1,0,0,t)
#define ADD(r1,r2,t)		ADD_(ADD_EN_NONE,ADD_CF_NV,r1,r2,t)
#define ADD_C(r1,r2,t)		ADD_(ADD_EN_C,ADD_CF_NV,r1,r2,t)
#define ADD_L(r1,r2,t)		ADD_(ADD_EN_L,ADD_CF_NV,r1,r2,t)
#define ADDB_(cc,r1,r2,t)	f17(0x28|((cc&1)<<1),r2,r1,cc>>1,t,0)
#define ADDB(r1,r2,t)		ADDB_(ADD_CF_NV,r1,r2,t)
#define ADDB_EQ(r1,r2,t)	ADDB_(ADD_CF_EQ,r1,r2,t)
#define ADDB_LT(r1,r2,t)	ADDB_(ADD_CF_LT,r1,r2,t)
#define ADDB_LE(r1,r2,t)	ADDB_(ADD_CF_LE,r1,r2,t)
#define ADDB_NUV(r1,r2,t)	ADDB_(ADD_CF_NUV,r1,r2,t)
#define ADDB_ZNV(r1,r2,t)	ADDB_(ADD_CF_ZNV,r1,r2,t)
#define ADDB_SV(r1,r2,t)	ADDB_(ADD_CF_SV,r1,r2,t)
#define ADDB_OD(r1,r2,t)	ADDB_(ADD_CF_OD,r1,r2,t)
#define ADDB_TR(r1,r2,t)	ADDB_(ADD_CF_TR,r1,r2,t)
#define ADDB_NE(r1,r2,t)	ADDB_(ADD_CF_NE,r1,r2,t)
#define ADDB_GE(r1,r2,t)	ADDB_(ADD_CF_GE,r1,r2,t)
#define ADDB_GT(r1,r2,t)	ADDB_(ADD_CF_GT,r1,r2,t)
#define ADDB_UV(r1,r2,t)	ADDB_(ADD_CF_UV,r1,r2,t)
#define ADDB_VNZ(r1,r2,t)	ADDB_(ADD_CF_VNZ,r1,r2,t)
#define ADDB_NSV(r1,r2,t)	ADDB_(ADD_CF_NSV,r1,r2,t)
#define ADDB_EV(r1,r2,t)	ADDB_(ADD_CF_EV,r2,r1,t)
#define ADDB_N_(cc,r1,r2,t)	f17(0x28|((cc&1)<<1),r2,r1,cc>>1,t,1)
#define ADDB_N(r1,r2,t)		ADDB_N_(ADD_CF_NV,r1,r2,t)
#define ADDB_N_EQ(r1,r2,t)	ADDB_N_(ADD_CF_EQ,r1,r2,t)
#define ADDB_N_LT(r1,r2,t)	ADDB_N_(ADD_CF_LT,r1,r2,t)
#define ADDB_N_LE(r1,r2,t)	ADDB_N_(ADD_CF_LE,r1,r2,t)
#define ADDB_N_NUV(r1,r2,t)	ADDB_N_(ADD_CF_NUV,r1,r2,t)
#define ADDB_N_ZNV(r1,r2,t)	ADDB_N_(ADD_CF_ZNV,r1,r2,t)
#define ADDB_N_SV(r1,r2,t)	ADDB_N_(ADD_CF_SV,r1,r2,t)
#define ADDB_N_OD(r1,r2,t)	ADDB_N_(ADD_CF_OD,r1,r2,t)
#define ADDB_N_TR(r1,r2,t)	ADDB_N_(ADD_CF_TR,r1,r2,t)
#define ADDB_N_NE(r1,r2,t)	ADDB_N_(ADD_CF_NE,r1,r2,t)
#define ADDB_N_GE(r1,r2,t)	ADDB_N_(ADD_CF_GE,r1,r2,t)
#define ADDB_N_GT(r1,r2,t)	ADDB_N_(ADD_CF_GT,r1,r2,t)
#define ADDB_N_UV(r1,r2,t)	ADDB_N_(ADD_CF_UV,r1,r2,t)
#define ADDB_N_VNZ(r1,r2,t)	ADDB_N_(ADD_CF_VNZ,r1,r2,t)
#define ADDB_N_NSV(r1,r2,t)	ADDB_N_(ADD_CF_NSV,r1,r2,t)
#define ADDB_N_EV(r1,r2,t)	ADDB_N_(ADD_CF_EV,r1,r2,t)
#define ADDI_(ec,cf,i,r,t)	f9(0x2c|(ec>>1),r,t,cf,ec&1,i)
#define ADDI(i,r,t)		ADDI_(ADDI_OE_NONE,ADD_CF_NV,i,r,t)
#define ADDIB_(cc,i,r,t)	f17x(0x29|((cc&1)<<1),r,i,cc>>1,t,0)
#define ADDIB(i,r,t)		ADDIB_(ADD_CF_NV,i,r,t)
#define ADDIB_EQ(i,r,t)		ADDIB_(ADD_CF_EQ,i,r,t)
#define ADDIB_LT(i,r,t)		ADDIB_(ADD_CF_LT,i,r,t)
#define ADDIB_LE(i,r,t)		ADDIB_(ADD_CF_LE,i,r,t)
#define ADDIB_NUV(i,r,t)	ADDIB_(ADD_CF_NUV,i,r,t)
#define ADDIB_ZNV(i,r,t)	ADDIB_(ADD_CF_ZNV,i,r,t)
#define ADDIB_SV(i,r,t)		ADDIB_(ADD_CF_SV,i,r,t)
#define ADDIB_OD(i,r,t)		ADDIB_(ADD_CF_OD,i,r,t)
#define ADDIB_TR(i,r,t)		ADDIB_(ADD_CF_TR,i,r,t)
#define ADDIB_NE(i,r,t)		ADDIB_(ADD_CF_NE,i,r,t)
#define ADDIB_GE(i,r,t)		ADDIB_(ADD_CF_GE,i,r,t)
#define ADDIB_GT(i,r,t)		ADDIB_(ADD_CF_GT,i,r,t)
#define ADDIB_UV(i,r,t)		ADDIB_(ADD_CF_UV,i,r,t)
#define ADDIB_VNZ(i,r,t)	ADDIB_(ADD_CF_VNZ,i,r,t)
#define ADDIB_NSV(i,r,t)	ADDIB_(ADD_CF_NSV,i,r,t)
#define ADDIB_EV(i,r,t)		ADDIB_(ADD_CF_EV,i,r,t)
#define ADDIB_N_(cc,i,r,t)	f17x(0x29|((cc&1)<<1),r,i,cc>>1,t,1)
#define ADDIB_N(i,r,t)		ADDIB_N_(ADD_CF_NV,i,r,t)
#define ADDIB_N_EQ(i,r,t)	ADDIB_N_(ADD_CF_EQ,i,r,t)
#define ADDIB_N_LT(i,r,t)	ADDIB_N_(ADD_CF_LT,i,r,t)
#define ADDIB_N_LE(i,r,t)	ADDIB_N_(ADD_CF_LE,i,r,t)
#define ADDIB_N_NUV(i,r,t)	ADDIB_N_(ADD_CF_NUV,i,r,t)
#define ADDIB_N_ZNV(i,r,t)	ADDIB_N_(ADD_CF_ZNV,i,r,t)
#define ADDIB_N_SV(i,r,t)	ADDIB_N_(ADD_CF_SV,i,r,t)
#define ADDIB_N_OD(i,r,t)	ADDIB_N_(ADD_CF_OD,i,r,t)
#define ADDIB_N_TR(i,r,t)	ADDIB_N_(ADD_CF_TR,i,r,t)
#define ADDIB_N_NE(i,r,t)	ADDIB_N_(ADD_CF_NE,i,r,t)
#define ADDIB_N_GE(i,r,t)	ADDIB_N_(ADD_CF_GE,i,r,t)
#define ADDIB_N_GT(i,r,t)	ADDIB_N_(ADD_CF_GT,i,r,t)
#define ADDIB_N_UV(i,r,t)	ADDIB_N_(ADD_CF_UV,i,r,t)
#define ADDIB_N_VNZ(i,r,t)	ADDIB_N_(ADD_CF_VNZ,i,r,t)
#define ADDIB_N_NSV(i,r,t)	ADDIB_N_(ADD_CF_NSV,i,r,t)
#define ADDIB_N_EV(i,r,t)	ADDIB_N_(ADD_CF_EV,0,i,r,t)
#define ADDIL(i,r)		f7(0xa,r,i)
#define LOG_CC_NV		0	/* never */
#define LOG_CC_EQ		1	/* all bits are 0 */
#define LOG_CC_LT		2	/* leftmost bit is 1 */
#define LOG_CC_LE		3	/* leftmost bit is 1 or all bits are 0 */
#define LOG_CC_OD		7	/* rightmost bit is 1 */
#define LOG_CC_TR		8	/* always */
#define LOG_CC_NE		9	/* some bits are 1 */
#define LOG_CC_GE		10	/* leftmost bit is 0 */
#define LOG_CC_GT		11	/* leftmost bit is 0 or some bits are 1 */
#define LOG_CC_EV		15	/* rightmost bit is 0 */
#define AND_(cc,r1,r2,t)	f8(0x2,r2,r1,cc,0,1,0,0,0,t)
#define AND(r1,r2,t)		AND_(LOG_CC_NV,r1,r2,t)
#define ANDCM_(cc,r1,r2,t)	f8(0x2,r2,r1,cc,0,0,0,0,0,t)
#define ANDCM(r1,r2,t)		ANDCM_(LOG_CC_NV,r1,r2,t)
#define B_(n,i,t)		f20(0x3a,t,i,0,n)
#define B(i,t)			B_(0,i,t)
#define B_N(i,t)		B_(1,i,t)
#define B_L(i)			B_(0,i,_RP_REGNO)
#define B_L_N(i)		B_(1,i,_RP_REGNO)
#define BB_CC_LT		0	/* leftmost bit in word is 1 */
#define BB_CC_GE		1	/* leftmost bit in word is 0 */
#define BB_(c,r,i)		f18(0x30,0,r,c,i,0)
#define BB_N_(c,r,i)		f18(0x30,0,r,c,i,1)
#define BBI_(c,r,p,i)		f18(0x31,p,r,c,i,0)
#define BBI_N_(c,r,p,i)		f18(0x31,p,r,c,i,1)
#define BB(c,r,i)		BB_(c,r,i)
#define BBI_LT(r,p,i)		BBI_(BB_CC_LT,r,p,i)
#define BBI_GE(r,p,i)		BBI_(BB_CC_GE,r,p,i)
#define BB_N(c,r,i)		BB_(c,r,i)
#define BBI_N_LT(r,p,i)		BBI_N_(BB_CC_LT,r,p,i)
#define BBI_N_GE(r,p,i)		BBI_N_(BB_CC_GE,r,p,i)
#define BE(i,s,b)		f19(0x38,b,s,i,0)
#define BE_L(i,s,b)		f19(0x39,b,s,i,0)
#define BE_L_N(i,s,b)		f19(0x39,b,s,i,1)
#define BLR(x,t)		f21(0x3a,t,x,2,0)
#define BLR_N(x,t)		f21(0x3a,t,x,2,1)
#define BREAK(i,j)		f27(0,j,i)
#define BV(x,b)			f21(0x3a,b,x,6,0)
#define BV_N(x,b)		f21(0x3a,b,x,6,1)
#define BVE(b)			f22(0x3a,b,6,0,0,0)
#define BVE_N(b)		f22(0x3a,b,6,0,1,0)
#define BVE_L(b)		f22(0x3a,b,7,0,0,0)
#define BVE_L_N(b)		f22(0x3a,b,7,0,0,1)
#define II_C_NONE		0
#define II_C_M			(1<<5)
#define II_C_S			(1<<13)
#define II_C_SM			(II_C_S|II_C_M)
#define II_AU_NONE		0
#define II_AU_PRE		((1<<13)|II_C_M)
#define II_AU_POS		II_C_M
#define LD_CC_H_NONE		0	/* No hint */
#define LD_CC_H_SPL		2	/* Spatial Locality */
#define CLRBTS()		f23(0x3a,0,0,2,0,0,1,0,1)
#define CS_CC_NV		0	/* never */
#define CS_CC_EQ		2	/* O1 =  O2 */
#define CS_CC_LT		4	/* O1 <  O2 (signed) */
#define CS_CC_LE		6	/* O1 <= O2 (signed) */
#define CS_CC_ULT		8	/* O1 <  O2 (unsigned) */
#define CS_CC_ULE		10	/* O1 <= O2 (unsigned) */
#define CS_CC_SV		12	/* O1 -  O2 overflows (signed) */
#define CS_CC_OD		14	/* O1 -  O2 is odd */
#define CS_CC_TR		1	/* always */
#define CS_CC_NE		3	/* O1 != O2 */
#define CS_CC_GE		5	/* O1 >= O2 (signed) */
#define CS_CC_GT		7	/* O1 >  O2 (signed) */
#define CS_CC_UGE		9	/* O1 >= O2 (unsigned) */
#define CS_CC_UGT		11	/* O1 >  O2 (unsigned) */
#define CS_CC_NSV		13	/* O1 -  O2 does not overflows (signed) */
#define CS_CC_EV		15	/* O1 -  O2 is even */
#define CMPB_(c,r1,r2,i)	f17((c)&1?0x22:0x20,r2,r1,(c)>>1,i,0)
#define CMPB(r1,r2,i)		CMPB_(CS_CC_NV,r1,r2,i)
#define CMPB_EQ(r1,r2,i)	CMPB_(CS_CC_EQ,r1,r2,i)
#define CMPB_LT(r1,r2,i)	CMPB_(CS_CC_LT,r1,r2,i)
#define CMPB_LE(r1,r2,i)	CMPB_(CS_CC_LE,r1,r2,i)
#define CMPB_ULT(r1,r2,i)	CMPB_(CS_CC_ULT,r1,r2,i)
#define CMPB_ULE(r1,r2,i)	CMPB_(CS_CC_ULE,r1,r2,i)
#define CMPB_SV(r1,r2,i)	CMPB_(CS_CC_SV,r1,r2,i)
#define CMPB_OD(r1,r2,i)	CMPB_(CS_CC_OD,r1,r2,i)
#define CMPB_TR(r1,r2,i)	CMPB_(CS_CC_TR,r1,r2,i)
#define CMPB_NE(r1,r2,i)	CMPB_(CS_CC_NE,r1,r2,i)
#define CMPB_GE(r1,r2,i)	CMPB_(CS_CC_GE,r1,r2,i)
#define CMPB_GT(r1,r2,i)	CMPB_(CS_CC_GT,r1,r2,i)
#define CMPB_UGE(r1,r2,i)	CMPB_(CS_CC_UGE,r1,r2,i)
#define CMPB_UGT(r1,r2,i)	CMPB_(CS_CC_UGT,r1,r2,i)
#define CMPB_NSV(r1,r2,i)	CMPB_(CS_CC_NSV,r1,r2,i)
#define CMPB_EV(r1,r2,i)	CMPB_(CS_CC_EV,r1,r2,i)
#define CMPB_N_(c,r1,r2,i)	f17((c)&1?0x22:0x20,r2,r1,(c)>>1,i,1)
#define CMPB_N(r1,r2,i)		CMPB_N_(CS_CC_NV,r1,r2,i)
#define CMPB_EQ_N(r1,r2,i)	CMPB_N_(CS_CC_EQ,r1,r2,i)
#define CMPB_LT_N(r1,r2,i)	CMPB_N_(CS_CC_LT,r1,r2,i)
#define CMPB_LE_N(r1,r2,i)	CMPB_N_(CS_CC_LE,r1,r2,i)
#define CMPB_ULT_N(r1,r2,i)	CMPB_N_(CS_CC_ULT,r1,r2,i)
#define CMPB_ULE_N(r1,r2,i)	CMPB_N_(CS_CC_ULE,r1,r2,i)
#define CMPB_SV_N(r1,r2,i)	CMPB_N_(CS_CC_SV,r1,r2,i)
#define CMPB_OD_N(r1,r2,i)	CMPB_N_(CS_CC_OD,r1,r2,i)
#define CMPB_TR_N(r1,r2,i)	CMPB_N_(CS_CC_TR,r1,r2,i)
#define CMPB_NE_N(r1,r2,i)	CMPB_N_(CS_CC_NE,r1,r2,i)
#define CMPB_GE_N(r1,r2,i)	CMPB_N_(CS_CC_GE,r1,r2,i)
#define CMPB_GT_N(r1,r2,i)	CMPB_N_(CS_CC_GT,r1,r2,i)
#define CMPB_UGE_N(r1,r2,i)	CMPB_N_(CS_CC_UGE,r1,r2,i)
#define CMPB_UGT_N(r1,r2,i)	CMPB_N_(CS_CC_UGT,r1,r2,i)
#define CMPB_NSV_N(r1,r2,i)	CMPB_N_(CS_CC_NSV,r1,r2,i)
#define CMPB_EV_N(r1,r2,i)	CMPB_N_(CS_CC_EV,r1,r2,i)
#define CMPCLR_(c,r1,r2,i)	f8(0x2,r2,r1,c,2,0,0,2,0,i)
#define CMPCLR(r1,r2,i)		CMPCLR_(CS_CC_NV,r1,r2,i)
#define CMPCLR_EQ(r1,r2,i)	CMPCLR_(CS_CC_EQ,r1,r2,i)
#define CMPCLR_LT(r1,r2,i)	CMPCLR_(CS_CC_LT,r1,r2,i)
#define CMPCLR_LE(r1,r2,i)	CMPCLR_(CS_CC_LE,r1,r2,i)
#define CMPCLR_ULT(r1,r2,i)	CMPCLR_(CS_CC_ULT,r1,r2,i)
#define CMPCLR_ULE(r1,r2,i)	CMPCLR_(CS_CC_ULE,r1,r2,i)
#define CMPCLR_SV(r1,r2,i)	CMPCLR_(CS_CC_SV,r1,r2,i)
#define CMPCLR_OD(r1,r2,i)	CMPCLR_(CS_CC_OD,r1,r2,i)
#define CMPCLR_TR(r1,r2,i)	CMPCLR_(CS_CC_TR,r1,r2,i)
#define CMPCLR_NE(r1,r2,i)	CMPCLR_(CS_CC_NE,r1,r2,i)
#define CMPCLR_GE(r1,r2,i)	CMPCLR_(CS_CC_GE,r1,r2,i)
#define CMPCLR_GT(r1,r2,i)	CMPCLR_(CS_CC_GT,r1,r2,i)
#define CMPCLR_UGE(r1,r2,i)	CMPCLR_(CS_CC_UGE,r1,r2,i)
#define CMPCLR_UGT(r1,r2,i)	CMPCLR_(CS_CC_UGT,r1,r2,i)
#define CMPCLR_NSV(r1,r2,i)	CMPCLR_(CS_CC_NSV,r1,r2,i)
#define CMPCLR_EV(r1,r2,i)	CMPCLR_(CS_CC_EV,r1,r2,i)
#define CMPIB_(c,i,r,t)		f17x((c)&1?0x23:0x21,r,i,(c)>>1,t,0)
#define CMPIB_NONE(i,r,t)	CMPIB_(CS_CC_NV,i,r,t)
#define CMPIB_EQ(i,r,t)		CMPIB_(CS_CC_EQ,i,r,t)
#define CMPIB_LT(i,r,t)		CMPIB_(CS_CC_LT,i,r,t)
#define CMPIB_LE(i,r,t)		CMPIB_(CS_CC_LE,i,r,t)
#define CMPIB_ULT(i,r,t)	CMPIB_(CS_CC_ULT,i,r,t)
#define CMPIB_ULE(i,r,t)	CMPIB_(CS_CC_ULE,i,r,t)
#define CMPIB_SV(i,r,t)		CMPIB_(CS_CC_SV,i,r,t)
#define CMPIB_OD(i,r,t)		CMPIB_(CS_CC_OD,i,r,t)
#define CMPIB(i,r,t)		CMPIB_(CS_CC_TR,i,r,t)
#define CMPIB_NE(i,r,t)		CMPIB_(CS_CC_NE,i,r,t)
#define CMPIB_GE(i,r,t)		CMPIB_(CS_CC_GE,i,r,t)
#define CMPIB_GT(i,r,t)		CMPIB_(CS_CC_GT,i,r,t)
#define CMPIB_UGE(i,r,t)	CMPIB_(CS_CC_UGE,i,r,t)
#define CMPIB_UGT(i,r,t)	CMPIB_(CS_CC_UGT,i,r,t)
#define CMPIB_NSV(i,r,t)	CMPIB_(CS_CC_NSV,i,r,t)
#define CMPIB_EV(i,r,t)		CMPIB_(CS_CC_EV,i,r,t)
#define CMPIB_N_(c,i,r,t)	f17x((c)&1?0x23:0x21,r,i,(c)>>1,t,1)
#define CMPIB_NONE_N(i,r,t)	CMPIB_N_(CS_CC_NV,i,r,t)
#define CMPIB_EQ_N(i,r,t)	CMPIB_N_(CS_CC_EQ,i,r,t)
#define CMPIB_LT_N(i,r,t)	CMPIB_N_(CS_CC_LT,i,r,t)
#define CMPIB_LE_N(i,r,t)	CMPIB_N_(CS_CC_LE,i,r,t)
#define CMPIB_ULT_N(i,r,t)	CMPIB_N_(CS_CC_ULT,i,r,t)
#define CMPIB_ULE_N(i,r,t)	CMPIB_N_(CS_CC_ULE,i,r,t)
#define CMPIB_SV_N(i,r,t)	CMPIB_N_(CS_CC_SV,i,r,t)
#define CMPIB_OD_N(i,r,t)	CMPIB_N_(CS_CC_OD,i,r,t)
#define CMPIB_N(i,r,t)		CMPIB_N_(CS_CC_TR,i,r,t)
#define CMPIB_NE_N(i,r,t)	CMPIB_N_(CS_CC_NE,i,r,t)
#define CMPIB_GE_N(i,r,t)	CMPIB_N_(CS_CC_GE,i,r,t)
#define CMPIB_GT_N(i,r,t)	CMPIB_N_(CS_CC_GT,i,r,t)
#define CMPIB_UGE_N(i,r,t)	CMPIB_N_(CS_CC_UGE,i,r,t)
#define CMPIB_UGT_N(i,r,t)	CMPIB_N_(CS_CC_UGT,i,r,t)
#define CMPIB_NSV_N(i,r,t)	CMPIB_N_(CS_CC_NSV,i,r,t)
#define CMPIB_EV_N(i,r,t)	CMPIB_N_(CS_CC_EV,i,r,t)
#define CMPICLR_(c,i,r,t)	f9(0x24,r,t,c,0,i)
#define CMPICLR(i,r,t)		CMPICLR_(CS_CC_NV,i,r,t)
#define CMPICLR_EQ(i,r,t)	CMPICLR_(CS_CC_EQ,i,r,t)
#define CMPICLR_LT(i,r,t)	CMPICLR_(CS_CC_LT,i,r,t)
#define CMPICLR_LE(i,r,t)	CMPICLR_(CS_CC_LE,i,r,t)
#define CMPICLR_ULT(i,r,t)	CMPICLR_(CS_CC_ULT,i,r,t)
#define CMPICLR_ULE(i,r,t)	CMPICLR_(CS_CC_ULE,i,r,t)
#define CMPICLR_SV(i,r,t)	CMPICLR_(CS_CC_SV,i,r,t)
#define CMPICLR_OD(i,r,t)	CMPICLR_(CS_CC_OD,i,r,t)
#define CMPICLR_TR(i,r,t)	CMPICLR_(CS_CC_TR,i,r,t)
#define CMPICLR_NE(i,r,t)	CMPICLR_(CS_CC_NE,i,r,t)
#define CMPICLR_GE(i,r,t)	CMPICLR_(CS_CC_GE,i,r,t)
#define CMPICLR_GT(i,r,t)	CMPICLR_(CS_CC_GT,i,r,t)
#define CMPICLR_UGE(i,r,t)	CMPICLR_(CS_CC_UGE,i,r,t)
#define CMPICLR_UGT(i,r,t)	CMPICLR_(CS_CC_UGT,i,r,t)
#define CMPICLR_NSV(i,r,t)	CMPICLR_(CS_CC_NSV,i,r,t)
#define CMPICLR_EV(i,r,t)	CMPICLR_(CS_CC_EV,i,r,t)
#define COPR(u,s)		f38(0x0c,s,u,0)
#define UI_CF_NONE		0	/* never */
#define UI_CF_SBZ		2	/* some byte zero */
#define UI_CF_SHZ		3	/* some halfword zero */
#define UI_CF_SDC		4	/* some digit carry */
#define UI_CF_SBC		6	/* some byte carry */
#define UI_CF_SHC		7	/* some halfword carry */
#define UI_TR_SHC		8	/* always */
#define UI_CF_NBZ		10	/* no byte zero */
#define UI_CF_NHZ		11	/* no halfword zero */
#define UI_CF_NDC		12	/* no digit carry */
#define UI_CF_NBC		14	/* no byte carry */
#define UI_CF_NHC		15	/* no halfword carry */
#define DCOR_(e1,cf,r,t)	f8(0x2,r,0,cf,2,1,1,e1,0,t)
#define DCOR(r,t)		DCOR_(2,UI_CF_NONE,r,t)
#define DCOR_I(r,t)		DCOR_(3,UI_CF_NONE,r,t)
#define SED_C_NEVER		0	/* never */
#define SED_C_EQ		1	/* all bits 0 */
#define SED_C_LT		2	/* lefmost bits 1 */
#define SED_C_OD		3	/* rightmost bit 1 */
#define SED_C_TR		4	/* always */
#define SED_C_NE		5	/* some bit 1 */
#define SED_C_GE		6	/* lefmost bits 1 */
#define SED_C_EV		7	/* rightmost bit 0 */
#define DEPW(r,len,t)		f13(0x35,t,r,SED_C_NEVER,0,1,0,len)
#define DEPW_Z(r,len,t)		f13(0x35,t,r,SED_C_NEVER,0,0,0,len)
#define DEPWR(r,pos,len,t)	f16(0x35,t,r,SED_C_NEVER,0,1,1,31-(pos),len)
#define DEPWR_Z(r,pos,len,t)	f16(0x35,t,r,SED_C_NEVER,0,1,0,31-(pos),len)
#define SHLWI(r,sa,t)		DEPWR_Z(r,31-(sa),32-(sa),t)
#define DEPWI(i,len,t)		f13x(0x35,t,i,SED_C_NEVER,2,1,0,len)
#define DEPWI_Z(i,len,t)	f13x(0x35,t,i,SED_C_NEVER,2,0,0,len)
#define DEPWRI(i,pos,len,t)	f16x(0x35,t,i,SED_C_NEVER,1,1,1,31-(pos),len)
#define DEPWRI_Z(i,pos,len,t)	f16x(0x35,t,i,SED_C_NEVER,1,1,0,31-(pos),len)
#define DIAG(i)			f28(0x5,i)
#define DS(r1,r2,t)		f8(0x2,r2,r1,ADD_CF_NV,1,0,0,1,0,t)
#define EXTRW(r,len,t)		f12(0x34,r,t,SED_C_NEVER,2,1,0,0,0,len)
#define EXTRW_U(r,len,t)	f12(0x34,r,t,SED_C_NEVER,2,0,0,0,0,len)
#define EXTRWR(r,pos,len,t)	f15(0x34,r,t,SED_C_NEVER,1,1,1,pos,len)
#define SHRWI(r,sa,t)		EXTRWR(r,31-(sa),32-(sa),t)
#define EXTRWR_U(r,pos,len,t)	f15(0x34,r,t,SED_C_NEVER,1,1,0,pos,len)
#define SHRWI_U(r,sa,t)		EXTRWR_U(r,31-(sa),32-(sa),t)
#define FDC(x,s,b)		f24(0x1,b,x,s,0x4a,0,0)
#define FDC_M(x,s,b)		f24(0x1,b,x,s,0x4a,1,0)
#define FDCI(i,s,b)		f25(0x1,b,i,s,0xca,0,0)
#define FDCE(x,s,b)		f24(0x1,b,x,s,0x4b,0,0)
#define FDCE_M(x,s,b)		f24(0x1,b,x,s,0x4b,1,0)
#define FIC(x,s,b)		f26(0x1,b,x,s,0xa,0,0)
#define FIC_M(x,s,b)		f26(0x1,b,x,s,0xa,1,0)
#define FICI(x,s,b)		f25(0x1,b,x,s,0x4f,0,0)
#define FICI_M(x,s,b)		f25(0x1,b,x,s,0x4f,1,0)
#define FICE(x,s,b)		f26(0x1,b,x,s,0xb,0,0)
#define FICE_M(x,s,b)		f26(0x1,b,x,s,0xb,1,0)
#define HADD_(c,r1,r2,t)	f8(0x2,r2,r1,0,0,1,1,c,0,t)
#define HADD(r1,r2,t)		HADD_(3,r1,r2,t)
#define HADD_SS(r1,r2,t)	HADD_(1,r1,r2,t)
#define HADD_US(r1,r2,t)	HADD_(0,r1,r2,t)
#define HAVG(r1,r2,t)		f8(0x2,r2,r1,0,0,1,0,3,0,t)
#define HSHL(r,sa,t)		f10(0x3e,0,r,1,0,0,2,sa,0,t)
#define HSHLADD(r1,sa,r2,t)	f8(0x2,r2,r1,0,1,1,1,sa,0,t)
#define HSHR(r,sa,t)		f10(0x3e,r,0,1,2,0,3,sa,0,t)
#define HSHR_U(r,sa,t)		f10(0x3e,r,0,1,2,0,2,sa,0,t)
#define HSHRADD(r1,sa,r2,t)	f8(0x2,r2,r1,0,1,0,1,sa,0,t)
#define HSUB_(c,r1,r2,t)	f8(0x2,r2,r1,0,0,0,1,c,0,t)
#define HSUB(r1,r2,t)		HSUB_(3,r1,r2,t)
#define HSUB_SS(r1,r2,t)	HSUB_(1,r1,r2,t)
#define HSUB_US(r1,r2,t)	HSUB_(0,r1,r2,t)
#define IDTLBT(r1,r2)		f26(0x1,r2,r1,0,0x60,0,0)
#define IITLBT(r1,r2)		f26(0x1,r2,r1,0,0x20,0,0)
#define LCI(x,s,b,t)		f24(0x1,b,x,s,0x4c,0,t)
#define LDBL(i,b,t)		f1(0x10,b,t,i)
#define LDB(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,0,0,t)
#define LDBI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,0,0,t)
#define LDCD(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,5,0,t)
#define LDCDI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,5,0,t)
#define LDCW(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,7,0,t)
#define LDCWI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,7,0,t)
#define LDDL(i,b,t)		f3(0x14,b,t,i,0)
#define LDD(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,3,0,t)
#define LDDI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,3,0,t)
#define LDDA(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,4,0,t)
#define LDDAI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,4,0,t)
#define LDHL(i,b,t)		f1(0x11,b,t,i)
#define LDH(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,1,0,t)
#define LDHI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,1,0,t)
#define LDIL(i,t)		f7(0x8,t,i)
#define LDO(i,b,t)		f1(0xd,b,t,i)
#define LDI(i,t)		LDO(i,0,t)
#define LDSID(s,b,t)		f30(0x0,b,0,s,0,0x85,t)
#define LDWL(i,b,t)		f1(0x12,b,t,i)
#define LDWL_MB(i,b,t)		f1(0x13,b,t,i)	/* pre-dec or post-inc */
#define LDWL_MA(i,b,t)		f2(0x17,b,t,i,2)/* post-dec or pre-inc */
#define LDW(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,2,0,t)
#define LDWI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,2,0,t)
#define LDWA(x,b,t)		f4(0x3,b,x,0,0,0,LD_CC_H_NONE,6,0,t)
#define LDWAI(i,b,t)		f5(0x3,b,i,0,0,1,LD_CC_H_NONE,6,0,t)
#define LPA(x,s,b,t)		f24(0x1,b,x,s,0x4d,0,t)
#define MFSP(s,t)		f29(0x0,0,0,s,0x25,t)
#define MIXH_L(r1,r2,t)		f10(0x3e,r2,r1,1,0,0,1,0,0,t)
#define MIXH_R(r1,r2,t)		f10(0x3e,r2,r1,1,2,0,1,0,0,t)
#define MIXW_L(r1,r2,t)		f10(0x3e,r2,r1,1,0,0,0,0,0,t)
#define MIXW_R(r1,r2,t)		f10(0x3e,r2,r1,1,2,0,0,0,0,t)
#define MOVB_(c,r1,r2,i)	f17(0x32,r2,r1,c,i,0)
#define MOVB(r1,r2,i)		MOVB_(SED_C_NEVER,r1,r2,i)
#define MOVB_EQ(r1,r2,i)	MOVB_(SED_C_EQ,r1,r2,i)
#define MOVB_LT(r1,r2,i)	MOVB_(SED_C_LT,r1,r2,i)
#define MOVB_OD(r1,r2,i)	MOVB_(SED_C_OD,r1,r2,i)
#define MOVB_TR(r1,r2,i)	MOVB_(SED_C_TR,r1,r2,i)
#define MOVB_NE(r1,r2,i)	MOVB_(SED_C_NE,r1,r2,i)
#define MOVB_GE(r1,r2,i)	MOVB_(SED_C_GE,r1,r2,i)
#define MOVB_EV(r1,r2,i)	MOVB_(SED_C_EV,r1,r2,i)
#define MOVIB_(c,r,i,t)		f17x(0x33,r,i,c,t,0)
#define MOVIB(i,r,t)		MOVIB_(SED_C_NEVER,i,r,t)
#define MOVIB_EQ(i,r,t)		MOVIB_(SED_C_EQ,i,r,t)
#define MOVIB_LT(i,r,t)		MOVIB_(SED_C_LT,i,r,t)
#define MOVIB_OD(i,r,t)		MOVIB_(SED_C_OD,i,r,t)
#define MOVIB_TR(i,r,t)		MOVIB_(SED_C_TR,i,r,t)
#define MOVIB_NE(i,r,t)		MOVIB_(SED_C_NE,i,r,t)
#define MOVIB_GE(i,r,t)		MOVIB_(SED_C_GE,i,r,t)
#define MOVIB_EV(i,r,t)		MOVIB_(SED_C_EV,i,r,t)
#define MTCTL(r,t)		f31(0x0,t,r,0,0xc2,0)
#define MTSAR(r)		MTCTL(r,_CR11_REGNO)
#define MTSARCM(r)		f31(0x0,0xb,r,0,0xc6,0)
#define MTSM(r)			f33(0x0,0,r,0,0xc3,0)
#define MTSP(r,s)		f29(0x0,0,r,s,0xc1,0)
#define OR_(c,r1,r2,t)		f8(0x2,r2,r1,c,0,1,0,1,0,t)
#define OR(r1,r2,t)		OR_(LOG_CC_NV,r1,r2,t)
#define NOP()			OR(_R0_REGNO,_R0_REGNO,_R0_REGNO)
#define COPY(r,t)		OR(r,0,t)
#define PDC(x,s,b)		f24(0x1,b,x,s,0x4e,0,0)
#define PDTLB(x,s,b)		f24(0x1,b,x,s,0x48,0,0)
#define PDTLB_L(x,s,b)		f24(0x1,b,x,s,0x58,0,0)
#define PDTLBE(x,s,b)		f24(0x1,b,x,s,0x49,0,0)
#define PERMH(c,r,t)		f10(0x3e,r,r,0,(c)&3,0,((c)>>2)&3,(((c)>>2)&6)|(((c)>>6)&3),0,t)
#define PITBL(x,s,b)		f26(0x1,b,x,s,0x08,0,0)
#define PITBL_L(x,s,b)		f26(0x1,b,x,s,0x18,0,0)
#define PITBLE(x,s,b)		f26(0x1,b,x,s,0x09,0,0)
#define POPBTS(i)		f23(0x3a,0,0,2,0,i,1,0,1)
#define PROBE_R(s,b,r,t)	f24(0x1,b,r,s,0x46,0,t)
#define PROBE_W(s,b,r,t)	f24(0x1,b,r,s,0x47,0,t)
#define PROBEI_R(s,b,i,t)	f24(0x1,b,i,s,0x46,0,t)
#define PROBEI_W(s,b,i,t)	f24(0x1,b,i,s,0x47,0,t)
#define PUSHBTS(r)		f23(0x3a,0,r,2,0,0,0,0,1)
#define PUSHNOM()		f23(0x3a,0,0,2,0,0,0,0,1)
#define RFI()			f33(0x0,0,0,0,0x60,0)
#define RFI_R()			f33(0x0,0,0,0,0x65,0)
#define RSM(i,t)		f33(0x0,((i)&0x3e0)>>5,(i)&0x1f,0,0x73,t)
#define SHLADD_(e,cf,r1,sa,r2,t) f8(0x2,r2,r1,cf,e,1,0,sa,0,t)
#define SHLADD(r1,sa,r2,t)	SHLADD_(1,ADD_CF_NV,r1,sa,r2,t)
#define SHLADD_L(r1,sa,r2,t)	SHLADD_(2,ADD_CF_NV,r1,sa,r2,t)
#define SHLADD_TSV(r1,sa,r2,t)	SHLADD_(3,ADD_CF_NV,r1,sa,r2,t)
#define SHRPD(r1,r2,t)		f11(0x34,r2,r1,SED_C_NEVER,0,0,1,0,t)
#define SHRPDI(r1,r2,sa,t)	f14(0x34,r2,r1,SED_C_NEVER,0,(63-(sa))>>5,1,(63-(sa))&0x1f,t)
#define SHRPW(r1,r2,t)		f11(0x34,r2,r1,SED_C_NEVER,0,0,0,0,t)
#define SHRPWI(r1,r2,sa,t)	f14(0x34,r2,r1,SED_C_NEVER,0,1,0,31-(sa),t)
#define SPOP0(sf,so)		f34(0x4,(so)>>5,0,sf,0,(so)&0x1f)
#define SPOP1(sf,so,t)		f35(0x4,so,1,sf,0,t)
#define SPOP2(sf,so,r)		f36(0x4,r,(so)>>5,2,sf,0,(so)&0x1f)
#define SPOP3(sf,so,r1,r2)	f37(0x4,r2,r1,(so)>>5,3,sf,0,(so)&0x1f)
#define SSM(i,t)		f33(0x00,(i)>>5,(i)&0x1f,0,0x6b,t)
#define STBL(r,i,b)		f1(0x18,b,r,i)
#define STBI(r,i,b)		f6(0x3,b,r,0,0,1,LD_CC_H_NONE,0x8,0,i)
#define STDL(r,i,b)		f3(0x1c,b,r,i,0)
#define STDI(r,i,b)		f6(0x3,b,r,0,0,1,LD_CC_H_NONE,0xc,0,i)
#define STDA(r,i,b)		f6(0x3,b,r,0,0,1,LD_CC_H_NONE,0xf,0,i)
#define STHL(r,i,b)		f1(0x19,b,r,i)
#define STHI(r,i,b)		f6(0x3,b,r,0,0,1,LD_CC_H_NONE,0x9,0,i)
#define STWL(r,i,b)		f1(0x1a,b,r,i)
#define STWL_MA(r,i,b)		f1(0x1b,b,r,i)	/* pre-dec or post-inc */
#define STWL_MB(r,i,b)		f2(0x1f,b,r,i,2)/* post-dec or pre-inc */
#define STWI(r,i,b)		f6(0x3,b,r,0,0,1,LD_CC_H_NONE,0xa,0,i)
#define STWA(r,i,b)		f6(0x3,b,r,0,0,1,LD_CC_H_NONE,0xe,0,i)
#define SUB_(e1,e2,cf,r1,r2,t)	f8(0x2,r2,r1,cf,e1,0,e2,0,0,t)
#define SUB(r1,r2,t)		SUB_(1,0,CS_CC_NV,r1,r2,t)
#define SUB_B(r1,r2,t)		SUB_(1,1,CS_CC_NV,r1,r2,t)
#define SUB_SV(r1,r2,t)		SUB_(1,0,CS_CC_SV,r1,r2,t)
#define SUB_NSV(r1,r2,t)	SUB_(1,0,CS_CC_NSV,r1,r2,t)
/* actually, rsbi */
#define SUBI_(e1,cf,i,r,t)	f9(0x25,r,t,cf,e1,i)
#define SUBI(i,r,t)		SUBI_(0,ADD_CF_NV,i,r,t)
#define SYNC()			f33(0x0,0,0,0,0x20,0)
#define SYNCDMA()		f33(0x0,0,1<<4,0,0x20,0)
#define UADDCM(r1,r2,t)		f8(0x2,r2,r1,ADD_CF_NV,2,0,1,2,0,t)
#define UXOR(r1,r2,t)		f8(0x2,r2,r1,LOG_CC_NV,0,1,1,2,0,t)
#define XOR(r1,r2,t)		f8(0x2,r2,r1,LOG_CC_NV,0,1,0,2,0,t)
#  define nop(c)			_nop(_jit,c)
static void _nop(jit_state_t*,jit_int32_t);
#define movr(r0,r1)		_movr(_jit,r0,r1)
static void _movr(jit_state_t*,jit_int32_t,jit_int32_t);
#define movi(r0,i0)		_movi(_jit,r0,i0)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#define movi_p(r0,i0)		_movi_p(_jit,r0,i0)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#define comr(r0,r1)		UADDCM(_R0_REGNO,r1,r0)
#define negr(r0,r1)		SUB(_R0_REGNO,r1,r0)
#define extr_c(r0,r1)		EXTRWR(r1,31,8,r0)
#define extr_uc(r0,r1)		EXTRWR_U(r1,31,8,r0)
#define extr_s(r0,r1)		EXTRWR(r1,31,16,r0)
#define extr_us(r0,r1)		EXTRWR_U(r1,31,16,r0)
#if __BYTE_ORDER == __BIG_ENDIAN
#  define htonr_us(r0,r1)	extr_us(r0,r1)
#  define htonr_ui(r0,r1)	movr(r0,r1)
#else
#  error need htonr implementation
#endif
#define addr(r0,r1,r2)		ADD(r1,r2,r0)
#define addi(r0,r1,i0)		_addi(_jit,r0,r1,i0)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define addcr(r0,r1,r2)		addr(r0,r1,r2)
#define addci(r0,r1,i0)		_addci(_jit,r0,r1,i0)
static void _addci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define addxr(r0,r1,r2)		ADD_C(r1,r2,r0)
#define addxi(r0,r1,i0)		_addxi(_jit,r0,r1,i0)
static void _addxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define subr(r0,r1,r2)		SUB(r1,r2,r0)
#define subi(r0,r1,i0)		_subi(_jit,r0,r1,i0)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define subcr(r0,r1,r2)		subr(r0,r1,r2)
#define subci(r0,r1,i0)		_subci(_jit,r0,r1,i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define subxr(r0,r1,r2)		SUB_B(r1,r2,r0)
#define subxi(r0,r1,i0)		_subxi(_jit,r0,r1,i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define rsbi(r0, r1, i0)	_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define mulr(r0,r1,r2)		_mulr(_jit,r0,r1,r2)
static void _mulr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define muli(r0,r1,i0)		_muli(_jit,r0,r1,i0)
static void _muli(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static long long __llmul(int, int);
#define qmulr(r0,r1,r2,r3)	_qmulr(_jit,r0,r1,r2,r3)
static void _qmulr(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define qmuli(r0,r1,r2,i0)	_qmuli(_jit,r0,r1,r2,i0)
static void _qmuli(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#define qmulr_u(r0,r1,r2,r3)	_qmulr_u(_jit,r0,r1,r2,r3)
static void _qmulr_u(jit_state_t*,
		     jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define qmuli_u(r0,r1,r2,i0)	_qmuli_u(_jit,r0,r1,r2,i0)
static void _qmuli_u(jit_state_t*,
		     jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
static int __idiv(int, int);
#define divr(r0,r1,r2)		_divr(_jit,r0,r1,r2)
static void _divr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define divi(r0,r1,i0)		_divi(_jit,r0,r1,i0)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static unsigned int __udiv(unsigned int, unsigned int);
#define divr_u(r0,r1,r2)	_divr_u(_jit,r0,r1,r2)
static void _divr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define divi_u(r0,r1,i0)	_divi_u(_jit,r0,r1,i0)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static int __irem(int, int);
#define remr(r0,r1,r2)		_remr(_jit,r0,r1,r2)
static void _remr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define remi(r0,r1,i0)		_remi(_jit,r0,r1,i0)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static unsigned int __urem(unsigned int, unsigned int);
#define remr_u(r0,r1,r2)	_remr_u(_jit,r0,r1,r2)
static void _remr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define remi_u(r0,r1,i0)	_remi_u(_jit,r0,r1,i0)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static idiv_t __idivrem(int, int);
#define qdivr(r0,r1,r2,r3)	_qdivr(_jit,r0,r1,r2,r3)
static void _qdivr(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define qdivi(r0,r1,r2,i0)	_qdivi(_jit,r0,r1,r2,i0)
static void _qdivi(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
static udiv_t __udivrem(unsigned int, unsigned int);
#define qdivr_u(r0,r1,r2,r3)	_qdivr_u(_jit,r0,r1,r2,r3)
static void _qdivr_u(jit_state_t*,
		     jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define qdivi_u(r0,r1,r2,i0)	_qdivi_u(_jit,r0,r1,r2,i0)
static void _qdivi_u(jit_state_t*,
		     jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#define andr(r0,r1,r2)		AND(r1,r2,r0)
#define andi(r0,r1,i0)		_andi(_jit,r0,r1,i0)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define orr(r0,r1,r2)		OR(r1,r2,r0)
#define ori(r0,r1,i0)		_ori(_jit,r0,r1,i0)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define xorr(r0,r1,r2)		XOR(r1,r2,r0)
#define xori(r0,r1,i0)		_xori(_jit,r0,r1,i0)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define lshr(r0,r1,r2)		_lshr(_jit,r0,r1,r2)
static void _lshr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lshi(r0,r1,i0)		SHLWI(r1,i0,r0)
#define rshr(r0,r1,r2)		_rshr(_jit,r0,r1,r2)
static void _rshr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define rshi(r0,r1,i0)		SHRWI(r1,i0,r0)
#define rshr_u(r0,r1,r2)	_rshr_u(_jit,r0,r1,r2)
static void _rshr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define rshi_u(r0,r1,i0)	SHRWI_U(r1,i0,r0)
#define cmpr(c,r0,r1,r2)	_cmpr(_jit,c,r0,r1,r2)
static void _cmpr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define cmpi(c,ci,r0,r1,i0)	_cmpi(_jit,c,ci,r0,r1,i0)
static void _cmpi(jit_state_t*,jit_word_t,jit_word_t,
		  jit_int32_t,jit_int32_t,jit_word_t);
#define ltr(r0,r1,r2)		cmpr(CS_CC_GE,r0,r1,r2)
#define lti(r0,r1,i0)		cmpi(CS_CC_GE,CS_CC_LE,r0,r1,i0)
#define ltr_u(r0,r1,r2)		cmpr(CS_CC_UGE,r0,r1,r2)
#define lti_u(r0,r1,i0)		cmpi(CS_CC_UGE,CS_CC_ULE,r0,r1,i0)
#define ler(r0,r1,r2)		cmpr(CS_CC_GT,r0,r1,r2)
#define lei(r0,r1,i0)		cmpi(CS_CC_GT,CS_CC_LT,r0,r1,i0)
#define ler_u(r0,r1,r2)		cmpr(CS_CC_UGT,r0,r1,r2)
#define lei_u(r0,r1,i0)		cmpi(CS_CC_UGT,CS_CC_ULT,r0,r1,i0)
#define eqr(r0,r1,r2)		cmpr(CS_CC_NE,r0,r1,r2)
#define eqi(r0,r1,i0)		cmpi(CS_CC_NE,CS_CC_NE,r0,r1,i0)
#define ger(r0,r1,r2)		cmpr(CS_CC_LT,r0,r1,r2)
#define gei(r0,r1,i0)		cmpi(CS_CC_LT,CS_CC_GT,r0,r1,i0)
#define ger_u(r0,r1,r2)		cmpr(CS_CC_ULT,r0,r1,r2)
#define gei_u(r0,r1,i0)		cmpi(CS_CC_ULT,CS_CC_UGT,r0,r1,i0)
#define gtr(r0,r1,r2)		cmpr(CS_CC_LE,r0,r1,r2)
#define gti(r0,r1,i0)		cmpi(CS_CC_LE,CS_CC_GE,r0,r1,i0)
#define gtr_u(r0,r1,r2)		cmpr(CS_CC_ULE,r0,r1,r2)
#define gti_u(r0,r1,i0)		cmpi(CS_CC_ULE,CS_CC_UGE,r0,r1,i0)
#define ner(r0,r1,r2)		cmpr(CS_CC_EQ,r0,r1,r2)
#define nei(r0,r1,i0)		cmpi(CS_CC_EQ,CS_CC_EQ,r0,r1,i0)
#define ldr_c(r0,r1)		_ldr_c(_jit,r0,r1)
static void _ldr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#define ldi_c(r0,i0)		_ldi_c(_jit,r0,i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_c(r0,r1,r2)	_ldxr_c(_jit,r0,r1,r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_c(r0,r1,i0)	_ldxi_c(_jit,r0,r1,i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldr_uc(r0,r1)		LDBI(_R0_REGNO,r1,r0)
#define ldi_uc(r0,i0)		_ldi_uc(_jit,r0,i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_uc(r0,r1,r2)	LDB(r2,r1,r0)
#define ldxi_uc(r0,r1,i0)	_ldxi_uc(_jit,r0,r1,i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldr_s(r0,r1)		_ldr_s(_jit,r0,r1)
static void _ldr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#define ldi_s(r0,i0)		_ldi_s(_jit,r0,i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_s(r0,r1,r2)	_ldxr_s(_jit,r0,r1,r2)
static void _ldxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define ldxi_s(r0,r1,i0)	_ldxi_s(_jit,r0,r1,i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldr_us(r0,r1)		LDHI(_R0_REGNO,r1,r0)
#define ldi_us(r0,i0)		_ldi_us(_jit,r0,i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_us(r0,r1,r2)	LDH(r2,r1,r0)
#define ldxi_us(r0,r1,i0)	_ldxi_us(_jit,r0,r1,i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ldr(r0,r1)		ldr_ui(r0,r1)
#define ldr_i(r0,r1)		ldr_ui(r0,r1)
#define ldr_ui(r0,r1)		LDWI(_R0_REGNO,r1,r0)
#define ldi_i(r0,i0)		ldi_ui(r0,i0)
#define ldi_ui(r0,i0)		_ldi_ui(_jit,r0,i0)
static void _ldi_ui(jit_state_t*,jit_int32_t,jit_word_t);
#define ldxr_i(r0,r1,r2)	ldxr_ui(r0,r1,r2)
#define ldxr_ui(r0,r1,r2)	LDW(r2,r1,r0)
#define ldxi(r0,r1,i0)		ldxi_ui(r0,r1,i0)
#define ldxi_i(r0,r1,i0)	ldxi_ui(r0,r1,i0)
#define ldxi_ui(r0,r1,i0)	_ldxi_ui(_jit,r0,r1,i0)
static void _ldxi_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define str_c(r0,r1)		STBI(r1,_R0_REGNO,r0)
#define sti_c(i0,r0)		_sti_c(_jit,i0,r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#define stxr_c(r0,r1,r2)	_stxr_c(_jit,r0,r1,r2)
static void _stxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_c(i0,r0,r1)	_stxi_c(_jit,i0,r0,r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define str_s(r0,r1)		STHI(r1,_R0_REGNO,r0)
#define sti_s(i0,r0)		_sti_s(_jit,i0,r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#define stxr_s(r0,r1,r2)	_stxr_s(_jit,r0,r1,r2)
static void _stxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi_s(i0,r0,r1)	_stxi_s(_jit,i0,r0,r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define str_i(r0,r1)		STWI(r1,_R0_REGNO,r0)
#define sti_i(i0,r0)		_sti_i(_jit,i0,r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#define stxr_i(r0,r1,r2)	_stxr_i(_jit,r0,r1,r2)
static void _stxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define stxi(i0,r0,r1)		stxi_i(i0,r0,r1)
#define stxi_i(i0,r0,r1)	_stxi_i(_jit,i0,r0,r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bcmpr(c,i0,r0,r1)	_bcmpr(_jit,c,i0,r0,r1)
static jit_word_t _bcmpr(jit_state_t*,jit_word_t,
			 jit_word_t,jit_int32_t,jit_int32_t);
#define bcmpi(c,ci,i0,r0,i1)	_bcmpi(_jit,c,ci,i0,r0,i1)
static jit_word_t _bcmpi(jit_state_t*,jit_word_t,jit_word_t,
			 jit_word_t,jit_int32_t,jit_word_t);
#define bltr(i0,r0,r1)		bcmpr(CS_CC_LT,i0,r0,r1)
#define blti(i0,r0,r1)		bcmpi(CS_CC_LT,CS_CC_GT,i0,r0,r1)
#define bltr_u(i0,r0,r1)	bcmpr(CS_CC_ULT,i0,r0,r1)
#define blti_u(i0,r0,r1)	bcmpi(CS_CC_ULT,CS_CC_UGT,i0,r0,r1)
#define bler(i0,r0,r1)		bcmpr(CS_CC_LE,i0,r0,r1)
#define blei(i0,r0,r1)		bcmpi(CS_CC_LE,CS_CC_GE,i0,r0,r1)
#define bler_u(i0,r0,r1)	bcmpr(CS_CC_ULE,i0,r0,r1)
#define blei_u(i0,r0,r1)	bcmpi(CS_CC_ULE,CS_CC_UGE,i0,r0,r1)
#define beqr(i0,r0,r1)		bcmpr(CS_CC_EQ,i0,r0,r1)
#define beqi(i0,r0,r1)		bcmpi(CS_CC_EQ,CS_CC_EQ,i0,r0,r1)
#define bger(i0,r0,r1)		bcmpr(CS_CC_GE,i0,r0,r1)
#define bgei(i0,r0,r1)		bcmpi(CS_CC_GE,CS_CC_LE,i0,r0,r1)
#define bger_u(i0,r0,r1)	bcmpr(CS_CC_UGE,i0,r0,r1)
#define bgei_u(i0,r0,r1)	bcmpi(CS_CC_UGE,CS_CC_ULE,i0,r0,r1)
#define bgtr(i0,r0,r1)		bcmpr(CS_CC_GT,i0,r0,r1)
#define bgti(i0,r0,r1)		bcmpi(CS_CC_GT,CS_CC_LT,i0,r0,r1)
#define bgtr_u(i0,r0,r1)	bcmpr(CS_CC_UGT,i0,r0,r1)
#define bgti_u(i0,r0,r1)	bcmpi(CS_CC_UGT,CS_CC_ULT,i0,r0,r1)
#define bner(i0,r0,r1)		bcmpr(CS_CC_NE,i0,r0,r1)
#define bnei(i0,r0,r1)		bcmpi(CS_CC_NE,CS_CC_NE,i0,r0,r1)
#define bmxr(c,i0,r0,r1)	_bmxr(_jit,c,i0,r0,r1)
static jit_word_t _bmxr(jit_state_t*,jit_bool_t,
			jit_word_t,jit_int32_t,jit_int32_t);
#define bmxi(c,i0,r0,i1)	_bmxi(_jit,c,i0,r0,i1)
static jit_word_t _bmxi(jit_state_t*,jit_bool_t,
			jit_word_t,jit_int32_t,jit_word_t);
#define bmcr(r0,r1,r2)		bmxr(0,r0,r1,r2)
#define bmci(r0,r1,r2)		bmxi(0,r0,r1,r2)
#define bmsr(r0,r1,r2)		bmxr(1,r0,r1,r2)
#define bmsi(r0,r1,r2)		bmxi(1,r0,r1,r2)
#define boaddr(i0,r0,r1)	_boaddr(_jit,i0,r0,r1)
static jit_word_t _boaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define boaddi(i0,r0,i1)	_boaddi(_jit,i0,r0,i1)
static jit_word_t _boaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define boaddr_u(i0,r0,r1)	_boaddr_u(_jit,i0,r0,r1)
static jit_word_t _boaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define boaddi_u(i0,r0,i1)	_boaddi_u(_jit,i0,r0,i1)
static jit_word_t _boaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bxaddr(i0,r0,r1)	_bxaddr(_jit,i0,r0,r1)
static jit_word_t _bxaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bxaddi(i0,r0,i1)	_bxaddi(_jit,i0,r0,i1)
static jit_word_t _bxaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bxaddr_u(i0,r0,r1)	_bxaddr_u(_jit,i0,r0,r1)
static jit_word_t _bxaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bxaddi_u(i0,r0,i1)	_bxaddi_u(_jit,i0,r0,i1)
static jit_word_t _bxaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bosubr(i0,r0,r1)	_bosubr(_jit,i0,r0,r1)
static jit_word_t _bosubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bosubi(i0,r0,i1)	_bosubi(_jit,i0,r0,i1)
static jit_word_t _bosubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bosubr_u(i0,r0,r1)	_bosubr_u(_jit,i0,r0,r1)
static jit_word_t _bosubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bosubi_u(i0,r0,i1)	_bosubi_u(_jit,i0,r0,i1)
static jit_word_t _bosubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bxsubr(i0,r0,r1)	_bxsubr(_jit,i0,r0,r1)
static jit_word_t _bxsubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bxsubi(i0,r0,i1)	_bxsubi(_jit,i0,r0,i1)
static jit_word_t _bxsubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bxsubr_u(i0,r0,r1)	_bxsubr_u(_jit,i0,r0,r1)
static jit_word_t _bxsubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bxsubi_u(i0,r0,i1)	_bxsubi_u(_jit,i0,r0,i1)
static jit_word_t _bxsubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define jmpr(r0)		_jmpr(_jit,r0)
static void _jmpr(jit_state_t*,jit_int32_t);
#define jmpi(i0)		_jmpi(_jit,i0)
static void _jmpi(jit_state_t*,jit_word_t);
#define jmpi_p(i0)		_jmpi_p(_jit,i0)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t);
#define callr(r0)		_callr(_jit,r0)
static void _callr(jit_state_t*,jit_int32_t);
#define calli(i0)		_calli(_jit,i0)
static void _calli(jit_state_t*,jit_word_t);
#define calli_p(i0)		_calli_p(_jit,i0)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#define prolog(node)		_prolog(_jit, node)
static void _prolog(jit_state_t*, jit_node_t*);
#define epilog(node)		_epilog(_jit, node)
static void _epilog(jit_state_t*, jit_node_t*);
#define vastart(r0)		_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#define vaarg(r0, r1)		_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#define patch_at(i,l)		_patch_at(_jit,i,l)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
#endif

#if CODE
/* from binutils bfd/libhppa.h */
static inline int
sign_extend (int x, int len)
{
  int signbit = (1 << (len - 1));
  int mask = (signbit << 1) - 1;
  return ((x & mask) ^ signbit) - signbit;
}

static inline int
sign_unext (int x, int len)
{
  int len_ones;

  len_ones = (1 << len) - 1;

  return x & len_ones;
}

static inline int
low_sign_unext (int x, int len)
{
  int temp;
  int sign;

  sign = (x >> (len-1)) & 1;

  temp = sign_unext (x, len-1);

  return (temp << 1) | sign;
}

static inline int
re_assemble_3 (int as3)
{
  return ((  (as3 & 4) << (13-2))
	  | ((as3 & 3) << (13+1)));
}

static inline int
re_assemble_12 (int as12)
{
  return ((  (as12 & 0x800) >> 11)
	  | ((as12 & 0x400) >> (10 - 2))
	  | ((as12 & 0x3ff) << (1 + 2)));
}

static inline int
re_assemble_16 (int as16)
{
  int s, t;

  /* Unusual 16-bit encoding, for wide mode only.  */
  t = (as16 << 1) & 0xffff;
  s = (as16 & 0x8000);
  return (t ^ s ^ (s >> 1)) | (s >> 15);
}

static inline int
re_assemble_17 (int as17)
{
  return ((  (as17 & 0x10000) >> 16)
	  | ((as17 & 0x0f800) << (16 - 11))
	  | ((as17 & 0x00400) >> (10 - 2))
	  | ((as17 & 0x003ff) << (1 + 2)));
}

static inline int
re_assemble_21 (int as21)
{
  return ((  (as21 & 0x100000) >> 20)
	  | ((as21 & 0x0ffe00) >> 8)
	  | ((as21 & 0x000180) << 7)
	  | ((as21 & 0x00007c) << 14)
	  | ((as21 & 0x000003) << 12));
}

static inline int
re_assemble_22 (int as22)
{
  return ((  (as22 & 0x200000) >> 21)
	  | ((as22 & 0x1f0000) << (21 - 16))
	  | ((as22 & 0x00f800) << (16 - 11))
	  | ((as22 & 0x000400) >> (10 - 2))
	  | ((as22 & 0x0003ff) << (1 + 2)));
}

static void
_f1(jit_state_t *_jit, jit_int32_t o,
    jit_int32_t b, jit_int32_t t, jit_int32_t i)
{
    assert(!(o  & ~0x3f));
    assert(!(b  & ~0x1f));
    assert(!(t  & ~0x1f));
    assert(i >= -8192 && i <= 8191);
    ii((o<<26)|(b<<21)|(t<<16)|(re_assemble_16(i)));
}

static void
_f2(jit_state_t *_jit, jit_int32_t o,
    jit_int32_t b, jit_int32_t t, jit_int32_t i, jit_int32_t j)
{
    assert(!(o  & ~0x3f));
    assert(!(b  & ~0x1f));
    assert(!(t  & ~0x1f));
    assert(i >= -32768 && i <= 32767);
    assert(!(j  &  ~0x3));
    ii((o<<26)|(b<<21)|(t<<16)|(j<<1)|(re_assemble_16(i)));
}

static void
_f3(jit_state_t *_jit, jit_int32_t o,
    jit_int32_t b, jit_int32_t t, jit_int32_t i, jit_int32_t j)
{
    assert(!(o  & ~0x3f));
    assert(!(b  & ~0x1f));
    assert(!(t  & ~0x1f));
    assert(i >= -32768 && i <= 32767);
    assert(!(j  &  ~0x1));
    ii((o<<26)|(b<<21)|(t<<16)|(j<<1)|(re_assemble_16(i)));
}

static void
_f4(jit_state_t *_jit, jit_int32_t o, jit_int32_t b,
    jit_int32_t x, jit_int32_t s, jit_int32_t u, jit_int32_t y,
    jit_int32_t c, jit_int32_t z, jit_int32_t m, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(b  & ~0x1f));
    assert(!(x  & ~0x1f));
    assert(!(s  &  ~0x3));
    assert(!(u  &  ~0x1));
    assert(!(y  &  ~0x1));
    assert(!(c  &  ~0x3));
    assert(!(z  &  ~0xf));
    assert(!(m  &  ~0x1));
    assert(!(t  & ~0x1f));
    ii((o<<26)|(b<<21)|(x<<16)|(s<<14)|(u<<13)|(y<<12)|(c<<10)|(z<<6)|(m<<5)|t);
}

static void
_f5(jit_state_t *_jit, jit_int32_t o, jit_int32_t b,
    jit_int32_t i, jit_int32_t s, jit_int32_t a, jit_int32_t y,
    jit_int32_t c, jit_int32_t z, jit_int32_t m, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(b  & ~0x1f));
    assert(!(s  &  ~0x3));
    assert(!(a  &  ~0x1));
    assert(!(y  &  ~0x1));
    assert(!(c  &  ~0x3));
    assert(!(z  &  ~0xf));
    assert(!(m  &  ~0x1));
    assert(i >= -16 && i <= 15);
    ii((o<<26)|(b<<21)|(low_sign_unext(i,5)<<16)|
       (s<<14)|(a<<13)|(y<<12)|(c<<10)|(z<<6)|(m<<5)|t);
}

static void
_f6(jit_state_t *_jit, jit_int32_t o, jit_int32_t b,
    jit_int32_t r, jit_int32_t s, jit_int32_t a, jit_int32_t x,
    jit_int32_t c, jit_int32_t y, jit_int32_t m, jit_int32_t i)
{
    assert(!(o  & ~0x3f));
    assert(!(b  & ~0x1f));
    assert(!(s  &  ~0x3));
    assert(!(a  &  ~0x1));
    assert(!(x  &  ~0x1));
    assert(!(c  &  ~0x3));
    assert(!(y  &  ~0xf));
    assert(!(m  &  ~0x1));
    assert(i >= -16 && i <= 15);
    ii((o<<26)|(b<<21)|(r<<16)|(s<<14)|(a<<13)|
       (x<<12)|(c<<10)|(y<<6)|(m<<5)|low_sign_unext(i,5));
}

static void
_f7(jit_state_t *_jit, jit_int32_t o, jit_int32_t r, jit_int32_t i)
{
    assert(!(o  & ~0x3f));
    assert(!(r  & ~0x1f));
    assert(!(i  &  0x7ff));
    ii((o<<26)|(r<<21)|re_assemble_21(i>>11));
}

static void
_f8(jit_state_t *_jit, jit_int32_t o,
    jit_int32_t r2, jit_int32_t r1, jit_int32_t cf,
    jit_int32_t e1, jit_int32_t x, jit_int32_t e2,
    jit_int32_t e3, jit_int32_t d, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(r2 & ~0x1f));
    assert(!(r1 & ~0x1f));
    assert(!(cf &  ~0xf));
    assert(!(e1 &  ~0x3));
    assert(!(x  &  ~0x1));
    assert(!(e2 &  ~0x3));
    assert(!(e3 &  ~0x3));
    assert(!(d  &  ~0x1));
    assert(!(t  & ~0x1f));
    ii((o<<26)|(r2<<21)|(r1<<16)|(cf<<12)|
       (e1<<10)|(x<<9)|(e2<<8)|(e3<<6)|(d<<5)|t);
}

static void
_f9(jit_state_t *_jit,
    jit_int32_t o, jit_int32_t r, jit_int32_t t,
    jit_int32_t cf, jit_int32_t e1, jit_int32_t i)
{
    assert(!(o  & ~0x3f));
    assert(!(r  & ~0x1f));
    assert(!(t  & ~0x1f));
    assert(!(cf &  ~0xf));
    assert(!(e1 &  ~0x1));
    assert(i >= -2048 && i <= 2047);
    ii((o<<26)|(r<<21)|(t<<16)|(cf<<12)|(e1<<11)|low_sign_unext(i,11));
}

static void
_f10(jit_state_t *_jit, jit_int32_t o, jit_int32_t r2,
     jit_int32_t r1, jit_int32_t u, jit_int32_t v, jit_int32_t w,
     jit_int32_t x, jit_int32_t sa, jit_int32_t y, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(r2 & ~0x1f));
    assert(!(r1 & ~0x1f));
    assert(!(u  &  ~0x1));
    assert(!(v  &  ~0x3));
    assert(!(w  &  ~0x1));
    assert(!(x  &  ~0x3));
    assert(!(sa &  ~0xf));
    assert(!(y  &  ~0x1));
    assert(!(t  & ~0x1f));
    ii((o<<26)|(r2<<21)|(r1<<16)|(u<<15)|
       (v<<13)|(w<<12)|(x<<10)|(sa<<6)|(y<<5)|t);
}

static void
_f11(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t r2, jit_int32_t r1, jit_int32_t c, jit_int32_t x,
     jit_int32_t y, jit_int32_t z, jit_int32_t u, jit_int32_t t)
{
    assert(!(o  & ~0x3f));
    assert(!(r1 & ~0x1f));
    assert(!(r2 & ~0x1f));
    assert(!(c  &  ~0x7));
    assert(!(x  &  ~0x3));
    assert(!(y  &  ~0x1));
    assert(!(z  &  ~0x1));
    assert(!(u  &  ~0xf));
    assert(!(t  & ~0x1f));
    ii((o<<26)|(r2<<21)|(r1<<16)|(c<<13)|(x<<11)|(y<<10)|(z<<9)|(u<<5)|t);
}

static void
_f12(jit_state_t *_jit, jit_int32_t o, jit_int32_t r,
     jit_int32_t t, jit_int32_t c, jit_int32_t x, jit_int32_t se,
     jit_int32_t y, jit_int32_t c1, jit_int32_t z, jit_int32_t clen)
{
    assert(!(o    & ~0x3f));
    assert(!(r    & ~0x1f));
    assert(!(t    & ~0x1f));
    assert(!(c    &  ~0x7));
    assert(!(x    &  ~0x3));
    assert(!(se   &  ~0x1));
    assert(!(y    &  ~0x1));
    assert(!(c1   &  ~0x1));
    assert(!(z    &  ~0x7));
    assert(!((32-clen) & ~0x1f));
    ii((o<<26)|(r<<21)|(t<<16)|(c<<13)|(x<<11)|
       (se<<10)|(y<<9)|(c1<<8)|(z<<5)|(32-clen));
}

static void
_f13(jit_state_t *_jit, jit_int32_t o, jit_int32_t t,
     jit_int32_t r, jit_int32_t c, jit_int32_t x,
     jit_int32_t nz, jit_int32_t c1, jit_int32_t clen)
{
    assert(!(o    & ~0x3f));
    assert(!(t    & ~0x1f));
    assert(!(r    & ~0x1f));
    assert(!(c    &  ~0x7));
    assert(!(x    &  ~0x3));
    assert(!(nz   &  ~0x1));
    assert(!(c1   &  ~0x1));
    assert(!((32-clen) & ~0x1f));
    ii((o<<26)|(t<<21)|(r<<16)|(c<<13)|
       (x<<11)|(nz<<10)|(c1<<8)|(32-clen));
}

static void
_f13x(jit_state_t *_jit, jit_int32_t o, jit_int32_t t,
      jit_int32_t i, jit_int32_t c, jit_int32_t x,
      jit_int32_t nz, jit_int32_t c1, jit_int32_t clen)
{
    assert(!(o    & ~0x3f));
    assert(!(t    & ~0x1f));
    assert(i >= -16 && i <= 15);
    assert(!(c    &  ~0x7));
    assert(!(x    &  ~0x3));
    assert(!(nz   &  ~0x1));
    assert(!((32-clen) & ~0x1f));
    ii((o<<26)|(t<<21)|(low_sign_unext(i,5)<<16)|
       (c<<13)|(x<<11)|(nz<<10)|(c1<<8)|(32-clen));
}

static void
_f14(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t r2, jit_int32_t r1, jit_int32_t c, jit_int32_t x,
     jit_int32_t cp, jit_int32_t y, jit_int32_t cpos, jit_int32_t t)
{
    assert(!(o    & ~0x3f));
    assert(!(r1   & ~0x1f));
    assert(!(r2   & ~0x1f));
    assert(!(c    &  ~0x7));
    assert(!(x    &  ~0x3));
    assert(!(cp   &  ~0x1));
    assert(!(y    &  ~0x1));
    assert(!(cpos & ~0x1f));
    assert(!(t    & ~0x1f));
    ii((o<<26)|(r2<<21)|(r1<<16)|(c<<13)|(x<<12)|(cp<<11)|(y<<10)|(cpos<<5)|t);
}

static void
_f15(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t r, jit_int32_t t, jit_int32_t c, jit_int32_t c1,
     jit_int32_t p, jit_int32_t se, jit_int32_t pos, jit_int32_t clen)
{
    assert(!(o    & ~0x3f));
    assert(!(r    & ~0x1f));
    assert(!(t    & ~0x1f));
    assert(!(c    &  ~0x7));
    assert(!(c1   &  ~0x1));
    assert(!(p    &  ~0x1));
    assert(!(se   &  ~0x1));
    assert(!(pos  & ~0x1f));
    assert(!((32-clen) & ~0x1f));
    ii((o<<26)|(r<<21)|(t<<16)|(c<<13)|(c1<<12)|
       (p<<11)|(se<<10)|(pos<<5)|(32-clen));
}

static void
_f16(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t t, jit_int32_t r, jit_int32_t c, jit_int32_t c1,
     jit_int32_t cp, jit_int32_t nz, jit_int32_t cpos, jit_int32_t clen)
{
    assert(!(o    & ~0x3f));
    assert(!(t    & ~0x1f));
    assert(!(r    & ~0x1f));
    assert(!(c    &  ~0x7));
    assert(!(c1   &  ~0x1));
    assert(!(cp   &  ~0x1));
    assert(!(nz   &  ~0x1));
    assert(!(cpos & ~0x1f));
    assert(!((32-clen) & ~0x3f));
    ii((o<<26)|(t<<21)|(r<<16)|(c<<13)|
       (c1<<12)|(cp<<11)|(nz<<10)|(cpos<<5)|(32-clen));
}

static void
_f16x(jit_state_t *_jit, jit_int32_t o,
      jit_int32_t t, jit_int32_t i, jit_int32_t c, jit_int32_t c1,
      jit_int32_t cp, jit_int32_t nz, jit_int32_t cpos, jit_int32_t clen)
{
    assert(!(o    & ~0x3f));
    assert(!(t    & ~0x1f));
    assert(i >= -16 && i <= 15);
    assert(!(c    &  ~0x7));
    assert(!(c1   &  ~0x1));
    assert(!(cp   &  ~0x1));
    assert(!(nz   &  ~0x1));
    assert(!(cpos & ~0x1f));
    assert(!((32-clen) & ~0x3f));
    ii((o<<26)|(t<<21)|(low_sign_unext(i,5)<<16)|(c<<13)|
       (c1<<12)|(cp<<11)|(nz<<10)|(cpos<<5)|(32-clen));
}

static void
_f17(jit_state_t *_jit, jit_int32_t o, jit_int32_t r2,
     jit_int32_t r1, jit_int32_t c, jit_int32_t i, jit_int32_t n)
{
    assert(!(o  & ~0x3f));
    assert(!(r2 & ~0x1f));
    assert(!(r1 & ~0x1f));
    assert(!(c  &  ~0x7));
    assert(i >= -2048 && i <= 2047);
    assert(!(n  &   ~0x1));
    ii((o<<26)|(r2<<21)|(r1<<16)|(c<<13)|re_assemble_12(i)|(n<<1));
}

static void
_f17x(jit_state_t *_jit, jit_int32_t o, jit_int32_t r,
      jit_int32_t i, jit_int32_t c, jit_int32_t t, jit_int32_t n)
{
    assert(!(o & ~0x3f));
    assert(!(r & ~0x1f));
    assert(i >= -16 && i <= 15);
    assert(!(c &  ~0x7));
    assert(t >= -2048 && t <= 2047);
    assert(!(n &  ~0x1));
    ii((o<<26)|(r<<21)|
       (low_sign_unext(i,5)<<16)|(c<<13)|re_assemble_12(t)|(n<<1));
}

static void
_f18(jit_state_t *_jit, jit_int32_t o, jit_int32_t p,
     jit_int32_t r, jit_int32_t c, jit_int32_t i, jit_int32_t n)
{
    assert(!(o & ~0x3f));
    assert(!(p & ~0x1f));
    assert(!(r & ~0x1f));
    assert(!(c &  ~0x1));
    assert(i >= -2048 && i <= 2047);
    assert(!(n &  ~0x1));
    ii((o<<26)|(p<<21)|(r<<16)|(c<<15)|(1<<14)|re_assemble_12(i)|(n<<1));
}

static void
_f19(jit_state_t *_jit, jit_int32_t o, jit_int32_t b,
     jit_int32_t s, jit_int32_t i, jit_int32_t n)
{
    assert(!(o & ~0x3f));
    assert(!(b & ~0x1f));
    assert(!(s &  ~0x7));
    assert(i >= -65536 && i <= 65535);
    assert(!(n &  ~0x1));
    ii((o<<26)|(b<<21)|re_assemble_3(s)|re_assemble_17(i)|(n<<1));
}

static void
_f20(jit_state_t *_jit, jit_int32_t o, jit_int32_t t,
     jit_int32_t i, jit_int32_t g, jit_int32_t n)
{
    assert(!(o & ~0x3f));
    assert(!(t & ~0x1f));
    assert(i >= -32768 && i <= 32767);
    assert(!(g &  ~0x7));
    assert(!(n &  ~0x1));
    ii((o<<26)|(t<<21)|(g<<13)|re_assemble_17(i)|(n<<1));
}

static void
_f21(jit_state_t *_jit, jit_int32_t o, jit_int32_t t,
     jit_int32_t x, jit_int32_t y, jit_int32_t n)
{
    assert(!(o & ~0x3f));
    assert(!(t & ~0x1f));
    assert(!(x & ~0x1f));
    assert(!(y &  ~0x7));
    assert(!(n &  ~0x1));
    ii((o<<26)|(t<<21)|(x<<16)|(y<<13)|(n<<1));
}

static void
_f22(jit_state_t *_jit, jit_int32_t o, jit_int32_t b,
     jit_int32_t x, jit_int32_t r, jit_int32_t n, jit_int32_t p)
{
    assert(!(o & ~0x3f));
    assert(!(b & ~0x1f));
    assert(!(x &  ~0x7));
    assert(!(r & ~0x3f));
    assert(!(n &  ~0x1));
    assert(!(p &  ~0x1));
    ii((o<<26)|(b<<21)|(x<<13)|(1<<12)|(r<<2)|(n<<1)|p);
}

static void
_f23(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t a, jit_int32_t b, jit_int32_t c, jit_int32_t d,
     jit_int32_t e, jit_int32_t f, jit_int32_t g, jit_int32_t h)
{
    assert(!(o &  ~0x3f));
    assert(!(a &  ~0x1f));
    assert(!(b &  ~0x1f));
    assert(!(c &   ~0x7));
    assert(!(d &   ~0x1));
    assert(!(e & ~0x1ff));
    assert(!(f &   ~0x1));
    assert(!(g &   ~0x1));
    assert(!(h &   ~0x1));
    ii((o<<26)|(a<<21)|(b<<16)|(c<<13)|(d<<12)|(e<<3)|(f<<2)|(g<<1)|h);
}

static void
_f24(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t b, jit_int32_t x, jit_int32_t s,
     jit_int32_t y, jit_int32_t m,jit_int32_t r)
{
    assert(!(o &  ~0x3f));
    assert(!(b &  ~0x1f));
    assert(!(x &  ~0x1f));
    assert(!(s &   ~0x3));
    assert(!(y &  ~0xff));
    assert(!(m &   ~0x1));
    assert(!(r &  ~0x1f));
    ii((o<<26)|(b<<21)|(x<<16)|(s<<14)|(y<<6)|(m<<5)|r);
}

static void
_f25(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t b, jit_int32_t i, jit_int32_t s,
     jit_int32_t y, jit_int32_t m, jit_int32_t r)
{
    assert(!(o &  ~0x3f));
    assert(!(b &  ~0x1f));
    assert(i >= -16 && i <= 15);
    assert(!(s &   ~0x3));
    assert(!(y &  ~0xff));
    assert(!(m &   ~0x1));
    assert(!(r &  ~0x1f));
    ii((o<<26)|(b<<21)|(low_sign_unext(i,5)<<16)|(s<<14)|(y<<6)|(m<<5)|r);
}

static void
_f26(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t b, jit_int32_t x, jit_int32_t s,
     jit_int32_t y, jit_int32_t m,jit_int32_t r)
{
    assert(!(o &  ~0x3f));
    assert(!(b &  ~0x1f));
    assert(!(x &  ~0x1f));
    assert(!(s &   ~0x7));
    assert(!(y &  ~0x7f));
    assert(!(m &   ~0x1));
    assert(!(r &  ~0x1f));
    ii((o<<26)|(b<<21)|(x<<16)|(s<<13)|(y<<6)|(m<<5)|r);
}

static void
_f27(jit_state_t *_jit, jit_int32_t o, jit_int32_t i, jit_int32_t j)
{
    assert(!(o & ~0x3f));
    assert(i >= -4096 && i < 4095);
    assert(j >=   -16 && j <   15);
    ii((o<<26)|(i<<13)|j);
}

static void
_f28(jit_state_t *_jit, jit_int32_t o, jit_int32_t i)
{
    assert(!(o &      ~0x3f));
    assert(!(i & ~0x1ffffff));
    ii((o<<26)|i);
}

static void
_f29(jit_state_t *_jit, jit_int32_t o, jit_int32_t r,
     jit_int32_t x, jit_int32_t s, jit_int32_t y, jit_int32_t t)
{
    assert(!(o & ~0x3f));
    assert(!(r & ~0x1f));
    assert(!(x & ~0x1f));
    assert(!(s &  ~0x7));
    assert(!(y & ~0xff));
    assert(!(t & ~0x1f));
    ii((o<<26)|(r<<21)|(x<<16)|re_assemble_3(s)|(y<<5)|t);
}

static void
_f30(jit_state_t *_jit, jit_int32_t o, jit_int32_t b, jit_int32_t r,
     jit_int32_t s, jit_int32_t x, jit_int32_t y, jit_int32_t t)
{
    assert(!(o & ~0x3f));
    assert(!(b & ~0x1f));
    assert(!(r & ~0x1f));
    assert(!(s &  ~0x3));
    assert(!(x &  ~0x1));
    assert(!(y & ~0xff));
    assert(!(t & ~0x1f));
    ii((o<<26)|(b<<21)|(r<<16)|(s<<14)|(x<<13)|(y<<5)|t);
}

static void
_f31(jit_state_t *_jit, jit_int32_t o, jit_int32_t t,
     jit_int32_t r, jit_int32_t v, jit_int32_t x, jit_int32_t y)
{
    assert(!(o & ~0x3f));
    assert(!(t & ~0x1f));
    assert(!(r & ~0x1f));
    assert(!(v & ~0x1f));
    assert(!(x & ~0xff));
    assert(!(y & ~0x1f));
    ii((o<<26)|(t<<21)|(r<<16)|(v<<14)|(x<<5)|y);
}

static void
_f33(jit_state_t *_jit, jit_int32_t o, jit_int32_t x,
     jit_int32_t r, jit_int32_t y, jit_int32_t z, jit_int32_t u)
{
    assert(!(o & ~0x3f));
    assert(!(x & ~0x1f));
    assert(!(r & ~0x1f));
    assert(!(y &  ~0x7));
    assert(!(z & ~0xff));
    assert(!(u & ~0x1f));
    ii((o<<26)|(x<<21)|(r<<16)|(y<<13)|(z<<5)|u);
}

static void
_f34(jit_state_t *_jit, jit_int32_t o, jit_int32_t o1,
     jit_int32_t x, jit_int32_t sf, jit_int32_t n, jit_int32_t o2)
{
    assert(!(o  &   ~0x3f));
    assert(!(o1 & ~0x7fff));
    assert(!(x  &    ~0x3));
    assert(!(sf &    ~0x7));
    assert(!(n  &    ~0x1));
    assert(!(o2 &   ~0x1f));
    ii((o<<26)|(o1<<11)|(x<<9)|(sf<<6)|(n<<5)|o2);
}

static void
_f35(jit_state_t *_jit, jit_int32_t o, jit_int32_t op,
     jit_int32_t x, jit_int32_t sf, jit_int32_t n, jit_int32_t t)
{
    assert(!(o  &   ~0x3f));
    assert(!(op & ~0x7fff));
    assert(!(x  &    ~0x3));
    assert(!(sf &    ~0x7));
    assert(!(n  &    ~0x1));
    assert(!(t  &   ~0x1f));
    ii((o<<26)|(op<<11)|(x<<9)|(sf<<6)|(n<<5)|t);
}

static void
_f36(jit_state_t *_jit, jit_int32_t o, jit_int32_t r, jit_int32_t o1,
     jit_int32_t x, jit_int32_t sf, jit_int32_t n, jit_int32_t o2)
{
    assert(!(o  &  ~0x3f));
    assert(!(r  &  ~0x1f));
    assert(!(o1 & ~0x3ff));
    assert(!(x  &   ~0x3));
    assert(!(sf &   ~0x7));
    assert(!(n  &   ~0x1));
    assert(!(o2 &  ~0x1f));
    ii((o<<26)|(r<<21)|(o1<<11)|(x<<9)|(sf<<6)|(n<<5)|o2);
}

static void
_f37(jit_state_t *_jit, jit_int32_t o, jit_int32_t r2,
     jit_int32_t r1, jit_int32_t o1, jit_int32_t x,
     jit_int32_t sf, jit_int32_t n, jit_int32_t o2)
{
    assert(!(o  & ~0x3f));
    assert(!(r2 & ~0x1f));
    assert(!(r1 & ~0x1f));
    assert(!(o1 & ~0x1f));
    assert(!(x  &  ~0x3));
    assert(!(sf &  ~0x7));
    assert(!(n  &  ~0x1));
    assert(!(o2 & ~0x1f));
    ii((o<<26)|(r2<<21)|(r1<<16)|(o1<<11)|(x<<9)|(sf<<6)|(n<<5)|o2);
}

static void
_f38(jit_state_t *_jit, jit_int32_t o,
     jit_int32_t s, jit_int32_t u, jit_int32_t n)
{
    assert(!(o &     ~0x3f));
    assert(!(s & ~0x3fffff));
    assert(!(u &      ~0x7));
    assert(!(n &      ~0x1));
    ii((o<<26)|((s>>5)<<9)|(u<<6)|(n<<1)|(s&0x1f));
}

static void
_nop(jit_state_t *_jit, jit_int32_t i0)
{
    for (; i0 > 0; i0 -= 4)
	NOP();
    assert(i0 == 0);
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	COPY(r1, r0);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    if (i0 >= -8192 && i0 <= 8191)
	LDI(i0, r0);
    else if (!(i0 & 0x7ff))
	LDIL(i0, r0);
    else {
	LDIL(i0 & ~0x7ff, r0);
	LDO(i0 & 0x7ff, r0, r0);
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    w = _jit->pc.w;
    LDIL(i0 & ~0x7ff, r0);
    LDO(i0 & 0x7ff, r0, r0);
    return (w);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -1024 && i0 <= 1023)
	ADDI(i0, r1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    addcr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    addxr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -1023 && i0 <= 1024)
	addi(r0, r1, -i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    subcr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    subxr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_rsbi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -1024 && i0 <= 1023)
	SUBI(i0, r1, r0);
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
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_fpr);
    t1 = jit_get_reg(jit_class_fpr);
    stxi(alloca_offset - 8, _FP_REGNO, r1);
    ldxi_f(rn(t0), _FP_REGNO, alloca_offset - 8);
    stxi(alloca_offset - 8, _FP_REGNO, r2);
    ldxi_f(rn(t1), _FP_REGNO, alloca_offset - 8);
    XMPYU(rn(t0), rn(t1), rn(t0));
    stxi_d(alloca_offset - 8, _FP_REGNO, rn(t0));
    ldxi(r0, _FP_REGNO, alloca_offset - 4);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (r0 != r1) {
	movi(r0, i0);
	mulr(r0, r1, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	mulr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static long long
__llmul(int u, int v)
{
    return ((long long)u * (long long)v);
}

static void
_qmulr(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    movr(_R26_REGNO, r2);
    movr(_R25_REGNO, r3);
    calli((jit_word_t)__llmul);
    movr(r0, _R29_REGNO);
    movr(r1, _R28_REGNO);
}

static void
_qmuli(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    movr(_R26_REGNO, r2);
    movi(_R25_REGNO, i0);
    calli((jit_word_t)__llmul);
    movr(r0, _R29_REGNO);
    movr(r1, _R28_REGNO);
}

static void
_qmulr_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_fpr);
    t1 = jit_get_reg(jit_class_fpr);
    stxi(alloca_offset - 8, _FP_REGNO, r2);
    ldxi_f(rn(t0), _FP_REGNO, alloca_offset - 8);
    stxi(alloca_offset - 8, _FP_REGNO, r3);
    ldxi_f(rn(t1), _FP_REGNO, alloca_offset - 8);
    XMPYU(rn(t0), rn(t1), rn(t0));
    stxi_d(alloca_offset - 8, _FP_REGNO, rn(t0));
    ldxi(r0, _FP_REGNO, alloca_offset - 4);
    ldxi(r1, _FP_REGNO, alloca_offset - 8);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}

static void
_qmuli_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    qmulr_u(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
}

static int
__idiv(int u, int v)
{
    return (u / v);
}

static void
_divr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_R26_REGNO, r1);
    movr(_R25_REGNO, r2);
    calli((jit_word_t)__idiv);
    movr(r0, _R28_REGNO);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_R26_REGNO, r1);
    movi(_R25_REGNO, i0);
    calli((jit_word_t)__idiv);
    movr(r0, _R28_REGNO);
}

static unsigned int
__udiv(unsigned int u, unsigned int v)
{
    return (u / v);
}

static void
_divr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_R26_REGNO, r1);
    movr(_R25_REGNO, r2);
    calli((jit_word_t)__udiv);
    movr(r0, _R28_REGNO);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_R26_REGNO, r1);
    movi(_R25_REGNO, i0);
    calli((jit_word_t)__udiv);
    movr(r0, _R28_REGNO);
}

static int
__irem(int u, int v)
{
    return (u % v);
}

static void
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_R26_REGNO, r1);
    movr(_R25_REGNO, r2);
    calli((jit_word_t)__irem);
    movr(r0, _R28_REGNO);
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_R26_REGNO, r1);
    movi(_R25_REGNO, i0);
    calli((jit_word_t)__irem);
    movr(r0, _R28_REGNO);
}

static unsigned int
__urem(unsigned int u, unsigned int v)
{
    return (u % v);
}

static void
_remr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_R26_REGNO, r1);
    movr(_R25_REGNO, r2);
    calli((jit_word_t)__urem);
    movr(r0, _R28_REGNO);
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_R26_REGNO, r1);
    movi(_R25_REGNO, i0);
    calli((jit_word_t)__urem);
    movr(r0, _R28_REGNO);
}

static idiv_t
__idivrem(int u, int v)
{
    idiv_t	div;
    div.quo = u / v;
    div.rem = u % v;
    return (div);
}

static void
_qdivr(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    movr(_R26_REGNO, r2);
    movr(_R25_REGNO, r3);
    calli((jit_word_t)__idivrem);
    movr(r0, _R28_REGNO);
    movr(r1, _R29_REGNO);
}

static void
_qdivi(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    movr(_R26_REGNO, r2);
    movi(_R25_REGNO, i0);
    calli((jit_word_t)__idivrem);
    movr(r0, _R28_REGNO);
    movr(r1, _R29_REGNO);
}

static udiv_t
__udivrem(unsigned int u, unsigned int v)
{
    udiv_t	div;
    div.quo = u / v;
    div.rem = u % v;
    return (div);
}

static void
_qdivr_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    movr(_R26_REGNO, r2);
    movr(_R25_REGNO, r3);
    calli((jit_word_t)__udivrem);
    movr(r0, _R28_REGNO);
    movr(r1, _R29_REGNO);
}

static void
_qdivi_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    movr(_R26_REGNO, r2);
    movi(_R25_REGNO, i0);
    calli((jit_word_t)__udivrem);
    movr(r0, _R28_REGNO);
    movr(r1, _R29_REGNO);
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    andr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    orr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    xorr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_lshr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    SUBI(0x1f, r2, _R1_REGNO);
    MTSAR(_R1_REGNO);
    DEPW_Z(r1, 32, r0);
}

static void
_rshr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    SUBI(0x1f, r2, _R1_REGNO);
    MTSAR(_R1_REGNO);
    EXTRW(r1, 32, r0);
}

static void
_rshr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    SUBI(0x1f, r2, _R1_REGNO);
    MTSAR(_R1_REGNO);
    EXTRW_U(r1, 32, r0);
}

static void
_cmpr(jit_state_t *_jit, jit_word_t c,
      jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMPCLR_(c, r1, r2, r0);
    LDI(1, r0);
}

static void
_cmpi(jit_state_t *_jit, jit_word_t c, jit_word_t ci,
      jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	CMPICLR_(ci, i0, r1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMPCLR_(c, r1, rn(reg), r0);
	jit_unget_reg(reg);
    }
    LDI(1, r0);
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
    ldi_uc(r0, i0);
    extr_c(r0, r0);
}

static void
_ldxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    ldxr_uc(r0, r1, r2);
    extr_c(r0, r0);
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    ldxi_uc(r0, r1, i0);
    extr_c(r0, r0);
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -8182 && i0 <= 8191)
	LDBL(i0, _R0_REGNO, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	LDIL(i0 & ~0x7ff, rn(reg));
	LDBL(sign_extend(i0, 11), rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	LDBI(i0, r1, r0);
    else if (i0 >= -8182 && i0 <= 8191)
	LDBL(i0, r1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_uc(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
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
    ldi_us(r0, i0);
    extr_s(r0, r0);
}

static void
_ldxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    ldxr_us(r0, r1, r2);
    extr_s(r0, r0);
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    ldxi_us(r0, r1, i0);
    extr_s(r0, r0);
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -8182 && i0 <= 8191)
	LDHL(i0, _R0_REGNO, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	LDIL(i0 & ~0x7ff, rn(reg));
	LDHL(sign_extend(i0, 11), rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	LDHI(i0, r1, r0);
    else if (i0 >= -8182 && i0 <= 8191)
	LDHL(i0, r1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_us(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -8182 && i0 <= 8191)
	LDWL(i0, _R0_REGNO, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	LDIL(i0 & ~0x7ff, rn(reg));
	LDWL(sign_extend(i0, 11), rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_ldxi_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= -16 && i0 <= 15)
	LDWI(i0, r1, r0);
    else if (i0 >= -8182 && i0 <= 8191)
	LDWL(i0, r1, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_ui(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (i0 >= -8182 && i0 <= 8191)
	STBL(r0, i0, _R0_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	LDIL(i0 & ~0x7ff, rn(reg));
	STBL(r0, sign_extend(i0, 11), rn(reg));
	jit_unget_reg(reg);
    }
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
    if (i0 >= -16 && i0 <= 15)
	STBI(r1, i0, r0);
    else if (i0 >= -8182 && i0 <= 8191)
	STBL(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_c(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (i0 >= -8182 && i0 <= 8191)
	STHL(r0, i0, _R0_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	LDIL(i0 & ~0x7ff, rn(reg));
	STHL(r0, sign_extend(i0, 11), rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_stxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
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
    if (i0 >= -16 && i0 <= 15)
	STHI(r1, i0, r0);
    else if (i0 >= -8182 && i0 <= 8191)
	STHL(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_s(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (i0 >= -8182 && i0 <= 8191)
	STWL(r0, i0, _R0_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	LDIL(i0 & ~0x7ff, rn(reg));
	STWL(r0, sign_extend(i0, 11), rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_stxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
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
    if (i0 >= -16 && i0 <= 15)
	STWI(r1, i0, r0);
    else if (i0 >= -8182 && i0 <= 8191)
	STWL(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_i(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static jit_word_t
_bcmpr(jit_state_t *_jit, jit_word_t c,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    CMPB_N_(c, r0, r1, ((i0 - w) >> 2) - 2);
    NOP();
    return (w);
}

static jit_word_t
_bcmpi(jit_state_t *_jit, jit_word_t c, jit_word_t ci,
       jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -16 && i1 <= 15) {
	w = _jit->pc.w;
	CMPIB_N_(ci, i1, r0, ((i0 - w) >> 2) - 2);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = _jit->pc.w;
	CMPB_N_(c, r0, rn(reg), ((i0 - w) >> 2) - 2);
	jit_unget_reg(reg);
    }
    NOP();
    return (w);
}

static jit_word_t
_bmxr(jit_state_t *_jit, jit_bool_t c,
      jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    andr(rn(reg), r0, r1);
    w = c ? bnei(i0, rn(reg), 0) : beqi(i0, rn(reg), 0);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bmxi(jit_state_t *_jit, jit_bool_t c,
      jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    andr(rn(reg), r0, rn(reg));
    w = c ? bnei(i0, rn(reg), 0) : beqi(i0, rn(reg), 0);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_boaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    ADDB_N_SV(r1, r0, ((i0 - w) >> 2) - 2);
    NOP();
    return (w);
}

static jit_word_t
_boaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -16 && i1 <= 15) {
	w = _jit->pc.w;
	ADDIB_N_SV(i1, r0, ((i0 - w) >> 2) - 2);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = boaddr(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_boaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    ADDB_N_UV(r1, r0, ((i0 - w) >> 2) - 2);
    NOP();
    return (w);
}

static jit_word_t
_boaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -16 && i1 <= 15) {
	w = _jit->pc.w;
	ADDIB_N_UV(i1, r0, ((i0 - w) >> 2) - 2);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = boaddr_u(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bxaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    ADDB_N_NSV(r1, r0, ((i0 - w) >> 2) - 2);
    NOP();
    return (w);
}

static jit_word_t
_bxaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -16 && i1 <= 15) {
	w = _jit->pc.w;
	ADDIB_N_NSV(i1, r0, ((i0 - w) >> 2) - 2);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = bxaddr(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bxaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    ADDB_N_NUV(r1, r0, ((i0 - w) >> 2) - 2);
    NOP();
    return (w);
}

static jit_word_t
_bxaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 >= -16 && i1 <= 15) {
	w = _jit->pc.w;
	ADDIB_N_NUV(i1, r0, ((i0 - w) >> 2) - 2);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = bxaddr_u(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bosubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    SUB_NSV(r0, r1, r0);
    w = _jit->pc.w;
    /* null'ed if no signed overflow */
    B_N(((i0 - w) >> 2) - 2, _R0_REGNO);
    NOP();
    return (w);
}

static jit_word_t
_bosubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    movi(_R1_REGNO, i1);
    return (bosubr(i0, r0, _R1_REGNO));
}

static jit_word_t
_bosubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    SUB(r0, r1, r0);
    SUB_B(_R0_REGNO, _R0_REGNO, _R1_REGNO);
    return (bnei(i0, _R1_REGNO, 0));
}

static jit_word_t
_bosubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    movi(_R1_REGNO, i1);
    SUB(r0, _R1_REGNO, r0);
    SUB_B(_R0_REGNO, _R0_REGNO, _R1_REGNO);
    return (bnei(i0, _R1_REGNO, 0));
}

static jit_word_t
_bxsubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    SUB_SV(r0, r1, r0);
    w = _jit->pc.w;
    /* null'ed if signed overflow */
    B_N(((i0 - w) >> 2) - 2, _R0_REGNO);
    NOP();
    return (w);
}

static jit_word_t
_bxsubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    movi(_R1_REGNO, i1);
    return (bxsubr(i0, r0, _R1_REGNO));
}

static jit_word_t
_bxsubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    SUB(r0, r1, r0);
    SUB_B(_R0_REGNO, _R0_REGNO, _R1_REGNO);
    return (beqi(i0, _R1_REGNO, 0));
}

static jit_word_t
_bxsubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    movi(_R1_REGNO, i1);
    SUB(r0, _R1_REGNO, r0);
    SUB_B(_R0_REGNO, _R0_REGNO, _R1_REGNO);
    return (beqi(i0, _R1_REGNO, 0));
}

static void
_jmpr(jit_state_t *_jit, jit_int32_t r0)
{
    BV_N(_R0_REGNO, r0);
}

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    w = ((i0 - _jit->pc.w) >> 2) - 2;
    if (w >= -32768 && w <= 32767)
	B_N(w, _R0_REGNO);
    else {
	movi(_R1_REGNO, w);
	BV_N(_R0_REGNO, _R1_REGNO);
    }
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    w = movi_p(_R1_REGNO, i0);
    jmpr(_R1_REGNO);
    return (w);
}

static void
_callr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_word_t		dyn;
    jit_word_t		imm;
    if (r0 != _R1_REGNO)
	COPY(r0, _R1_REGNO);
    /* inline $$dyncall */
    imm = _jit->pc.w;
    BBI_N_GE(_R1_REGNO, 30, 0);		/*   if (r1 & 2) {		*/
    DEPWRI(0, 31, 2, _R1_REGNO);	/*	r1 &= ~2;		*/
    LDWI(4, _R1_REGNO, _R19_REGNO);	/*	r19 = r1[1];		*/
    LDWI(0, _R1_REGNO, _R1_REGNO);	/*	r1  = r1[0];		*/
					/*   }				*/
    BVE_L(_R1_REGNO);
    STWL(_RP_REGNO, -24, _SP_REGNO);
    dyn = _jit->pc.w;
    jmpi(_jit->pc.w);
    patch_at(imm, _jit->pc.w);
    BVE_L_N(_R1_REGNO);
    NOP();
    patch_at(dyn, _jit->pc.w);
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    if (i0 & 2) {
	i0 &= -4;
	movi(_R1_REGNO, i0);
	LDWI(4, _R1_REGNO, _R19_REGNO);
	LDWI(0, _R1_REGNO, _R1_REGNO);
	BVE_L(_R1_REGNO);
	STWL(_RP_REGNO, -24, _SP_REGNO);
    }
    else {
	w = ((i0 - _jit->pc.w) >> 2) - 2;
	if (w >= -32768 && w <= 32767)
	    B_L_N(w);
	else {
	    movi(_R1_REGNO, i0);
	    BVE_L_N(_R1_REGNO);
	}
	NOP();
    }
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    w = movi_p(_R1_REGNO, i0);
    callr(_R1_REGNO);
    return (w);
}

static jit_int32_t gr[] = {
     _R4,  _R5,  _R6,  _R7,  _R8,
     _R9, _R10, _R11, _R12, _R13,
    _R14, _R15, _R16, _R17, _R18
};

static jit_int32_t fr[] = {
    _F12, _F13, _F14, _F15, _F16,
    _F17, _F18, _F19, _F20, _F21
};

static void
_prolog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		regno;
    jit_word_t		offset;

    if (_jitc->function->define_frame || _jitc->function->assume_frame) {
	/* hppa stack grows up */
	assert(_jitc->function->self.aoff <= _jitc->function->frame);
	if (_jitc->function->assume_frame)
	    return;
	_jitc->function->self.aoff = _jitc->function->frame;
    }
    if (_jitc->function->allocar) {
	_jitc->function->self.aoff += 63;
	_jitc->function->self.aoff &= -64;
    }
    _jitc->function->stack = ((_jitc->function->self.aoff -
			       _jitc->function->self.alen -
			       _jitc->function->self.size) + 63) & -64;

    /* Save stack frame (FIXME Only required if non leaf) */
    STWL(_RP_REGNO, -20, _SP_REGNO);

    /* Create stack frame */
    COPY(_FP_REGNO, _R1_REGNO);
    COPY(_SP_REGNO, _FP_REGNO);
    STWL_MA(_R1_REGNO, _jitc->function->stack, _SP_REGNO);

    /* Save any modified callee save registers */
    offset = alloca_offset - 140;
    for (regno = 0; regno < jit_size(gr); regno++, offset += 4) {
	if (jit_regset_tstbit(&_jitc->function->regset, gr[regno]))
	    stxi(offset, _FP_REGNO, rn(gr[regno]));
    }
    for (regno = 0; regno < jit_size(fr); regno++, offset += 8) {
	if (jit_regset_tstbit(&_jitc->function->regset, fr[regno]))
	    stxi_d(offset, _FP_REGNO, rn(fr[regno]));
    }

    if (_jitc->function->allocar) {
	regno = jit_get_reg(jit_class_gpr);
	movi(rn(regno), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _FP_REGNO, rn(regno));
	jit_unget_reg(regno);
    }

    if (_jitc->function->self.call & jit_call_varargs) {
	for (regno = 3; regno >= _jitc->function->vagp; --regno)
	    stxi(params_offset - regno * 4 - 4, _FP_REGNO, rn(_R26 - regno));
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		regno;
    jit_word_t		offset;

    if (_jitc->function->assume_frame)
	return;
    /* Restore any modified callee save registers */
    offset = alloca_offset - 140;
    for (regno = 0; regno < jit_size(gr); regno++, offset += 4) {
	if (jit_regset_tstbit(&_jitc->function->regset, gr[regno]))
	    ldxi(rn(gr[regno]), _FP_REGNO, offset);
    }
    for (regno = 0; regno < jit_size(fr); regno++, offset += 8) {
	if (jit_regset_tstbit(&_jitc->function->regset, fr[regno]))
	    ldxi_d(rn(fr[regno]), _FP_REGNO, offset);
    }

    /* Restore stack frame (FIXME Only required if non leaf) */
    LDWL(-20, _FP_REGNO, _RP_REGNO);
    LDO(_jitc->function->stack, _FP_REGNO, _SP_REGNO);
    LDWL_MB(-_jitc->function->stack, _SP_REGNO, _FP_REGNO);
#if defined(__hpux)
    BVE_N(_RP_REGNO);
#else
    BV_N(_R0_REGNO, _RP_REGNO);
#endif
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    /* Initialize stack pointer to the first stack argument. */
    if (jit_arg_reg_p(_jitc->function->vagp))
	addi(r0, _FP_REGNO, params_offset - _jitc->function->vagp * 4);
    else
	addi(r0, _FP_REGNO, _jitc->function->self.size);
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);

    /* Update vararg stack pointer. */
    subi(r1, r1, 4);

    /* Load argument. */
    ldr(r0, r1);
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    jit_word_t		 w;
    union {
	jit_uint32_t	*i;
	jit_word_t	 w;
    } u;
    u.w = instr;
    switch (u.i[0] >> 26) {
	case 0x8:				/* LDIL */
	    u.i[0] &= ~0x1fffff;
	    u.i[0] |= re_assemble_21((label & ~0x7ff) >> 11);
	    assert((u.i[1] >> 26) == 0xd);	/* LDO */
	    u.i[1] &= ~0xffff;
	    u.i[1] |= re_assemble_16(label & 0x7ff);
	    break;
	case 0x20:	case 0x22:		/* CMPB */
	case 0x21:	case 0x23:		/* CMPIB */
	case 0x28:	case 0x2a:		/* ADDB */
	case 0x29:	case 0x2b:		/* ADDIB */
	case 0x31:				/* BBI */
	    w = ((label - instr) >> 2) - 2;
	    assert(w >= -2048 && w <= 2047);
	    u.i[0] &= ~0x1ffd;
	    u.i[0] |= re_assemble_12(w);
	    break;
	case 0x3a:				/* B */
	    w = ((label - instr) >> 2) - 2;
	    assert(w >= -32768 && w <= 32767);
	    u.i[0] &= ~0x1f1ffd;
	    u.i[0] |= re_assemble_17(w);
	    break;
	default:
	    abort();
    }
}
#endif
