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
#  define _F2_REGNO			0x02
#  define _F3_REGNO			0x03
#  define _F4_REGNO			0x04
#  define _F5_REGNO			0x05
#  define _F6_REGNO			0x06
#  define _F7_REGNO			0x07
#  define _F8_REGNO			0x08
#  define _F9_REGNO			0x09
#  define F_P(o,ra,rb,f,rc)		_Opr(_jit,o,ra,rb,f,rc)
static void _Opr(jit_state_t*,int,int,int,unsigned int,int);
#  define ADDF(ra,rb,rc)		F_P(0x15,ra,rb,0x080,rc)
#  define ADDG(ra,rb,rc)		F_P(0x15,ra,rb,0x0a0,rc)
#  define ADDS(ra,rb,rc)		F_P(0x16,ra,rb,0x080,rc)
#  define ADDS_C(ra,rb,rc)		F_P(0x16,ra,rb,0x000,rc)
#  define ADDS_M(ra,rb,rc)		F_P(0x16,ra,rb,0x040,rc)
#  define ADDS_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0c0,rc)
#  define ADDS_U(ra,rb,rc)		F_P(0x16,ra,rb,0x180,rc)
#  define ADDS_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x100,rc)
#  define ADDS_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x140,rc)
#  define ADDS_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1c0,rc)
#  define ADDS_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x580,rc)
#  define ADDS_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x500,rc)
#  define ADDS_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x540,rc)
#  define ADDS_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5c0,rc)
#  define ADDS_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x780,rc)
#  define ADDS_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x700,rc)
#  define ADDS_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x740,rc)
#  define ADDS_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7c0,rc)
#  define ADDT(ra,rb,rc)		F_P(0x16,ra,rb,0x0a0,rc)
#  define ADDT_C(ra,rb,rc)		F_P(0x16,ra,rb,0x020,rc)
#  define ADDT_M(ra,rb,rc)		F_P(0x16,ra,rb,0x060,rc)
#  define ADDT_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0e0,rc)
#  define ADDT_U(ra,rb,rc)		F_P(0x16,ra,rb,0x1a0,rc)
#  define ADDT_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x120,rc)
#  define ADDT_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x160,rc)
#  define ADDT_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1e0,rc)
#  define ADDT_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a0,rc)
#  define ADDT_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x520,rc)
#  define ADDT_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x560,rc)
#  define ADDT_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5e0,rc)
#  define ADDT_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x7a0,rc)
#  define ADDT_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x720,rc)
#  define ADDT_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x760,rc)
#  define ADDT_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7e0,rc)
#  define CMPGEQ(ra,rb,rc)		F_P(0x15,ra,rb,0x0a5,rc)
#  define CMPGLE(ra,rb,rc)		F_P(0x15,ra,rb,0x0a7,rc)
#  define CMPTEQ(ra,rb,rc)		F_P(0x16,ra,rb,0x0a5,rc)
#  define CMPTEQ_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a5,rc)
#  define CMPTLE(ra,rb,rc)		F_P(0x16,ra,rb,0x0a7,rc)
#  define CMPTLE_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a7,rc)
#  define CMPTLT(ra,rb,rc)		F_P(0x16,ra,rb,0x0a6,rc)
#  define CMPTLT_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a6,rc)
#  define CMPTUN(ra,rb,rc)		F_P(0x16,ra,rb,0x0a4,rc)
#  define CMPTUN_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a4,rc)
#  define CPYS(ra,rb,rc)		F_P(0x17,ra,rb,0x020,rc)
#  define CPYSE(ra,rb,rc)		F_P(0x17,ra,rb,0x022,rc)
#  define CPYSN(ra,rb,rc)		F_P(0x17,ra,rb,0x021,rc)
#  define DIVF(ra,rb,rc)		F_P(0x15,ra,rb,0x083,rc)
#  define DIVG(ra,rb,rc)		F_P(0x15,ra,rb,0x0a3,rc)
#  define DIVS(ra,rb,rc)		F_P(0x16,ra,rb,0x083,rc)
#  define DIVS_C(ra,rb,rc)		F_P(0x16,ra,rb,0x003,rc)
#  define DIVS_M(ra,rb,rc)		F_P(0x16,ra,rb,0x043,rc)
#  define DIVS_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0c3,rc)
#  define DIVS_U(ra,rb,rc)		F_P(0x16,ra,rb,0x183,rc)
#  define DIVS_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x103,rc)
#  define DIVS_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x143,rc)
#  define DIVS_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1c3,rc)
#  define DIVS_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x583,rc)
#  define DIVS_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x503,rc)
#  define DIVS_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x543,rc)
#  define DIVS_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5c3,rc)
#  define DIVS_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x783,rc)
#  define DIVS_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x703,rc)
#  define DIVS_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x743,rc)
#  define DIVS_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7c3,rc)
#  define DIVT(ra,rb,rc)		F_P(0x16,ra,rb,0x0a3,rc)
#  define DIVT_C(ra,rb,rc)		F_P(0x16,ra,rb,0x023,rc)
#  define DIVT_M(ra,rb,rc)		F_P(0x16,ra,rb,0x063,rc)
#  define DIVT_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0e3,rc)
#  define DIVT_U(ra,rb,rc)		F_P(0x16,ra,rb,0x1a3,rc)
#  define DIVT_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x123,rc)
#  define DIVT_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x163,rc)
#  define DIVT_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1e3,rc)
#  define DIVT_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a3,rc)
#  define DIVT_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x523,rc)
#  define DIVT_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x563,rc)
#  define DIVT_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5e3,rc)
#  define DIVT_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x7a3,rc)
#  define DIVT_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x723,rc)
#  define DIVT_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x7a3,rc)
#  define DIVT_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7e3,rc)
#  define CVTDG(rb,rc)			F_P(0x15,_R31_REGNO,rb,0x09e,rc)
#  define CVTGD(rb,rc)			F_P(0x15,_R31_REGNO,rb,0x0ad,rc)
#  define CVTGF(rb,rc)			F_P(0x15,_R31_REGNO,rb,0x0ac,rc)
#  define CVTGQ(rb,rc)			F_P(0x15,_R31_REGNO,rb,0x0af,rc)
#  define CVTLQ(rb,rc)			F_P(0x17,_R31_REGNO,rb,0x010,rc)
#  define CVTQF(rb,rc)			F_P(0x15,_R31_REGNO,rb,0x0bc,rc)
#  define CVTQG(rb,rc)			F_P(0x15,_R31_REGNO,rb,0x0be,rc)
#  define CVTQL(rb,rc)			F_P(0x17,_R31_REGNO,rb,0x030,rc)
#  define CVTQS(rb,rc)			F_P(0x16,_R31_REGNO,rb,0x0bc,rc)
#  define CVTQS_C(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x03c,rc)
#  define CVTQS_M(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x07c,rc)
#  define CVTQS_D(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x0fc,rc)
#  define CVTQS_SUI(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x7bc,rc)
#  define CVTQS_SUIC(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x73c,rc)
#  define CVTQS_SUIM(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x77c,rc)
#  define CVTQS_SUID(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x7fc,rc)
#  define CVTQT(rb,rc)			F_P(0x16,_R31_REGNO,rb,0x0be,rc)
#  define CVTQT_C(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x03e,rc)
#  define CVTQT_M(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x0te,rc)
#  define CVTQT_D(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x0fe,rc)
#  define CVTQT_SUI(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x7be,rc)
#  define CVTQT_SUIC(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x73e,rc)
#  define CVTQT_SUIM(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x77e,rc)
#  define CVTQT_SUID(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x7fe,rc)
#  define CVTST(rb,rc)			F_P(0x16,_R31_REGNO,rb,0x2ac,rc)
#  define CVTST_S(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x6ac,rc)
#  define CVTTQ(rb,rc)			F_P(0x16,_R31_REGNO,rb,0x0af,rc)
#  define CVTTQ_C(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x02f,rc)
#  define CVTTQ_V(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x1af,rc)
#  define CVTTQ_VC(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x12f,rc)
#  define CVTTQ_SV(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x5af,rc)
#  define CVTTQ_SVC(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x52f,rc)
#  define CVTTQ_SVI(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x7af,rc)
#  define CVTTQ_SVIC(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x72f,rc)
#  define CVTTQ_D(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x0ef,rc)
#  define CVTTQ_VD(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x1ef,rc)
#  define CVTTQ_SVD(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x5ef,rc)
#  define CVTTQ_SVID(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x7ef,rc)
#  define CVTTQ_M(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x06f,rc)
#  define CVTTQ_VM(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x16f,rc)
#  define CVTTQ_SVM(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x56f,rc)
#  define CVTTQ_SVIM(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x76f,rc)
#  define CVTTS(rb,rc)			F_P(0x16,_R31_REGNO,rb,0x0ac,rc)
#  define CVTTS_C(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x02c,rc)
#  define CVTTS_M(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x06c,rc)
#  define CVTTS_D(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x0ec,rc)
#  define CVTTS_U(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x1ac,rc)
#  define CVTTS_UC(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x12c,rc)
#  define CVTTS_UM(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x16c,rc)
#  define CVTTS_UD(rb,rc)		F_P(0x16,_R31_REGNO,rb,0x1ec,rc)
#  define FBEQ(ra,d)			Bra(0x31,ra,d)
#  define FBGE(ra,d)			Bra(0x36,ra,d)
#  define FBGT(ra,d)			Bra(0x37,ra,d)
#  define FBLE(ra,d)			Bra(0x33,ra,d)
#  define FBLT(ra,d)			Bra(0x32,ra,d)
#  define FBNE(ra,d)			Bra(0x35,ra,d)
#  define FCMOVEQ(ra,rb,rc)		F_P(0x17,ra,rb,0x02a,rc)
#  define FCMOVGE(ra,rb,rc)		F_P(0x17,ra,rb,0x02d,rc)
#  define FCMOVGT(ra,rb,rc)		F_P(0x17,ra,rb,0x02f,rc)
#  define FCMOVLE(ra,rb,rc)		F_P(0x17,ra,rb,0x02e,rc)
#  define FCMOVLT(ra,rb,rc)		F_P(0x17,ra,rb,0x02c,rc)
#  define FCMOVNE(ra,rb,rc)		F_P(0x17,ra,rb,0x02b,rc)
#  define FTOIS(ra,rc)			F_P(0x1c,ra,_R31_REGNO,0x078,rc)
#  define FTOIT(ra,rc)			F_P(0x1c,ra,_R31_REGNO,0x070,rc)
#  define ITOFF(ra,rc)			F_P(0x14,ra,_R31_REGNO,0x014,rc)
#  define ITOFS(ra,rc)			F_P(0x14,ra,_R31_REGNO,0x004,rc)
#  define ITOFT(ra,rc)			F_P(0x14,ra,_R31_REGNO,0x024,rc)
#  define LDF(ra,rb,d)			Mem(0x20,ra,rb,d)
#  define LDG(ra,rb,d)			Mem(0x21,ra,rb,d)
#  define LDS(ra,rb,d)			Mem(0x22,ra,rb,d)
#  define LDT(ra,rb,d)			Mem(0x23,ra,rb,d)
#  define MF_FPCR(ra)			F_P(0x17,ra,ra,0x025,ra)
#  define MT_FPCR(ra)			F_P(0x17,ra,ra,0x024,ra)
#  define MULF(ra,rb,rc)		F_P(0x15,ra,rb,0x082,rc)
#  define MULG(ra,rb,rc)		F_P(0x15,ra,rb,0x0a2,rc)
#  define MULS(ra,rb,rc)		F_P(0x16,ra,rb,0x082,rc)
#  define MULS_C(ra,rb,rc)		F_P(0x16,ra,rb,0x002,rc)
#  define MULS_M(ra,rb,rc)		F_P(0x16,ra,rb,0x042,rc)
#  define MULS_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0c2,rc)
#  define MULS_U(ra,rb,rc)		F_P(0x16,ra,rb,0x182,rc)
#  define MULS_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x102,rc)
#  define MULS_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x142,rc)
#  define MULS_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1c2,rc)
#  define MULS_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x582,rc)
#  define MULS_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x502,rc)
#  define MULS_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x642,rc)
#  define MULS_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5c2,rc)
#  define MULS_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x782,rc)
#  define MULS_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x702,rc)
#  define MULS_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x742,rc)
#  define MULS_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7c2,rc)
#  define MULT(ra,rb,rc)		F_P(0x16,ra,rb,0x0a2,rc)
#  define MULT_C(ra,rb,rc)		F_P(0x16,ra,rb,0x022,rc)
#  define MULT_M(ra,rb,rc)		F_P(0x16,ra,rb,0x062,rc)
#  define MULT_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0e2,rc)
#  define MULT_U(ra,rb,rc)		F_P(0x16,ra,rb,0x1a2,rc)
#  define MULT_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x122,rc)
#  define MULT_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x162,rc)
#  define MULT_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1e2,rc)
#  define MULT_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a2,rc)
#  define MULT_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x522,rc)
#  define MULT_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x562,rc)
#  define MULT_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5e2,rc)
#  define MULT_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x7a2,rc)
#  define MULT_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x722,rc)
#  define MULT_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x762,rc)
#  define MULT_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7e2,rc)
#  define SQRTF(rb,rc)			F_P(0x14,_R31_REGNO,rb,0x08a,rc)
#  define SQRTG(rb,rc)			F_P(0x14,_R31_REGNO,rb,0x0aa,rc)
#  define SQRTS(rb,rc)			F_P(0x14,_R31_REGNO,rb,0x08b,rc)
#  define SQRTS_C(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x00b,rc)
#  define SQRTS_M(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x04b,rc)
#  define SQRTS_D(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x0cb,rc)
#  define SQRTS_U(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x18b,rc)
#  define SQRTS_UC(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x10b,rc)
#  define SQRTS_UM(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x14b,rc)
#  define SQRTS_UD(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x1cb,rc)
#  define SQRTS_SU(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x58b,rc)
#  define SQRTS_SUC(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x50b,rc)
#  define SQRTS_SUM(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x54b,rc)
#  define SQRTS_SUD(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x5cb,rc)
#  define SQRTS_SUI(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x78b,rc)
#  define SQRTS_SUIC(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x70b,rc)
#  define SQRTS_SUIM(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x74b,rc)
#  define SQRTS_SUID(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x7cb,rc)
#  define SQRTT(rb,rc)			F_P(0x14,_R31_REGNO,rb,0x0ab,rc)
#  define SQRTT_C(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x02b,rc)
#  define SQRTT_M(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x06b,rc)
#  define SQRTT_D(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x0eb,rc)
#  define SQRTT_U(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x1ab,rc)
#  define SQRTT_UC(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x12b,rc)
#  define SQRTT_UM(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x16b,rc)
#  define SQRTT_UD(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x1eb,rc)
#  define SQRTT_SU(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x5ab,rc)
#  define SQRTT_SUC(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x52b,rc)
#  define SQRTT_SUM(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x56b,rc)
#  define SQRTT_SUD(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x5eb,rc)
#  define SQRTT_SUI(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x7ab,rc)
#  define SQRTT_SUIC(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x72b,rc)
#  define SQRTT_SUIM(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x76b,rc)
#  define SQRTT_SUID(rb,rc)		F_P(0x14,_R31_REGNO,rb,0x7eb,rc)
#  define STF(ra,rb,d)			Mem(0x24,ra,rb,d)
#  define STG(ra,rb,d)			Mem(0x25,ra,rb,d)
#  define STS(ra,rb,d)			Mem(0x26,ra,rb,d)
#  define STT(ra,rb,d)			Mem(0x27,ra,rb,d)
#  define SUBF(ra,rb,rc)		F_P(0x15,ra,rb,0x081,rc)
#  define SUBG(ra,rb,rc)		F_P(0x15,ra,rb,0x0a1,rc)
#  define SUBS(ra,rb,rc)		F_P(0x16,ra,rb,0x081,rc)
#  define SUBS_C(ra,rb,rc)		F_P(0x16,ra,rb,0x001,rc)
#  define SUBS_M(ra,rb,rc)		F_P(0x16,ra,rb,0x041,rc)
#  define SUBS_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0c1,rc)
#  define SUBS_U(ra,rb,rc)		F_P(0x16,ra,rb,0x181,rc)
#  define SUBS_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x101,rc)
#  define SUBS_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x141,rc)
#  define SUBS_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1c1,rc)
#  define SUBS_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x581,rc)
#  define SUBS_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x501,rc)
#  define SUBS_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x541,rc)
#  define SUBS_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5c1,rc)
#  define SUBS_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x781,rc)
#  define SUBS_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x701,rc)
#  define SUBS_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x741,rc)
#  define SUBS_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7c1,rc)
#  define SUBT(ra,rb,rc)		F_P(0x16,ra,rb,0x0a1,rc)
#  define SUBT_C(ra,rb,rc)		F_P(0x16,ra,rb,0x021,rc)
#  define SUBT_M(ra,rb,rc)		F_P(0x16,ra,rb,0x061,rc)
#  define SUBT_D(ra,rb,rc)		F_P(0x16,ra,rb,0x0e1,rc)
#  define SUBT_U(ra,rb,rc)		F_P(0x16,ra,rb,0x1a1,rc)
#  define SUBT_UC(ra,rb,rc)		F_P(0x16,ra,rb,0x121,rc)
#  define SUBT_UM(ra,rb,rc)		F_P(0x16,ra,rb,0x161,rc)
#  define SUBT_UD(ra,rb,rc)		F_P(0x16,ra,rb,0x1e1,rc)
#  define SUBT_SU(ra,rb,rc)		F_P(0x16,ra,rb,0x5a1,rc)
#  define SUBT_SUC(ra,rb,rc)		F_P(0x16,ra,rb,0x521,rc)
#  define SUBT_SUM(ra,rb,rc)		F_P(0x16,ra,rb,0x561,rc)
#  define SUBT_SUD(ra,rb,rc)		F_P(0x16,ra,rb,0x5e1,rc)
#  define SUBT_SUI(ra,rb,rc)		F_P(0x16,ra,rb,0x7a1,rc)
#  define SUBT_SUIC(ra,rb,rc)		F_P(0x16,ra,rb,0x721,rc)
#  define SUBT_SUIM(ra,rb,rc)		F_P(0x16,ra,rb,0x761,rc)
#  define SUBT_SUID(ra,rb,rc)		F_P(0x16,ra,rb,0x7e1,rc)
#  define FABS(ra,rc)			CPYS(_R31_REGNO,ra,rc)
#  define FMOV(ra,rc)			CPYS(ra,ra,rc)
#  define NEGF(ra,rc)			SUBF(_R31_REGNO,ra,rc)
#  define NEGG(ra,rc)			SUBG(_R31_REGNO,ra,rc)
#  define NEGS(ra,rc)			SUBS(_R31_REGNO,ra,rc)
#  define NEGT(ra,rc)			SUBT(_R31_REGNO,ra,rc)
#  define FNEGF(ra,rc)			CPYSN(ra,ra,rc)
#  define FNEGG(ra,rc)			CPYSN(ra,ra,rc)
#  define FNEGS(ra,rc)			CPYSN(ra,ra,rc)
#  define FNEGT(ra,rc)			CPYSN(ra,ra,rc)
#  define movr_f(r0,r1)			movr_d(r0,r1)
#  define movr_d(r0,r1)			_movr_d(_jit,r0,r1)
static void _movr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_f(r0,i0)			_movi_f(_jit,r0,i0)
static void _movi_f(jit_state_t*,jit_int32_t,jit_float32_t*);
#  define movi_d(r0,i0)			_movi_d(_jit,r0,i0)
static void _movi_d(jit_state_t*,jit_int32_t,jit_float64_t*);
#  define absr_f(r0,r1)			FABS(r1,r0)
#  define absr_d(r0,r1)			FABS(r1,r0)
#  define negr_f(r0,r1)			FNEGS(r1,r0)
#  define negr_d(r0,r1)			FNEGT(r1,r0)
#  define sqrtr_f(r0,r1)		_sqrtr_f(_jit,r0,r1)
static void _sqrtr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sqrtr_d(r0,r1)		_sqrtr_d(_jit,r0,r1)
static void _sqrtr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_f_d(r0,r1)		movr_d(r0,r1)
#  define extr_d_f(r0,r1)		movr_f(r0,r1)
#  define truncr_f_i(r0,r1)		truncr_d_i(r0,r1)
#  define truncr_f_l(r0,r1)		truncr_d_l(r0,r1)
#  define truncr_d_i(r0,r1)		truncr_d_l(r0,r1)
#  define truncr_d_l(r0,r1)		_truncr_d_l(_jit,r0,r1)
static void _truncr_d_l(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_f(r0,r1)			_extr_f(_jit,r0,r1)
static void _extr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_d(r0,r1)			_extr_d(_jit,r0,r1)
static void _extr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define addr_f(r0,r1,r2)		_addr_f(_jit,r0,r1,r2)
static void _addr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addi_f(r0,r1,i0)		_addi_f(_jit,r0,r1,i0)
static void _addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define addr_d(r0,r1,r2)		_addr_d(_jit,r0,r1,r2)
static void _addr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addi_d(r0,r1,i0)		_addi_d(_jit,r0,r1,i0)
static void _addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define subr_f(r0,r1,r2)		_subr_f(_jit,r0,r1,r2)
static void _subr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subi_f(r0,r1,i0)		_subi_f(_jit,r0,r1,i0)
static void _subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define subr_d(r0,r1,r2)		_subr_d(_jit,r0,r1,r2)
static void _subr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subi_d(r0,r1,i0)		_subi_d(_jit,r0,r1,i0)
static void _subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define rsbr_f(r0, r1, r2)		subr_f(r0, r2, r1)
#  define rsbi_f(r0, r1, i0)		_rsbi_f(_jit, r0, r1, i0)
static void _rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define rsbr_d(r0, r1, r2)		subr_d(r0, r2, r1)
#  define rsbi_d(r0, r1, i0)		_rsbi_d(_jit, r0, r1, i0)
static void _rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define mulr_f(r0,r1,r2)		_mulr_f(_jit,r0,r1,r2)
static void _mulr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define muli_f(r0,r1,i0)		_muli_f(_jit,r0,r1,i0)
static void _muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define mulr_d(r0,r1,r2)		_mulr_d(_jit,r0,r1,r2)
static void _mulr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define muli_d(r0,r1,i0)		_muli_d(_jit,r0,r1,i0)
static void _muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define divr_f(r0,r1,r2)		_divr_f(_jit,r0,r1,r2)
static void _divr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi_f(r0,r1,i0)		_divi_f(_jit,r0,r1,i0)
static void _divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define divr_d(r0,r1,r2)		_divr_d(_jit,r0,r1,r2)
static void _divr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
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
#  define ldr_f(r0,r1)			LDS(r0,r1,0)
#  define ldi_f(r0,i0)			_ldi_f(_jit,r0,i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_d(r0,r1)			LDT(r0,r1,0)
#  define ldi_d(r0,i0)			_ldi_d(_jit,r0,i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_f(r0,r1,r2)		_ldxr_f(_jit,r0,r1,r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_f(r0,r1,i0)		_ldxi_f(_jit,r0,r1,i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_d(r0,r1,r2)		_ldxr_d(_jit,r0,r1,r2)
static void _ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_d(r0,r1,i0)		_ldxi_d(_jit,r0,r1,i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_f(r0,r1)			STS(r1,r0,0)
#  define sti_f(i0,r0)			_sti_f(_jit,i0,r0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_d(r0,r1)			STT(r1,r0,0)
#  define sti_d(i0,r0)			_sti_d(_jit,i0,r0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_f(r0,r1,r2)		_stxr_f(_jit,r0,r1,r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_f(i0,r0,r1)		_stxi_f(_jit,i0,r0,r1)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_d(r0,r1,r2)		_stxr_d(_jit,r0,r1,r2)
static void _stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_d(i0,r0,r1)		_stxi_d(_jit,i0,r0,r1)
static void _stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
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
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|jit_class_nospill);\
    movi_##type(rn(reg), i1);						\
    word = b##name##r_##type(i0, r0, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define fopi(name)			fpr_opi(name, f, 32)
#  define fbopi(name)			fpr_bopi(name, f, 32)
#  define dopi(name)			fpr_opi(name, d, 64)
#  define dbopi(name)			fpr_bopi(name, d, 64)

static void
_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	FMOV(r1, r0);
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
	stxi_i(-8, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
	ldxi_f(r0, _FP_REGNO, -8);
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
	stxi_l(-8, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
	ldxi_d(r0, _FP_REGNO, -8);
    }
    else
	ldi_d(r0, (jit_word_t)i0);
}

static void
_truncr_d_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    CVTTQ_SVC(r1, rn(reg));
    TRAPB();
    stxi_d(-8, _FP_REGNO, rn(reg));
    ldxi(r0, _FP_REGNO, -8);
    jit_unget_reg(reg);
}

