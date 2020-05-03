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
#  define RND_CUR			0
#  define RND_BIAS_NEAR			1
#  define RND_NEAR			4
#  define RND_ZERO			5
#  define RND_POS_INF			6
#  define RND_NEG_INF			7
/****************************************************************
 * Floating Point Instructions					*
 ****************************************************************/
/* CONVERT BFP TO HFP */
#  define THDER(R1,R2)			RRE_(0xB358,R1,R2)
#  define THDR(R1,R2)			RRE_(0xB359,R1,R2)
/* CONVERT HFP TO BFP */
#  define TBEDR(R1,R2)			RRE_(0xB350,R1,R2)
#  define TBDR(R1,R2)			RRE_(0xB351,R1,R2)
/* LOAD */
#  define LER(R1,R2)			RR_(0x38,R1,R2)
#  define LDR(R1,R2)			RR_(0x28,R1,R2)
#  define LXR(R1,R2)			RRE_(0xB365,R1,R2)
#  define LE(R1,D2,X2,B2)		RX_(0x78,R1,X2,B2,D2)
#  define LD(R1,D2,X2,B2)		RX_(0x68,R1,X2,B2,D2)
#  define LEY(R1,D2,X2,B2)		RXY_(0xED,R1,X2,B2,D2,0x64)
#  define LDY(R1,D2,X2,B2)		RXY_(0xED,R1,X2,B2,D2,0x65)
/* LOAD ZERO */
#  define LZER(R1)			RRE_(0xB374,R1,0)
#  define LZDR(R1)			RRE_(0xB375,R1,0)
#  define LZXR(R1)			RRE_(0xB376,R1,0)
/* STORE */
#  define STE(R1,D2,X2,B2)		RX_(0x70,R1,X2,B2,D2)
#  define STD(R1,D2,X2,B2)		RX_(0x60,R1,X2,B2,D2)
#  define STEY(R1,D2,X2,B2)		RXY_(0xED,R1,X2,B2,D2,0x66)
#  define STDY(R1,D2,X2,B2)		RXY_(0xED,R1,X2,B2,D2,0x67)
/****************************************************************
 * Hexadecimal Floating Point Instructions			*
 ****************************************************************/
/* ADD NORMALIZED */
#  define AER(R1,R2)			RR_(0x3A,R1,R2)
#  define ADR(R1,R2)			RR_(0x2A,R1,R2)
#  define AXR(R1,R2)			RR_(0x36,R1,R2)
#  define AE(R1,D2,X2,B2)		RX_(0x7A,R1,X2,B2,D2)
#  define AD(R1,D2,X2,B2)		RX_(0x6A,R1,X2,B2,D2)
/* ADD UNNORMALIZED */
#  define AUR(R1,R2)			RR_(0x3E,R1,R2)
#  define AWR(R1,R2)			RR_(0x2E,R1,R2)
#  define AU(R1,D2,X2,B2)		RX_(0x7E,R1,X2,B2,D2)
#  define AW(R1,D2,X2,B2)		RX_(0x6E,R1,X2,B2,D2)
/* COMPARE */
#  define CER(R1,R2)			RR_(0x39,R1,R2)
#  define CDR(R1,R2)			RR_(0x29,R1,R2)
#  define CXR(R1,R2)			RRE_(0xB369,R1,R2)
#  define CE(R1,D2,X2,B2)		RX_(0x79,R1,X2,B2,D2)
#  define CD(R1,D2,X2,B2)		RX_(0x69,R1,X2,B2,D2)
/* CONVERT FROM FIXED */
#  define CEFR(R1,R2)			RRE_(0xB3B4,R1,R2)
#  define CDFR(R1,R2)			RRE_(0xB3B5,R1,R2)
#  define CXFR(R1,R2)			RRE_(0xB3B6,R1,R2)
#  define CEGR(R1,R2)			RRE_(0xB3C4,R1,R2)
#  define CDGR(R1,R2)			RRE_(0xB3C5,R1,R2)
#  define CXGR(R1,R2)			RRE_(0xB3C6,R1,R2)
/* CONVERT TO FIXED */
#  define CFER(R1,R2)			RRE_(0xB3B8,R1,R2)
#  define CFDR(R1,R2)			RRE_(0xB3B9,R1,R2)
#  define CFXR(R1,R2)			RRE_(0xB3BA,R1,R2)
#  define CGER(R1,R2)			RRE_(0xB3C8,R1,R2)
#  define CGDR(R1,R2)			RRE_(0xB3C9,R1,R2)
#  define CGXR(R1,R2)			RRE_(0xB3CA,R1,R2)
/* DIVIDE */
#  define DER(R1,R2)			RR_(0x3D,R1,R2)
#  define DDR(R1,R2)			RR_(0x2D,R1,R2)
#  define DXR(R1,R2)			RRE_(0xB22D,R1,R2)
#  define DE(R1,D2,X2,B2)		RX_(0x7D,R1,X2,B2,D2)
#  define DD(R1,D2,X2,B2)		RX_(0x6D,R1,X2,B2,D2)
/* HALVE */
#  define HER(R1,R2)			RR_(0x34,R1,R2)
#  define HDR(R1,R2)			RR_(0x24,R1,R2)
/* LOAD AND TEST */
#  define LTER(R1,R2)			RR_(0x32,R1,R2)
#  define LTDR(R1,R2)			RR_(0x22,R1,R2)
#  define LTXR(R1,R2)			RRE_(0xB362,R1,R2)
/* LOAD COMPLEMENT */
#  define LCER(R1,R2)			RR_(0x33,R1,R2)
#  define LCDR(R1,R2)			RR_(0x23,R1,R2)
#  define LCXR(R1,R2)			RRE_(0xB363,R1,R2)
/* LOAD FP INTEGER */
#  define FIER(R1,R2)			RRE_(0xB377,R1,R2)
#  define FIDR(R1,R2)			RRE_(0xB37F,R1,R2)
#  define FIXR(R1,R2)			RRE_(0xB367,R1,R2)
/* LOAD LENGHTENED */
#  define LDER(R1,R2)			RRE_(0xB324,R1,R2)
#  define LXDR(R1,R2)			RRE_(0xB325,R1,R2)
#  define LXER(R1,R2)			RRE_(0xB326,R1,R2)
#  define LDE(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x24)
#  define LXD(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x25)
#  define LXE(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x26)
/* LOAD NEGATIVE */
#  define LNER(R1,R2)			RR_(0x31,R1,R2)
#  define LNDR(R1,R2)			RR_(0x21,R1,R2)
#  define LNXR(R1,R2)			RRE_(0xB361,R1,R2)
/* LOAD POSITIVE */
#  define LPER(R1,R2)			RR_(0x30,R1,R2)
#  define LPDR(R1,R2)			RR_(0x20,R1,R2)
#  define LPXR(R1,R2)			RRE_(0xB360,R1,R2)
/* LOAD ROUNDED */
#  define LEDR(R1,R2)			RR_(0x35,R1,R2)
#  define LDXR(R1,R2)			RR_(0x25,R1,R2)
#  define LRER(R1,R2)			LEDR(R1,R2)
#  define LRDR(R1,R2)			LDXR(R1,R2)
#  define LRXR(R1,R2)			RRE_(0xB366,R1,R2)
/* MULTIPLY */
#  define MEER(R1,R2)			RRE_(0xB337,R1,R2)
#  define MDR(R1,R2)			RR_(0x2C,R1,R2)
#  define MXR(R1,R2)			RR_(0x26,R1,R2)
#  define MDER(R1,R2)			RR_(0x3C,R1,R2)
#  define MXDR(R1,R2)			RR_(0x27,R1,R2)
#  define MER(R1,R2)			MDER(R1,R2)
#  define MEE(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x37)
#  define MD(R1,D2,X2,B2)		RX_(0x6C,R1,X2,B2,D2)
#  define MDE(R1,D2,X2,B2)		RX_(0x7C,R1,X2,B2,D2)
#  define MXD(R1,D2,X2,B2)		RX_(0x67,R1,X2,B2,D2)
#  define ME(R1,D2,X2,B2)		MDE(R1,D2,X2,B2)
/* MULTIPLY AND ADD */
#  define MAER(R1,R3,R2)		RRF_(0xB32E,R1,0,R3,R2)
#  define MADR(R1,R3,R2)		RRF_(0xB33E,R1,0,R3,R2)
#  define MAE(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x2E)
#  define MAD(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x3E)
/* MULTIPLY AND SUBTRACT */
#  define MSER(R1,R3,R2)		RRF_(0xB32F,R1,0,R3,R2)
#  define MSDR(R1,R3,R2)		RRF_(0xB33F,R1,0,R3,R2)
#  define MSE(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x2F)
#  define MSD(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x3F)
/* SQUARE ROOT */
#  define SQER(R1,R2)			RRE_(0xB245,R1,R2)
#  define SQDR(R1,R2)			RRE_(0xB244,R1,R2)
#  define SQXR(R1,R2)			RRE_(0xB336,R1,R2)
#  define SQE(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x34)
#  define SQD(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x35)
/* SUBTRACT NORMALIZED */
#  define SER(R1,R2)			RR_(0x3B,R1,R2)
#  define SDR(R1,R2)			RR_(0x2B,R1,R2)
#  define SXR(R1,R2)			RR_(0x37,R1,R2)
#  define SE(R1,D2,X2,B2)		RX_(0x7B,R1,X2,B2,D2)
#  define SD(R1,D2,X2,B2)		RX_(0x6B,R1,X2,B2,D2)
/* SUBTRACT UNNORMALIZED */
#  define SUR(R1,R2)			RR_(0x3F,R1,R2)
#  define SWR(R1,R2)			RR_(0x2F,R1,R2)
#  define SU(R1,D2,X2,B2)		RX_(0x7F,R1,X2,B2,D2)
#  define SW(R1,D2,X2,B2)		RX_(0x6F,R1,X2,B2,D2)
/****************************************************************
 * Binary Floating Point Instructions				*
 ****************************************************************/
