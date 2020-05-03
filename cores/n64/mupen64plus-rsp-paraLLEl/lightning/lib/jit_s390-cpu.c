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
#  if __WORDSIZE == 32
#    define ldr(r0,r1)			ldr_i(r0,r1)
#    define ldxr(r0,r1,r2)		ldxr_i(r0,r1,r2)
#    define ldxi(r0,r1,i0)		ldxi_i(r0,r1,i0)
#    define stxi(i0,r0,r1)		stxi_i(i0,r0,r1)
#  else
#    define ldr(r0,r1)			ldr_l(r0,r1)
#    define ldxr(r0,r1,r2)		ldxr_l(r0,r1,r2)
#    define ldxi(r0,r1,i0)		ldxi_l(r0,r1,i0)
#    define stxi(i0,r0,r1)		stxi_l(i0,r0,r1)
#  endif
#  define is(i)				*_jit->pc.us++ = i
#  if __WORDSIZE == 32
#    define stack_framesize		96
#  else
#    define stack_framesize		160
#  endif
#  define _R0_REGNO			0
#  define _R1_REGNO			1
#  define _R7_REGNO			7
#  define _R13_REGNO			13
#  define _FP_REGNO			_R13_REGNO
#  define _R14_REGNO			14
#  define _R15_REGNO			15
#  define u12_p(i0)			((i0) >= 0 && (i0) <= 4095)
#  define s16_p(i0)			((i0) >= -32768 && (i0) <= 32767)
#  define x16(i0)			((i0) & 0xffff)
#  define s20_p(i0)			((i0) >= -524288 && (i0) <= 524287)
#  define x20(i0)			((i0) & 0xfffff)
#  if __WORDSIZE == 32
#    define s32_p(i0)			1
#  else
#    define s32_p(i0)							\
    ((i0) >= -2147483648L && (i0) < 2147483647L)
#  endif

/*
	Condition Code		Instruction	(Mask) Bit Mask Value
	0			8		8
	1			9		4
	2			10		2
	3			11		1

AGR:
	0	Zero
	1	< zero
	2	> zero
	3	Overflow
--
1	->	overflow		CC_O
14	->	no overflow		CC_NO

ALGR:
 	0	Zero, no carry
 	1	Not zero, no carry
 	2	Zero, carry
 	3	Not zero, carry
--
2|1	->	carry			CC_NLE
8|4	->	no carry		CC_LE

SGR:
	0	Zero
	1	< zero
	2	> zero
	3	Overflow
--
1	->	overflow		CC_O
14	->	no overflow		CC_NO

SLGR:
	0	--
	1	Not zero, borrow
	2	Zero, no borrow
	3	Not zero, no borrow
--
4	->	borrow			CC_L
11	->	no borrow		CC_NL
  */

#  define CC_NV				0x0
#  define CC_O				0x1
#  define CC_H				0x2
#  define CC_NLE			0x3
#  define CC_L				0x4
#  define CC_NHE			0x5
#  define CC_LH				0x6
#  define CC_NE				0x7
#  define CC_E				0x8
#  define CC_NLH			0x9
#  define CC_HE				0xA
#  define CC_NL				0xB
#  define CC_LE				0xC
#  define CC_NH				0xD
#  define CC_NO				0xE
#  define CC_AL				0xF
#  define _us				jit_uint16_t
#  define _ui				jit_uint32_t
#  define E_(Op)			_E(_jit,Op)
static void _E(jit_state_t*,_ui);
#  define I_(Op,I)			_I(_jit,Op,I)
static void _I(jit_state_t*,_ui,_ui);
#  define RR_(Op,R1,R2)			_RR(_jit,Op,R1,R2)
static void _RR(jit_state_t*,_ui,_ui,_ui);
#  define RRE_(Op,R1,R2)		_RRE(_jit,Op,R1,R2)
static void _RRE(jit_state_t*,_ui,_ui,_ui);
#  define RRF_(Op,R3,M4,R1,R2)		_RRF(_jit,Op,R3,M4,R1,R2)
static void _RRF(jit_state_t*,_ui,_ui,_ui,_ui,_ui);
#  define RX_(Op,R1,X2,B2,D2)		_RX(_jit,Op,R1,X2,B2,D2)
static void _RX(jit_state_t*,_ui,_ui,_ui,_ui,_ui);
#  define RXE_(Op,R1,X2,B2,D2,Op2)	_RXE(_jit,Op,R1,X2,B2,D2,Op2)
static void _RXE(jit_state_t*,_ui,_ui,_ui,_ui,_ui,_ui);
#  define RXF_(Op,R3,X2,B2,D2,R1,Op2)	_RXF(_jit,Op,R3,X2,B2,D2,R1,Op2)
static void _RXF(jit_state_t*,_ui,_ui,_ui,_ui,_ui,_ui,_ui);
#  define RXY_(Op,R1,X2,B2,D2,Op2)	_RXY(_jit,Op,R1,X2,B2,D2,Op2)
static void _RXY(jit_state_t*,_ui,_ui,_ui,_ui,_ui,_ui);
#  define RS_(Op,R1,R3,B2,D2)		_RS(_jit,Op,R1,R3,B2,D2)
static void _RS(jit_state_t*,_ui,_ui,_ui,_ui,_ui);
#  define RSY_(Op,R1,R3,B2,D2,Op2)	RXY_(Op,R1,R3,B2,D2,Op2)
#  define RSL_(Op,L1,B1,D1,Op2)		_RSL(_jit,Op,L1,B1,D1,Op2)
static void _RSL(jit_state_t*,_ui,_ui,_ui,_ui,_ui);
#  define RSI_(Op,R1,R3,I2)		_RSI(_jit,Op,R1,R3,I2)
static void _RSI(jit_state_t*,_ui,_ui,_ui,_ui);
#  define RI_(Op,R1,Op2,I2)		RSI_(Op,R1,Op2,I2)
#  define RIE_(Op,R1,R3,I2,Op2)		_RIE(_jit,Op,R1,R3,I2,Op2)
static void _RIE(jit_state_t*,_ui,_ui,_ui,_ui,_ui);
#  define RIL_(Op,R1,Op2,I2)		_RIL(_jit,Op,R1,Op2,I2)
static void _RIL(jit_state_t*,_ui,_ui,_ui,_ui);
#  define SI_(Op,I2,B1,D1)		_SI(_jit,Op,I2,B1,D1)
static void _SI(jit_state_t*,_ui,_ui,_ui,_ui);
#  define SIY_(Op,I2,B1,D1,Op2)		_SIY(_jit,Op,I2,B1,D1,Op2)
static void _SIY(jit_state_t*,_ui,_ui,_ui,_ui,_ui);
#  define S_(Op,B2,D2)			_S(_jit,Op,B2,D2)
static void _S(jit_state_t*,_ui,_ui,_ui);
#  define SSL_(Op,L,B1,D1,B2,D2)	SS_(Op,(L)>>4,(L)&0xF,B1,D1,B2,D2)
#  define SS_(Op,LL,LH,B1,D1,B2,D2)	_SS(_jit,Op,LL,LH,B1,D1,B2,D2)
static void _SS(jit_state_t*,_ui,_ui,_ui,_ui,_ui,_ui,_ui);
#  define SSE_(Op,B1,D1,B2,D2)		_SSE(_jit,Op,B1,D1,B2,D2)
static void _SSE(jit_state_t*,_ui,_ui,_ui,_ui,_ui);
#  undef _us
#  undef _ui
#  define nop(c)			_nop(_jit,c)
static void _nop(jit_state_t*,jit_int32_t);
#  if __WORDSIZE == 32
#    define ADD_(r0,r1)			AR(r0,r1)
#    define ADDI_(r0,i0)		AHI(r0,i0)
#    define ADDC_(r0,r1)		ALR(r0,r1)
#    define ADDX_(r0,r1)		ALCR(r0,r1)
#    define AND_(r0,r1)			NR(r0,r1)
#    define CMP_(r0,r1)			CR(r0,r1)
#    define CMPU_(r0,r1)		CLR(r0,r1)
#    define DIVREM_(r0,r1)		DR(r0,r1)
#    define DIVREMU_(r0,r1)		DLR(r0,r1)
#    define OR_(r0,r1)			OR(r0,r1)
#    define MUL_(r0,r1)			MSR(r0,r1)
#    define MULI_(r0,i0)		MHI(r0,i0)
#    define MULU_(r0,r1)		MLR(r0,r1)
#    define SUB_(r0,r1)			SR(r0,r1)
#    define SUBC_(r0,r1)		SLR(r0,r1)
#    define SUBX_(r0,r1)		SLBR(r0,r1)
#    define TEST_(r0,r1)		LTR(r0,r1)
#    define XOR_(r0,r1)			XR(r0,r1)
#  else
#    define ADD_(r0,r1)			AGR(r0,r1)
#    define ADDI_(r0,i0)		AGHI(r0,i0)
#    define ADDC_(r0,r1)		ALGR(r0,r1)
#    define ADDX_(r0,r1)		ALCGR(r0,r1)
#    define AND_(r0,r1)			NGR(r0,r1)
#    define CMP_(r0,r1)			CGR(r0,r1)
#    define CMPU_(r0,r1)		CLGR(r0,r1)
#    define DIVREM_(r0,r1)		DSGR(r0,r1)
#    define DIVREMU_(r0,r1)		DLGR(r0,r1)
#    define MUL_(r0,r1)			MSGR(r0,r1)
#    define MULI_(r0,i0)		MGHI(r0,i0)
#    define MULU_(r0,r1)		MLGR(r0,r1)
#    define OR_(r0,r1)			OGR(r0,r1)
#    define SUB_(r0,r1)			SGR(r0,r1)
#    define SUBC_(r0,r1)		SLGR(r0,r1)
#    define SUBX_(r0,r1)		SLBGR(r0,r1)
#    define TEST_(r0,r1)		LTGR(r0,r1)
#    define XOR_(r0,r1)			XGR(r0,r1)
#  endif
/****************************************************************
 * General Instructions						*
 ****************************************************************/
