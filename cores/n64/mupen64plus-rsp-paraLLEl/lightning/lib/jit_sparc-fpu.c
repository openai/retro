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
#    define FPR(r)			(r)
#    define CLASS_SNG			jit_class_fpr
#    define CLASS_DBL			jit_class_fpr
#  else
#    define single_precision_p(r)	((r) >= 0 && (r) <= 31)
#    define FPR(r)			((r) > 31 ? (r) - 31 : (r))
#    define CLASS_SNG			(jit_class_fpr | jit_class_sng)
#    define CLASS_DBL			(jit_class_fpr | jit_class_dbl)
#  endif
#  define LDF(rs1, rs2, rd)		f3r(3, FPR(rd), 32, FPR(rs1), FPR(rs2))
#  define LDFI(rs1, imm, rd)		f3i(3, FPR(rd), 32, FPR(rs1), imm)
#  define LDDF(rs1, rs2, rd)		f3r(3, FPR(rd), 35, FPR(rs1), FPR(rs2))
#  define LDDFI(rs1, imm, rd)		f3i(3, FPR(rd), 35, FPR(rs1), imm)
#  define LDFSR(rs1, rs2, rd)		f3r(3, FPR(rd), 33, FPR(rs1), FPR(rs2))
#  define LDFSRI(rs1, imm, rd)		f3i(3, FPR(rd), 33, FPR(rs1), imm)
#  define STF(rd, rs1, rs2)		f3r(3, FPR(rd), 36, FPR(rs1), FPR(rs2))
#  define STFI(rd, rs1, imm)		f3i(3, FPR(rd), 36, FPR(rs1), imm)
#  define STDF(rd, rs1, rs2)		f3r(3, FPR(rd), 39, FPR(rs1), FPR(rs2))
#  define STDFI(rd, rs1, imm)		f3i(3, FPR(rd), 39, FPR(rs1), imm)
#  define STFSR(rd, rs1, rs2)		f3r(3, FPR(rd), 37, FPR(rs1), FPR(rs2))
#  define STFSRI(rd, rs1, imm)		f3i(3, FPR(rd), 37, FPR(rs1), imm)
#  define STDFQ(rd, rs1, rs2)		f3r(3, FPR(rd), 38, FPR(rs1), FPR(rs2))
#  define STFDFQ(rd, rs1, imm)		f3i(3, FPR(rd), 38, FPR(rs1), imm)
#  define SPARC_FBA			8	/* always - 1 */
#  define SPARC_FBN			0	/* never - 0 */
#  define SPARC_FBU			7	/* unordered - U */
#  define SPARC_FBG			6	/* greater - G */
#  define SPARC_FBUG			5	/* unordered or greater - G or U */
#  define SPARC_FBL			4	/* less - L */
#  define SPARC_FBUL			3	/* unordered or less - L or U */
#  define SPARC_FBLG			2	/* less or greater - L or G */
#  define SPARC_FBNE			1	/* not equal - L or G or U */
#  define SPARC_FBNZ			SPARC_FBNE
#  define SPARC_FBE			9	/* equal - E */
#  define SPARC_FBZ			SPARC_FBE
#  define SPARC_FBUE			10	/* unordered or equal - E or U */
#  define SPARC_FBGE			11	/* greater or equal - E or G */
#  define SPARC_FBUGE			12	/* unordered or greater or equal - E or G or U */
#  define SPARC_FBLE			13	/* less or equal - E or L */
#  define SPARC_FBULE			14	/* unordered or less or equal - E or L or U */
#  define SPARC_FBO			15	/* ordered - E or L or G */
#  define FB(cc, imm)			f2b(0, 0, cc, 6, imm)
#  define FBa(cc, imm)			f2b(0, 1, cc, 6, imm)
#  define FBA(imm)			FB(SPARC_FBA, imm)
#  define FBAa(imm)			FBa(SPARC_FBA, imm)
#  define FBN(imm)			FB(SPARC_FBN, imm)
#  define FBNa(imm)			FBa(SPARC_FBN, imm)
#  define FBU(imm)			FB(SPARC_FBU, imm)
#  define FBUa(imm)			FBa(SPARC_FBU, imm)
#  define FBG(imm)			FB(SPARC_FBG, imm)
#  define FBGa(imm)			FBa(SPARC_FBG, imm)
#  define FBUG(imm)			FB(SPARC_FBUG, imm)
#  define FBUGa(imm)			FBa(SPARC_FBUG, imm)
#  define FBL(imm)			FB(SPARC_FBL, imm)
#  define FBLa(imm)			FBa(SPARC_FBL, imm)
#  define FBUL(imm)			FB(SPARC_FBUL, imm)
#  define FBULa(imm)			FBa(SPARC_FBUL, imm)
#  define FBLG(imm)			FB(SPARC_FBLG, imm)
#  define FBLGa(imm)			FBa(SPARC_FBLG, imm)
#  define FBNE(imm)			FB(SPARC_FBNE, imm)
#  define FBNEa(imm)			FBa(SPARC_FBNE, imm)
#  define FBE(imm)			FB(SPARC_FBE, imm)
#  define FBEa(imm)			FBa(SPARC_FBE, imm)
#  define FBUE(imm)			FB(SPARC_FBUE, imm)
#  define FBUEa(imm)			FBa(SPARC_FBUE, imm)
#  define FBLE(imm)			FB(SPARC_FBLE, imm)
#  define FBLEa(imm)			FBa(SPARC_FBLE, imm)
#  define FBO(imm)			FB(SPARC_FBO, imm)
#  define FBOa(imm)			FBa(SPARC_FBO, imm)
#  define FPop1(rd, rs1, opf, rs2)	f3f(rd, 52, rs1, opf, rs2)
#  define FPop2(rd, rs1, opf, rs2)	f3f(rd, 53, rs1, opf, rs2)
#  define f3f(rd, op3, rs1, opf, rs2)	_f3f(_jit, rd, op3, rs1, opf, rs2)
static void
_f3f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t, jit_int32_t,jit_int32_t);
#  define FITOS(rs2, rd)		FPop1(rd, 0, 196, rs2)
#  define FITOD(rs2, rd)		FPop1(rd, 0, 200, rs2)
#  define FITOQ(rs2, rd)		FPop1(rd, 0, 204, rs2)
#  if __WORDSIZE == 64
#    define FXTOS(rs2, rd)		FPop1(rd, 0, 132, rs2)
#    define FXTOD(rs2, rd)		FPop1(rd, 0, 136, rs2)
#    define FxTOQ(rs2, rd)		FPop1(rd, 0, 140, rs2)
#  endif
#  define FSTOI(rs2, rd)		FPop1(rd, 0, 209, rs2)
#  define FDTOI(rs2, rd)		FPop1(rd, 0, 210, rs2)
#  define FQTOI(rs2, rd)		FPop1(rd, 0, 211, rs2)
#  define FSTOX(rs2, rd)		FPop1(rd, 0, 129, rs2)
#  define FDTOX(rs2, rd)		FPop1(rd, 0, 130, rs2)
#  define FQTOX(rs2, rd)		FPop1(rd, 0, 131, rs2)
#  define FSTOD(rs2, rd)		FPop1(rd, 0, 201, rs2)
#  define FSTOQ(rs2, rd)		FPop1(rd, 0, 205, rs2)
#  define FDTOS(rs2, rd)		FPop1(rd, 0, 198, rs2)
#  define FDTOQ(rs2, rd)		FPop1(rd, 0, 206, rs2)
#  define FQTOS(rs2, rd)		FPop1(rd, 0, 199, rs2)
#  define FQTOD(rs2, rd)		FPop1(rd, 0, 203, rs2)
#  define FMOVS(rs2, rd)		FPop1(rd, 0,   1, rs2)
#  define FMOVD(rs2, rd)		FPop1(rd, 0,   2, rs2)
#  define FMOVQ(rs2, rd)		FPop1(rd, 0,   3, rs2)
#  define FNEGS(rs2, rd)		FPop1(rd, 0,   5, rs2)
#  define FNEGD(rs2, rd)		FPop1(rd, 0,   6, rs2)
#  define FNEGQ(rs2, rd)		FPop1(rd, 0,   7, rs2)
#  define FABSS(rs2, rd)		FPop1(rd, 0,   9, rs2)
#  define FABSD(rs2, rd)		FPop1(rd, 0,  10, rs2)
#  define FABSQ(rs2, rd)		FPop1(rd, 0,  11, rs2)
#  define FSQRTS(rs2, rd)		FPop1(rd, 0,  41, rs2)
#  define FSQRTD(rs2, rd)		FPop1(rd, 0,  42, rs2)
#  define FSQRTQ(rs2, rd)		FPop1(rd, 0,  43, rs2)
#  define SPARC_FADDS			65
#  define SPARC_FADDD			66
#  define SPARC_FADDQ			67
#  define SPARC_FSUBS			69
#  define SPARC_FSUBD			70
#  define SPARC_FSUBQ			71
#  define SPARC_FMULS			73
#  define SPARC_FMULD			74
#  define SPARC_FMULQ			75
#  define SPARC_FSMULD			105
#  define SPARC_FDMULQ			110
#  define SPARC_FDIVS			77
#  define SPARC_FDIVD			78
#  define SPARC_FDIVQ			79
#  define FADDS(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FADDS, rs2)
#  define FADDD(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FADDD, rs2)
#  define FADDQ(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FADDQ, rs2)
#  define FSUBS(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FSUBS, rs2)
#  define FSUBD(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FSUBD, rs2)
#  define FSUBQ(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FSUBQ, rs2)
#  define FMULS(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FMULS, rs2)
#  define FMULD(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FMULD, rs2)
#  define FMULQ(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FMULQ, rs2)
#  define FSMULD(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FSMULD, rs2)
#  define FDMULQ(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FDMULQ, rs2)
#  define FDIVS(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FDIVS, rs2)
#  define FDIVD(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FDIVD, rs2)
#  define FDIVQ(rs1, rs2, rd)		FPop1(rd, rs1,  SPARC_FDIVQ, rs2)
#  define SPARC_FCMPS			81
#  define SPARC_FCMPD			82
#  define SPARC_FCMPQ			83
#  define SPARC_FCMPES			85
#  define SPARC_FCMPED			86
#  define SPARC_FCMPEQ			87
#  define FCMPS(rs1, rs2)		FPop2(0, rs1, SPARC_FCMPS, rs2)
#  define FCMPD(rs1, rs2)		FPop2(0, rs1, SPARC_FCMPD, rs2)
#  define FCMPQ(rs1, rs2)		FPop2(0, rs1, SPARC_FCMPQ, rs2)
#  define FCMPES(rs1, rs2)		FPop2(0, rs1, SPARC_FCMPES, rs2)
#  define FCMPED(rs1, rs2)		FPop2(0, rs1, SPARC_FCMPED, rs2)
#  define FCMPEQ(rs1, rs2)		FPop2(0, rs1, SPARC_FCMPEQ, rs2)
#  define CPop1(rd, rs1, opc, rs2)	f3f(rd, 54, rs1, opf, rs2)
#  define CPop2(rd, rs1, opc, rs2)	f3f(rd, 55, rs1, opf, rs2)
#  define extr_f(r0, r1)		_extr_f(_jit, r0, r1)
static void _extr_f(jit_state_t*, jit_int32_t, jit_int32_t);
#  if __WORDSIZSE == 32
#    define truncr_f(r0, r1)		truncr_f_i(r0, r1)
#  define truncr_d(r0, r1)		truncr_d_i(r0, r1)
#  else
#    define truncr_f(r0, r1)		truncr_f_l(r0, r1)
#  define truncr_d(r0, r1)		truncr_d_l(r0, r1)
#  endif
#  define truncr_f_i(r0, r1)		_truncr_f_i(_jit, r0, r1)
static void _truncr_f_i(jit_state_t*, jit_int32_t, jit_int32_t);
#  if __WORDSIZE == 64
#    define truncr_f_l(r0, r1)		_truncr_f_l(_jit, r0, r1)
static void _truncr_f_l(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  if __WORDSIZE == 32
#    define extr_d_f(r0, r1)		FDTOS(r1, r0)
#  else
#    define extr_d_f(r0, r1)		_extr_d_f(_jit, r0, r1)
static void _extr_d_f(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  define movi_f(r0, i0)		_movi_f(_jit, r0, i0)
#  if __WORDSIZE == 32
#    define movr_f(r0, r1)		FMOVS(r1, r0)
#  else
#    define movr_f(r0, r1)		_movr_f(_jit, r0, r1)
static void _movr_f(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
static void _movi_f(jit_state_t*, jit_int32_t, jit_float32_t*);
#  if __WORDSIZE == 32
#    define negr_f(r0, r1)		FNEGS(r1, r0)
#    define absr_f(r0, r1)		FABSS(r1, r0)
#    define sqrtr_f(r0, r1)		FSQRTS(r1, r0)
#  else
#    define negr_f(r0, r1)		_negr_f(_jit, r0, r1)
static void _negr_f(jit_state_t*, jit_int32_t, jit_int32_t);
#    define absr_f(r0, r1)		_absr_f(_jit, r0, r1)
static void _absr_f(jit_state_t*, jit_int32_t, jit_int32_t);
#    define sqrtr_f(r0, r1)		_sqrtr_f(_jit, r0, r1)
static void _sqrtr_f(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  define extr_d(r0, r1)		_extr_d(_jit, r0, r1)
static void _extr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define truncr_d_i(r0, r1)		_truncr_d_i(_jit, r0, r1)
static void _truncr_d_i(jit_state_t*, jit_int32_t, jit_int32_t);
#  if __WORDSIZE == 64
#    define truncr_d_l(r0, r1)		_truncr_d_l(_jit, r0, r1)
static void _truncr_d_l(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  if __WORDSIZE == 32
#    define extr_f_d(r0, r1)		FSTOD(r1, r0)
#  else
#    define extr_f_d(r0, r1)		_extr_f_d(_jit, r0, r1)
static void _extr_f_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  define movi_d(r0, i0)		_movi_d(_jit, r0, i0)
static void _movi_d(jit_state_t*, jit_int32_t, jit_float64_t*);
#  if __WORDSIZE == 32
#  define movr_d(r0, r1)		_movr_d(_jit, r0, r1)
static void _movr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define negr_d(r0, r1)		_negr_d(_jit, r0, r1)
static void _negr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  define absr_d(r0, r1)		_absr_d(_jit, r0, r1)
static void _absr_d(jit_state_t*, jit_int32_t, jit_int32_t);
#  else
#    define movr_d(r0, r1)		FMOVD(r1, r0)
#    define negr_d(r0, r1)		FNEGD(r1, r0)
#    define absr_d(r0, r1)		FABSD(r1, r0)
#  endif
#  define sqrtr_d(r0, r1)		FSQRTD(r1, r0)
#  define fop1f(op, r0, r1, i0)		_fop1f(_jit, op, r0, r1, i0)
static void _fop1f(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_float32_t*);
#  define rfop1f(op, r0, r1, i0)	_rfop1f(_jit, op, r0, r1, i0)
static void _rfop1f(jit_state_t*,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_float32_t*);
#  define fop1d(op, r0, r1, i0)		_fop1d(_jit, op, r0, r1, i0)
static void _fop1d(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_float64_t*);
#  define rfop1d(op, r0, r1, i0)	_rfop1d(_jit, op, r0, r1, i0)
static void _rfop1d(jit_state_t*,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_float64_t*);
#  if __WORDSIZE == 32
#    define addr_f(r0, r1, r2)		FADDS(r1, r2, r0)
#    define subr_f(r0, r1, r2)		FSUBS(r1, r2, r0)
#    define mulr_f(r0, r1, r2)		FMULS(r1, r2, r0)
#    define divr_f(r0, r1, r2)		FDIVS(r1, r2, r0)
#  else
#    define fop2f(op, r0, r1, r2)	_fop2f(_jit, op, r0, r1, r2)
static void _fop2f(jit_state_t*, jit_int32_t,
		   jit_int32_t, jit_int32_t, jit_int32_t);
#    define addr_f(r0, r1, r2)		fop2f(SPARC_FADDS, r0, r1, r2)
#    define subr_f(r0, r1, r2)		fop2f(SPARC_FSUBS, r0, r1, r2)
#    define mulr_f(r0, r1, r2)		fop2f(SPARC_FMULS, r0, r1, r2)
#    define divr_f(r0, r1, r2)		fop2f(SPARC_FDIVS, r0, r1, r2)
#  endif
#  define addi_f(r0, r1, i0)		fop1f(SPARC_FADDS, r0, r1, i0)
#  define subi_f(r0, r1, i0)		fop1f(SPARC_FSUBS, r0, r1, i0)
#  define rsbr_f(r0, r1, r2)		subr_f(r0, r2, r1)
#  define rsbi_f(r0, r1, i0)		rfop1f(SPARC_FSUBS, r0, r1, i0)
#  define rsbr_d(r0, r1, r2)		subr_d(r0, r2, r1)
#  define rsbi_d(r0, r1, i0)		rfop1d(SPARC_FSUBD, r0, r1, i0)
#  define muli_f(r0, r1, i0)		fop1f(SPARC_FMULS, r0, r1, i0)
#  define divi_f(r0, r1, i0)		fop1f(SPARC_FDIVS, r0, r1, i0)
#  define addr_d(r0, r1, r2)		FADDD(r1, r2, r0)
#  define addi_d(r0, r1, i0)		fop1d(SPARC_FADDD, r0, r1, i0)
#  define subr_d(r0, r1, r2)		FSUBD(r1, r2, r0)
#  define subi_d(r0, r1, i0)		fop1d(SPARC_FSUBD, r0, r1, i0)
#  define rsbr_d(r0, r1, r2)		subr_d(r0, r2, r1)
#  define rsbi_d(r0, r1, i0)		rfop1d(SPARC_FSUBD, r0, r1, i0)
#  define mulr_d(r0, r1, r2)		FMULD(r1, r2, r0)
#  define muli_d(r0, r1, i0)		fop1d(SPARC_FMULD, r0, r1, i0)
#  define divr_d(r0, r1, r2)		FDIVD(r1, r2, r0)
#  define divi_d(r0, r1, i0)		fop1d(SPARC_FDIVD, r0, r1, i0)
#define fcr(cc, r0, r1, r2)		_fcr(_jit, cc, r0, r1, r2)
static void _fcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define fcw(cc, r0, r1, i0)		_fcw(_jit, cc, r0, r1, i0)
static void
_fcw(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ltr_f(r0, r1, r2)		fcr(SPARC_FBL, r0, r1, r2)
#  define lti_f(r0, r1, i0)		fcw(SPARC_FBL, r0, r1, i0)
#  define ler_f(r0, r1, r2)		fcr(SPARC_FBLE, r0, r1, r2)
#  define lei_f(r0, r1, i0)		fcw(SPARC_FBLE, r0, r1, i0)
#  define eqr_f(r0, r1, r2)		fcr(SPARC_FBE, r0, r1, r2)
#  define eqi_f(r0, r1, i0)		fcw(SPARC_FBE, r0, r1, i0)
#  define ger_f(r0, r1, r2)		fcr(SPARC_FBGE, r0, r1, r2)
#  define gei_f(r0, r1, i0)		fcw(SPARC_FBGE, r0, r1, i0)
#  define gtr_f(r0, r1, r2)		fcr(SPARC_FBG, r0, r1, r2)
#  define gti_f(r0, r1, i0)		fcw(SPARC_FBG, r0, r1, i0)
#  define ner_f(r0, r1, r2)		fcr(SPARC_FBNE, r0, r1, r2)
#  define nei_f(r0, r1, i0)		fcw(SPARC_FBNE, r0, r1, i0)
#  define unltr_f(r0, r1, r2)		fcr(SPARC_FBUL, r0, r1, r2)
#  define unlti_f(r0, r1, i0)		fcw(SPARC_FBUL, r0, r1, i0)
#  define unler_f(r0, r1, r2)		fcr(SPARC_FBULE, r0, r1, r2)
#  define unlei_f(r0, r1, i0)		fcw(SPARC_FBULE, r0, r1, i0)
#  define uneqr_f(r0, r1, r2)		fcr(SPARC_FBUE, r0, r1, r2)
#  define uneqi_f(r0, r1, i0)		fcw(SPARC_FBUE, r0, r1, i0)
#  define unger_f(r0, r1, r2)		fcr(SPARC_FBUGE, r0, r1, r2)
#  define ungei_f(r0, r1, i0)		fcw(SPARC_FBUGE, r0, r1, i0)
#  define ungtr_f(r0, r1, r2)		fcr(SPARC_FBUG, r0, r1, r2)
#  define ungti_f(r0, r1, i0)		fcw(SPARC_FBUG, r0, r1, i0)
#  define ltgtr_f(r0, r1, r2)		fcr(SPARC_FBLG, r0, r1, r2)
#  define ltgti_f(r0, r1, i0)		fcw(SPARC_FBLG, r0, r1, i0)
#  define ordr_f(r0, r1, r2)		fcr(SPARC_FBO, r0, r1, r2)
#  define ordi_f(r0, r1, i0)		fcw(SPARC_FBO, r0, r1, i0)
#  define unordr_f(r0, r1, r2)		fcr(SPARC_FBU, r0, r1, r2)
#  define unordi_f(r0, r1, i0)		fcw(SPARC_FBU, r0, r1, i0)
#define dcr(cc, r0, r1, r2)		_dcr(_jit, cc, r0, r1, r2)
static void _dcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define dcw(cc, r0, r1, i0)		_dcw(_jit, cc, r0, r1, i0)
static void
_dcw(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ltr_d(r0, r1, r2)		dcr(SPARC_FBL, r0, r1, r2)
#  define lti_d(r0, r1, i0)		dcw(SPARC_FBL, r0, r1, i0)
#  define ler_d(r0, r1, r2)		dcr(SPARC_FBLE, r0, r1, r2)
#  define lei_d(r0, r1, i0)		dcw(SPARC_FBLE, r0, r1, i0)
#  define eqr_d(r0, r1, r2)		dcr(SPARC_FBE, r0, r1, r2)
#  define eqi_d(r0, r1, i0)		dcw(SPARC_FBE, r0, r1, i0)
#  define ger_d(r0, r1, r2)		dcr(SPARC_FBGE, r0, r1, r2)
#  define gei_d(r0, r1, i0)		dcw(SPARC_FBGE, r0, r1, i0)
#  define gtr_d(r0, r1, r2)		dcr(SPARC_FBG, r0, r1, r2)
#  define gti_d(r0, r1, i0)		dcw(SPARC_FBG, r0, r1, i0)
#  define ner_d(r0, r1, r2)		dcr(SPARC_FBNE, r0, r1, r2)
#  define nei_d(r0, r1, i0)		dcw(SPARC_FBNE, r0, r1, i0)
#  define unltr_d(r0, r1, r2)		dcr(SPARC_FBUL, r0, r1, r2)
#  define unlti_d(r0, r1, i0)		dcw(SPARC_FBUL, r0, r1, i0)
#  define unler_d(r0, r1, r2)		dcr(SPARC_FBULE, r0, r1, r2)
#  define unlei_d(r0, r1, i0)		dcw(SPARC_FBULE, r0, r1, i0)
#  define uneqr_d(r0, r1, r2)		dcr(SPARC_FBUE, r0, r1, r2)
#  define uneqi_d(r0, r1, i0)		dcw(SPARC_FBUE, r0, r1, i0)
#  define unger_d(r0, r1, r2)		dcr(SPARC_FBUGE, r0, r1, r2)
#  define ungei_d(r0, r1, i0)		dcw(SPARC_FBUGE, r0, r1, i0)
#  define ungtr_d(r0, r1, r2)		dcr(SPARC_FBUG, r0, r1, r2)
#  define ungti_d(r0, r1, i0)		dcw(SPARC_FBUG, r0, r1, i0)
#  define ltgtr_d(r0, r1, r2)		dcr(SPARC_FBLG, r0, r1, r2)
#  define ltgti_d(r0, r1, i0)		dcw(SPARC_FBLG, r0, r1, i0)
#  define ordr_d(r0, r1, r2)		dcr(SPARC_FBO, r0, r1, r2)
#  define ordi_d(r0, r1, i0)		dcw(SPARC_FBO, r0, r1, i0)
#  define unordr_d(r0, r1, r2)		dcr(SPARC_FBU, r0, r1, r2)
#  define unordi_d(r0, r1, i0)		dcw(SPARC_FBU, r0, r1, i0)
#  if __WORDSIZE == 32
#    define ldr_f(r0, r1)		LDF(r1, 0, r0)
#  else
#  define ldr_f(r0, r1)			_ldr_f(_jit, r0, r1)
static void _ldr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#  define ldi_f(r0, i0)			_ldi_f(_jit, r0, i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define ldxr_f(r0, r1, r2)		LDF(r1, r2, r0)
#  else
#  define ldxr_f(r0, r1, r2)		_ldxr_f(_jit, r0, r1, r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  endif
#  define ldxi_f(r0, r1, i0)		_ldxi_f(_jit, r0, r1, i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define str_f(r0, r1)		STF(r1, r0, 0)
#  else
#  define str_f(r0, r1)			_str_f(_jit, r0, r1)
static void _str_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#  define sti_f(r0, i0)			_sti_f(_jit, r0, i0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  if __WORDSIZE == 32
#    define stxr_f(r0, r1, r2)		STF(r2, r1, r0)
#  else
#  define stxr_f(r0, r1, r2)		_stxr_f(_jit, r0, r1, r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  endif
#  define stxi_f(r0, r1, i0)		_stxi_f(_jit, r0, r1, i0)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define ldr_d(r0, r1)			LDDF(r1, 0, r0)
#  define ldi_d(r0, i0)			_ldi_d(_jit, r0, i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_d(r0, r1, r2)		LDDF(r1, r2, r0)
#  define ldxi_d(r0, r1, i0)		_ldxi_d(_jit, r0, r1, i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define str_d(r0, r1)			STDF(r1, r0, 0)
#  define sti_d(r0, i0)			_sti_d(_jit, r0, i0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_d(r0, r1, r2)		STDF(r2, r1, r0)
#  define stxi_d(r0, r1, i0)		_stxi_d(_jit, r0, r1, i0)
static void _stxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define fbr(cc, i0, r0, r1)		_fbr(_jit, cc, i0, r0, r1)
static jit_word_t
_fbr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define fbw(cc, i0, r0, i1)		_fbw(_jit, cc, i0, r0, i1)
static jit_word_t
_fbw(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_float32_t*);
#  define bltr_f(i0, r0, r1)		fbr(SPARC_FBL, i0, r0, r1)
#  define blti_f(i0, r0, i1)		fbw(SPARC_FBL, i0, r0, i1)
#  define bler_f(i0, r0, r1)		fbr(SPARC_FBLE, i0, r0, r1)
#  define blei_f(i0, r0, i1)		fbw(SPARC_FBLE, i0, r0, i1)
#  define beqr_f(i0, r0, r1)		fbr(SPARC_FBE, i0, r0, r1)
#  define beqi_f(i0, r0, i1)		fbw(SPARC_FBE, i0, r0, i1)
#  define bger_f(i0, r0, r1)		fbr(SPARC_FBGE, i0, r0, r1)
#  define bgei_f(i0, r0, i1)		fbw(SPARC_FBGE, i0, r0, i1)
#  define bgtr_f(i0, r0, r1)		fbr(SPARC_FBG, i0, r0, r1)
#  define bgti_f(i0, r0, i1)		fbw(SPARC_FBG, i0, r0, i1)
#  define bner_f(i0, r0, r1)		fbr(SPARC_FBNE, i0, r0, r1)
#  define bnei_f(i0, r0, i1)		fbw(SPARC_FBNE, i0, r0, i1)
#  define bunltr_f(i0, r0, r1)		fbr(SPARC_FBUL, i0, r0, r1)
#  define bunlti_f(i0, r0, i1)		fbw(SPARC_FBUL, i0, r0, i1)
#  define bunler_f(i0, r0, r1)		fbr(SPARC_FBULE, i0, r0, r1)
#  define bunlei_f(i0, r0, i1)		fbw(SPARC_FBULE, i0, r0, i1)
#  define buneqr_f(i0, r0, r1)		fbr(SPARC_FBUE, i0, r0, r1)
#  define buneqi_f(i0, r0, i1)		fbw(SPARC_FBUE, i0, r0, i1)
#  define bunger_f(i0, r0, r1)		fbr(SPARC_FBUGE, i0, r0, r1)
#  define bungei_f(i0, r0, i1)		fbw(SPARC_FBUGE, i0, r0, i1)
#  define bungtr_f(i0, r0, r1)		fbr(SPARC_FBUG, i0, r0, r1)
#  define bungti_f(i0, r0, i1)		fbw(SPARC_FBUG, i0, r0, i1)
#  define bltgtr_f(i0, r0, r1)		fbr(SPARC_FBLG, i0, r0, r1)
#  define bltgti_f(i0, r0, i1)		fbw(SPARC_FBLG, i0, r0, i1)
#  define bordr_f(i0, r0, r1)		fbr(SPARC_FBO, i0, r0, r1)
#  define bordi_f(i0, r0, i1)		fbw(SPARC_FBO, i0, r0, i1)
#  define bunordr_f(i0, r0, r1)		fbr(SPARC_FBU, i0, r0, r1)
#  define bunordi_f(i0, r0, i1)		fbw(SPARC_FBU, i0, r0, i1)
#  define dbr(cc, i0, r0, r1)		_dbr(_jit, cc, i0, r0, r1)
static jit_word_t
_dbr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define dbw(cc, i0, r0, i1)		_dbw(_jit, cc, i0, r0, i1)
static jit_word_t
_dbw(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_float64_t*);
#  define bltr_d(i0, r0, r1)		dbr(SPARC_FBL, i0, r0, r1)
#  define blti_d(i0, r0, i1)		dbw(SPARC_FBL, i0, r0, i1)
#  define bler_d(i0, r0, r1)		dbr(SPARC_FBLE, i0, r0, r1)
#  define blei_d(i0, r0, i1)		dbw(SPARC_FBLE, i0, r0, i1)
#  define beqr_d(i0, r0, r1)		dbr(SPARC_FBE, i0, r0, r1)
#  define beqi_d(i0, r0, i1)		dbw(SPARC_FBE, i0, r0, i1)
#  define bger_d(i0, r0, r1)		dbr(SPARC_FBGE, i0, r0, r1)
#  define bgei_d(i0, r0, i1)		dbw(SPARC_FBGE, i0, r0, i1)
#  define bgtr_d(i0, r0, r1)		dbr(SPARC_FBG, i0, r0, r1)
#  define bgti_d(i0, r0, i1)		dbw(SPARC_FBG, i0, r0, i1)
#  define bner_d(i0, r0, r1)		dbr(SPARC_FBNE, i0, r0, r1)
#  define bnei_d(i0, r0, i1)		dbw(SPARC_FBNE, i0, r0, i1)
#  define bunltr_d(i0, r0, r1)		dbr(SPARC_FBUL, i0, r0, r1)
#  define bunlti_d(i0, r0, i1)		dbw(SPARC_FBUL, i0, r0, i1)
#  define bunler_d(i0, r0, r1)		dbr(SPARC_FBULE, i0, r0, r1)
#  define bunlei_d(i0, r0, i1)		dbw(SPARC_FBULE, i0, r0, i1)
#  define buneqr_d(i0, r0, r1)		dbr(SPARC_FBUE, i0, r0, r1)
#  define buneqi_d(i0, r0, i1)		dbw(SPARC_FBUE, i0, r0, i1)
#  define bunger_d(i0, r0, r1)		dbr(SPARC_FBUGE, i0, r0, r1)
#  define bungei_d(i0, r0, i1)		dbw(SPARC_FBUGE, i0, r0, i1)
#  define bungtr_d(i0, r0, r1)		dbr(SPARC_FBUG, i0, r0, r1)
#  define bungti_d(i0, r0, i1)		dbw(SPARC_FBUG, i0, r0, i1)
#  define bltgtr_d(i0, r0, r1)		dbr(SPARC_FBLG, i0, r0, r1)
#  define bltgti_d(i0, r0, i1)		dbw(SPARC_FBLG, i0, r0, i1)
#  define bordr_d(i0, r0, r1)		dbr(SPARC_FBO, i0, r0, r1)
#  define bordi_d(i0, r0, i1)		dbw(SPARC_FBO, i0, r0, i1)
#  define bunordr_d(i0, r0, r1)		dbr(SPARC_FBU, i0, r0, r1)
#  define bunordi_d(i0, r0, i1)		dbw(SPARC_FBU, i0, r0, i1)
#  define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
static void
_f3f(jit_state_t *_jit, jit_int32_t rd,
     jit_int32_t op3, jit_int32_t rs1, jit_int32_t opf, jit_int32_t rs2)
{
    jit_instr_t		v;
#  if __WORDSIZE == 64
    if (rd > 31) {
	assert(rd <= 63 && (rd & 1) == 0);
	rd -= 31;
    }
    if (rs1 > 31) {
	assert(rs1 <= 63 && (rs1 & 1) == 0);
	rs1 -= 31;
    }
    if (rs2 > 31) {
	assert(rs2 <= 63 && (rs2 & 1) == 0);
	rs2 -= 31;
    }
#  endif
    assert(!(rd  & 0xffffffe0));
    assert(!(op3 & 0xffffffc0));
    assert(!(rs1 & 0xffffffe0));
    assert(!(opf & 0xfffffe00));
    assert(!(rs2 & 0xffffffe0));
    v.op.b    = 2;
    v.rd.b    = rd;
    v.op3.b   = op3;
    v.rs1.b   = rs1;
    v.opf.b   = opf;
    v.rs2.b   = rs2;
    ii(v.v);
}

#  if __WORDSIZE == 64
static void
_movr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, t1;
    if (r0 != r1) {
	if (single_precision_p(r0)) {
	    if (single_precision_p(r1))
		FMOVS(r1, r0);
	    else {
		t1 = jit_get_reg(CLASS_SNG);
		movr_d(rn(t1), r1);
		FMOVS(rn(t1), r0);
		jit_unget_reg(t1);
	    }
	}
	else {
	    if (single_precision_p(r1)) {
		t0 = jit_get_reg(CLASS_SNG);
		FMOVS(r1, rn(t0));
		movr_d(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	    else {
		t1 = jit_get_reg(CLASS_SNG);
		movr_d(rn(t1), r1);
		FMOVS(rn(t1), rn(t1));
		movr_d(r0, rn(t1));
		jit_unget_reg(t1);
	    }
	}
    }
}

static void
_negr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, t1;
    if (single_precision_p(r0)) {
	if (single_precision_p(r1))
	    FNEGS(r1, r0);
	else {
	    t1 = jit_get_reg(CLASS_SNG);
	    movr_d(rn(t1), r1);
	    FNEGS(rn(t1), r0);
	    jit_unget_reg(t1);
	}
    }
    else {
	if (single_precision_p(r1)) {
	    t0 = jit_get_reg(CLASS_SNG);
	    FNEGS(r1, rn(t0));
	    movr_d(r0, rn(t0));
	    jit_unget_reg(t0);
	}
	else {
	    t1 = jit_get_reg(CLASS_SNG);
	    movr_d(rn(t1), r1);
	    FNEGS(rn(t1), rn(t1));
	    movr_d(r0, rn(t1));
	    jit_unget_reg(t1);
	}
    }
}

static void
_absr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, t1;
    if (single_precision_p(r0)) {
	if (single_precision_p(r1))
	    FABSS(r1, r0);
	else {
	    t1 = jit_get_reg(CLASS_SNG);
	    movr_d(rn(t1), r1);
	    FABSS(rn(t1), r0);
	    jit_unget_reg(t1);
	}
    }
    else {
	if (single_precision_p(r1)) {
	    t0 = jit_get_reg(CLASS_SNG);
	    FABSS(r1, rn(t0));
	    movr_d(r0, rn(t0));
	    jit_unget_reg(t0);
	}
	else {
	    t1 = jit_get_reg(CLASS_SNG);
	    movr_d(rn(t1), r1);
	    FABSS(rn(t1), rn(t1));
	    movr_d(r0, rn(t1));
	    jit_unget_reg(t1);
	}
    }
}

static void
_sqrtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, t1;
    if (single_precision_p(r0)) {
	if (single_precision_p(r1))
	    FSQRTS(r1, r0);
	else {
	    t1 = jit_get_reg(CLASS_SNG);
	    movr_d(rn(t1), r1);
	    FSQRTS(rn(t1), r0);
	    jit_unget_reg(t1);
	}
    }
    else {
	if (single_precision_p(r1)) {
	    t0 = jit_get_reg(CLASS_SNG);
	    FSQRTS(r1, rn(t0));
	    movr_d(r0, rn(t0));
	    jit_unget_reg(t0);
	}
	else {
	    t1 = jit_get_reg(CLASS_SNG);
	    movr_d(rn(t1), r1);
	    FSQRTS(rn(t1), rn(t1));
	    movr_d(r0, rn(t1));
	    jit_unget_reg(t1);
	}
    }
}
#  endif

#  if __WORDSIZE == 64
static void
_extr_d_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (!single_precision_p(r0)) {
	reg = jit_get_reg(CLASS_SNG);
	movr_d(rn(reg), r0);
	FDTOS(r1, rn(reg));
	movr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	FDTOS(r1, r0);
}
#  endif

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
	stxi_i(BIAS(-8), _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
	ldxi_f(r0, _FP_REGNO, BIAS(-8));
    }
    else
	ldi_f(r0, (jit_word_t)i0);
}

#  if __WORDSIZE == 64
static void
_extr_f_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (!single_precision_p(r1)) {
	reg = jit_get_reg(CLASS_SNG);
	movr_d(rn(reg), r1);
	FSTOD(rn(reg), r0);
	jit_unget_reg(reg);
    }
    else
	FSTOD(r1, r0);
}
#  endif

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
#  if __WORDSIZE == 32
	jit_int32_t	 i[2];
#  else
	jit_word_t	 w;
#  endif
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;

    if (_jitc->no_data) {
	data.d = *i0;
	reg = jit_get_reg(jit_class_gpr);
# if __WORDSIZE == 32
	movi(rn(reg), data.i[0]);
#  else
	movi(rn(reg), data.w);
#  endif
	stxi(BIAS(-8), _FP_REGNO, rn(reg));
#  if __WORDSIZE == 32
	movi(rn(reg), data.i[1]);
	stxi_i(BIAS(-4), _FP_REGNO, rn(reg));
#  endif
	jit_unget_reg(reg);
	ldxi_d(r0, _FP_REGNO, BIAS(-8));
    }
    else
	ldi_d(r0, (jit_word_t)i0);
}

#  if __WORDSIZE == 32
static void
_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(!(r0 & 1));
    assert(!(r1 & 1));
    if (r0 != r1) {
	FMOVS(r1, r0);
	FMOVS(r1 + 1, r0 + 1);
    }
}

static void
_negr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(!(r0 & 1));
    assert(!(r1 & 1));
    FNEGS(r1, r0);
    if (r0 != r1)
	FMOVS(r1 + 1, r0 + 1);
}