static void
_sqrtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    SQRTS_SU(r1, r0);
    TRAPB();
}

static void
_sqrtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    SQRTT_SU(r1, r0);
    TRAPB();
}

static void
_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    stxi_l(-8, _FP_REGNO, r1);
    ldxi_d(r0, _FP_REGNO, -8);
    CVTQS(r0, r0);
}

static void
_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    stxi_l(-8, _FP_REGNO, r1);
    ldxi_d(r0, _FP_REGNO, -8);
    CVTQT(r0, r0);
}

static void
_addr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    ADDS_SU(r1, r2, r0);
    TRAPB();
}
fopi(add)

static void
_addr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    ADDT_SU(r1, r2, r0);
    TRAPB();
}
dopi(add)

static void
_subr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SUBS_SU(r1, r2, r0);
    TRAPB();
}
fopi(sub)
fopi(rsb)

static void
_subr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SUBT_SU(r1, r2, r0);
    TRAPB();
}
dopi(sub)
dopi(rsb)

static void
_mulr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MULS_SU(r1, r2, r0);
    TRAPB();
}
fopi(mul)

static void
_mulr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    MULT_SU(r1, r2, r0);
    TRAPB();
}
dopi(mul)

static void
_divr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    DIVS_SU(r1, r2, r0);
    TRAPB();
}
fopi(div)