/* ADD */
#  define AEBR(R1,R2)			RRE_(0xB30A,R1,R2)
#  define ADBR(R1,R2)			RRE_(0xB31A,R1,R2)
#  define AXBR(R1,R2)			RRE_(0xB34A,R1,R2)
#  define AEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x0A)
#  define ADB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x1A)
/* COMPARE */
#  define CEBR(R1,R2)			RRE_(0xB309,R1,R2)
#  define CDBR(R1,R2)			RRE_(0xB319,R1,R2)
#  define CXBR(R1,R2)			RRE_(0xB349,R1,R2)
#  define CEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x09)
#  define CDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x19)
/* COMPARE AND SIGNAL */
#  define KEBR(R1,R2)			RRE_(0xB308,R1,R2)
#  define KDBR(R1,R2)			RRE_(0xB318,R1,R2)
#  define KXBR(R1,R2)			RRE_(0xB348,R1,R2)
#  define KEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x08)
#  define KDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x18)
/* CONVERT FROM FIXED */
#  define CEFBR(R1,R2)			RRE_(0xB394,R1,R2)
#  define CDFBR(R1,R2)			RRE_(0xB395,R1,R2)
#  define CXFBR(R1,R2)			RRE_(0xB396,R1,R2)
#  define CEGBR(R1,R2)			RRE_(0xB3A4,R1,R2)
#  define CDGBR(R1,R2)			RRE_(0xB3A5,R1,R2)
#  define CXGBR(R1,R2)			RRE_(0xB3A6,R1,R2)
/* CONVERT TO FIXED */
#  define CFEBR(R1,M3,R2)		RRF_(0xB398,M3,0,R1,R2)
#  define CFDBR(R1,M3,R2)		RRF_(0xB399,M3,0,R1,R2)
#  define CFXBR(R1,M3,R2)		RRF_(0xB39A,M3,0,R1,R2)
#  define CGEBR(R1,M3,R2)		RRF_(0xB3A8,M3,0,R1,R2)
#  define CGDBR(R1,M3,R2)		RRF_(0xB3A9,M3,0,R1,R2)
#  define CGXBR(R1,M3,R2)		RRF_(0xB3AA,M3,0,R1,R2)
/* DIVIDE */
#  define DEBR(R1,R2)			RRE_(0xB30D,R1,R2)
#  define DDBR(R1,R2)			RRE_(0xB31D,R1,R2)
#  define DXBR(R1,R2)			RRE_(0xB34D,R1,R2)
#  define DEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x0D)
#  define DDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x1D)
/* DIVIDE TO INTEGER */
#  define DIEBR(R1,R3,R2,M4)		RRF_(0xB353,R3,M4,R1,R2)
#  define DIDBR(R1,R3,R2,M4)		RRF_(0xB35B,R3,M4,R1,R2)
/* EXTRACT FPC */
#  define EFPC(R1)			RRE_(0xB38C,R1,0)
/* LOAD AND TEST */
#  define LTEBR(R1,R2)			RRE_(0xB302,R1,R2)
#  define LTDBR(R1,R2)			RRE_(0xB312,R1,R2)
#  define LTXBR(R1,R2)			RRE_(0xB342,R1,R2)
/* LOAD COMPLEMENT */
#  define LCEBR(R1,R2)			RRE_(0xB303,R1,R2)
#  define LCDBR(R1,R2)			RRE_(0xB313,R1,R2)
#  define LCXBR(R1,R2)			RRE_(0xB343,R1,R2)
/* LOAD FP INTEGER */
#  define FIEBR(R1,M3,R2)		RRF_(0xB357,M3,0,R1,R2)
#  define FIDBR(R1,M3,R2)		RRF_(0xB35F,M3,0,R1,R2)
#  define FIXBR(R1,M3,R2)		RRF_(0xB347,M3,0,R1,R2)
/* LOAD FPC */
#  define LFPC(D2,B2)			S_(0xB29D,B2,D2)
/* LOAD LENGTHENED */
#  define LDEBR(R1,R2)			RRE_(0xB304,R1,R2)
#  define LXDBR(R1,R2)			RRE_(0xB305,R1,R2)
#  define LXEBR(R1,R2)			RRE_(0xB306,R1,R2)
#  define LDEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x04)
#  define LXDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x05)
#  define LXEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x06)
/* LOAD NEGATIVE */
#  define LNEBR(R1,R2)			RRE_(0xB301,R1,R2)
#  define LNDBR(R1,R2)			RRE_(0xB311,R1,R2)
#  define LNXBR(R1,R2)			RRE_(0xB341,R1,R2)
/* LOAD POSITIVE */
#  define LPEBR(R1,R2)			RRE_(0xB300,R1,R2)
#  define LPDBR(R1,R2)			RRE_(0xB310,R1,R2)
#  define LPXBR(R1,R2)			RRE_(0xB340,R1,R2)
/* LOAD ROUNDED */
#  define LEDBR(R1,R2)			RRE_(0xB344,R1,R2)
#  define LDXBR(R1,R2)			RRE_(0xB345,R1,R2)
#  define LEXBR(R1,R2)			RRE_(0xB346,R1,R2)
/* MULTIPLY */
#  define MEEBR(R1,R2)			RRE_(0xB317,R1,R2)
#  define MDBR(R1,R2)			RRE_(0xB31C,R1,R2)
#  define MXBR(R1,R2)			RRE_(0xB34C,R1,R2)
#  define MDEBR(R1,R2)			RRE_(0xB30C,R1,R2)
#  define MXDBR(R1,R2)			RRE_(0xB307,R1,R2)
#  define MEEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x17)
#  define MDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x1C)
#  define MDEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x0C)
#  define MXDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x07)
/* MULTIPLY AND ADD */
#  define MAEBR(R1,R3,R2)		RRF_(0xB30E,R1,0,R3,R2)
#  define MADBR(R1,R3,R2)		RRF_(0xB31E,R1,0,R3,R2)
#  define MAEB(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x0E)
#  define MADB(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x1E)
/* MULTIPLY AND SUBTRACT */
#  define MSEBR(R1,R3,R2)		RRF_(0xB30F,R1,0,R3,R2)
#  define MSDBR(R1,R3,R2)		RRF_(0xB31F,R1,0,R3,R2)
#  define MSEB(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x0F)
#  define MSDB(R1,R3,D2,X2,B2)		RXF_(0xED,R3,X2,B2,D2,R1,0x1F)
/* SET FPC */
#  define SFPC(R1)			RRE_(0xB384,R1,0)
/* SET ROUNDING MODE */
#  define SRNM(D2,B2)			S_(0xB299,B2,D2)
/* SQUARE ROOT */
#  define SQEBR(R1,R2)			RRE_(0xB314,R1,R2)
#  define SQDBR(R1,R2)			RRE_(0xB315,R1,R2)
#  define SQXBR(R1,R2)			RRE_(0xB316,R1,R2)
/* STORE FPC */
#  define STFPC(D2,B2)			S_(0xB29C,B2,D2)
/* SUBTRACT */
#  define SEBR(R1,R2)			RRE_(0xB30B,R1,R2)
#  define SDBR(R1,R2)			RRE_(0xB31B,R1,R2)
#  define SXBR(R1,R2)			RRE_(0xB34B,R1,R2)
#  define SEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x0B)
#  define SDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x1B)
/* TEST DATA CLASS */
#  define TCEB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x10)
#  define TCDB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x11)
#  define TCXB(R1,D2,X2,B2)		RXE_(0xED,R1,X2,B2,D2,0x12)
#  define fp(code,r0,r1,i0)		_fp(_jit,jit_code_##code##i_f,r0,r1,i0)
static void _fp(jit_state_t*,jit_code_t,
		jit_int32_t,jit_int32_t,jit_float32_t*);
