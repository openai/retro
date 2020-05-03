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

check_PROGRAMS = lightning ccall self setcode nodata ctramp carg cva_list

lightning_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
lightning_SOURCES = lightning.c

ccall_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
ccall_SOURCES = ccall.c

self_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
self_SOURCES = self.c

setcode_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
setcode_SOURCES = setcode.c

nodata_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
nodata_SOURCES = nodata.c

ctramp_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
ctramp_SOURCES = ctramp.c

carg_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
carg_SOURCES = carg.c

cva_list_LDADD = $(top_builddir)/lib/liblightning.la -lm $(SHLIB)
cva_list_SOURCES = cva_list.c

$(top_builddir)/lib/liblightning.la:
	cd $(top_builddir)/lib; $(MAKE) $(AM_MAKEFLAGS) liblightning.la

EXTRA_DIST =				\
	3to2.tst	3to2.ok		\
	add.tst		add.ok		\
	align.tst	align.ok	\
	allocai.tst	allocai.ok	\
	allocar.tst	allocar.ok	\
	bp.tst		bp.ok		\
	divi.tst	divi.ok		\
	fib.tst		fib.ok		\
	rpn.tst		rpn.ok		\
	ldst.inc			\
	ldstr.tst	ldstr.ok	\
	ldsti.tst	ldsti.ok	\
	ldstxr.tst	ldstxr.ok	\
	ldstxi.tst	ldstxi.ok	\
	ldstr-c.tst	ldstr-c.ok	\
	ldstxr-c.tst	ldstxr-c.ok	\
	ldstxi-c.tst	ldstxi-c.ok	\
	cvt.tst		cvt.ok		\
	hton.tst	hton.ok		\
	branch.tst	branch.ok	\
	alu.inc				\
	alu_add.tst	alu_add.ok	\
	alux_add.tst	alux_add.ok	\
	alu_sub.tst	alu_sub.ok	\
	alux_sub.tst	alux_sub.ok	\
	alu_rsb.tst	alu_rsb.ok	\
	alu_mul.tst	alu_mul.ok	\
	alu_div.tst	alu_div.ok	\
	alu_rem.tst	alu_rem.ok	\
	alu_and.tst	alu_and.ok	\
	alu_or.tst	alu_or.ok	\
	alu_xor.tst	alu_xor.ok	\
	alu_lsh.tst	alu_lsh.ok	\
	alu_rsh.tst	alu_rsh.ok	\
	alu_com.tst	alu_com.ok	\
	alu_neg.tst	alu_neg.ok	\
	fop_abs.tst	fop_abs.ok	\
	fop_sqrt.tst	fop_sqrt.ok	\
	varargs.tst	varargs.ok	\
	stack.tst	stack.ok	\
	clobber.tst	clobber.ok	\
	carry.tst	carry.ok	\
	call.tst	call.ok		\
	float.tst	float.ok	\
	jmpr.tst	jmpr.ok		\
	put.tst		put.ok		\
	qalu.inc			\
	qalu_mul.tst	qalu_mul.ok	\
	qalu_div.tst	qalu_div.ok	\
	range.tst	range.ok	\
	ranger.tst	ranger.ok	\
	ret.tst		ret.ok		\
	tramp.tst	tramp.ok	\
	va_list.tst	va_list.ok	\
	check.sh			\
	check.x87.sh			\
	check.arm.sh	check.swf.sh	\
	check.arm.swf.sh		\
	check.arm4.swf.sh		\
	check.nodata.sh			\
	check.x87.nodata.sh		\
	run-test	all.tst

base_TESTS =				\
	3to2 add align allocai		\
	allocar bp divi fib rpn		\
	ldstr ldsti			\
	ldstxr ldstxi			\
	ldstr-c ldstxr-c ldstxi-c	\
	cvt hton branch			\
	alu_add alux_add		\
	alu_sub alux_sub alu_rsb	\
	alu_mul alu_div alu_rem		\
	alu_and alu_or alu_xor		\
	alu_lsh alu_rsh			\
	alu_com alu_neg			\
	fop_abs fop_sqrt		\
	varargs stack			\
	clobber carry call		\
	float jmpr put			\
	qalu_mul qalu_div		\
	range ranger ret tramp		\
	va_list

$(base_TESTS):	check.sh
	$(LN_S) $(srcdir)/check.sh $@

TESTS = $(base_TESTS)

if test_x86_x87
#x87_TESTS = $(addsuffix .x87, $(base_TESTS))
x87_TESTS =					\
	3to2.x87 add.x87 allocai.x87		\
	allocar.x87 bp.x87 divi.x87 fib.x87	\
	rpn.x87 ldstr.x87 ldsti.x87		\
	ldstxr.x87 ldstxi.x87			\
	ldstr-c.x87 ldstxr-c.x87 ldstxi-c.x87	\
	cvt.x87 branch.x87			\
	alu_add.x87 alux_add.x87		\
	alu_sub.x87 alux_sub.x87 alu_rsb.x87	\
	alu_mul.x87 alu_div.x87 alu_rem.x87	\
	alu_and.x87 alu_or.x87 alu_xor.x87	\
	alu_lsh.x87 alu_rsh.x87			\
	alu_com.x87 alu_neg.x87			\
	fop_abs.x87 fop_sqrt.x87		\
	varargs.x87 stack.x87			\
	clobber.x87 carry.x87 call.x87		\
	float.x87 jmpr.x87 put.x87		\
	va_list.x87
