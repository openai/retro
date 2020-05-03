/*
 * Copyright (C) 2015-2019  Free Software Foundation, Inc.
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

#if PROTO
#  define free_synth_list(node)		_free_synth_list(_jit,node)
static jit_node_t *_free_synth_list(jit_state_t*,jit_node_t*);
#define rewind_prolog()			_rewind_prolog(_jit)
static void _rewind_prolog(jit_state_t*);
#define rewind_prepare()		_rewind_prepare(_jit)
static void _rewind_prepare(jit_state_t*);
#endif

#if CODE
/*
 * Implementation
 */
static jit_node_t *
_free_synth_list(jit_state_t *_jit, jit_node_t *node)
{
    jit_node_t		*next;
    next = node->next;
    free_node(node);
    for (node = next; node && (node->flag & jit_flag_synth); node = next) {
	next = node->next;
	free_node(node);
    }
    return (next);
}

static void
_rewind_prolog(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_node_t		*next;
    _jitc->function->self.size = stack_framesize;
#if __arm__
    assert(jit_cpu.abi);
    _jitc->function->self.size += 64;
#endif
#if __mips__ && NEW_ABI
    /* Only add extra stack space if there are varargs
     * arguments in registers. */
    assert(jit_arg_reg_p(_jitc->function->self.argi));
    _jitc->function->self.size += 64;
#endif
    _jitc->function->self.argi =
	_jitc->function->self.argf = _jitc->function->self.argn = 0;
    _jitc->tail = _jitc->function->prolog;
    node = _jitc->tail->next;
    _jitc->tail->next = (jit_node_t *)0;
    _jitc->tail->link = (jit_node_t *)0;
    for (; node; node = next) {
	next = node->next;
	switch (node->code) {
	    case jit_code_arg:
		node->next = (jit_node_t *)0;
		jit_make_arg(node);
		break;
	    case jit_code_arg_f:
		node->next = (jit_node_t *)0;
		jit_make_arg_f(node);
		break;
	    case jit_code_arg_d:
		node->next = (jit_node_t *)0;
		jit_make_arg_d(node);
		break;
	    case jit_code_getarg_c:
		jit_getarg_c(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_getarg_uc:
		jit_getarg_uc(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_getarg_s:
		jit_getarg_s(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_getarg_us:
		jit_getarg_us(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_getarg_i:
		jit_getarg_i(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_getarg_f:
		jit_getarg_f(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_getarg_d:
		jit_getarg_d(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_putargr:
		jit_putargr(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_putargi:
		jit_putargi(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_putargr_f:
		jit_putargr_f(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_putargi_f:
		jit_putargi_f(node->u.f, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_putargr_d:
		jit_putargr_d(node->u.w, node->v.n);
		next = free_synth_list(node);
		break;
	    case jit_code_putargi_d:
		jit_putargi_d(node->u.d, node->v.n);
		next = free_synth_list(node);
		break;
	    default:
		node->next = (jit_node_t *)0;
		link_node(node);
		break;
	}
    }
}

static void
_rewind_prepare(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_node_t		*next;
    _jitc->function->call.argi =
	_jitc->function->call.argf =
	_jitc->function->call.size = 0;
    _jitc->tail = _jitc->prepare;
    node = _jitc->tail->next;
    _jitc->tail->next = (jit_node_t *)0;
    _jitc->tail->link = (jit_node_t *)0;
    for (; node; node = next) {
	next = node->next;
	switch (node->code) {
	    case jit_code_pushargr:
		jit_pushargr(node->u.w);
		next = free_synth_list(node);
		break;
	    case jit_code_pushargi:
		jit_pushargi(node->u.w);
		next = free_synth_list(node);
		break;
	    case jit_code_pushargr_f:
		jit_pushargr_f(node->u.w);
		next = free_synth_list(node);
		break;
	    case jit_code_pushargi_f:
		jit_pushargi_f(node->u.f);
		next = free_synth_list(node);
		break;
	    case jit_code_pushargr_d:
		jit_pushargr_d(node->u.w);
		next = free_synth_list(node);
		break;
	    case jit_code_pushargi_d:
		jit_pushargi_d(node->u.d);
		next = free_synth_list(node);
		break;
	    default:
		node->next = (jit_node_t *)0;
		link_node(node);
		break;
	}
    }
}
#endif
