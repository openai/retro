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

#define jit_arg_reg_p(i)		((i) >= 0 && (i) < 8)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#  define C_DISP			0
#  define S_DISP			0
#  define I_DISP			0
#  define F_DISP			0
#else
#  define C_DISP			8 - sizeof(jit_int8_t)
#  define S_DISP			8 - sizeof(jit_int16_t)
#  define I_DISP			8 - sizeof(jit_int32_t)
#  define F_DISP			8 - sizeof(jit_float32_t)
#endif

/*
 * Types
 */
typedef jit_pointer_t jit_va_list_t;

/*
 * Prototypes
 */
#define patch(instr, node)		_patch(_jit, instr, node)
static void _patch(jit_state_t*,jit_word_t,jit_node_t*);

#if defined(__GNUC__)
/* libgcc */
extern void __clear_cache(void *, void *);
#endif

#define PROTO				1
#  include "jit_ia64-cpu.c"
#  include "jit_ia64-fpu.c"
#undef PROTO

/*
 * Initialization
 */
jit_register_t		_rvs[] = {
    /* Always 0 */
    { 0,		 "r0"  },
    /* Global Pointer */
    { rc(sav)|1,	 "r1"  },
    /* Used when a register cannot be allocated */
    { 2,		 "r2"  },
    /* First scratch register */
    { rc(gpr)|3,	 "r3"  },
    /* Use r4 as lightning fp register */
    { rc(sav)|4,	 "r4"  },
    /* Do not touch callee save registers not automatically spill/reloaded */
    { rc(sav)|5,	 "r5"  },	{ rc(sav)|6,		"r6"  },
    { rc(sav)|7,	 "r7"  },
    /* Do not touch return register for the sake of simplicity, besides
     * having JIT_R0 being the same as JIT_RET usually an optimization */
    { 8,		 "r8"  },
    /* Return registers, use as temporaries */
    { rc(gpr)|9,	 "r9"  },
    { rc(gpr)|10,	 "r10" },	{ rc(gpr)|11,		"r11" },
    /* Stack pointer */
    { rc(sav)|12,	 "r12" },
    /* Thread pointer */
    { rc(sav)|13,	 "r13" },
    /* (Usually) assembly temporaries */
    { rc(gpr)|31,	 "r31" },	{ rc(gpr)|30,		"r30" },
    { rc(gpr)|29,	 "r29" },	{ rc(gpr)|28,		"r28" },
    { rc(gpr)|27,	 "r27" },	{ rc(gpr)|26,		"r26" },
    { rc(gpr)|25,	 "r25" },	{ rc(gpr)|24,		"r24" },
    { rc(gpr)|23,	 "r23" },	{ rc(gpr)|22,		"r22" },
    { rc(gpr)|21,	 "r21" },	{ rc(gpr)|20,		"r20" },
    { rc(gpr)|19,	 "r19" },	{ rc(gpr)|18,		"r18" },
    { rc(gpr)|17,	 "r17" },	{ rc(gpr)|16,		"r16" },
    { rc(gpr)|15,	 "r15" },	{ rc(gpr)|14,		"r14" },
    /* Do not allow allocating r32-r41 as temoraries for the sake of
     * avoiding the need of extra complexity  in the non backend code */
    { rc(arg)|32,	 "r32" },	{ rc(arg)|33,		"r33" },
    { rc(arg)|34,	 "r34" },	{ rc(arg)|35,		"r35" },
    { rc(arg)|36,	 "r36" },	{ rc(arg)|37,		"r37" },
    { rc(arg)|38,	 "r38" },	{ rc(arg)|39,		"r39" },
    /* JIT_R0-JIT_V3 */
    { rc(gpr)|rc(sav)|40, "r40" },	{ rc(gpr)|rc(sav)|41,	"r41" },
    { rc(gpr)|rc(sav)|42, "r42" },	{ rc(gpr)|rc(sav)|43,	"r43" },
    { rc(gpr)|rc(sav)|44, "r44" },	{ rc(gpr)|rc(sav)|45,	"r45" },
    { rc(gpr)|rc(sav)|46, "r46" },	{ rc(gpr)|rc(sav)|47,	"r47" },
    /* Temporaries/locals */
    { rc(gpr)|rc(sav)|48, "r48" },	{ rc(gpr)|rc(sav)|49,	"r49" },
    { rc(gpr)|rc(sav)|50, "r50" },	{ rc(gpr)|rc(sav)|51,	"r51" },
    { rc(gpr)|rc(sav)|52, "r52" },	{ rc(gpr)|rc(sav)|53,	"r53" },
    { rc(gpr)|rc(sav)|54, "r54" },	{ rc(gpr)|rc(sav)|55,	"r55" },
    { rc(gpr)|rc(sav)|56, "r56" },	{ rc(gpr)|rc(sav)|57,	"r57" },
    { rc(gpr)|rc(sav)|58, "r58" },	{ rc(gpr)|rc(sav)|59,	"r59" },
    { rc(gpr)|rc(sav)|60, "r60" },	{ rc(gpr)|rc(sav)|61,	"r61" },
    { rc(gpr)|rc(sav)|62, "r62" },	{ rc(gpr)|rc(sav)|63,	"r63" },
    { rc(gpr)|rc(sav)|64, "r64" },	{ rc(gpr)|rc(sav)|65,	"r65" },
    { rc(gpr)|rc(sav)|66, "r66" },	{ rc(gpr)|rc(sav)|67,	"r67" },
    { rc(gpr)|rc(sav)|68, "r68" },	{ rc(gpr)|rc(sav)|69,	"r69" },
    { rc(gpr)|rc(sav)|70, "r70" },	{ rc(gpr)|rc(sav)|71,	"r71" },
    { rc(gpr)|rc(sav)|72, "r72" },	{ rc(gpr)|rc(sav)|73,	"r73" },
    { rc(gpr)|rc(sav)|74, "r74" },	{ rc(gpr)|rc(sav)|75,	"r75" },
    { rc(gpr)|rc(sav)|76, "r76" },	{ rc(gpr)|rc(sav)|77,	"r77" },
    { rc(gpr)|rc(sav)|78, "r78" },	{ rc(gpr)|rc(sav)|79,	"r79" },
    { rc(gpr)|rc(sav)|80, "r80" },	{ rc(gpr)|rc(sav)|81,	"r81" },
    { rc(gpr)|rc(sav)|82, "r82" },	{ rc(gpr)|rc(sav)|83,	"r83" },
    { rc(gpr)|rc(sav)|84, "r84" },	{ rc(gpr)|rc(sav)|85,	"r85" },
    { rc(gpr)|rc(sav)|86, "r86" },	{ rc(gpr)|rc(sav)|87,	"r87" },
    { rc(gpr)|rc(sav)|88, "r88" },	{ rc(gpr)|rc(sav)|89,	"r89" },
    { rc(gpr)|rc(sav)|90, "r90" },	{ rc(gpr)|rc(sav)|91,	"r91" },
    { rc(gpr)|rc(sav)|92, "r92" },	{ rc(gpr)|rc(sav)|93,	"r93" },
    { rc(gpr)|rc(sav)|94, "r94" },	{ rc(gpr)|rc(sav)|95,	"r95" },
    { rc(gpr)|rc(sav)|96, "r96" },	{ rc(gpr)|rc(sav)|97,	"r97" },
    { rc(gpr)|rc(sav)|98, "r98" },	{ rc(gpr)|rc(sav)|99,	"r99" },
    { rc(gpr)|rc(sav)|100,"r100"},	{ rc(gpr)|rc(sav)|101,	"r101"},
    { rc(gpr)|rc(sav)|102,"r102"},	{ rc(gpr)|rc(sav)|103,	"r103"},
    { rc(gpr)|rc(sav)|104,"r104"},	{ rc(gpr)|rc(sav)|105,	"r105"},
    { rc(gpr)|rc(sav)|106,"r106"},	{ rc(gpr)|rc(sav)|107,	"r107"},
    { rc(gpr)|rc(sav)|108,"r108"},	{ rc(gpr)|rc(sav)|109,	"r109"},
    { rc(gpr)|rc(sav)|110,"r110"},	{ rc(gpr)|rc(sav)|111,	"r111"},
    { rc(gpr)|rc(sav)|112,"r112"},	{ rc(gpr)|rc(sav)|113,	"r113"},
    { rc(gpr)|rc(sav)|114,"r114"},
    /* Do not enable these because no matter what, want 13 free registers,
     * 5 for prolog and epilog and 8 for outgoing arguments */
    { 115,		 "r115"},
    { 116,		 "r116"},	{ 117,			"r117"},
    { 118,		 "r118"},	{ 119,			"r119"},
    { 120,		 "r120"},	{ 121,			"r121"},
    { 122,		 "r122"},	{ 123,			"r123"},
    { 124,		 "r124"},	{ 125,			"r125"},
    { 126,		 "r126"},	{ 127,			"r127"},
    /* Always 0.0 */
    { 0,		 "f0"  },
    /* Always 1.0 */
    { 1,		 "f1"  },
    /* Do not touch callee save registers not automatically spill/reloaded */
    { rc(sav)|2,	 "f2"  },	{ rc(sav)|3,		"f3"  },
    { rc(sav)|4,	 "f4"  },	{ rc(sav)|5,		"f5"  },
    /* Scratch */
    { rc(fpr)|6,	 "f6"  },	{ rc(fpr)|7,		"f7"  },
    /* Do not allocate for the sake of simplification */
    { rc(arg)|8,	 "f8"  },
    /* Scratch - Argument/return registers */
    { rc(arg)|9,	 "f9"  },
    { rc(arg)|10,	 "f10" },	{ rc(arg)|11,		"f11" },
    { rc(arg)|12,	 "f12" },	{ rc(arg)|13,		"f13" },
    { rc(arg)|14,	 "f14" },	{ rc(arg)|15,		"f15" },
    /* Do not touch callee save registers not automatically spill/reloaded */
    { rc(sav)|16,	 "f16" },	{ rc(sav)|17,		"f17" },
    { rc(sav)|18,	 "f18" },	{ rc(sav)|19,		"f19" },
    { rc(sav)|20,	 "f20" },	{ rc(sav)|21,		"f21" },
    { rc(sav)|22,	 "f22" },	{ rc(sav)|23,		"f23" },
    { rc(sav)|24,	 "f24" },	{ rc(sav)|25,		"f25" },
    { rc(sav)|26,	 "f26" },	{ rc(sav)|27,		"f27" },
    { rc(sav)|28,	 "f28" },	{ rc(sav)|29,		"f29" },
    { rc(sav)|30,	 "f30" },	{ rc(sav)|31,		"f31" },
    /* Scratch */
    { rc(fpr)|32,	 "f32" },	{ rc(fpr)|33,		"f33" },
    { rc(fpr)|34,	 "f34" },	{ rc(fpr)|35,		"f35" },
    { rc(fpr)|36,	 "f36" },	{ rc(fpr)|37,		"f37" },
    { rc(fpr)|38,	 "f38" },	{ rc(fpr)|39,		"f39" },
    { rc(fpr)|40,	 "f40" },	{ rc(fpr)|41,		"f41" },
    { rc(fpr)|42,	 "f42" },	{ rc(fpr)|43,		"f43" },
    { rc(fpr)|44,	 "f44" },	{ rc(fpr)|45,		"f45" },
    { rc(fpr)|46,	 "f46" },	{ rc(fpr)|47,		"f47" },
    { rc(fpr)|48,	 "f48" },	{ rc(fpr)|49,		"f49" },
    { rc(fpr)|50,	 "f50" },	{ rc(fpr)|51,		"f51" },
    { rc(fpr)|52,	 "f52" },	{ rc(fpr)|53,		"f53" },
    { rc(fpr)|54,	 "f54" },	{ rc(fpr)|55,		"f55" },
    { rc(fpr)|56,	 "f56" },	{ rc(fpr)|57,		"f57" },
    { rc(fpr)|58,	 "f58" },	{ rc(fpr)|59,		"f59" },
    { rc(fpr)|60,	 "f60" },	{ rc(fpr)|61,		"f61" },
    { rc(fpr)|62,	 "f62" },	{ rc(fpr)|63,		"f63" },
    { rc(fpr)|64,	 "f64" },	{ rc(fpr)|65,		"f65" },
    { rc(fpr)|66,	 "f66" },	{ rc(fpr)|67,		"f67" },
    { rc(fpr)|68,	 "f68" },	{ rc(fpr)|69,		"f69" },
    { rc(fpr)|70,	 "f70" },	{ rc(fpr)|71,		"f71" },
    { rc(fpr)|72,	 "f72" },	{ rc(fpr)|73,		"f73" },
    { rc(fpr)|74,	 "f74" },	{ rc(fpr)|75,		"f75" },
    { rc(fpr)|76,	 "f76" },	{ rc(fpr)|77,		"f77" },
    { rc(fpr)|78,	 "f78" },	{ rc(fpr)|79,		"f79" },
    { rc(fpr)|80,	 "f80" },	{ rc(fpr)|81,		"f81" },
    { rc(fpr)|82,	 "f82" },	{ rc(fpr)|83,		"f83" },
    { rc(fpr)|84,	 "f84" },	{ rc(fpr)|85,		"f85" },
    { rc(fpr)|86,	 "f86" },	{ rc(fpr)|87,		"f87" },
    { rc(fpr)|88,	 "f88" },	{ rc(fpr)|89,		"f89" },
    { rc(fpr)|90,	 "f90" },	{ rc(fpr)|91,		"f91" },
    { rc(fpr)|92,	 "f92" },	{ rc(fpr)|93,		"f93" },
    { rc(fpr)|94,	 "f94" },	{ rc(fpr)|95,		"f95" },
    { rc(fpr)|96,	 "f96" },	{ rc(fpr)|97,		"f97" },
    { rc(fpr)|98,	 "f98" },	{ rc(fpr)|99,		"f99" },
    { rc(fpr)|100,	 "f100"},	{ rc(fpr)|101,		"f101"},
    { rc(fpr)|102,	 "f102"},	{ rc(fpr)|103,		"f103"},
    { rc(fpr)|104,	 "f104"},	{ rc(fpr)|105,		"f105"},
    { rc(fpr)|106,	 "f106"},	{ rc(fpr)|107,		"f107"},
    { rc(fpr)|108,	 "f108"},	{ rc(fpr)|109,		"f109"},
    { rc(fpr)|110,	 "f110"},	{ rc(fpr)|111,		"f111"},
    { rc(fpr)|112,	 "f112"},	{ rc(fpr)|113,		"f113"},
    { rc(fpr)|114,	 "f114"},	{ rc(fpr)|115,		"f115"},
    { rc(fpr)|116,	 "f116"},	{ rc(fpr)|117,		"f117"},
    { rc(fpr)|118,	 "f118"},	{ rc(fpr)|119,		"f119"},
#if 0
    /* commented to fit a jit_regset_t in 256 bits, so that the fake
     * O0-O7 registers are easily patched when an argument is pushed */
    { rc(fpr)|120,	 "f120"},	{ rc(fpr)|121,		"f121"},
    { rc(fpr)|122,	 "f122"},	{ rc(fpr)|123,		"f123"},
    { rc(fpr)|124,	 "f124"},	{ rc(fpr)|125,		"f125"},
    { rc(fpr)|126,	 "f126"},	{ rc(fpr)|127,		"f127"},
#endif
    /* Fake registers to patch in movr and movi arguments */
    { rc(arg)|120,	 "o0"  },	{ rc(arg)|121,		"o1"  },
    { rc(arg)|122,	 "o2"  },	{ rc(arg)|123,		"o3"  },
    { rc(arg)|124,	 "o4"  },	{ rc(arg)|125,		"o5"  },
    { rc(arg)|126,	 "o6"  },	{ rc(arg)|127,		"o7"  },
    { _NOREG,		 "<none>" },
};