static void
_absr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(!(r0 & 1));
    assert(!(r1 & 1));
    FABSS(r1, r0);
    if (r0 != r1)
	FMOVS(r1 + 1, r0 + 1);
}
#  endif

#  if __WORDSIZE == 64
#    define single_rrr(NAME, CODE)					\
static void								\
NAME(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)	\
{									\
    jit_int32_t		x0, t0, x1, t1, x2, t2, mask = 0;		\
    if (!single_precision_p(r0)) {					\
	mask |= 1;							\
	t0 = jit_get_reg(CLASS_SNG);					\
	x0 = rn(t0);							\
	if (r0 == r1) {							\
	    x1 = x0;							\
	    movr_d(x1, r1);						\
	    if (r0 == r2)						\
		x2 = x0;						\
	}								\
	else if (r0 == r2) {						\
	    x2 = x0;							\
	    movr_d(x2, r2);						\
	}								\
    }									\
    else								\
	x0 = r0;							\
    if (!single_precision_p(r1)) {					\
	if (r0 != r1) {							\
	    mask |= 2;							\
	    t1 = jit_get_reg(CLASS_SNG);				\
	    x1 = rn(t1);						\
	    movr_d(x1, r1);						\
	    if (r1 == r2)						\
		x2 = x1;						\
	}								\
    }									\
    else								\
	x1 = r1;							\
    if (!single_precision_p(r2)) {					\
	if (r0 != r2 && r1 != r2) {					\
	    mask |= 4;							\
	    t2 = jit_get_reg(CLASS_SNG);				\
	    x2 = rn(t2);						\
	    movr_d(x2, r2);						\
	}								\
    }									\
    else								\
	x2 = r2;							\
    CODE(x1, x2, x0);							\
    if (mask & 1) {							\
	movr_d(r0, x0);							\
	jit_unget_reg(t0);						\
    }									\
    if (mask & 2)							\
	jit_unget_reg(t1);						\
    if (mask & 4)							\
	jit_unget_reg(t2);						\
}

