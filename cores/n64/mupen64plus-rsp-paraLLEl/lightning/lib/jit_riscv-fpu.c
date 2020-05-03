/*
 * Copyright (C) 2019  Free Software Foundation, Inc.
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
/*
 * RV32F Standard Extension
 */
#  define FLW(rd, rs1, im)		Itype(7, rd, 2, rs1, im)
#  define FSW(rs1, rs2, imm)		Stype(39, 2, rs1, rs2, imm)
#  define FMADD_S(rd, rs1, rs2, rs3)	R4type(67, rd, 0, rs1, rs2, 0, rs3)
#  define FMSUB_S(rd, rs1, rs2, rs3)	R4type(71, rd, 0, rs1, rs2, 0, rs3)
#  define FNMSUB_S(rd, rs1, rs2, rs3)	R4type(75, rd, 0, rs1, rs2, 0, rs3)
#  define FNMADD_S(rd, rs1, rs2, rs3)	R4type(79, rd, 0, rs1, rs2, 0, rs3)
#  define FADD_S(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 0)
#  define FSUB_S(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 4)
#  define FMUL_S(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 8)
#  define FDIV_S(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 12)
#  define FSQRT_S(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 44)
#  define FSGNJ_S(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 16)
#  define FSGNJN_S(rd, rs1, rs2)	Rtype(83, rd, 1, rs1, rs2, 16)
#  define FSGNJX_S(rd, rs1, rs2)	Rtype(83, rd, 2, rs1, rs2, 16)
#  define FMIN_S(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 20)
#  define FMAX_S(rd, rs1, rs2)		Rtype(83, rd, 1, rs1, rs2, 20)
#  define FCVT_W_S(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 96)
#  define FCVT_WU_S(rd, rs1)		Rtype(83, rd, 1, rs1, 1, 96)
#  define FMV_X_W(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 112)
#  define FEQ_S(rd, rs1, rs2)		Rtype(83, rd, 2, rs1, rs2, 80)
#  define FLT_S(rd, rs1, rs2)		Rtype(83, rd, 1, rs1, rs2, 80)
#  define FLE_S(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 80)
#  define FCLASS_S(rd, rs1)		Rtype(83, rd, 1, rs1, 0, 112)
#  define FCVT_S_W(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 104)
#  define FCVT_S_WU(rd, rs1)		Rtype(83, rd, 0, rs1, 1, 104)
#  define FMV_W_X(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 120)
/*
 * RV64F Standard Extension (in addition to RV32F)
 */
#  define FCVT_L_S(rd, rs1)		Rtype(83, rd, 0, rs1, 2, 96)
#  define FCVT_LU_S(rd, rs1)		Rtype(83, rd, 0, rs1, 3, 96)
#  define FCVT_S_L(rd, rs1)		Rtype(83, rd, 0, rs1, 2, 104)
#  define FCVT_S_LU(rd, rs1)		Rtype(83, rd, 0, rs1, 3, 104)
/*
 * RV32D Standard Extension
 */
#  define FLD(rd, rs1, im)		Itype(7, rd, 3, rs1, im)
#  define FSD(rs1, rs2, imm)		Stype(39, 3, rs1, rs2, imm)
#  define FMADD_D(rd, rs1, rs2, rs3)	R4type(67, rd, 0, rs1, rs2, 1, rs3)
#  define FMSUB_D(rd, rs1, rs2, rs3)	R4type(71, rd, 0, rs1, rs2, 1, rs3)
#  define FNMSUB_D(rd, rs1, rs2, rs3)	R4type(75, rd, 0, rs1, rs2, 1, rs3)
#  define FNMADD_D(rd, rs1, rs2, rs3)	R4type(79, rd, 0, rs1, rs2, 1, rs3)
#  define FADD_D(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 1)
#  define FSUB_D(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 5)
#  define FMUL_D(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 9)
#  define FDIV_D(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 13)
#  define FSQRT_D(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 45)
#  define FSGNJ_D(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 17)
#  define FSGNJN_D(rd, rs1, rs2)	Rtype(83, rd, 1, rs1, rs2, 17)
#  define FSGNJX_D(rd, rs1, rs2)	Rtype(83, rd, 2, rs1, rs2, 17)
#  define FMIN_D(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 21)
#  define FMAX_D(rd, rs1, rs2)		Rtype(83, rd, 1, rs1, rs2, 21)
#  define FCVT_S_D(rd, rs1)		Rtype(83, rd, 0, rs1, 1, 32)
#  define FCVT_D_S(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 33)
#  define FEQ_D(rd, rs1, rs2)		Rtype(83, rd, 2, rs1, rs2, 81)
#  define FLT_D(rd, rs1, rs2)		Rtype(83, rd, 1, rs1, rs2, 81)
#  define FLE_D(rd, rs1, rs2)		Rtype(83, rd, 0, rs1, rs2, 81)
#  define FCLASS_D(rd, rs1)		Rtype(83, rd, 1, rs1, 0, 113)
#  define FCVT_W_D(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 97)
#  define FCVT_WU_D(rd, rs1)		Rtype(83, rd, 0, rs1, 1, 97)
#  define FCVT_D_W(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 105)
#  define FCVT_D_WU(rd, rs1)		Rtype(83, rd, 0, rs1, 1, 105)
/*
 * RV64D Standard Extension (in addition to RV32D)
 */