/*
 * Implementation
 */
void
jit_get_cpu(void)
{
}

void
_jit_init(jit_state_t *_jit)
{
    _jitc->reglen = jit_size(_rvs) - 1;
    jit_regset_new(&_jitc->regs);
    jit_carry = _NOREG;
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
    /* params_offset << 1, once for ours arguments, once for arguments
     * to any called function; required because offsets are computed
     * as JIT_FP displacement */
    _jitc->function->self.size = stack_framesize + (params_offset << 1);
    _jitc->function->self.argi = _jitc->function->self.argf =
	_jitc->function->self.aoff = _jitc->function->self.alen = 0;
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
    assert(_jitc->function);
    switch (length) {
	case 0:	case 1:						break;
	case 2:		_jitc->function->self.aoff &= -2;	break;
	case 3:	case 4:	_jitc->function->self.aoff &= -4;	break;
	default:	_jitc->function->self.aoff &= -8;	break;
    }
    _jitc->function->self.aoff -= length;
    if (!_jitc->realize) {
	jit_inc_synth_ww(allocai, _jitc->function->self.aoff, length);
	jit_dec_synth();
    }
    return (_jitc->function->self.aoff);
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
    jit_negr(reg, v);
    jit_andi(reg, reg, -16);
    jit_ldxi_i(u, JIT_FP, _jitc->function->aoffoff);
    jit_addr(u, u, reg);
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
	assert(!(_jitc->function->call.call & jit_call_varargs));
	_jitc->function->call.call |= jit_call_varargs;
	jit_link_prepare();
    }
    else {
	assert(!(_jitc->function->self.call & jit_call_varargs));
	_jitc->function->self.call |= jit_call_varargs;
	_jitc->function->vagp = _jitc->function->self.argi;
	jit_link_prolog();
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
    if (jit_arg_reg_p(_jitc->function->self.argi))
	offset = _jitc->function->self.argi++;
    else {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_word_t);
    }
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
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
	if (!(_jitc->function->self.call & jit_call_varargs))
	    offset = 8 + _jitc->function->self.argf++;
	else
	    offset = _jitc->function->self.argi;
	++_jitc->function->self.argi;
    }
   else {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_word_t);
    }
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
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
	if (!(_jitc->function->self.call & jit_call_varargs))
	    offset = 8 + _jitc->function->self.argf++;
	else
	    offset = _jitc->function->self.argi;
	++_jitc->function->self.argi;
    }
   else {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_float64_t);
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
    if (jit_arg_reg_p(v->u.w))
	jit_extr_c(u, _R32 + v->u.w);
    else
	jit_ldxi_c(u, JIT_FP, v->u.w + C_DISP);
    jit_dec_synth();
}

