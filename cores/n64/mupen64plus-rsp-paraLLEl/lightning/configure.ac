dnl
dnl Copyright 2000, 2001, 2002, 2012-2019 Free Software Foundation, Inc.
dnl
dnl This file is part of GNU lightning.
dnl
dnl GNU lightning is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU Lesser General Public License as published
dnl by the Free Software Foundation; either version 3, or (at your option)
dnl any later version.
dnl 
dnl GNU lightning is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
dnl or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
dnl License for more details.
dnl

AC_PREREQ(2.57)
AC_INIT([GNU lightning], 2.1.3, pcpa@gnu.org, lightning)
AC_CANONICAL_TARGET
AC_CONFIG_SRCDIR([Makefile.am])
AM_INIT_AUTOMAKE([dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR(m4)

AC_CONFIG_HEADERS(config.h)

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

case "$target_cpu" in
    ia64)
	case "$host_os" in
	    # Only supported mode
	    *hpux*)
		LIGHTNING_CFLAGS="$LIGHTNING_CFLAGS -mlp64"		;;
	    *)					;;
	esac					;;
    *mips*)
	case "$host_os" in
	    # (Hack) Flags to pass configure with gcc 3.x
	    # Should not set LIGHTNINT_CFLAGS
	    *irix*)
		CFLAGS="$CFLAGS -D__c99 -Drestrict=";;
	    *)					;;
	esac					;;
    alpha*)
	case "$host_os" in
	    osf*)
		# Get proper varargs and mmap prototypes and definitions
		CFLAGS="$CFLAGS -D_ANSI_C_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_OSF_SOURCE -D_POSIX_C_SOURCE=199309L"
		# Want to generate NaN with 0.0/0.0 and Inf with 1.0/0.0
		if test x$GCC = "xyes"; then
		    CFLAGS="$CFLAGS -mieee"
		else
		    CFLAGS="$CFLAGS -ieee_with_no_inexact"
		fi				;;
	    *)					;;
	esac					;;
    *)						;;
esac

AC_CHECK_FUNCS(mremap ffsl getopt_long_only isnan isinf,,)

AC_CHECK_HEADERS([getopt.h stdint.h],,,)

AC_ARG_ENABLE(disassembler,
	      AS_HELP_STRING([--enable-disassembler],
			     [Enable jit disassembler using binutils]),
	      [DISASSEMBLER=$enableval], [DISASSEMBLER=auto])