#  define FCVT_L_D(rd, rs1)		Rtype(83, rd, 0, rs1, 2, 97)
#  define FCVT_LU_D(rd, rs1)		Rtype(83, rd, 0, rs1, 3, 97)
#  define FMV_X_D(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 113)
#  define FCVT_D_L(rd, rs1)		Rtype(83, rd, 0, rs1, 2, 105)
#  define FCVT_D_LU(rd, rs1)		Rtype(83, rd, 0, rs1, 3, 105)
#  define FMV_D_X(rd, rs1)		Rtype(83, rd, 0, rs1, 0, 121)
/*
 * Pseudo instructions
 */
#  define FMV_S(r0, r1)			FSGNJ_S(r0, r1, r1)
#  define FABS_S(r0, r1)		FSGNJX_S(r0, r1, r1)
#  define FNEG_S(r0, r1)		FSGNJN_S(r0, r1, r1)
#  define FMV_D(r0, r1)			FSGNJ_D(r0, r1, r1)
#  define FABS_D(r0, r1)		FSGNJX_D(r0, r1, r1)
#  define FNEG_D(r0, r1)		FSGNJN_D(r0, r1, r1)

/*
 * Lightning instructions
 */
#  define truncr_f_i(r0, r1)		FCVT_W_S(r0, r1)
#  define truncr_d_i(r0, r1)		FCVT_W_D(r0, r1)
#  define truncr_f_l(r0, r1)		FCVT_L_S(r0, r1)
#  define truncr_d_l(r0, r1)		FCVT_L_D(r0, r1)
#  define addr_f(r0, r1, r2)		FADD_S(r0, r1, r2)
#  define addi_f(r0, r1, im)		_addi_f(_jit, r0, r1, im)
static void _addi_f(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float32_t);
#  define subr_f(r0, r1, r2)		FSUB_S(r0, r1, r2)
#  define subi_f(r0, r1, im)		_subi_f(_jit, r0, r1, im)
static void _subi_f(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float32_t);
#  define rsbr_f(r0, r1, r2)		FSUB_S(r0, r2, r1)
#  define rsbi_f(r0, r1, im)		_rsbi_f(_jit, r0, r1, im)
static void _rsbi_f(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float32_t);
#  define mulr_f(r0, r1, r2)		FMUL_S(r0, r1, r2)
#  define muli_f(r0, r1, im)		_muli_f(_jit, r0, r1, im)
static void _muli_f(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float32_t);
#  define divr_f(r0, r1, r2)		FDIV_S(r0, r1, r2)
#  define divi_f(r0, r1, im)		_divi_f(_jit, r0, r1, im)
static void _divi_f(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float32_t);
#  define absr_f(r0, r1)		FABS_S(r0, r1)
#  define negr_f(r0, r1)		FNEG_S(r0, r1)
#  define sqrtr_f(r0, r1)		FSQRT_S(r0, r1)
#  define extr_f(r0, r1)		FCVT_S_L(r0, r1)
#  define ldr_f(r0, r1)			FLW(r0, r1, 0)
#  define ldi_f(r0, im)			_ldi_f(_jit, r0, im)
static void _ldi_f(jit_state_t*, jit_int32_t, jit_word_t);
#  define ldxr_f(r0, r1, r2)		_ldxr_f(_jit, r0, r1, r2)
static void _ldxr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ldxi_f(r0, r1, i0)		_ldxi_f(_jit, r0, r1, i0)
static void _ldxi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define str_f(r0, r1)			FSW(r0, r1, 0)
#  define sti_f(im, r0)			_sti_f(_jit, im, r0)
static void _sti_f(jit_state_t*, jit_word_t, jit_int32_t);
#  define stxr_f(r0, r1, r2)		_stxr_f(_jit, r0, r1, r2)
static void _stxr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define stxi_f(im, r0, r1)		_stxi_f(_jit, im, r0, r1)
static void _stxi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define movr_f(r0, r1)		FMV_S(r0, r1)
#  define movi_f(r0, im)		_movi_f(_jit, r0, im)
static void _movi_f(jit_state_t*, jit_int32_t, jit_float32_t);
#  define movr_f_w(r0, r1)		FMV_X_W(r0, r1)
#  define movi_f_w(r0, im)		_movi_f_w(_jit, r0, im)
static void _movi_f_w(jit_state_t*, jit_int32_t, jit_float32_t);
#  define movr_w_f(r0, r1)		FMV_W_X(r0, r1)
#  define extr_d_f(r0, r1)		FCVT_S_D(r0, r1)
#  define ltr_f(r0, r1, r2)		FLT_S(r0, r1, r2)
#  define lti_f(r0, r1, im)		_lti_f(_jit, r0, r1, im)
static void _lti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define ler_f(r0, r1, r2)		FLE_S(r0, r1, r2)
#  define lei_f(r0, r1, im)		_lei_f(_jit, r0, r1, im)
static void _lei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define eqr_f(r0, r1, r2)		FEQ_S(r0, r1, r2)
#  define eqi_f(r0, r1, im)		_eqi_f(_jit, r0, r1, im)
static void _eqi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define ger_f(r0, r1, r2)		FLE_S(r0, r2, r1)
#  define gei_f(r0, r1, im)		_gei_f(_jit, r0, r1, im)
static void _gei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define gtr_f(r0, r1, r2)		FLT_S(r0, r2, r1)
#  define gti_f(r0, r1, im)		_gti_f(_jit, r0, r1, im)
static void _gti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define ner_f(r0, r1, r2)		_ner_f(_jit, r0, r1, r2)
static void _ner_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define nei_f(r0, r1, im)		_nei_f(_jit, r0, r1, im)
static void _nei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define unltr_f(r0, r1, r2)		_unltr_f(_jit, r0, r1, r2)
static void _unltr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define unlti_f(r0, r1, im)		_unlti_f(_jit, r0, r1, im)
static void _unlti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define unler_f(r0, r1, r2)		_unler_f(_jit, r0, r1, r2)
static void _unler_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define unlei_f(r0, r1, im)		_unlei_f(_jit, r0, r1, im)
static void _unlei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define uneqr_f(r0, r1, r2)		_uneqr_f(_jit, r0, r1, r2)
static void _uneqr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define uneqi_f(r0, r1, im)		_uneqi_f(_jit, r0, r1, im)
static void _uneqi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define unger_f(r0, r1, r2)		_unger_f(_jit, r0, r1, r2)
static void _unger_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ungei_f(r0, r1, im)		_ungei_f(_jit, r0, r1, im)
static void _ungei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define ungtr_f(r0, r1, r2)		_ungtr_f(_jit, r0, r1, r2)
static void _ungtr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ungti_f(r0, r1, im)		_ungti_f(_jit, r0, r1, im)
static void _ungti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define ltgtr_f(r0, r1, r2)		_ltgtr_f(_jit, r0, r1, r2)
static void _ltgtr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ltgti_f(r0, r1, im)		_ltgti_f(_jit, r0, r1, im)
static void _ltgti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define ordr_f(r0, r1, r2)		_ordr_f(_jit, r0, r1, r2)
static void _ordr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ordi_f(r0, r1, im)		_ordi_f(_jit, r0, r1, im)
static void _ordi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define unordr_f(r0, r1, r2)		_unordr_f(_jit, r0, r1, r2)
static void _unordr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define unordi_f(r0, r1, im)		_unordi_f(_jit, r0, r1, im)
static void _unordi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
#  define bltr_f(br, r0, r1)		_bltr_f(_jit,br,r0,r1)
static jit_word_t _bltr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti_f(br, r0, im)		_blti_f(_jit,br,r0,im)
static jit_word_t _blti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bler_f(br, r0, r1)		_bler_f(_jit,br,r0,r1)
static jit_word_t _bler_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei_f(br, r0, im)		_blei_f(_jit,br,r0,im)
static jit_word_t _blei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define beqr_f(br, r0, r1)		_beqr_f(_jit,br,r0,r1)
static jit_word_t _beqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define beqi_f(br, r0, im)		_beqi_f(_jit,br,r0,im)
static jit_word_t _beqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bger_f(br, r0, r1)		_bger_f(_jit,br,r0,r1)
static jit_word_t _bger_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei_f(br, r0, im)		_bgei_f(_jit,br,r0,im)
static jit_word_t _bgei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bgtr_f(br, r0, r1)		_bgtr_f(_jit,br,r0,r1)
static jit_word_t _bgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti_f(br, r0, im)		_bgti_f(_jit,br,r0,im)
static jit_word_t _bgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bner_f(br, r0, r1)		_bner_f(_jit,br,r0,r1)
static jit_word_t _bner_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bnei_f(br, r0, im)		_bnei_f(_jit,br,r0,im)
static jit_word_t _bnei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bunltr_f(br, r0, r1)		_bunltr_f(_jit,br,r0,r1)
static jit_word_t _bunltr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlti_f(br, r0, im)		_bunlti_f(_jit,br,r0,im)
static jit_word_t _bunlti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bunler_f(br, r0, r1)		_bunler_f(_jit,br,r0,r1)
static jit_word_t _bunler_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlei_f(br, r0, im)		_bunlei_f(_jit,br,r0,im)
static jit_word_t _bunlei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define buneqr_f(br, r0, r1)		_buneqr_f(_jit,br,r0,r1)
static jit_word_t _buneqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi_f(br, r0, im)		_buneqi_f(_jit,br,r0,im)
static jit_word_t _buneqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bunger_f(br, r0, r1)		_bunger_f(_jit,br,r0,r1)
static jit_word_t _bunger_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungei_f(br, r0, im)		_bungei_f(_jit,br,r0,im)
static jit_word_t _bungei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bungtr_f(br, r0, r1)		_bungtr_f(_jit,br,r0,r1)
static jit_word_t _bungtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungti_f(br, r0, im)		_bungti_f(_jit,br,r0,im)
static jit_word_t _bungti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bltgtr_f(br, r0, r1)		_bltgtr_f(_jit,br,r0,r1)
static jit_word_t _bltgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti_f(br, r0, im)		_bltgti_f(_jit,br,r0,im)
static jit_word_t _bltgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bordr_f(br, r0, r1)		_bordr_f(_jit,br,r0,r1)
static jit_word_t _bordr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bordi_f(br, r0, im)		_bordi_f(_jit,br,r0,im)
static jit_word_t _bordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define bunordr_f(br, r0, r1)		_bunordr_f(_jit,br,r0,r1)
static jit_word_t _bunordr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunordi_f(br, r0, im)		_bunordi_f(_jit,br,r0,im)
static jit_word_t _bunordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define addr_d(r0, r1, r2)		FADD_D(r0, r1, r2)
#  define addi_d(r0, r1, im)		_addi_d(_jit, r0, r1, im)
static void _addi_d(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float64_t);
#  define subr_d(r0, r1, r2)		FSUB_D(r0, r1, r2)
#  define subi_d(r0, r1, im)		_subi_d(_jit, r0, r1, im)
static void _subi_d(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float64_t);
#  define rsbr_d(r0, r1, r2)		FSUB_D(r0, r2, r1)
#  define rsbi_d(r0, r1, im)		_rsbi_d(_jit, r0, r1, im)
static void _rsbi_d(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float64_t);
#  define mulr_d(r0, r1, r2)		FMUL_D(r0, r1, r2)
#  define muli_d(r0, r1, im)		_muli_d(_jit, r0, r1, im)
static void _muli_d(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float64_t);
#  define divr_d(r0, r1, r2)		FDIV_D(r0, r1, r2)
#  define divi_d(r0, r1, im)		_divi_d(_jit, r0, r1, im)
static void _divi_d(jit_state_t *_jit,jit_int32_t,jit_int32_t,jit_float64_t);
#  define absr_d(r0, r1)		FABS_D(r0, r1)
#  define negr_d(r0, r1)		FNEG_D(r0, r1)
#  define sqrtr_d(r0, r1)		FSQRT_D(r0, r1)
#  define extr_d(r0, r1)		FCVT_D_L(r0, r1)
#  define ldr_d(r0, r1)			FLD(r0, r1, 0)
#  define ldi_d(r0, im)			_ldi_d(_jit, r0, im)
static void _ldi_d(jit_state_t*, jit_int32_t, jit_word_t);
#  define ldxr_d(r0, r1, r2)		_ldxr_d(_jit, r0, r1, r2)
static void _ldxr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ldxi_d(r0, r1, i0)		_ldxi_d(_jit, r0, r1, i0)
static void _ldxi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define str_d(r0, r1)			FSD(r0, r1, 0)
#  define sti_d(im, r0)			_sti_d(_jit, im, r0)
static void _sti_d(jit_state_t*, jit_word_t, jit_int32_t);
#  define stxr_d(r0, r1, r2)		_stxr_d(_jit, r0, r1, r2)
static void _stxr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define stxi_d(im, r0, r1)		_stxi_d(_jit, im, r0, r1)
static void _stxi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define movr_d(r0, r1)		FMV_D(r0, r1)
#  define movi_d(r0, im)		_movi_d(_jit, r0, im)
static void _movi_d(jit_state_t*, jit_int32_t, jit_float64_t);
#  define movr_d_w(r0, r1)		FMV_X_D(r0, r1)
#  define movi_d_w(r0, im)		_movi_d_w(_jit, r0, im)
static void _movi_d_w(jit_state_t*, jit_int32_t, jit_float64_t);
#  define movr_w_d(r0, r1)		FMV_D_X(r0, r1)
#  define extr_f_d(r0, r1)		FCVT_D_S(r0, r1)
#  define ltr_d(r0, r1, r2)		FLT_D(r0, r1, r2)
#  define lti_d(r0, r1, r2)		_lti_d(_jit, r0, r1, r2)
static void _lti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define ler_d(r0, r1, r2)		FLE_D(r0, r1, r2)
#  define lei_d(r0, r1, r2)		_lei_d(_jit, r0, r1, r2)
static void _lei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define eqr_d(r0, r1, r2)		FEQ_D(r0, r1, r2)
#  define eqi_d(r0, r1, r2)		_eqi_d(_jit, r0, r1, r2)
static void _eqi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define ger_d(r0, r1, r2)		FLE_D(r0, r2, r1)
#  define gei_d(r0, r1, r2)		_gei_d(_jit, r0, r1, r2)
static void _gei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define gtr_d(r0, r1, r2)		FLT_D(r0, r2, r1)
#  define gti_d(r0, r1, r2)		_gti_d(_jit, r0, r1, r2)
static void _gti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define ner_d(r0, r1, r2)		_ner_d(_jit, r0, r1, r2)
static void _ner_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define nei_d(r0, r1, r2)		_nei_d(_jit, r0, r1, r2)
static void _nei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define unltr_d(r0, r1, r2)		_unltr_d(_jit, r0, r1, r2)
static void _unltr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define unlti_d(r0, r1, im)		_unlti_d(_jit, r0, r1, im)
static void _unlti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define unler_d(r0, r1, r2)		_unler_d(_jit, r0, r1, r2)
static void _unler_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define unlei_d(r0, r1, im)		_unlei_d(_jit, r0, r1, im)
static void _unlei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define uneqr_d(r0, r1, r2)		_uneqr_d(_jit, r0, r1, r2)
static void _uneqr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define uneqi_d(r0, r1, im)		_uneqi_d(_jit, r0, r1, im)
static void _uneqi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define unger_d(r0, r1, r2)		_unger_d(_jit, r0, r1, r2)
static void _unger_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ungei_d(r0, r1, im)		_ungei_d(_jit, r0, r1, im)
static void _ungei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define ungtr_d(r0, r1, r2)		_ungtr_d(_jit, r0, r1, r2)
static void _ungtr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ungti_d(r0, r1, im)		_ungti_d(_jit, r0, r1, im)
static void _ungti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define ltgtr_d(r0, r1, r2)		_ltgtr_d(_jit, r0, r1, r2)
static void _ltgtr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ltgti_d(r0, r1, im)		_ltgti_d(_jit, r0, r1, im)
static void _ltgti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define ordr_d(r0, r1, r2)		_ordr_d(_jit, r0, r1, r2)
static void _ordr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ordi_d(r0, r1, im)		_ordi_d(_jit, r0, r1, im)
static void _ordi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define unordr_d(r0, r1, r2)		_unordr_d(_jit, r0, r1, r2)
static void _unordr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define unordi_d(r0, r1, im)		_unordi_d(_jit, r0, r1, im)
static void _unordi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
#  define bltr_d(br, r0, r1)		_bltr_d(_jit,br,r0,r1)
static jit_word_t _bltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti_d(br, r0, im)		_blti_d(_jit,br,r0,im)
static jit_word_t _blti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bler_d(br, r0, r1)		_bler_d(_jit,br,r0,r1)
static jit_word_t _bler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei_d(br, r0, im)		_blei_d(_jit,br,r0,im)
static jit_word_t _blei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define beqr_d(br, r0, r1)		_beqr_d(_jit,br,r0,r1)
static jit_word_t _beqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define beqi_d(br, r0, im)		_beqi_d(_jit,br,r0,im)
static jit_word_t _beqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bger_d(br, r0, r1)		_bger_d(_jit,br,r0,r1)
static jit_word_t _bger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei_d(br, r0, im)		_bgei_d(_jit,br,r0,im)
static jit_word_t _bgei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bgtr_d(br, r0, r1)		_bgtr_d(_jit,br,r0,r1)
static jit_word_t _bgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti_d(br, r0, im)		_bgti_d(_jit,br,r0,im)
static jit_word_t _bgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bner_d(br, r0, r1)		_bner_d(_jit,br,r0,r1)
static jit_word_t _bner_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bnei_d(br, r0, im)		_bnei_d(_jit,br,r0,im)
static jit_word_t _bnei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bunltr_d(br, r0, r1)		_bunltr_d(_jit,br,r0,r1)
static jit_word_t _bunltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlti_d(br, r0, im)		_bunlti_d(_jit,br,r0,im)
static jit_word_t _bunlti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bunler_d(br, r0, r1)		_bunler_d(_jit,br,r0,r1)
static jit_word_t _bunler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlei_d(br, r0, im)		_bunlei_d(_jit,br,r0,im)
static jit_word_t _bunlei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define buneqr_d(br, r0, r1)		_buneqr_d(_jit,br,r0,r1)
static jit_word_t _buneqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi_d(br, r0, im)		_buneqi_d(_jit,br,r0,im)
static jit_word_t _buneqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bunger_d(br, r0, r1)		_bunger_d(_jit,br,r0,r1)
static jit_word_t _bunger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungei_d(br, r0, im)		_bungei_d(_jit,br,r0,im)
static jit_word_t _bungei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bungtr_d(br, r0, r1)		_bungtr_d(_jit,br,r0,r1)
static jit_word_t _bungtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungti_d(br, r0, im)		_bungti_d(_jit,br,r0,im)
static jit_word_t _bungti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bltgtr_d(br, r0, r1)		_bltgtr_d(_jit,br,r0,r1)
static jit_word_t _bltgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti_d(br, r0, im)		_bltgti_d(_jit,br,r0,im)
static jit_word_t _bltgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bordr_d(br, r0, r1)		_bordr_d(_jit,br,r0,r1)
static jit_word_t _bordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bordi_d(br, r0, im)		_bordi_d(_jit,br,r0,im)
static jit_word_t _bordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define bunordr_d(br, r0, r1)		_bunordr_d(_jit,br,r0,r1)
static jit_word_t _bunordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunordi_d(br, r0, im)		_bunordi_d(_jit,br,r0,im)
static jit_word_t _bunordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif /* PROTO */