/* ADD */
#  define AR(R1,R2)			RR_(0x1A,R1,R2)
#  define AGR(R1,R2)			RRE_(0xB908,R1,R2)
#  define AGFR(R1,R2)			RRE_(0xB918,R1,R2)
#  define A(R1,D2,X2,B2)		RX_(0x5A,R1,X2,B2,D2)
#  define AY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x5A)
#  define AG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x08)
#  define AGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x18)
/* ADD HALFWORD */
#  define AH(R1,D2,X2,B2)		RX_(0x4A,R1,X2,B2,D2)
#  define AHY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x7A)
/* ADD HALFWORD IMMEDIATE */
#  define AHI(R1,I2)			RI_(0xA7,R1,0xA,I2)
#  define AGHI(R1,I2)			RI_(0xA7,R1,0xB,I2)
/* ADD LOGICAL */
#  define ALR(R1,R2)			RR_(0x1E,R1,R2)
#  define ALGR(R1,R2)			RRE_(0xB90A,R1,R2)
#  define ALGFR(R1,R2)			RRE_(0xB91A,R1,R2)
#  define AL(R1,D2,X2,B2)		RX_(0x5E,R1,X2,B2,D2)
#  define ALY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x5E)
#  define ALG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x0A)
#  define ALGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x1A)
/* ADD LOGICAL WITH CARRY */
#  define ALCR(R1,R2)			RRE_(0xB998,R1,R2)
#  define ALCGR(R1,R2)			RRE_(0xB988,R1,R2)
#  define ALC(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x98)
#  define ALCG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x88)
/* AND */
#  define NR(R1,R2)			RR_(0x14,R1,R2)
#  define NGR(R1,R2)			RRE_(0xB980,R1,R2)
#  define N(R1,D2,X2,B2)		RX_(0x54,R1,X2,B2,D2)
#  define NY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x54)
#  define NG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x80)
#  define NI(D1,B1,I2)			SI_(0x94,I2,B1,D1)
#  define NIY(D1,B1,I2)			SIY_(0xEB,I2,B1,D1,0x54)
#  define NC(D1,L,B1,D2,B2)		SSL_(0xD4,L,B1,D1,B2,D2)
/* AND IMMEDIATE */
#  define NIHH(R1,I2)			RI_(0xA5,R1,0x4,I2)
#  define NIHL(R1,I2)			RI_(0xA5,R1,0x5,I2)
#  define NILH(R1,I2)			RI_(0xA5,R1,0x6,I2)
#  define NILL(R1,I2)			RI_(0xA5,R1,0x7,I2)
/* BRANCH AND LINK */
#  define BALR(R1,R2)			RR_(0x05,R1,R2)
#  define BAL(R1,D2,X2,B2)		RX_(0x45,R1,X2,B2,D2)
/* BRANCH AND SAVE */
#  define BASR(R1,R2)			RR_(0x0D,R1,R2)
#  define BAS(R1,D2,X2,B2)		RX_(0x4D,R1,X2,B2,D2)
/* BRANCH AND SAVE AND SET MODE */
#  define BASSM(R1,R2)			RR_(0x0C,R1,R2)
/* BRANCH AND SET MODE */
#  define BSM(R1,R2)			RR_(0x0B,R1,R2)
/* BRANCH ON CONDITION */
#  define BCR(M1,R2)			RR_(0x07,M1,R2)
#  define BR(R2)			BCR(CC_AL,R2)
#  define NOPR(R2)			BCR(CC_NV,R2)
#  define BC(M1,D2,X2,B2)		RX_(0x47,M1,X2,B2,D2)
/* BRANCH ON COUNT */
#  define BCTR(R1,R2)			RR_(0x06,R1,R2)
#  define BCTGR(R1,R2)			RRE_(0xB946,R1,R2)
#  define BCT(R1,D2,X2,B2)		RX_(0x46,R1,X2,B2,D2)
#  define BCTG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x46)
/* BRANCH ON INDEX HIGH */
#  define BXH(R1,R3,D2,B2)		RS_(0x86,R1,R3,B2,D2)
#  define BXHG(R1,R3,B2,D2)		RSY_(0xEB,R1,R3,B2,D2,0x44)
/* BRANCH ON INDEX LOW OR EQUAL */
#  define BXLE(R1,R3,D2,B2)		RS_(0x87,R1,R3,B2,D2)
#  define BXLEG(R1,R3,B2,D2)		RSY_(0xEB,R1,R3,B2,D2,0x45)
/* BRANCH RELATIVE AND SAVE */
#  define BRAS(R1,I2)			RI_(0xA7,R1,0x5,I2)
/* BRANCH RELATIVE AND SAVE LONG */
#  define BRASL(R1,I2)			RIL_(0xC0,R1,0x5,I2)
/* BRANCH RELATIVE ON CONDITION */
#  define BRC(M1,I2)			RI_(0xA7,M1,0x4,I2)
#  define J(I2)				BRC(CC_AL,I2)
/* BRANCH RELATIVE ON CONDITION LONG */
#  define BRCL(M1,I2)			RIL_(0xC0,M1,0x4,I2)
#  define BRL(I2)			BRCL(CC_AL,I2)
/* BRANCH RELATIVE ON COUNT */
#  define BRCT(M1,I2)			RI_(0xA7,M1,0x6,I2)
#  define BRCTG(M1,I2)			RI_(0xA7,M1,0x7,I2)
/* BRANCH RELATIVE ON INDEX HIGH */
#  define BRXH(R1,R3,I2)		RSI_(0x84,R1,R3,I2)
#  define BRXHG(R1,R3,I2)		RIE_(0xEC,R1,R3,I2,0x44)
/* BRANCH RELATIVE ON INDEX LOW OR EQUAL */
#  define BRXLE(R1,R3,I2)		RSI_(0x85,R1,R3,I2)
#  define BRXLEG(R1,R3,I2)		RIE_(0xEC,R1,R3,I2,0x45)
/* CHECKSUM */
#  define CKSUM(R1,R2)			RRE_(0xB241,R1,R2)
/* CIPHER MESAGE (KM) */
#  define KM(R1,R2)			RRE_(0xB92E,R1,R2)
/* CIPHER MESAGE WITH CHAINING (KMC) */
#  define KMC(R1,R2)			RRE_(0xB92F,R1,R2)
/* COMPARE */
#  define CR(R1,R2)			RR_(0x19,R1,R2)
#  define CGR(R1,R2)			RRE_(0xB920,R1,R2)
#  define CGFR(R1,R2)			RRE_(0xB930,R1,R2)
#  define C(R1,D2,X2,B2)		RX_(0x59,R1,X2,B2,D2)
#  define CY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x59)
#  define CG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x20)
#  define CGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x30)
/* COMPARE AND FORM CODEWORD */
#  define CFC(D2,B2)			S_(0xB21A,B2,D2)
/* COMPARE AND SWAP */
#  define CS(R1,R3,D2,B2)		RS_(0xBA,R1,R3,B2,D2)
#  define CSY(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x14)
#  define CSG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x30)
/* COMPARE DOUBLE AND SWAP */
#  define CDS(R1,R3,D2,B2)		RS_(0xBB,R1,R3,B2,D2)
#  define CSDY(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x31)
#  define CSDG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x3E)
/* COMPARE HALFWORD */
#  define CH(R1,D2,X2,B2)		RX_(0x49,R1,X2,B2,D2)
#  define CHY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x79)
/* COMPARE HALFWORD IMMEDIATE */
#  define CHI(R1,I2)			RI_(0xA7,R1,0xE,I2)
#  define CGHI(R1,I2)			RI_(0xA7,R1,0xF,I2)
/* COMPARE LOGICAL */
#  define CLR(R1,R2)			RR_(0x15,R1,R2)
#  define CLGR(R1,R2)			RRE_(0xB921,R1,R2)
#  define CLGFR(R1,R2)			RRE_(0xB931,R1,R2)
#  define CL(R1,D2,X2,B2)		RX_(0x55,R1,X2,B2,D2)
#  define CLY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x55)
#  define CLG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x21)
#  define CLGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x31)
#  define CLI(D1,B1,I2)			SI_(0x95,I2,B1,D1)
#  define CLIY(D1,B1,I2)		SIY_(0xEB,I2,B1,D1,0x55)
#  define CLC(D1,L,B1,D2,B2)		SSL_(0xD5,L,B1,D1,B2,D2)
/* COMPARE LOGICAL CHARACTERS UNDER MASK */
#  define CLM(R1,M3,D2,B2)		RS_(0xBD,R1,M3,B2,D2)
#  define CLMY(R1,M3,D2,B2)		RSY_(0xEB,R1,M3,B2,D2,0x21)
#  define CLMH(R1,M3,D2,B2)		RSY_(0xEB,R1,M3,B2,D2,0x20)
/* COMPARE LOGICAL LONG */
#  define CLCL(R1,R2)			RR_(0x0F,R1,R2)
/* COMPARE LOGICAL LONG EXTENDED */
#  define CLCLE(R1,R3,D2,B2)		RS_(0xA9,R1,R3,B2,D2)
/* COMPARE LOGICAL LONG UNICODE */
#  define CLCLU(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x8F)
/* COMPARE LOGICAL STRING */
#  define CLST(R1,R2)			RRE_(0xB25D,R1,R2)
/* COMPARE UNTIL SUBSTRING EQUAL */
#  define CUSE(R1,R2)			RRE_(0xB257,R1,R2)
/* COMPRESSION CALL */
#  define CMPSC(R1,R2)			RRE_(0xB263,R1,R2)
/* COMPUTE INTERMEDIATE MESSAGE DIGEST (KIMD) */
#  define KIMD(R1,R2)			RRE_(0xB93E,R1,R2)
/* COMPUTE LAST MESSAGE DIGEST (KIMD) */
#  define KLMD(R1,R2)			RRE_(0xB93F,R1,R2)
/* COMPUTE MESSAGE AUTHENTICATION CODE (KMAC) */
#  define KMAC(R1,R2)			RRE_(0xB91E,R1,R2)
/* CONVERT TO BINARY */
#  define CVB(R1,D2,X2,B2)		RX_(0x4F,R1,X2,B2,D2)
#  define CVBY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x06)
#  define CVBG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x0e)
/* CONVERT TO DECIMAL */
#  define CVD(R1,D2,X2,B2)		RX_(0x4E,R1,X2,B2,D2)
#  define CVDY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x26)
#  define CVDG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x2E)
/* CONVERT UNICODE TO UTF-8 */
#  define CUUTF(R1,R2)			RRE_(0xB2A6,R1,R2)
/* CONVERT UTF-8 TO UNICODE */
#  define CUTFU(R1,R2)			RRE_(0xB2A7,R1,R2)
/* COPY ACCESS */
#  define CPYA(R1,R2)			RRE_(0xB24D,R1,R2)
/* DIVIDE */
#  define DR(R1,R2)			RR_(0x1D,R1,R2)
#  define D(R1,D2,X2,B2)		RX_(0x5D,R1,X2,B2,D2)
/* DIVIDE LOGICAL */
#  define DLR(R1,R2)			RRE_(0xB997,R1,R2)
#  define DLGR(R1,R2)			RRE_(0xB987,R1,R2)
#  define DL(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x97)
#  define DLG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x87)
/* DIVIDE SINGLE */
#  define DSGR(R1,R2)			RRE_(0xB90D,R1,R2)
#  define DSGFR(R1,R2)			RRE_(0xB91D,R1,R2)
#  define DSG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x0D)
#  define DSGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x1D)
/* EXCLUSIVE OR */
#  define XR(R1,R2)			RR_(0x17,R1,R2)
#  define XGR(R1,R2)			RRE_(0xB982,R1,R2)
#  define X(R1,D2,X2,B2)		RX_(0x57,R1,X2,B2,D2)
#  define XY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x57)
#  define XG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x82)
#  define XI(D1,B1,I2)			SI_(0x97,I2,B1,D1)
#  define XIY(D1,B1,I2)			SIY_(0xEB,I2,B1,D1,0x57)
#  define XC(D1,L,B1,D2,B2)		SSL_(0xD7,L,B1,D1,B2,D2)
/* EXECUTE */
#  define EX(R1,D2,X2,B2)		RX_(0x44,R1,X2,B2,D2)
/* EXTRACT ACCESS */
#  define EAR(R1,R2)			RRE_(0xB24F,R1,R2)
/* EXTRACT PSW */
#  define EPSW(R1,R2)			RRE_(0xB98D,R1,R2)
/* INSERT CHARACTER */
#  define IC(R1,D2,X2,B2)		RX_(0x43,R1,X2,B2,D2)
#  define ICY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x73)
/* INSERT CHARACTERS UNDER MASK */
#  define ICM(R1,M3,D2,B2)		RS_(0xBF,R1,M3,B2,D2)
#  define ICMY(R1,M3,D2,B2)		RSY_(0xEB,R1,M3,B2,D2,0x81)
#  define ICMH(R1,M3,D2,B2)		RSY_(0xEB,R1,M3,B2,D2,0x80)
/* INSERT IMMEDIATE */
#  define IIHH(R1,I2)			RI_(0xA5,R1,0x0,I2)
#  define IIHL(R1,I2)			RI_(0xA5,R1,0x1,I2)
#  define IILH(R1,I2)			RI_(0xA5,R1,0x2,I2)
#  define IILL(R1,I2)			RI_(0xA5,R1,0x3,I2)
/* INSERT PROGRAM MASK */
#  define IPM(R1)			RRE_(0xB222,R1,0)
/* LOAD */
#  define LR(R1,R2)			RR_(0x18,R1,R2)
#  define LGR(R1,R2)			RRE_(0xB904,R1,R2)
#  define LGFR(R1,R2)			RRE_(0xB914,R1,R2)
#  define L(R1,D2,X2,B2)		RX_(0x58,R1,X2,B2,D2)
#  define LY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x58)
#  define LG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x04)
#  define LGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x14)
/* LOAD ACCESS MULTIPLE */
#  define LAM(R1,R3,D2,B2)		RS_(0x9A,R1,R3,B2,D2)
#  define LAMY(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x9A)
/* LOAD ADDRESS */
#  define LA(R1,D2,X2,B2)		RX_(0x41,R1,X2,B2,D2)
#  define LAY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x71)
/* LOAD ADDRESS EXTENDED */
#  define LAE(R1,D2,X2,B2)		RX_(0x51,R1,X2,B2,D2)
/* LOAD ADDRESS RELATIVE LONG */
#  define LARL(R1,I2)			RIL_(0xC0,R1,0x0,I2)
/* LOAD AND TEST */
#  define LTR(R1,R2)			RR_(0x12,R1,R2)
#  define LTGR(R1,R2)			RRE_(0xB902,R1,R2)
#  define LTGFR(R1,R2)			RRE_(0xB912,R1,R2)
/* LOAD BYTE */
#  define LGBR(R1,R2)			RRE_(0xB906,R1,R2)	/* disasm */
#  define LB(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x76)
#  define LGB(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x77)
/* LOAD COMPLEMENT */
#  define LCR(R1,R2)			RR_(0x13,R1,R2)
#  define LCGR(R1,R2)			RRE_(0xB903,R1,R2)
#  define LCGFR(R1,R2)			RRE_(0xB913,R1,R2)
/* LOAD HALFWORD */
#  define LH(R1,D2,X2,B2)		RX_(0x48,R1,X2,B2,D2)
#  define LHY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x78)
#  define LGHR(R1,R2)			RRE_(0xB907,R1,R2)	/* disasm */
#  define LGH(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x15)
/* LOAD HALFWORD IMMEDIATE */
#  define LHI(R1,I2)			RI_(0xA7,R1,0x8,I2)
#  define LGHI(R1,I2)			RI_(0xA7,R1,0x9,I2)
/* LOAD LOGICAL */
#  define LLGFR(R1,R2)			RRE_(0xB916,R1,R2)
#  define LLGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x16)
/* LOAD LOGICAL CHARACTER */
#  define LLGCR(R1,R2)			RRE_(0xB984,R1,R2)	/* disasm */
#  define LLGC(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x90)
/* LOAD LOGICAL HALFWORD */
#  define LLGHR(R1,R2)			RRE_(0xB985,R1,R2)	/* disasm */
#  define LLGH(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x91)
/* LOAD LOGICAL IMMEDIATE */
#  define LLIHH(R1,I2)			RI_(0xA5,R1,0xC,I2)
#  define LLIHL(R1,I2)			RI_(0xA5,R1,0xD,I2)
#  define LLILH(R1,I2)			RI_(0xA5,R1,0xE,I2)
#  define LLILL(R1,I2)			RI_(0xA5,R1,0xF,I2)
/* LOAD LOGICAL THIRTY ONE BITS */
#  define LLGTR(R1,R2)			RRE_(0xB917,R1,R2)
#  define LLGT(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x17)
/* LOAD MULTIPLE */
#  define LM(R1,R3,D2,B2)		RS_(0x98,R1,R3,B2,D2)
#  define LMY(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x98)
#  define LMG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x04)
/* LOAD MULTIPLE DISJOINT */
#  define LMD(R1,R3,D2,B2,D4,B4)	SS_(0xEF,R1,R3,B2,D2,B4,D4)
/* LOAD MULTIPLE HIGH */
#  define LMH(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x96)
/* LOAD NEGATIVE */
#  define LNR(R1,R2)			RR_(0x11,R1,R2)
#  define LNGR(R1,R2)			RRE_(0xB901,R1,R2)
#  define LNGFR(R1,R2)			RRE_(0xB911,R1,R2)
/* LOAD PAIR FROM QUADWORD */
#  define LPQ(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x8F)
/* LOAD POSITIVE */
#  define LPR(R1,R2)			RR_(0x10,R1,R2)
#  define LPGR(R1,R2)			RRE_(0xB900,R1,R2)
#  define LPGFR(R1,R2)			RRE_(0xB910,R1,R2)
/* LOAD REVERSED */
#  define LRVR(R1,R2)			RRE_(0xB91F,R1,R2)
#  define LRVGR(R1,R2)			RRE_(0xB90F,R1,R2)
#  define LRVH(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x1F)
#  define LRV(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x1E)
#  define LRVG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x0F)
/* MONITOR CALL */
#  define MC(D1,B1,I2)			SI_(0xAF,I2,B1,D1)
/* MOVE */
#  define MVI(D1,B1,I2)			SI_(0x92,I2,B1,D1)
#  define MVIY(D1,B1,I2)		SIY_(0xEB,I2,B1,D1,0x52)
#  define MVC(D1,L,B1,D2,B2)		SSL_(0xD2,L,B1,D1,B2,D2)
/* MOVE INVERSE */
#  define MVCIN(D1,L,B1,D2,B2)		SSL_(0xE8,L,B1,D1,B2,D2)
/* MOVE LONG */
#  define MVCL(R1,R2)			RR_(0x0E,R1,R2)
/* MOVE LONG EXTENDED */
#  define MVCLE(R1,R3,D2,B2)		RS_(0xA8,R1,R3,B2,D2)
/* MOVE LONG UNICODE */
#  define MVCLU(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x8E)
/* MOVE NUMERICS */
#  define MVN(D1,L,B1,D2,B2)		SSL_(0xD1,L,B1,D1,B2,D2)
/* MOVE STRING */
#  define MVST(R1,R2)			RRE_(0xB255,R1,R2)
/* MOVE WITH OFFSET */
#  define MVO(D1,L1,B1,D2,L2,B2)	SS_(0xF1,L1,L2,B1,D1,B2,D2)
/* MOVE ZONES */
#  define MVZ(D1,L,B1,D2,B2)		SSL_(0xD3,L,B1,D1,B2,D2)
/* MULTIPLY */
#  define MR(R1,R2)			RR_(0x1C,R1,R2)
#  define M(R1,D2,X2,B2)		RX_(0x5C,R1,X2,B2,D2)
/* MULTIPLY HALFWORD */
#  define MH(R1,D2,X2,B2)		RX_(0x4C,R1,X2,B2,D2)
/* MULTIPLY HALFWORD IMMEDIATE */
#  define MHI(R1,I2)			RI_(0xA7,R1,0xC,I2)
#  define MGHI(R1,I2)			RI_(0xA7,R1,0xD,I2)
/* MULTIPLY LOGICAL */
#  define MLR(R1,R2)			RRE_(0xB996,R1,R2)
#  define MLGR(R1,R2)			RRE_(0xB986,R1,R2)
#  define ML(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x96)
#  define MLG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x86)
/* MULTIPLY SINGLE */
#  define MSR(R1,R2)			RRE_(0xB252,R1,R2)
#  define MSGR(R1,R2)			RRE_(0xB90C,R1,R2)
#  define MSGFR(R1,R2)			RRE_(0xB91C,R1,R2)
#  define MS(R1,D2,X2,B2)		RX_(0x71,R1,X2,B2,D2)
#  define MSY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x51)
#  define MSG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x0C)
#  define MSGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x1C)
/* OR */
#  define OR(R1,R2)			RR_(0x16,R1,R2)
#  define OGR(R1,R2)			RRE_(0xB981,R1,R2)
#  define O(R1,D2,X2,B2)		RX_(0x56,R1,X2,B2,D2)
#  define OY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x56)
#  define OG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x81)
#  define OI(D1,B1,I2)			SI_(0x96,I2,B1,D1)
#  define OIY(D1,B1,I2)			SIY_(0xEB,I2,B1,D1,0x56)
#  define OC(D1,L,B1,D2,B2)		SSL_(0xD6,L,B1,D1,B2,D2)
/* OR IMMEDIATE */
#  define OIHH(R1,I2)			RI_(0xA5,R1,0x8,I2)
#  define OIHL(R1,I2)			RI_(0xA5,R1,0x9,I2)
#  define OILH(R1,I2)			RI_(0xA5,R1,0xA,I2)
#  define OILL(R1,I2)			RI_(0xA5,R1,0xB,I2)
/* PACK */
#  define PACK(D1,L1,B1,D2,L2,B2)	SS_(0xF2,L1,L2,B1,D1,B2,D2)
/* PACK ASCII */
#  define PKA(D1,B1,D2,L2,B2)		SSL_(0xE9,L2,B1,D1,B2,D2)
/* PACK UNICODE */
#  define PKU(D1,B1,D2,L2,B2)		SSL_(0xE1,L2,B1,D1,B2,D2)
/* PERFORM LOCKED OPERATION */
#  define PLO(R1,D2,B2,R3,D4,B4)	SS_(0xEE,R1,R3,B2,D2,B4,D4)
/* ROTATE LEFT SINGLE LOGICAL */
#  define RLL(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x1D)
#  define RLLG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x1C)
/* SEARCH STRING */
#  define SRST(R1,R2)			RRE_(0xB25E,R1,R2)
/* SET ACCESS */
#  define SAR(R1,R2)			RRE_(0xB24E,R1,R2)
/* SET ADDRESSING MODE */
#  define SAM24()			E_(0x10C)
#  define SAM31()			E_(0x10D)
#  define SAM64()			E_(0x10E)
/* SET PROGRAM MASK */
#  define SPM(R1)			RR_(0x04,R1,0)
/* SHIFT LEFT DOUBLE */
#  define SLDA(R1,D2,B2)		RS_(0x8F,R1,0,B2,D2)
/* SHIFT LEFT DOUBLE LOGICAL */
#  define SLDL(R1,D2,B2)		RS_(0x8D,R1,0,B2,D2)
/* SHIFT LEFT SINGLE */
#  define SLA(R1,D2,B2)			RS_(0x8B,R1,0,B2,D2)
#  define SLAG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x0B)
/* SHIFT LEFT SINGLE LOGICAL */
#  define SLL(R1,D2,B2)			RS_(0x89,R1,0,B2,D2)
#  define SLLG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x0D)
/* SHIFT RIGHT DOUBLE */
#  define SRDA(R1,D2,B2)		RS_(0x8E,R1,0,B2,D2)
/* SHIFT RIGHT DOUBLE LOGICAL */
#  define SRDL(R1,D2,B2)		RS_(0x8C,R1,0,B2,D2)
/* SHIFT RIGHT SINGLE */
#  define SRA(R1,D2,B2)			RS_(0x8A,R1,0,B2,D2)
#  define SRAG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x0A)
/* SHIFT RIGHT SINGLE LOGICAL */
#  define SRL(R1,D2,B2)			RS_(0x88,R1,0,B2,D2)
#  define SRLG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x0C)
/* STORE */
#  define ST(R1,D2,X2,B2)		RX_(0x50,R1,X2,B2,D2)
#  define STY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x50)
#  define STG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x24)
/* STORE ACCESS MULTIPLE */
#  define STAM(R1,R3,D2,B2)		RS_(0x9B,R1,R3,B2,D2)
#  define STAMY(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x9B)
/* STORE CHARACTER */
#  define STC(R1,D2,X2,B2)		RX_(0x42,R1,X2,B2,D2)
#  define STCY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x72)
/* STORE CHARACTERS UNDER MASK */
#  define STCM(R1,M3,D2,B2)		RS_(0xBE,R1,M3,B2,D2)
#  define STCMY(R1,M3,D2,B2)		RSY_(0xEB,R1,M3,B2,D2,0x2D)
#  define STCMH(R1,M3,D2,B2)		RSY_(0xEB,R1,M3,B2,D2,0x2C)
/* STORE CLOCK */
#  define STCK(D2,B2)			S_(0xB205,B2,D2)
/* STORE CLOCK EXTENDED */
#  define STCKE(D2,B2)			S_(0xB278,B2,D2)
/* STORE HALFWORD */
#  define STH(R1,D2,X2,B2)		RX_(0x40,R1,X2,B2,D2)
#  define STHY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x70)
/* STORE MULTIPLE */
#  define STM(R1,R3,D2,B2)		RS_(0x90,R1,R3,B2,D2)
#  define STMY(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x90)
#  define STMG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x24)
/* STORE MULTIPLE HIGH */
#  define STMH(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x26)
/* STORE PAIR TO QUADWORD */
#  define STPQ(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x8E)
/* STORE REVERSED */
#  define STRVH(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x3F)
#  define STRV(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x3E)
#  define STRVG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x2F)
/* SUBTRACT */
#  define SR(R1,R2)			RR_(0x1B,R1,R2)
#  define SGR(R1,R2)			RRE_(0xB909,R1,R2)
#  define SGFR(R1,R2)			RRE_(0xB919,R1,R2)
#  define S(R1,D2,X2,B2)		RX_(0x5B,R1,X2,B2,D2)
#  define SY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x5B)
#  define SG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x09)
#  define SGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x19)
/* SUBTRACT HALFWORD */
#  define SH(R1,D2,X2,B2)		RX_(0x4B,R1,X2,B2,D2)
#  define SHY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x7B)
/* SUBTRACT LOGICAL */
#  define SLR(R1,R2)			RR_(0x1F,R1,R2)
#  define SLGR(R1,R2)			RRE_(0xB90B,R1,R2)
#  define SLGFR(R1,R2)			RRE_(0xB91B,R1,R2)
#  define SL(R1,D2,X2,B2)		RX_(0x5F,R1,X2,B2,D2)
#  define SLY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x5F)
#  define SLG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x0B)
#  define SLGF(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x1B)
/* SUBTRACT LOGICAL WITH BORROW */
#  define SLBR(R1,R2)			RRE_(0xB999,R1,R2)
#  define SLBGR(R1,R2)			RRE_(0xB989,R1,R2)
#  define SLB(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x99)
#  define SLBG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x89)
/* SUPERVISOR CALL */
#  define SVC(I)			I_(0xA,I)
/* TEST ADDRESSING MODE */
#  define TAM()				E_(0x10B)
/* TEST AND SET */
#  define TS(D2,B2)			RS_(0x93,0,0,B2,D2)
/* TEST UNDER MASK (TEST UNDER MASK HIGH, TEST UNDER MASK LOW) */
#  define TM(D1,B1,I2)			SI_(0x91,I2,B1,D1)
#  define TMY(D1,B1,I2)			SIY_(0xEB,I2,B1,D1,0x51)
#  define TMHH(R1,I2)			RI_(0xA7,R1,0x2,I2)
#  define TMHL(R1,I2)			RI_(0xA7,R1,0x3,I2)
#  define TMLH(R1,I2)			RI_(0xA7,R1,0x0,I2)
#  define TMH(R1,I2)			TMLH(R1,I2)
#  define TMLL(R1,I2)			RI_(0xA7,R1,0x1,I2)
#  define TML(R1,I2)			TMLL(R1,I2)
/* TRANSLATE */
#  define TR(D1,L,B1,D2,B2)		SSL_(0xDC,L,B1,D1,B2,D2)
/* TRANSLATE AND TEST */
#  define TRT(D1,L,B1,D2,B2)		SSL_(0xDD,L,B1,D1,B2,D2)
/* TRANSLATE EXTENDED */
#  define TRE(R1,R2)			RRE_(0xB2A5,R1,R2)
/* TRANSLATE ONE TO ONE */
#  define TROO(R1,R2)			RRE_(0xB993,R1,R2)
/* TRANSLATE ONE TO TWO */
#  define TROT(R1,R2)			RRE_(0xB992,R1,R2)
/* TRANSLATE TWO TO ONE */
#  define TRTO(R1,R2)			RRE_(0xB991,R1,R2)
/* TRANSLATE TWO TO TWO */
#  define TRTT(R1,R2)			RRE_(0xB990,R1,R2)
/* UNPACK */
#  define UNPK(D1,L1,B1,D2,L2,B2)	SS_(0xF3,L1,L2,B1,D1,B2,D2)
/* UNPACK ASCII */
#  define UNPKA(D1,L1,B1,D2,L2,B2)	SS_(0xEA,L1,L2,B1,D1,B2,D2)
/* UNPACK UNICODE */
#  define UNPKU(D1,L1,B1,D2,L2,B2)	SS_(0xE2,L1,L2,B1,D1,B2,D2)
/* UPDATE TREE */
#  define UPT()				E_(0x0102)
/****************************************************************
 * Decimal Instructions						*
 ****************************************************************/