static void
_divr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    DIVT_SU(r1, r2, r0);
    TRAPB();
}
dopi(div)

static void
_ltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 0);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLT_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), 1);
    movi(r0, 1);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(lt);
dopi(lt);

static void
_ler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 0);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLE_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), 1);
    movi(r0, 1);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(le);
dopi(le);

static void
_eqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 0);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTEQ_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), 1);
    movi(r0, 1);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(eq);
dopi(eq);

static void
_ger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 0);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLT_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    movi(r0, 1);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(ge);
dopi(ge);

static void
_gtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 0);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLE_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    movi(r0, 1);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(gt);
dopi(gt);

static void
_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 1);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 3);
    CMPTEQ_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), 1);
    movi(r0, 0);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(ne);
dopi(ne);

static void
_unltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 1);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLT_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    movi(r0, 0);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(unlt);
dopi(unlt);

static void
_unler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 1);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLE_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    movi(r0, 0);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(unle);
dopi(unle);

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 1);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTEQ_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    movi(r0, 0);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(uneq);
dopi(uneq);

static void
_unger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 1);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLT_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), 1);
    movi(r0, 0);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(unge);
dopi(unge);

static void
_ungtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 1);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLE_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), 1);
    movi(r0, 0);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(ungt);
dopi(ungt);

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 0);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTEQ_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    movi(r0, 1);
    patch_at(v, _jit->pc.w);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(ltgt);
