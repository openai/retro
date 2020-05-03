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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS =		\
	check		\
	doc		\
	include		\
	lib

pkgconfiglibdir = $(libdir)/pkgconfig
pkgconfiglib_DATA = lightning.pc

if get_jit_size
JIT_SIZE_PATH = "$(top_builddir)/jit_$(cpu)-sz.c"
AM_CPPFLAGS=-DGET_JIT_SIZE=1 -DJIT_SIZE_PATH='$(JIT_SIZE_PATH)'
AM_CFLAGS = -I$(top_srcdir)/include -D_GNU_SOURCE $(LIGHTNING_CFLAGS)

noinst_PROGRAMS = size
size_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
size_SOURCES = size.c

get_jit_size::	$(JIT_SIZE_PATH)

$(JIT_SIZE_PATH):
	make clean
	make check
	$(top_builddir)/size

CLEANFILES = $(JIT_SIZE_PATH)
endif
