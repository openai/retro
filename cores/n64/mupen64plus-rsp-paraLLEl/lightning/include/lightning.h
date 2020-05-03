/*
 * Copyright (C) 2012-2019  Free Software Foundation, Inc.
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

#ifndef _lightning_h
#define _lightning_h

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#if defined(__hpux) && defined(__hppa__)
#  include <machine/param.h>
#endif
#if defined(__alpha__) && defined(__osf__)
#  include <machine/endian.h>
#endif

#ifndef __WORDSIZE
#  if defined(WORDSIZE)				/* ppc darwin */
#    define __WORDSIZE		WORDSIZE
#  elif defined(__SIZEOF_POINTER__)		/* ppc aix */
#    define __WORDSIZE		(__SIZEOF_POINTER__ << 3)
#  elif defined(_ILP32)				/* hppa hp-ux */
#    define __WORDSIZE		32
#  elif defined(_LP64)				/* ia64 hp-ux (with cc +DD64) */
#    define __WORDSIZE		64
#  elif defined(_MIPS_SZPTR)			/* mips irix */
#    if _MIPS_SZPTR == 32
#      define __WORDSIZE	32
#    else
#      define __WORDSIZE	64
#    endif
#  else						/* From FreeBSD 9.1 stdint.h */
#    if defined(UINTPTR_MAX) && defined(UINT64_MAX) && \
	(UINTPTR_MAX == UINT64_MAX)
#      define __WORDSIZE	64
#    else
#      define __WORDSIZE	32
#    endif
#  endif
#endif
#ifndef __LITTLE_ENDIAN
#  if defined(LITTLE_ENDIAN)			/* ppc darwin */
#    define __LITTLE_ENDIAN	LITTLE_ENDIAN
#  elif defined(__ORDER_LITTLE_ENDIAN__)	/* ppc aix */
#    define __LITTLE_ENDIAN	__ORDER_LITTLE_ENDIAN__
#  else
#    define __LITTLE_ENDIAN	1234
#  endif
#endif
#ifndef __BIG_ENDIAN
#  if defined(BIG_ENDIAN)			/* ppc darwin */
#    define __BIG_ENDIAN	BIG_ENDIAN
#  elif defined(__ORDER_BIG_ENDIAN__)		/* ppc aix */
#    define __BIG_ENDIAN	__ORDER_BIG_ENDIAN__
#  else
#    define __BIG_ENDIAN	4321
#  endif
#endif
#ifndef __BYTE_ORDER
#  if defined(BYTE_ORDER)			/* ppc darwin */
#    define __BYTE_ORDER	BYTE_ORDER
#  elif defined(__BYTE_ORDER__)			/* ppc aix */
#    define __BYTE_ORDER	__BYTE_ORDER__
#  elif defined(_BIG_ENDIAN)			/* hppa hp-ux */
#    define __BYTE_ORDER	__BIG_ENDIAN
#  elif defined(__BIG_ENDIAN__)			/* ia64 hp-ux */
#    define __BYTE_ORDER	__BIG_ENDIAN
#  elif defined(__i386__)			/* 32 bit x86 solaris */
#    define __BYTE_ORDER	__LITTLE_ENDIAN
#  elif defined(__x86_64__)			/* 64 bit x86 solaris */
#    define __BYTE_ORDER	__LITTLE_ENDIAN
#  elif defined(__MIPSEB)			/* mips irix */
#    define __BYTE_ORDER	__BIG_ENDIAN
#  else
#    error cannot figure __BYTE_ORDER
#  endif
#endif

typedef signed char		jit_int8_t;
typedef unsigned char		jit_uint8_t;
typedef signed short		jit_int16_t;
typedef unsigned short		jit_uint16_t;
typedef signed int		jit_int32_t;
typedef unsigned int		jit_uint32_t;
#if __WORDSIZE == 32
typedef signed long long	jit_int64_t;
typedef unsigned long long	jit_uint64_t;
typedef jit_int32_t		jit_word_t;
typedef jit_uint32_t		jit_uword_t;
#elif (_WIN32 && !__CYGWIN__)
typedef signed long long	jit_int64_t;
typedef unsigned long long	jit_uint64_t;
typedef jit_int64_t		jit_word_t;
typedef jit_uint64_t		jit_uword_t;
#else
typedef signed long		jit_int64_t;
typedef unsigned long		jit_uint64_t;
typedef jit_int64_t		jit_word_t;
typedef jit_uint64_t		jit_uword_t;
#endif
typedef float			jit_float32_t;
typedef double			jit_float64_t;
typedef void*			jit_pointer_t;
typedef jit_int32_t		jit_bool_t;
typedef jit_int32_t		jit_gpr_t;
typedef jit_int32_t		jit_fpr_t;

#if defined(__i386__) || defined(__x86_64__)
#  include <lightning/jit_x86.h>
#elif defined(__mips__)
#  include <lightning/jit_mips.h>
#elif defined(__arm__)
#  include <lightning/jit_arm.h>
#elif defined(__powerpc__)
#  include <lightning/jit_ppc.h>
#elif defined(__sparc__)
#  include <lightning/jit_sparc.h>
#elif defined(__ia64__)
#  include <lightning/jit_ia64.h>
#elif defined(__hppa__)
#  include <lightning/jit_hppa.h>
#elif defined(__aarch64__)
#  include <lightning/jit_aarch64.h>
#elif defined(__s390__) || defined(__s390x__)
#  include <lightning/jit_s390.h>
#elif defined(__alpha__)
#  include <lightning/jit_alpha.h>
#elif defined(__riscv)
#  include <lightning/jit_riscv.h>
#endif

#define jit_flag_node		0x0001	/* patch node not absolute */
#define jit_flag_patch		0x0002	/* jump already patched */
#define jit_flag_data		0x0004	/* data in the constant pool */
#define jit_flag_use		0x0008	/* do not remove marker label */
#define jit_flag_synth		0x0010	/* synthesized instruction */
#define jit_flag_head		0x1000	/* label reached by normal flow */
#define jit_flag_varargs	0x2000	/* call{r,i} to varargs function */

#define JIT_R(index)		jit_r(index)
#define JIT_V(index)		jit_v(index)
#define JIT_F(index)		jit_f(index)
#define JIT_R_NUM		jit_r_num()
#define JIT_V_NUM		jit_v_num()
#define JIT_F_NUM		jit_f_num()

#define JIT_DISABLE_DATA	1	/* force synthesize of constants */
#define JIT_DISABLE_NOTE	2	/* disable debug info generation */

#define jit_class_chk		0x02000000	/* just checking */
#define jit_class_arg		0x08000000	/* argument register */
#define jit_class_sav		0x10000000	/* callee save */
#define jit_class_gpr		0x20000000	/* general purpose */
#define jit_class_fpr		0x40000000	/* float */
#define jit_class(reg)		((reg) & 0xffff0000)
#define jit_regno(reg)		((reg) & 0x00007fff)

typedef struct jit_node		jit_node_t;
typedef struct jit_state	jit_state_t;

