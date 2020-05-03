/*
 * Copyright (C) 2014-2019  Free Software Foundation, Inc.
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
#  define _u2(v)			((v) & 0x3)
#  define _s2_p(v)			((v) >= -0x2 && (v) <= 0x1)
#  define _u2_p(v)			((v) >= 0 && (v) <= 0x3)
#  define _u5(v)			((v) & 0x1f)
#  define _s5_p(v)			((v) >= -0x10 && (v) <= 0xf)
#  define _u5_p(v)			((v) >= 0 && (v) <= 0x1f)
#  define _u6(v)			((v) & 0x3f)
#  define _s6_p(v)			((v) >= -0x20 && (v) <= 0x1f)
#  define _u6_p(v)			((v) >= 0 && (v) <= 0x3f)
#  define _u7(v)			((v) & 0x7f)
#  define _s7_p(v)			((v) >= -0x40 && (v) <= 0x3f)
#  define _u7_p(v)			((v) >= 0 && (v) <= 0x7f)
#  define _u8(v)			((v) & 0xff)
#  define _s8_p(v)			((v) >= -0x80 && (v) <= 0x7f)
#  define _u8_p(v)			((v) >= 0 && (v) <= 0xff)
#  define _u11(v)			((v) & 0x7ff)
#  define _s11_p(v)			((v) >= -0x400 && (v) <= 0x3ff)
#  define _u11_p(v)			((v) >= 0 && (v) <= 0x7ff)
#  define _u14(v)			((v) & 0x3fff)
#  define _s14_p(v)			((v) >= -0x2000 && (v) <= 0x1fff)
#  define _u14_p(v)			((v) >= 0 && (v) <= 0x3fff)
#  define _u16(v)			((v) & 0xffff)
#  define _s16_p(v)			((v) >= -0x8000 && (v) <= 0x7fff)
#  define _u16_p(v)			((v) >= 0 && (v) <= 0xffff)
#  define _u21(v)			((v) & 0x1fffff)
#  define _s21_p(v)			((v) >= -0x100000 && (v) <= 0xfffff)
#  define _u21_p(v)			((v) >= 0 && (v) <= 0x1fffff)
#  define _u26(v)			((v) & 0x3ffffff)
#  define _s26_p(v)			((v) >= -0x2000000 && (v) <= 0x1ffffff)
#  define _u26_p(v)			((v) >= 0 && (v) <= 0x3ffffff)
#  define _u32(v)			((v) & 0xffffffff)
#  define _s32_p(v)			((v) >= -0x80000000 && (v) <= 0x7fffffff)
#  define _u32_p(v)			((v) >= 0 && (v) <= 0xffffffff)
#  define ii(i)				*_jit->pc.ui++ = i
#  define stack_framesize		224
#  define _S0_REGNO			0x09
#  define _S1_REGNO			0x0a
#  define _S2_REGNO			0x0b
#  define _S3_REGNO			0x0c
#  define _S4_REGNO			0x0d
#  define _S5_REGNO			0x0e
#  define _FP_REGNO			0x0f
#  define _A0_REGNO			0x10
#  define _A1_REGNO			0x11
#  define _A2_REGNO			0x12
#  define _V0_REGNO			0x00
#  define _T0_REGNO			0x01
#  define _T1_REGNO			0x02
#  define _RA_REGNO			0x1a
#  define _PV_REGNO			0x1b
#  define _GP_REGNO			0x1d
#  define _SP_REGNO			0x1e
#  define _R31_REGNO			0x1f
#  define Pcd(o,n)			_Pcd(_jit,o,n)
static void _Pcd(jit_state_t*,int,unsigned int) maybe_unused;
#  define Bra(o,ra,d)			_Bra(_jit,o,ra,d)
static void _Bra(jit_state_t*,int,int,int);
#  define Mem(o,ra,rb,d)			_Mem(_jit,o,ra,rb,d)
static void _Mem(jit_state_t*,int,int,int,unsigned int);
#  define Mbr(o,ra,rb,h,d)		_Mbr(_jit,o,ra,rb,h,d)
static void _Mbr(jit_state_t*,int,int,int,int,int);
#  define Opr(o,ra,rb,f,rc)		_Opr(_jit,o,ra,rb,f,rc)
static void _Opr(jit_state_t*,int,int,int,unsigned int,int);
#  define Opi(o,ra,i,f,rc)		_Opi(_jit,o,ra,i,f,rc)
static void _Opi(jit_state_t*,int,int,unsigned int,unsigned int,int);
#  define ADDL(ra,rb,rc)		Opr(0x10,ra,rb,0x00,rc)
#  define ADDLi(ra,im,rc)		Opi(0x10,ra,im,0x00,rc)
#  define ADDL_V(ra,rb,rc)		Opr(0x10,ra,rb,0x40,rc)
#  define ADDL_Vi(ra,im,rc)		Opi(0x10,ra,im,0x40,rc)
#  define ADDQ(ra,rb,rc)		Opr(0x10,ra,rb,0x20,rc)
#  define ADDQi(ra,im,rc)		Opi(0x10,ra,im,0x20,rc)
#  define ADDQ_V(ra,rb,rc)		Opr(0x10,ra,rb,0x60,rc)
#  define ADDQ_Vi(ra,im,rc)		Opi(0x10,ra,im,0x60,rc)
#  define AMASK(rb,rc)			Opr(0x11,_R31_REGNO,rb,0x61,rc)
#  define AND(ra,rb,rc)			Opr(0x11,ra,rb,0x00,rc)
#  define ANDi(ra,im,rc)		Opi(0x11,ra,im,0x00,rc)
#  define BEQ(ra,d)			Bra(0x39,ra,d)
#  define BGE(ra,d)			Bra(0x3e,ra,d)
#  define BGT(ra,d)			Bra(0x3f,ra,d)
#  define BIC(ra,rb,rc)			Opr(0x11,ra,rb,0x08,rc)
#  define ANDNOT(ra,rb,rc)		BIC(ra,rb,rc)
#  define BICi(ra,im,rc)		Opi(0x11,ra,im,0x08,rc)
#  define ANDNOTi(ra,im,rc)		BICi(ra,im,rc)
#  define BIS(ra,rb,rc)			Opr(0x11,ra,rb,0x20,rc)
#  define BISi(ra,im,rc)		Opi(0x11,ra,im,0x20,rc)
#  define OR(ra,rb,rc)			BIS(ra,rb,rc)
#  define ORi(ra,im,rc)			BISi(ra,im,rc)
#  define BLBC(ra,d)			Bra(0x38,ra,d)
#  define BLBS(ra,d)			Bra(0x3c,ra,d)
#  define BLE(ra,d)			Bra(0x3b,ra,d)
#  define BLT(ra,d)			Bra(0x3a,ra,d)
#  define BNE(ra,d)			Bra(0x3d,ra,d)
#  define BR(ra,d)			Bra(0x30,ra,d)
#  define BSR(ra,d)			Bra(0x34,ra,d)
#  define CALL_PAL(c)			Pcd(0x00,c)
#  define CMOVEQ(ra,rb,rc)		Opr(0x11,ra,rb,0x24,rc)
#  define CMOVEQi(ra,im,rc)		Opi(0x11,ra,im,0x24,rc)
#  define CMOVGE(ra,rb,rc)		Opr(0x11,ra,rb,0x46,rc)
#  define CMOVGEi(ra,im,rc)		Opi(0x11,ra,im,0x46,rc)
#  define CMOVGT(ra,rb,rc)		Opr(0x11,ra,rb,0x66,rc)
#  define CMOVGTi(ra,im,rc)		Opi(0x11,ra,im,0x66,rc)
#  define CMOVLBC(ra,rb,rc)		Opr(0x11,ra,rb,0x16,rc)
#  define CMOVLBCi(ra,im,rc)		Opi(0x11,ra,im,0x16,rc)
#  define CMOVLBS(ra,rb,rc)		Opr(0x11,ra,rb,0x14,rc)
#  define CMOVLBSi(ra,im,rc)		Opi(0x11,ra,im,0x14,rc)
#  define CMOVLE(ra,rb,rc)		Opr(0x11,ra,rb,0x64,rc)
#  define CMOVLEi(ra,im,rc)		Opi(0x11,ra,im,0x64,rc)
#  define CMOVLT(ra,rb,rc)		Opr(0x11,ra,rb,0x44,rc)
#  define CMOVLTi(ra,im,rc)		Opi(0x11,ra,im,0x44,rc)
#  define CMOVNE(ra,rb,rc)		Opr(0x11,ra,rb,0x26,rc)
#  define CMOVNEi(ra,im,rc)		Opi(0x11,ra,im,0x26,rc)
#  define CMPBGE(ra,rb,rc)		Opr(0x10,ra,rb,0x0f,rc)
#  define CMPBGEi(ra,im,rc)		Opi(0x10,ra,im,0x0f,rc)
#  define CMPEQ(ra,rb,rc)		Opr(0x10,ra,rb,0x2d,rc)
#  define CMPEQi(ra,im,rc)		Opi(0x10,ra,im,0x2d,rc)
#  define CMPLE(ra,rb,rc)		Opr(0x10,ra,rb,0x6d,rc)
#  define CMPLEi(ra,im,rc)		Opi(0x10,ra,im,0x6d,rc)
#  define CMPLT(ra,rb,rc)		Opr(0x10,ra,rb,0x4d,rc)
#  define CMPLTi(ra,im,rc)		Opi(0x10,ra,im,0x4d,rc)
#  define CMPULE(ra,rb,rc)		Opr(0x10,ra,rb,0x3d,rc)
#  define CMPULEi(ra,im,rc)		Opi(0x10,ra,im,0x3d,rc)
#  define CMPULT(ra,rb,rc)		Opr(0x10,ra,rb,0x1d,rc)
#  define CMPULTi(ra,im,rc)		Opi(0x10,ra,im,0x1d,rc)
#  define CTLZ(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x32,rc)
#  define CTPOP(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x30,rc)
#  define CTTZ(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x33,rc)
#  define ECB(rb,d)			Mem(0x18,_R31_REGNO,rb,0xe800)
#  define EQV(ra,rb,rc)			Opr(0x11,ra,rb,0x48,rc)
#  define XORNOT(ra,rb,rc)		EQV(ra,rb,rc)
#  define EQVi(ra,im,rc)		Opi(0x11,ra,im,0x48,rc)
#  define XORNOTi(ra,im,rc)		EQVi(ra,im,rc)
#  define EXCB()			Mem(0x18,0,0,0x0400)
#  define EXTBL(ra,rb,rc)		Opr(0x12,ra,rb,0x06,rc)
#  define EXTBLi(ra,im,rc)		Opi(0x12,ra,im,0x06,rc)
#  define EXTLH(ra,rb,rc)		Opr(0x12,ra,rb,0x6a,rc)
#  define EXTLHi(ra,im,rc)		Opi(0x12,ra,im,0x6a,rc)
#  define EXTLL(ra,rb,rc)		Opr(0x12,ra,rb,0x26,rc)
#  define EXTLLi(ra,im,rc)		Opi(0x12,ra,im,0x26,rc)
#  define EXTQH(ra,rb,rc)		Opr(0x12,ra,rb,0x7a,rc)
#  define EXTQHi(ra,im,rc)		Opi(0x12,ra,im,0x7a,rc)
#  define EXTQL(ra,rb,rc)		Opr(0x12,ra,rb,0x36,rc)
#  define EXTQLi(ra,im,rc)		Opi(0x12,ra,im,0x36,rc)
#  define EXTWH(ra,rb,rc)		Opr(0x12,ra,rb,0x5a,rc)
#  define EXTWHi(ra,im,rc)		Opi(0x12,ra,im,0x5a,rc)
#  define EXTWL(ra,rb,rc)		Opr(0x12,ra,rb,0x16,rc)
#  define EXTWLi(ra,im,rc)		Opi(0x12,ra,im,0x16,rc)
#  define FETCH(rb,d)			Mem(0x18,_R31_REGNO,rb,0x8000)
#  define FETCH_Mem(rb,d)		Mem(0x18,_R31_REGNO,rb,0xa000)
/* FIXME IMPLVER not disassembled */
#  define IMPLVER(rc)			Opr(0x11,_R31_REGNO,1,0x6c,rc)
#  define INSBL(ra,rb,rc)		Opr(0x12,ra,rb,0x0b,rc)
#  define INSBLi(ra,im,rc)		Opi(0x12,ra,im,0x0b,rc)
#  define INSLH(ra,rb,rc)		Opr(0x12,ra,rb,0x67,rc)
#  define INSLHi(ra,im,rc)		Opi(0x12,ra,im,0x67,rc)
#  define INSLL(ra,rb,rc)		Opr(0x12,ra,rb,0x2b,rc)
#  define INSLLi(ra,im,rc)		Opi(0x12,ra,im,0x2b,rc)
#  define INSQH(ra,rb,rc)		Opr(0x12,ra,rb,0x77,rc)
#  define INSQHi(ra,im,rc)		Opi(0x12,ra,im,0x77,rc)
#  define INSQL(ra,rb,rc)		Opr(0x12,ra,rb,0x3b,rc)
#  define INSQLi(ra,im,rc)		Opi(0x12,ra,im,0x3b,rc)
#  define INSWH(ra,rb,rc)		Opr(0x12,ra,rb,0x57,rc)
#  define INSWHi(ra,im,rc)		Opi(0x12,ra,im,0x57,rc)
#  define INSWL(ra,rb,rc)		Opr(0x12,ra,rb,0x1b,rc)
#  define INSWLi(ra,im,rc)		Opi(0x12,ra,im,0x1b,rc)
#  define JMP(ra,rb,d)			Mbr(0x1a,ra,rb,0,d)
#  define JSR(ra,rb,d)			Mbr(0x1a,ra,rb,1,d)
#  define JSR_COROUTINE(ra,rb,d)	Mbr(0x1a,ra,rb,3,d)
#  define JCR(ra,rb,rd)			JSR_COROUTINE(ra,rb,d)
#  define LDA(ra,rb,d)			Mem(0x08,ra,rb,d)
#  define LDAH(ra,rb,d)			Mem(0x09,ra,rb,d)
#  define LDBU(ra,rb,d)			Mem(0x0a,ra,rb,d)
#  define LDWU(ra,rb,d)			Mem(0x0c,ra,rb,d)
#  define LDL(ra,rb,d)			Mem(0x28,ra,rb,d)
#  define LDL_L(ra,rb,d)		Mem(0x2a,ra,rb,d)
#  define LDQ(ra,rb,d)			Mem(0x29,ra,rb,d)
#  define LDQ_L(ra,rb,d)		Mem(0x2b,ra,rb,d)
#  define LDQ_U(ra,rb,d)		Mem(0x0b,ra,rb,d)
#  define MAXSB8(ra,rb,rc)		Opr(0x1c,ra,rb,0x3e,rc)
#  define MAXSW4(ra,rb,rc)		Opr(0x1c,ra,rb,0x3f,rc)
#  define MAXSUB8(ra,rb,rc)		Opr(0x1c,ra,rb,0x3c,rc)
#  define MAXSUW4(ra,rb,rc)		Opr(0x1c,ra,rb,0x3d,rc)
#  define MB()				Mem(0x18,_R31_REGNO,_R31_REGNO,0x4000)
#  define MINSB8(ra,rb,rc)		Opr(0x1c,ra,rb,0x38,rc)
#  define MINSW4(ra,rb,rc)		Opr(0x1c,ra,rb,0x39,rc)
#  define MINSUB8(ra,rb,rc)		Opr(0x1c,ra,rb,0x3a,rc)
#  define MINSUW4(ra,rb,rc)		Opr(0x1c,ra,rb,0x3b,rc)
#  define MSKBL(ra,rb,rc)		Opr(0x12,ra,rb,0x02,rc)
#  define MSKBLi(ra,im,rc)		Opi(0x12,ra,im,0x02,rc)
#  define MSKLH(ra,rb,rc)		Opr(0x12,ra,rb,0x62,rc)
#  define MSKLHi(ra,im,rc)		Opi(0x12,ra,im,0x62,rc)
#  define MSKLL(ra,rb,rc)		Opr(0x12,ra,rb,0x22,rc)
#  define MSKLLi(ra,im,rc)		Opi(0x12,ra,im,0x22,rc)
#  define MSKQH(ra,rb,rc)		Opr(0x12,ra,rb,0x72,rc)
#  define MSKQHi(ra,im,rc)		Opi(0x12,ra,im,0x72,rc)
#  define MSKQL(ra,rb,rc)		Opr(0x12,ra,rb,0x32,rc)
#  define MSKQLi(ra,im,rc)		Opi(0x12,ra,im,0x32,rc)
#  define MSKWH(ra,rb,rc)		Opr(0x12,ra,rb,0x52,rc)
#  define MSKWHi(ra,im,rc)		Opi(0x12,ra,im,0x52,rc)
#  define MSKWL(ra,rb,rc)		Opr(0x12,ra,rb,0x12,rc)
#  define MSKWLi(ra,im,rc)		Opi(0x12,ra,im,0x12,rc)
#  define MULL(ra,rb,rc)		Opr(0x13,ra,rb,0x00,rc)
#  define MULLi(ra,im,rc)		Opi(0x13,ra,im,0x00,rc)
#  define MULL_V(ra,rb,rc)		Opr(0x13,ra,rb,0x40,rc)
#  define MULL_Vi(ra,im,rc)		Opi(0x13,ra,im,0x40,rc)
#  define MULQ(ra,rb,rc)		Opr(0x13,ra,rb,0x20,rc)
#  define MULQi(ra,im,rc)		Opi(0x13,ra,im,0x20,rc)
#  define MULQ_V(ra,rb,rc)		Opr(0x13,ra,rb,0x60,rc)
#  define MULQ_Vi(ra,im,rc)		Opi(0x13,ra,im,0x60,rc)
#  define ORNOT(ra,rb,rc)		Opr(0x11,ra,rb,0x28,rc)
#  define ORNOTi(ra,im,rc)		Opi(0x11,ra,im,0x28,rc)
#  define PERR(ra,rb,rc)		Opr(0x1c,ra,rb,0x31,rc)
#  define PKLB(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x37,rc)
#  define PKWB(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x36,rc)
/* FIXME PREFETCH* not disassembled */
#  define PREFETCH(rb,d)		Mem(0x28,_R31_REGNO,rb,d)
#  define PREFETCH_EN(rb,d)		Mem(0x29,_R31_REGNO,rb,d)
#  define PREFETCH_M(rb,d)		Mem(0x22,_R31_REGNO,rb,d)
#  define PREFETCH_MEN(rb,d)		Mem(0x23,_R31_REGNO,rb,d)
#  define RC(ra)			Mem(0x18,ra,_R31_REGNO,0xe000)
#  define RET(ra,rb,d)			Mbr(0x1a,ra,rb,2,d)
#  define RPCC(ra)			Mem(0x18,ra,_R31_REGNO,0xc000)
#  define RS(ra)			Mem(0x18,ra,_R31_REGNO,0xf000)
#  define S4ADDL(ra,rb,rc)		Opr(0x10,ra,rb,0x02,rc)
#  define S4ADDi(ra,im,rc)		Opi(0x10,ra,im,0x02,rc)
#  define S4ADDQ(ra,rb,rc)		Opr(0x10,ra,rb,0x22,rc)
#  define S4ADDQi(ra,im,rc)		Opi(0x10,ra,im,0x22,rc)
#  define S4SUBL(ra,rb,rc)		Opr(0x10,ra,rb,0x0b,rc)
#  define S4SUBLi(ra,im,rc)		Opi(0x10,ra,im,0x0b,rc)
#  define S4SUBQ(ra,rb,rc)		Opr(0x10,ra,rb,0x2b,rc)
#  define S4SUBQi(ra,im,rc)		Opi(0x10,ra,im,0x2b,rc)
#  define S8ADDL(ra,rb,rc)		Opr(0x10,ra,rb,0x12,rc)
#  define S8ADDLi(ra,im,rc)		Opi(0x10,ra,im,0x12,rc)
#  define S8ADDQ(ra,rb,rc)		Opr(0x10,ra,rb,0x32,rc)
#  define S8ADDQi(ra,im,rc)		Opi(0x10,ra,im,0x32,rc)
#  define S8SUBL(ra,rb,rc)		Opr(0x10,ra,rb,0x1b,rc)
#  define S8SUBLi(ra,im,rc)		Opi(0x10,ra,im,0x1b,rc)
#  define S8SUBQ(ra,rb,rc)		Opr(0x10,ra,rb,0x3b,rc)
#  define S8SUBQi(ra,im,rc)		Opi(0x10,ra,im,0x3b,rc)
#  define SEXTB(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x00,rc)
/* FIXME not disassembled */
#  define SEXTBi(im,rc)			Opi(0x1c,_R31_REGNO,im,0x00,rc)
#  define SEXTW(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x01,rc)
/* FIXME not disassembled */
#  define SEXTWi(im,rc)			Opi(0x1c,_R31_REGNO,im,0x01,rc)
#  define SLL(ra,rb,rc)			Opr(0x12,ra,rb,0x39,rc)
#  define SLLi(ra,im,rc)		Opi(0x12,ra,im,0x39,rc)
#  define SRA(ra,rb,rc)			Opr(0x12,ra,rb,0x3c,rc)
#  define SRAi(ra,im,rc)		Opi(0x12,ra,im,0x3c,rc)
#  define SRL(ra,rb,rc)			Opr(0x12,ra,rb,0x34,rc)
#  define SRLi(ra,im,rc)		Opi(0x12,ra,im,0x34,rc)
#  define STB(ra,rb,d)			Mem(0x0e,ra,rb,d)
#  define STL(ra,rb,d)			Mem(0x2c,ra,rb,d)
#  define STL_C(ra,rb,d)		Mem(0x2e,ra,rb,d)
#  define STQ(ra,rb,d)			Mem(0x2d,ra,rb,d)
#  define STQ_C(ra,rb,d)		Mem(0x2f,ra,rb,d)
#  define STQ_U(ra,rb,d)		Mem(0x0f,ra,rb,d)
#  define STW(ra,rb,d)			Mem(0x0d,ra,rb,d)
#  define SUBL(ra,rb,rc)		Opr(0x10,ra,rb,0x09,rc)
#  define SUBLi(ra,im,rc)		Opi(0x10,ra,im,0x09,rc)
#  define SUBL_V(ra,rb,rc)		Opr(0x10,ra,rb,0x49,rc)
#  define SUBL_Vi(ra,im,rc)		Opi(0x10,ra,im,0x49,rc)
#  define SUBQ(ra,rb,rc)		Opr(0x10,ra,rb,0x29,rc)
#  define SUBQi(ra,im,rc)		Opi(0x10,ra,im,0x29,rc)
#  define SUBQ_V(ra,rb,rc)		Opr(0x10,ra,rb,0x69,rc)
#  define SUBQ_Vi(ra,im,rc)		Opi(0x10,ra,im,0x69,rc)
#  define TRAPB()			Mem(0x18,_R31_REGNO,_R31_REGNO,0x0000)
#  define UMULH(ra,rb,rc)		Opr(0x13,ra,rb,0x30,rc)
#  define UMULHi(ra,im,rc)		Opi(0x13,ra,im,0x30,rc)
#  define UNPKBL(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x35,rc)
#  define UNPKBW(rb,rc)			Opr(0x1c,_R31_REGNO,rb,0x34,rc)
#  define WH64(ra)			Mem(0x18,ra,_R31_REGNO,0xf800)
#  define WH64EN(ra)			Mem(0x18,ra,_R31_REGNO,0xfc00)
#  define WMB()				Mem(0x18,_R31_REGNO,_R31_REGNO,0x4400)
#  define XOR(ra,rb,rc)			Opr(0x11,ra,rb,0x40,rc)
#  define XORi(ra,im,rc)		Opi(0x11,ra,im,0x40,rc)
#  define ZAP(ra,rb,rc)			Opr(0x12,ra,rb,0x30,rc)
#  define ZAPi(ra,im,rc)		Opi(0x12,ra,im,0x30,rc)
#  define ZAPNOT(ra,rb,rc)		Opr(0x12,ra,rb,0x31,rc)
#  define ZAPNOTi(ra,im,rc)		Opi(0x12,ra,im,0x31,rc)
#  define NOP()				BIS(_R31_REGNO,_R31_REGNO,_R31_REGNO)
#  define MOV(ra,rc)			BIS(ra,ra,rc)
#  define MOVi(im,rc)			BISi(_R31_REGNO,im,rc)
#  define NEGL(ra,rc)			SUBL(_R31_REGNO,ra,rc)
#  define NEGQ(ra,rc)			SUBQ(_R31_REGNO,ra,rc)
#  define NOT(ra,rc)			ORNOT(_R31_REGNO,ra,rc)
#  define nop(i0)			_nop(_jit,i0)
static void _nop(jit_state_t*,jit_int32_t);
#  define movr(r0,r1)			_movr(_jit,r0,r1)
static void _movr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi(r0,i0)			_movi(_jit,r0,i0)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#  define movi_p(r0,i0)			_movi_p(_jit,r0,i0)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#  define negr(r0,r1)			NEGQ(r1,r0)
#  define comr(r0,r1)			NOT(r1,r0)
#  define addr(r0,r1,r2)		ADDQ(r1,r2,r0)
#  define addi(r0,r1,i0)		_addi(_jit,r0,r1,i0)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addcr(r0,r1,i0)		_addcr(_jit,r0,r1,i0)
static void _addcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addci(r0,r1,i0)		_addci(_jit,r0,r1,i0)
static void _addci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addxr(r0,r1,i0)		_addxr(_jit,r0,r1,i0)
static void _addxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addxi(r0,r1,i0)		_addxi(_jit,r0,r1,i0)
static void _addxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subr(r0,r1,r2)		SUBQ(r1,r2,r0)
#  define subi(r0,r1,i0)		_subi(_jit,r0,r1,i0)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subcr(r0,r1,i0)		_subcr(_jit,r0,r1,i0)
static void _subcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subci(r0,r1,i0)		_subci(_jit,r0,r1,i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subxr(r0,r1,i0)		_subxr(_jit,r0,r1,i0)
static void _subxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subxi(r0,r1,i0)		_subxi(_jit,r0,r1,i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t, jit_word_t);
#  define mulr(r0,r1,r2)		MULQ(r1,r2,r0)
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
static jit_word_t __idiv(jit_word_t, jit_word_t);
#  define divr(r0,r1,r2)		_divr(_jit,r0,r1,r2)
static void _divr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi(r0,r1,i0)		_divi(_jit,r0,r1,i0)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static jit_uword_t __udiv(jit_uword_t, jit_uword_t);
#  define divr_u(r0,r1,r2)		_divr_u(_jit,r0,r1,r2)
static void _divr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi_u(r0,r1,i0)		_divi_u(_jit,r0,r1,i0)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static jit_word_t __irem(jit_word_t, jit_word_t);
#  define remr(r0,r1,r2)		_remr(_jit,r0,r1,r2)
static void _remr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi(r0,r1,i0)		_remi(_jit,r0,r1,i0)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static jit_uword_t __urem(jit_uword_t, jit_uword_t);
#  define remr_u(r0,r1,r2)		_remr_u(_jit,r0,r1,r2)
static void _remr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi_u(r0,r1,i0)		_remi_u(_jit,r0,r1,i0)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
static jit_word_t __idivrem(jit_word_t, jit_word_t, jit_word_t*);
#  define qdivr(r0,r1,r2,r3)		_qdivr(_jit,r0,r1,r2,r3)
static void _qdivr(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define qdivi(r0,r1,r2,i0)		_qdivi(_jit,r0,r1,r2,i0)
static void _qdivi(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
static jit_word_t __udivrem(jit_uword_t, jit_uword_t, jit_uword_t*);
#  define qdivr_u(r0,r1,r2,r3)		_qdivr_u(_jit,r0,r1,r2,r3)
static void _qdivr_u(jit_state_t*,
		     jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define qdivi_u(r0,r1,r2,i0)		_qdivi_u(_jit,r0,r1,r2,i0)
static void _qdivi_u(jit_state_t*,
		     jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define lshr(r0,r1,r2)		SLL(r1,r2,r0)
#  define lshi(r0,r1,i0)		_lshi(_jit,r0,r1,i0)
static void _lshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr(r0,r1,r2)		SRA(r1,r2,r0)
#  define rshi(r0,r1,i0)		_rshi(_jit,r0,r1,i0)
static void _rshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr_u(r0,r1,r2)		SRL(r1,r2,r0)
#  define rshi_u(r0,r1,i0)		_rshi_u(_jit,r0,r1,i0)
static void _rshi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define andr(r0,r1,r2)		AND(r1,r2,r0)
#  define andi(r0,r1,i0)		_andi(_jit,r0,r1,i0)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define orr(r0,r1,r2)			OR(r1,r2,r0)
#  define ori(r0,r1,i0)			_ori(_jit,r0,r1,i0)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define xorr(r0,r1,r2)		XOR(r1,r2,r0)
#  define xori(r0,r1,i0)		_xori(_jit,r0,r1,i0)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ltr(r0,r1,r2)			CMPLT(r1,r2,r0)
#  define lti(r0,r1,i0)			_lti(_jit,r0,r1,i0)
static void _lti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ltr_u(r0,r1,r2)		CMPULT(r1,r2,r0)
#  define lti_u(r0,r1,i0)		_lti_u(_jit,r0,r1,i0)
static void _lti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ler(r0,r1,r2)			CMPLE(r1,r2,r0)
#  define lei(r0,r1,i0)			_lei(_jit,r0,r1,i0)
static void _lei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ler_u(r0,r1,r2)		CMPULE(r1,r2,r0)
#  define lei_u(r0,r1,i0)		_lei_u(_jit,r0,r1,i0)
static void _lei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define eqr(r0,r1,r2)			CMPEQ(r1,r2,r0)
#  define eqi(r0,r1,i0)			_eqi(_jit,r0,r1,i0)
static void _eqi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ger(r0,r1,r2)			CMPLE(r2,r1,r0)
#  define gei(r0,r1,i0)			_gei(_jit,r0,r1,i0)
static void _gei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ger_u(r0,r1,r2)		CMPULE(r2,r1,r0)
#  define gei_u(r0,r1,i0)		_gei_u(_jit,r0,r1,i0)
static void _gei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define gtr(r0,r1,r2)			CMPLT(r2,r1,r0)
#  define gti(r0,r1,i0)			_gti(_jit,r0,r1,i0)
static void _gti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define gtr_u(r0,r1,r2)		CMPULT(r2,r1,r0)
#  define gti_u(r0,r1,i0)		_gti_u(_jit,r0,r1,i0)
static void _gti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ner(r0,r1,r2)			_ner(_jit,r0,r1,r2)
static void _ner(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nei(r0,r1,i0)			_nei(_jit,r0,r1,i0)
static void _nei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define bltr(i0,r0,r1)		_bltr(_jit,i0,r0,r1)
static jit_word_t _bltr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti(i0,r0,i1)		_blti(_jit,i0,r0,i1)
static jit_word_t _blti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bltr_u(i0,r0,r1)		_bltr_u(_jit,i0,r0,r1)
static jit_word_t _bltr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti_u(i0,r0,i1)		_blti_u(_jit,i0,r0,i1)
static jit_word_t _blti_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bler(i0,r0,r1)		_bler(_jit,i0,r0,r1)
static jit_word_t _bler(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei(i0,r0,i1)		_blei(_jit,i0,r0,i1)
static jit_word_t _blei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bler_u(i0,r0,r1)		_bler_u(_jit,i0,r0,r1)
static jit_word_t _bler_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei_u(i0,r0,i1)		_blei_u(_jit,i0,r0,i1)
static jit_word_t _blei_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define beqr(i0,r0,r1)		_beqr(_jit,i0,r0,r1)
static jit_word_t _beqr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define beqi(i0,r0,i1)		_beqi(_jit,i0,r0,i1)
static jit_word_t _beqi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bger(i0,r0,r1)		_bger(_jit,i0,r0,r1)
static jit_word_t _bger(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei(i0,r0,i1)			_bgei(_jit,i0,r0,i1)
static jit_word_t _bgei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bger_u(i0,r0,r1)		_bger_u(_jit,i0,r0,r1)
static jit_word_t _bger_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei_u(i0,r0,i1)		_bgei_u(_jit,i0,r0,i1)
static jit_word_t _bgei_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bgtr(i0,r0,r1)		_bgtr(_jit,i0,r0,r1)
static jit_word_t _bgtr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti(i0,r0,i1)		_bgti(_jit,i0,r0,i1)
static jit_word_t _bgti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bgtr_u(i0,r0,r1)		_bgtr_u(_jit,i0,r0,r1)
static jit_word_t _bgtr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti_u(i0,r0,i1)		_bgti_u(_jit,i0,r0,i1)
static jit_word_t _bgti_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bner(i0,r0,r1)		_bner(_jit,i0,r0,r1)
static jit_word_t _bner(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bnei(i0,r0,i1)		_bnei(_jit,i0,r0,i1)
static jit_word_t _bnei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define baddr(i0,r0,r1,cc)		_baddr(_jit,i0,r0,r1,cc)
static jit_word_t _baddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,
			 jit_bool_t);
#  define baddi(i0,r0,i1,cc)		_baddi(_jit,i0,r0,i1,cc)
static jit_word_t _baddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t,
			 jit_bool_t);
#  define baddr_u(i0,r0,r1,cc)		_baddr_u(_jit,i0,r0,r1,cc)
static jit_word_t _baddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,
			   jit_bool_t);
#  define baddi_u(i0,r0,i1,cc)		_baddi_u(_jit,i0,r0,i1,cc)
static jit_word_t _baddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t,
			   jit_bool_t);
#  define boaddr(i0,r0,r1)		baddr(i0,r0,r1,1)
#  define boaddi(i0,r0,i1)		baddi(i0,r0,i1,1)
#  define boaddr_u(i0,r0,r1)		baddr_u(i0,r0,r1,1)
#  define boaddi_u(i0,r0,i1)		baddi_u(i0,r0,i1,1)
#  define bxaddr(i0,r0,r1)		baddr(i0,r0,r1,0)
#  define bxaddi(i0,r0,i1)		baddi(i0,r0,i1,0)
#  define bxaddr_u(i0,r0,r1)		baddr_u(i0,r0,r1,0)
#  define bxaddi_u(i0,r0,i1)		baddi_u(i0,r0,i1,0)
#  define bsubr(i0,r0,r1,cc)		_bsubr(_jit,i0,r0,r1,cc)
static jit_word_t _bsubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,
			 jit_bool_t);
#  define bsubi(i0,r0,i1,cc)		_bsubi(_jit,i0,r0,i1,cc)
static jit_word_t _bsubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t,
			 jit_bool_t);
#  define bsubr_u(i0,r0,r1,cc)		_bsubr_u(_jit,i0,r0,r1,cc)
static jit_word_t _bsubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,
			   jit_bool_t);
#  define bsubi_u(i0,r0,i1,cc)		_bsubi_u(_jit,i0,r0,i1,cc)
static jit_word_t _bsubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t,
			   jit_bool_t);
#  define bosubr(i0,r0,r1)		bsubr(i0,r0,r1,1)
#  define bosubi(i0,r0,i1)		bsubi(i0,r0,i1,1)
#  define bosubr_u(i0,r0,r1)		bsubr_u(i0,r0,r1,1)
#  define bosubi_u(i0,r0,i1)		bsubi_u(i0,r0,i1,1)
#  define bxsubr(i0,r0,r1)		bsubr(i0,r0,r1,0)
#  define bxsubi(i0,r0,i1)		bsubi(i0,r0,i1,0)
#  define bxsubr_u(i0,r0,r1)		bsubr_u(i0,r0,r1,0)
#  define bxsubi_u(i0,r0,i1)		bsubi_u(i0,r0,i1,0)
#  define bmxr(i0,r0,r1,cc)		_bmxr(_jit,i0,r0,r1,cc)
static jit_word_t _bmxr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,
			jit_bool_t);