#  define dp(code,r0,r1,i0)		_dp(_jit,jit_code_##code##i_d,r0,r1,i0)
static void _dp(jit_state_t*,jit_code_t,
		jit_int32_t,jit_int32_t,jit_float64_t*);
#  define fr(cc,r0,r1,r2)		_fr(_jit,cc,r0,r1,r2)
static void _fr(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#  define dr(cc,r0,r1,r2)		_dr(_jit,cc,r0,r1,r2)
static void _dr(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_int32_t);
#  define fi(cc,r0,r1,i0)		_fi(_jit,cc,r0,r1,i0)
static void _fi(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_float32_t*);
#  define di(cc,r0,r1,i0)		_di(_jit,cc,r0,r1,i0)
static void _di(jit_state_t*,jit_int32_t,
		jit_int32_t,jit_int32_t,jit_float64_t*);
#  define bfr(cc,i0,r0,r1)		_bfr(_jit,cc,i0,r0,r1)
static void _bfr(jit_state_t*,jit_int32_t,
		 jit_word_t,jit_int32_t,jit_int32_t);
#  define bdr(cc,i0,r0,r1)		_bdr(_jit,cc,i0,r0,r1)
static void _bdr(jit_state_t*,jit_int32_t,
		 jit_word_t,jit_int32_t,jit_int32_t);
#  define bfr_p(cc,i0,r0,r1)		_bfr_p(_jit,cc,i0,r0,r1)
static jit_word_t _bfr_p(jit_state_t*,jit_int32_t,
			 jit_word_t,jit_int32_t,jit_int32_t);
#  define bdr_p(cc,i0,r0,r1)		_bdr_p(_jit,cc,i0,r0,r1)
static jit_word_t _bdr_p(jit_state_t*,jit_int32_t,
			 jit_word_t,jit_int32_t,jit_int32_t);
#  define bfi(cc,i0,r0,i1)		_bfi(_jit,cc,i0,r0,i1)
static void _bfi(jit_state_t*,jit_int32_t,
		 jit_word_t,jit_int32_t,jit_float32_t*);
#  define bdi(cc,i0,r0,i1)		_bdi(_jit,cc,i0,r0,i1)
static void _bdi(jit_state_t*,jit_int32_t,
		 jit_word_t,jit_int32_t,jit_float64_t*);
#  define bfi_p(cc,i0,r0,i1)		_bfi_p(_jit,cc,i0,r0,i1)
static jit_word_t _bfi_p(jit_state_t*,jit_int32_t,
			 jit_word_t,jit_int32_t,jit_float32_t*);
#  define bdi_p(cc,i0,r0,i1)		_bdi_p(_jit,cc,i0,r0,i1)
static jit_word_t _bdi_p(jit_state_t*,jit_int32_t,
			 jit_word_t,jit_int32_t,jit_float64_t*);
#  define buneqr(db,i0,r0,r1)		_buneqr(_jit,db,i0,r0,r1)
static jit_word_t _buneqr(jit_state_t*,jit_int32_t,
			  jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi(db,i0,r0,i1)		_buneqi(_jit,db,i0,r0,(jit_word_t)i1)
static jit_word_t _buneqi(jit_state_t*,jit_int32_t,
			  jit_word_t,jit_int32_t,jit_word_t);
#  define bltgtr(db,i0,r0,r1)		_bltgtr(_jit,db,i0,r0,r1)
static jit_word_t _bltgtr(jit_state_t*,jit_int32_t,
			  jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti(db,i0,r0,i1)		_bltgti(_jit,db,i0,r0,(jit_word_t)i1)
static jit_word_t _bltgti(jit_state_t*,jit_int32_t,
			  jit_word_t,jit_int32_t,jit_word_t);
#  define movr_f(r0,r1)			_movr_f(_jit,r0,r1)
static void _movr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_f(r0,i0)			_movi_f(_jit,r0,i0)
static void _movi_f(jit_state_t*,jit_int32_t,jit_float32_t*);
#  define movr_d(r0,r1)			_movr_d(_jit,r0,r1)
static void _movr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_d(r0,i0)			_movi_d(_jit,r0,i0)
static void _movi_d(jit_state_t*,jit_int32_t,jit_float64_t*);
#  define absr_f(r0,r1)			LPEBR(r0,r1)
#  define absr_d(r0,r1)			LPDBR(r0,r1)
#  define negr_f(r0,r1)			LCEBR(r0,r1)
#  define negr_d(r0,r1)			LCDBR(r0,r1)
#  define sqrtr_f(r0,r1)		SQEBR(r0,r1)
#  define sqrtr_d(r0,r1)		SQDBR(r0,r1)
#  define truncr_f_i(r0,r1)		CFEBR(r0,RND_ZERO,r1)
#  define truncr_d_i(r0,r1)		CFDBR(r0,RND_ZERO,r1)
#  if __WORDSIZE == 64
#    define truncr_f_l(r0,r1)		CGEBR(r0,RND_ZERO,r1)
#    define truncr_d_l(r0,r1)		CGDBR(r0,RND_ZERO,r1)
#  endif
#  if __WORDSIZE == 32
#    define extr_f(r0,r1)		CEFBR(r0,r1)
#    define extr_d(r0,r1)		CDFBR(r0,r1)
#  else
#    define extr_f(r0,r1)		CEGBR(r0,r1)
#    define extr_d(r0,r1)		CDGBR(r0,r1)
#  endif
#  define extr_d_f(r0,r1)		LEDBR(r0,r1)
#  define extr_f_d(r0,r1)		LDEBR(r0,r1)
#  define addr_f(r0,r1,r2)		_addr_f(_jit,r0,r1,r2)
static void _addr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addi_f(r0,r1,i0)		fp(add,r0,r1,i0)
#  define addr_d(r0,r1,r2)		_addr_d(_jit,r0,r1,r2)
static void _addr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addi_d(r0,r1,i0)		dp(add,r0,r1,i0)
#  define subr_f(r0,r1,r2)		_subr_f(_jit,r0,r1,r2)
static void _subr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subi_f(r0,r1,i0)		fp(sub,r0,r1,i0)
#  define subr_d(r0,r1,r2)		_subr_d(_jit,r0,r1,r2)
static void _subr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subi_d(r0,r1,i0)		dp(sub,r0,r1,i0)
#  define rsbr_f(r0,r1,r2)		subr_f(r0,r2,r1)
#  define rsbi_f(r0,r1,i0)		fp(rsb,r0,r1,i0)
#  define rsbr_d(r0,r1,r2)		subr_d(r0,r2,r1)
#  define rsbi_d(r0,r1,i0)		dp(rsb,r0,r1,i0)
#  define mulr_f(r0,r1,r2)		_mulr_f(_jit,r0,r1,r2)
static void _mulr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define muli_f(r0,r1,i0)		fp(mul,r0,r1,i0)
#  define mulr_d(r0,r1,r2)		_mulr_d(_jit,r0,r1,r2)
static void _mulr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define muli_d(r0,r1,i0)		dp(mul,r0,r1,i0)
#  define divr_f(r0,r1,r2)		_divr_f(_jit,r0,r1,r2)
static void _divr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi_f(r0,r1,i0)		fp(div,r0,r1,i0)
#  define divr_d(r0,r1,r2)		_divr_d(_jit,r0,r1,r2)
static void _divr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi_d(r0,r1,i0)		dp(div,r0,r1,i0)
#  define ldr_f(r0,r1)			LE(r0,0,0,r1)
#  define ldr_d(r0,r1)			LD(r0,0,0,r1)
#  define ldi_f(r0,i0)			_ldi_f(_jit,r0,i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldi_d(r0,i0)			_ldi_d(_jit,r0,i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_f(r0,r1,r2)		_ldxr_f(_jit,r0,r1,r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxr_d(r0,r1,r2)		_ldxr_d(_jit,r0,r1,r2)
static void _ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_f(r0,r1,i0)		_ldxi_f(_jit,r0,r1,i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxi_d(r0,r1,i0)		_ldxi_d(_jit,r0,r1,i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_f(r0,r1)			STE(r1,0,0,r0)
#  define str_d(r0,r1)			STD(r1,0,0,r0)
#  define sti_f(i0,r0)			_sti_f(_jit,i0,r0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  define sti_d(i0,r0)			_sti_d(_jit,i0,r0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_f(r0,r1,r2)		_stxr_f(_jit,r0,r1,r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxr_d(r0,r1,r2)		_stxr_d(_jit,r0,r1,r2)
static void _stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_f(i0,r0,r1)		_stxi_f(_jit,i0,r0,r1)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxi_d(i0,r0,r1)		_stxi_d(_jit,i0,r0,r1)
static void _stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define ltr_f(r0,r1,r2)		fr(CC_L,r0,r1,r2)
#  define ltr_d(r0,r1,r2)		dr(CC_L,r0,r1,r2)
#  define lti_f(r0,r1,i0)		fi(CC_L,r0,r1,i0)
#  define lti_d(r0,r1,i0)		di(CC_L,r0,r1,i0)
#  define ler_f(r0,r1,r2)		fr(CC_LE,r0,r1,r2)
#  define ler_d(r0,r1,r2)		dr(CC_LE,r0,r1,r2)
#  define lei_f(r0,r1,i0)		fi(CC_LE,r0,r1,i0)
#  define lei_d(r0,r1,i0)		di(CC_LE,r0,r1,i0)
#  define eqr_f(r0,r1,r2)		fr(CC_E,r0,r1,r2)
#  define eqr_d(r0,r1,r2)		dr(CC_E,r0,r1,r2)
#  define eqi_f(r0,r1,i0)		fi(CC_E,r0,r1,i0)
#  define eqi_d(r0,r1,i0)		di(CC_E,r0,r1,i0)
#  define ger_f(r0,r1,r2)		fr(CC_HE,r0,r1,r2)
#  define ger_d(r0,r1,r2)		dr(CC_HE,r0,r1,r2)
#  define gei_f(r0,r1,i0)		fi(CC_HE,r0,r1,i0)
#  define gei_d(r0,r1,i0)		di(CC_HE,r0,r1,i0)
#  define gtr_f(r0,r1,r2)		fr(CC_H,r0,r1,r2)
#  define gtr_d(r0,r1,r2)		dr(CC_H,r0,r1,r2)
#  define gti_f(r0,r1,i0)		fi(CC_H,r0,r1,i0)
#  define gti_d(r0,r1,i0)		di(CC_H,r0,r1,i0)
#  define ner_f(r0,r1,r2)		fr(CC_NE,r0,r1,r2)
#  define ner_d(r0,r1,r2)		dr(CC_NE,r0,r1,r2)
#  define nei_f(r0,r1,i0)		fi(CC_NE,r0,r1,i0)
#  define nei_d(r0,r1,i0)		di(CC_NE,r0,r1,i0)
#  define unltr_f(r0,r1,r2)		fr(CC_NHE,r0,r1,r2)
#  define unltr_d(r0,r1,r2)		dr(CC_NHE,r0,r1,r2)
#  define unlti_f(r0,r1,i0)		fi(CC_NHE,r0,r1,i0)
#  define unlti_d(r0,r1,i0)		di(CC_NHE,r0,r1,i0)
#  define unler_f(r0,r1,r2)		fr(CC_NH,r0,r1,r2)
#  define unler_d(r0,r1,r2)		dr(CC_NH,r0,r1,r2)
#  define unlei_f(r0,r1,i0)		fi(CC_NH,r0,r1,i0)
#  define unlei_d(r0,r1,i0)		di(CC_NH,r0,r1,i0)
#  define uneqr_f(r0,r1,r2)		_uneqr_f(_jit,r0,r1,r2)
static void _uneqr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define uneqr_d(r0,r1,r2)		_uneqr_d(_jit,r0,r1,r2)
static void _uneqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define uneqi_f(r0,r1,i0)		fp(uneq,r0,r1,i0)
#  define uneqi_d(r0,r1,i0)		dp(uneq,r0,r1,i0)
#  define unger_f(r0,r1,r2)		fr(CC_NL,r0,r1,r2)
#  define unger_d(r0,r1,r2)		dr(CC_NL,r0,r1,r2)
#  define ungei_f(r0,r1,i0)		fi(CC_NL,r0,r1,i0)
#  define ungei_d(r0,r1,i0)		di(CC_NL,r0,r1,i0)
#  define ungtr_f(r0,r1,r2)		fr(CC_NLE,r0,r1,r2)
#  define ungtr_d(r0,r1,r2)		dr(CC_NLE,r0,r1,r2)
#  define ungti_f(r0,r1,i0)		fi(CC_NLE,r0,r1,i0)
#  define ungti_d(r0,r1,i0)		di(CC_NLE,r0,r1,i0)
#  define ltgtr_f(r0,r1,r2)		_ltgtr_f(_jit,r0,r1,r2)
static void _ltgtr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ltgtr_d(r0,r1,r2)		_ltgtr_d(_jit,r0,r1,r2)
static void _ltgtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ltgti_f(r0,r1,i0)		fp(ltgt,r0,r1,i0)
#  define ltgti_d(r0,r1,i0)		dp(ltgt,r0,r1,i0)
#  define ordr_f(r0,r1,r2)		fr(CC_NO,r0,r1,r2)
#  define ordr_d(r0,r1,r2)		dr(CC_NO,r0,r1,r2)
#  define ordi_f(r0,r1,i0)		fi(CC_NO,r0,r1,i0)
#  define ordi_d(r0,r1,i0)		di(CC_NO,r0,r1,i0)
#  define unordr_f(r0,r1,r2)		fr(CC_O,r0,r1,r2)
#  define unordr_d(r0,r1,r2)		dr(CC_O,r0,r1,r2)
#  define unordi_f(r0,r1,i0)		fi(CC_O,r0,r1,i0)
#  define unordi_d(r0,r1,i0)		di(CC_O,r0,r1,i0)
#  define bltr_f(i0,r0,r1)		bfr(CC_L,i0,r0,r1)
#  define bltr_d(i0,r0,r1)		bdr(CC_L,i0,r0,r1)
#  define blti_f(i0,r0,i1)		bfi(CC_L,i0,r0,i1)
#  define blti_d(i0,r0,i1)		bdi(CC_L,i0,r0,i1)
#  define bltr_f_p(i0,r0,r1)		bfr_p(CC_L,i0,r0,r1)
#  define bltr_d_p(i0,r0,r1)		bdr_p(CC_L,i0,r0,r1)
#  define blti_f_p(i0,r0,i1)		bfi_p(CC_L,i0,r0,i1)
#  define blti_d_p(i0,r0,i1)		bdi_p(CC_L,i0,r0,i1)
#  define bler_f(i0,r0,r1)		bfr(CC_LE,i0,r0,r1)
#  define bler_d(i0,r0,r1)		bdr(CC_LE,i0,r0,r1)
#  define blei_f(i0,r0,i1)		bfi(CC_LE,i0,r0,i1)
#  define blei_d(i0,r0,i1)		bdi(CC_LE,i0,r0,i1)
#  define bler_f_p(i0,r0,r1)		bfr_p(CC_LE,i0,r0,r1)
#  define bler_d_p(i0,r0,r1)		bdr_p(CC_LE,i0,r0,r1)
#  define blei_f_p(i0,r0,i1)		bfi_p(CC_LE,i0,r0,i1)
#  define blei_d_p(i0,r0,i1)		bdi_p(CC_LE,i0,r0,i1)
#  define beqr_f(i0,r0,r1)		bfr(CC_E,i0,r0,r1)
#  define beqr_d(i0,r0,r1)		bdr(CC_E,i0,r0,r1)
#  define beqi_f(i0,r0,i1)		bfi(CC_E,i0,r0,i1)
#  define beqi_d(i0,r0,i1)		bdi(CC_E,i0,r0,i1)
#  define beqr_f_p(i0,r0,r1)		bfr_p(CC_E,i0,r0,r1)
#  define beqr_d_p(i0,r0,r1)		bdr_p(CC_E,i0,r0,r1)
#  define beqi_f_p(i0,r0,i1)		bfi_p(CC_E,i0,r0,i1)
#  define beqi_d_p(i0,r0,i1)		bdi_p(CC_E,i0,r0,i1)
#  define bger_f(i0,r0,r1)		bfr(CC_HE,i0,r0,r1)
#  define bger_d(i0,r0,r1)		bdr(CC_HE,i0,r0,r1)
#  define bgei_f(i0,r0,i1)		bfi(CC_HE,i0,r0,i1)
#  define bgei_d(i0,r0,i1)		bdi(CC_HE,i0,r0,i1)
#  define bger_f_p(i0,r0,r1)		bfr_p(CC_HE,i0,r0,r1)
#  define bger_d_p(i0,r0,r1)		bdr_p(CC_HE,i0,r0,r1)
#  define bgei_f_p(i0,r0,i1)		bfi_p(CC_HE,i0,r0,i1)
#  define bgei_d_p(i0,r0,i1)		bdi_p(CC_HE,i0,r0,i1)
#  define bgtr_f(i0,r0,r1)		bfr(CC_H,i0,r0,r1)
#  define bgtr_d(i0,r0,r1)		bdr(CC_H,i0,r0,r1)
#  define bgti_f(i0,r0,i1)		bfi(CC_H,i0,r0,i1)
#  define bgti_d(i0,r0,i1)		bdi(CC_H,i0,r0,i1)
#  define bgtr_f_p(i0,r0,r1)		bfr_p(CC_H,i0,r0,r1)
#  define bgtr_d_p(i0,r0,r1)		bdr_p(CC_H,i0,r0,r1)
#  define bgti_f_p(i0,r0,i1)		bfi_p(CC_H,i0,r0,i1)
#  define bgti_d_p(i0,r0,i1)		bdi_p(CC_H,i0,r0,i1)
#  define bner_f(i0,r0,r1)		bfr(CC_NE,i0,r0,r1)
#  define bner_d(i0,r0,r1)		bdr(CC_NE,i0,r0,r1)
#  define bnei_f(i0,r0,i1)		bfi(CC_NE,i0,r0,i1)
#  define bnei_d(i0,r0,i1)		bdi(CC_NE,i0,r0,i1)
#  define bner_f_p(i0,r0,r1)		bfr_p(CC_NE,i0,r0,r1)
#  define bner_d_p(i0,r0,r1)		bdr_p(CC_NE,i0,r0,r1)
#  define bnei_f_p(i0,r0,i1)		bfi_p(CC_NE,i0,r0,i1)
#  define bnei_d_p(i0,r0,i1)		bdi_p(CC_NE,i0,r0,i1)
#  define bunltr_f(i0,r0,r1)		bfr(CC_NHE,i0,r0,r1)
#  define bunltr_d(i0,r0,r1)		bdr(CC_NHE,i0,r0,r1)
#  define bunlti_f(i0,r0,i1)		bfi(CC_NHE,i0,r0,i1)
#  define bunlti_d(i0,r0,i1)		bdi(CC_NHE,i0,r0,i1)
#  define bunltr_f_p(i0,r0,r1)		bfr_p(CC_NHE,i0,r0,r1)
#  define bunltr_d_p(i0,r0,r1)		bdr_p(CC_NHE,i0,r0,r1)
#  define bunlti_f_p(i0,r0,i1)		bfi_p(CC_NHE,i0,r0,i1)
#  define bunlti_d_p(i0,r0,i1)		bdi_p(CC_NHE,i0,r0,i1)
#  define bunler_f(i0,r0,r1)		bfr(CC_NH,i0,r0,r1)
#  define bunler_d(i0,r0,r1)		bdr(CC_NH,i0,r0,r1)
#  define bunlei_f(i0,r0,i1)		bfi(CC_NH,i0,r0,i1)
#  define bunlei_d(i0,r0,i1)		bdi(CC_NH,i0,r0,i1)
#  define bunler_f_p(i0,r0,r1)		bfr_p(CC_NH,i0,r0,r1)
#  define bunler_d_p(i0,r0,r1)		bdr_p(CC_NH,i0,r0,r1)
#  define bunlei_f_p(i0,r0,i1)		bfi_p(CC_NH,i0,r0,i1)
#  define bunlei_d_p(i0,r0,i1)		bdi_p(CC_NH,i0,r0,i1)
#  define buneqr_f(i0,r0,r1)		buneqr(0,i0,r0,r1)
#  define buneqr_d(i0,r0,r1)		buneqr(1,i0,r0,r1)
#  define buneqi_f(i0,r0,i1)		buneqi(0,i0,r0,i1)
#  define buneqi_d(i0,r0,i1)		buneqi(1,i0,r0,i1)
#  define buneqr_f_p(i0,r0,r1)		buneqr(0,i0,r0,r1)
#  define buneqr_d_p(i0,r0,r1)		buneqr(1,i0,r0,r1)
#  define buneqi_f_p(i0,r0,i1)		buneqi(0,i0,r0,i1)
#  define buneqi_d_p(i0,r0,i1)		buneqi(1,i0,r0,i1)
#  define bunger_f(i0,r0,r1)		bfr(CC_NL,i0,r0,r1)
#  define bunger_d(i0,r0,r1)		bdr(CC_NL,i0,r0,r1)
#  define bungei_f(i0,r0,i1)		bfi(CC_NL,i0,r0,i1)
#  define bungei_d(i0,r0,i1)		bdi(CC_NL,i0,r0,i1)
#  define bunger_f_p(i0,r0,r1)		bfr_p(CC_NL,i0,r0,r1)
#  define bunger_d_p(i0,r0,r1)		bdr_p(CC_NL,i0,r0,r1)
#  define bungei_f_p(i0,r0,i1)		bfi_p(CC_NL,i0,r0,i1)
#  define bungei_d_p(i0,r0,i1)		bdi_p(CC_NL,i0,r0,i1)
#  define bungtr_f(i0,r0,r1)		bfr(CC_NLE,i0,r0,r1)
#  define bungtr_d(i0,r0,r1)		bdr(CC_NLE,i0,r0,r1)
#  define bungti_f(i0,r0,i1)		bfi(CC_NLE,i0,r0,i1)
#  define bungti_d(i0,r0,i1)		bdi(CC_NLE,i0,r0,i1)
#  define bungtr_f_p(i0,r0,r1)		bfr_p(CC_NLE,i0,r0,r1)
#  define bungtr_d_p(i0,r0,r1)		bdr_p(CC_NLE,i0,r0,r1)
#  define bungti_f_p(i0,r0,i1)		bfi_p(CC_NLE,i0,r0,i1)
#  define bungti_d_p(i0,r0,i1)		bdi_p(CC_NLE,i0,r0,i1)
#  define bltgtr_f(i0,r0,r1)		bltgtr(0,i0,r0,r1)
#  define bltgtr_d(i0,r0,r1)		bltgtr(1,i0,r0,r1)
#  define bltgti_f(i0,r0,i1)		bltgti(0,i0,r0,i1)
#  define bltgti_d(i0,r0,i1)		bltgti(1,i0,r0,i1)
#  define bltgtr_f_p(i0,r0,r1)		bltgtr(0,i0,r0,r1)
#  define bltgtr_d_p(i0,r0,r1)		bltgtr(1,i0,r0,r1)
#  define bltgti_f_p(i0,r0,i1)		bltgti(0,i0,r0,i1)
#  define bltgti_d_p(i0,r0,i1)		bltgti(1,i0,r0,i1)
#  define bordr_f(i0,r0,r1)		bfr(CC_NO,i0,r0,r1)
#  define bordr_d(i0,r0,r1)		bdr(CC_NO,i0,r0,r1)
#  define bordi_f(i0,r0,i1)		bfi(CC_NO,i0,r0,i1)
#  define bordi_d(i0,r0,i1)		bdi(CC_NO,i0,r0,i1)
#  define bordr_f_p(i0,r0,r1)		bfr_p(CC_NO,i0,r0,r1)
#  define bordr_d_p(i0,r0,r1)		bdr_p(CC_NO,i0,r0,r1)
#  define bordi_f_p(i0,r0,i1)		bfi_p(CC_NO,i0,r0,i1)
#  define bordi_d_p(i0,r0,i1)		bdi_p(CC_NO,i0,r0,i1)
#  define bunordr_f(i0,r0,r1)		bfr(CC_O,i0,r0,r1)
#  define bunordr_d(i0,r0,r1)		bdr(CC_O,i0,r0,r1)
#  define bunordi_f(i0,r0,i1)		bfi(CC_O,i0,r0,i1)
#  define bunordi_d(i0,r0,i1)		bdi(CC_O,i0,r0,i1)
#  define bunordr_f_p(i0,r0,r1)		bfr_p(CC_O,i0,r0,r1)
#  define bunordr_d_p(i0,r0,r1)		bdr_p(CC_O,i0,r0,r1)
#  define bunordi_f_p(i0,r0,i1)		bfi_p(CC_O,i0,r0,i1)
#  define bunordi_d_p(i0,r0,i1)		bdi_p(CC_O,i0,r0,i1)
#  define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
static void
_fp(jit_state_t *_jit, jit_code_t code,
    jit_int32_t r0, jit_int32_t r1, jit_float32_t *i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    movi_f(rn(reg), i0);
    switch (code) {
	case jit_code_addi_f:	addr_f(r0, r1, rn(reg));	break;
	case jit_code_subi_f:	subr_f(r0, r1, rn(reg));	break;
	case jit_code_rsbi_f:	rsbr_f(r0, r1, rn(reg));	break;
	case jit_code_muli_f:	mulr_f(r0, r1, rn(reg));	break;
	case jit_code_divi_f:	divr_f(r0, r1, rn(reg));	break;
	case jit_code_uneqi_f:	uneqr_f(r0, r1, rn(reg));	break;
	case jit_code_ltgti_f:	ltgtr_f(r0, r1, rn(reg));	break;
	default:		abort();
    }
    jit_unget_reg(reg);
}

static void
_dp(jit_state_t *_jit, jit_code_t code,
    jit_int32_t r0, jit_int32_t r1, jit_float64_t *i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    movi_d(rn(reg), i0);
    switch (code) {
	case jit_code_addi_d:	addr_d(r0, r1, rn(reg));	break;
	case jit_code_subi_d:	subr_d(r0, r1, rn(reg));	break;
	case jit_code_rsbi_d:	rsbr_d(r0, r1, rn(reg));	break;
	case jit_code_muli_d:	mulr_d(r0, r1, rn(reg));	break;
	case jit_code_divi_d:	divr_d(r0, r1, rn(reg));	break;
	case jit_code_uneqi_d:	uneqr_d(r0, r1, rn(reg));	break;
	case jit_code_ltgti_d:	ltgtr_d(r0, r1, rn(reg));	break;
	default:		abort();
    }
    jit_unget_reg(reg);
}

static void
_fr(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    LGHI(r0, 1);
    CEBR(r1, r2);
    w = _jit->pc.w;
    BRC(cc, 0);
    LGHI(r0, 0);
    patch_at(w, _jit->pc.w);
}

static void
_dr(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    LGHI(r0, 1);
    CDBR(r1, r2);
    w = _jit->pc.w;
    BRC(cc, 0);
    LGHI(r0, 0);
    patch_at(w, _jit->pc.w);
}

static void
_fi(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_float32_t *i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi_f(rn(reg), i0);
    fr(cc, r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_di(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_float64_t *i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi_d(rn(reg), i0);
    dr(cc, r0, r1, rn(reg));
    jit_unget_reg(reg);
}


static void
_bfr(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d;
    CEBR(r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(cc, x16(d));
    else {
	assert(s32_p(d));
	BRCL(cc, d);
    }
}

static void
_bdr(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		d;
    CDBR(r0, r1);
    d = (i0 - _jit->pc.w) >> 1;
    if (s16_p(d))
	BRC(cc, x16(d));
    else {
	assert(s32_p(d));
	BRCL(cc, d);
    }
}

static jit_word_t
_bfr_p(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CEBR(r0, r1);
    w = _jit->pc.w;
    BRCL(cc, 0);
    return (w);
}

static jit_word_t
_bdr_p(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CDBR(r0, r1);
    w = _jit->pc.w;
    BRCL(cc, 0);
    return (w);
}

static void
_bfi(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_float32_t *i1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi_f(rn(reg), i1);
    bfr(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_bdi(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_float64_t *i1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi_d(rn(reg), i1);
    bdr(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
}

static jit_word_t
_bfi_p(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_float32_t *i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi_f(rn(reg), i1);
    w = bfr_p(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bdi_p(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_float64_t *i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi_d(rn(reg), i1);
    w = bdr_p(cc, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_buneqr(jit_state_t *_jit, jit_int32_t db,
	jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		unord, ne, w;
    if (db)	CDBR(r0, r1);
    else	CEBR(r0, r1);
    unord = _jit->pc.w;
    BRC(CC_O, 0);			/* unord satisfies condition */
    ne = _jit->pc.w;
    BRC(CC_NE, 0);			/* ne does not satisfy condition */
    patch_at(unord, _jit->pc.w);
    w = _jit->pc.w;
    BRCL(CC_AL, (i0 - _jit->pc.w) >> 1);
    patch_at(ne, _jit->pc.w);
    return (w);
}

static jit_word_t
_buneqi(jit_state_t *_jit, jit_int32_t db,
	jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    if (db)
	movi_d(rn(reg), (jit_float64_t *)i1);
    else
	movi_f(rn(reg), (jit_float32_t *)i1);
    w = buneqr(db, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bltgtr(jit_state_t *_jit, jit_int32_t db,
	jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		unord, eq, w;
    if (db)	CDBR(r0, r1);
    else	CEBR(r0, r1);
    unord = _jit->pc.w;
    BRC(CC_O, 0);			/* unord does not satisfy condition */
    eq = _jit->pc.w;
    BRC(CC_E, 0);			/* eq does not satisfy condition */
    w = _jit->pc.w;
    BRCL(CC_AL, (i0 - _jit->pc.w) >> 1);
    patch_at(unord, _jit->pc.w);
    patch_at(eq, _jit->pc.w);
    return (w);
}

static jit_word_t
_bltgti(jit_state_t *_jit, jit_int32_t db,
	jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    if (db)
	movi_d(rn(reg), (jit_float64_t *)i1);
    else
	movi_f(rn(reg), (jit_float32_t *)i1);
    w = bltgtr(db, i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_movr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	LER(r0, r1);
}

static void
_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t *i0)
{
    union {
	jit_int32_t	 i;
	jit_float32_t	 f;
    } data;
    jit_int32_t		 reg;

    if (*(jit_int32_t *)i0 == 0)
	LZER(r0);
    else if (_jitc->no_data) {
	data.f = *i0;
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), data.i & 0xffffffff);
	stxi_i(-4, _FP_REGNO, rn(reg));
	jit_unget_reg_but_zero(reg);
	ldxi_f(r0, _FP_REGNO, -4);
    }
    else
	ldi_f(r0, (jit_word_t)i0);
}

static void
_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	LDR(r0, r1);
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
#if __WORDSIZE == 32
	jit_int32_t	 i[2];
#else
	jit_int64_t	 l;
#endif
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;

    if (*(jit_int64_t *)i0 == 0)
	LZDR(r0);
    else if (_jitc->no_data) {
	data.d = *i0;
	reg = jit_get_reg_but_zero(0);
#if __WORDSIZE == 32
	movi(rn(reg), data.i[0]);
	stxi_i(-8, _FP_REGNO, rn(reg));
	movi(rn(reg), data.i[1]);
	stxi_i(-4, _FP_REGNO, rn(reg));
#else
	movi(rn(reg), data.l);
	stxi_l(-8, _FP_REGNO, rn(reg));
#endif
	jit_unget_reg_but_zero(reg);
	ldxi_d(r0, _FP_REGNO, -8);
    }
    else
	ldi_d(r0, (jit_word_t)i0);
}

static void
_addr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	AEBR(r0, r1);
    else {
	movr_f(r0, r1);
	AEBR(r0, r2);
    }
}

static void
_addr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	ADBR(r0, r1);
    else {
	movr_d(r0, r1);
	ADBR(r0, r2);
    }
}

static void
_subr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr);
	movr_f(rn(reg), r2);
	movr_f(r0, r1);
	SEBR(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr_f(r0, r1);
	SEBR(r0, r2);
    }
}

static void
_subr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr);
	movr_d(rn(reg), r2);
	movr_d(r0, r1);
	SDBR(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr_d(r0, r1);
	SDBR(r0, r2);
    }
}

static void
_mulr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	MEEBR(r0, r1);
    else {
	movr_f(r0, r1);
	MEEBR(r0, r2);
    }
}

static void
_mulr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	MDBR(r0, r1);
    else {
	movr_d(r0, r1);
	MDBR(r0, r2);
    }
}

static void
_divr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr);
	movr_f(rn(reg), r2);
	movr_f(r0, r1);
	DEBR(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr_f(r0, r1);
	DEBR(r0, r2);
    }
}

static void
_divr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2) {
	reg = jit_get_reg(jit_class_fpr);
	movr_d(rn(reg), r2);
	movr_d(r0, r1);
	DDBR(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr_d(r0, r1);
	DDBR(r0, r2);
    }
}

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    ldr_f(r0, rn(reg));
    jit_unget_reg_but_zero(reg);
}

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    ldr_d(r0, rn(reg));
    jit_unget_reg_but_zero(reg);
}

static void
_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r1);
    addr(rn(reg), rn(reg), r2);
    ldr_f(r0, rn(reg));
    jit_unget_reg_but_zero(reg);
}