typedef enum {
    jit_code_data,
#define jit_live(u)		jit_new_node_w(jit_code_live, u)
#define jit_align(u)		jit_new_node_w(jit_code_align, u)
    jit_code_live,		jit_code_align,
    jit_code_save,		jit_code_load,
#define jit_name(u)		_jit_name(_jit,u)
    jit_code_name,
#define jit_note(u, v)		_jit_note(_jit, u, v)
#define jit_label()		_jit_label(_jit)
#define jit_forward()		_jit_forward(_jit)
#define jit_indirect()		_jit_indirect(_jit)
#define jit_link(u)		_jit_link(_jit,u)
    jit_code_note,		jit_code_label,

#define jit_prolog()		_jit_prolog(_jit)
    jit_code_prolog,

#define jit_ellipsis()		_jit_ellipsis(_jit)
    jit_code_ellipsis,
#define jit_va_push(u)		_jit_va_push(_jit,u)
    jit_code_va_push,
#define jit_allocai(u)		_jit_allocai(_jit,u)
#define jit_allocar(u, v)	_jit_allocar(_jit,u,v)
    jit_code_allocai,		jit_code_allocar,

#define jit_arg()		_jit_arg(_jit)
    jit_code_arg,
#define jit_getarg_c(u,v)	_jit_getarg_c(_jit,u,v)
#define jit_getarg_uc(u,v)	_jit_getarg_uc(_jit,u,v)
    jit_code_getarg_c,		jit_code_getarg_uc,
#define jit_getarg_s(u,v)	_jit_getarg_s(_jit,u,v)
#define jit_getarg_us(u,v)	_jit_getarg_us(_jit,u,v)
    jit_code_getarg_s,		jit_code_getarg_us,
#define jit_getarg_i(u,v)	_jit_getarg_i(_jit,u,v)
#if __WORDSIZE == 32
#  define jit_getarg(u,v)	jit_getarg_i(u,v)
#else
#  define jit_getarg(u,v)	jit_getarg_l(u,v)
#  define jit_getarg_ui(u,v)	_jit_getarg_ui(_jit,u,v)
#  define jit_getarg_l(u,v)	_jit_getarg_l(_jit,u,v)
#endif
    jit_code_getarg_i,		jit_code_getarg_ui,
    jit_code_getarg_l,
#  define jit_putargr(u,v)	_jit_putargr(_jit,u,v)
#  define jit_putargi(u,v)	_jit_putargi(_jit,u,v)
    jit_code_putargr,		jit_code_putargi,

#define jit_va_start(u)		jit_new_node_w(jit_code_va_start, u)
    jit_code_va_start,
#define jit_va_arg(u, v)	jit_new_node_ww(jit_code_va_arg, u, v)
#define jit_va_arg_d(u, v)	jit_new_node_ww(jit_code_va_arg_d, u, v)
    jit_code_va_arg,		jit_code_va_arg_d,
#define jit_va_end(u)		jit_new_node_w(jit_code_va_end, u)
    jit_code_va_end,

#define jit_addr(u,v,w)		jit_new_node_www(jit_code_addr,u,v,w)
#define jit_addi(u,v,w)		jit_new_node_www(jit_code_addi,u,v,w)
    jit_code_addr,		jit_code_addi,
#define jit_addcr(u,v,w)	jit_new_node_www(jit_code_addcr,u,v,w)
#define jit_addci(u,v,w)	jit_new_node_www(jit_code_addci,u,v,w)
    jit_code_addcr,		jit_code_addci,
#define jit_addxr(u,v,w)	jit_new_node_www(jit_code_addxr,u,v,w)
#define jit_addxi(u,v,w)	jit_new_node_www(jit_code_addxi,u,v,w)
    jit_code_addxr,		jit_code_addxi,
#define jit_subr(u,v,w)		jit_new_node_www(jit_code_subr,u,v,w)
#define jit_subi(u,v,w)		jit_new_node_www(jit_code_subi,u,v,w)
    jit_code_subr,		jit_code_subi,
#define jit_subcr(u,v,w)	jit_new_node_www(jit_code_subcr,u,v,w)
#define jit_subci(u,v,w)	jit_new_node_www(jit_code_subci,u,v,w)
    jit_code_subcr,		jit_code_subci,
#define jit_subxr(u,v,w)	jit_new_node_www(jit_code_subxr,u,v,w)
#define jit_subxi(u,v,w)	jit_new_node_www(jit_code_subxi,u,v,w)
    jit_code_subxr,		jit_code_subxi,
#define jit_rsbr(u,v,w)		jit_subr(u,w,v)
#define jit_rsbi(u,v,w)		jit_new_node_www(jit_code_rsbi,u,v,w)
    jit_code_rsbi,
#define jit_mulr(u,v,w)		jit_new_node_www(jit_code_mulr,u,v,w)
#define jit_muli(u,v,w)		jit_new_node_www(jit_code_muli,u,v,w)
    jit_code_mulr,		jit_code_muli,
#define jit_qmulr(l,h,v,w)	jit_new_node_qww(jit_code_qmulr,l,h,v,w)
#define jit_qmuli(l,h,v,w)	jit_new_node_qww(jit_code_qmuli,l,h,v,w)
    jit_code_qmulr,		jit_code_qmuli,
#define jit_qmulr_u(l,h,v,w)	jit_new_node_qww(jit_code_qmulr_u,l,h,v,w)
#define jit_qmuli_u(l,h,v,w)	jit_new_node_qww(jit_code_qmuli_u,l,h,v,w)
    jit_code_qmulr_u,		jit_code_qmuli_u,
#define jit_divr(u,v,w)		jit_new_node_www(jit_code_divr,u,v,w)
#define jit_divi(u,v,w)		jit_new_node_www(jit_code_divi,u,v,w)
    jit_code_divr,		jit_code_divi,
#define jit_divr_u(u,v,w)	jit_new_node_www(jit_code_divr_u,u,v,w)
#define jit_divi_u(u,v,w)	jit_new_node_www(jit_code_divi_u,u,v,w)
    jit_code_divr_u,		jit_code_divi_u,
#define jit_qdivr(l,h,v,w)	jit_new_node_qww(jit_code_qdivr,l,h,v,w)
#define jit_qdivi(l,h,v,w)	jit_new_node_qww(jit_code_qdivi,l,h,v,w)
    jit_code_qdivr,		jit_code_qdivi,
#define jit_qdivr_u(l,h,v,w)	jit_new_node_qww(jit_code_qdivr_u,l,h,v,w)
#define jit_qdivi_u(l,h,v,w)	jit_new_node_qww(jit_code_qdivi_u,l,h,v,w)
    jit_code_qdivr_u,		jit_code_qdivi_u,
#define jit_remr(u,v,w)		jit_new_node_www(jit_code_remr,u,v,w)
#define jit_remi(u,v,w)		jit_new_node_www(jit_code_remi,u,v,w)
    jit_code_remr,		jit_code_remi,
#define jit_remr_u(u,v,w)	jit_new_node_www(jit_code_remr_u,u,v,w)
#define jit_remi_u(u,v,w)	jit_new_node_www(jit_code_remi_u,u,v,w)
    jit_code_remr_u,		jit_code_remi_u,

#define jit_andr(u,v,w)		jit_new_node_www(jit_code_andr,u,v,w)
#define jit_andi(u,v,w)		jit_new_node_www(jit_code_andi,u,v,w)
    jit_code_andr,		jit_code_andi,
#define jit_orr(u,v,w)		jit_new_node_www(jit_code_orr,u,v,w)
#define jit_ori(u,v,w)		jit_new_node_www(jit_code_ori,u,v,w)
    jit_code_orr,		jit_code_ori,
#define jit_xorr(u,v,w)		jit_new_node_www(jit_code_xorr,u,v,w)
#define jit_xori(u,v,w)		jit_new_node_www(jit_code_xori,u,v,w)
    jit_code_xorr,		jit_code_xori,

#define jit_lshr(u,v,w)		jit_new_node_www(jit_code_lshr,u,v,w)
#define jit_lshi(u,v,w)		jit_new_node_www(jit_code_lshi,u,v,w)
    jit_code_lshr,		jit_code_lshi,
#define jit_rshr(u,v,w)		jit_new_node_www(jit_code_rshr,u,v,w)
#define jit_rshi(u,v,w)		jit_new_node_www(jit_code_rshi,u,v,w)
    jit_code_rshr,		jit_code_rshi,
#define jit_rshr_u(u,v,w)	jit_new_node_www(jit_code_rshr_u,u,v,w)
#define jit_rshi_u(u,v,w)	jit_new_node_www(jit_code_rshi_u,u,v,w)
    jit_code_rshr_u,		jit_code_rshi_u,

#define jit_negr(u,v)		jit_new_node_ww(jit_code_negr,u,v)
#define jit_comr(u,v)		jit_new_node_ww(jit_code_comr,u,v)
    jit_code_negr,		jit_code_comr,

#define jit_ltr(u,v,w)		jit_new_node_www(jit_code_ltr,u,v,w)
#define jit_lti(u,v,w)		jit_new_node_www(jit_code_lti,u,v,w)
    jit_code_ltr,		jit_code_lti,
#define jit_ltr_u(u,v,w)	jit_new_node_www(jit_code_ltr_u,u,v,w)
#define jit_lti_u(u,v,w)	jit_new_node_www(jit_code_lti_u,u,v,w)
    jit_code_ltr_u,		jit_code_lti_u,
#define jit_ler(u,v,w)		jit_new_node_www(jit_code_ler,u,v,w)
#define jit_lei(u,v,w)		jit_new_node_www(jit_code_lei,u,v,w)
    jit_code_ler,		jit_code_lei,
#define jit_ler_u(u,v,w)	jit_new_node_www(jit_code_ler_u,u,v,w)
#define jit_lei_u(u,v,w)	jit_new_node_www(jit_code_lei_u,u,v,w)
    jit_code_ler_u,		jit_code_lei_u,
#define jit_eqr(u,v,w)		jit_new_node_www(jit_code_eqr,u,v,w)
#define jit_eqi(u,v,w)		jit_new_node_www(jit_code_eqi,u,v,w)
    jit_code_eqr,		jit_code_eqi,
#define jit_ger(u,v,w)		jit_new_node_www(jit_code_ger,u,v,w)
#define jit_gei(u,v,w)		jit_new_node_www(jit_code_gei,u,v,w)
    jit_code_ger,		jit_code_gei,
#define jit_ger_u(u,v,w)	jit_new_node_www(jit_code_ger_u,u,v,w)
#define jit_gei_u(u,v,w)	jit_new_node_www(jit_code_gei_u,u,v,w)
    jit_code_ger_u,		jit_code_gei_u,
#define jit_gtr(u,v,w)		jit_new_node_www(jit_code_gtr,u,v,w)
#define jit_gti(u,v,w)		jit_new_node_www(jit_code_gti,u,v,w)
    jit_code_gtr,		jit_code_gti,
#define jit_gtr_u(u,v,w)	jit_new_node_www(jit_code_gtr_u,u,v,w)
#define jit_gti_u(u,v,w)	jit_new_node_www(jit_code_gti_u,u,v,w)
    jit_code_gtr_u,		jit_code_gti_u,
#define jit_ner(u,v,w)		jit_new_node_www(jit_code_ner,u,v,w)
#define jit_nei(u,v,w)		jit_new_node_www(jit_code_nei,u,v,w)
    jit_code_ner,		jit_code_nei,

#define jit_movr(u,v)		jit_new_node_ww(jit_code_movr,u,v)
#define jit_movi(u,v)		jit_new_node_ww(jit_code_movi,u,v)
    jit_code_movr,		jit_code_movi,
#define jit_extr_c(u,v)		jit_new_node_ww(jit_code_extr_c,u,v)
#define jit_extr_uc(u,v)	jit_new_node_ww(jit_code_extr_uc,u,v)
    jit_code_extr_c,		jit_code_extr_uc,
#define jit_extr_s(u,v)		jit_new_node_ww(jit_code_extr_s,u,v)
#define jit_extr_us(u,v)	jit_new_node_ww(jit_code_extr_us,u,v)
    jit_code_extr_s,		jit_code_extr_us,
#if __WORDSIZE == 64
#  define jit_extr_i(u,v)	jit_new_node_ww(jit_code_extr_i,u,v)
#  define jit_extr_ui(u,v)	jit_new_node_ww(jit_code_extr_ui,u,v)
#endif
    jit_code_extr_i,		jit_code_extr_ui,

#define jit_htonr_us(u,v)	jit_new_node_ww(jit_code_htonr_us,u,v)
#define jit_ntohr_us(u,v)	jit_new_node_ww(jit_code_htonr_us,u,v)
    jit_code_htonr_us,
#define jit_htonr_ui(u,v)	jit_new_node_ww(jit_code_htonr_ui,u,v)
#define jit_ntohr_ui(u,v)	jit_new_node_ww(jit_code_htonr_ui,u,v)
#if __WORDSIZE == 32
#  define jit_htonr(u,v)	jit_new_node_ww(jit_code_htonr_ui,u,v)
#  define jit_ntohr(u,v)	jit_new_node_ww(jit_code_htonr_ui,u,v)
#else
#define jit_htonr_ul(u,v)	jit_new_node_ww(jit_code_htonr_ul,u,v)
#define jit_ntohr_ul(u,v)	jit_new_node_ww(jit_code_htonr_ul,u,v)
#  define jit_htonr(u,v)	jit_new_node_ww(jit_code_htonr_ul,u,v)
#  define jit_ntohr(u,v)	jit_new_node_ww(jit_code_htonr_ul,u,v)
#endif
    jit_code_htonr_ui,		jit_code_htonr_ul,

#define jit_ldr_c(u,v)		jit_new_node_ww(jit_code_ldr_c,u,v)
#define jit_ldi_c(u,v)		jit_new_node_wp(jit_code_ldi_c,u,v)
    jit_code_ldr_c,		jit_code_ldi_c,
#define jit_ldr_uc(u,v)		jit_new_node_ww(jit_code_ldr_uc,u,v)
#define jit_ldi_uc(u,v)		jit_new_node_wp(jit_code_ldi_uc,u,v)
    jit_code_ldr_uc,		jit_code_ldi_uc,
#define jit_ldr_s(u,v)		jit_new_node_ww(jit_code_ldr_s,u,v)
#define jit_ldi_s(u,v)		jit_new_node_wp(jit_code_ldi_s,u,v)
    jit_code_ldr_s,		jit_code_ldi_s,
#define jit_ldr_us(u,v)		jit_new_node_ww(jit_code_ldr_us,u,v)
#define jit_ldi_us(u,v)		jit_new_node_wp(jit_code_ldi_us,u,v)
    jit_code_ldr_us,		jit_code_ldi_us,
#define jit_ldr_i(u,v)		jit_new_node_ww(jit_code_ldr_i,u,v)
#define jit_ldi_i(u,v)		jit_new_node_wp(jit_code_ldi_i,u,v)
    jit_code_ldr_i,		jit_code_ldi_i,
#if __WORDSIZE == 32
#  define jit_ldr(u,v)		jit_ldr_i(u,v)
#  define jit_ldi(u,v)		jit_ldi_i(u,v)
#else
#  define jit_ldr(u,v)		jit_ldr_l(u,v)
#  define jit_ldi(u,v)		jit_ldi_l(u,v)
#  define jit_ldr_ui(u,v)	jit_new_node_ww(jit_code_ldr_ui,u,v)
#  define jit_ldi_ui(u,v)	jit_new_node_wp(jit_code_ldi_ui,u,v)
#define jit_ldr_l(u,v)		jit_new_node_ww(jit_code_ldr_l,u,v)
#define jit_ldi_l(u,v)		jit_new_node_wp(jit_code_ldi_l,u,v)
#endif
    jit_code_ldr_ui,		jit_code_ldi_ui,
    jit_code_ldr_l,		jit_code_ldi_l,

#define jit_ldxr_c(u,v,w)	jit_new_node_www(jit_code_ldxr_c,u,v,w)
#define jit_ldxi_c(u,v,w)	jit_new_node_www(jit_code_ldxi_c,u,v,w)
    jit_code_ldxr_c,		jit_code_ldxi_c,
#define jit_ldxr_uc(u,v,w)	jit_new_node_www(jit_code_ldxr_uc,u,v,w)
#define jit_ldxi_uc(u,v,w)	jit_new_node_www(jit_code_ldxi_uc,u,v,w)
    jit_code_ldxr_uc,		jit_code_ldxi_uc,
#define jit_ldxr_s(u,v,w)	jit_new_node_www(jit_code_ldxr_s,u,v,w)
#define jit_ldxi_s(u,v,w)	jit_new_node_www(jit_code_ldxi_s,u,v,w)
    jit_code_ldxr_s,		jit_code_ldxi_s,
#define jit_ldxr_us(u,v,w)	jit_new_node_www(jit_code_ldxr_us,u,v,w)
#define jit_ldxi_us(u,v,w)	jit_new_node_www(jit_code_ldxi_us,u,v,w)
    jit_code_ldxr_us,		jit_code_ldxi_us,
#define jit_ldxr_i(u,v,w)	jit_new_node_www(jit_code_ldxr_i,u,v,w)
#define jit_ldxi_i(u,v,w)	jit_new_node_www(jit_code_ldxi_i,u,v,w)
    jit_code_ldxr_i,		jit_code_ldxi_i,
#if __WORDSIZE == 32
#  define jit_ldxr(u,v,w)	jit_ldxr_i(u,v,w)
#  define jit_ldxi(u,v,w)	jit_ldxi_i(u,v,w)
#else
#  define jit_ldxr_ui(u,v,w)	jit_new_node_www(jit_code_ldxr_ui,u,v,w)
#  define jit_ldxi_ui(u,v,w)	jit_new_node_www(jit_code_ldxi_ui,u,v,w)
#  define jit_ldxr_l(u,v,w)	jit_new_node_www(jit_code_ldxr_l,u,v,w)
#  define jit_ldxi_l(u,v,w)	jit_new_node_www(jit_code_ldxi_l,u,v,w)
#  define jit_ldxr(u,v,w)	jit_ldxr_l(u,v,w)
#  define jit_ldxi(u,v,w)	jit_ldxi_l(u,v,w)
#endif
    jit_code_ldxr_ui,		jit_code_ldxi_ui,
    jit_code_ldxr_l,		jit_code_ldxi_l,

#define jit_str_c(u,v)		jit_new_node_ww(jit_code_str_c,u,v)
#define jit_sti_c(u,v)		jit_new_node_pw(jit_code_sti_c,u,v)
    jit_code_str_c,		jit_code_sti_c,
#define jit_str_s(u,v)		jit_new_node_ww(jit_code_str_s,u,v)
#define jit_sti_s(u,v)		jit_new_node_pw(jit_code_sti_s,u,v)
    jit_code_str_s,		jit_code_sti_s,
#define jit_str_i(u,v)		jit_new_node_ww(jit_code_str_i,u,v)
#define jit_sti_i(u,v)		jit_new_node_pw(jit_code_sti_i,u,v)
    jit_code_str_i,		jit_code_sti_i,
#if __WORDSIZE == 32
#  define jit_str(u,v)		jit_str_i(u,v)
#  define jit_sti(u,v)		jit_sti_i(u,v)
#else
#  define jit_str(u,v)		jit_str_l(u,v)
#  define jit_sti(u,v)		jit_sti_l(u,v)
#  define jit_str_l(u,v)	jit_new_node_ww(jit_code_str_l,u,v)
#  define jit_sti_l(u,v)	jit_new_node_pw(jit_code_sti_l,u,v)
#endif
    jit_code_str_l,		jit_code_sti_l,

#define jit_stxr_c(u,v,w)	jit_new_node_www(jit_code_stxr_c,u,v,w)
#define jit_stxi_c(u,v,w)	jit_new_node_www(jit_code_stxi_c,u,v,w)
    jit_code_stxr_c,		jit_code_stxi_c,
#define jit_stxr_s(u,v,w)	jit_new_node_www(jit_code_stxr_s,u,v,w)
#define jit_stxi_s(u,v,w)	jit_new_node_www(jit_code_stxi_s,u,v,w)
    jit_code_stxr_s,		jit_code_stxi_s,
#define jit_stxr_i(u,v,w)	jit_new_node_www(jit_code_stxr_i,u,v,w)
#define jit_stxi_i(u,v,w)	jit_new_node_www(jit_code_stxi_i,u,v,w)
    jit_code_stxr_i,		jit_code_stxi_i,
#if __WORDSIZE == 32
#  define jit_stxr(u,v,w)	jit_stxr_i(u,v,w)
#  define jit_stxi(u,v,w)	jit_stxi_i(u,v,w)
#else
#  define jit_stxr(u,v,w)	jit_stxr_l(u,v,w)
#  define jit_stxi(u,v,w)	jit_stxi_l(u,v,w)
#  define jit_stxr_l(u,v,w)	jit_new_node_www(jit_code_stxr_l,u,v,w)
#  define jit_stxi_l(u,v,w)	jit_new_node_www(jit_code_stxi_l,u,v,w)
#endif
    jit_code_stxr_l,		jit_code_stxi_l,

#define jit_bltr(v,w)		jit_new_node_pww(jit_code_bltr,NULL,v,w)
#define jit_blti(v,w)		jit_new_node_pww(jit_code_blti,NULL,v,w)
    jit_code_bltr,		jit_code_blti,
#define jit_bltr_u(v,w)		jit_new_node_pww(jit_code_bltr_u,NULL,v,w)
#define jit_blti_u(v,w)		jit_new_node_pww(jit_code_blti_u,NULL,v,w)
    jit_code_bltr_u,		jit_code_blti_u,
#define jit_bler(v,w)		jit_new_node_pww(jit_code_bler,NULL,v,w)
#define jit_blei(v,w)		jit_new_node_pww(jit_code_blei,NULL,v,w)
    jit_code_bler,		jit_code_blei,
#define jit_bler_u(v,w)		jit_new_node_pww(jit_code_bler_u,NULL,v,w)
#define jit_blei_u(v,w)		jit_new_node_pww(jit_code_blei_u,NULL,v,w)
    jit_code_bler_u,		jit_code_blei_u,
#define jit_beqr(v,w)		jit_new_node_pww(jit_code_beqr,NULL,v,w)
#define jit_beqi(v,w)		jit_new_node_pww(jit_code_beqi,NULL,v,w)
    jit_code_beqr,		jit_code_beqi,
#define jit_bger(v,w)		jit_new_node_pww(jit_code_bger,NULL,v,w)
#define jit_bgei(v,w)		jit_new_node_pww(jit_code_bgei,NULL,v,w)
    jit_code_bger,		jit_code_bgei,
#define jit_bger_u(v,w)		jit_new_node_pww(jit_code_bger_u,NULL,v,w)
#define jit_bgei_u(v,w)		jit_new_node_pww(jit_code_bgei_u,NULL,v,w)
    jit_code_bger_u,		jit_code_bgei_u,
#define jit_bgtr(v,w)		jit_new_node_pww(jit_code_bgtr,NULL,v,w)
#define jit_bgti(v,w)		jit_new_node_pww(jit_code_bgti,NULL,v,w)
    jit_code_bgtr,		jit_code_bgti,
#define jit_bgtr_u(v,w)		jit_new_node_pww(jit_code_bgtr_u,NULL,v,w)
#define jit_bgti_u(v,w)		jit_new_node_pww(jit_code_bgti_u,NULL,v,w)
    jit_code_bgtr_u,		jit_code_bgti_u,
#define jit_bner(v,w)		jit_new_node_pww(jit_code_bner,NULL,v,w)
#define jit_bnei(v,w)		jit_new_node_pww(jit_code_bnei,NULL,v,w)
    jit_code_bner,		jit_code_bnei,

#define jit_bmsr(v,w)		jit_new_node_pww(jit_code_bmsr,NULL,v,w)
#define jit_bmsi(v,w)		jit_new_node_pww(jit_code_bmsi,NULL,v,w)
    jit_code_bmsr,		jit_code_bmsi,
#define jit_bmcr(v,w)		jit_new_node_pww(jit_code_bmcr,NULL,v,w)
#define jit_bmci(v,w)		jit_new_node_pww(jit_code_bmci,NULL,v,w)
    jit_code_bmcr,		jit_code_bmci,

#define jit_boaddr(v,w)		jit_new_node_pww(jit_code_boaddr,NULL,v,w)
#define jit_boaddi(v,w)		jit_new_node_pww(jit_code_boaddi,NULL,v,w)
    jit_code_boaddr,		jit_code_boaddi,
#define jit_boaddr_u(v,w)	jit_new_node_pww(jit_code_boaddr_u,NULL,v,w)
#define jit_boaddi_u(v,w)	jit_new_node_pww(jit_code_boaddi_u,NULL,v,w)
    jit_code_boaddr_u,		jit_code_boaddi_u,
#define jit_bxaddr(v,w)		jit_new_node_pww(jit_code_bxaddr,NULL,v,w)
#define jit_bxaddi(v,w)		jit_new_node_pww(jit_code_bxaddi,NULL,v,w)
    jit_code_bxaddr,		jit_code_bxaddi,
#define jit_bxaddr_u(v,w)	jit_new_node_pww(jit_code_bxaddr_u,NULL,v,w)
#define jit_bxaddi_u(v,w)	jit_new_node_pww(jit_code_bxaddi_u,NULL,v,w)
    jit_code_bxaddr_u,		jit_code_bxaddi_u,
#define jit_bosubr(v,w)		jit_new_node_pww(jit_code_bosubr,NULL,v,w)
#define jit_bosubi(v,w)		jit_new_node_pww(jit_code_bosubi,NULL,v,w)
    jit_code_bosubr,		jit_code_bosubi,
#define jit_bosubr_u(v,w)	jit_new_node_pww(jit_code_bosubr_u,NULL,v,w)
#define jit_bosubi_u(v,w)	jit_new_node_pww(jit_code_bosubi_u,NULL,v,w)
    jit_code_bosubr_u,		jit_code_bosubi_u,
#define jit_bxsubr(v,w)		jit_new_node_pww(jit_code_bxsubr,NULL,v,w)
#define jit_bxsubi(v,w)		jit_new_node_pww(jit_code_bxsubi,NULL,v,w)
    jit_code_bxsubr,		jit_code_bxsubi,
#define jit_bxsubr_u(v,w)	jit_new_node_pww(jit_code_bxsubr_u,NULL,v,w)
#define jit_bxsubi_u(v,w)	jit_new_node_pww(jit_code_bxsubi_u,NULL,v,w)
    jit_code_bxsubr_u,		jit_code_bxsubi_u,

#define jit_jmpr(u)		jit_new_node_w(jit_code_jmpr,u)
#define jit_jmpi()		jit_new_node_p(jit_code_jmpi,NULL)
    jit_code_jmpr,		jit_code_jmpi,
#define jit_callr(u)		jit_new_node_w(jit_code_callr,u)
#define jit_calli(u)		jit_new_node_p(jit_code_calli,u)
    jit_code_callr,		jit_code_calli,

#define jit_prepare()		_jit_prepare(_jit)
    jit_code_prepare,
#define jit_pushargr(u)		_jit_pushargr(_jit,u)
#define jit_pushargi(u)		_jit_pushargi(_jit,u)
    jit_code_pushargr,		jit_code_pushargi,
#define jit_finishr(u)		_jit_finishr(_jit,u)
#define jit_finishi(u)		_jit_finishi(_jit,u)
    jit_code_finishr,		jit_code_finishi,
#define jit_ret()		_jit_ret(_jit)
    jit_code_ret,
#define jit_retr(u)		_jit_retr(_jit,u)
#define jit_reti(u)		_jit_reti(_jit,u)
    jit_code_retr,		jit_code_reti,
#define jit_retval_c(u)		_jit_retval_c(_jit,u)
#define jit_retval_uc(u)	_jit_retval_uc(_jit,u)
    jit_code_retval_c,		jit_code_retval_uc,
#define jit_retval_s(u)		_jit_retval_s(_jit,u)
#define jit_retval_us(u)	_jit_retval_us(_jit,u)
    jit_code_retval_s,		jit_code_retval_us,
#define jit_retval_i(u)		_jit_retval_i(_jit,u)
#if __WORDSIZE == 32
#  define jit_retval(u)		jit_retval_i(u)
#else
#  define jit_retval(u)		jit_retval_l(u)
#  define jit_retval_ui(u)	_jit_retval_ui(_jit,u)
#  define jit_retval_l(u)	_jit_retval_l(_jit,u)
#endif
    jit_code_retval_i,		jit_code_retval_ui,
    jit_code_retval_l,

#define jit_epilog()		_jit_epilog(_jit)
    jit_code_epilog,

#define jit_arg_f()		_jit_arg_f(_jit)
    jit_code_arg_f,
#define jit_getarg_f(u,v)	_jit_getarg_f(_jit,u,v)
    jit_code_getarg_f,
#define jit_putargr_f(u,v)	_jit_putargr_f(_jit,u,v)
#define jit_putargi_f(u,v)	_jit_putargi_f(_jit,u,v)
    jit_code_putargr_f,		jit_code_putargi_f,

#define jit_addr_f(u,v,w)	jit_new_node_www(jit_code_addr_f,u,v,w)
#define jit_addi_f(u,v,w)	jit_new_node_wwf(jit_code_addi_f,u,v,w)
    jit_code_addr_f,		jit_code_addi_f,
#define jit_subr_f(u,v,w)	jit_new_node_www(jit_code_subr_f,u,v,w)
#define jit_subi_f(u,v,w)	jit_new_node_wwf(jit_code_subi_f,u,v,w)
    jit_code_subr_f,		jit_code_subi_f,
#define jit_rsbr_f(u,v,w)	jit_subr_f(u,w,v)
#define jit_rsbi_f(u,v,w)	jit_new_node_wwf(jit_code_rsbi_f,u,v,w)
    jit_code_rsbi_f,
#define jit_mulr_f(u,v,w)	jit_new_node_www(jit_code_mulr_f,u,v,w)
#define jit_muli_f(u,v,w)	jit_new_node_wwf(jit_code_muli_f,u,v,w)
    jit_code_mulr_f,		jit_code_muli_f,
#define jit_divr_f(u,v,w)	jit_new_node_www(jit_code_divr_f,u,v,w)
#define jit_divi_f(u,v,w)	jit_new_node_wwf(jit_code_divi_f,u,v,w)
    jit_code_divr_f,		jit_code_divi_f,
#define jit_negr_f(u,v)		jit_new_node_ww(jit_code_negr_f,u,v)
#define jit_absr_f(u,v)		jit_new_node_ww(jit_code_absr_f,u,v)
#define jit_sqrtr_f(u,v)	jit_new_node_ww(jit_code_sqrtr_f,u,v)
    jit_code_negr_f,		jit_code_absr_f,	jit_code_sqrtr_f,

#define jit_ltr_f(u,v,w)	jit_new_node_www(jit_code_ltr_f,u,v,w)
#define jit_lti_f(u,v,w)	jit_new_node_wwf(jit_code_lti_f,u,v,w)
    jit_code_ltr_f,		jit_code_lti_f,
#define jit_ler_f(u,v,w)	jit_new_node_www(jit_code_ler_f,u,v,w)
#define jit_lei_f(u,v,w)	jit_new_node_wwf(jit_code_lei_f,u,v,w)
    jit_code_ler_f,		jit_code_lei_f,
#define jit_eqr_f(u,v,w)	jit_new_node_www(jit_code_eqr_f,u,v,w)
#define jit_eqi_f(u,v,w)	jit_new_node_wwf(jit_code_eqi_f,u,v,w)
    jit_code_eqr_f,		jit_code_eqi_f,
#define jit_ger_f(u,v,w)	jit_new_node_www(jit_code_ger_f,u,v,w)
#define jit_gei_f(u,v,w)	jit_new_node_wwf(jit_code_gei_f,u,v,w)
    jit_code_ger_f,		jit_code_gei_f,
#define jit_gtr_f(u,v,w)	jit_new_node_www(jit_code_gtr_f,u,v,w)
#define jit_gti_f(u,v,w)	jit_new_node_wwf(jit_code_gti_f,u,v,w)
    jit_code_gtr_f,		jit_code_gti_f,
#define jit_ner_f(u,v,w)	jit_new_node_www(jit_code_ner_f,u,v,w)
#define jit_nei_f(u,v,w)	jit_new_node_wwf(jit_code_nei_f,u,v,w)
    jit_code_ner_f,		jit_code_nei_f,
#define jit_unltr_f(u,v,w)	jit_new_node_www(jit_code_unltr_f,u,v,w)
#define jit_unlti_f(u,v,w)	jit_new_node_wwf(jit_code_unlti_f,u,v,w)
    jit_code_unltr_f,		jit_code_unlti_f,
#define jit_unler_f(u,v,w)	jit_new_node_www(jit_code_unler_f,u,v,w)
#define jit_unlei_f(u,v,w)	jit_new_node_wwf(jit_code_unlei_f,u,v,w)
    jit_code_unler_f,		jit_code_unlei_f,
#define jit_uneqr_f(u,v,w)	jit_new_node_www(jit_code_uneqr_f,u,v,w)
#define jit_uneqi_f(u,v,w)	jit_new_node_wwf(jit_code_uneqi_f,u,v,w)
    jit_code_uneqr_f,		jit_code_uneqi_f,
#define jit_unger_f(u,v,w)	jit_new_node_www(jit_code_unger_f,u,v,w)
#define jit_ungei_f(u,v,w)	jit_new_node_wwf(jit_code_ungei_f,u,v,w)
    jit_code_unger_f,		jit_code_ungei_f,
#define jit_ungtr_f(u,v,w)	jit_new_node_www(jit_code_ungtr_f,u,v,w)
#define jit_ungti_f(u,v,w)	jit_new_node_wwf(jit_code_ungti_f,u,v,w)
    jit_code_ungtr_f,		jit_code_ungti_f,
#define jit_ltgtr_f(u,v,w)	jit_new_node_www(jit_code_ltgtr_f,u,v,w)
#define jit_ltgti_f(u,v,w)	jit_new_node_wwf(jit_code_ltgti_f,u,v,w)
    jit_code_ltgtr_f,		jit_code_ltgti_f,
#define jit_ordr_f(u,v,w)	jit_new_node_www(jit_code_ordr_f,u,v,w)
#define jit_ordi_f(u,v,w)	jit_new_node_wwf(jit_code_ordi_f,u,v,w)
    jit_code_ordr_f,		jit_code_ordi_f,
#define jit_unordr_f(u,v,w)	jit_new_node_www(jit_code_unordr_f,u,v,w)
#define jit_unordi_f(u,v,w)	jit_new_node_wwf(jit_code_unordi_f,u,v,w)
    jit_code_unordr_f,		jit_code_unordi_f,

#define jit_truncr_f_i(u,v)	jit_new_node_ww(jit_code_truncr_f_i,u,v)
    jit_code_truncr_f_i,
#if __WORDSIZE == 32
#  define jit_truncr_f(u,v)	jit_truncr_f_i(u,v)
#else
#  define jit_truncr_f(u,v)	jit_truncr_f_l(u,v)
#  define jit_truncr_f_l(u,v)	jit_new_node_ww(jit_code_truncr_f_l,u,v)
#endif
    jit_code_truncr_f_l,
#define jit_extr_f(u,v)		jit_new_node_ww(jit_code_extr_f,u,v)
#define jit_extr_d_f(u,v)	jit_new_node_ww(jit_code_extr_d_f,u,v)
    jit_code_extr_f,		jit_code_extr_d_f,
#define jit_movr_f(u,v)		jit_new_node_ww(jit_code_movr_f,u,v)
#define jit_movi_f(u,v)		jit_new_node_wf(jit_code_movi_f,u,v)
    jit_code_movr_f,		jit_code_movi_f,

#define jit_ldr_f(u,v)		jit_new_node_ww(jit_code_ldr_f,u,v)
#define jit_ldi_f(u,v)		jit_new_node_wp(jit_code_ldi_f,u,v)
    jit_code_ldr_f,		jit_code_ldi_f,
#define jit_ldxr_f(u,v,w)	jit_new_node_www(jit_code_ldxr_f,u,v,w)
#define jit_ldxi_f(u,v,w)	jit_new_node_www(jit_code_ldxi_f,u,v,w)
    jit_code_ldxr_f,		jit_code_ldxi_f,
#define jit_str_f(u,v)		jit_new_node_ww(jit_code_str_f,u,v)
#define jit_sti_f(u,v)		jit_new_node_pw(jit_code_sti_f,u,v)
    jit_code_str_f,		jit_code_sti_f,
#define jit_stxr_f(u,v,w)	jit_new_node_www(jit_code_stxr_f,u,v,w)
#define jit_stxi_f(u,v,w)	jit_new_node_www(jit_code_stxi_f,u,v,w)
    jit_code_stxr_f,		jit_code_stxi_f,

#define jit_bltr_f(v,w)		jit_new_node_pww(jit_code_bltr_f,NULL,v,w)
#define jit_blti_f(v,w)		jit_new_node_pwf(jit_code_blti_f,NULL,v,w)
    jit_code_bltr_f,		jit_code_blti_f,
#define jit_bler_f(v,w)		jit_new_node_pww(jit_code_bler_f,NULL,v,w)
#define jit_blei_f(v,w)		jit_new_node_pwf(jit_code_blei_f,NULL,v,w)
    jit_code_bler_f,		jit_code_blei_f,
#define jit_beqr_f(v,w)		jit_new_node_pww(jit_code_beqr_f,NULL,v,w)
#define jit_beqi_f(v,w)		jit_new_node_pwf(jit_code_beqi_f,NULL,v,w)
    jit_code_beqr_f,		jit_code_beqi_f,
#define jit_bger_f(v,w)		jit_new_node_pww(jit_code_bger_f,NULL,v,w)
#define jit_bgei_f(v,w)		jit_new_node_pwf(jit_code_bgei_f,NULL,v,w)
    jit_code_bger_f,		jit_code_bgei_f,
#define jit_bgtr_f(v,w)		jit_new_node_pww(jit_code_bgtr_f,NULL,v,w)
#define jit_bgti_f(v,w)		jit_new_node_pwf(jit_code_bgti_f,NULL,v,w)
    jit_code_bgtr_f,		jit_code_bgti_f,
#define jit_bner_f(v,w)		jit_new_node_pww(jit_code_bner_f,NULL,v,w)
#define jit_bnei_f(v,w)		jit_new_node_pwf(jit_code_bnei_f,NULL,v,w)
    jit_code_bner_f,		jit_code_bnei_f,
#define jit_bunltr_f(v,w)	jit_new_node_pww(jit_code_bunltr_f,NULL,v,w)
#define jit_bunlti_f(v,w)	jit_new_node_pwf(jit_code_bunlti_f,NULL,v,w)
    jit_code_bunltr_f,		jit_code_bunlti_f,
#define jit_bunler_f(v,w)	jit_new_node_pww(jit_code_bunler_f,NULL,v,w)
#define jit_bunlei_f(v,w)	jit_new_node_pwf(jit_code_bunlei_f,NULL,v,w)
    jit_code_bunler_f,		jit_code_bunlei_f,
#define jit_buneqr_f(v,w)	jit_new_node_pww(jit_code_buneqr_f,NULL,v,w)
#define jit_buneqi_f(v,w)	jit_new_node_pwf(jit_code_buneqi_f,NULL,v,w)
    jit_code_buneqr_f,		jit_code_buneqi_f,
#define jit_bunger_f(v,w)	jit_new_node_pww(jit_code_bunger_f,NULL,v,w)
#define jit_bungei_f(v,w)	jit_new_node_pwf(jit_code_bungei_f,NULL,v,w)
    jit_code_bunger_f,		jit_code_bungei_f,
#define jit_bungtr_f(v,w)	jit_new_node_pww(jit_code_bungtr_f,NULL,v,w)
#define jit_bungti_f(v,w)	jit_new_node_pwf(jit_code_bungti_f,NULL,v,w)
    jit_code_bungtr_f,		jit_code_bungti_f,
#define jit_bltgtr_f(v,w)	jit_new_node_pww(jit_code_bltgtr_f,NULL,v,w)
#define jit_bltgti_f(v,w)	jit_new_node_pwf(jit_code_bltgti_f,NULL,v,w)
    jit_code_bltgtr_f,		jit_code_bltgti_f,
#define jit_bordr_f(v,w)	jit_new_node_pww(jit_code_bordr_f,NULL,v,w)
#define jit_bordi_f(v,w)	jit_new_node_pwf(jit_code_bordi_f,NULL,v,w)
    jit_code_bordr_f,		jit_code_bordi_f,
#define jit_bunordr_f(v,w)	jit_new_node_pww(jit_code_bunordr_f,NULL,v,w)
#define jit_bunordi_f(v,w)	jit_new_node_pwf(jit_code_bunordi_f,NULL,v,w)
    jit_code_bunordr_f,		jit_code_bunordi_f,

#define jit_pushargr_f(u)	_jit_pushargr_f(_jit,u)
#define jit_pushargi_f(u)	_jit_pushargi_f(_jit,u)
    jit_code_pushargr_f,	jit_code_pushargi_f,
#define jit_retr_f(u)		_jit_retr_f(_jit,u)
#define jit_reti_f(u)		_jit_reti_f(_jit,u)
    jit_code_retr_f,		jit_code_reti_f,
#define jit_retval_f(u)		_jit_retval_f(_jit,u)
    jit_code_retval_f,

#define jit_arg_d()		_jit_arg_d(_jit)
    jit_code_arg_d,
#define jit_getarg_d(u,v)	_jit_getarg_d(_jit,u,v)
    jit_code_getarg_d,
#define jit_putargr_d(u,v)	_jit_putargr_d(_jit,u,v)
#define jit_putargi_d(u,v)	_jit_putargi_d(_jit,u,v)
    jit_code_putargr_d,		jit_code_putargi_d,

#define jit_addr_d(u,v,w)	jit_new_node_www(jit_code_addr_d,u,v,w)
#define jit_addi_d(u,v,w)	jit_new_node_wwd(jit_code_addi_d,u,v,w)
    jit_code_addr_d,		jit_code_addi_d,
#define jit_subr_d(u,v,w)	jit_new_node_www(jit_code_subr_d,u,v,w)
#define jit_subi_d(u,v,w)	jit_new_node_wwd(jit_code_subi_d,u,v,w)
    jit_code_subr_d,		jit_code_subi_d,
#define jit_rsbr_d(u,v,w)	jit_subr_d(u,w,v)
#define jit_rsbi_d(u,v,w)	jit_new_node_wwd(jit_code_rsbi_d,u,v,w)
    jit_code_rsbi_d,
#define jit_mulr_d(u,v,w)	jit_new_node_www(jit_code_mulr_d,u,v,w)
#define jit_muli_d(u,v,w)	jit_new_node_wwd(jit_code_muli_d,u,v,w)
    jit_code_mulr_d,		jit_code_muli_d,
#define jit_divr_d(u,v,w)	jit_new_node_www(jit_code_divr_d,u,v,w)
#define jit_divi_d(u,v,w)	jit_new_node_wwd(jit_code_divi_d,u,v,w)
    jit_code_divr_d,		jit_code_divi_d,

#define jit_negr_d(u,v)		jit_new_node_ww(jit_code_negr_d,u,v)
#define jit_absr_d(u,v)		jit_new_node_ww(jit_code_absr_d,u,v)
#define jit_sqrtr_d(u,v)	jit_new_node_ww(jit_code_sqrtr_d,u,v)
    jit_code_negr_d,		jit_code_absr_d,	jit_code_sqrtr_d,

#define jit_ltr_d(u,v,w)	jit_new_node_www(jit_code_ltr_d,u,v,w)
#define jit_lti_d(u,v,w)	jit_new_node_wwd(jit_code_lti_d,u,v,w)
    jit_code_ltr_d,		jit_code_lti_d,
#define jit_ler_d(u,v,w)	jit_new_node_www(jit_code_ler_d,u,v,w)
#define jit_lei_d(u,v,w)	jit_new_node_wwd(jit_code_lei_d,u,v,w)
    jit_code_ler_d,		jit_code_lei_d,
#define jit_eqr_d(u,v,w)	jit_new_node_www(jit_code_eqr_d,u,v,w)
#define jit_eqi_d(u,v,w)	jit_new_node_wwd(jit_code_eqi_d,u,v,w)
    jit_code_eqr_d,		jit_code_eqi_d,
#define jit_ger_d(u,v,w)	jit_new_node_www(jit_code_ger_d,u,v,w)
#define jit_gei_d(u,v,w)	jit_new_node_wwd(jit_code_gei_d,u,v,w)
    jit_code_ger_d,		jit_code_gei_d,
#define jit_gtr_d(u,v,w)	jit_new_node_www(jit_code_gtr_d,u,v,w)
#define jit_gti_d(u,v,w)	jit_new_node_wwd(jit_code_gti_d,u,v,w)
    jit_code_gtr_d,		jit_code_gti_d,
#define jit_ner_d(u,v,w)	jit_new_node_www(jit_code_ner_d,u,v,w)
#define jit_nei_d(u,v,w)	jit_new_node_wwd(jit_code_nei_d,u,v,w)
    jit_code_ner_d,		jit_code_nei_d,
#define jit_unltr_d(u,v,w)	jit_new_node_www(jit_code_unltr_d,u,v,w)
#define jit_unlti_d(u,v,w)	jit_new_node_wwd(jit_code_unlti_d,u,v,w)
    jit_code_unltr_d,		jit_code_unlti_d,
#define jit_unler_d(u,v,w)	jit_new_node_www(jit_code_unler_d,u,v,w)
#define jit_unlei_d(u,v,w)	jit_new_node_wwd(jit_code_unlei_d,u,v,w)
    jit_code_unler_d,		jit_code_unlei_d,
#define jit_uneqr_d(u,v,w)	jit_new_node_www(jit_code_uneqr_d,u,v,w)
#define jit_uneqi_d(u,v,w)	jit_new_node_wwd(jit_code_uneqi_d,u,v,w)
    jit_code_uneqr_d,		jit_code_uneqi_d,
#define jit_unger_d(u,v,w)	jit_new_node_www(jit_code_unger_d,u,v,w)
#define jit_ungei_d(u,v,w)	jit_new_node_wwd(jit_code_ungei_d,u,v,w)
    jit_code_unger_d,		jit_code_ungei_d,
#define jit_ungtr_d(u,v,w)	jit_new_node_www(jit_code_ungtr_d,u,v,w)
#define jit_ungti_d(u,v,w)	jit_new_node_wwd(jit_code_ungti_d,u,v,w)
    jit_code_ungtr_d,		jit_code_ungti_d,
#define jit_ltgtr_d(u,v,w)	jit_new_node_www(jit_code_ltgtr_d,u,v,w)
#define jit_ltgti_d(u,v,w)	jit_new_node_wwd(jit_code_ltgti_d,u,v,w)
    jit_code_ltgtr_d,		jit_code_ltgti_d,
#define jit_ordr_d(u,v,w)	jit_new_node_www(jit_code_ordr_d,u,v,w)
#define jit_ordi_d(u,v,w)	jit_new_node_wwd(jit_code_ordi_d,u,v,w)
    jit_code_ordr_d,		jit_code_ordi_d,
#define jit_unordr_d(u,v,w)	jit_new_node_www(jit_code_unordr_d,u,v,w)
#define jit_unordi_d(u,v,w)	jit_new_node_wwd(jit_code_unordi_d,u,v,w)
    jit_code_unordr_d,		jit_code_unordi_d,

#define jit_truncr_d_i(u,v)	jit_new_node_ww(jit_code_truncr_d_i,u,v)
    jit_code_truncr_d_i,
#if __WORDSIZE == 32
#  define jit_truncr_d(u,v)	jit_truncr_d_i(u,v)
#else
#  define jit_truncr_d(u,v)	jit_truncr_d_l(u,v)
#  define jit_truncr_d_l(u,v)	jit_new_node_ww(jit_code_truncr_d_l,u,v)
#endif
    jit_code_truncr_d_l,
#define jit_extr_d(u,v)		jit_new_node_ww(jit_code_extr_d,u,v)
#define jit_extr_f_d(u,v)	jit_new_node_ww(jit_code_extr_f_d,u,v)
    jit_code_extr_d,		jit_code_extr_f_d,
#define jit_movr_d(u,v)		jit_new_node_ww(jit_code_movr_d,u,v)
#define jit_movi_d(u,v)		jit_new_node_wd(jit_code_movi_d,u,v)
    jit_code_movr_d,		jit_code_movi_d,

#define jit_ldr_d(u,v)		jit_new_node_ww(jit_code_ldr_d,u,v)
#define jit_ldi_d(u,v)		jit_new_node_wp(jit_code_ldi_d,u,v)
    jit_code_ldr_d,		jit_code_ldi_d,
#define jit_ldxr_d(u,v,w)	jit_new_node_www(jit_code_ldxr_d,u,v,w)
#define jit_ldxi_d(u,v,w)	jit_new_node_www(jit_code_ldxi_d,u,v,w)
    jit_code_ldxr_d,		jit_code_ldxi_d,
#define jit_str_d(u,v)		jit_new_node_ww(jit_code_str_d,u,v)
#define jit_sti_d(u,v)		jit_new_node_pw(jit_code_sti_d,u,v)
    jit_code_str_d,		jit_code_sti_d,
#define jit_stxr_d(u,v,w)	jit_new_node_www(jit_code_stxr_d,u,v,w)
#define jit_stxi_d(u,v,w)	jit_new_node_www(jit_code_stxi_d,u,v,w)
    jit_code_stxr_d,		jit_code_stxi_d,

#define jit_bltr_d(v,w)		jit_new_node_pww(jit_code_bltr_d,NULL,v,w)
#define jit_blti_d(v,w)		jit_new_node_pwd(jit_code_blti_d,NULL,v,w)
    jit_code_bltr_d,		jit_code_blti_d,
#define jit_bler_d(v,w)		jit_new_node_pww(jit_code_bler_d,NULL,v,w)
#define jit_blei_d(v,w)		jit_new_node_pwd(jit_code_blei_d,NULL,v,w)
    jit_code_bler_d,		jit_code_blei_d,
#define jit_beqr_d(v,w)		jit_new_node_pww(jit_code_beqr_d,NULL,v,w)
#define jit_beqi_d(v,w)		jit_new_node_pwd(jit_code_beqi_d,NULL,v,w)
    jit_code_beqr_d,		jit_code_beqi_d,
#define jit_bger_d(v,w)		jit_new_node_pww(jit_code_bger_d,NULL,v,w)
#define jit_bgei_d(v,w)		jit_new_node_pwd(jit_code_bgei_d,NULL,v,w)
    jit_code_bger_d,		jit_code_bgei_d,
#define jit_bgtr_d(v,w)		jit_new_node_pww(jit_code_bgtr_d,NULL,v,w)
#define jit_bgti_d(v,w)		jit_new_node_pwd(jit_code_bgti_d,NULL,v,w)
    jit_code_bgtr_d,		jit_code_bgti_d,
#define jit_bner_d(v,w)		jit_new_node_pww(jit_code_bner_d,NULL,v,w)
#define jit_bnei_d(v,w)		jit_new_node_pwd(jit_code_bnei_d,NULL,v,w)
    jit_code_bner_d,		jit_code_bnei_d,
#define jit_bunltr_d(v,w)	jit_new_node_pww(jit_code_bunltr_d,NULL,v,w)
#define jit_bunlti_d(v,w)	jit_new_node_pwd(jit_code_bunlti_d,NULL,v,w)
    jit_code_bunltr_d,		jit_code_bunlti_d,
#define jit_bunler_d(v,w)	jit_new_node_pww(jit_code_bunler_d,NULL,v,w)
#define jit_bunlei_d(v,w)	jit_new_node_pwd(jit_code_bunlei_d,NULL,v,w)
    jit_code_bunler_d,		jit_code_bunlei_d,
#define jit_buneqr_d(v,w)	jit_new_node_pww(jit_code_buneqr_d,NULL,v,w)
#define jit_buneqi_d(v,w)	jit_new_node_pwd(jit_code_buneqi_d,NULL,v,w)
    jit_code_buneqr_d,		jit_code_buneqi_d,
#define jit_bunger_d(v,w)	jit_new_node_pww(jit_code_bunger_d,NULL,v,w)
#define jit_bungei_d(v,w)	jit_new_node_pwd(jit_code_bungei_d,NULL,v,w)
    jit_code_bunger_d,		jit_code_bungei_d,
#define jit_bungtr_d(v,w)	jit_new_node_pww(jit_code_bungtr_d,NULL,v,w)
#define jit_bungti_d(v,w)	jit_new_node_pwd(jit_code_bungti_d,NULL,v,w)
    jit_code_bungtr_d,		jit_code_bungti_d,
#define jit_bltgtr_d(v,w)	jit_new_node_pww(jit_code_bltgtr_d,NULL,v,w)
#define jit_bltgti_d(v,w)	jit_new_node_pwd(jit_code_bltgti_d,NULL,v,w)
    jit_code_bltgtr_d,		jit_code_bltgti_d,
#define jit_bordr_d(v,w)	jit_new_node_pww(jit_code_bordr_d,NULL,v,w)
#define jit_bordi_d(v,w)	jit_new_node_pwd(jit_code_bordi_d,NULL,v,w)
    jit_code_bordr_d,		jit_code_bordi_d,
#define jit_bunordr_d(v,w)	jit_new_node_pww(jit_code_bunordr_d,NULL,v,w)
#define jit_bunordi_d(v,w)	jit_new_node_pwd(jit_code_bunordi_d,NULL,v,w)
    jit_code_bunordr_d,		jit_code_bunordi_d,

#define jit_pushargr_d(u)	_jit_pushargr_d(_jit,u)
#define jit_pushargi_d(u)	_jit_pushargi_d(_jit,u)
    jit_code_pushargr_d,	jit_code_pushargi_d,
#define jit_retr_d(u)		_jit_retr_d(_jit,u)
#define jit_reti_d(u)		_jit_reti_d(_jit,u)
    jit_code_retr_d,		jit_code_reti_d,
#define jit_retval_d(u)		_jit_retval_d(_jit,u)
    jit_code_retval_d,

    /* Special internal backend specific codes */
    jit_code_movr_w_f,		jit_code_movr_ww_d,	/* w* -> f|d */
#define jit_movr_w_f(u, v)	jit_new_node_ww(jit_code_movr_w_f, u, v)
#define jit_movr_ww_d(u, v, w)	jit_new_node_www(jit_code_movr_ww_d, u, v, w)
    jit_code_movr_w_d,					/* w -> d */
#define jit_movr_w_d(u, v)	jit_new_node_ww(jit_code_movr_w_d, u, v)

    jit_code_movr_f_w,		jit_code_movi_f_w,	/* f|d -> w* */
#define jit_movr_f_w(u, v)	jit_new_node_ww(jit_code_movr_f_w, u, v)
#define jit_movi_f_w(u, v)	jit_new_node_wf(jit_code_movi_f_w, u, v)
    jit_code_movr_d_ww,		jit_code_movi_d_ww,
#define jit_movr_d_ww(u, v, w)	jit_new_node_www(jit_code_movr_d_ww, u, v, w)
#define jit_movi_d_ww(u, v, w)	jit_new_node_wwd(jit_code_movi_d_ww, u, v, w)

    jit_code_movr_d_w,		jit_code_movi_d_w,	/* d -> w */
#define jit_movr_d_w(u, v)	jit_new_node_ww(jit_code_movr_d_w, u, v)
#define jit_movi_d_w(u, v)	jit_new_node_wd(jit_code_movi_d_w, u, v)

    jit_code_last_code
} jit_code_t;