if test "x$DISASSEMBLER" != "xno"; then
    # FIXME need to check for libiberty first or will fail to link
    AC_CHECK_LIB(iberty, htab_try_create, ,
		 [HAVE_IBERTY="no"])
    AC_CHECK_LIB(bfd, bfd_init, ,
		 [HAVE_BFD="no"])
    AC_CHECK_LIB(z, compressBound, ,
		 [HAVE_Z="no"])
    AC_CHECK_LIB(opcodes, init_disassemble_info, ,
		 [HAVE_OPCODES="no"])
    if test "x$HAVE_IBERTY"  = "xno" -o \
	    "x$HAVE_BFD"     = "xno" -o \
	    "x$HAVE_Z"       = "xno" -o \
	    "x$HAVE_OPCODES" = "xno"; then
	if test "x$DISASSEMBLER" != "xauto"; then
	    AC_MSG_ERROR([binutils not found, see http://www.gnu.org/software/binutils/])
	else
	    AC_MSG_WARN([binutils not found, see http://www.gnu.org/software/binutils/])
	    DISASSEMBLER="no"
	fi
    fi
fi
AM_CONDITIONAL(with_disassembler, [test "x$DISASSEMBLER" != "xno"])
if test "x$DISASSEMBLER" != "xno"; then
    LIGHTNING_CFLAGS="$LIGHTNING_CFLAGS -DDISASSEMBLER=1"
    save_CFLAGS=$CFLAGS
    CFLAGS="$CFLAGS -I$PWD/include -D_GNU_SOURCE"
    AC_COMPILE_IFELSE([AC_LANG_SOURCE(
	#include <lightning.h>
	#include <lightning/jit_private.h>
	#include <dis-asm.h>
	int main(int argc, char *argv[])
	{
		disassembler_ftype	 print;
		bfd			*abfd;
		print = disassembler(abfd);
		return 0;
	}
    )], [ac_cv_test_new_disassembler=no],,)
    CFLAGS="$save_CFLAGS"
    if test "x$ac_cv_test_new_disassembler" != "xno"; then
	LIGHTNING_CFLAGS="$LIGHTNING_CFLAGS -DBINUTILS_2_29=1"
    fi
fi

AC_ARG_ENABLE(devel-disassembler,
	      AS_HELP_STRING([--enable-devel-disassembler],
			     [Enable extra disassembly options]),
	      [DEVEL_DISASSEMBLER=$enableval], [DEVEL_DISASSEMBLER=no])
if test "x$DEVEL_DISASSEMBLER" != "xno"; then
    if test "x$DISASSEMBLER" = "xno"; then
	AC_MSG_ERROR(devel-disassembler needs disassembler enabled)
    fi
    LIGHTNING_CFLAGS="$LIGHTNING_CFLAGS -DDEVEL_DISASSEMBLER=1"
fi

AC_ARG_ENABLE(assertions,
	      AS_HELP_STRING([--enable-assertions],
			     [Enable runtime code generation assertions]),
	      [DEBUG=$enableval], [DEBUG=auto])
if test "x$DEBUG" = xyes; then
    LIGHTNING_CFLAGS="$LIGHTNING_CFLAGS -DDEBUG=1"
else
    LIGHTNING_CFLAGS="$LIGHTNING_CFLAGS -DNDEBUG"
    DEBUG=no
fi

# This option is only useful during development.
AC_ARG_ENABLE(devel-get-jit-size,
	      AS_HELP_STRING([--enable-devel-get-jit-size],
			     [Devel mode to regenerate jit size information]),
	      [GET_JIT_SIZE=$enableval], [GET_JIT_SIZE=no])
AM_CONDITIONAL(get_jit_size, [test $GET_JIT_SIZE = yes])

case "$host_os" in
    *bsd*|osf*)		SHLIB=""	;;
    *hpux*)		SHLIB="-ldld"	;;
    *)			SHLIB="-ldl"	;;
esac
AC_SUBST(SHLIB)

cpu=
case "$target_cpu" in
    i?86|x86_64|amd64)	cpu=x86		;;
    *arm*)		cpu=arm		;;
    *mips*)		cpu=mips	;;
    *powerpc*)		cpu=ppc		;;
    *sparc*)		cpu=sparc	;;
    ia64)		cpu=ia64	;;
    hppa*)		cpu=hppa	;;
    aarch64)		cpu=aarch64	;;
    s390*)		cpu=s390	;;
    alpha*)		cpu=alpha	;;
    riscv*)		cpu=riscv	;;
    *)					;;
esac
AM_CONDITIONAL(cpu_arm,     [test cpu-$cpu = cpu-arm])
AM_CONDITIONAL(cpu_mips,    [test cpu-$cpu = cpu-mips])
AM_CONDITIONAL(cpu_ppc,     [test cpu-$cpu = cpu-ppc])
AM_CONDITIONAL(cpu_sparc,   [test cpu-$cpu = cpu-sparc])
AM_CONDITIONAL(cpu_x86,     [test cpu-$cpu = cpu-x86])
AM_CONDITIONAL(cpu_ia64,    [test cpu-$cpu = cpu-ia64])
AM_CONDITIONAL(cpu_hppa,    [test cpu-$cpu = cpu-hppa])
AM_CONDITIONAL(cpu_aarch64, [test cpu-$cpu = cpu-aarch64])
AM_CONDITIONAL(cpu_s390,    [test cpu-$cpu = cpu-s390])
AM_CONDITIONAL(cpu_alpha,   [test cpu-$cpu = cpu-alpha])
AM_CONDITIONAL(cpu_riscv,   [test cpu-$cpu = cpu-riscv])

# Test x87 if both, x87 and sse2 available
ac_cv_test_x86_x87=
# Test arm instruction set if thumb instruction set available
ac_cv_test_arm_arm=
# Test sofware float if vfp available and not using hard float abi
ac_cv_test_arm_swf=

save_CFLAGS=$CFLAGS
CFLAGS="$CFLAGS -I$PWD/include -D_GNU_SOURCE"
if test x$cpu = x; then
    AC_MSG_ERROR([cpu $target_cpu not supported])