dopi(ltgt);

static void
_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 0);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    movi(r0, 1);
    patch_at(w, _jit->pc.w);
    jit_unget_reg(reg);
}
fopi(ord);
dopi(ord);

static void
_unordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    movi(r0, 1);
    CMPTUN_SU(r1, r2, rn(reg));
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), 1);
    jit_unget_reg(reg);
    movi(r0, 0);
    patch_at(w, _jit->pc.w);
}
fopi(unord);
dopi(unord);

static jit_word_t
_bltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord does not satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 3);
    CMPTLT_SU(r0, r1, rn(reg));		/* lt satisfy condition */
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(lt);
dbopi(lt);

static jit_word_t
_bler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord does not satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 3);
    CMPTLE_SU(r0, r1, rn(reg));		/* le satisfy condition */
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(le);
dbopi(le);

static jit_word_t
_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord does not satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 3);
    CMPTEQ_SU(r0, r1, rn(reg));		/* eq satisfy condition */
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(eq);
dbopi(eq);

static jit_word_t
_bger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord does not satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 3);
    CMPTLT_SU(r0, r1, rn(reg));		/* ge satisfy condition */
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ge);
dbopi(ge);

static jit_word_t
_bgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord does not satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 3);
    CMPTLE_SU(r0, r1, rn(reg));		/* gt satisfy condition */
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(gt);
dbopi(gt);

