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

lightning_includedir = $(includedir)/lightning

EXTRA_DIST =		\
	jit_private.h

if cpu_arm
lightning_include_HEADERS = 	\
	jit_arm.h
endif
if cpu_mips
lightning_include_HEADERS = 	\
	jit_mips.h
endif
if cpu_ppc
lightning_include_HEADERS = 	\
	jit_ppc.h
endif
if cpu_sparc
lightning_include_HEADERS = 	\
	jit_sparc.h
endif
if cpu_x86
lightning_include_HEADERS = 	\
	jit_x86.h
endif
if cpu_ia64
lightning_include_HEADERS = 	\
	jit_ia64.h
endif
if cpu_hppa
lightning_include_HEADERS = 	\
	jit_hppa.h
endif
if cpu_aarch64
lightning_include_HEADERS = 	\
	jit_aarch64.h
endif
if cpu_s390
lightning_include_HEADERS = 	\
	jit_s390.h
endif
if cpu_alpha
lightning_include_HEADERS = 	\
	jit_alpha.h
endif
if cpu_riscv
lightning_include_HEADERS = 	\
	jit_riscv.h
endif