static void
_fop2f(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		x0, t0, x1, t1, x2, t2, mask = 0;
    if (!single_precision_p(r0)) {
	mask |= 1;
	t0 = jit_get_reg(CLASS_SNG);
	x0 = rn(t0);
	if (r0 == r1) {
	    x1 = x0;
	    movr_d(x1, r1);
	    if (r0 == r2)
		x2 = x0;
	}
	else if (r0 == r2) {
	    x2 = x0;
	    movr_d(x2, r2);
	}
    }
    else
	x0 = r0;
    if (!single_precision_p(r1)) {
	if (r0 != r1) {
	    mask |= 2;
	    t1 = jit_get_reg(CLASS_SNG);
	    x1 = rn(t1);
	    movr_d(x1, r1);
	    if (r1 == r2)
		x2 = x1;
	}
    }
    else
	x1 = r1;
    if (!single_precision_p(r2)) {
	if (r0 != r2 && r1 != r2) {
	    mask |= 4;
	    t2 = jit_get_reg(CLASS_SNG);
	    x2 = rn(t2);
	    movr_d(x2, r2);
	}
    }
    else
	x2 = r2;
    FPop1(x0, x1,  op, x2);
    if (mask & 1) {
	movr_d(r0, x0);
	jit_unget_reg(t0);
    }
    if (mask & 2)
	jit_unget_reg(t1);
    if (mask & 4)
	jit_unget_reg(t2);
}
#  endif