typedef void* (*jit_alloc_func_ptr)	(size_t);
typedef void* (*jit_realloc_func_ptr)	(void*, size_t);
typedef void  (*jit_free_func_ptr)	(void*);

/*
 * Prototypes
 */
extern void init_jit(const char*);
extern void finish_jit(void);

extern jit_state_t *jit_new_state(void);
#define jit_clear_state()	_jit_clear_state(_jit)
extern void _jit_clear_state(jit_state_t*);
#define jit_destroy_state()	_jit_destroy_state(_jit)
extern void _jit_destroy_state(jit_state_t*);

#define jit_address(node)	_jit_address(_jit, node)
extern jit_pointer_t _jit_address(jit_state_t*, jit_node_t*);
extern jit_node_t *_jit_name(jit_state_t*, const char*);
extern jit_node_t *_jit_note(jit_state_t*, const char*, int);
extern jit_node_t *_jit_label(jit_state_t*);
extern jit_node_t *_jit_forward(jit_state_t*);
extern jit_node_t *_jit_indirect(jit_state_t*);
extern void _jit_link(jit_state_t*, jit_node_t*);
#define jit_forward_p(u)	_jit_forward_p(_jit,u)
extern jit_bool_t _jit_forward_p(jit_state_t*,jit_node_t*);
#define jit_indirect_p(u)	_jit_indirect_p(_jit,u)
extern jit_bool_t _jit_indirect_p(jit_state_t*,jit_node_t*);
#define jit_target_p(u)		_jit_target_p(_jit,u)
extern jit_bool_t _jit_target_p(jit_state_t*,jit_node_t*);