#if CODE
#  define fpr_opi(name, type, size)					\
static void								\
_##name##i_##type(jit_state_t *_jit,					\
		  jit_int32_t r0, jit_int32_t r1,			\
		  jit_float##size##_t i0)				\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    movi_##type(rn(reg), i0);						\
    name##r_##type(r0, r1, rn(reg));					\
    jit_unget_reg(reg);							\
}
#  define fopi(name)			fpr_opi(name, f, 32)
#  define dopi(name)			fpr_opi(name, d, 64)

fopi(add)
fopi(sub)
fopi(rsb)
fopi(mul)
fopi(div)

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FLW(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ldr_f(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    addr(rn(t0), r1, r2);
    ldr_f(r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FLW(r0, r1, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	addi(rn(t0), r1, i0);
	ldr_f(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FSW(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	str_f(rn(t0), r0);
	jit_unget_reg(t0);
    }
}

static void
_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    addr(rn(t0), r0, r1);
    str_f(rn(t0), r2);
    jit_unget_reg(t0);
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FSW(r0, r1, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	addi(rn(t0), r0, i0);
	str_f(rn(t0), r1);
	jit_unget_reg(t0);
    }
}

static void
_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t i0)
{
    union {
	jit_int32_t	 i;
	jit_float32_t	 f;
    } data;
    jit_int32_t		 reg;
    data.f = i0;
    if (data.i == 0)
	movr_w_f(r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.i);
	movr_w_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_movi_f_w(jit_state_t *_jit, jit_int32_t r0, jit_float32_t i0)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    data.f = i0;
    movi(r0, data.i);
}

fopi(lt)
fopi(le)
fopi(eq)
fopi(ge)
fopi(gt)

static void
_ner_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    eqr_f(r0, r1, r2);
    xori(r0, r0, 1);
}
fopi(ne)

#  define fpr_bopi(name, type, size)					\
static jit_word_t							\
_b##name##i_##type(jit_state_t *_jit,					\
		  jit_word_t i0, jit_int32_t r0,			\
		  jit_float##size##_t i1)				\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    movi_##type(rn(reg), i1);						\
    word = b##name##r_##type(i0, r0, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define fbopi(name)			fpr_bopi(name, f, 32)
#  define dbopi(name)			fpr_bopi(name, d, 64)

#  define unop(CLASS, OP)						\
    jit_word_t		w;						\
    jit_int32_t		t0, t1;						\
    t0 = jit_get_reg(jit_class_gpr);					\
    FCLASS_##CLASS(rn(t0), r1);						\
    t1 = jit_get_reg(jit_class_gpr);					\
    FCLASS_##CLASS(rn(t1), r2);						\
    orr(rn(t0), rn(t0), rn(t1));					\
    jit_unget_reg(t1);							\
    rshi(rn(t0), rn(t0), 8);						\
    ltr(r0, _ZERO_REGNO, rn(t0));					\
    jit_unget_reg(t0);							\
    w = _jit->pc.w;							\
    BLT(_ZERO_REGNO, r0, 0);						\
    OP(r0, r1, r2);							\
    patch_at(w, _jit->pc.w)

static void
_unltr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(S, ltr_f);
}
fopi(unlt)