void
_jit_getarg_uc(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_uc, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_uc(u, _R32 + v->u.w);
    else
	jit_ldxi_uc(u, JIT_FP, v->u.w + C_DISP);
    jit_dec_synth();
}

void
_jit_getarg_s(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_s, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_s(u, _R32 + v->u.w);
    else
	jit_ldxi_s(u, JIT_FP, v->u.w + S_DISP);
    jit_dec_synth();
}

void
_jit_getarg_us(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_us, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_us(u, _R32 + v->u.w);
    else
	jit_ldxi_us(u, JIT_FP, v->u.w + S_DISP);
    jit_dec_synth();
}

void
_jit_getarg_i(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_i, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_i(u, _R32 + v->u.w);
    else
	jit_ldxi_i(u, JIT_FP, v->u.w + I_DISP);
    jit_dec_synth();
}

void
_jit_getarg_ui(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_ui, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_ui(u, _R32 + v->u.w);
    else
	jit_ldxi_ui(u, JIT_FP, v->u.w + I_DISP);
    jit_dec_synth();
}

void
_jit_getarg_l(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_l, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr(u, _R32 + v->u.w);
    else
	jit_ldxi(u, JIT_FP, v->u.w);
    jit_dec_synth();
}

void
_jit_putargr(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(putargr, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr(_R32 + v->u.w, u);
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
    if (jit_arg_reg_p(v->u.w))
	jit_movi(_R32 + v->u.w, u);
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
    if (jit_arg_reg_p(v->u.w))
	jit_movr_w_f(u, _OUT0 + v->u.w);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_f(u, _F8 + (v->u.w - 8));
    else
	jit_ldxi_f(u, JIT_FP, v->u.w + F_DISP);
    jit_dec_synth();
}

void
_jit_putargr_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_wp(putargr_f, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr_f_w(_OUT0 + v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_f(_F8 + (v->u.w - 8), u);
    else
	jit_stxi_f(v->u.w, JIT_FP, u + F_DISP);
    jit_dec_synth();
}

void
_jit_putargi_f(jit_state_t *_jit, jit_float32_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_fp(putargi_f, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movi_f_w(_OUT0 + v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movi_f(_F8 + (v->u.w - 8), u);
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
	jit_stxi_f(v->u.w, JIT_FP, regno + F_DISP);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_getarg_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(getarg_d, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr_w_d(u, _OUT0 + v->u.w);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_d(u, _F8 + (v->u.w - 8));
    else
	jit_ldxi_d(u, JIT_FP, v->u.w);
    jit_dec_synth();
}

void
_jit_putargr_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(putargr_d, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr_d_w(_OUT0 + v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_d(_F8 + (v->u.w - 8), u);
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
    if (jit_arg_reg_p(v->u.w))
	jit_movi_d_w(_OUT0 + v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movi_d(_F8 + (v->u.w - 8), u);
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
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr(_OUT0 + _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	jit_stxi(_jitc->function->call.size + params_offset, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
    }
    jit_dec_synth();
}

void
_jit_pushargi(jit_state_t *_jit, jit_word_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_inc_synth_w(pushargi, u);
    jit_link_prepare();
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi(_OUT0 + _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(_jitc->function->call.size + params_offset, JIT_SP, regno);
	_jitc->function->call.size += sizeof(jit_word_t);
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
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    jit_movr_f(_F8 + _jitc->function->call.argf, u);
	    ++_jitc->function->call.argf;
	}
	else
	    jit_movr_f_w(_OUT0 + _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	jit_stxi_f(_jitc->function->call.size + params_offset + F_DISP,
		   JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
    }
    jit_dec_synth();
}

void
_jit_pushargi_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_inc_synth_f(pushargi_f, u);
    jit_link_prepare();
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    jit_movi_f(_F8 + _jitc->function->call.argf, u);
	    ++_jitc->function->call.argf;
	}
	else
	    jit_movi_f_w(_OUT0 + _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
	jit_stxi_f(_jitc->function->call.size + params_offset + F_DISP,
		   JIT_SP, regno);
	_jitc->function->call.size += sizeof(jit_word_t);
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
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    jit_movr_d(_F8 + _jitc->function->call.argf, u);
	    ++_jitc->function->call.argf;
	}
	else
	    jit_movr_d_w(_OUT0 + _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	jit_stxi_d(_jitc->function->call.size + params_offset, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_float64_t);
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
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    jit_movi_d(_F8 + _jitc->function->call.argf, u);
	    ++_jitc->function->call.argf;
	}
	else
	    jit_movi_d_w(_OUT0 + _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
	jit_stxi_d(_jitc->function->call.size + params_offset, JIT_SP, regno);
	_jitc->function->call.size += sizeof(jit_float64_t);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

jit_bool_t
_jit_regarg_p(jit_state_t *_jit, jit_node_t *node, jit_int32_t regno)
{
    /* Argument registers are allocated from the pool of unused registers */
    return (0);
}

void
_jit_finishr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_node_t		*call;
    assert(_jitc->function);
    jit_inc_synth_w(finishr, r0);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    call = jit_callr(r0);
    call->v.w = _jitc->function->call.argi;
    call->w.w = _jitc->function->call.argf;
    _jitc->function->call.argi = _jitc->function->call.argf =
	_jitc->function->call.size = 0;
    _jitc->prepare = 0;
    jit_dec_synth();
}

jit_node_t *
_jit_finishi(jit_state_t *_jit, jit_pointer_t i0)
{
    jit_node_t		*node;
    assert(_jitc->function);
    jit_inc_synth_w(finishi, (jit_word_t)i0);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    node = jit_calli(i0);
    node->v.w = _jitc->function->call.argi;
    node->w.w = _jitc->function->call.argf;
    _jitc->function->call.argi = _jitc->function->call.argf =
	_jitc->function->call.size = 0;
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
    jit_extr_i(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_ui(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_ui, r0);
    jit_extr_ui(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_l(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_l, r0);
    jit_movr(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_f(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_l, r0);
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
	jit_word_t	 prolog_offset;
    } undo;
#if DEVEL_DISASSEMBLER
    jit_word_t		 prevw;
#endif

    _jitc->function = NULL;

    /* If did resize the code buffer, these were not reset */
    _jitc->ioff = 0;
    jit_regset_set_ui(&_jitc->regs, 0);
    _jitc->pred = 0;

    jit_reglive_setup();

    undo.word = 0;
    undo.node = NULL;
    undo.patch_offset = 0;

#if DEVEL_DISASSEMBLER
    prevw = _jit->pc.w;
#endif
    undo.prolog_offset = 0;
    for (node = _jitc->head; node; node = node->next)
	if (node->code != jit_code_label &&
	    node->code != jit_code_note &&
	    node->code != jit_code_name)
	    break;
    if (node && (node->code != jit_code_prolog ||
		 !(_jitc->functions.ptr + node->w.w)->assume_frame)) {
	/* code may start with a jump so add an initial function descriptor */
	word = _jit->pc.w + 16;
	il(word);		/* addr */
	il(0);			/* gp */
    }
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
		name##i##type(rn(node->u.w), rn(node->v.w), node->w.w);	\
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
		name##i##type(node->u.w, rn(node->v.w), rn(node->w.w));	\
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
    for (node = _jitc->head; node; node = node->next) {
	if (_jit->pc.uc >= _jitc->code.end)
	    return (NULL);

	value = jit_classify(node->code);
#if GET_JIT_SIZE
	sync();
#endif
#if DEVEL_DISASSEMBLER
	/* FIXME DEVEL_DISASSEMBLER should become DISASSEMBLER,
	 * but a "real" DEVEL_DISASSEMBLER should be required
	 * to turn the below "#if 0" into "#if 1" */
#  if 0		/* Since disassembly outputs 3 instructions at a time,
		 * make it "#if 1" for more clear debug output. */
	sync();
#  endif
	node->offset = (jit_uword_t)_jit->pc.w - (jit_uword_t)prevw;
	prevw = _jit->pc.w;
#endif
	jit_regarg_set(node, value);
	switch (node->code) {
	    case jit_code_align:
		assert(!(node->u.w & (node->u.w - 1)) &&
		       node->u.w <= sizeof(jit_word_t));
		/* nothing done */
		break;
	    case jit_code_note:		case jit_code_name:
		sync();
		node->u.w = _jit->pc.w;
		break;
	    case jit_code_label:
		sync();
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
		case_rrr(subx,);
		case_rrw(subx,);
		case_rrw(rsb,);
		case_rrr(subc,);
		case_rrw(subc,);
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
		case_rr(neg,);
		case_rr(com,);
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
		case_rr(hton, _us);
		case_rr(hton, _ui);
		case_rr(hton, _ul);
		case_rr(ext, _c);
		case_rr(ext, _uc);
		case_rr(ext, _s);
		case_rr(ext, _us);
		case_rr(ext, _i);
		case_rr(ext, _ui);
		case_rr(trunc, _f_i);
		case_rr(trunc, _d_i);
		case_rr(trunc, _f_l);
		case_rr(trunc, _d_l);
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
		case_rr(ld, _ui);
		case_rw(ld, _ui);
		case_rr(ld, _l);
		case_rw(ld, _l);
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
		case_rrr(ldx, _ui);
		case_rrw(ldx, _ui);
		case_rrr(ldx, _l);
		case_rrw(ldx, _l);
		case_rr(st, _c);
		case_wr(st, _c);
		case_rr(st, _s);
		case_wr(st, _s);
		case_rr(st, _i);
		case_wr(st, _i);
		case_rr(st, _l);
		case_wr(st, _l);
		case_rrr(stx, _c);
		case_wrr(stx, _c);
		case_rrr(stx, _s);
		case_wrr(stx, _s);
		case_rrr(stx, _i);
		case_wrr(stx, _i);
		case_rrr(stx, _l);
		case_wrr(stx, _l);
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
		case_rrr(add, _f);
		case_rrf(add, _f, 32);
		case_rrr(sub, _f);
		case_rrf(sub, _f, 32);
		case_rrf(rsb, _f, 32);
		case_rrr(mul, _f);
		case_rrf(mul, _f, 32);
		case_rrr(div, _f);
		case_rrf(div, _f, 32);
		case_rr(ext, _f);
		case_rr(abs, _f);
		case_rr(neg, _f);
		case_rr(sqrt, _f);
		case_rr(ld, _f);
		case_rw(ld, _f);
		case_rrr(ldx, _f);
		case_rrw(ldx, _f);
		case_rr(st, _f);
		case_wr(st, _f);
		case_rrr(stx, _f);
		case_wrr(stx, _f);
		case_rr(mov, _f);
	    case jit_code_movi_f:
		assert(node->flag & jit_flag_data);
		movi_f(rn(node->u.w), (jit_float32_t *)node->v.n->u.w);
		break;
		case_rr(ext, _d_f);
		case_rrr(lt, _f);
		case_rrf(lt, _f, 32);
		case_rrr(le, _f);
		case_rrf(le, _f, 32);
		case_rrr(eq, _f);
		case_rrf(eq, _f, 32);
		case_rrr(ge, _f);
		case_rrf(ge, _f, 32);
		case_rrr(gt, _f);
		case_rrf(gt, _f, 32);
		case_rrr(ne, _f);
		case_rrf(ne, _f, 32);
		case_rrr(unlt, _f);
		case_rrf(unlt, _f, 32);
		case_rrr(unle, _f);
		case_rrf(unle, _f, 32);
		case_rrr(uneq, _f);
		case_rrf(uneq, _f, 32);
		case_rrr(unge, _f);
		case_rrf(unge, _f, 32);
		case_rrr(ungt, _f);
		case_rrf(ungt, _f, 32);
		case_rrr(ltgt, _f);
		case_rrf(ltgt, _f, 32);
		case_rrr(ord, _f);
		case_rrf(ord, _f, 32);
		case_rrr(unord, _f);
		case_rrf(unord, _f, 32);
		case_brr(blt, _f);
		case_brf(blt, _f, 32);
		case_brr(ble, _f);
		case_brf(ble, _f, 32);
		case_brr(beq, _f);
		case_brf(beq, _f, 32);
		case_brr(bge, _f);
		case_brf(bge, _f, 32);
		case_brr(bgt, _f);
		case_brf(bgt, _f, 32);
		case_brr(bne, _f);
		case_brf(bne, _f, 32);
		case_brr(bunlt, _f);
		case_brf(bunlt, _f, 32);
		case_brr(bunle, _f);
		case_brf(bunle, _f, 32);
		case_brr(buneq, _f);
		case_brf(buneq, _f, 32);
		case_brr(bunge, _f);
		case_brf(bunge, _f, 32);
		case_brr(bungt, _f);
		case_brf(bungt, _f, 32);
		case_brr(bltgt, _f);
		case_brf(bltgt, _f, 32);
		case_brr(bord, _f);
		case_brf(bord, _f, 32);
		case_brr(bunord, _f);
		case_brf(bunord, _f, 32);
		case_rrr(add, _d);
		case_rrf(add, _d, 64);
		case_rrr(sub, _d);
		case_rrf(sub, _d, 64);
		case_rrf(rsb, _d, 64);
		case_rrr(mul, _d);
		case_rrf(mul, _d, 64);
		case_rrr(div, _d);
		case_rrf(div, _d, 64);
		case_rr(ext, _d);
		case_rr(abs, _d);
		case_rr(neg, _d);
		case_rr(sqrt, _d);
		case_rr(ld, _d);
		case_rw(ld, _d);
		case_rrr(ldx, _d);
		case_rrw(ldx, _d);
		case_rr(st, _d);
		case_wr(st, _d);
		case_rrr(stx, _d);
		case_wrr(stx, _d);
		case_rr(mov, _d);
	    case jit_code_movi_d:
		assert(node->flag & jit_flag_data);
		movi_d(rn(node->u.w), (jit_float64_t *)node->v.n->u.w);
		break;
		case_rr(ext, _f_d);
		case_rrr(lt, _d);
		case_rrf(lt, _d, 64);
		case_rrr(le, _d);
		case_rrf(le, _d, 64);
		case_rrr(eq, _d);
		case_rrf(eq, _d, 64);
		case_rrr(ge, _d);
		case_rrf(ge, _d, 64);
		case_rrr(gt, _d);
		case_rrf(gt, _d, 64);
		case_rrr(ne, _d);
		case_rrf(ne, _d, 64);
		case_rrr(unlt, _d);
		case_rrf(unlt, _d, 64);
		case_rrr(unle, _d);
		case_rrf(unle, _d, 64);
		case_rrr(uneq, _d);
		case_rrf(uneq, _d, 64);
		case_rrr(unge, _d);
		case_rrf(unge, _d, 64);
		case_rrr(ungt, _d);
		case_rrf(ungt, _d, 64);
		case_rrr(ltgt, _d);
		case_rrf(ltgt, _d, 64);
		case_rrr(ord, _d);
		case_rrf(ord, _d, 64);
		case_rrr(unord, _d);
		case_rrf(unord, _d, 64);
		case_brr(blt, _d);
		case_brf(blt, _d, 64);
		case_brr(ble, _d);
		case_brf(ble, _d, 64);
		case_brr(beq, _d);
		case_brf(beq, _d, 64);
		case_brr(bge, _d);
		case_brf(bge, _d, 64);
		case_brr(bgt, _d);
		case_brf(bgt, _d, 64);
		case_brr(bne, _d);
		case_brf(bne, _d, 64);
		case_brr(bunlt, _d);
		case_brf(bunlt, _d, 64);
		case_brr(bunle, _d);
		case_brf(bunle, _d, 64);
		case_brr(buneq, _d);
		case_brf(buneq, _d, 64);
		case_brr(bunge, _d);
		case_brf(bunge, _d, 64);
		case_brr(bungt, _d);
		case_brf(bungt, _d, 64);
		case_brr(bltgt, _d);
		case_brf(bltgt, _d, 64);
		case_brr(bord, _d);
		case_brf(bord, _d, 64);
		case_brr(bunord, _d);
		case_brf(bunord, _d, 64);
	    case jit_code_jmpr:
		jmpr(rn(node->u.w));
		break;
	    case jit_code_jmpi:
		if (node->flag & jit_flag_node) {
		    if (_jit->pc.uc == _jit->code.ptr + 16)
			_jitc->jump = 1;
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
		    word = calli_p(temp->u.w);
		    if (!(temp->flag & jit_flag_patch))
			patch(word, node);
		}
		else
		    calli(node->u.w);
		break;
	    case jit_code_prolog:
		jit_regset_set_ui(&_jitc->regs, 0);
		_jitc->pred = 0;
		sync();
		_jitc->function = _jitc->functions.ptr + node->w.w;
		undo.node = node;
		undo.word = _jit->pc.w;
#if DEVEL_DISASSEMBLER
		undo.prevw = prevw;
#endif
		undo.patch_offset = _jitc->patches.offset;
		undo.prolog_offset = _jitc->prolog.offset;
	    restart_function:
		_jitc->again = 0;
		if (_jitc->jump && !_jitc->function->assume_frame) {
		    /* remember prolog to hide offset adjustment for a jump
		     * to the start of a function, what is expected to be
		     * a common practice as first jit instruction */
		    if (_jitc->prolog.offset >= _jitc->prolog.length) {
			_jitc->prolog.length += 16;
			jit_realloc((jit_pointer_t *)&_jitc->prolog.ptr,
				    (_jitc->prolog.length - 16) *
				    sizeof(jit_word_t),
				    _jitc->prolog.length * sizeof(jit_word_t));
		    }
		    _jitc->prolog.ptr[_jitc->prolog.offset++] = _jit->pc.w;
		    /* function descriptor */
		    word = _jit->pc.w + 16;
		    il(word);			/* addr */
		    il(0);			/* gp */
		}
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
		    _jitc->prolog.offset = undo.prolog_offset;
		    _jitc->ioff = 0;
		    jit_regset_set_ui(&_jitc->regs, 0);
		    _jitc->pred = 0;
		    goto restart_function;
		}
		/* remember label is defined */
		sync();
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		epilog(node);
		jit_regset_set_ui(&_jitc->regs, 0);
		_jitc->pred = 0;
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
	    case jit_code_getarg_i:		case jit_code_getarg_ui:
	    case jit_code_getarg_l:
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
	    case jit_code_retval_ui:		case jit_code_retval_l:
	    case jit_code_retval_f:		case jit_code_retval_d:
	    case jit_code_prepare:
	    case jit_code_finishr:		case jit_code_finishi:
		break;
	    case jit_code_movr_w_f:
		movr_w_f(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movr_f_w:
		movr_f_w(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movr_w_d:
		movr_w_d(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movr_d_w:
		movr_d_w(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movi_f_w:
		movi_f_w(rn(node->u.w), node->v.n->u.p);
		break;
	    case jit_code_movi_d_w:
		movi_d_w(rn(node->u.w), node->v.n->u.p);
		break;
	    default:
		abort();
	}
	if (jit_carry != _NOREG) {
	    switch (node->code) {
		case jit_code_note:
		case jit_code_addcr:		case jit_code_addci:
		case jit_code_addxr:		case jit_code_addxi:
		case jit_code_subcr:		case jit_code_subci:
		case jit_code_subxr:		case jit_code_subxi:
		    break;
		default:
		    jit_unget_reg(jit_carry);
		    jit_carry = _NOREG;
		    break;
	    }
	}
#if GET_JIT_SIZE
	sync();
#endif
	jit_regarg_clr(node, value);
	if (jit_regset_cmp_ui(&_jitc->regarg, 0) != 0) {
	    assert(jit_regset_scan1(&_jitc->regarg, 0) == jit_carry);
	    assert(jit_regset_scan1(&_jitc->regarg, jit_carry + 1) == ULONG_MAX);
	}
	assert(_jitc->synth == 0);
	/* update register live state */
	jit_reglive(node);
    }
    sync();
#undef case_brf
#undef case_brw
#undef case_brr
#undef case_wrr
#undef case_rrrw
#undef case_rrf
#undef case_rrw
#undef case_rrrr
#undef case_rrr
#undef case_wr
#undef case_rw
#undef case_rr

    for (offset = 0; offset < _jitc->patches.offset; offset++) {
	node = _jitc->patches.ptr[offset].node;
	word = node->code == jit_code_movi ? node->v.n->u.w : node->u.n->u.w;
	patch_at(node->code, _jitc->patches.ptr[offset].inst, word);
    }

    jit_flush(_jit->code.ptr, _jit->pc.uc);

    return (_jit->code.ptr);
}

#define CODE				1
#  include "jit_ia64-cpu.c"
#  include "jit_ia64-fpu.c"
#undef CODE

void
jit_flush(void *fptr, void *tptr)
{
#if defined(__GNUC__)
    jit_word_t		f, t, s;

    s = sysconf(_SC_PAGE_SIZE);
    f = (jit_word_t)fptr & -s;
    t = (((jit_word_t)tptr) + s - 1) & -s;
#  if 0
    __clear_cache((void *)f, (void *)t);
#  else
    /* __clear_cache is a noop in (old?) gcc, but cache flush is
     * required on a multi processor Linux system. */
    for (s = f; s < t; s += 32)
	asm volatile("fc %0" :: "r"(s) : "memory");
#  endif
#endif
}

/* Use r2 that is reserved to not require a jit_get_reg call, also note
 * that addil needs a register that first in 2 bits, so, if using a
 * register other than r2 must be less than r8 (or check for a smaller
 * immediate, i.e. i0 >= -8192 && i0 <= 8191) */
void
_emit_ldxi(jit_state_t *_jit, jit_gpr_t r0, jit_gpr_t r1, jit_word_t i0)
{
    assert(i0 >= -2097152 && i0 < 2097151);
    addi(GR_2, rn(r1), i0);
    ldr(rn(r0), GR_2);
}

void
_emit_stxi(jit_state_t *_jit, jit_word_t i0, jit_gpr_t r0, jit_gpr_t r1)
{
    assert(i0 >= -2097152 && i0 < 2097151);
    addi(GR_2, rn(r0), i0);
    str(GR_2, rn(r1));
}

void
_emit_ldxi_d(jit_state_t *_jit, jit_fpr_t r0, jit_gpr_t r1, jit_word_t i0)
{
    assert(i0 >= -2097152 && i0 < 2097151);
    addi(GR_2, rn(r1), i0);
    ldr_d(rn(r0), GR_2);
}

void
_emit_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_gpr_t r0, jit_fpr_t r1)
{
    assert(i0 >= -2097152 && i0 < 2097151);
    addi(GR_2, rn(r0), i0);
    str_d(GR_2, rn(r1));
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
