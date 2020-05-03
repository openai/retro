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

#include <lightning.h>
#include <lightning/jit_private.h>

#define jit_arg_reg_p(i)		(i >= 0 && i < 4)

#define PROTO				1
#  include "jit_hppa-cpu.c"
#  include "jit_hppa-fpu.c"
#undef PROTO

/*
 * Types
 */
typedef jit_pointer_t	jit_va_list;

/*
 * Prototypes
 */
#define patch(instr, node)		_patch(_jit, instr, node)
static void _patch(jit_state_t*,jit_word_t,jit_node_t*);

/* libgcc */
extern void __clear_cache(void *, void *);

/*
 * Initialization
 */
jit_register_t		_rvs[] = {
    { 0,			"r0" },		/* Zero */
    /* Not register starved, so, avoid allocating r1 and rp
     * due to being implicit target of ADDIL and B,L */
    { 1,			"r1" },		/* Scratch */
    { 2,			"rp" },		/* Return Pointer and scratch */
    { rc(sav) | 3,		"r3" },
    { 19,			"r19" },	/* Linkage Table */
    { rc(gpr) | 20,		"r20" },
    { rc(gpr) | 21,		"r21" },
    { rc(gpr) | 22,		"r22" },
    { rc(gpr) | 29,		"ret1" },
    { rc(gpr) | 28,		"ret0" },
    /* JIT_Rx in callee save registers due to need to call
     * functions to implement some instructions */
    /* JIT_R0- JIT_R2 */
    { rc(gpr) | rc(sav) | 4,	"r4" },
    { rc(gpr) | rc(sav) | 5,	"r5" },
    { rc(gpr) | rc(sav) | 6,	"r6" },
    /* JIT_V0- JIT_V2 */
    { rc(gpr) | rc(sav) | 7,	"r7" },
    { rc(sav) | rc(sav) | 8,	"r8" },
    { rc(gpr) | rc(sav) | 9,	"r9" },
    /* JIT_R3 */
    { rc(gpr) | rc(sav) | 10,	"r10" },
    /* JIT_V3+ */
    { rc(gpr) | rc(sav) | 11,	"r11" },
    { rc(gpr) | rc(sav) | 12,	"r12" },
    { rc(gpr) | rc(sav) | 13,	"r13" },
    { rc(gpr) | rc(sav) | 14,	"r14" },
    { rc(gpr) | rc(sav) | 15,	"r15" },
    { rc(gpr) | rc(sav) | 16,	"r16" },
    { rc(gpr) | rc(sav) | 17,	"r17" },
    { rc(gpr) | rc(sav) | 18,	"r18" },
    /* Arguments */
    { rc(gpr) | rc(arg) | 23,	"r23" },
    { rc(gpr) | rc(arg) | 24,	"r24" },
    { rc(gpr) | rc(arg) | 25,	"r25" },
    { rc(gpr) | rc(arg) | 26,	"r26" },
    { 27,			"dp" },		/* Data Pointer */
    { 30,			"sp" },
    { 31,			"r31" },	/* Link Register */
    { rc(fpr) | 31,		"fr31" },
    { rc(fpr) | 30,		"fr30" },
    { rc(fpr) | 29,		"fr29" },
    { rc(fpr) | 28,		"fr28" },
    { rc(fpr) | 27,		"fr27" },
    { rc(fpr) | 26,		"fr26" },
    { rc(fpr) | 25,		"fr25" },
    { rc(fpr) | 24,		"fr24" },
    { rc(fpr) | 23,		"fr23" },
    { rc(fpr) | 22,		"fr22" },
    { rc(fpr) | 11,		"fr11" },
    { rc(fpr) | 10,		"fr10" },
    { rc(fpr) | 9,		"fr9" },
    { rc(fpr) | 8,		"fr8" },
    /* Arguments */
    { rc(fpr) | rc(arg) | 7,	"fr7" },
    { rc(fpr) | rc(arg) | 6,	"fr6" },
    { rc(fpr) | rc(arg) | 5,	"fr5" },
    { rc(fpr) | rc(arg) | 4,	"fr4" },
    /* Callee Saves */
    { rc(fpr) | rc(sav) | 21,	"fr21" },
    { rc(fpr) | rc(sav) | 20,	"fr20" },
    { rc(fpr) | rc(sav) | 19,	"fr19" },
    { rc(fpr) | rc(sav) | 18,	"fr18" },
    { rc(fpr) | rc(sav) | 17,	"fr17" },
    { rc(fpr) | rc(sav) | 16,	"fr16" },
    { rc(fpr) | rc(sav) | 15,	"fr15" },
    { rc(fpr) | rc(sav) | 14,	"fr14" },
    { rc(fpr) | rc(sav) | 13,	"fr13" },
    { rc(fpr) | rc(sav) | 12,	"fr12" },
    { 0,			"fpsr" },
    { 1,			"fpe2" },
    { 2,			"fpe4" },
    { 3,			"fpe6" },
    { _NOREG,			"<none>" },
};

/*
 * Implementation
 */
void
jit_get_cpu(void)
{
    /* FIXME Expecting PARISC 2.0, for PARISC 1.0 should not use fr16-fr31 */
}

void
_jit_init(jit_state_t *_jit)
{
    _jitc->reglen = jit_size(_rvs) - 1;
}