elif test $cpu = x86; then
    AC_RUN_IFELSE([AC_LANG_SOURCE([[
    #include <lightning.h>
    int main(void) {
	int		    ac, flags;
	unsigned int	    eax, ebx, ecx, edx;
	if (__WORDSIZE == 64)
	    return 1;
	__asm__ volatile ("pushfl;\n\t"
			  "popl %0;\n\t"
			  "movl \$0x240000, %1;\n\t"
			  "xorl %0, %1;\n\t"
			  "pushl %1;\n\t"
			  "popfl;\n\t"
			  "pushfl;\n\t"
			  "popl %1;\n\t"
			  "xorl %0, %1;\n\t"
			  "pushl %0;\n\t"
			  "popfl"
			  : "=r" (flags), "=r" (ac));
	if ((ac & (1 << 21)) == 0)
	    return 1;
	__asm__ volatile ("xchgl %%ebx, %1; cpuid; xchgl %%ebx, %1"
			  : "=a" (eax), "=r" (ebx),
			  "=c" (ecx), "=d" (edx)
			  : "0" (1));
	return (edx & 1 << 26) ? 0 : 1;
    }
    ]])],[ac_cv_test_x86_x87=yes],[],[ac_cv_test_x86_x87=no])
elif test $cpu = arm; then
    AC_RUN_IFELSE([AC_LANG_SOURCE([[
    #include <stdio.h>
    int main(void) {
    #if defined(__linux__)
	FILE	*fp;
	char	 buf[128];
	if ((fp = fopen("/proc/cpuinfo", "r")) == NULL)
	    return 1;
	while (fgets(buf, sizeof(buf), fp)) {
	    if (strncmp(buf, "Features\t:", 10) == 0 &&
		strstr(buf + 10, "thumb")) {
		fclose(fp);
		return 0;
	    }
	}
	fclose(fp);
    #elif defined(__thumb2__)
	return 0;
    #endif
	return 1;
    }
    ]])],[ac_cv_test_arm_arm=yes],[],[ac_cv_test_arm_arm=no])
    AC_RUN_IFELSE([AC_LANG_SOURCE([[
    #include <stdio.h>
    int main(void) {
    #if defined(__linux__)
	FILE	*fp;
	char	 buf[128];
    #  if !defined(__ARM_PCS_VFP)
	if ((fp = fopen("/proc/cpuinfo", "r")) == NULL)
	    return 1;
	while (fgets(buf, sizeof(buf), fp)) {
	    if (strncmp(buf, "Features\t:", 10) == 0 &&
		strstr(buf + 10, "vfp")) {
		fclose(fp);
		return 0;
	    }
	}
	fclose(fp);
    #  endif
    #endif
	return 1;
    }
    ]])],[ac_cv_test_arm_swf=yes],[],[ac_cv_test_arm_swf=no])
elif test $cpu = ppc; then
    if test "x$DISASSEMBLER" != "xno"; then
	save_LIBS="$LIBS"
	LIBS="$LIBS $SHLIB"
	AC_CHECK_FUNCS(disassemble_init_for_target disassemble_init_powerpc)
	LIBS="$save_LIBS"
    fi
fi
CFLAGS=$save_CFLAGS

AM_CONDITIONAL(test_x86_x87, [test x$ac_cv_test_x86_x87 = xyes])
AM_CONDITIONAL(test_arm_arm, [test x$ac_cv_test_arm_arm = xyes])
AM_CONDITIONAL(test_arm_swf, [test x$ac_cv_test_arm_swf = xyes])

AM_CONDITIONAL(test_nodata, [test cpu-$cpu = cpu-mips -o cpu-$cpu = cpu-ppc -o cpu-$cpu = cpu-sparc -o cpu-$cpu = cpu-x86 -o cpu-$cpu = cpu-ia64 -o cpu-$cpu = cpu-hppa -o cpu-$cpu = cpu-s390 -o cpu-$cpu = cpu-alpha])

if test $cpu = arm; then
     AC_CHECK_LIB(m, sqrtf, ,
        [AC_MSG_ERROR([sqrtf required but not available])])
fi
AC_SUBST(cpu)

AC_SUBST([LIGHTNING_CFLAGS])

if test $ac_cv_header_stdint_h = yes; then
    AC_SUBST(MAYBE_INCLUDE_STDINT_H, ["#include <stdint.h>"])
fi

AC_OUTPUT([Makefile
	   lightning.pc
	   doc/Makefile
	   include/Makefile
	   include/lightning/Makefile
	   include/lightning.h
	   lib/Makefile
	   check/Makefile])