static void
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r1);
    addr(rn(reg), rn(reg), r2);
    ldr_d(r0, rn(reg));
    jit_unget_reg_but_zero(reg);
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (u12_p(i0))
	LE(r0, i0, 0, r1);
    else if (s20_p(i0))
	LEY(r0, x20(i0), 0, r1);
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_f(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (u12_p(i0))
	LD(r0, i0, 0, r1);
    else if (s20_p(i0))
	LDY(r0, x20(i0), 0, r1);
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r1);
	ldr_d(r0, rn(reg));
	jit_unget_reg_but_zero(reg);
    }
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    str_f(rn(reg), r0);
    jit_unget_reg_but_zero(reg);
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movi(rn(reg), i0);
    str_d(rn(reg), r0);
    jit_unget_reg_but_zero(reg);
}

static void
_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r0);
    addr(rn(reg), rn(reg), r1);
    str_f(rn(reg), r2);
    jit_unget_reg_but_zero(reg);
}

static void
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg_but_zero(0);
    movr(rn(reg), r0);
    addr(rn(reg), rn(reg), r1);
    str_d(rn(reg), r2);
    jit_unget_reg_but_zero(reg);
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (u12_p(i0))
	STE(r1, i0, 0, r0);
    else if (s20_p(i0))
	STEY(r1, x20(i0), 0, r0);
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r0);
	str_f(rn(reg), r1);
	jit_unget_reg_but_zero(reg);
    }
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (u12_p(i0))
	STD(r1, i0, 0, r0);
    else if (s20_p(i0))
	STDY(r1, x20(i0), 0, r0);
    else {
	reg = jit_get_reg_but_zero(0);
	movi(rn(reg), i0);
	addr(rn(reg), rn(reg), r0);
	str_d(rn(reg), r1);
	jit_unget_reg_but_zero(reg);
    }
}