/* ADD DECIMAL */
#  define AP(D1,L1,B1,D2,L2,B2)		SS_(0xFA,L1,L2,B1,D1,B2,D2)
/* COMPARE DECIMAL */
#  define CP(D1,L1,B1,D2,L2,B2)		SS_(0xF9,L1,L2,B1,D1,B2,D2)
/* DIVIDE DECIMAL */
#  define DP(D1,L1,B1,D2,L2,B2)		SS_(0xFD,L1,L2,B1,D1,B2,D2)
/* EDIT */
#  define ED(D1,L,B1,D2,B2)		SSL_(0xDE,L,B1,D1,B2,D2)
/* EDIT AND MARK */
#  define EDMK(D1,L,B1,D2,B2)		SSL_(0xDE,L,B1,D1,B2,D2)
/* MULTIPLY DECIMAL */
#  define MP(D1,L1,B1,D2,L2,B2)		SS_(0xFC,L1,L2,B1,D1,B2,D2)
/* SHIFT AND ROUND DECIMAL */
#  define SRP(D1,L1,B1,D2,L2,B2)	SS_(0xF0,L1,L2,B1,D1,B2,D2)
/* SUBTRACE DECIMAL */
#  define SP(D1,L1,B1,D2,L2,B2)		SS_(0xFB,L1,L2,B1,D1,B2,D2)
/* TEST DECIMAL */
#  define TP(D1,L1,B1)			RSL_(0xEB,L1,B1,D1,0xC0)
/* ZERO AND ADD */
#  define ZAP(D1,L1,B1,D2,L2,B2)	SS_(0xF8,L1,L2,B1,D1,B2,D2)
/****************************************************************
 * Control Instructions						*
 ****************************************************************/
/* BRANCH AND SET AUTHORITY */
#  define BSA(R1,R2)			RRE_(0xB25A,R1,R2)
/* BRANCH AND STACK */
#  define BAKR(R1,R2)			RRE_(0xB240,R1,R2)
/* BRANCH IN SUBSPACE GROUP */
#  define BSG(R1,R2)			RRE_(0xB258,R1,R2)
/* COMPARE AND SWAP AND PURGE */
#  define CSP(R1,R2)			RRE_(0xB250,R1,R2)
#  define CSPG(R1,R2)			RRE_(0xB98A,R1,R2)
/* DIAGNOSE */
#  define DIAG()			SI_(0x83,0,0,0)
/* EXTRACT AND SET EXTENDED AUTHORITY */
#  define ESEA(R1)			RRE_(0xB99D,R1,0)
/* EXTRACT PRIMARY ASN */
#  define EPAR(R1)			RRE_(0xB226,R1,0)
/* EXTRACT SECONDARY ASN */
#  define ESAR(R1)			RRE_(0xB227,R1,0)
/* EXTRACT STACKED REGISTERS */
#  define EREG(R1,R2)			RRE_(0xB249,R1,R2)
#  define EREGG(R1,R2)			RRE_(0xB90E,R1,R2)
/* EXTRACT STACKED STATE */
#  define ESTA(R1,R2)			RRE_(0xB24A,R1,R2)
/* INSERT ADDRESS SPACE CONTROL */
#  define IAC(R1)			RRE_(0xB224,R1,0)
/* INSERT PSW KEY */
#  define IPK()				S_(0xB20B,0,0)
/* INSERT STORAGE KEY EXTENDED */
#  define ISKE(R1,R2)			RRE_(0xB229,R1,R2)
/* INSERT VIRTUAL STORAGE KEY */
#  define IVSK(R1,R2)			RRE_(0xB223,R1,R2)
/* INVALIDATE DAT TABLE ENTRY */
#  define IDTE(R1,R2,R3)		RRF_(0xB98E,R3,0,R1,R2)
/* INVALIDATE PAGE TABLE ENTRY */
#  define IPTE(R1,R2)			RRE_(0xB221,R1,R2)
/* LOAD ADDRESS SPACE PARAMETER */
#  define LASP(D1,B1,D2,B2)		SSE_(0xE500,B1,D1,B2,D2)
/* LOAD CONTROL */
#  define LCTL(R1,R3,D2,B2)		RS_(0xB7,R1,R3,B2,D2)
#  define LCTLG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x2F)
/* LOAD PSW */
#  define LPSW(D2,B2)			SI_(0x82,0,B2,D2)
/* LOAD PSW EXTENDED */
#  define LPSWE(D2,B2)			S_(0xB2B2,B2,D2)
/* LOAD REAL ADDRESS */
#  define LRA(R1,D2,X2,B2)		RX_(0xB1,R1,X2,B2,D2)
#  define LRAY(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x13)
#  define LRAG(R1,D2,X2,B2)		RXY_(0xE3,R1,X2,B2,D2,0x03)
/* LOAD USING REAL ADDRESS */
#  define LURA(R1,R2)			RRE_(0xB24B,R1,R2)
#  define LURAG(R1,R2)			RRE_(0xB905,R1,R2)
/* MODIFY STACKED STATE */
#  define MSTA(R1)			RRE_(0xB247,R1,0)
/* MOVE PAGE */
#  define MVPG(R1,R2)			RRE_(0xB254,R1,R2)
/* MOVE TO PRIMARY */
#  define MVCP(D1,R1,B1,D2,B2,R3)	SS_(0xDA,R1,R3,B1,D1,B2,D2)
/* MOVE TO SECONDARY */
#  define MVCS(D1,R1,B1,D2,B2,R3)	SS_(0xDB,R1,R3,B1,D1,B2,D2)
/* MOVE WITH DESTINATION KEY */
#  define MVCDK(D1,B1,D2,B2)		SSE_(0xE50F,B1,D1,B2,D2)
/* MOVE WITH KEY */
#  define MVCK(D1,R1,B1,D2,B2,R3)	SS_(0xD9,R1,R3,B1,D1,B2,D2)
/* MOVE WITH SOURCE KEY */
#  define MVCSK(D1,B1,D2,B2)		SSE_(0xE50E,B1,D1,B2,D2)
/* PAGE IN */
#  define PGIN(R1,R2)			RRE_(0xB22E,R1,R2)
/* PAGE OUT */
#  define PGOUT(R1,R2)			RRE_(0xB22F,R1,R2)
/* PROGRAM CALL */
#  define PC(D2,B2)			S_(0xB218,B2,D2)
/* PROGRAM RETURN */
#  define PR()				E_(0x0101)
/* PROGRAM TRANSFER */
#  define PT(R1,R2)			RRE_(0xB228,R1,R2)
/* PURGE ALB */
#  define PALB()			RRE_(0xB248,0,0)
/* PURGE TLB */
#  define PTLB()			S_(0xB20D,0,0)
/* RESET REFERENCE BIT EXTENDED */
#  define RRBE(R1,R2)			RRE_(0xB22A,R1,R2)
/* RESUME PROGRAM */
#  define RP(D2,B2)			S_(0xB277,B2,D2)
/* SET ADDRESS SPACE CONTROL */
#  define SAC(D2,B2)			S_(0xB219,B2,D2)
/* SET ADDRESS SPACE CONTROL FAST */
#  define SACF(D2,B2)			S_(0xB279,B2,D2)
/* SET CLOCK */
#  define SCK(D2,B2)			S_(0xB204,B2,D2)
/* SET CLOCK COMPARATOR */
#  define SCKC(D2,B2)			S_(0xB206,B2,D2)
/* SET CLOCK PROGRAMMABLE FIELD */
#  define SCKPF()			E_(0x0107)
/* SET CPU TIMER */
#  define SPT(D2,B2)			S_(0xB208,B2,D2)
/* SET PREFIX */
#  define SPX(D2,B2)			S_(0xB210,B2,D2)
/* SET PSW FROM ADDRESS */
#  define SPKA(D2,B2)			S_(0xB20A,B2,D2)
/* SET SECONDARY ASN */
#  define SSAR(R1)			RRE_(0xB225,R1,0)
/* SET STORAGE KEY EXTENDED */
#  define SSKE(R1,R2)			RRE_(0xB22B,R1,R2)
/* SET SYSTEM MASK */
#  define SSM(D2,B2)			SI_(0x80,0,B2,D2)
/* SIGNAL PROCESSOR */
#  define SIGP(R1,R3,D2,B2)		RS_(0xAE,R1,R3,B2,D2)
/* STORE CLOCK COMPARATOR */
#  define STCKC(D2,B2)			S_(0xB207,B2,D2)
/* STORE CONTROL */
#  define STCTL(R1,R3,D2,B2)		RS_(0xB6,R1,R3,B2,D2)
#  define STCTG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x25)
/* STORE CPU ADDRESS */
#  define STAP(D2,B2)			S_(0xB212,B2,D2)
/* STORE CPU ID */
#  define STIDP(D2,B2)			S_(0xB202,B2,D2)
/* STORE CPU TIMER */
#  define STPT(D2,B2)			S_(0xB209,B2,D2)
/* STORE FACILITY LIST */
#  define STFL(D2,B2)			S_(0xB2B1,B2,D2)
/* STORE PREFIX */
#  define STPX(D2,B2)			S_(0xB211,B2,D2)
/* STORE REAL ADDRES */
#  define STRAG(D1,B1,D2,B2)		SSE_(0xE502,B1,D1,B2,D2)
/* STORE SYSTEM INFORMATION */
#  define STSI(D2,B2)			S_(0xB27D,B2,D2)
/* STORE THEN AND SYSTEM MASK */
#  define STNSM(D1,B1,I2)		SI_(0xAC,I2,B1,D1)
/* STORE THEN OR SYSTEM MASK */
#  define STOSM(D1,B1,I2)		SI_(0xAD,I2,B1,D1)
/* STORE USING REAL ADDRESS */
#  define STURA(R1,R2)			RRE_(0xB246,R1,R2)
#  define STURG(R1,R2)			RRE_(0xB925,R1,R2)
/* TEST ACCESS */
#  define TAR(R1,R2)			RRE_(0xB24C,R1,R2)
/* TEST BLOCK */
#  define TB(R1,R2)			RRE_(0xB22C,R1,R2)
/* TEST PROTECTION */
#  define TPROT(D1,B1,D2,B2)		SSE_(0xE501,B1,D1,B2,D2)
/* TRACE */
#  define TRACE(R1,R3,D2,B2)		RS_(0x99,R1,R3,B2,D2)
#  define TRACG(R1,R3,D2,B2)		RSY_(0xEB,R1,R3,B2,D2,0x0F)
/* TRAP */
#  define TRAP2()			E_(0x01FF)
#  define TRAP4(D2,B2)			S_(0xB2FF,B2,D2)
/****************************************************************
 * I/O Instructions						*
 ****************************************************************/
