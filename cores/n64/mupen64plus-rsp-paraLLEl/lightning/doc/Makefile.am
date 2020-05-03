#
# Copyright 2012-2019 Free Software Foundation, Inc.
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

AM_CFLAGS = -I$(top_srcdir)/include -D_GNU_SOURCE

info_TEXINFOS = lightning.texi
MOSTLYCLEANFILES = lightning.tmp

lightning_TEXINFOS = body.texi version.texi

noinst_PROGRAMS = incr printf rpn rfib ifib fact

$(top_builddir)/lib/liblightning.la:
	cd $(top_builddir)/lib; $(MAKE) $(AM_MAKEFLAGS) liblightning.la

incr_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
incr_SOURCES = incr.c

printf_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
printf_SOURCES = printf.c

rpn_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
rpn_SOURCES = rpn.c

rfib_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
rfib_SOURCES = rfib.c

ifib_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
ifib_SOURCES = ifib.c

fact_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
fact_SOURCES = fact.c