static void
_uneqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		unord, eq;
    movi(r0, 1);			/* set to one */
    CEBR(r1, r2);
    unord = _jit->pc.w;			/* keep set to one if unord */
    BRC(CC_O, 0);
    eq = _jit->pc.w;
    BRC(CC_E, 0);			/* keep set to one if eq */
    movi(r0, 0);			/* set to zero */
    patch_at(unord, _jit->pc.w);
    patch_at(eq, _jit->pc.w);
}

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		unord, eq;
    movi(r0, 1);			/* set to one */
    CDBR(r1, r2);
    unord = _jit->pc.w;			/* keep set to one if unord */
    BRC(CC_O, 0);
    eq = _jit->pc.w;
    BRC(CC_E, 0);			/* keep set to one if eq */
    movi(r0, 0);			/* set to zero */
    patch_at(unord, _jit->pc.w);
    patch_at(eq, _jit->pc.w);
}

static void
_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		unord, eq;
    movi(r0, 0);			/* set to zero */
    CEBR(r1, r2);
    unord = _jit->pc.w;			/* keep set to zero if unord */
    BRC(CC_O, 0);
    eq = _jit->pc.w;
    BRC(CC_E, 0);			/* keep set to zero if eq */
    movi(r0, 1);			/* set to one */
    patch_at(unord, _jit->pc.w);
    patch_at(eq, _jit->pc.w);
}

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		unord, eq;
    movi(r0, 0);			/* set to zero */
    CDBR(r1, r2);
    unord = _jit->pc.w;			/* keep set to zero if unord */
    BRC(CC_O, 0);
    eq = _jit->pc.w;
    BRC(CC_E, 0);			/* keep set to zero if eq */
    movi(r0, 1);			/* set to one */
    patch_at(unord, _jit->pc.w);
    patch_at(eq, _jit->pc.w);
}

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		rg0;
    jit_int32_t		rg1;
    jit_int32_t		rg2;
    jit_word_t		ge_code;
    jit_word_t		lt_code;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg_but_zero(jit_class_gpr);
    rg1 = jit_get_reg_but_zero(jit_class_gpr);

    /* Load the fp offset in save area in the first temporary. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, fpoff));

    /* Jump over if there are no remaining arguments in the save area. */
    ge_code = bgei_p(_jit->pc.w, rn(rg0), NUM_FLOAT_REG_ARGS);

    /* Load the save area pointer in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, save));

    /* Scale offset. */
    rg2 = jit_get_reg_but_zero(0);
    lshi(rn(rg2), rn(rg0), 3);
    /* Add offset to saved area */
    addi(rn(rg2), rn(rg2), 16 * sizeof(jit_word_t));

    /* Load the vararg argument in the first argument. */
    ldxr_d(r0, rn(rg1), rn(rg2));
    jit_unget_reg_but_zero(rg2);

    /* Update the fp offset. */
    addi(rn(rg0), rn(rg0), 1);
    stxi(offsetof(jit_va_list_t, fpoff), r1, rn(rg0));

    /* Will only need one temporary register below. */
    jit_unget_reg_but_zero(rg1);

    /* Jump over overflow code. */
    lt_code = jmpi_p(_jit->pc.w);

    /* Where to land if argument is in overflow area. */
    patch_at(ge_code, _jit->pc.w);

    /* Load overflow pointer. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, over));

    /* Load argument. */
    ldr_d(r0, rn(rg0));

    /* Update overflow pointer. */
    addi(rn(rg0), rn(rg0), sizeof(jit_float64_t));
    stxi(offsetof(jit_va_list_t, over), r1, rn(rg0));

    /* Where to land if argument is in save area. */
    patch_at(lt_code, _jit->pc.w);

    jit_unget_reg_but_zero(rg0);
}
#endif