/* CANCEL SUBCHANNEL */
#  define XSCH()			S_(0xB276,0,0)
/* CLEAR SUBCHANNEL */
#  define CSCH()			S_(0xB230,0,0)
/* HALT SUBCHANNEL */
#  define HSCH()			S_(0xB231,0,0)
/* MODIFY SUBCHANNEL */
#  define MSCH(D2,B2)			S_(0xB232,B2,D2)
/* RESET CHANNEL PATH */
#  define RCHP()			S_(0xB23B,0,0)
/* RESUME SUBCHANNEL */
#  define RSCH()			S_(0xB238,0,0)
/* SET ADDRESS LIMIT */
#  define SAL()				S_(0xB237,0,0)
/* SET CHANNEL MONITOR */
#  define SCHM()			S_(0xB23C,0,0)
/* START SUBCHANNEL */
#  define SSCH(D2,B2)			S_(0xB233,B2,D2)
/* STORE CHANNEL PATH STATUS */
#  define STCPS(D2,B2)			S_(0xB23A,B2,D2)
/* STORE CHANNEL REPORT WORD */
#  define STCRW(D2,B2)			S_(0xB239,B2,D2)
/* STORE SUBCHANNEL */
#  define STSCH(D2,B2)			S_(0xB234,B2,D2)
/* TEST PENDING INTERRUPTION */
#  define TPI(D2,B2)			S_(0xB236,B2,D2)
/* TEST SUBCHANNEL */
#  define TSCH(D2,B2)			S_(0xB235,B2,D2)
#  define xdivr(r0,r1)			_xdivr(_jit,r0,r1)
static jit_int32_t _xdivr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define xdivr_u(r0,r1)		_xdivr_u(_jit,r0,r1)
static jit_int32_t _xdivr_u(jit_state_t*,jit_int32_t,jit_int32_t);
#  define xdivi(r0,i0)			_xdivi(_jit,r0,i0)
static jit_int32_t _xdivi(jit_state_t*,jit_int32_t,jit_word_t);
#  define xdivi_u(r0,i0)		_xdivi_u(_jit,r0,i0)
static jit_int32_t _xdivi_u(jit_state_t*,jit_int32_t,jit_word_t);
#  define crr(cc,r0,r1,r2)		_crr(_jit,cc,r0,r1,r2)
static void _crr(jit_state_t*,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define cri(cc,r0,r1,i0)		_cri(_jit,cc,r0,r1,i0)
static void _cri(jit_state_t*,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define crr_u(cc,r0,r1,r2)		_crr_u(_jit,cc,r0,r1,r2)
static void _crr_u(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define cri_u(cc,r0,r1,i0)		_cri_u(_jit,cc,r0,r1,i0)
static void _cri_u(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define brr(cc,i0,r0,r1)		_brr(_jit,cc,i0,r0,r1)
static void _brr(jit_state_t*,jit_int32_t,
		 jit_word_t,jit_int32_t,jit_int32_t);
#  define brr_p(cc,i0,r0,r1)		_brr_p(_jit,cc,i0,r0,r1)
static jit_word_t _brr_p(jit_state_t*,jit_int32_t,
			 jit_word_t,jit_int32_t,jit_int32_t);
#  define bri(cc,i0,r0,i1)		_bri(_jit,cc,i0,r0,i1)
static void _bri(jit_state_t*,jit_int32_t,
		 jit_word_t,jit_int32_t,jit_word_t);
#  define bri_p(cc,i0,r0,i1)		_bri_p(_jit,cc,i0,r0,i1)
static jit_word_t _bri_p(jit_state_t*,jit_int32_t,
			 jit_word_t,jit_int32_t,jit_word_t);
#  define brr_u(cc,i0,r0,r1)		_brr_u(_jit,cc,i0,r0,r1)
static void _brr_u(jit_state_t*,jit_int32_t,
		   jit_word_t,jit_int32_t,jit_int32_t);
#  define brr_u_p(cc,i0,r0,r1)		_brr_u_p(_jit,cc,i0,r0,r1)
static jit_word_t _brr_u_p(jit_state_t*,jit_int32_t,
			   jit_word_t,jit_int32_t,jit_int32_t);
#  define bri_u(cc,i0,r0,i1)		_bri_u(_jit,cc,i0,r0,i1)
static void _bri_u(jit_state_t*,jit_int32_t,
		   jit_word_t,jit_int32_t,jit_word_t);
#  define bri_u_p(cc,i0,r0,i1)		_bri_u_p(_jit,cc,i0,r0,i1)
static jit_word_t _bri_u_p(jit_state_t*,jit_int32_t,
			   jit_word_t,jit_int32_t,jit_word_t);
#  define baddr(c,s,i0,r0,r1)		_baddr(_jit,c,s,i0,r0,r1)
static void _baddr(jit_state_t*,jit_int32_t,jit_bool_t,
		   jit_word_t,jit_int32_t,jit_int32_t);
#  define baddr_p(c,s,i0,r0,r1)		_baddr_p(_jit,c,s,i0,r0,r1)
static jit_word_t _baddr_p(jit_state_t*,jit_int32_t,jit_bool_t,
			   jit_word_t,jit_int32_t,jit_int32_t);
#  define baddi(c,s,i0,r0,i1)		_baddi(_jit,c,s,i0,r0,i1)
static void _baddi(jit_state_t*,jit_int32_t,jit_bool_t,
		   jit_word_t,jit_int32_t,jit_word_t);
#  define baddi_p(c,s,i0,r0,i1)		_baddi_p(_jit,c,s,i0,r0,i1)
static jit_word_t _baddi_p(jit_state_t*,jit_int32_t,jit_bool_t,
			   jit_word_t,jit_int32_t,jit_word_t);
#  define bsubr(c,s,i0,r0,r1)		_bsubr(_jit,c,s,i0,r0,r1)
static void _bsubr(jit_state_t*,jit_int32_t,jit_bool_t,
		   jit_word_t,jit_int32_t,jit_int32_t);
#  define bsubr_p(c,s,i0,r0,r1)		_bsubr_p(_jit,c,s,i0,r0,r1)
static jit_word_t _bsubr_p(jit_state_t*,jit_int32_t,jit_bool_t,
			   jit_word_t,jit_int32_t,jit_int32_t);
#  define bsubi(c,s,i0,r0,i1)		_bsubi(_jit,c,s,i0,r0,i1)
static void _bsubi(jit_state_t*,jit_int32_t,jit_bool_t,
		   jit_word_t,jit_int32_t,jit_word_t);
#  define bsubi_p(c,s,i0,r0,i1)		_bsubi_p(_jit,c,s,i0,r0,i1)
static jit_word_t _bsubi_p(jit_state_t*,jit_int32_t,jit_bool_t,
			   jit_word_t,jit_int32_t,jit_word_t);
#  define bmxr(cc,i0,r0,r1)		_bmxr(_jit,cc,i0,r0,r1)
static void _bmxr(jit_state_t*,jit_int32_t,
		  jit_word_t,jit_int32_t,jit_int32_t);
#  define bmxr_p(cc,i0,r0,r1)		_bmxr_p(_jit,cc,i0,r0,r1)
static jit_word_t _bmxr_p(jit_state_t*,jit_int32_t,
			  jit_word_t,jit_int32_t,jit_int32_t);
#  define bmxi(cc,i0,r0,i1)		_bmxi(_jit,cc,i0,r0,i1)
static void _bmxi(jit_state_t*,jit_int32_t,
		  jit_word_t,jit_int32_t,jit_word_t);
#  define bmxi_p(cc,i0,r0,i1)		_bmxi_p(_jit,cc,i0,r0,i1)
static jit_word_t _bmxi_p(jit_state_t*,jit_int32_t,
			  jit_word_t,jit_int32_t,jit_word_t);
#  define movr(r0,r1)			_movr(_jit,r0,r1)
static void _movr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi(r0,i0)			_movi(_jit,r0,i0)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#  define movi_p(r0,i0)			_movi_p(_jit,r0,i0)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#  define addr(r0,r1,r2)		_addr(_jit,r0,r1,r2)
static void _addr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addi(r0,r1,i0)		_addi(_jit,r0,r1,i0)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addcr(r0,r1,r2)		_addcr(_jit,r0,r1,r2)
static void _addcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addci(r0,r1,i0)		_addci(_jit,r0,r1,i0)
static void _addci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addxr(r0,r1,r2)		_addxr(_jit,r0,r1,r2)
static void _addxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addxi(r0,r1,i0)		_addxi(_jit,r0,r1,i0)
static void _addxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subr(r0,r1,r2)		_subr(_jit,r0,r1,r2)
static void _subr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subi(r0,r1,i0)		_subi(_jit,r0,r1,i0)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subcr(r0,r1,r2)		_subcr(_jit,r0,r1,r2)
static void _subcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subci(r0,r1,i0)		_subci(_jit,r0,r1,i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subxr(r0,r1,r2)		_subxr(_jit,r0,r1,r2)
static void _subxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subxi(r0,r1,i0)		_subxi(_jit,r0,r1,i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define mulr(r0,r1,r2)		_mulr(_jit,r0,r1,r2)
static void _mulr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define muli(r0,r1,i0)		_muli(_jit,r0,r1,i0)
static void _muli(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr(r0,r1,r2,r3)		_qmulr(_jit,r0,r1,r2,r3)
static void _qmulr(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define qmuli(r0,r1,r2,i0)		_qmuli(_jit,r0,r1,r2,i0)
static void _qmuli(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr_u(r0,r1,r2,r3)		_qmulr_u(_jit,r0,r1,r2,r3)
static void _qmulr_u(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_int32_t);
#  define qmuli_u(r0,r1,r2,i0)		_qmuli_u(_jit,r0,r1,r2,i0)
static void _qmuli_u(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_word_t);
#  define divr(r0,r1,r2)		_divr(_jit,r0,r1,r2)
static void _divr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi(r0,r1,i0)		_divi(_jit,r0,r1,i0)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define divr_u(r0,r1,r2)		_divr_u(_jit,r0,r1,r2)
static void _divr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi_u(r0,r1,i0)		_divi_u(_jit,r0,r1,i0)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define remr(r0,r1,r2)		_remr(_jit,r0,r1,r2)
static void _remr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi(r0,r1,i0)		_remi(_jit,r0,r1,i0)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define remr_u(r0,r1,r2)		_remr_u(_jit,r0,r1,r2)
static void _remr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi_u(r0,r1,i0)		_remi_u(_jit,r0,r1,i0)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qdivr(r0,r1,r2,r3)		_qdivr(_jit,r0,r1,r2,r3)
static void _qdivr(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define qdivi(r0,r1,r2,i0)		_qdivi(_jit,r0,r1,r2,i0)
static void _qdivi(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_word_t);
#  define qdivr_u(r0,r1,r2,r3)		_qdivr_u(_jit,r0,r1,r2,r3)
static void _qdivr_u(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_int32_t);
#  define qdivi_u(r0,r1,r2,i0)		_qdivi_u(_jit,r0,r1,r2,i0)
static void _qdivi_u(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define lshr(r0,r1,r2)		_lshr(_jit,r0,r1,r2)
static void _lshr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  else
#    define lshr(r0,r1,r2)		SLLG(r0,r1,0,r2)
#  endif
#  define lshi(r0,r1,i0)		_lshi(_jit,r0,r1,i0)
static void _lshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define rshr(r0,r1,r2)		_rshr(_jit,r0,r1,r2)
static void _rshr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  else
#    define rshr(r0,r1,r2)		SRAG(r0,r1,0,r2)
#  endif
#  define rshi(r0,r1,i0)		_rshi(_jit,r0,r1,i0)
static void _rshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define rshr_u(r0,r1,r2)		_rshr_u(_jit,r0,r1,r2)
static void _rshr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  else
#    define rshr_u(r0,r1,r2)		SRLG(r0,r1,0,r2)
#  endif
#  define rshi_u(r0,r1,i0)		_rshi_u(_jit,r0,r1,i0)
static void _rshi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define negr(r0,r1)			LCR(r0,r1)
#  else
#    define negr(r0,r1)			LCGR(r0,r1)
#  endif
#  define comr(r0,r1)			_comr(_jit,r0,r1)
static void _comr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define andr(r0,r1,r2)		_andr(_jit,r0,r1,r2)
static void _andr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define andi(r0,r1,i0)		_andi(_jit,r0,r1,i0)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define orr(r0,r1,r2)			_orr(_jit,r0,r1,r2)
static void _orr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ori(r0,r1,i0)			_ori(_jit,r0,r1,i0)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define xorr(r0,r1,r2)		_xorr(_jit,r0,r1,r2)
static void _xorr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define xori(r0,r1,i0)		_xori(_jit,r0,r1,i0)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define htonr_us(r0,r1)		extr_us(r0,r1)
#  if __WORDSIZE == 32
#    define htonr_ui(r0,r1)		movr(r0,r1)
#  else
#    define htonr_ui(r0,r1)		extr_ui(r0,r1)
#    define htonr_ul(r0,r1)		movr(r0,r1)
#  endif
#  define extr_c(r0,r1)			LGBR(r0,r1)
#  define extr_uc(r0,r1)		LLGCR(r0,r1)
#  define extr_s(r0,r1)			LGHR(r0,r1)
#  define extr_us(r0,r1)		LLGHR(r0,r1)
#  if __WORDSIZE == 64
#    define extr_i(r0,r1)		LGFR(r0,r1)
#    define extr_ui(r0,r1)		LLGFR(r0,r1)
#  endif
#  define ldr_c(r0,r1)			LGB(r0,0,0,r1)
#  define ldi_c(r0,i0)			_ldi_c(_jit,r0,i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_c(r0,r1,r2)		_ldxr_c(_jit,r0,r1,r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_c(r0,r1,i0)		_ldxi_c(_jit,r0,r1,i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_uc(r0,r1)			LLGC(r0,0,0,r1)
#  define ldi_uc(r0,i0)			_ldi_uc(_jit,r0,i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_uc(r0,r1,r2)		_ldxr_uc(_jit,r0,r1,r2)
static void _ldxr_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_uc(r0,r1,i0)		_ldxi_uc(_jit,r0,r1,i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define ldr_s(r0,r1)		LH(r0,0,0,r1)
#  else
#    define ldr_s(r0,r1)		LGH(r0,0,0,r1)
#  endif
#  define ldi_s(r0,i0)			_ldi_s(_jit,r0,i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_s(r0,r1,r2)		_ldxr_s(_jit,r0,r1,r2)
static void _ldxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_s(r0,r1,i0)		_ldxi_s(_jit,r0,r1,i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_us(r0,r1)			LLGH(r0,0,0,r1)
#  define ldi_us(r0,i0)			_ldi_us(_jit,r0,i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_us(r0,r1,r2)		_ldxr_us(_jit,r0,r1,r2)
static void _ldxr_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_us(r0,r1,i0)		_ldxi_us(_jit,r0,r1,i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define ldr_i(r0,r1)		LLGF(r0,0,0,r1)
#  else
#    define ldr_i(r0,r1)		LGF(r0,0,0,r1)
#  endif
#  define ldi_i(r0,i0)			_ldi_i(_jit,r0,i0)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_i(r0,r1,r2)		_ldxr_i(_jit,r0,r1,r2)
static void _ldxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_i(r0,r1,i0)		_ldxi_i(_jit,r0,r1,i0)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 64
#    define ldr_ui(r0,r1)		LLGF(r0,0,0,r1)
#    define ldi_ui(r0,i0)		_ldi_ui(_jit,r0,i0)
static void _ldi_ui(jit_state_t*,jit_int32_t,jit_word_t);
#    define ldxr_ui(r0,r1,r2)		_ldxr_ui(_jit,r0,r1,r2)
static void _ldxr_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define ldxi_ui(r0,r1,i0)		_ldxi_ui(_jit,r0,r1,i0)
static void _ldxi_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#    define ldr_l(r0,r1)		LG(r0,0,0,r1)
#    define ldi_l(r0,i0)		_ldi_l(_jit,r0,i0)
static void _ldi_l(jit_state_t*,jit_int32_t,jit_word_t);
#    define ldxr_l(r0,r1,r2)		_ldxr_l(_jit,r0,r1,r2)
static void _ldxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define ldxi_l(r0,r1,i0)		_ldxi_l(_jit,r0,r1,i0)
static void _ldxi_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  endif
#  define str_c(r0,r1)			STC(r1,0,0,r0)
#  define sti_c(i0,r0)			_sti_c(_jit,i0,r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_c(r0,r1,r2)		_stxr_c(_jit,r0,r1,r2)
static void _stxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_c(i0,r0,r1)		_stxi_c(_jit,i0,r0,r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define str_s(r0,r1)			STH(r1,0,0,r0)
#  define sti_s(i0,r0)			_sti_s(_jit,i0,r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_s(r0,r1,r2)		_stxr_s(_jit,r0,r1,r2)
static void _stxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_s(i0,r0,r1)		_stxi_s(_jit,i0,r0,r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define str_i(r0,r1)			ST(r1,0,0,r0)
#  define sti_i(i0,r0)			_sti_i(_jit,i0,r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_i(r0,r1,r2)		_stxr_i(_jit,r0,r1,r2)
static void _stxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_i(i0,r0,r1)		_stxi_i(_jit,i0,r0,r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  if __WORDSIZE == 64
#    define str_l(r0,r1)		STG(r1,0,0,r0)
#    define sti_l(i0,r0)		_sti_l(_jit,i0,r0)
static void _sti_l(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_l(r0,r1,r2)		_stxr_l(_jit,r0,r1,r2)
static void _stxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_l(i0,r0,r1)		_stxi_l(_jit,i0,r0,r1)
static void _stxi_l(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  endif
#  define ltr(r0,r1,r2)			crr(CC_L,r0,r1,r2)
#  define lti(r0,r1,i0)			cri(CC_L,r0,r1,i0)
#  define ltr_u(r0,r1,r2)		crr_u(CC_L,r0,r1,r2)
#  define lti_u(r0,r1,i0)		cri_u(CC_L,r0,r1,i0)
#  define ler(r0,r1,r2)			crr(CC_LE,r0,r1,r2)
#  define lei(r0,r1,i0)			cri(CC_LE,r0,r1,i0)
#  define ler_u(r0,r1,r2)		crr_u(CC_LE,r0,r1,r2)
#  define lei_u(r0,r1,i0)		cri_u(CC_LE,r0,r1,i0)
#  define eqr(r0,r1,r2)			crr(CC_E,r0,r1,r2)
#  define eqi(r0,r1,i0)			cri(CC_E,r0,r1,i0)
#  define ger(r0,r1,r2)			crr(CC_HE,r0,r1,r2)
#  define gei(r0,r1,i0)			cri(CC_HE,r0,r1,i0)
#  define ger_u(r0,r1,r2)		crr_u(CC_HE,r0,r1,r2)
#  define gei_u(r0,r1,i0)		cri_u(CC_HE,r0,r1,i0)
#  define gtr(r0,r1,r2)			crr(CC_H,r0,r1,r2)
#  define gti(r0,r1,i0)			cri(CC_H,r0,r1,i0)
#  define gtr_u(r0,r1,r2)		crr_u(CC_H,r0,r1,r2)
#  define gti_u(r0,r1,i0)		cri_u(CC_H,r0,r1,i0)
#  define ner(r0,r1,r2)			crr(CC_NE,r0,r1,r2)
#  define nei(r0,r1,i0)			cri(CC_NE,r0,r1,i0)
#  define bltr(i0,r0,r1)		brr(CC_L,i0,r0,r1)
#  define bltr_p(i0,r0,r1)		brr_p(CC_L,i0,r0,r1)
#  define blti(i0,r0,i1)		bri(CC_L,i0,r0,i1)
#  define blti_p(i0,r0,i1)		bri_p(CC_L,i0,r0,i1)
#  define bltr_u(i0,r0,r1)		brr_u(CC_L,i0,r0,r1)
#  define bltr_u_p(i0,r0,r1)		brr_u_p(CC_L,i0,r0,r1)
#  define blti_u(i0,r0,i1)		bri_u(CC_L,i0,r0,i1)
#  define blti_u_p(i0,r0,i1)		bri_u_p(CC_L,i0,r0,i1)
#  define bler(i0,r0,r1)		brr(CC_LE,i0,r0,r1)
#  define bler_p(i0,r0,r1)		brr_p(CC_LE,i0,r0,r1)
#  define blei(i0,r0,i1)		bri(CC_LE,i0,r0,i1)
#  define blei_p(i0,r0,i1)		bri_p(CC_LE,i0,r0,i1)
#  define bler_u(i0,r0,r1)		brr_u(CC_LE,i0,r0,r1)
#  define bler_u_p(i0,r0,r1)		brr_u_p(CC_LE,i0,r0,r1)
#  define blei_u(i0,r0,i1)		bri_u(CC_LE,i0,r0,i1)
#  define blei_u_p(i0,r0,i1)		bri_u_p(CC_LE,i0,r0,i1)
#  define beqr(i0,r0,r1)		brr(CC_E,i0,r0,r1)
#  define beqr_p(i0,r0,r1)		brr_p(CC_E,i0,r0,r1)
#  define beqi(i0,r0,i1)		bri(CC_E,i0,r0,i1)
#  define beqi_p(i0,r0,i1)		bri_p(CC_E,i0,r0,i1)
#  define bger(i0,r0,r1)		brr(CC_HE,i0,r0,r1)
#  define bger_p(i0,r0,r1)		brr_p(CC_HE,i0,r0,r1)
#  define bgei(i0,r0,i1)		bri(CC_HE,i0,r0,i1)
#  define bgei_p(i0,r0,i1)		bri_p(CC_HE,i0,r0,i1)
#  define bger_u(i0,r0,r1)		brr_u(CC_HE,i0,r0,r1)
#  define bger_u_p(i0,r0,r1)		brr_u_p(CC_HE,i0,r0,r1)
#  define bgei_u(i0,r0,i1)		bri_u(CC_HE,i0,r0,i1)
#  define bgei_u_p(i0,r0,i1)		bri_u_p(CC_HE,i0,r0,i1)
#  define bgtr(i0,r0,r1)		brr(CC_H,i0,r0,r1)
#  define bgtr_p(i0,r0,r1)		brr_p(CC_H,i0,r0,r1)
#  define bgti(i0,r0,i1)		bri(CC_H,i0,r0,i1)
#  define bgti_p(i0,r0,i1)		bri_p(CC_H,i0,r0,i1)
#  define bgtr_u(i0,r0,r1)		brr_u(CC_H,i0,r0,r1)
#  define bgtr_u_p(i0,r0,r1)		brr_u_p(CC_H,i0,r0,r1)
#  define bgti_u(i0,r0,i1)		bri_u(CC_H,i0,r0,i1)
#  define bgti_u_p(i0,r0,i1)		bri_u_p(CC_H,i0,r0,i1)
#  define bner(i0,r0,r1)		brr(CC_NE,i0,r0,r1)
#  define bner_p(i0,r0,r1)		brr_p(CC_NE,i0,r0,r1)
#  define bnei(i0,r0,i1)		bri(CC_NE,i0,r0,i1)
#  define bnei_p(i0,r0,i1)		bri_p(CC_NE,i0,r0,i1)
#  define boaddr(i0,r0,r1)		baddr(CC_O,1,i0,r0,r1)
#  define boaddr_p(i0,r0,r1)		baddr_p(CC_O,1,i0,r0,r1)
#  define boaddi(i0,r0,i1)		baddi(CC_O,1,i0,r0,i1)
#  define boaddi_p(i0,r0,i1)		baddi_p(CC_O,1,i0,r0,i1)
#  define boaddr_u(i0,r0,r1)		baddr(CC_NLE,0,i0,r0,r1)
#  define boaddr_u_p(i0,r0,r1)		baddr_p(CC_NLE,0,i0,r0,r1)
#  define boaddi_u(i0,r0,i1)		baddi(CC_NLE,0,i0,r0,i1)
#  define boaddi_u_p(i0,r0,i1)		baddi_p(CC_NLE,0,i0,r0,i1)
#  define bxaddr(i0,r0,r1)		baddr(CC_NO,1,i0,r0,r1)
#  define bxaddr_p(i0,r0,r1)		baddr_p(CC_NO,1,i0,r0,r1)
#  define bxaddi(i0,r0,i1)		baddi(CC_NO,1,i0,r0,i1)
#  define bxaddi_p(i0,r0,i1)		baddi_p(CC_NO,1,i0,r0,i1)
#  define bxaddr_u(i0,r0,r1)		baddr(CC_LE,0,i0,r0,r1)
#  define bxaddr_u_p(i0,r0,r1)		baddr_p(CC_LE,0,i0,r0,r1)
#  define bxaddi_u(i0,r0,i1)		baddi(CC_LE,0,i0,r0,i1)
#  define bxaddi_u_p(i0,r0,i1)		baddi_p(CC_LE,0,i0,r0,i1)
#  define bosubr(i0,r0,r1)		bsubr(CC_O,1,i0,r0,r1)
#  define bosubr_p(i0,r0,r1)		bsubr_p(CC_O,1,i0,r0,r1)
#  define bosubi(i0,r0,i1)		bsubi(CC_O,1,i0,r0,i1)
#  define bosubi_p(i0,r0,i1)		bsubi_p(CC_O,1,i0,r0,i1)
#  define bosubr_u(i0,r0,r1)		bsubr(CC_L,0,i0,r0,r1)
#  define bosubr_u_p(i0,r0,r1)		bsubr_p(CC_L,0,i0,r0,r1)
#  define bosubi_u(i0,r0,i1)		bsubi(CC_L,0,i0,r0,i1)
#  define bosubi_u_p(i0,r0,i1)		bsubi_p(CC_L,0,i0,r0,i1)
#  define bxsubr(i0,r0,r1)		bsubr(CC_NO,1,i0,r0,r1)
#  define bxsubr_p(i0,r0,r1)		bsubr_p(CC_NO,1,i0,r0,r1)
#  define bxsubi(i0,r0,i1)		bsubi(CC_NO,1,i0,r0,i1)
#  define bxsubi_p(i0,r0,i1)		bsubi_p(CC_NO,1,i0,r0,i1)
#  define bxsubr_u(i0,r0,r1)		bsubr(CC_NL,0,i0,r0,r1)
#  define bxsubr_u_p(i0,r0,r1)		bsubr_p(CC_NL,0,i0,r0,r1)
#  define bxsubi_u(i0,r0,i1)		bsubi(CC_NL,0,i0,r0,i1)
#  define bxsubi_u_p(i0,r0,i1)		bsubi_p(CC_NL,0,i0,r0,i1)
#  define bmsr(i0,r0,r1)		bmxr(CC_NE,i0,r0,r1)
#  define bmsr_p(i0,r0,r1)		bmxr_p(CC_NE,i0,r0,r1)
#  define bmsi(i0,r0,i1)		bmxi(CC_NE,i0,r0,i1)
#  define bmsi_p(i0,r0,i1)		bmxi_p(CC_NE,i0,r0,i1)
#  define bmcr(i0,r0,r1)		bmxr(CC_E,i0,r0,r1)
#  define bmcr_p(i0,r0,r1)		bmxr_p(CC_E,i0,r0,r1)
#  define bmci(i0,r0,i1)		bmxi(CC_E,i0,r0,i1)
#  define bmci_p(i0,r0,i1)		bmxi_p(CC_E,i0,r0,i1)
#  define jmpr(r0)			BR(r0)
#  define jmpi(i0)			_jmpi(_jit,i0)
static void _jmpi(jit_state_t*,jit_word_t);
#  define jmpi_p(i0)			_jmpi_p(_jit,i0)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t);
#  define callr(r0)			BALR(_R14_REGNO,r0)
#  define calli(i0)			_calli(_jit,i0)
static void _calli(jit_state_t*,jit_word_t);
#  define calli_p(i0)			_calli_p(_jit,i0)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#  define prolog(i0)			_prolog(_jit,i0)
static void _prolog(jit_state_t*,jit_node_t*);
#  define epilog(i0)			_epilog(_jit,i0)
static void _epilog(jit_state_t*,jit_node_t*);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#  define patch_at(instr,label)		_patch_at(_jit,instr,label)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
#endif

#if CODE
#  define _us				jit_uint16_t
#  define _ui				jit_uint32_t
static void
_E(jit_state_t *_jit, _ui Op)
{
    union {
	struct {
	    _us	op;
	} b;
	_us	s;
    } i0;
    i0.b.op = Op;
    assert(i0.b.op == Op);
    is(i0.s);
}

static void
_I(jit_state_t *_jit, _ui Op, _ui I)
{
    union {
	struct {
	    _us	op : 8;
	    _us	i  : 8;
	} b;
	_us	s;
    } i0;
    i0.b.op = Op;
    i0.b.i  = I;
    assert(i0.b.op == Op);
    assert(i0.b.i  == I);
    is(i0.s);
}

static void
_RR(jit_state_t *_jit, _ui Op, _ui R1, _ui R2)
{
    union {
	struct {
	    _us	op : 8;
	    _us	r1 : 4;
	    _us	r2 : 4;
	} b;
	_us	s;
    } i0;
    i0.b.op = Op;
    i0.b.r1 = R1;
    i0.b.r2 = R2;
    assert(i0.b.op == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.r2 == R2);
    is(i0.s);
}

static void
_RRE(jit_state_t *_jit, _ui Op, _ui R1, _ui R2)
{
    union {
	struct {
	    _us	op;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	_  : 8;
	    _us	r1 : 4;
	    _us	r2 : 4;
	} b;
	_us	s;
    } i1;
    i0.b.op = Op;
    i1.b._ = 0;
    i1.b.r1 = R1;
    i1.b.r2 = R2;
    assert(i0.b.op == Op);
    assert(i1.b.r1 == R1);
    assert(i1.b.r2 == R2);
    is(i0.s);
    is(i1.s);
}

static void
_RRF(jit_state_t *_jit, _ui Op, _ui R3, _ui M4, _ui R1, _ui R2)
{
    union {
	struct {
	    _us	op;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	r3 : 4;
	    _us	m4 : 4;
	    _us	r1 : 4;
	    _us	r2 : 4;
	} b;
	_us	s;
    } i1;
    i0.b.op = Op;
    i1.b.r3 = R3;
    i1.b.m4 = M4;
    i1.b.r1 = R1;
    i1.b.r2 = R2;
    assert(i0.b.op == Op);
    assert(i1.b.r3 == R3);
    assert(i1.b.m4 == M4);
    assert(i1.b.r1 == R1);
    assert(i1.b.r2 == R2);
    is(i0.s);
    is(i1.s);
}

static void
_RX(jit_state_t *_jit, _ui Op, _ui R1, _ui X2, _ui B2, _ui D2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	r1 :  4;
	    _us	x2 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b2 :  4;
	    _us	d2 : 12;
	} b;
	_us	s;
    } i1;
    i0.b.op = Op;
    i0.b.r1 = R1;
    i0.b.x2 = X2;
    i1.b.b2 = B2;
    i1.b.d2 = D2;
    assert(i0.b.op == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.x2 == X2);
    assert(i1.b.b2 == B2);
    assert(i1.b.d2 == D2);
    is(i0.s);
    is(i1.s);
}

static void
_RXE(jit_state_t *_jit, _ui Op, _ui R1, _ui X2, _ui B2, _ui D2, _ui Op2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	r1 :  4;
	    _us	x2 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b2 :  4;
	    _us	d2 : 12;
	} b;
	_ui	s;
    } i1;
    union {
	struct {
	    _us	_  :  8;
	    _us	op :  8;
	} b;
	_us	s;
    } i2;
    i2.b._ = 0;
    i0.b.op = Op;
    i0.b.r1 = R1;
    i0.b.x2 = X2;
    i1.b.b2 = B2;
    i1.b.d2 = D2;
    i2.b.op = Op2;
    assert(i0.b.op == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.x2 == X2);
    assert(i1.b.b2 == B2);
    assert(i1.b.d2 == D2);
    assert(i2.b.op == Op2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}

static void
_RXF(jit_state_t *_jit, _ui Op, _ui R3, _ui X2, _ui B2, _ui D2, _ui R1, _ui Op2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	r3 :  4;
	    _us	x2 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b2 :  4;
	    _us	d2 : 12;
	} b;
	_us	s;
    } i1;
    union {
	struct {
	    _us	r1 :  4;
	    _us	_  :  4;
	    _us	op :  8;
	} b;
	_us	s;
    } i2;
    i2.b._ = 0;
    i0.b.op = Op;
    i0.b.r3 = R3;
    i0.b.x2 = X2;
    i1.b.b2 = B2;
    i1.b.d2 = D2;
    i2.b.r1 = R1;
    i2.b.op = Op2;
    assert(i0.b.op == Op);
    assert(i0.b.r3 == R3);
    assert(i0.b.x2 == X2);
    assert(i1.b.b2 == B2);
    assert(i1.b.d2 == D2);
    assert(i2.b.r1 == R1);
    assert(i2.b.op == Op2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}

static void
_RXY(jit_state_t *_jit, _ui Op, _ui R1, _ui X2, _ui B2, _ui D2, _ui Op2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	r1 :  4;
	    _us	x2 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b2 :  4;
	    _us	dl : 12;
	} b;
	_us	s;
    } i1;
    union {
	struct {
	    _us	dh :  8;
	    _us	op :  8;
	} b;
	_us	s;
    } i2;
    i0.s = i1.s = i2.s = 0;
    i0.b.op = Op;
    i0.b.r1 = R1;
    i0.b.x2 = X2;
    i1.b.b2 = B2;
    i1.b.dl = D2 & 0xfff;
    i2.b.dh = D2 >> 12;
    i2.b.op = Op2;
    assert(i0.b.op == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.x2 == X2);
    assert(i1.b.b2 == B2);
    assert(i2.b.dh == D2 >> 12);
    assert(i2.b.op == Op2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}

static void
_RS(jit_state_t *_jit, _ui Op, _ui R1, _ui R3, _ui B2, _ui D2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	r1 :  4;
	    _us	r3 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b2 :  4;
	    _us	d2 : 12;
	} b;
	_us	s;
    } i1;
    i0.s = i1.s = 0;
    i0.b.op = Op;
    i0.b.r1 = R1;
    i0.b.r3 = R3;
    i1.b.b2 = B2;
    i1.b.d2 = D2;
    assert(i0.b.op == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.r3 == R3);
    assert(i1.b.b2 == B2);
    assert(i1.b.d2 == D2);
    is(i0.s);
    is(i1.s);
}

static void
_RSL(jit_state_t *_jit, _ui Op, _ui L1, _ui B1, _ui D1, _ui Op2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	l1 :  4;
	    _us _  :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b1 :  4;
	    _us	d1 : 12;
	} b;
	_us	s;
    } i1;
    union {
	struct {
	    _us	_  :  8;
	    _us	op :  8;
	} b;
	_us	s;
    } i2;
    i0.b._ = 0;
    i2.b._ = 0;
    i0.b.op = Op;
    i0.b.l1 = L1;
    i1.b.b1 = B1;
    i1.b.d1 = D1;
    i2.b.op = Op2;
    assert(i0.b.op == Op);
    assert(i0.b.l1 == L1);
    assert(i1.b.b1 == B1);
    assert(i1.b.d1 == D1);
    assert(i2.b.op == Op2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}

static void
_RSI(jit_state_t *_jit, _ui Op, _ui R1, _ui R3, _ui I2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	r1 :  4;
	    _us	r3 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	i2;
	} b;
	_us	s;
    } i1;
    i0.b.op = Op;
    i0.b.r1 = R1;
    i0.b.r3 = R3;
    i1.b.i2 = I2;
    assert(i0.b.op == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.r3 == R3);
    assert(i1.b.i2 == I2);
    is(i0.s);
    is(i1.s);
}

static void
_RIE(jit_state_t *_jit, _ui Op, _ui R1, _ui R3, _ui I2, _ui Op2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	r1 :  4;
	    _us	r3 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	i2;
	} b;
	_us	s;
    } i1;
    union {
	struct {
	    _us	_  :  8;
	    _us	op :  8;
	} b;
	_us	s;
    } i2;
    i2.b._ = 0;
    i0.b.op = Op;
    i0.b.r1 = R1;
    i0.b.r3 = R3;
    i1.b.i2 = I2;
    i2.b.op = Op2;
    assert(i0.b.op == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.r3 == R3);
    assert(i1.b.i2 == I2);
    assert(i2.b.op == Op2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}

static void
_RIL(jit_state_t *_jit, _ui Op, _ui R1, _ui Op2, _ui I2)
{
    union {
	struct {
	    _us	o1 :  8;
	    _us	r1 :  4;
	    _us	o2 :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _ui	ih : 16;
	    _ui	il : 16;
	} b;
	_ui	i;
    } i12;
    i0.b.o1 = Op;
    i0.b.r1 = R1;
    i0.b.o2 = Op2;
    i12.i   = I2;
    assert(i0.b.o1 == Op);
    assert(i0.b.r1 == R1);
    assert(i0.b.o2 == Op2);
    is(i0.s);
    is(i12.b.ih);
    is(i12.b.il);
}

static void
_SI(jit_state_t *_jit, _ui Op, _ui I2, _ui B1, _ui D1)
{
    union {
	struct {
	    _us	op :  8;
	    _us	i2 :  8;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b1 :  4;
	    _us	d1 : 12;
	} b;
	_us	s;
    } i1;
    i0.b.op = Op;
    i0.b.i2 = I2;
    i1.b.b1 = B1;
    i1.b.d1 = D1;
    assert(i0.b.op == Op);
    assert(i0.b.i2 == I2);
    assert(i1.b.b1 == B1);
    assert(i1.b.d1 == D1);
    is(i0.s);
    is(i1.s);
}

static void
_SIY(jit_state_t *_jit, _ui Op, _ui I2, _ui B1, _ui D1, _ui Op2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	i2 :  8;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b1 :  4;
	    _us	dl : 12;
	} b;
	_us	s;
    } i1;
    union {
	struct {
	    _us	dh :  8;
	    _us	op :  8;
	} b;
	_us	s;
    } i2;
    i0.b.op = Op;
    i0.b.i2 = I2;
    i1.b.b1 = B1;
    i1.b.dl = D1 & 0xfff;
    i2.b.dh = D1 >> 8;
    i2.b.op = Op2;
    assert(i0.b.op == Op);
    assert(i0.b.i2 == I2);
    assert(i1.b.b1 == B1);
    assert(i2.b.dh == D1 >> 8);
    assert(i2.b.op == Op2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}

static void
_S(jit_state_t *_jit, _ui Op, _ui B2, _ui D2)
{
    union {
	struct {
	    _us	op;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b2 :  4;
	    _us	d2 : 12;
	} b;
	_us	s;
    } i1;
    i0.b.op = Op;
    i1.b.b2 = B2;
    i1.b.d2 = D2;
    assert(i0.b.op == Op);
    assert(i1.b.b2 == B2);
    assert(i1.b.d2 == D2);
    is(i0.s);
    is(i1.s);
}

static void
_SS(jit_state_t *_jit, _ui Op, _ui LL, _ui LH, _ui B1, _ui D1, _ui B2, _ui D2)
{
    union {
	struct {
	    _us	op :  8;
	    _us	ll :  4;
	    _us	lh :  4;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b1 :  4;
	    _us	d1 : 12;
	} b;
	_us	s;
    } i1;
    union {
	struct {
	    _us	b2 :  4;
	    _us	d2 : 12;
	} b;
	_us	s;
    } i2;
    i0.b.op = Op;
    i0.b.ll = LL;
    i0.b.lh = LH;
    i1.b.b1 = B1;
    i1.b.d1 = D1;
    i2.b.b2 = B2;
    i2.b.d2 = D2;
    assert(i0.b.op == Op);
    assert(i0.b.ll == LL);
    assert(i0.b.lh == LH);
    assert(i1.b.b1 == B1);
    assert(i1.b.d1 == D1);
    assert(i2.b.b2 == B2);
    assert(i2.b.d2 == D2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}

static void
_SSE(jit_state_t *_jit, _ui Op, _ui B1, _ui D1, _ui B2, _ui D2)
{
    union {
	struct {
	    _us	op;
	} b;
	_us	s;
    } i0;
    union {
	struct {
	    _us	b1 :  4;
	    _us	d1 : 12;
	} b;
	_us	s;
    } i1;
    union {
	struct {
	    _us	b2 :  4;
	    _us	d2 : 12;
	} b;
	_us	s;
    } i2;
    i0.b.op = Op;
    i1.b.b1 = B1;
    i1.b.d1 = D1;
    i2.b.b2 = B2;
    i2.b.d2 = D2;
    assert(i0.b.op == Op);
    assert(i1.b.b1 == B1);
    assert(i1.b.d1 == D1);
    assert(i2.b.b2 == B2);
    assert(i2.b.d2 == D2);
    is(i0.s);
    is(i1.s);
    is(i2.s);
}
#  undef _us
#  undef _ui

static void
_nop(jit_state_t *_jit, jit_int32_t c)
{
    assert(c >= 0 && !(c & 1));
    while (c) {
	NOPR(_R7_REGNO);
	c -= 2;
    }
}

static jit_int32_t
_xdivr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		regno;
    regno = jit_get_reg_pair();
#if __WORDSIZE == 32
    movr(rn(regno), r0);
    SRDA(rn(regno), 32, 0);
#else
    movr(rn(regno) + 1, r0);
#endif
    DIVREM_(rn(regno), r1);
    jit_unget_reg_pair(regno);
    return (regno);
}

static jit_int32_t
_xdivr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		regno;
    regno = jit_get_reg_pair();
#if __WORDSIZE == 32
    movr(rn(regno), r0);
    SRDL(rn(regno), 32, 0);
#else
    movr(rn(regno) + 1, r0);
#endif
    movi(rn(regno), 0);
    DIVREMU_(rn(regno), r1);
    jit_unget_reg_pair(regno);
    return (regno);
}

static jit_int32_t
_xdivi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		imm, regno;
    regno = jit_get_reg_pair();
    imm = jit_get_reg(jit_class_gpr);
#if __WORDSIZE == 32
    movr(rn(regno), r0);
    SRDA(rn(regno), 32, 0);
#else
    movr(rn(regno) + 1, r0);
#endif
    movi(rn(imm), i0);
    DIVREM_(rn(regno), rn(imm));
    jit_unget_reg(imm);
    jit_unget_reg_pair(regno);
    return (regno);
}

static jit_int32_t
_xdivi_u(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    /* cannot overlap because operand is 128-bit */
    jit_int32_t		imm, regno;
    regno = jit_get_reg_pair();
    imm = jit_get_reg(jit_class_gpr);
#if __WORDSIZE == 32
    movr(rn(regno), r0);
    SRDL(rn(regno), 32, 0);
#else
    movr(rn(regno) + 1, r0);
#endif
    movi(rn(regno), 0);
    movi(rn(imm), i0);
    DIVREMU_(rn(regno), rn(imm));
    jit_unget_reg(imm);
    jit_unget_reg_pair(regno);
    return (regno);
}

static void
_crr(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg, rg;
    if (r0 == r1 || r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	rg = rn(reg);
    }
    else
	rg = r0;
    movi(rg, 1);
    CMP_(r1, r2);
    w = _jit->pc.w;
    BRC(cc, 0);
    movi(rg, 0);
    patch_at(w, _jit->pc.w);
    if (r0 == r1 || r0 == r2) {
	movr(r0, rg);
	jit_unget_reg(reg);
    }
}

static void
_cri(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    crr(cc, r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_crr_u(jit_state_t *_jit, jit_int32_t cc,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg, rg;
    if (r0 == r1 || r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	rg = rn(reg);
    }
    else
	rg = r0;
    movi(rg, 1);
    CMPU_(r1, r2);
    w = _jit->pc.w;
    BRC(cc, 0);
    movi(rg, 0);
    patch_at(w, _jit->pc.w);
    if (r0 == r1 || r0 == r2) {
	movr(r0, rg);
	jit_unget_reg(reg);
    }
}

static void
_cri_u(jit_state_t *_jit, jit_int32_t cc,
       jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    crr_u(cc, r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_brr(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d;
    CMP_(r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(cc, x16(d));
    else {
	assert(s32_p(d));
	BRCL(cc, d);
    }
}

static jit_word_t
_brr_p(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP_(r0, r1);
    w = _jit->pc.w;
    BRCL(cc, 0);
    return (w);
}

static void
_bri(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    brr(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static jit_word_t
_bri_p(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = brr_p(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_brr_u(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d;
    CMPU_(r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(cc, x16(d));
    else {
	assert(s32_p(d));
	BRCL(cc, d);
    }
}

static jit_word_t
_brr_u_p(jit_state_t *_jit, jit_int32_t cc,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMPU_(r0, r1);
    w = _jit->pc.w;
    BRCL(cc, 0);
    return (w);
}

static void
_bri_u(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    brr_u(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static jit_word_t
_bri_u_p(jit_state_t *_jit, jit_int32_t cc,
	 jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = brr_u_p(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_baddr(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d;
    if (s)		addr(r0, r0, r1);
    else		addcr(r0, r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(c, x16(d));
    else {
	assert(s32_p(d));
	BRCL(c, d);
    }
}

static void
_baddi(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
       jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    baddr(c, s, i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static jit_word_t
_baddr_p(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    if (s)		addr(r0, r0, r1);
    else		addcr(r0, r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    w = _jit->pc.w;
    BRCL(c, d);
    return (w);
}

static jit_word_t
_baddi_p(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
	 jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = baddr_p(c, s, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_bsubr(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d;
    if (s)		subr(r0, r0, r1);
    else		subcr(r0, r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(c, x16(d));
    else {
	assert(s32_p(d));
	BRCL(c, d);
    }
}

static void
_bsubi(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
       jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    bsubr(c, s, i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static jit_word_t
_bsubr_p(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d, w;
    if (s)		subr(r0, r0, r1);
    else		subcr(r0, r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    w = _jit->pc.w;
    BRCL(c, d);
    return (w);
}

static jit_word_t
_bsubi_p(jit_state_t *_jit, jit_int32_t c, jit_bool_t s,
	 jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bsubr_p(c, s, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_bmxr(jit_state_t *_jit, jit_int32_t cc,
      jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movr(rn(reg), r0);
    andr(rn(reg), rn(reg), r1);
    TEST_(rn(reg), rn(reg));
    jit_unget_reg(reg);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(cc, x16(d));
    else {
	assert(s32_p(d));
	BRCL(cc, d);
    }
}

static jit_word_t
_bmxr_p(jit_state_t *_jit, jit_int32_t cc,
	jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movr(rn(reg), r0);
    andr(rn(reg), rn(reg), r1);
    TEST_(rn(reg), rn(reg));
    jit_unget_reg(reg);
    w = _jit->pc.w;
    BRCL(cc, 0);
    return (w);
}

static void
_bmxi(jit_state_t *_jit, jit_int32_t cc,
      jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		d;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i1);
    andr(rn(reg), rn(reg), r0);
    TEST_(rn(reg), rn(reg));
    jit_unget_reg(reg);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(cc, x16(d));
    else {
	assert(s32_p(d));
	BRCL(cc, d);
    }
}

static jit_word_t
_bmxi_p(jit_state_t *_jit, jit_int32_t cc,
	jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i1);
    andr(rn(reg), rn(reg), r0);
    TEST_(rn(reg), rn(reg));
    jit_unget_reg(reg);
    w = _jit->pc.w;
    BRCL(cc, 0);
    return (w);
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if __WORDSIZE == 32
    if (r0 != r1)
	LR(r0, r1);
#else
    if (r0 != r1)
	LGR(r0, r1);
#endif
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		d;
#if __WORDSIZE == 64
    jit_int32_t		bits;
#endif
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(i0)) {
#if __WORDSIZE == 32
	LHI(r0, x16(i0));
#else
	LGHI(r0, x16(i0));
#endif
    }
    /* easy way of loading a large amount of 32 bit values and
     * usually address of constants */
    else if (!(i0 & 1) &&
#if __WORDSIZE == 32
	     i0 > 0
#else
	     s32_p(d)
#endif
	     )
	LARL(r0, d);
    else {
#if __WORDSIZE == 32
	LHI(r0, x16(i0));
	IILH(r0, x16((jit_uword_t)i0 >> 16));
#else
	bits = 0;
	if (i0 &             0xffffL)	bits |= 1;
	if (i0 &         0xffff0000L)	bits |= 2;
	if (i0 &     0xffff00000000L)	bits |= 4;
	if (i0 & 0xffff000000000000L)	bits |= 8;
	if (bits != 15)			LGHI(r0, 0);
	if (bits & 1)			IILL(r0, x16(i0));
	if (bits & 2)			IILH(r0, x16((jit_uword_t)i0 >> 16));
	if (bits & 4)			IIHL(r0, x16((jit_uword_t)i0 >> 32));
	if (bits & 8)			IIHH(r0, x16((jit_uword_t)i0 >> 48));
#endif
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    w = _jit->pc.w;
#if __WORDSIZE == 32
    LHI(r0, x16(i0));
#else
    IILL(r0, x16(i0));
#endif
    IILH(r0, x16((jit_uword_t)i0 >> 16));
#if __WORDSIZE == 64
    IIHL(r0, x16((jit_uword_t)i0 >> 32));
    IIHH(r0, x16((jit_uword_t)i0 >> 48));
#endif
    return (w);
}

static void
_addr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	ADD_(r0, r1);
    else {
	movr(r0, r1);
	ADD_(r0, r2);
    }
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (r0 == r1 && s16_p(i0))
	ADDI_(r0, x16(i0));
#if __WORDSIZE == 64
    else if (s20_p(i0))
	LAY(r0, x20(i0), 0, r1);
#endif
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
    if (r0 == r2)
	ADDC_(r0, r1);
    else {
	movr(r0, r1);
	ADDC_(r0, r2);
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
_addxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	ADDX_(r0, r1);
    else {
	movr(r0, r1);
	ADDX_(r0, r2);
    }
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
_subr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r2);
	movr(r0, r1);
	SUB_(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr(r0, r1);
	SUB_(r0, r2);
    }
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (r0 == r1 && s16_p(-i0))
	ADDI_(r0, x16(-i0));
#if __WORDSIZE == 64
    else if (s20_p(-i0))
	LAY(r0, x20(-i0), 0, r1);
#endif
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r2);
	movr(r0, r1);
	SUBC_(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr(r0, r1);
	SUBC_(r0, r2);
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
_subxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r2);
	movr(r0, r1);
	SUBX_(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr(r0, r1);
	SUBX_(r0, r2);
    }
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
    subi(r0, r1, i0);
    negr(r0, r0);
}

static void
_mulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	MUL_(r0, r1);
    else {
	movr(r0, r1);
	MUL_(r0, r2);
    }
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s16_p(i0)) {
	movr(r0, r1);
	MULI_(r0, x16(i0));
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	mulr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_qmulr(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		reg;
    /* The only invalid condition is r0 == r1 */
    jit_int32_t		t2, t3, s2, s3;
    if (r2 == r0 || r2 == r1) {
	s2 = jit_get_reg(jit_class_gpr);
	t2 = rn(s2);
	movr(t2, r2);
    }
    else
	t2 = r2;
    if (r3 == r0 || r3 == r1) {
	s3 = jit_get_reg(jit_class_gpr);
	t3 = rn(s3);
	movr(t3, r3);
    }
    else
	t3 = r3;
    qmulr_u(r0, r1, r2, r3);
    reg = jit_get_reg(jit_class_gpr);
    /**/
    rshi(rn(reg), t2, 63);
    mulr(rn(reg), rn(reg), t3);
    addr(r1, r1, rn(reg));
    /**/
    rshi(rn(reg), t3, 63);
    mulr(rn(reg), rn(reg), t2);
    addr(r1, r1, rn(reg));
    jit_unget_reg(reg);
    if (t2 != r2)
	jit_unget_reg(s2);
    if (t3 != r3)
	jit_unget_reg(s3);
}

static void
_qmuli(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    qmulr(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
}

static void
_qmulr_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		regno;
    regno = jit_get_reg_pair();
    movr(rn(regno) + 1, r2);
    MULU_(rn(regno), r3);
    movr(r0, rn(regno) + 1);
    movr(r1, rn(regno));
    jit_unget_reg_pair(regno);
}

static void
_qmuli_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		regno;
    regno = jit_get_reg_pair();
    movr(rn(regno) + 1, r2);
    movi(rn(regno), i0);
    MULU_(rn(regno), rn(regno));
    movr(r0, rn(regno) + 1);
    movr(r1, rn(regno));
    jit_unget_reg_pair(regno);
}

static void
_divr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		regno;
    regno = xdivr(r1, r2);
    movr(r0, rn(regno) + 1);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		regno;
    regno = xdivi(r1, i0);
    movr(r0, rn(regno) + 1);
}

static void
_divr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		regno;
    regno = xdivr_u(r1, r2);
    movr(r0, rn(regno) + 1);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		regno;
    regno = xdivi_u(r1, i0);
    movr(r0, rn(regno) + 1);
}

static void
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		regno;
    regno = xdivr(r1, r2);
    movr(r0, rn(regno));
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		regno;
    regno = xdivi(r1, i0);
    movr(r0, rn(regno));
}

static void
_remr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		regno;
    regno = xdivr_u(r1, r2);
    movr(r0, rn(regno));
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		regno;
    regno = xdivi_u(r1, i0);
    movr(r0, rn(regno));
}

static void
_qdivr(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		regno;
    regno = xdivr(r2, r3);
    movr(r0, rn(regno) + 1);
    movr(r1, rn(regno));
}

static void
_qdivi(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		regno;
    regno = xdivi(r2, i0);
    movr(r0, rn(regno) + 1);
    movr(r1, rn(regno));
}

static void
_qdivr_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		regno;
    regno = xdivr_u(r2, r3);
    movr(r0, rn(regno) + 1);
    movr(r1, rn(regno));
}

static void
_qdivi_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		regno;
    regno = xdivi_u(r2, i0);
    movr(r0, rn(regno) + 1);
    movr(r1, rn(regno));
}

#  if __WORDSIZE == 32
static void
_lshr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg_but_zero(0);
	movr(rn(reg), r2);
	movr(r0, r1);
	SLL(r0, 0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
    else {
	movr(r0, r1);
	SLL(r0, 0, r2);
    }
}
#endif

static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    lshr(r0, r1, rn(reg));
    jit_unget_reg_but_zero(reg);
}

#  if __WORDSIZE == 32
static void
_rshr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg_but_zero(0);
	movr(rn(reg), r2);
	movr(r0, r1);
	SRA(r0, 0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
    else {
	movr(r0, r1);
	SRA(r0, 0, r2);
    }
}
#endif

static void
_rshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    rshr(r0, r1, rn(reg));
    jit_unget_reg_but_zero(reg);
}

#  if __WORDSIZE == 32
static void
_rshr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg_but_zero(0);
	movr(rn(reg), r2);
	movr(r0, r1);
	SRL(r0, 0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
    else {
	movr(r0, r1);
	SRL(r0, 0, r2);
    }
}
#endif

static void
_rshi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    rshr_u(r0, r1, rn(reg));
    jit_unget_reg_but_zero(reg);
}

static void
_comr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), -1);
    movr(r0, r1);
    XOR_(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_andr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	AND_(r0, r1);
    else {
	movr(r0, r1);
	AND_(r0, r2);
    }
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(r0, r1);
    NILL(r0, x16(i0));
    NILH(r0, x16((jit_uword_t)i0 >> 16));
#if __WORDSIZE == 64
    NIHL(r0, x16((jit_uword_t)i0 >> 32));
    NIHH(r0, x16((jit_uword_t)i0 >> 48));
#endif
}

static void
_orr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	OR_(r0, r1);
    else {
	movr(r0, r1);
	OR_(r0, r2);
    }
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(r0, r1);
    OILL(r0, x16(i0));
    OILH(r0, x16((jit_uword_t)i0 >> 16));
#if __WORDSIZE == 64
    OIHL(r0, x16((jit_uword_t)i0 >> 32));
    OIHH(r0, x16((jit_uword_t)i0 >> 48));
#endif
}

static void
_xorr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	XOR_(r0, r1);
    else {
	movr(r0, r1);
	XOR_(r0, r2);
    }
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
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    movi(r0, i0);
    ldr_c(r0, r0);
}

static void
_ldxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2) {
	addr(r0, r0, r1);
	ldr_c(r0, r0);
    }
    else {
	movr(r0, r1);
	addr(r0, r0, r2);
	ldr_c(r0, r0);
    }
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s20_p(i0)) {
#if __WORDSIZE == 32
	LB(r0, x20(i0), 0, r1);
#else
	LGB(r0, x20(i0), 0, r1);
#endif
    }
    else if (r0 != r1) {
	movi(r0, i0);
	addr(r0, r0, r1);
	ldr_c(r0, r0);
    }
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_c(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    movi(r0, i0);
    ldr_uc(r0, r0);
}

static void
_ldxr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2) {
	addr(r0, r0, r1);
	ldr_uc(r0, r0);
    }
    else {
	movr(r0, r1);
	addr(r0, r0, r2);
	ldr_uc(r0, r0);
    }
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s20_p(i0))
	LLGC(r0, x20(i0), 0, r1);
    else if (r0 != r1) {
	movi(r0, i0);
	addr(r0, r0, r1);
	ldr_uc(r0, r0);
    }
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_uc(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    movi(r0, i0);
    ldr_s(r0, r0);
}

static void
_ldxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2) {
	addr(r0, r0, r1);
	ldr_s(r0, r0);
    }
    else {
	movr(r0, r1);
	addr(r0, r0, r2);
	ldr_s(r0, r0);
    }
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
#if __WORDSIZE == 32
    if (u12_p(i0))
	LH(r0, i0, 0, r1);
    else
#endif
    if (s20_p(i0)) {
#if __WORDSIZE == 32
	LHY(r0, x20(i0), 0, r1);
#else
	LGH(r0, x20(i0), 0, r1);
#endif
    }
    else if (r0 != r1) {
	movi(r0, i0);
	addr(r0, r0, r1);
	ldr_s(r0, r0);
    }
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_s(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    movi(r0, i0);
    ldr_us(r0, r0);
}

static void
_ldxr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2) {
	addr(r0, r0, r1);
	ldr_us(r0, r0);
    }
    else {
	movr(r0, r1);
	addr(r0, r0, r2);
	ldr_us(r0, r0);
    }
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s20_p(i0))
	LLGH(r0, x20(i0), 0, r1);
    else if (r0 != r1) {
	movi(r0, i0);
	addr(r0, r0, r1);
	ldr_us(r0, r0);
    }
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_us(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    movi(r0, i0);
    ldr_i(r0, r0);
}

static void
_ldxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2) {
	addr(r0, r0, r1);
	ldr_i(r0, r0);
    }
    else {
	movr(r0, r1);
	addr(r0, r0, r2);
	ldr_i(r0, r0);
    }
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s20_p(i0))
	LGF(r0, x20(i0), 0, r1);
    else if (r0 != r1) {
	movi(r0, i0);
	addr(r0, r0, r1);
	ldr_i(r0, r0);
    }
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_i(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

#if __WORDSIZE == 64
static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    movi(r0, i0);
    ldr_ui(r0, r0);
}

static void
_ldxr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2) {
	addr(r0, r0, r1);
	ldr_ui(r0, r0);
    }
    else {
	movr(r0, r1);
	addr(r0, r0, r2);
	ldr_ui(r0, r0);
    }
}

static void
_ldxi_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s20_p(i0))
	LLGF(r0, x20(i0), 0, r1);
    else if (r0 != r1) {
	movi(r0, i0);
	addr(r0, r0, r1);
	ldr_ui(r0, r0);
    }
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_ui(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static void
_ldi_l(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    movi(r0, i0);
    ldr_l(r0, r0);
}

static void
_ldxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2) {
	addr(r0, r0, r1);
	ldr_l(r0, r0);
    }
    else {
	movr(r0, r1);
	addr(r0, r0, r2);
	ldr_l(r0, r0);
    }
}