static void
_unler_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(S, ler_f);
}
fopi(unle)

static void
_uneqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(S, eqr_f);
}
fopi(uneq)

static void
_unger_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(S, ger_f);
}
fopi(unge)

static void
_ungtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(S, gtr_f);
}
fopi(ungt)

static void
_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w0, w1;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr);
    FCLASS_S(rn(t0), r1);
    t1 = jit_get_reg(jit_class_gpr);
    FCLASS_S(rn(t1), r2);
    orr(rn(t0), rn(t0), rn(t1));
    jit_unget_reg(t1);
    rshi(rn(t0), rn(t0), 8);
    ltr(r0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    w0 = _jit->pc.w;
    BEQ(_ZERO_REGNO, r0, 0);
    movr(r0, _ZERO_REGNO);
    w1 = _jit->pc.w;
    JAL(_ZERO_REGNO, 0);
    patch_at(w0, _jit->pc.w);
    ner_f(r0, r1, r2);
    patch_at(w1, _jit->pc.w);
}
fopi(ltgt)

static void
_ordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0, t1;	
    t0 = jit_get_reg(jit_class_gpr);
    FCLASS_S(rn(t0), r1);
    t1 = jit_get_reg(jit_class_gpr);
    FCLASS_S(rn(t1), r2);
    orr(rn(t0), rn(t0), rn(t1));
    jit_unget_reg(t1);
    rshi(rn(t0), rn(t0), 8);
    eqr(r0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
}
fopi(ord)