static jit_word_t
_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		u, v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord satisfy condition */
    TRAPB();
    u = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTEQ_SU(r0, r1, rn(reg));		/* ne satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 1);
    patch_at(u, _jit->pc.w);
    w = _jit->pc.w;
    BR(_R31_REGNO, ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ne);
dbopi(ne);

static jit_word_t
_bunltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		u, v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord satisfy condition */
    TRAPB();
    u = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLT_SU(r0, r1, rn(reg));		/* lt satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBEQ(rn(reg), 1);
    w = _jit->pc.w;
    patch_at(u, _jit->pc.w);
    BR(_R31_REGNO, ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unlt);
dbopi(unlt);

static jit_word_t
_bunler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		u, v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord satisfy condition */
    TRAPB();
    u = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLE_SU(r0, r1, rn(reg));		/* le satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBEQ(rn(reg), 1);
    w = _jit->pc.w;
    patch_at(u, _jit->pc.w);
    BR(_R31_REGNO, ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unle);
dbopi(unle);

static jit_word_t
_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		u, v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord satisfy condition */
    TRAPB();
    u = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTEQ_SU(r0, r1, rn(reg));		/* eq satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBEQ(rn(reg), 1);
    w = _jit->pc.w;
    patch_at(u, _jit->pc.w);
    BR(_R31_REGNO, ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(uneq);
dbopi(uneq);

static jit_word_t
_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		u, v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord satisfy condition */
    TRAPB();
    u = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLT_SU(r0, r1, rn(reg));		/* ge satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 1);
    w = _jit->pc.w;
    patch_at(u, _jit->pc.w);
    BR(_R31_REGNO, ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unge);
dbopi(unge);

static jit_word_t
_bungtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_word_t		u, v, w;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord satisfy condition */
    TRAPB();
    u = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTLE_SU(r0, r1, rn(reg));		/* gt does satisfy condition */
    TRAPB();
    v = _jit->pc.w;
    FBNE(rn(reg), 1);
    w = _jit->pc.w;
    patch_at(u, _jit->pc.w);
    BR(_R31_REGNO, ((i0 - w) >> 2) - 1);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ungt);