extern void _jit_prolog(jit_state_t*);

extern jit_int32_t _jit_allocai(jit_state_t*, jit_int32_t);
extern void _jit_allocar(jit_state_t*, jit_int32_t, jit_int32_t);
extern void _jit_ellipsis(jit_state_t*);

extern jit_node_t *_jit_arg(jit_state_t*);
extern void _jit_getarg_c(jit_state_t*, jit_gpr_t, jit_node_t*);
extern void _jit_getarg_uc(jit_state_t*, jit_gpr_t, jit_node_t*);
extern void _jit_getarg_s(jit_state_t*, jit_gpr_t, jit_node_t*);
extern void _jit_getarg_us(jit_state_t*, jit_gpr_t, jit_node_t*);
extern void _jit_getarg_i(jit_state_t*, jit_gpr_t, jit_node_t*);
#if __WORDSIZE == 64
extern void _jit_getarg_ui(jit_state_t*, jit_gpr_t, jit_node_t*);
extern void _jit_getarg_l(jit_state_t*, jit_gpr_t, jit_node_t*);
#endif
extern void _jit_putargr(jit_state_t*, jit_gpr_t, jit_node_t*);
extern void _jit_putargi(jit_state_t*, jit_word_t, jit_node_t*);

extern void _jit_prepare(jit_state_t*);
extern void _jit_ellipsis(jit_state_t*);
extern void _jit_va_push(jit_state_t*, jit_gpr_t);
extern void _jit_pushargr(jit_state_t*, jit_gpr_t);
extern void _jit_pushargi(jit_state_t*, jit_word_t);
extern void _jit_finishr(jit_state_t*, jit_gpr_t);
extern jit_node_t *_jit_finishi(jit_state_t*, jit_pointer_t);
extern void _jit_ret(jit_state_t*);
extern void _jit_retr(jit_state_t*, jit_gpr_t);
extern void _jit_reti(jit_state_t*, jit_word_t);
extern void _jit_retval_c(jit_state_t*, jit_gpr_t);
extern void _jit_retval_uc(jit_state_t*, jit_gpr_t);
extern void _jit_retval_s(jit_state_t*, jit_gpr_t);
extern void _jit_retval_us(jit_state_t*, jit_gpr_t);
extern void _jit_retval_i(jit_state_t*, jit_gpr_t);
#if __WORDSIZE == 64
extern void _jit_retval_ui(jit_state_t*, jit_gpr_t);
extern void _jit_retval_l(jit_state_t*, jit_gpr_t);
#endif
extern void _jit_epilog(jit_state_t*);