#  define bmxi(i0,r0,i1,cc)		_bmxi(_jit,i0,r0,i1,cc)
static jit_word_t _bmxi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t,
			jit_bool_t);
#  define bmsr(i0,r0,r1)		bmxr(i0,r0,r1,1)
#  define bmsi(i0,r0,i1)		bmxi(i0,r0,i1,1)
#  define bmcr(i0,r0,r1)		bmxr(i0,r0,r1,0)
#  define bmci(i0,r0,i1)		bmxi(i0,r0,i1,0)
#  define ldr_c(r0,r1)			_ldr_c(_jit,r0,r1)
static void _ldr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_c(r0,i0)			_ldi_c(_jit,r0,i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_uc(r0,r1)			LDBU(r0,r1,0)
#  define ldi_uc(r0,i0)			_ldi_uc(_jit,r0,i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_s(r0,r1)			_ldr_s(_jit,r0,r1)
static void _ldr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_s(r0,i0)			_ldi_s(_jit,r0,i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_us(r0,r1)			LDWU(r0,r1,0)
#  define ldi_us(r0,i0)			_ldi_us(_jit,r0,i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_i(r0,r1)			LDL(r0,r1,0)
#  define ldi_i(r0,i0)			_ldi_i(_jit,r0,i0)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_ui(r0,r1)			_ldr_ui(_jit,r0,r1)
static void _ldr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_ui(r0,i0)			_ldi_ui(_jit,r0,i0)
static void _ldi_ui(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr(r0,r1)			ldr_l(r0,r1)
#  define ldr_l(r0,r1)			LDQ(r0,r1,0)
#  define ldi_l(r0,i0)			_ldi_l(_jit,r0,i0)
static void _ldi_l(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_c(r0,r1,r2)		_ldxr_c(_jit,r0,r1,r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_c(r0,r1,i0)		_ldxi_c(_jit,r0,r1,i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_uc(r0,r1,r2)		_ldxr_uc(_jit,r0,r1,r2)
static void _ldxr_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_uc(r0,r1,i0)		_ldxi_uc(_jit,r0,r1,i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_s(r0,r1,r2)		_ldxr_s(_jit,r0,r1,r2)
static void _ldxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_s(r0,r1,i0)		_ldxi_s(_jit,r0,r1,i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_us(r0,r1,r2)		_ldxr_us(_jit,r0,r1,r2)
static void _ldxr_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_us(r0,r1,i0)		_ldxi_us(_jit,r0,r1,i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_i(r0,r1,r2)		_ldxr_i(_jit,r0,r1,r2)
static void _ldxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_i(r0,r1,i0)		_ldxi_i(_jit,r0,r1,i0)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_ui(r0,r1,r2)		_ldxr_ui(_jit,r0,r1,r2)
static void _ldxr_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_ui(r0,r1,i0)		_ldxi_ui(_jit,r0,r1,i0)
static void _ldxi_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr(r0,r1,r2)		ldxr_l(r0,r1,r2)
#  define ldxr_l(r0,r1,r2)		_ldxr_l(_jit,r0,r1,r2)
static void _ldxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi(r0,r1,i0)		ldxi_l(r0,r1,i0)
#  define ldxi_l(r0,r1,i0)		_ldxi_l(_jit,r0,r1,i0)
static void _ldxi_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_c(r0,r1)			STB(r1,r0,0)
#  define sti_c(i0,r0)			_sti_c(_jit,i0,r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_s(r0,r1)			STW(r1,r0,0)
#  define sti_s(i0,r0)			_sti_s(_jit,i0,r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_i(r0,r1)			STL(r1,r0,0)
#  define sti_i(i0,r0)			_sti_i(_jit,i0,r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#  define str(r0,r1)			str_l(r0,r1)
#  define str_l(r0,r1)			STQ(r1,r0,0)
#  define sti_l(i0,r0)			_sti_l(_jit,i0,r0)
static void _sti_l(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_c(r0,r1,r2)		_stxr_c(_jit,r0,r1,r2)
static void _stxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_c(i0,r0,r1)		_stxi_c(_jit,i0,r0,r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_s(r0,r1,r2)		_stxr_s(_jit,r0,r1,r2)
static void _stxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_s(i0,r0,r1)		_stxi_s(_jit,i0,r0,r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_i(r0,r1,r2)		_stxr_i(_jit,r0,r1,r2)
static void _stxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_i(i0,r0,r1)		_stxi_i(_jit,i0,r0,r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_l(r0,r1,r2)		_stxr_l(_jit,r0,r1,r2)
static void _stxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi(i0,r0,r1)		stxi_l(i0,r0,r1)
#  define stxi_l(i0,r0,r1)		_stxi_l(_jit,i0,r0,r1)
static void _stxi_l(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define extr_c(r0,r1)			_extr_c(_jit,r0,r1)
static void _extr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_uc(r0,r1)		_extr_uc(_jit,r0,r1)
static void _extr_uc(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_s(r0,r1)			_extr_s(_jit,r0,r1)
static void _extr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_us(r0,r1)		_extr_us(_jit,r0,r1)
static void _extr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_i(r0,r1)			_extr_i(_jit,r0,r1)
static void _extr_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_ui(r0,r1)		_extr_ui(_jit,r0,r1)
static void _extr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#    define htonr_us(r0,r1)		_htonr_us(_jit,r0,r1)
static void _htonr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#    define htonr_ui(r0,r1)		_htonr_ui(_jit,r0,r1)
static void _htonr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#    define htonr_ul(r0,r1)		_htonr_ul(_jit,r0,r1)
static void _htonr_ul(jit_state_t*,jit_int32_t,jit_int32_t);
#  else
#    define htonr_us(r0,r1)		extr_us(r0,r1)
#    define htonr_ui(r0,r1)		extr_ui(r0,r1)
#    define htonr_ul(r0,r1)		movr(r0,r1)
#  endif
#  define jmpr(r0)			JMP(_R31_REGNO,r0,0)
#  define jmpi(i0)			_jmpi(_jit,i0)
static void _jmpi(jit_state_t*, jit_word_t);
#  define jmpi_p(i0)			_jmpi_p(_jit,i0)
static jit_word_t _jmpi_p(jit_state_t*, jit_word_t);
#define callr(r0)			_callr(_jit,r0)
static void _callr(jit_state_t*, jit_int32_t);
#  define calli(i0)			_calli(_jit,i0)
static void _calli(jit_state_t*, jit_word_t);
#  define calli_p(i0)			_calli_p(_jit,i0)
static jit_word_t _calli_p(jit_state_t*, jit_word_t);
#  define prolog(node)			_prolog(_jit,node)
static void _prolog(jit_state_t*,jit_node_t*);
#  define epilog(node)			_epilog(_jit,node)
static void _epilog(jit_state_t*,jit_node_t*);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#  define patch_at(jump,label)		_patch_at(_jit,jump,label)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
#endif

#if CODE
static void
_Pcd(jit_state_t *_jit, int o, unsigned int n)
{
    assert(_u6_p(o));
    assert(_u26_p(n));
    ii((o<<26)|_u26(n));
}

static void
_Bra(jit_state_t *_jit, int o, int ra, int d)
{
    assert(_u6_p(o));
    assert(_u5_p(ra));
    assert(_s21_p(d));
    ii((o<<26)|(ra<<21)|_u21(d));
}

static void
_Mem(jit_state_t *_jit, int o, int ra, int rb, unsigned int d)
{
    assert(_u6_p(o));
    assert(_u5_p(ra));
    assert(_u5_p(rb));
    assert(_u16_p(d));
    ii((o<<26)|(ra<<21)|(rb<<16)|_u16(d));
}

static void
_Mbr(jit_state_t *_jit, int o, int ra, int rb, int h, int d)
{
    assert(_u6_p(o));
    assert(_u5_p(ra));
    assert(_u5_p(rb));
    assert(_u2_p(h));
    assert(_s14_p(d));
    ii((o<<26)|(ra<<21)|(rb<<16)|(h<<14)|_u14(d));
}

static void
_Opr(jit_state_t *_jit, int o, int ra, int rb, unsigned int f, int rc)
{
    assert(_u6_p(o));
    assert(_u5_p(ra));
    assert(_u5_p(rb));
    assert(_u5_p(rc));
    assert(_u11_p(f));
    ii((o<<26)|(ra<<21)|(rb<<16)|(_u11(f)<<5)|rc);
}

static void
_Opi(jit_state_t *_jit, int o, int ra, unsigned int i, unsigned int f, int rc)
{
    assert(_u6_p(o));
    assert(_u5_p(ra));
    assert(_u8_p(i));
    assert(_u5_p(rc));
    assert(_u7_p(f));
    ii((o<<26)|(ra<<21)|(_u8(i)<<13)|(1<<12)|(_u7(f)<<5)|rc);
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
	MOV(r1, r0);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_int16_t		s0, s1, s2, s3;
    s0 = i0;
    s1 = i0 >> 16;
    s2 = i0 >> 32;
    s3 = i0 >> 48;
    if (s0 < 0)
	++s1;
    if (s2 < 0)
	++s3;
    if (_u8_p(i0))
	MOVi(_u8(i0), r0);
    else if (_s16_p(i0))
	LDA(r0, _R31_REGNO, _u16(s0));
    else if (_s32_p(i0)) {
	LDA(r0, _R31_REGNO, _u16(s0));
	LDAH(r0, r0, _u16(s1));
    }
    else if (_u32_p(i0)) {
	LDA(r0, _R31_REGNO, _u16(s0));
	if (s1)
	    LDAH(r0, r0, _u16(s1));
	lshi(r0, r0, 32);
	rshi_u(r0, r0, 32);
    }
    else if (_u32(i0) == 0) {
	LDA(r0, _R31_REGNO, _u16(s2));
	if (s3)
	    LDAH(r0, r0, _u16(s3));
	lshi(r0, r0, 32);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	LDA(r0, _R31_REGNO, _u16(s0));
	LDA(rn(reg), _R31_REGNO, _u16(s2));
	if (s1)
	    LDAH(r0, r0, _u16(s1));
	if (s3)
	    LDAH(rn(reg), rn(reg), _u16(s3));
	lshi(r0, r0, 32);
	rshi_u(r0, r0, 32);
	lshi(rn(reg), rn(reg), 32);
	orr(r0, r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    jit_int16_t		s0, s1, s2, s3;
    w = _jit->pc.w;
    reg = jit_get_reg(jit_class_gpr);
    s0 = i0;
    s1 = i0 >> 16;
    s2 = i0 >> 32;
    s3 = i0 >> 48;
    if (s0 < 0)
	++s1;
    if (s2 < 0)
	++s3;
    LDA(r0, _R31_REGNO, _u16(s0));
    LDA(rn(reg), _R31_REGNO, _u16(s2));
    LDAH(r0, r0, _u16(s1));
    LDAH(rn(reg), rn(reg), _u16(s3));
    lshi(r0, r0, 32);
    rshi_u(r0, r0, 32);
    lshi(rn(reg), rn(reg), 32);
    orr(r0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0))
	ADDQi(r1, i0, r0);
    else if (_s16_p(i0))
	LDA(r0, r1, _u16(i0));
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
    if (_u8_p(i0))
	SUBQi(r1, i0, r0);
    else if (_s16_p(-i0))
	LDA(r0, r1, _u16(-i0));
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
    subi(r0, r1, i0);
    negr(r0, r0);
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0))
	MULQi(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	mulr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_qmulr(jit_state_t *_jit, jit_int32_t r0,
       jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
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
_qmuli(jit_state_t *_jit, jit_int32_t r0,
       jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    qmulr(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
}

static void
_qmulr_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		reg;
    if (r0 == r2 || r0 == r3) {
	reg = jit_get_reg(jit_class_gpr);
	mulr(rn(reg), r2, r3);
    }
    else
	mulr(r0, r2, r3);
    UMULH(r2, r3, r1);
    if (r0 == r2 || r0 == r3) {
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_qmuli_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0)) {
	if (r0 == r2) {
	    reg = jit_get_reg(jit_class_gpr);
	    muli(rn(reg), r2, i0);
	}
	else
	    muli(r0, r2, i0);
	UMULHi(r2, i0, r1);
	if (r0 == r2) {
	    movr(r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	qmulr_u(r0, r1, r2, rn(reg));
	jit_unget_reg(reg);
    }
}

static jit_word_t
__idiv(jit_word_t u, jit_word_t v)
{
    return (u / v);
}

static void
_divr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_A0_REGNO, r1);
    movr(_A1_REGNO, r2);
    calli((jit_word_t)__idiv);
    movr(r0, _V0_REGNO);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_A0_REGNO, r1);
    movi(_A1_REGNO, i0);
    calli((jit_word_t)__idiv);
    movr(r0, _V0_REGNO);
}

static jit_uword_t
__udiv(jit_uword_t u, jit_uword_t v)
{
    return (u / v);
}

static void
_divr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_A0_REGNO, r1);
    movr(_A1_REGNO, r2);
    calli((jit_word_t)__udiv);
    movr(r0, _V0_REGNO);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_A0_REGNO, r1);
    movi(_A1_REGNO, i0);
    calli((jit_word_t)__udiv);
    movr(r0, _V0_REGNO);
}

static jit_word_t
__irem(jit_word_t u, jit_word_t v)
{
    return (u % v);
}

static void
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_A0_REGNO, r1);
    movr(_A1_REGNO, r2);
    calli((jit_word_t)__irem);
    movr(r0, _V0_REGNO);
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_A0_REGNO, r1);
    movi(_A1_REGNO, i0);
    calli((jit_word_t)__irem);
    movr(r0, _V0_REGNO);
}

static jit_uword_t
__urem(jit_uword_t u, jit_uword_t v)
{
    return (u % v);
}

static void
_remr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    movr(_A0_REGNO, r1);
    movr(_A1_REGNO, r2);
    calli((jit_word_t)__urem);
    movr(r0, _V0_REGNO);
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(_A0_REGNO, r1);
    movi(_A1_REGNO, i0);
    calli((jit_word_t)__urem);
    movr(r0, _V0_REGNO);
}

static jit_word_t
__idivrem(jit_word_t u, jit_word_t v, jit_word_t *rem)
{
    *rem = u % v;
    return (u / v);
}

static void
_qdivr(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    movr(_A0_REGNO, r2);
    movr(_A1_REGNO, r3);
    subi(_A2_REGNO, _FP_REGNO, 8);
    calli((jit_word_t)__idivrem);
    movr(r0, _V0_REGNO);
    ldxi(r1, _FP_REGNO, -8);
}

static void
_qdivi(jit_state_t *_jit,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    movr(_A0_REGNO, r2);
    movi(_A1_REGNO, i0);
    subi(_A2_REGNO, _FP_REGNO, 8);
    calli((jit_word_t)__idivrem);
    movr(r0, _V0_REGNO);
    ldxi(r1, _FP_REGNO, -8);
}

static jit_word_t
__udivrem(jit_uword_t u, jit_uword_t v, jit_uword_t *rem)
{
    *rem = u % v;
    return (u / v);
}

static void
_qdivr_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    movr(_A0_REGNO, r2);
    movr(_A1_REGNO, r3);
    subi(_A2_REGNO, _FP_REGNO, 8);
    calli((jit_word_t)__udivrem);
    movr(r0, _V0_REGNO);
    ldxi(r1, _FP_REGNO, -8);
}

static void
_qdivi_u(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    movr(_A0_REGNO, r2);
    movi(_A1_REGNO, i0);
    subi(_A2_REGNO, _FP_REGNO, 8);
    calli((jit_word_t)__udivrem);
    movr(r0, _V0_REGNO);
    ldxi(r1, _FP_REGNO, -8);
}

static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 < 64);
    SLLi(r1, i0, r0);
}

static void
_rshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 < 64);
    SRAi(r1, i0, r0);
}

static void
_rshi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 < 64);
    SRLi(r1, i0, r0);
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0))
	ANDi(r1, i0, r0);
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
    if (_u8_p(i0))
	ORi(r1, i0, r0);
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
    if (_u8_p(i0))
	XORi(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	xorr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_lti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0))
	CMPLTi(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ltr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_lti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0))
	CMPULTi(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ltr_u(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_lei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0))
	CMPLEi(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ler(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_lei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_word_t		ni0;
    ni0 = -i0;
    if (_u8_p(i0))
	CMPULEi(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ler_u(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_eqi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0))
	CMPEQi(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	eqr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_gei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ger(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_gei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ger_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
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
    CMPEQ(r1, r2, r0);
    CMPEQi(r0, 0, r0);
}

static void
_nei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_u8_p(i0)) {
	CMPEQi(r1, i0, r0);
	CMPEQi(r0, 0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ner(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static jit_word_t
_bltr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ltr(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_blti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BLT(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	lti(rn(reg), r0, i1);
	w = _jit->pc.w;
	BNE(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bltr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ltr_u(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_blti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    /* FIXME cannot optimize zero because need to return a patcheable address */
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    lti_u(rn(reg), r0, i1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bler(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ler(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_blei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BLE(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	lei(rn(reg), r0, i1);
	w = _jit->pc.w;
	BNE(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bler_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ler_u(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_blei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BEQ(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	lei_u(rn(reg), r0, i1);
	w = _jit->pc.w;
	BNE(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
     }
    return (w);
}

static jit_word_t
_beqr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    eqr(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_beqi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BEQ(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	eqi(rn(reg), r0, i1);
	w = _jit->pc.w;
	BNE(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bger(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ger(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BGE(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	gei(rn(reg), r0, i1);
	w = _jit->pc.w;
	BNE(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bger_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ger_u(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    /* always true if i1 == 0 */
    if (i0 == 0) {
	w = _jit->pc.w;
	BR(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	gei_u(rn(reg), r0, i1);
	w = _jit->pc.w;
	BNE(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bgtr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    gtr(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BGT(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	gti(rn(reg), r0, i1);
	w = _jit->pc.w;
	BNE(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bgtr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    gtr_u(rn(reg), r0, r1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    gti_u(rn(reg), r0, i1);
    w = _jit->pc.w;
    BNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bner(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    eqr(rn(reg), r0, r1);
    w = _jit->pc.w;
    BEQ(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bnei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BNE(r0, ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	eqi(rn(reg), r0, i1);
	w = _jit->pc.w;
	BEQ(rn(reg), ((i0 - w) >> 2) - 1);
	jit_unget_reg(reg);
    }
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
    jit_int32_t		t3;
    /* t0 = r0 + r1;	overflow = r1 < 0 ? r0 < t0 : t0 < r0 */
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr);
    t2 = jit_get_reg(jit_class_gpr);
    t3 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);		/* t0 = r0 + r1 */
    ltr(rn(t1), r1, _R31_REGNO);	/* t1 = r1 < 0 */
    ltr(rn(t2), r0, rn(t0));		/* t2 = r0 < t0 */
    ltr(rn(t3), rn(t0), r0);		/* t3 = t0 < r0 */
    movr(r0, rn(t0));			/* r0 += r1 */
    CMOVNE(rn(t1), rn(t2), rn(t3));	/* if (t1 == 0) t3 = t2; */
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    w = _jit->pc.w;
    if (carry)
	BNE(rn(t3), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t3), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t3);
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
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    ltr_u(rn(t1), rn(t0), r0);
    movr(r0, rn(t0));
    jit_unget_reg(t0);
    w = _jit->pc.w;
    if (carry)
	BNE(rn(t1), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t1), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t1);
    return (w);
}

static jit_word_t
_baddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
	 jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addi(rn(t0), r0, i1);
    ltr_u(rn(t1), rn(t0), r0);
    movr(r0, rn(t0));
    jit_unget_reg(t0);
    w = _jit->pc.w;
    if (carry)
	BNE(rn(t1), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t1), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t1);
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
    jit_int32_t		t3;
    /* t0 = r0 - r1;	overflow = 0 < r1 ? r0 < t0 : t0 < r0 */
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr);
    t2 = jit_get_reg(jit_class_gpr);
    t3 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);		/* r0 = r0 - r1 */
    ltr(rn(t1), _R31_REGNO, r1);	/* t1 = 0 < r1 */
    ltr(rn(t2), r0, rn(t0));		/* t2 = r0 < t0 */
    ltr(rn(t3), rn(t0), r0);		/* t3 = t0 < r0 */
    movr(r0, rn(t0));			/* r0 -= r1 */
    CMOVNE(rn(t1), rn(t2), rn(t3));	/* if (t1 == 0) t3 = t2; */
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    w = _jit->pc.w;
    if (carry)
	BNE(rn(t3), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t3), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t3);
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
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    ltr_u(rn(t1), r0, rn(t0));
    movr(r0, rn(t0));
    jit_unget_reg(t0);
    w = _jit->pc.w;
    if (carry)
	BNE(rn(t1), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t1), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t1);
    return (w);
}

static jit_word_t
_bsubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
	 jit_bool_t carry)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subi(rn(t0), r0, i1);
    ltr_u(rn(t1), r0, rn(t0));
    movr(r0, rn(t0));
    jit_unget_reg(t0);
    w = _jit->pc.w;
    if (carry)
	BNE(rn(t1), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t1), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t1);
    return (w);
}

static jit_word_t
_bmxr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1,
      jit_bool_t set)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    andr(rn(t0), r0, r1);
    w = _jit->pc.w;
    if (set)
	BNE(rn(t0), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t0), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmxi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
      jit_bool_t set)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    andi(rn(t0), r0, i1);
    w = _jit->pc.w;
    if (set)
	BNE(rn(t0), ((i0 - w) >> 2) - 1);
    else
	BEQ(rn(t0), ((i0 - w) >> 2) - 1);
    jit_unget_reg(t0);
    return (w);
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
    if (_s16_p(i0)) {
	LDBU(r0, _R31_REGNO, _u16(i0));
	extr_c(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_c(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	LDBU(r0, _R31_REGNO, _u16(i0));
    else  {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_uc(r0, rn(reg));
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
    jit_int32_t		reg;
    if (_s16_p(i0)) {
	LDWU(r0, _R31_REGNO, _u16(i0));
	extr_s(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_s(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	LDWU(r0, _R31_REGNO, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_us(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	LDL(r0, _R31_REGNO, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_i(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    ldr_i(r0, r1);
    extr_ui(r0, r0);
}

static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_s16_p(i0)) {
	LDL(r0, _R31_REGNO, _u16(i0));
	extr_ui(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_ui(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_l(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	LDQ(r0, _R31_REGNO, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_l(r0, rn(reg));
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0)) {
	LDBU(r0, r1, _u16(i0));
	extr_c(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_c(r0, rn(reg));
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0))
	LDBU(r0, r1, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_uc(r0, rn(reg));
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0)) {
	LDWU(r0, r1, _u16(i0));
	extr_s(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_s(r0, rn(reg));
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0))
	LDWU(r0, r1, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_us(r0, rn(reg));
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0))
	LDL(r0, r1, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_i(r0, rn(reg));
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0)) {
	LDL(r0, r1, _u16(i0));
	extr_ui(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_ui(r0, rn(reg));
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0))
	LDQ(r0, r1, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_l(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	STB(r0, _R31_REGNO, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_c(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	STW(r0, _R31_REGNO, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_s(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	STL(r0, _R31_REGNO, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_i(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (_s16_p(i0))
	STQ(r0, _R31_REGNO, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_l(rn(reg), r0);
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
    if (_s16_p(i0))
	STB(r1, r0, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_c(rn(reg), r1);
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0))
	STW(r1, r0, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_s(rn(reg), r1);
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0))
	STL(r1, r0, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_i(rn(reg), r1);
	jit_unget_reg(reg);
    }
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
    if (_s16_p(i0))
	STQ(r1, r0, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_l(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_extr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 56);
    rshi(r0, r0, 56);
}

static void
_extr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 56);
    rshi_u(r0, r0, 56);
}

static void
_extr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 48);
    rshi(r0, r0, 48);
}

static void
_extr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 48);
    rshi_u(r0, r0, 48);
}

static void
_extr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 32);
    rshi(r0, r0, 32);
}

static void
_extr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 32);
    rshi_u(r0, r0, 32);
}

static void
_htonr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    EXTBLi(r1, 0, rn(t0));
    EXTBLi(r1, 1, r0);
    SLLi(rn(t0), 8, rn(t0));
    OR(r0, rn(t0), r0);
    jit_unget_reg(t0);
}

static void
_htonr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;
    jit_int32_t		t3;
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr);
    t2 = jit_get_reg(jit_class_gpr);
    t3 = jit_get_reg(jit_class_gpr);
    EXTBLi(r1, 3, rn(t0));
    INSBLi(r1, 3, rn(t1));
    SLLi(r1, 8, rn(t2));
    ZAPNOTi(rn(t2), 4, rn(t2));
    SRLi(r1, 8, rn(t3));
    OR(rn(t0), rn(t1), r0);
    OR(rn(t2), r0, r0);
    ZAPNOTi(rn(t3), 2, rn(t3));
    OR(rn(t3), r0, r0);
    jit_unget_reg(t3);
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}

static void
_htonr_ul(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;
    assert(_jitc->function != NULL);
    t0 = jit_get_reg(jit_class_fpr);
    t1 = jit_get_reg(jit_class_gpr);
    t2 = jit_get_reg(jit_class_gpr);
    stxi(-8, _FP_REGNO, r1);			/* r1 = ABCD EFGH */
    LDG(rn(t0), _FP_REGNO, _u16(-8));		/* t0 = GHEF CDAB */
    STT(rn(t0), _FP_REGNO, _u16(-8));
    ldxi(rn(t1), _FP_REGNO, -8);		/* t1 = GHEF CDAB */
    lshi(rn(t2), rn(t1), 8);			/* t2 = HEFC DAB. */
    rshi_u(rn(t1), rn(t1), 8);			/* t1 = .GHE FCDA */
    ZAPi(rn(t2), 0x55, rn(t2));			/* t2 = H.F. D.B. */
    ZAPi(rn(t1), 0xaa, rn(t1));			/* t1 = .G.E .C.A */
    orr(r0, rn(t1), rn(t2));			/* r0 = HGFE DCBA */
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_word_t		d;
    w = _jit->pc.w;
    d = ((i0 - w) >> 2) - 1;
    if (_s21_p(d))
	BR(_R31_REGNO, d);
    else
	(void)jmpi_p(i0);
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    w = movi_p(rn(reg), i0);
    jmpr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_callr(jit_state_t *_jit, jit_int32_t r0)
{
    if (r0 != _PV_REGNO)
	MOV(r0, _PV_REGNO);
    JSR(_RA_REGNO, _PV_REGNO, 0);
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    /* FIXME use a small buffer to load constants - using gp */
#if 0
    jit_word_t		w;
    jit_word_t		d;
    w = _jit->pc.w;
    d = ((i0 - w) >> 2) - 1;
    if (_s21_p(d))
	BSR(_RA_REGNO, d);
    else
	(void)calli_p(i0);
#else
    movi(_PV_REGNO, i0);
    callr(_PV_REGNO);
#endif
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    w = movi_p(_PV_REGNO, i0);
    callr(_PV_REGNO);
    return (w);
}

static void
_prolog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg;
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
			       _jitc->function->self.aoff) + 7) & -8;
    /* ldgp gp, 0(pv) */
    LDAH(_PV_REGNO, _GP_REGNO, 0);
    LDA(_GP_REGNO, _GP_REGNO, 0);
    /* callee save registers */
    subi(_SP_REGNO, _SP_REGNO, stack_framesize);
    stxi(0, _SP_REGNO, _RA_REGNO);
    stxi(8, _SP_REGNO, _FP_REGNO);
#  define SPILL(N, O)							\
    if (jit_regset_tstbit(&_jitc->function->regset, N))			\
	stxi(O, _SP_REGNO, N##_REGNO)
#  define SPILLD(N, O)							\
    if (jit_regset_tstbit(&_jitc->function->regset, N))			\
	stxi_d(O, _SP_REGNO, N##_REGNO)
    SPILL(_S0, 16);
    SPILL(_S1, 24);
    SPILL(_S2, 32);
    SPILL(_S3, 40);
    SPILL(_S4, 48);
    SPILL(_S5, 56);
    SPILLD(_F2, 64);
    SPILLD(_F3, 72);
    SPILLD(_F4, 80);
    SPILLD(_F5, 88);
    SPILLD(_F6, 96);
    SPILLD(_F7, 104);
    SPILLD(_F8, 112);
    SPILLD(_F9, 120);
#  undef SPILLD
#  undef SPILL
    movr(_FP_REGNO, _SP_REGNO);
    /* alloca */
    if (_jitc->function->stack)
	subi(_SP_REGNO, _SP_REGNO, _jitc->function->stack);
    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
    }

    if (_jitc->function->self.call & jit_call_varargs) {
	for (reg = _jitc->function->self.argi; jit_arg_reg_p(reg); ++reg)
	    stxi(stack_framesize - 48 + reg * 8, _FP_REGNO, rn(_A0 - reg));
	for (reg = _jitc->function->self.argi; jit_arg_reg_p(reg); ++reg)
	    stxi_d(stack_framesize - 96 + reg * 8, _FP_REGNO, rn(_F16 - reg));
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    if (_jitc->function->assume_frame)
	return;
    movr(_SP_REGNO, _FP_REGNO);
    ldxi(_RA_REGNO, _SP_REGNO, 0);
    ldxi(_FP_REGNO, _SP_REGNO, 8);
#  define LOAD(N, O)							\
    if (jit_regset_tstbit(&_jitc->function->regset, N))			\
	ldxi(N##_REGNO, _SP_REGNO,  O)
#  define LOADD(N, O)							\
    if (jit_regset_tstbit(&_jitc->function->regset, N))			\
	ldxi_d(N##_REGNO, _SP_REGNO,  O)
    LOAD(_S0, 16);
    LOAD(_S1, 24);
    LOAD(_S2, 32);
    LOAD(_S3, 40);
    LOAD(_S4, 48);
    LOAD(_S5, 56);
    LOADD(_F2, 64);
    LOADD(_F3, 72);
    LOADD(_F4, 80);
    LOADD(_F5, 88);
    LOADD(_F6, 96);
    LOADD(_F7, 104);
    LOADD(_F8, 112);
    LOADD(_F9, 120);
#  undef LOADD
#  undef LOAD
    addi(_SP_REGNO, _SP_REGNO, stack_framesize);
    RET(_R31_REGNO, _RA_REGNO, 1);	/* 1 means procedure return
					 * 0 means no procedure return
					 * other values are reserved */
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		reg;

    /* Return jit_va_list_t in the register argument */
    addi(r0, _FP_REGNO, _jitc->function->vaoff);

    reg = jit_get_reg(jit_class_gpr);

    /* The base field is constant. */
    addi(rn(reg), _FP_REGNO, stack_framesize - 48);
    stxi(offsetof(jit_va_list_t, base), r0, rn(reg));

    /* Initialize the offset field */
    if (_jitc->function->vagp < 6)
	movi(rn(reg), _jitc->function->vagp * 8);
    else
	movi(rn(reg), _jitc->function->self.size - (stack_framesize - 48));
    stxi(offsetof(jit_va_list_t, offset), r0, rn(reg));

    jit_unget_reg(reg);
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		rg0, rg1;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg(jit_class_gpr);
    rg1 = jit_get_reg(jit_class_gpr);

    /* Load the base in first temporary. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, base));

    /* Load the offset in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, offset));

    /* Load the argument */
    ldxr(r0, rn(rg0), rn(rg1));

    /* No longer needed. */
    jit_unget_reg(rg0);

    /* Update offset. */
    addi(rn(rg1), rn(rg1), 8);
    stxi(offsetof(jit_va_list_t, offset), r1, rn(rg1));
    jit_unget_reg(rg1);
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    union {
	jit_int32_t	*i;
	jit_word_t	 w;
    } u;
    jit_word_t		 d;
    jit_int16_t		s0, s1, s2, s3;
    u.w = instr;
    switch (_u6(u.i[0] >> 26)) {
	/*   BLT	     BLE	     BEQ	     BGE */
	case 0x3a:	case 0x3b:	case 0x39:	case 0x3e:
	/*   BGT	     BNE	     BLBC	     BLBS */
	case 0x3f:	case 0x3d:	case 0x38:	case 0x3c:
	/*   BR		     BSR */
	case 0x30:	case 0x34:
	/*   FBLT	     FBLE	     FBEQ	     FBGE */
	case 0x32:	case 0x33:	case 0x31:	case 0x36:
	/*   FBGT	     FBNE */
	case 0x37:	case 0x35:
	    d = ((label - instr) >> 2) - 1;
	    assert(_s21_p(d));
	    u.i[0] &= ~0x1fffff;
	    u.i[0] |= _u21(d);
	    break;
	/*   LDA */
	case 0x08:		/* movi_p */
	    s0 = label;
	    s1 = label >> 16;
	    s2 = label >> 32;
	    s3 = label >> 48;
	    if (s0 < 0)
		++s1;
	    if (s2 < 0)
		++s3;
	    u.i[0] &= ~0xffff;
	    u.i[0] |= _u16(s0);
	    /*				LDA */
	    assert(_u6(u.i[1] >> 26) == 0x08);
	    u.i[1] &= ~0xffff;
	    u.i[1] |= _u16(s2);
	    /*				LDAH */
	    assert(_u6(u.i[2] >> 26) == 0x09);
	    u.i[2] &= ~0xffff;
	    u.i[2] |= _u16(s1);
	    /*				LDAH */
	    assert(_u6(u.i[3] >> 26) == 0x09);
	    u.i[3] &= ~0xffff;
	    u.i[3] |= _u16(s3);
	    /*				SLL */
	    assert(_u6(u.i[4] >> 26) == 0x12 && _u7(u.i[4] >> 5) == 0x39);
	    /*				SRL */
	    assert(_u6(u.i[5] >> 26) == 0x12 && _u7(u.i[5] >> 5) == 0x34);
	    /*				SLL */
	    assert(_u6(u.i[6] >> 26) == 0x12 && _u7(u.i[6] >> 5) == 0x39);
	    /*				BIS */
	    assert(_u6(u.i[7] >> 26) == 0x11 && _u7(u.i[7] >> 5) == 0x20);
	    break;
	default:
	    abort();
    }
}
#endif