void
_jit_prolog(jit_state_t *_jit)
{
    jit_int32_t		offset;

    if (_jitc->function)
	jit_epilog();
    assert(jit_regset_cmp_ui(&_jitc->regarg, 0) == 0);
    jit_regset_set_ui(&_jitc->regsav, 0);
    offset = _jitc->functions.offset;
    if (offset >= _jitc->functions.length) {
	jit_realloc((jit_pointer_t *)&_jitc->functions.ptr,
		    _jitc->functions.length * sizeof(jit_function_t),
		    (_jitc->functions.length + 16) * sizeof(jit_function_t));
	_jitc->functions.length += 16;
    }
    _jitc->function = _jitc->functions.ptr + _jitc->functions.offset++;
    _jitc->function->self.size = params_offset;
    _jitc->function->self.argi = _jitc->function->self.alen = 0;
    /* float conversion */
    _jitc->function->self.aoff = alloca_offset;
    _jitc->function->self.call = jit_call_default;
    jit_alloc((jit_pointer_t *)&_jitc->function->regoff,
	      _jitc->reglen * sizeof(jit_int32_t));

    /* _no_link here does not mean the jit_link() call can be removed
     * by rewriting as:
     * _jitc->function->prolog = jit_new_node(jit_code_prolog);
     */
    _jitc->function->prolog = jit_new_node_no_link(jit_code_prolog);
    jit_link(_jitc->function->prolog);
    _jitc->function->prolog->w.w = offset;
    _jitc->function->epilog = jit_new_node_no_link(jit_code_epilog);
    /*	u:	label value
     *	v:	offset in blocks vector
     *	w:	offset in functions vector
     */
    _jitc->function->epilog->w.w = offset;

    jit_regset_new(&_jitc->function->regset);
}

jit_int32_t
_jit_allocai(jit_state_t *_jit, jit_int32_t length)
{
    jit_int32_t		offset;
    assert(_jitc->function);
    switch (length) {
	case 0:	case 1:
	    break;
	case 2:
	    _jitc->function->self.aoff = (_jitc->function->self.aoff + 1) & -2;
	    break;
	case 3:	case 4:
	    _jitc->function->self.aoff = (_jitc->function->self.aoff + 3) & -4;
	    break;
	default:
	    _jitc->function->self.aoff = (_jitc->function->self.aoff + 7) & -8;
	    break;
    }
    if (!_jitc->realize) {
	jit_inc_synth_ww(allocai, _jitc->function->self.aoff, length);
	jit_dec_synth();
    }
    offset = _jitc->function->self.aoff;
    _jitc->function->self.aoff += length;
    return (offset);
}

void
_jit_allocar(jit_state_t *_jit, jit_int32_t u, jit_int32_t v)
{
    jit_int32_t		 reg;
    assert(_jitc->function);
    jit_inc_synth_ww(allocar, u, v);
    if (!_jitc->function->allocar) {
	_jitc->function->aoffoff = jit_allocai(sizeof(jit_int32_t));
	_jitc->function->allocar = 1;
    }
    reg = jit_get_reg(jit_class_gpr);
    jit_addi(reg, v, 63);
    jit_andi(reg, reg, -64);
    jit_ldxi_i(u, JIT_FP, _jitc->function->aoffoff);
    jit_addr(JIT_SP, JIT_SP, reg);
    jit_stxi_i(_jitc->function->aoffoff, JIT_FP, u);
    jit_unget_reg(reg);
    jit_dec_synth();
}

void
_jit_ret(jit_state_t *_jit)
{
    jit_node_t		*instr;
    assert(_jitc->function);
    jit_inc_synth(ret);
    /* jump to epilog */
    instr = jit_jmpi();
    jit_patch_at(instr, _jitc->function->epilog);
    jit_dec_synth();
}