static void
_fop1f(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t r0, jit_int32_t r1, jit_float32_t *i0)
{
    jit_int32_t		reg;
#  if __WORDSIZE == 64
    jit_int32_t		x0, t0, x1, t1, mask = 0;
#  endif
    reg = jit_get_reg(CLASS_SNG);
    movi_f(rn(reg), i0);
#  if __WORDSIZE == 64
    if (!single_precision_p(r0)) {
	mask |= 1;
	t0 = jit_get_reg(CLASS_SNG);
	x0 = rn(t0);
	if (r0 == r1) {
	    x1 = x0;
	    movr_d(x1, r1);
	}
    }
    else
	x0 = r0;
    if (!single_precision_p(r1)) {
	if (r0 != r1) {
	    mask |= 2;
	    t1 = jit_get_reg(CLASS_SNG);
	    x1 = rn(t1);
	    movr_d(x1, r1);
	}
    }
    else
	x1 = r1;
    FPop1(x0, x1, op, rn(reg));
    if (mask & 1) {
	movr_d(r0, x0);
	jit_unget_reg(t0);
    }
    if (mask & 2)
	jit_unget_reg(t1);
#  else
    FPop1(r0, r1, op, rn(reg));
#  endif
    jit_unget_reg(reg);
}

static void
_rfop1f(jit_state_t *_jit, jit_int32_t op,
	jit_int32_t r0, jit_int32_t r1, jit_float32_t *i0)
{
    jit_int32_t		reg;
#  if __WORDSIZE == 64
    jit_int32_t		x0, t0, x1, t1, mask = 0;
#  endif
    reg = jit_get_reg(CLASS_SNG);
    movi_f(rn(reg), i0);
#  if __WORDSIZE == 64
    if (!single_precision_p(r0)) {
	mask |= 1;
	t0 = jit_get_reg(CLASS_SNG);
	x0 = rn(t0);
	if (r0 == r1) {
	    x1 = x0;
	    movr_d(x1, r1);
	}
    }
    else
	x0 = r0;
    if (!single_precision_p(r1)) {
	if (r0 != r1) {
	    mask |= 2;
	    t1 = jit_get_reg(CLASS_SNG);
	    x1 = rn(t1);
	    movr_d(x1, r1);
	}
    }
    else
	x1 = r1;
    FPop1(x0, rn(reg), op, x1);
    if (mask & 1) {
	movr_d(r0, x0);
	jit_unget_reg(t0);
    }
    if (mask & 2)
	jit_unget_reg(t1);
#  else
    FPop1(r0, rn(reg), op, r1);
#  endif
    jit_unget_reg(reg);
}

