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
#if GET_JIT_SIZE
#  include <stdio.h>
#endif

/*
 * Initialization
 */
static jit_int16_t	_szs[jit_code_last_code] = {
#if GET_JIT_SIZE
#  define JIT_INSTR_MAX		256
#else
#  if defined(__i386__) || defined(__x86_64__)
#    include "jit_x86-sz.c"
#  elif defined(__mips__)
#    include "jit_mips-sz.c"
#  elif defined(__arm__)
#    include "jit_arm-sz.c"
#  elif defined(__powerpc__)
#    include "jit_ppc-sz.c"
#  elif defined(__sparc__)
#    include "jit_sparc-sz.c"
#  elif defined(__ia64__)
#    include "jit_ia64-sz.c"
#  elif defined(__hppa__)
#    include "jit_hppa-sz.c"
#  elif defined(__aarch64__)
#    include "jit_aarch64-sz.c"
#  elif defined(__s390__) || defined(__s390x__)
#    include "jit_s390-sz.c"
#  elif defined(__alpha__)
#    include "jit_alpha-sz.c"
#  elif defined(__riscv)
#    include "jit_riscv-sz.c"
#  endif
#endif
};

/*
 * Implementation
 */
void
jit_init_size(void)
{
#if DEBUG
#  if !GET_JIT_SIZE
    jit_word_t		offset;

    for (offset = 0; offset < jit_size(_szs); offset++)
	if (_szs[offset] != 0)
	    return;
    /* Ensure data was collected */
    abort();
#  endif
#endif
}

#if GET_JIT_SIZE
void
_jit_size_prepare(jit_state_t *_jit)
{
    _jitc->cptr = _jit->code.ptr;
    _jitc->size = _jit->pc.w;
}

void
_jit_size_collect(jit_state_t *_jit, jit_node_t *node)
{
    jit_word_t		length;

    if (_jitc->cptr == _jit->code.ptr) {
	length = _jit->pc.w - _jitc->size;
	if (_szs[node->code] < length)
	    _szs[node->code] = length;
    }
}

#else
jit_word_t
_jit_get_size(jit_state_t *_jit)
{
    jit_word_t		 size;
    jit_node_t		*node;

    for (size = JIT_INSTR_MAX, node = _jitc->head; node; node = node->next)
	size += _szs[node->code];

    return ((size + 4095) & -4096);
}
#endif

jit_word_t
jit_get_max_instr(void)
{
    return (JIT_INSTR_MAX >= 144 ? JIT_INSTR_MAX : 144);
}

void
jit_finish_size(void)
{
#if GET_JIT_SIZE
    FILE		*fp;
    jit_word_t		 offset;

    /* Define a single path */
    fp = fopen(JIT_SIZE_PATH, "a");
    assert(fp);
    for (offset = 0; offset < jit_size(_szs); offset++)
	fprintf(fp, "%d %d\n", offset, _szs[offset]);
    fclose(fp);
#endif
}