static void
_unordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0, t1;	
    t0 = jit_get_reg(jit_class_gpr);
    FCLASS_S(rn(t0), r1);
    t1 = jit_get_reg(jit_class_gpr);
    FCLASS_S(rn(t1), r2);
    orr(rn(t0), rn(t0), rn(t1));
    jit_unget_reg(t1);
    rshi(rn(t0), rn(t0), 8);
    ltr(r0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
}
fopi(unord)

static jit_word_t
_bltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ltr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(lt)

static jit_word_t
_bler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ler_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(le)

static jit_word_t
_beqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    eqr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(eq)

static jit_word_t
_bger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ger_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(ge)

static jit_word_t
_bgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    gtr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(gt)

static jit_word_t
_bner_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    eqr_f(rn(t0), r1, r2);
    w = beqr(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(ne)

static jit_word_t
_bunltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unltr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(unlt)

static jit_word_t
_bunler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unler_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(unle)

static jit_word_t
_buneqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    uneqr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(uneq)

static jit_word_t
_bunger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unger_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(unge)

static jit_word_t
_bungtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ungtr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(ungt)

static jit_word_t
_bltgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ltgtr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(ltgt)

static jit_word_t
_bordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ordr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(ord)

static jit_word_t
_bunordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unordr_f(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
fbopi(unord)

dopi(add)
dopi(sub)
dopi(rsb)
dopi(mul)
dopi(div)

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FLD(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ldr_d(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    addr(rn(t0), r1, r2);
    ldr_d(r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FLD(r0, r1, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	addi(rn(t0), r1, i0);
	ldr_d(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FSD(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	str_d(rn(t0), r0);
	jit_unget_reg(t0);
    }
}

static void
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    addr(rn(t0), r0, r1);
    str_d(rn(t0), r2);
    jit_unget_reg(t0);
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    if (simm12_p(i0))
	FSD(r0, r1, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	addi(rn(t0), r0, i0);
	str_d(rn(t0), r1);
	jit_unget_reg(t0);
    }
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t i0)
{
    union {
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;
    data.d = i0;
    if (data.w == 0)
	movr_w_d(r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.w);
	movr_w_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_movi_d_w(jit_state_t *_jit, jit_int32_t r0, jit_float64_t i0)
{
    union {
	jit_int64_t	l;
	jit_float64_t	d;
    } data;
    data.d = i0;
    movi(r0, data.l);
}

dopi(lt)
dopi(le)
dopi(eq)
dopi(ge)
dopi(gt)

static void
_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    eqr_d(r0, r1, r2);
    xori(r0, r0, 1);
}
dopi(ne)

static void
_unltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(D, ltr_d);
}
dopi(unlt)

static void
_unler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(D, ler_d);
}
dopi(unle)

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(D, eqr_d);
}
dopi(uneq)

static void
_unger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(D, ger_d);
}
dopi(unge)

static void
_ungtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    unop(D, gtr_d);
}
dopi(ungt)

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w0, w1;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr);
    FCLASS_D(rn(t0), r1);
    t1 = jit_get_reg(jit_class_gpr);
    FCLASS_D(rn(t1), r2);
    orr(rn(t0), rn(t0), rn(t1));
    jit_unget_reg(t1);
    rshi(rn(t0), rn(t0), 8);
    ltr(r0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    w0 = _jit->pc.w;
    BEQ(_ZERO_REGNO, r0, 0);
    movr(r0, _ZERO_REGNO);
    w1 = _jit->pc.w;
    JAL(_ZERO_REGNO, 0);
    patch_at(w0, _jit->pc.w);
    ner_d(r0, r1, r2);
    patch_at(w1, _jit->pc.w);
}
dopi(ltgt)