void
_jit_retr(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr, u);
    jit_movr(JIT_RET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti(jit_state_t *_jit, jit_word_t u)
{
    jit_inc_synth_w(reti, u);
    jit_movi(JIT_RET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_retr_f(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr_f, u);
    jit_movr_f(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_inc_synth_f(reti_f, u);
    jit_movi_f(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_retr_d(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr_d, u);
    jit_movr_d(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_inc_synth_d(reti_d, u);
    jit_movi_d(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_epilog(jit_state_t *_jit)
{
    assert(_jitc->function);
    assert(_jitc->function->epilog->next == NULL);
    jit_link(_jitc->function->epilog);
    _jitc->function = NULL;
}

jit_bool_t
_jit_arg_register_p(jit_state_t *_jit, jit_node_t *u)
{
    assert(u->code == jit_code_arg ||
	   u->code == jit_code_arg_f || u->code == jit_code_arg_d);
    return (jit_arg_reg_p(u->u.w));
}

void
_jit_ellipsis(jit_state_t *_jit)
{
    jit_inc_synth(ellipsis);
    if (_jitc->prepare) {
	jit_link_prepare();
	assert(!(_jitc->function->call.call & jit_call_varargs));
	_jitc->function->call.call |= jit_call_varargs;
    }
    else {
	jit_link_prolog();
	assert(!(_jitc->function->self.call & jit_call_varargs));
	_jitc->function->self.call |= jit_call_varargs;

	_jitc->function->vagp = _jitc->function->self.argi;
    }
    jit_dec_synth();
}

void
_jit_va_push(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(va_push, u);
    jit_pushargr(u);
    jit_dec_synth();
}

jit_node_t *
_jit_arg(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    _jitc->function->self.size -= sizeof(jit_word_t);
    if (jit_arg_reg_p(_jitc->function->self.argi))
	offset = _jitc->function->self.argi++;
    else
	offset = _jitc->function->self.size;
    node = jit_new_node_ww(jit_code_arg, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

jit_node_t *
_jit_arg_f(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    _jitc->function->self.size -= sizeof(jit_word_t);
    if (jit_arg_reg_p(_jitc->function->self.argi))
	offset = _jitc->function->self.argi++;
    else
	offset = _jitc->function->self.size;
    node = jit_new_node_ww(jit_code_arg_f, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

jit_node_t *
_jit_arg_d(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    if (_jitc->function->self.argi & 1) {
	++_jitc->function->self.argi;
	_jitc->function->self.size -= sizeof(jit_word_t);
    }
    _jitc->function->self.size -= sizeof(jit_float64_t);
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
	offset = _jitc->function->self.argi + 1;
	_jitc->function->self.argi += 2;
    }
    else {
	/* "Standard" initial value (-52) is unaligned */
	if (_jitc->function->self.size & 7)
	    _jitc->function->self.size -= sizeof(jit_word_t);
	offset = _jitc->function->self.size;
    }
    node = jit_new_node_ww(jit_code_arg_d, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

void
_jit_getarg_c(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_c, u, v);
    if (v->u.w >= 0)
	jit_extr_c(u, _R26 - v->u.w);
    else
	jit_ldxi_c(u, JIT_FP, v->u.w + 3);
    jit_dec_synth();
}

void
_jit_getarg_uc(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_uc, u, v);
    if (v->u.w >= 0)
	jit_extr_uc(u, _R26 - v->u.w);
    else
	jit_ldxi_uc(u, JIT_FP, v->u.w + 3);
    jit_dec_synth();
}

void
_jit_getarg_s(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_s, u, v);
    if (v->u.w >= 0)
	jit_extr_s(u, _R26 - v->u.w);
    else
	jit_ldxi_s(u, JIT_FP, v->u.w + 2);
    jit_dec_synth();
}

void
_jit_getarg_us(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_us, u, v);
    if (v->u.w >= 0)
	jit_extr_us(u, _R26 - v->u.w);
    else
	jit_ldxi_us(u, JIT_FP, v->u.w + 2);
    jit_dec_synth();
}

void
_jit_getarg_i(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_i, u, v);
    if (v->u.w >= 0)
	jit_movr(u, _R26 - v->u.w);
    else
	jit_ldxi_i(u, JIT_FP, v->u.w);
    jit_dec_synth();
}

void
_jit_putargr(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(putargr, u, v);
    if (v->u.w >= 0)
	jit_movr(_R26 - v->u.w, u);
    else
	jit_stxi(v->u.w, JIT_FP, u);
    jit_dec_synth();
}

void
_jit_putargi(jit_state_t *_jit, jit_word_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(putargi, u, v);
    if (v->u.w >= 0)
	jit_movi(_R26 - v->u.w, u);
    else {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(v->u.w, JIT_FP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_getarg_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_wp(getarg_f, u, v);
    if (v->u.w >= 0)
	jit_movr_f(u, _F4 - v->u.w);
    else
	jit_ldxi_f(u, JIT_FP, v->u.w);
    jit_dec_synth();
}

void
_jit_putargr_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_wp(putargr_f, u, v);
    if (v->u.w >= 0)
	jit_movr_f(_F4 - v->u.w, u);
    else
	jit_stxi_f(v->u.w, JIT_FP, u);
    jit_dec_synth();
}

void
_jit_putargi_f(jit_state_t *_jit, jit_float32_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_fp(putargi_f, u, v);
    if (v->u.w >= 0)
	jit_movi_f(_R26 - v->u.w, u);
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
	jit_stxi_f(v->u.w, JIT_FP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_getarg_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(getarg_d, u, v);
    if (v->u.w >= 0)
	jit_movr_d(u, _F4 - v->u.w);
    else
	jit_ldxi_d(u, JIT_FP, v->u.w);
    jit_dec_synth();
}

void
_jit_putargr_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(putargr_d, u, v);
    if (v->u.w >= 0)
	jit_movr_d(_F4 - v->u.w, u);
    else
	jit_stxi_d(v->u.w, JIT_FP, u);
    jit_dec_synth();
}

void
_jit_putargi_d(jit_state_t *_jit, jit_float64_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_dp(putargi_d, u, v);
    if (v->u.w >= 0)
	jit_movi_d(_R26 - v->u.w, u);
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
	jit_stxi_d(v->u.w, JIT_FP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_pushargr(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr, u);
    jit_link_prepare();
    _jitc->function->call.size -= sizeof(jit_word_t);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr(_R26 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else
	jit_stxi(_jitc->function->call.size + params_offset, JIT_SP, u);
    jit_dec_synth();
}

void
_jit_pushargi(jit_state_t *_jit, jit_word_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_inc_synth_w(pushargi, u);
    jit_link_prepare();
    _jitc->function->call.size -= sizeof(jit_word_t);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi(_R26 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(_jitc->function->call.size + params_offset, JIT_SP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_pushargr_f(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr_f, u);
    jit_link_prepare();
    _jitc->function->call.size -= sizeof(jit_word_t);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr_f(_F4 - _jitc->function->call.argi, u);
#if !defined(__hpux)
	/* HP-UX appears to always pass float arguments in gpr registers */
	if (_jitc->function->call.call & jit_call_varargs)
#endif
	{
	    jit_stxi_f(alloca_offset - 8, JIT_FP, u);
	    jit_ldxi(_R26 - _jitc->function->call.argi, JIT_FP,
		     alloca_offset - 8);
	}
	++_jitc->function->call.argi;
    }
    else
	jit_stxi_f(_jitc->function->call.size + params_offset, JIT_SP, u);
    jit_dec_synth();
}

void
_jit_pushargi_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_inc_synth_f(pushargi_f, u);
    jit_link_prepare();
    _jitc->function->call.size -= sizeof(jit_word_t);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi_f(_F4 - _jitc->function->call.argi, u);
#if !defined(__hpux)
	/* HP-UX appears to always pass float arguments in gpr registers */
	if (_jitc->function->call.call & jit_call_varargs)
#endif
	{
	    jit_stxi_f(alloca_offset - 8, JIT_FP,
		       _F4 - _jitc->function->call.argi);
	    jit_ldxi(_R26 - _jitc->function->call.argi,
		     JIT_FP, alloca_offset - 8);
	}
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
	jit_stxi_f(_jitc->function->call.size + params_offset, JIT_SP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_pushargr_d(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr_d, u);
    jit_link_prepare();
    _jitc->function->call.size -= sizeof(jit_float64_t);
    if (_jitc->function->call.argi & 1) {
	++_jitc->function->call.argi;
	_jitc->function->call.size -= sizeof(jit_word_t);
    }
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr_d(_F4 - (_jitc->function->call.argi + 1), u);
#if !defined(__hpux)
	/* HP-UX appears to always pass float arguments in gpr registers */
	if (_jitc->function->call.call & jit_call_varargs)
#endif
	{
	    jit_stxi_d(alloca_offset - 8, JIT_FP, u);
	    jit_ldxi(_R26 - _jitc->function->call.argi,
		     JIT_FP, alloca_offset - 4);
	    jit_ldxi(_R25 - _jitc->function->call.argi,
		     JIT_FP, alloca_offset - 8);
	}
	_jitc->function->call.argi += 2;
    }
    else {
	/* "Standard" initial value (-52) is unaligned */
	if ((_jitc->function->call.size + params_offset) & 7)
	    _jitc->function->call.size -= sizeof(jit_word_t);
	jit_stxi_d(_jitc->function->call.size + params_offset, JIT_SP, u);
    }
    jit_dec_synth();
}

void
_jit_pushargi_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_inc_synth_d(pushargi_d, u);
    jit_link_prepare();
    _jitc->function->call.size -= sizeof(jit_float64_t);
    if (_jitc->function->call.argi & 1) {
	++_jitc->function->call.argi;
	_jitc->function->call.size -= sizeof(jit_word_t);
    }
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi_d(_F4 - (_jitc->function->call.argi + 1), u);
#if !defined(__hpux)
	/* HP-UX appears to always pass float arguments in gpr registers */
	if (_jitc->function->call.call & jit_call_varargs)
#endif
	{
	    jit_stxi_d(alloca_offset - 8, JIT_FP,
		       _F4 - (_jitc->function->call.argi + 1));
	    jit_ldxi(_R26 - _jitc->function->call.argi,
		     JIT_FP, alloca_offset - 4);
	    jit_ldxi(_R25 - _jitc->function->call.argi,
		     JIT_FP, alloca_offset - 8);
	}
	_jitc->function->call.argi += 2;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
	if ((_jitc->function->call.size + params_offset) & 7)
	    _jitc->function->call.size -= sizeof(jit_word_t);
	jit_stxi_d(_jitc->function->call.size + params_offset, JIT_SP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

jit_bool_t
_jit_regarg_p(jit_state_t *_jit, jit_node_t *node, jit_int32_t regno)
{
    jit_int32_t		spec;
    spec = jit_class(_rvs[regno].spec);
    if (spec & jit_class_arg) {
	if (spec & jit_class_gpr) {
	    regno -= _R23;
	    if (regno >= 0 && regno < node->v.w)
		return (1);
	}
	else if (spec & jit_class_fpr) {
	    regno = _F4 - regno;
	    if (regno >= 0 && regno < node->w.w)
		return (1);
	}
    }
    return (0);
}

void
_jit_finishr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_node_t		*call;
    assert(_jitc->function);
    jit_inc_synth_w(finishr, r0);
    if (_jitc->function->self.alen > _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    call = jit_callr(r0);
    call->v.w = call->w.w = _jitc->function->call.argi;
    _jitc->function->call.argi = _jitc->function->call.size = 0;
    _jitc->prepare = 0;
    jit_dec_synth();
}

jit_node_t *
_jit_finishi(jit_state_t *_jit, jit_pointer_t i0)
{
    jit_node_t		*node;
    assert(_jitc->function);
    jit_inc_synth_w(finishi, (jit_word_t)i0);
    if (_jitc->function->self.alen > _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    node = jit_calli(i0);
    node->v.w = node->w.w = _jitc->function->call.argi;
    _jitc->function->call.argi = _jitc->function->call.size = 0;
    _jitc->prepare = 0;
    jit_dec_synth();
    return (node);
}

void
_jit_retval_c(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_c, r0);
    jit_extr_c(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_uc(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_uc, r0);
    jit_extr_uc(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_s(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_s, r0);
    jit_extr_s(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_us(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_us, r0);
    jit_extr_us(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_i(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_i, r0);
    jit_movr(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_f(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_f, r0);
    jit_movr_f(r0, JIT_FRET);
    jit_dec_synth();
}

void
_jit_retval_d(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_d, r0);
    jit_movr_d(r0, JIT_FRET);
    jit_dec_synth();
}

jit_pointer_t
_emit_code(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_node_t		*temp;
    jit_word_t		 word;
    jit_int32_t		 value;
    jit_int32_t		 offset;
    struct {
	jit_node_t	*node;
	jit_word_t	 word;
#if DEVEL_DISASSEMBLER
	jit_word_t	 prevw;
#endif
	jit_int32_t	 patch_offset;
    } undo;
#if DEVEL_DISASSEMBLER
    jit_word_t		 prevw;
#endif

    _jitc->function = NULL;

    jit_reglive_setup();

    undo.word = 0;
    undo.node = NULL;
    undo.patch_offset = 0;

#define case_rr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w), rn(node->v.w));		\
		break
#define case_rw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w), node->v.w);		\
		break
#define case_wr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w, rn(node->v.w));		\
		break
#define case_rrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w),				\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_rrrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_rrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w),rn(node->v.w), node->w.w);	\
		break
#define case_rrrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), node->w.w);		\
		break
#define case_rrf(name, type, size)					\
	    case jit_code_##name##i##type:				\
		assert(node->flag & jit_flag_data);			\
		name##i##type(rn(node->u.w), rn(node->v.w),		\
			      (jit_float##size##_t *)node->w.n->u.w);	\
		break
#define case_wrr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w,rn(node->v.w), rn(node->w.w));	\
		break
#define case_brr(name, type)						\
	    case jit_code_##name##r##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##r##type(temp->u.w, rn(node->v.w),		\
				  rn(node->w.w));			\
		else {							\
		    word = name##r##type(_jit->pc.w,			\
					 rn(node->v.w), rn(node->w.w));	\
		    patch(word, node);					\
		}							\
		break
#define case_brw(name, type)						\
	    case jit_code_##name##i##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i##type(temp->u.w,				\
				  rn(node->v.w), node->w.w);		\
		else {							\
		    word = name##i##type(_jit->pc.w,			\
					 rn(node->v.w), node->w.w);	\
		    patch(word, node);					\
		}							\
		break
#define case_brf(name, type, size)					\
	    case jit_code_##name##i##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i##type(temp->u.w, rn(node->v.w),		\
				(jit_float##size##_t *)node->w.n->u.w);	\
		else {							\
		    word = name##i##type(_jit->pc.w, rn(node->v.w),	\
				(jit_float##size##_t *)node->w.n->u.w);	\
		    patch(word, node);					\
		}							\
		break
#if DEVEL_DISASSEMBLER
    prevw = _jit->pc.w;
#endif
    for (node = _jitc->head; node; node = node->next) {
	if (_jit->pc.uc >= _jitc->code.end)
	    return (NULL);

#if DEVEL_DISASSEMBLER
	node->offset = (jit_uword_t)_jit->pc.w - (jit_uword_t)prevw;
	prevw = _jit->pc.w;
#endif
	value = jit_classify(node->code);
	jit_regarg_set(node, value);
	switch (node->code) {
	    case jit_code_align:
		assert(!(node->u.w & (node->u.w - 1)) &&
		       node->u.w <= sizeof(jit_word_t));
		if (node->u.w == sizeof(jit_word_t) &&
		    (word = _jit->pc.w & (sizeof(jit_word_t) - 1)))
		    nop(sizeof(jit_word_t) - word);
		break;
	    case jit_code_note:		case jit_code_name:
		node->u.w = _jit->pc.w;
		break;
	    case jit_code_label:
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		break;
		case_rrr(add,);
		case_rrw(add,);
		case_rrr(addc,);
		case_rrw(addc,);
		case_rrr(addx,);
		case_rrw(addx,);
		case_rrr(sub,);
		case_rrw(sub,);
		case_rrr(subc,);
		case_rrw(subc,);
		case_rrr(subx,);
		case_rrw(subx,);
		case_rrw(rsb,);
		case_rrr(mul,);
		case_rrw(mul,);
		case_rrrr(qmul,);
		case_rrrw(qmul,);
		case_rrrr(qmul, _u);
		case_rrrw(qmul, _u);
		case_rrr(div,);
		case_rrw(div,);
		case_rrr(div, _u);
		case_rrw(div, _u);
		case_rrr(rem,);
		case_rrw(rem,);
		case_rrr(rem, _u);
		case_rrw(rem, _u);
		case_rrrr(qdiv,);
		case_rrrw(qdiv,);
		case_rrrr(qdiv, _u);
		case_rrrw(qdiv, _u);
		case_rrr(and,);
		case_rrw(and,);
		case_rrr(or,);
		case_rrw(or,);
		case_rrr(xor,);
		case_rrw(xor,);
		case_rrr(lsh,);
		case_rrw(lsh,);
		case_rrr(rsh,);
		case_rrw(rsh,);
		case_rrr(rsh, _u);
		case_rrw(rsh, _u);
		case_rr(mov,);
	    case jit_code_movi:
		if (node->flag & jit_flag_node) {
		    temp = node->v.n;
		    if (temp->code == jit_code_data ||
			(temp->code == jit_code_label &&
			 (temp->flag & jit_flag_patch)))
			movi(rn(node->u.w), temp->u.w);
		    else {
			assert(temp->code == jit_code_label ||
			       temp->code == jit_code_epilog);
			word = movi_p(rn(node->u.w), node->v.w);
			patch(word, node);
		    }
		}
		else
		    movi(rn(node->u.w), node->v.w);
		break;
		case_rr(neg,);
		case_rr(com,);
		case_rr(ext, _c);
		case_rr(ext, _uc);
		case_rr(ext, _s);
		case_rr(ext, _us);
		case_rr(hton, _us);
		case_rr(hton, _ui);
		case_rrr(lt,);
		case_rrw(lt,);
		case_rrr(lt, _u);
		case_rrw(lt, _u);
		case_rrr(le,);
		case_rrw(le,);
		case_rrr(le, _u);
		case_rrw(le, _u);
		case_rrr(eq,);
		case_rrw(eq,);
		case_rrr(ge,);
		case_rrw(ge,);
		case_rrr(ge, _u);
		case_rrw(ge, _u);
		case_rrr(gt,);
		case_rrw(gt,);
		case_rrr(gt, _u);
		case_rrw(gt, _u);
		case_rrr(ne,);
		case_rrw(ne,);
		case_rr(ld, _c);
		case_rw(ld, _c);
		case_rr(ld, _uc);
		case_rw(ld, _uc);
		case_rr(ld, _s);
		case_rw(ld, _s);
		case_rr(ld, _us);
		case_rw(ld, _us);
		case_rr(ld, _i);
		case_rw(ld, _i);
		case_rrr(ldx, _c);
		case_rrw(ldx, _c);
		case_rrr(ldx, _uc);
		case_rrw(ldx, _uc);
		case_rrr(ldx, _s);
		case_rrw(ldx, _s);
		case_rrr(ldx, _us);
		case_rrw(ldx, _us);
		case_rrr(ldx, _i);
		case_rrw(ldx, _i);
		case_rr(st, _c);
		case_wr(st, _c);
		case_rr(st, _s);
		case_wr(st, _s);
		case_rr(st, _i);
		case_wr(st, _i);
		case_rrr(stx, _c);
		case_wrr(stx, _c);
		case_rrr(stx, _s);
		case_wrr(stx, _s);
		case_rrr(stx, _i);
		case_wrr(stx, _i);
		case_brr(blt,);
		case_brw(blt,);
		case_brr(blt, _u);
		case_brw(blt, _u);
		case_brr(ble,);
		case_brw(ble,);
		case_brr(ble, _u);
		case_brw(ble, _u);
		case_brr(beq,);
		case_brw(beq,);
		case_brr(bge,);
		case_brw(bge,);
		case_brr(bge, _u);
		case_brw(bge, _u);
		case_brr(bgt,);
		case_brw(bgt,);
		case_brr(bgt, _u);
		case_brw(bgt, _u);
		case_brr(bne,);
		case_brw(bne,);
		case_brr(bms,);
		case_brw(bms,);
		case_brr(bmc,);
		case_brw(bmc,);
		case_brr(boadd,);
		case_brw(boadd,);
		case_brr(boadd, _u);
		case_brw(boadd, _u);
		case_brr(bxadd,);
		case_brw(bxadd,);
		case_brr(bxadd, _u);
		case_brw(bxadd, _u);
		case_brr(bosub,);
		case_brw(bosub,);
		case_brr(bosub, _u);
		case_brw(bosub, _u);
		case_brr(bxsub,);
		case_brw(bxsub,);
		case_brr(bxsub, _u);
		case_brw(bxsub, _u);
		case_rr(mov, _f);
	    case jit_code_movi_f:
		assert(node->flag & jit_flag_data);
		movi_f(rn(node->u.w), (jit_float32_t *)node->v.n->u.w);
		break;
		case_rr(mov, _d);
	    case jit_code_movi_d:
		assert(node->flag & jit_flag_data);
		movi_d(rn(node->u.w), (jit_float64_t *)node->v.n->u.w);
		break;
		case_rr(trunc, _f_i);
		case_rr(trunc, _d_i);
		case_rr(ext, _f);
		case_rr(ext, _d);
		case_rr(ext, _d_f);
		case_rr(ext, _f_d);
		case_rr(abs, _f);
		case_rr(abs, _d);
		case_rr(neg, _f);
		case_rr(neg, _d);
		case_rr(sqrt, _f);
		case_rr(sqrt, _d);
		case_rrr(add, _f);
		case_rrf(add, _f, 32);
		case_rrr(add, _d);
		case_rrf(add, _d, 64);
		case_rrr(sub, _f);
		case_rrf(sub, _f, 32);
		case_rrf(rsb, _f, 32);
		case_rrr(sub, _d);
		case_rrf(sub, _d, 64);
		case_rrf(rsb, _d, 64);
		case_rrr(mul, _f);
		case_rrf(mul, _f, 32);
		case_rrr(mul, _d);
		case_rrf(mul, _d, 64);
		case_rrr(div, _f);
		case_rrf(div, _f, 32);
		case_rrr(div, _d);
		case_rrf(div, _d, 64);
		case_rrr(lt, _f);
		case_rrf(lt, _f, 32);
		case_rrr(lt, _d);
		case_rrf(lt, _d, 64);
		case_rrr(le, _f);
		case_rrf(le, _f, 32);
		case_rrr(le, _d);
		case_rrf(le, _d, 64);
		case_rrr(eq, _f);
		case_rrf(eq, _f, 32);
		case_rrr(eq, _d);
		case_rrf(eq, _d, 64);
		case_rrr(ge, _f);
		case_rrf(ge, _f, 32);
		case_rrr(ge, _d);
		case_rrf(ge, _d, 64);
		case_rrr(gt, _f);
		case_rrf(gt, _f, 32);
		case_rrr(gt, _d);
		case_rrf(gt, _d, 64);
		case_rrr(ne, _f);
		case_rrf(ne, _f, 32);
		case_rrr(ne, _d);
		case_rrf(ne, _d, 64);
		case_rrr(unlt, _f);
		case_rrf(unlt, _f, 32);
		case_rrr(unlt, _d);
		case_rrf(unlt, _d, 64);
		case_rrr(unle, _f);
		case_rrf(unle, _f, 32);
		case_rrr(unle, _d);
		case_rrf(unle, _d, 64);
		case_rrr(uneq, _f);
		case_rrf(uneq, _f, 32);
		case_rrr(uneq, _d);
		case_rrf(uneq, _d, 64);
		case_rrr(unge, _f);
		case_rrf(unge, _f, 32);
		case_rrr(unge, _d);
		case_rrf(unge, _d, 64);
		case_rrr(ungt, _f);
		case_rrf(ungt, _f, 32);
		case_rrr(ungt, _d);
		case_rrf(ungt, _d, 64);
		case_rrr(ltgt, _f);
		case_rrf(ltgt, _f, 32);
		case_rrr(ltgt, _d);
		case_rrf(ltgt, _d, 64);
		case_rrr(ord, _f);
		case_rrf(ord, _f, 32);
		case_rrr(ord, _d);
		case_rrf(ord, _d, 64);
		case_rrr(unord, _f);
		case_rrf(unord, _f, 32);
		case_rrr(unord, _d);
		case_rrf(unord, _d, 64);
		case_rr(ld, _f);
		case_rw(ld, _f);
		case_rr(ld, _d);
		case_rw(ld, _d);
		case_rrr(ldx, _f);
		case_rrw(ldx, _f);
		case_rrr(ldx, _d);
		case_rrw(ldx, _d);
		case_rr(st, _f);
		case_wr(st, _f);
		case_rr(st, _d);
		case_wr(st, _d);
		case_rrr(stx, _f);
		case_wrr(stx, _f);
		case_rrr(stx, _d);
		case_wrr(stx, _d);
		case_brr(blt, _f);
		case_brf(blt, _f, 32);
		case_brr(blt, _d);
		case_brf(blt, _d, 64);
		case_brr(ble, _f);
		case_brf(ble, _f, 32);
		case_brr(ble, _d);
		case_brf(ble, _d, 64);
		case_brr(beq, _f);
		case_brf(beq, _f, 32);
		case_brr(beq, _d);
		case_brf(beq, _d, 64);
		case_brr(bge, _f);
		case_brf(bge, _f, 32);
		case_brr(bge, _d);
		case_brf(bge, _d, 64);
		case_brr(bgt, _f);
		case_brf(bgt, _f, 32);
		case_brr(bgt, _d);
		case_brf(bgt, _d, 64);
		case_brr(bne, _f);
		case_brf(bne, _f, 32);
		case_brr(bne, _d);
		case_brf(bne, _d, 64);
		case_brr(bunlt, _f);
		case_brf(bunlt, _f, 32);
		case_brr(bunlt, _d);
		case_brf(bunlt, _d, 64);
		case_brr(bunle, _f);
		case_brf(bunle, _f, 32);
		case_brr(bunle, _d);
		case_brf(bunle, _d, 64);
		case_brr(buneq, _f);
		case_brf(buneq, _f, 32);
		case_brr(buneq, _d);
		case_brf(buneq, _d, 64);
		case_brr(bunge, _f);
		case_brf(bunge, _f, 32);
		case_brr(bunge, _d);
		case_brf(bunge, _d, 64);
		case_brr(bungt, _f);
		case_brf(bungt, _f, 32);
		case_brr(bungt, _d);
		case_brf(bungt, _d, 64);
		case_brr(bltgt, _f);
		case_brf(bltgt, _f, 32);
		case_brr(bltgt, _d);
		case_brf(bltgt, _d, 64);
		case_brr(bord, _f);
		case_brf(bord, _f, 32);
		case_brr(bord, _d);
		case_brf(bord, _d, 64);
		case_brr(bunord, _f);
		case_brf(bunord, _f, 32);
		case_brr(bunord, _d);
		case_brf(bunord, _d, 64);
	    case jit_code_jmpr:
		jmpr(rn(node->u.w));
		break;
	    case jit_code_jmpi:
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    if (temp->flag & jit_flag_patch)
			jmpi(temp->u.w);
		    else {
			word = jmpi_p(_jit->pc.w);
			patch(word, node);
		    }
		}
		else
		    jmpi(node->u.w);
		break;
	    case jit_code_callr:
		callr(rn(node->u.w));
		break;
	    case jit_code_calli:
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    if (!(temp->flag & jit_flag_patch)) {
			word = calli_p(temp->u.w);
			patch(word, node);
		    }
		    else
			calli(temp->u.w);
		}
		else
		    calli(node->u.w);
		break;
	    case jit_code_prolog:
		_jitc->function = _jitc->functions.ptr + node->w.w;
		undo.node = node;
		undo.word = _jit->pc.w;
#if DEVEL_DISASSEMBLER
		undo.prevw = prevw;
#endif
		undo.patch_offset = _jitc->patches.offset;
	    restart_function:
		_jitc->again = 0;
		prolog(node);
		break;
	    case jit_code_epilog:
		assert(_jitc->function == _jitc->functions.ptr + node->w.w);
		if (_jitc->again) {
		    for (temp = undo.node->next;
			 temp != node; temp = temp->next) {
			if (temp->code == jit_code_label ||
			    temp->code == jit_code_epilog)
			    temp->flag &= ~jit_flag_patch;
		    }
		    temp->flag &= ~jit_flag_patch;
		    node = undo.node;
		    _jit->pc.w = undo.word;
#if DEVEL_DISASSEMBLER
		    prevw = undo.prevw;
#endif
		    _jitc->patches.offset = undo.patch_offset;
		    goto restart_function;
		}
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		epilog(node);
		_jitc->function = NULL;
		break;
	    case jit_code_va_start:
		vastart(rn(node->u.w));
		break;
	    case jit_code_va_arg:
		vaarg(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_va_arg_d:
		vaarg_d(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_live:
	    case jit_code_arg:			case jit_code_ellipsis:
	    case jit_code_va_push:
	    case jit_code_allocai:		case jit_code_allocar:
	    case jit_code_arg_f:		case jit_code_arg_d:
	    case jit_code_va_end:
	    case jit_code_ret:
	    case jit_code_retr:			case jit_code_reti:
	    case jit_code_retr_f:		case jit_code_reti_f:
	    case jit_code_retr_d:		case jit_code_reti_d:
	    case jit_code_getarg_c:		case jit_code_getarg_uc:
	    case jit_code_getarg_s:		case jit_code_getarg_us:
	    case jit_code_getarg_i:
	    case jit_code_getarg_f:		case jit_code_getarg_d:
	    case jit_code_putargr:		case jit_code_putargi:
	    case jit_code_putargr_f:		case jit_code_putargi_f:
	    case jit_code_putargr_d:		case jit_code_putargi_d:
	    case jit_code_pushargr:		case jit_code_pushargi:
	    case jit_code_pushargr_f:		case jit_code_pushargi_f:
	    case jit_code_pushargr_d:		case jit_code_pushargi_d:
	    case jit_code_retval_c:		case jit_code_retval_uc:
	    case jit_code_retval_s:		case jit_code_retval_us:
	    case jit_code_retval_i:
	    case jit_code_retval_f:		case jit_code_retval_d:
	    case jit_code_prepare:
	    case jit_code_finishr:		case jit_code_finishi:
		break;
	    default:
		abort();
	}
	jit_regarg_clr(node, value);
	assert(_jitc->regarg == 0 && _jitc->synth == 0);
	/* update register live state */
	jit_reglive(node);
    }
#undef case_brf
#undef case_brw
#undef case_brr
#undef case_wrr
#undef case_rrf
#undef case_rrrw
#undef case_rrw
#undef case_rrrr
#undef case_rrr
#undef case_wr
#undef case_rw
#undef case_rr

    for (offset = 0; offset < _jitc->patches.offset; offset++) {
	node = _jitc->patches.ptr[offset].node;
	word = node->code == jit_code_movi ? node->v.n->u.w : node->u.n->u.w;
	patch_at(_jitc->patches.ptr[offset].inst, word);
    }

    jit_flush(_jit->code.ptr, _jit->pc.uc);

    return (_jit->code.ptr);
}

#define CODE				1
#  include "jit_hppa-cpu.c"
#  include "jit_hppa-fpu.c"
#undef CODE

void
jit_flush(void *fptr, void *tptr)
{
    jit_word_t		f, t, s;
    s = sysconf(_SC_PAGE_SIZE);
    f = (jit_word_t)fptr & -s;
    t = (((jit_word_t)tptr) + s - 1) & -s;
#if defined(__hppa)
/* --- parisc2.0.pdf ---
		Programming Note

The minimum spacing that is guaranteed to work for "self-modifying code" is
shown in the code segment below. Since instruction prefetching is permitted,
any data cache flushes must be separated from any instruction cache flushes
by a SYNC. This will ensure that the "new" instruction will be written to
memory prior to any attempts at prefetching it as an instruction.

	LDIL	l%newinstr,rnew
	LDW	r%newinstr(0,rnew),temp
	LDIL	l%instr,rinstr
	STW	temp,r%instr(0,rinstr)
	FDC	r%instr(0,rinstr)
	SYNC
	FIC	r%instr(rinstr)
	SYNC
	instr	...
	(at least seven instructions)

This sequence assumes a uniprocessor system. In a multiprocessor system,
software must ensure no processor is executing code which is in the process
of being modified.
*/

/*
  Adapted from ffcall/trampoline/cache-hppa.c:__TR_clear_cache to
loop over addresses as it is unlikely from and to addresses would fit in
at most two cachelines.
  FIXME A cache line can be 16, 32, or 64 bytes.
 */
    /*
     * Copyright 1995-1997 Bruno Haible, <bruno@clisp.org>
     *
     * This is free software distributed under the GNU General Public Licence
     * described in the file COPYING. Contact the author if you don't have this
     * or can't live with it. There is ABSOLUTELY NO WARRANTY, explicit or implied,
     * on this software.
     */
    {
	jit_word_t	n = f + 32;
	register int	u, v;
	for (; f <= t; n = f + 32, f += 64) {
	    asm volatile ("fdc 0(0,%0)"
			  "\n\t" "fdc 0(0,%1)"
			  "\n\t" "sync"
			  :
			  : "r" (f), "r" (n)
			  );
	    asm volatile ("mfsp %%sr0,%1"
			  "\n\t" "ldsid (0,%4),%0"
			  "\n\t" "mtsp %0,%%sr0"
			  "\n\t" "fic 0(%%sr0,%2)"
			  "\n\t" "fic 0(%%sr0,%3)"
			  "\n\t" "sync"
			  "\n\t" "mtsp %1,%%sr0"
			  "\n\t" "nop"
			  "\n\t" "nop"
			  "\n\t" "nop"
			  "\n\t" "nop"
			  "\n\t" "nop"
			  "\n\t" "nop"
			  : "=r" (u), "=r" (v)
			  : "r" (f), "r" (n), "r" (f)
			  );
	}
    }
#else
    /* This is supposed to work but appears to fail on multiprocessor systems */
    __clear_cache((void *)f, (void *)t);
#endif
}

void
_emit_ldxi(jit_state_t *_jit, jit_gpr_t r0, jit_gpr_t r1, jit_word_t i0)
{
    ldxi(rn(r0), rn(r1), i0);
}

void
_emit_stxi(jit_state_t *_jit, jit_word_t i0, jit_gpr_t r0, jit_gpr_t r1)
{
    stxi(i0, rn(r0), rn(r1));
}

void
_emit_ldxi_d(jit_state_t *_jit, jit_fpr_t r0, jit_gpr_t r1, jit_word_t i0)
{
    ldxi_d(rn(r0), rn(r1), i0);
}

void
_emit_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_gpr_t r0, jit_fpr_t r1)
{
    stxi_d(i0, rn(r0), rn(r1));
}

static void
_patch(jit_state_t *_jit, jit_word_t instr, jit_node_t *node)
{
    jit_int32_t		flag;

    assert(node->flag & jit_flag_node);
    if (node->code == jit_code_movi)
	flag = node->v.n->flag;
    else
	flag = node->u.n->flag;
    assert(!(flag & jit_flag_patch));
    if (_jitc->patches.offset >= _jitc->patches.length) {
	jit_realloc((jit_pointer_t *)&_jitc->patches.ptr,
		    _jitc->patches.length * sizeof(jit_patch_t),
		    (_jitc->patches.length + 1024) * sizeof(jit_patch_t));
	_jitc->patches.length += 1024;
    }
    _jitc->patches.ptr[_jitc->patches.offset].inst = instr;
    _jitc->patches.ptr[_jitc->patches.offset].node = node;
    ++_jitc->patches.offset;
}