$(x87_TESTS):	check.x87.sh
	$(LN_S) $(srcdir)/check.x87.sh $@
TESTS += $(x87_TESTS)

#x87_nodata_TESTS = $(addsuffix .x87.nodata, $(base_TESTS))
x87_nodata_TESTS =							\
	3to2.x87.nodata add.x87.nodata allocai.x87.nodata		\
	allocar.x87.nodata bp.x87.nodata divi.x87.nodata fib.x87.nodata	\
	rpn.x87.nodata ldstr.x87.nodata ldsti.x87.nodata		\
	ldstxr.x87.nodata ldstxi.x87.nodata				\
	ldstr-c.x87.nodata ldstxr-c.x87.nodata ldstxi-c.x87.nodata	\
	cvt.x87.nodata branch.x87.nodata				\
	alu_add.x87.nodata alux_add.x87.nodata				\
	alu_sub.x87.nodata alux_sub.x87.nodata alu_rsb.x87.nodata	\
	alu_mul.x87.nodata alu_div.x87.nodata alu_rem.x87.nodata	\
	alu_and.x87.nodata alu_or.x87.nodata alu_xor.x87.nodata		\
	alu_lsh.x87.nodata alu_rsh.x87.nodata				\
	alu_com.x87.nodata alu_neg.x87.nodata				\
	fop_abs.x87.nodata fop_sqrt.x87.nodata				\
	varargs.x87.nodata stack.x87.nodata				\
	clobber.x87.nodata carry.x87.nodata call.x87.nodata		\
	float.x87.nodata jmpr.x87.nodata put.x87.nodata			\
	va_list.x87.nodata
$(x87_nodata_TESTS):	check.x87.nodata.sh
	$(LN_S) $(srcdir)/check.x87.nodata.sh $@
TESTS += $(x87_nodata_TESTS)
endif

if test_arm_arm
#arm_TESTS = $(addsuffix .arm, $(base_TESTS))
arm_TESTS =					\
	3to2.arm add.arm align.arm allocai.arm	\
	allocar.arm bp.arm divi.arm fib.arm	\
	rpn.arm ldstr.arm ldsti.arm		\
	ldstxr.arm ldstxi.arm			\
	ldstr-c.arm ldstxr-c.arm ldstxi-c.arm	\
	cvt.arm hton.arm branch.arm		\
	alu_add.arm alux_add.arm		\
	alu_sub.arm alux_sub.arm alu_rsb.arm	\
	alu_mul.arm alu_div.arm alu_rem.arm	\
	alu_and.arm alu_or.arm alu_xor.arm	\
	alu_lsh.arm alu_rsh.arm			\
	alu_com.arm alu_neg.arm			\
	fop_abs.arm fop_sqrt.arm		\
	varargs.arm stack.arm			\
	clobber.arm carry.arm call.arm		\
	float.arm jmpr.arm tramp.arm range.arm	\
	ranger.arm put.arm va_list.arm
$(arm_TESTS):	check.arm.sh
	$(LN_S) $(srcdir)/check.arm.sh $@
TESTS += $(arm_TESTS)
endif

if test_arm_swf
#swf_TESTS = $(addsuffix .swf, $(base_TESTS))
swf_TESTS =					\
	3to2.swf add.swf allocai.swf		\
	allocar.swf bp.swf divi.swf fib.swf	\
	rpn.swf ldstr.swf ldsti.swf		\
	ldstxr.swf ldstxi.swf			\
	ldstr-c.swf ldstxr-c.swf ldstxi-c.swf	\
	cvt.swf hton.swf branch.swf		\
	alu_add.swf alux_add.swf		\
	alu_sub.swf alux_sub.swf alu_rsb.swf	\
	alu_mul.swf alu_div.swf alu_rem.swf	\
	alu_and.swf alu_or.swf alu_xor.swf	\
	alu_lsh.swf alu_rsh.swf			\
	alu_com.swf alu_neg.swf			\
	fop_abs.swf fop_sqrt.swf		\
	varargs.swf stack.swf			\
	clobber.swf carry.swf call.swf		\
	float.swf jmpr.swf tramp.swf range.swf	\
	ranger.swf put.swf va_list.swf
$(swf_TESTS):	check.swf.sh
	$(LN_S) $(srcdir)/check.swf.sh $@
