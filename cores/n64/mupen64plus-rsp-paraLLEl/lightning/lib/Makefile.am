#
# Copyright 2000, 2001, 2002, 2012-2019 Free Software Foundation, Inc.
#
# This file is part of GNU lightning.
#
# GNU lightning is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU lightning is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#

AM_CFLAGS = -I$(top_srcdir)/include -D_GNU_SOURCE $(LIGHTNING_CFLAGS)
liblightning_LTLIBRARIES = liblightning.la
liblightning_la_LDFLAGS = -version-info 1:0:0

if get_jit_size
JIT_SIZE_PATH = "$(top_builddir)/jit_$(cpu)-sz.c"
AM_CPPFLAGS=-DGET_JIT_SIZE=1 -DJIT_SIZE_PATH='$(JIT_SIZE_PATH)'
endif

liblightningdir = $(libdir)
liblightning_la_SOURCES =	\
	jit_disasm.c		\
	jit_memory.c		\
	jit_names.c		\
	jit_note.c		\
	jit_print.c		\
	jit_size.c		\
	lightning.c

EXTRA_DIST =			\
	jit_rewind.c		\
	jit_aarch64.c		\
	jit_aarch64-cpu.c	\
	jit_aarch64-fpu.c	\
	jit_aarch64-sz.c	\
	jit_alpha.c		\
	jit_alpha-cpu.c		\
	jit_alpha-fpu.c		\
	jit_alpha-sz.c		\
	jit_arm.c		\
	jit_arm-cpu.c		\
	jit_arm-swf.c		\
	jit_arm-vfp.c		\
	jit_arm-sz.c		\
	jit_hppa.c		\
	jit_hppa-cpu.c		\
	jit_hppa-fpu.c		\
	jit_hppa-sz.c		\
	jit_ia64.c		\
	jit_ia64-cpu.c		\
	jit_ia64-fpu.c		\
	jit_ia64-sz.c		\
	jit_mips.c		\
	jit_mips-cpu.c		\
	jit_mips-fpu.c		\
	jit_mips-sz.c		\
	jit_ppc.c		\
	jit_ppc-cpu.c		\
	jit_ppc-fpu.c		\
	jit_ppc-sz.c		\
	jit_riscv.c		\
	jit_riscv-cpu.c		\
	jit_riscv-fpu.c		\
	jit_riscv-sz.c		\
	jit_s390.c		\
	jit_s390-cpu.c		\
	jit_s390-fpu.c		\
	jit_s390-sz.c		\
	jit_sparc.c		\
	jit_sparc-cpu.c		\
	jit_sparc-fpu.c		\
	jit_sparc-sz.c		\
	jit_x86.c		\
	jit_x86-cpu.c		\
	jit_x86-sse.c		\
	jit_x86-x87.c		\
	jit_x86-sz.c