static void
_fop1d(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t r0, jit_int32_t r1, jit_float64_t *i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_DBL);
    movi_d(rn(reg), i0);
    FPop1(r0, r1, op, rn(reg));
    jit_unget_reg(reg);
}

static void
_rfop1d(jit_state_t *_jit, jit_int32_t op,
	jit_int32_t r0, jit_int32_t r1, jit_float64_t *i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_DBL);
    movi_d(rn(reg), i0);
    FPop1(r0, rn(reg), op, r1);
    jit_unget_reg(reg);
}

static void
_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    stxi(BIAS(-8), _FP_REGNO, r1);
#  if __WORDSIZE == 32
    ldxi_f(r0, _FP_REGNO, BIAS(-8));
    FITOS(r0, r0);
#  else
    ldxi_d(r0, _FP_REGNO, BIAS(-8));
    if (!single_precision_p(r0)) {
	jit_int32_t	reg;
	reg = jit_get_reg(CLASS_SNG);
	movr_d(rn(reg), r0);
	FXTOS(rn(reg), rn(reg));
	movr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	FXTOS(r0, r0);
#  endif
}

static void
_truncr_f_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_SNG);
#  if __WORDSIZE == 64
    if (!single_precision_p(r1)) {
	movr_d(rn(reg), r1);
	FSTOI(rn(reg), rn(reg));
    }
    else