static void
_ldxi_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s20_p(i0))
	LG(r0, x20(i0), 0, r1);
    else if (r0 != r1) {
	movi(r0, i0);
	addr(r0, r0, r1);
	ldr_l(r0, r0);
    }
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_l(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}
#endif

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    str_c(rn(reg), r0);
    jit_unget_reg_but_zero(reg);
}

static void
_stxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r0);
    addr(rn(reg), rn(reg), r1);
    str_c(rn(reg), r2);
    jit_unget_reg_but_zero(reg);
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (u12_p(i0))
	STC(r1, i0, 0, r0);
    else if (s20_p(i0))
	STCY(r1, x20(i0), 0, r0);
    else {
	reg = jit_get_reg_but_zero(0);
	addi(rn(reg), r0, i0);
	str_c(rn(reg), r1);
	jit_unget_reg_but_zero(reg);
    }
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    str_s(rn(reg), r0);
    jit_unget_reg_but_zero(reg);
}

static void
_stxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r0);
    addr(rn(reg), rn(reg), r1);
    str_s(rn(reg), r2);
    jit_unget_reg_but_zero(reg);
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (u12_p(i0))
	STH(r1, i0, 0, r0);
    else if (s20_p(i0))
	STHY(r1, x20(i0), 0, r0);
    else {
	reg = jit_get_reg_but_zero(0);
	addi(rn(reg), r0, i0);
	str_s(rn(reg), r1);
	jit_unget_reg_but_zero(reg);
    }
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    str_i(rn(reg), r0);
    jit_unget_reg_but_zero(reg);
}