#define jit_patch(u)		_jit_patch(_jit,u)
extern void _jit_patch(jit_state_t*, jit_node_t*);
#define jit_patch_at(u,v)	_jit_patch_at(_jit,u,v)
extern void _jit_patch_at(jit_state_t*, jit_node_t*, jit_node_t*);
#define jit_patch_abs(u,v)	_jit_patch_abs(_jit,u,v)
extern void _jit_patch_abs(jit_state_t*, jit_node_t*, jit_pointer_t);
#define jit_realize()		_jit_realize(_jit)
extern void _jit_realize(jit_state_t*);
#define jit_get_code(u)		_jit_get_code(_jit,u)
extern jit_pointer_t _jit_get_code(jit_state_t*, jit_word_t*);
#define jit_set_code(u,v)	_jit_set_code(_jit,u,v)
extern void _jit_set_code(jit_state_t*, jit_pointer_t, jit_word_t);
#define jit_get_data(u,v)	_jit_get_data(_jit,u,v)
extern jit_pointer_t _jit_get_data(jit_state_t*, jit_word_t*, jit_word_t*);
#define jit_set_data(u,v,w)	_jit_set_data(_jit,u,v,w)
extern void _jit_set_data(jit_state_t*, jit_pointer_t, jit_word_t, jit_word_t);
#define jit_frame(u)		_jit_frame(_jit,u)
extern void _jit_frame(jit_state_t*, jit_int32_t);
#define jit_tramp(u)		_jit_tramp(_jit,u)
extern void _jit_tramp(jit_state_t*, jit_int32_t);
#define jit_emit()		_jit_emit(_jit)
extern jit_pointer_t _jit_emit(jit_state_t*);