static void
_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0, t1;	
    t0 = jit_get_reg(jit_class_gpr);
    FCLASS_D(rn(t0), r1);
    t1 = jit_get_reg(jit_class_gpr);
    FCLASS_D(rn(t1), r2);
    orr(rn(t0), rn(t0), rn(t1));
    jit_unget_reg(t1);
    rshi(rn(t0), rn(t0), 8);
    eqr(r0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
}
dopi(ord)

static void
_unordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0, t1;	
    t0 = jit_get_reg(jit_class_gpr);
    FCLASS_D(rn(t0), r1);
    t1 = jit_get_reg(jit_class_gpr);
    FCLASS_D(rn(t1), r2);
    orr(rn(t0), rn(t0), rn(t1));
    jit_unget_reg(t1);
    rshi(rn(t0), rn(t0), 8);
    ltr(r0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
}
dopi(unord)

static jit_word_t
_bltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ltr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(lt)

static jit_word_t
_bler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ler_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(le)

static jit_word_t
_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    eqr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(eq)

static jit_word_t
_bger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ger_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(ge)

static jit_word_t
_bgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    gtr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(gt)

static jit_word_t
_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    eqr_d(rn(t0), r1, r2);
    w = beqr(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(ne)

static jit_word_t
_bunltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unltr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(unlt)

static jit_word_t
_bunler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unler_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(unle)

static jit_word_t
_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    uneqr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(uneq)

static jit_word_t
_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unger_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(unge)

static jit_word_t
_bungtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ungtr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(ungt)

static jit_word_t
_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ltgtr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(ltgt)

static jit_word_t
_bordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    ordr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(ord)

static jit_word_t
_bunordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    unordr_d(rn(t0), r1, r2);
    w = bner(i0, _ZERO_REGNO, rn(t0));
    jit_unget_reg(t0);
    return (w);
}
dbopi(unord)

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Load argument. */
    ldr_d(r0, r1);
    /* Update va_list. */
    addi(r1, r1, sizeof(jit_float64_t));
}

#endif /* CODE */