static void
_stxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r0);
    addr(rn(reg), rn(reg), r1);
    str_i(rn(reg), r2);
    jit_unget_reg_but_zero(reg);
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (u12_p(i0))
	ST(r1, i0, 0, r0);
    else if (s20_p(i0))
	STY(r1, x20(i0), 0, r0);
    else {
	reg = jit_get_reg_but_zero(0);
	addi(rn(reg), r0, i0);
	str_i(rn(reg), r1);
	jit_unget_reg_but_zero(reg);
    }
}

#if __WORDSIZE == 64
static void
_sti_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    str_l(rn(reg), r0);
    jit_unget_reg_but_zero(reg);
}

static void
_stxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r0);
    addr(rn(reg), rn(reg), r1);
    str_l(rn(reg), r2);
    jit_unget_reg_but_zero(reg);
}

static void
_stxi_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (s20_p(i0))
	STG(r1, x20(i0), 0, r0);
    else {
	reg = jit_get_reg_but_zero(0);
	addi(rn(reg), r0, i0);
	str_l(rn(reg), r1);
	jit_unget_reg_but_zero(reg);
    }
}
#endif

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d;
    jit_int32_t		reg;
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	J(x16(d));
    else if (s32_p(d))
	BRL(d);
    else {
	reg = jit_get_reg_but_zero(jit_class_nospill);
	movi(rn(reg), i0);
	jmpr(rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(jit_class_nospill);
    w = movi_p(rn(reg), i0);
    jmpr(rn(reg));
    jit_unget_reg_but_zero(reg);
    return (w);
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d;
    jit_int32_t		reg;
    d = (i0 - _jit->pc.w) >> 1;
    if (s32_p(d))
	BRASL(_R14_REGNO, d);
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	callr(rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    w = movi_p(rn(reg), i0);
    callr(rn(reg));
    jit_unget_reg_but_zero(reg);
    return (w);
}

static jit_int32_t	gprs[] = {
    _R2, _R3, _R4, _R5,
    _R6, _R7, _R8, _R9, _R10, _R11, _R12, _R13
};

static void
_prolog(jit_state_t *_jit, jit_node_t *i0)
{
    jit_int32_t		regno, offset;
    if (_jitc->function->define_frame || _jitc->function->assume_frame) {
	jit_int32_t	frame = -_jitc->function->frame;
	assert(_jitc->function->self.aoff >= frame);
	if (_jitc->function->assume_frame)
	    return;
	_jitc->function->self.aoff = frame;
    }
    if (_jitc->function->allocar)
	_jitc->function->self.aoff &= -8;
    _jitc->function->stack = ((_jitc->function->self.alen -
			      /* align stack at 8 bytes */
			      _jitc->function->self.aoff) + 7) & -8;
    /* *IFF* a non variadic function,
     * Lightning does not reserve stack space for spilling arguments
     * in registers.
     * S390x, as per gcc, has 8 stack slots for spilling arguments,
     * (%r6 is callee save) and uses an alloca like approach to save
     * callee save fpr registers.
     * Since argument registers are not saved in any lightning port,
     * use the 8 slots to spill any modified fpr register, and still
     * use the same stack frame logic as gcc.
     * Save at least %r13 to %r15, as %r13 is used as frame pointer.
     * *IFF* a variadic function, a "standard" stack frame, with
     * fpr registers saved in an alloca'ed area, is used.
     */
    if ((_jitc->function->self.call & jit_call_varargs) &&
	jit_arg_reg_p(_jitc->function->vagp))
	regno = _jitc->function->vagp;
    else {
	for (regno = 4; regno < jit_size(gprs) - 1; regno++) {
	    if (jit_regset_tstbit(&_jitc->function->regset, gprs[regno]))
		break;
	}
    }
#if __WORDSIZE == 32
#  define FP_OFFSET		64
    if (_jitc->function->self.call & jit_call_varargs)
	offset = regno * 4 + 8;
    else
	offset = (regno - 4) * 4 + 32;
    STM(rn(gprs[regno]), _R15_REGNO, x20(offset), _R15_REGNO);
#else
#  define FP_OFFSET		128
    if (_jitc->function->self.call & jit_call_varargs)
	offset = regno * 8 + 16;
    else
	offset = (regno - 4) * 8 + 48;
    STMG(rn(gprs[regno]), _R15_REGNO, x20(offset), _R15_REGNO);
#endif

#define SPILL(R, O)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->function->regset, R))		\
	    stxi_d(O, _R15_REGNO, rn(R));				\
    } while (0)
    if (_jitc->function->self.call & jit_call_varargs) {
	for (regno = _jitc->function->vafp; jit_arg_f_reg_p(regno); ++regno)
	    stxi_d(FP_OFFSET + regno * 8, _R15_REGNO, rn(_F0 - regno));
	SPILL(_F8, _jitc->function->vaoff + offsetof(jit_va_list_t, f8));
	SPILL(_F9, _jitc->function->vaoff + offsetof(jit_va_list_t, f9));
	SPILL(_F10, _jitc->function->vaoff + offsetof(jit_va_list_t, f10));
	SPILL(_F11, _jitc->function->vaoff + offsetof(jit_va_list_t, f11));
	SPILL(_F12, _jitc->function->vaoff + offsetof(jit_va_list_t, f12));
	SPILL(_F13, _jitc->function->vaoff + offsetof(jit_va_list_t, f13));
	SPILL(_F14, _jitc->function->vaoff + offsetof(jit_va_list_t, f14));
    }
    else {
	/* First 4 in low address */
#if __WORDSIZE == 32
	SPILL(_F10, 0);
	SPILL(_F11, 8);
	SPILL(_F12, 16);
	SPILL(_F13, 24);
	/* gpr registers here */
	SPILL(_F14, 72);
	SPILL(_F8, 80);
	SPILL(_F9, 88);
#else
	SPILL(_F10, 16);
	SPILL(_F11, 24);
	SPILL(_F12, 32);
	SPILL(_F13, 48);
	/* Last 3 in high address */
	SPILL(_F14, 136);
	SPILL(_F8, 144);
	SPILL(_F9, 152);
#endif
    }
#undef SPILL
    movr(_R13_REGNO, _R15_REGNO);
    subi(_R15_REGNO, _R15_REGNO, stack_framesize + _jitc->function->stack);
    if (_jitc->function->allocar) {
	regno = jit_get_reg(jit_class_gpr);
	movi(rn(regno), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _R13_REGNO, rn(regno));
	jit_unget_reg(regno);
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *i0)
{
    jit_int32_t		regno, offset;
    if (_jitc->function->assume_frame)
	return;
    if ((_jitc->function->self.call & jit_call_varargs) &&
	jit_arg_reg_p(_jitc->function->vagp))
	regno = _jitc->function->vagp;
    else {
	for (regno = 4; regno < jit_size(gprs) - 1; regno++) {
	    if (jit_regset_tstbit(&_jitc->function->regset, gprs[regno]))
		break;
	}
    }
#if __WORDSIZE == 32
    if (_jitc->function->self.call & jit_call_varargs)
	offset = regno * 4 + 8;
    else
	offset = (regno - 4) * 4 + 32;
#else
    if (_jitc->function->self.call & jit_call_varargs)
	offset = regno * 8 + 16;
    else
	offset = (regno - 4) * 8 + 48;
#endif
    movr(_R15_REGNO, _R13_REGNO);

#define LOAD(R, O)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->function->regset, R))		\
	    ldxi_d(rn(R), _R15_REGNO, O);				\
    } while (0)
    if (_jitc->function->self.call & jit_call_varargs) {
	LOAD(_F8, _jitc->function->vaoff + offsetof(jit_va_list_t, f8));
	LOAD(_F9, _jitc->function->vaoff + offsetof(jit_va_list_t, f9));
	LOAD(_F10, _jitc->function->vaoff + offsetof(jit_va_list_t, f10));
	LOAD(_F11, _jitc->function->vaoff + offsetof(jit_va_list_t, f11));
	LOAD(_F12, _jitc->function->vaoff + offsetof(jit_va_list_t, f12));
	LOAD(_F13, _jitc->function->vaoff + offsetof(jit_va_list_t, f13));
	LOAD(_F14, _jitc->function->vaoff + offsetof(jit_va_list_t, f14));
    }
    else {
#if __WORDSIZE == 32
	LOAD(_F10, 0);
	LOAD(_F11, 8);
	LOAD(_F12, 16);
	LOAD(_F13, 24);
	LOAD(_F14, 72);
	LOAD(_F8, 80);
	LOAD(_F9, 88);
#else
	LOAD(_F10, 16);
	LOAD(_F11, 24);
	LOAD(_F12, 32);
	LOAD(_F13, 48);
	LOAD(_F14, 136);
	LOAD(_F8, 144);
	LOAD(_F9, 152);
#endif
    }
#undef LOAD
#if __WORDSIZE == 32
    LM(rn(gprs[regno]), _R15_REGNO, x20(offset), _R15_REGNO);
#else
    LMG(rn(gprs[regno]), _R15_REGNO, x20(offset), _R15_REGNO);
#endif
    BR(_R14_REGNO);
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		reg;

    assert(_jitc->function->self.call & jit_call_varargs);

    /* Return jit_va_list_t in the register argument */
    addi(r0, _R13_REGNO, _jitc->function->vaoff);
    reg = jit_get_reg(jit_class_gpr);

    /* Initialize gp offset in the save area. */
    movi(rn(reg), _jitc->function->vagp);
    stxi(offsetof(jit_va_list_t, gpoff), r0, rn(reg));

    /* Initialize fp offset in the save area. */
    movi(rn(reg), _jitc->function->vafp);
    stxi(offsetof(jit_va_list_t, fpoff), r0, rn(reg));

    /* Initialize overflow pointer to the first stack argument. */
    addi(rn(reg), _R13_REGNO, _jitc->function->self.size);
    stxi(offsetof(jit_va_list_t, over), r0, rn(reg));

    /* Initialize register save area pointer. */
    stxi(offsetof(jit_va_list_t, save), r0, _R13_REGNO);

    jit_unget_reg(reg);
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		rg0;
    jit_int32_t		rg1;
    jit_int32_t		rg2;
    jit_word_t		ge_code;
    jit_word_t		lt_code;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg_but_zero(0);
    rg1 = jit_get_reg_but_zero(0);

    /* Load the gp offset in save area in the first temporary. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, gpoff));

    /* Jump over if there are no remaining arguments in the save area. */
    ge_code = bgei_p(_jit->pc.w, rn(rg0), 5);

    /* Load the save area pointer in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, save));

    /* Scale offset */
    rg2 = jit_get_reg_but_zero(0);
    lshi(rn(rg2), rn(rg0),
#if __WORDSIZE == 32
	 2
#else
	 3
#endif
	 );
    /* Add offset to saved area. */
    addi(rn(rg2), rn(rg2), 2 * sizeof(jit_word_t));

    /* Load the vararg argument in the first argument. */
    ldxr(r0, rn(rg1), rn(rg2));
    jit_unget_reg_but_zero(rg2);

    /* Update the gp offset. */
    addi(rn(rg0), rn(rg0), 1);
    stxi(offsetof(jit_va_list_t, gpoff), r1, rn(rg0));

    /* Will only need one temporary register below. */
    jit_unget_reg_but_zero(rg1);

    /* Jump over overflow code. */
    lt_code = jmpi_p(_jit->pc.w);

    /* Where to land if argument is in overflow area. */
    patch_at(ge_code, _jit->pc.w);

    /* Load overflow pointer. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, over));

    /* Load argument. */
    ldr(r0, rn(rg0));

    /* Update overflow pointer. */
    addi(rn(rg0), rn(rg0), sizeof(jit_word_t));
    stxi(offsetof(jit_va_list_t, over), r1, rn(rg0));

    /* Where to land if argument is in save area. */
    patch_at(lt_code, _jit->pc.w);

    jit_unget_reg_but_zero(rg0);
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    jit_word_t			 d;
    union {
	jit_uint16_t		*s;
	jit_word_t		 w;
    } u;
    u.w = instr;
    union {
	struct {
	    jit_uint16_t	op :  8;
	    jit_uint16_t	r1 :  4;
	    jit_uint16_t	r3 :  4;
	} b;
	jit_uint16_t		s;
    } i0;
    union {
	struct {
	    jit_uint16_t	i2;
	} b;
	jit_uint16_t		s;
    } i1;
    union {
	struct {
	    jit_uint32_t	ih : 16;
	    jit_uint32_t	il : 16;
	} b;
	jit_uint32_t		i;
    } i12;
    i0.s = u.s[0];
    /* movi_p */
    if (i0.b.op ==
#if __WORDSIZE == 32
	0xA7 && i0.b.r3 == 8
#else
	0xA5
#endif
	) {
#if __WORDSIZE == 64
	assert(i0.b.r3 == 3);
#endif
	i1.b.i2 = (jit_uword_t)label;
	u.s[1] = i1.s;
	i0.s = u.s[2];
	assert(i0.b.op == 0xA5 && i0.b.r3 == 2);
	i1.b.i2 = (jit_uword_t)label >> 16;
	u.s[3] = i1.s;
#if __WORDSIZE == 64
	i0.s = u.s[4];
	assert(i0.b.op == 0xA5 && i0.b.r3 == 1);
	i1.b.i2 = (jit_uword_t)label >> 32;
	u.s[5] = i1.s;
	i0.s = u.s[6];
	assert(i0.b.op == 0xA5 && i0.b.r3 == 0);
	i1.b.i2 = (jit_uword_t)label >> 48;
	u.s[7] = i1.s;
#endif
    }
    /* BRC */
    else if (i0.b.op == 0xA7) {
	assert(i0.b.r3 == 0x4);
	d = (label - instr) >> 1;
	assert(s16_p(d));
	i1.b.i2 = d;
	u.s[1] = i1.s;
    }
    /* BRCL */
    else if (i0.b.op == 0xC0) {
	assert(i0.b.r3 == 0x4);
	d = (label - instr) >> 1;
	assert(s32_p(d));
	i12.i = d;
	u.s[1] = i12.b.ih;
	u.s[2] = i12.b.il;
    }
    else
	abort();
}
#endif