#define jit_print()		_jit_print(_jit)
extern void _jit_print(jit_state_t*);

extern jit_node_t *_jit_arg_f(jit_state_t*);
extern void _jit_getarg_f(jit_state_t*, jit_fpr_t, jit_node_t*);
extern void _jit_putargr_f(jit_state_t*, jit_fpr_t, jit_node_t*);
extern void _jit_putargi_f(jit_state_t*, jit_float32_t, jit_node_t*);
extern void _jit_pushargr_f(jit_state_t*, jit_fpr_t);
extern void _jit_pushargi_f(jit_state_t*, jit_float32_t);
extern void _jit_retr_f(jit_state_t*, jit_fpr_t);
extern void _jit_reti_f(jit_state_t*, jit_float32_t);
extern void _jit_retval_f(jit_state_t*, jit_fpr_t);

extern jit_node_t *_jit_arg_d(jit_state_t*);
extern void _jit_getarg_d(jit_state_t*, jit_fpr_t, jit_node_t*);
extern void _jit_putargr_d(jit_state_t*, jit_fpr_t, jit_node_t*);
extern void _jit_putargi_d(jit_state_t*, jit_float64_t, jit_node_t*);
extern void _jit_pushargr_d(jit_state_t*, jit_fpr_t);
extern void _jit_pushargi_d(jit_state_t*, jit_float64_t);
extern void _jit_retr_d(jit_state_t*, jit_fpr_t);
extern void _jit_reti_d(jit_state_t*, jit_float64_t);
extern void _jit_retval_d(jit_state_t*, jit_fpr_t);