#  endif
	FSTOI(r1, rn(reg));
    stxi_f(BIAS(-8), _FP_REGNO, rn(reg));
    ldxi_i(r0, _FP_REGNO, BIAS(-8));
    jit_unget_reg(reg);
}

#  if __WORDSIZE == 64
static void
_truncr_f_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_SNG);
#  if __WORDSIZE == 64
    if (!single_precision_p(r1)) {
	movr_d(rn(reg), r1);
	FSTOX(rn(reg), rn(reg));
    }
    else
#  endif
	FSTOX(r1, rn(reg));
    stxi_d(BIAS(-8), _FP_REGNO, rn(reg));
    ldxi_l(r0, _FP_REGNO, BIAS(-8));
    jit_unget_reg(reg);
}
#  endif

static void
_fcr(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#  if __WORDSIZE == 64
    jit_int32_t		x0, t0, x1, t1, mask = 0;
    if (!single_precision_p(r1)) {
	mask |= 1;
	t0 = jit_get_reg(CLASS_SNG);
	x0 = rn(t0);
	movr_d(x0, r1);
    }
    else
	x0 = r1;
    if (r1 == r2)
	x1 = x0;
    else if (!single_precision_p(r2)) {
	mask |= 2;
	t1 = jit_get_reg(CLASS_SNG);
	x1 = rn(t1);
	movr_d(x1, r2);
    }
    else
	x1 = r2;
    FCMPS(x0, x1);
    if (mask & 1)
	jit_unget_reg(t0);
    if (mask & 2)
	jit_unget_reg(t1);
#  else
    FCMPS(r1, r2);
#  endif
    FBa(cc, 3);
    movi(r0, 1);
    movi(r0, 0);
}