TESTS += $(swf_TESTS)
if test_arm_arm
#arm_swf_TESTS = $(addsuffix .arm.swf, $(base_TESTS))
arm_swf_TESTS =							\
	3to2.arm.swf add.arm.swf allocai.arm.swf		\
	allocar.arm.swf bp.arm.swf divi.arm.swf fib.arm.swf	\
	rpn.arm.swf ldstr.arm.swf ldsti.arm.swf			\
	ldstxr.arm.swf ldstxi.arm.swf				\
	ldstr-c.arm.swf ldstxr-c.arm.swf ldstxi-c.arm.swf	\
	cvt.arm.swf hton.arm.swf branch.arm.swf			\
	alu_add.arm.swf alux_add.arm.swf			\
	alu_sub.arm.swf alux_sub.arm.swf alu_rsb.arm.swf	\
	alu_mul.arm.swf alu_div.arm.swf alu_rem.arm.swf		\
	alu_and.arm.swf alu_or.arm.swf alu_xor.arm.swf		\
	alu_lsh.arm.swf alu_rsh.arm.swf				\
	alu_com.arm.swf alu_neg.arm.swf				\
	fop_abs.arm.swf fop_sqrt.arm.swf			\
	varargs.arm.swf stack.arm.swf				\
	clobber.arm.swf carry.arm.swf call.arm.swf		\
	float.arm.swf jmpr.arm.swf tramp.arm.swf range.arm.swf	\
	ranger.arm.swf put.arm.swf va_list.arm.swf
$(arm_swf_TESTS):	check.arm.swf.sh
	$(LN_S) $(srcdir)/check.arm.swf.sh $@
TESTS += $(arm_swf_TESTS)
endif
if test_arm_arm
#arm4_swf_TESTS = $(addsuffix .arm4.swf, $(base_TESTS))
arm4_swf_TESTS =						\
	3to2.arm4.swf add.arm4.swf allocai.arm4.swf		\
	allocar.arm4.swf bp.arm4.swf divi.arm4.swf fib.arm4.swf	\
	rpn.arm4.swf ldstr.arm4.swf ldsti.arm4.swf		\
	ldstxr.arm4.swf ldstxi.arm4.swf				\
	ldstr-c.arm4.swf ldstxr-c.arm4.swf ldstxi-c.arm4.swf	\
	cvt.arm4.swf hton.arm4.swf branch.arm4.swf		\
	alu_add.arm4.swf alux_add.arm4.swf			\
	alu_sub.arm4.swf alux_sub.arm4.swf alu_rsb.arm4.swf	\
	alu_mul.arm4.swf alu_div.arm4.swf alu_rem.arm4.swf	\
	alu_and.arm4.swf alu_or.arm4.swf alu_xor.arm4.swf	\
	alu_lsh.arm4.swf alu_rsh.arm4.swf			\
	alu_com.arm4.swf alu_neg.arm4.swf			\
	fop_abs.arm4.swf fop_sqrt.arm4.swf			\
	varargs.arm4.swf stack.arm4.swf				\
	clobber.arm4.swf carry.arm4.swf call.arm4.swf		\
	float.arm4.swf jmpr.arm4.swf tramp.arm4.swf		\
	range.arm4.swf ranger.arm4.swf put.arm4.swf		\
	va_list.arm4.swf
$(arm4_swf_TESTS):	check.arm4.swf.sh
	$(LN_S) $(srcdir)/check.arm4.swf.sh $@
TESTS += $(arm4_swf_TESTS)
endif
endif

if test_nodata
#nodata_TESTS = $(addsuffix .nodata, $(base_TESTS))
nodata_TESTS =						\
	3to2.nodata add.nodata allocai.nodata		\
	allocar.nodata bp.nodata divi.nodata fib.nodata	\
	rpn.nodata ldstr.nodata ldsti.nodata		\
	ldstxr.nodata ldstxi.nodata			\
	ldstr-c.nodata ldstxr-c.nodata ldstxi-c.nodata	\
	cvt.nodata branch.nodata			\
	alu_add.nodata alux_add.nodata			\
	alu_sub.nodata alux_sub.nodata alu_rsb.nodata	\
	alu_mul.nodata alu_div.nodata alu_rem.nodata	\
	alu_and.nodata alu_or.nodata alu_xor.nodata	\
	alu_lsh.nodata alu_rsh.nodata			\
	alu_com.nodata alu_neg.nodata			\
	fop_abs.nodata fop_sqrt.nodata			\
	varargs.nodata stack.nodata			\
	clobber.nodata carry.nodata call.nodata		\
	float.nodata jmpr.nodata tramp.nodata		\
	range.nodata ranger.nodata put.nodata		\
	va_list.nodata
$(nodata_TESTS):	check.nodata.sh
	$(LN_S) $(srcdir)/check.nodata.sh $@
TESTS += $(nodata_TESTS)
endif

TESTS += ccall self setcode nodata ctramp carg cva_list
CLEANFILES = $(TESTS)

#TESTS_ENVIRONMENT=$(srcdir)/run-test;

debug:		lightning
	$(LIBTOOL) --mode=execute gdb lightning