dbopi(ungt);

static jit_word_t
_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		u, v, w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord does not satisfy condition */
    TRAPB();
    u = _jit->pc.w;
    FBNE(rn(reg), 4);
    CMPTEQ_SU(r1, r0, rn(reg));
    TRAPB();
    v = _jit->pc.w;			/* eq does not satisfy condition */
    FBNE(rn(reg), 1);
    w = _jit->pc.w;
    BR(_R31_REGNO, ((i0 - w) >> 2) - 1);
    patch_at(u, _jit->pc.w);
    patch_at(v, _jit->pc.w);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ltgt);
dbopi(ltgt);

static jit_word_t
_bordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord does not satisfy condition */
    TRAPB();
    w = _jit->pc.w;
    FBEQ(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ord);
dbopi(ord);

static jit_word_t
_bunordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr|jit_class_nospill);
    CMPTUN_SU(r0, r1, rn(reg));		/* unord satisfy condition */
    TRAPB();
    w = _jit->pc.w;
    FBNE(rn(reg), ((i0 - w) >> 2) - 1);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unord);
dbopi(unord);

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		reg;
    if (_s16_p(i0))
	LDS(r0, _R31_REGNO, _u16(i0));
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
    jit_word_t		reg;
    if (_s16_p(i0))
	LDT(r0, _R31_REGNO, _u16(i0));
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
    jit_word_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_f(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_word_t		reg;
    if (_s16_p(i0))
	LDS(r0, r1, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_d(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_word_t		reg;
    if (_s16_p(i0))
	LDT(r0, r1, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_word_t		reg;
    if (_s16_p(i0))
	STS(r0, _R31_REGNO, _u16(i0));
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
    jit_word_t		reg;
    if (_s16_p(i0))
	STT(r0, _R31_REGNO, _u16(i0));
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
    jit_word_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_f(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		reg;
    if (_s16_p(i0))
	STS(r1, r0, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_f(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_d(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		reg;
    if (_s16_p(i0))
	STT(r1, r0, _u16(i0));
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_d(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		ge_code;
    jit_int32_t		rg0, rg1, rg2;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg(jit_class_gpr);
    rg1 = jit_get_reg(jit_class_gpr);
    rg2 = jit_get_reg(jit_class_gpr);

    /* Load the base in first temporary. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, base));

    /* Load the offset in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, offset));

    /* Remember absolute offset */
    movr(rn(rg2), rn(rg1));

    /* Jump if overflowed register saved area. */
    ge_code = bgei(_jit->pc.w, rn(rg1), 48);
    /* Otherwise load from the float registers save area. */
    subi(rn(rg1), rn(rg1), 48);
    patch_at(ge_code, _jit->pc.w);

    /* Load the argument */
    ldxr_d(r0, rn(rg0), rn(rg1));

    /* No longer needed. */
    jit_unget_reg(rg1);
    jit_unget_reg(rg0);

    /* Update offset. */
    addi(rn(rg2), rn(rg2), 8);
    stxi(offsetof(jit_va_list_t, offset), r1, rn(rg2));
    jit_unget_reg(rg2);
}
#endif