static void
_fcw(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_float32_t *i0)
{
    jit_int32_t		reg;
#  if __WORDSIZE == 64
    jit_int32_t		x0, t0, mask = 0;
    if (!single_precision_p(r1)) {
	mask |= 1;
	t0 = jit_get_reg(CLASS_SNG);
	x0 = rn(t0);
	movr_d(x0, r1);
    }
    else
	x0 = r1;
#  endif
    reg = jit_get_reg(CLASS_SNG);
    movi_f(rn(reg), i0);
#  if __WORDSIZE == 64
    FCMPS(x0, rn(reg));
    if (mask & 1)
	jit_unget_reg(t0);
#  else
    FCMPS(r1, rn(reg));
#  endif
    jit_unget_reg(reg);
    FBa(cc, 3);
    movi(r0, 1);
    movi(r0, 0);
}

static void
_dcr(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMPD(r1, r2);
    FBa(cc, 3);
    movi(r0, 1);
    movi(r0, 0);
}

static void
_dcw(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_float64_t *i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_DBL);
    movi_d(rn(reg), i0);
    FCMPD(r1, rn(reg));
    jit_unget_reg(reg);
    FBa(cc, 3);
    movi(r0, 1);
    movi(r0, 0);
}

#  if __WORDSIZE == 64
static void
_ldr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (!single_precision_p(r0)) {
	reg = jit_get_reg(CLASS_SNG);
	LDF(r1, 0, rn(reg));
	movr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	LDF(r1, 0, r0);
}
#  endif

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0)) {
#  if __WORDSIZE == 64
	if (!single_precision_p(r0)) {
	    reg = jit_get_reg(CLASS_SNG);
	    LDFI(0, i0, rn(reg));
	    movr_d(r0, rn(reg));
	    jit_unget_reg(reg);
	}
	else
#  endif
	    LDFI(0, i0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

#  if __WORDSIZE == 64
static void
_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (!single_precision_p(r0)) {
	reg = jit_get_reg(CLASS_SNG);
	LDF(r1, r2, rn(reg));
	movr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	LDF(r1, r2, r0);
}
#  endif

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0)) {
#  if __WORDSIZE == 64
	if (!single_precision_p(r0)) {
	    reg = jit_get_reg(CLASS_SNG);
	    LDFI(r1, i0, rn(reg));
	    movr_d(r0, rn(reg));
	    jit_unget_reg(reg);
	}
	else
#  endif
	    LDFI(r1, i0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_f(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

#  if __WORDSIZE == 64
static void
_str_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (!single_precision_p(r1)) {
	reg = jit_get_reg(CLASS_SNG);
	movr_d(rn(reg), r1);
	STF(rn(reg), r0, 0);
	jit_unget_reg(reg);
    }
    else
	STF(r1, r0, 0);
}
# endif

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (s13_p(i0)) {
#  if __WORDSIZE == 64
	if (!single_precision_p(r0)) {
	    reg = jit_get_reg(CLASS_SNG);
	    movr_d(rn(reg), r0);
	    STFI(rn(reg), 0, i0);
	    jit_unget_reg(reg);
	}
	else
#  endif
	    STFI(r0, 0, i0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_f(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

#  if __WORDSIZE == 64
static void
_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (!single_precision_p(r2)) {
	reg = jit_get_reg(CLASS_SNG);
	movr_d(rn(reg), r2);
	STF(rn(reg), r1, r0);
	jit_unget_reg(reg);
    }
    else
	STF(r2, r1, r0);
}
# endif

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (s13_p(i0)) {
#  if __WORDSIZE == 64
	if (!single_precision_p(r1)) {
	    reg = jit_get_reg(CLASS_SNG);
	    movr_d(rn(reg), r1);
	    STFI(rn(reg), r0, i0);
	    jit_unget_reg(reg);
	}
	else
#  endif
	    STFI(r1, r0, i0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_f(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}

static void
_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    stxi(BIAS(-8), _FP_REGNO, r1);
#  if __WORDSIZE == 32
    stxi(BIAS(-4), _FP_REGNO, 0);
#  endif
    ldxi_d(r0, _FP_REGNO, BIAS(-8));
#  if __WORDSIZE == 32
    FITOD(r0, r0);
#  else
    FXTOD(r0, r0);
#  endif
}

static void
_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_SNG);
#  if __WORDSIZE == 64
    if (!single_precision_p(r1)) {
	movr_d(rn(reg), r1);
	FDTOI(rn(reg), rn(reg));
    }
    else
#  endif
	FDTOI(r1, rn(reg));
    stxi_d(BIAS(-8), _FP_REGNO, rn(reg));
    ldxi_i(r0, _FP_REGNO, BIAS(-8));
    jit_unget_reg(reg);
}

#  if __WORDSIZE == 64
static void
_truncr_d_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_DBL);
    FDTOX(r1, rn(reg));
    stxi_d(BIAS(-8), _FP_REGNO, rn(reg));
    ldxi_l(r0, _FP_REGNO, BIAS(-8));
    jit_unget_reg(reg);
}
#  endif

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDDFI(0, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDDFI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_d(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STDFI(r0, 0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_d(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_stxi_d(jit_state_t *_jit, jit_int32_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STDFI(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_d(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}

static jit_word_t
_fbr(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0,jit_int32_t r1)
{
#  if __WORDSIZE == 64
    jit_int32_t		x0, t0, x1, t1, mask = 0;
#  endif
    jit_word_t		w;
#  if __WORDSIZE == 64
    if (!single_precision_p(r0)) {
	mask |= 1;
	t0 = jit_get_reg(CLASS_SNG);
	x0 = rn(t0);
	movr_d(x0, r0);
    }
    else
	x0 = r0;
    if (r0 == r1)
	x1 = x0;
    else if (!single_precision_p(r1)) {
	mask |= 2;
	t1 = jit_get_reg(CLASS_SNG);
	x1 = rn(t1);
	movr_d(x1, r1);
    }
    else
	x1 = r1;
    FCMPS(x0, x1);
    if (mask & 1)
	jit_unget_reg(t0);
    if (mask & 2)
	jit_unget_reg(t1);
#  else
    FCMPS(r0, r1);
#  endif
    w = _jit->pc.w;
    FB(cc, (i0 - w) >> 2);
    NOP();
    return (w);
}

static jit_word_t
_fbw(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_float32_t *i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
#  if __WORDSIZE == 64
    jit_int32_t		x0, t0, mask = 0;
    if (!single_precision_p(r0)) {
	mask |= 1;
	t0 = jit_get_reg(CLASS_SNG);
	x0 = rn(t0);
	movr_d(x0, r0);
    }
    else
	x0 = r0;
#  endif
    reg = jit_get_reg(CLASS_SNG);
    movi_f(rn(reg), i1);
#  if __WORDSIZE == 64
    FCMPS(x0, rn(reg));
    if (mask & 1)
	jit_unget_reg(t0);
#  else
    FCMPS(r0, rn(reg));
#  endif
    jit_unget_reg(reg);
    w = _jit->pc.w;
    FB(cc, (i0 - w) >> 2);
    NOP();
    return (w);
}

static jit_word_t
_dbr(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    FCMPD(r0, r1);
    w = _jit->pc.w;
    FB(cc, (i0 - w) >> 2);
    NOP();
    return (w);
}

static jit_word_t
_dbw(jit_state_t *_jit, jit_int32_t cc,
     jit_word_t i0, jit_int32_t r0, jit_float64_t *i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(CLASS_DBL);
    movi_d(rn(reg), i1);
    FCMPD(r0, rn(reg));
    jit_unget_reg(reg);
    w = _jit->pc.w;
    FB(cc, (i0 - w) >> 2);
    NOP();
    return (w);
}

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);

    /* Load argument. */
    ldr_d(r0, r1);

    /* Update vararg stack pointer. */
    addi(r1, r1, 8);
}
#endif