#define jit_new_node(c)		_jit_new_node(_jit,c)
extern jit_node_t *_jit_new_node(jit_state_t*, jit_code_t);
#define jit_new_node_w(c,u)	_jit_new_node_w(_jit,c,u)
extern jit_node_t *_jit_new_node_w(jit_state_t*, jit_code_t,
				   jit_word_t);
#define jit_new_node_f(c,u)	_jit_new_node_f(_jit,c,u)
extern jit_node_t *_jit_new_node_f(jit_state_t*, jit_code_t,
				   jit_float32_t);
#define jit_new_node_d(c,u)	_jit_new_node_d(_jit,c,u)
extern jit_node_t *_jit_new_node_d(jit_state_t*, jit_code_t,
				   jit_float64_t);
#define jit_new_node_p(c,u)	_jit_new_node_p(_jit,c,u)
extern jit_node_t *_jit_new_node_p(jit_state_t*, jit_code_t,
				   jit_pointer_t);
#define jit_new_node_ww(c,u,v)	_jit_new_node_ww(_jit,c,u,v)
extern jit_node_t *_jit_new_node_ww(jit_state_t*,jit_code_t,
				    jit_word_t, jit_word_t);
#define jit_new_node_wp(c,u,v)	_jit_new_node_wp(_jit,c,u,v)
extern jit_node_t *_jit_new_node_wp(jit_state_t*,jit_code_t,
				    jit_word_t, jit_pointer_t);
#define jit_new_node_fp(c,u,v)	_jit_new_node_fp(_jit,c,u,v)
extern jit_node_t *_jit_new_node_fp(jit_state_t*,jit_code_t,
				    jit_float32_t, jit_pointer_t);
#define jit_new_node_dp(c,u,v)	_jit_new_node_dp(_jit,c,u,v)
extern jit_node_t *_jit_new_node_dp(jit_state_t*,jit_code_t,
				    jit_float64_t, jit_pointer_t);
#define jit_new_node_pw(c,u,v)	_jit_new_node_pw(_jit,c,u,v)
extern jit_node_t *_jit_new_node_pw(jit_state_t*,jit_code_t,
				    jit_pointer_t, jit_word_t);
#define jit_new_node_wf(c,u,v)	_jit_new_node_wf(_jit,c,u,v)
extern jit_node_t *_jit_new_node_wf(jit_state_t*, jit_code_t,
				    jit_word_t, jit_float32_t);
#define jit_new_node_wd(c,u,v)	_jit_new_node_wd(_jit,c,u,v)
extern jit_node_t *_jit_new_node_wd(jit_state_t*, jit_code_t,
				    jit_word_t, jit_float64_t);
#define jit_new_node_www(c,u,v,w) _jit_new_node_www(_jit,c,u,v,w)
extern jit_node_t *_jit_new_node_www(jit_state_t*, jit_code_t,
				     jit_word_t, jit_word_t, jit_word_t);
#define jit_new_node_qww(c,l,h,v,w) _jit_new_node_qww(_jit,c,l,h,v,w)
extern jit_node_t *_jit_new_node_qww(jit_state_t*, jit_code_t,
				     jit_int32_t, jit_int32_t,
				     jit_word_t, jit_word_t);
#define jit_new_node_wwf(c,u,v,w) _jit_new_node_wwf(_jit,c,u,v,w)
extern jit_node_t *_jit_new_node_wwf(jit_state_t*, jit_code_t,
				     jit_word_t, jit_word_t, jit_float32_t);
#define jit_new_node_wwd(c,u,v,w) _jit_new_node_wwd(_jit,c,u,v,w)
extern jit_node_t *_jit_new_node_wwd(jit_state_t*, jit_code_t,
				     jit_word_t, jit_word_t, jit_float64_t);
#define jit_new_node_pww(c,u,v,w) _jit_new_node_pww(_jit,c,u,v,w)
extern jit_node_t *_jit_new_node_pww(jit_state_t*, jit_code_t,
				     jit_pointer_t, jit_word_t, jit_word_t);
#define jit_new_node_pwf(c,u,v,w) _jit_new_node_pwf(_jit,c,u,v,w)
extern jit_node_t *_jit_new_node_pwf(jit_state_t*, jit_code_t,
				     jit_pointer_t, jit_word_t, jit_float32_t);
#define jit_new_node_pwd(c,u,v,w) _jit_new_node_pwd(_jit,c,u,v,w)
extern jit_node_t *_jit_new_node_pwd(jit_state_t*, jit_code_t,
				     jit_pointer_t, jit_word_t, jit_float64_t);

#define jit_arg_register_p(u)		_jit_arg_register_p(_jit,u)
extern jit_bool_t _jit_arg_register_p(jit_state_t*, jit_node_t*);
#define jit_callee_save_p(u)		_jit_callee_save_p(_jit,u)
extern jit_bool_t _jit_callee_save_p(jit_state_t*, jit_int32_t);
#define jit_pointer_p(u)		_jit_pointer_p(_jit,u)
extern jit_bool_t _jit_pointer_p(jit_state_t*,jit_pointer_t);

#define jit_get_note(n,u,v,w)	_jit_get_note(_jit,n,u,v,w)
extern jit_bool_t _jit_get_note(jit_state_t*,jit_pointer_t,char**,char**,int*);

#define jit_disassemble()		_jit_disassemble(_jit)
extern void _jit_disassemble(jit_state_t*);

extern void jit_set_memory_functions(jit_alloc_func_ptr,
				     jit_realloc_func_ptr,
				     jit_free_func_ptr);
extern void jit_get_memory_functions(jit_alloc_func_ptr*,
				     jit_realloc_func_ptr*,
				     jit_free_func_ptr*);

#endif /* _lightning_h */
