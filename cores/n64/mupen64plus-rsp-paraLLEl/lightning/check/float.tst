
.data	4
ok:
.c	"ok"

.	$($NaN  =		 0.0 / 0.0)
.	$($pInf =		 1.0 / 0.0)
.	$($nInf =		-1.0 / 0.0)
#if __WORDSIZE == 32
#  define x7f			0x7fffffff
#  define x80			0x80000000
#else
#  define x7f			0x7fffffffffffffff
#  define x80			0x8000000000000000
#endif

#if __mips__ || __sparc__ || __hppa__ || __riscv
#  define wnan			x7f
#elif __arm__ || __aarch64__ || __alpha__
#  define wnan			0
#else
#  define wnan			x80
#endif
#if __mips__ || __arm__ || __ppc__ || __sparc__ || __hppa__ || __aarch64__ || __s390__ || __riscv
#  define wpinf			x7f
#elif __alpha__
/* (at least) bug compatible with gcc 4.2.3 -ieee */
#  define wpinf			0
#else
#  define wpinf			x80
#endif
#if __alpha__
/* (at least) bug compatible with gcc 4.2.3 -ieee */
#  define wninf			0
#else
#  define wninf			x80
#endif

/* ensure result is correct and 0 or 1 in the result register */
#define xtcmp(l, t, op, r0, f0, f1, li, ri)		\
	movi##t %f0 li					\
	movi##t %f1 ri					\
	op##r##t %r0 %f0 %f1				\
	bnei T##op##r##t##r0##f0##f1##l %r0 0		\
	calli @abort					\
T##op##r##t##r0##f0##f1##l:				\
	movi##t %f0 li					\
	movi##t %f1 ri					\
	b##op##r##t bT##op##r##t##r0##f0##f1##l %f0 %f1	\
	calli @abort					\
bT##op##r##t##r0##f0##f1##l:				\
	movi##t %f1 li					\
	op##i##t %r0 %f0 ri				\
	bnei T##op##i##t##r0##f0##f1##l %r0 0		\
	calli @abort					\
T##op##i##t##r0##f0##f1##l:				\
	movi##t %f1 li					\
	b##op##i##t bT##op##i##t##r0##f0##f1##l %f0 ri	\
	calli @abort					\
bT##op##i##t##r0##f0##f1##l:				\
	movi##t %f0 li					\
	movi##t %f1 ri					\
	op##r##t %r0 %f0 %f1				\
	beqi F##op##r##t##r0##f0##f1##l %r0 1		\
	calli @abort					\
F##op##r##t##r0##f0##f1##l:				\
	movi##t %f1 li					\
	op##i##t %r0 %f0 ri				\
	beqi F##op##i##t##r0##f0##f1##l %r0 1		\
	calli @abort					\
F##op##i##t##r0##f0##f1##l:
#define tcmp1(l, t, op, r0, li, ri)			\
	xtcmp(l, t, op, r0, f0, f1, li, ri)		\
	xtcmp(l, t, op, r0, f1, f2, li, ri)		\
	xtcmp(l, t, op, r0, f2, f3, li, ri)		\
	xtcmp(l, t, op, r0, f3, f4, li, ri)		\
	xtcmp(l, t, op, r0, f4, f5, li, ri)
#define tcmp0(l, t, op, li, ri)				\
	tcmp1(l, t, op, v0, li, ri)			\
	tcmp1(l, t, op, v1, li, ri)			\
	tcmp1(l, t, op, v2, li, ri)			\
	tcmp1(l, t, op, r0, li, ri)			\
	tcmp1(l, t, op, r1, li, ri)			\
	tcmp1(l, t, op, r2, li, ri)
#if __ia64__
#  define tcmp(l, op, li, ri)				\
	 xtcmp(l, _f, op, r0, f0, f1, li, ri)		\
	 xtcmp(l, _d, op, r0, f0, f1, li, ri)
#else
#  define tcmp(l, op, li, ri)				\
	 tcmp0(l, _f, op, li, ri)			\
	 tcmp0(l, _d, op, li, ri)
#endif

#define xfcmp(l, t, op, r0, f0, f1, li, ri)		\
	movi##t %f0 li					\
	movi##t %f1 ri					\
	op##r##t %r0 %f0 %f1				\
	beqi T##op##r##t##r0##f0##f1##l %r0 0		\
	calli @abort					\
T##op##r##t##r0##f0##f1##l:				\
	movi##t %f1 li					\
	op##i##t %r0 %f0 ri				\
	beqi T##op##i##t##r0##f0##f1##l %r0 0		\
	calli @abort					\
T##op##i##t##r0##f0##f1##l:				\
	movi##t %f0 li					\
	movi##t %f1 ri					\
	op##r##t %r0 %f0 %f1				\
	bnei F##op##r##t##r0##f0##f1##l %r0 1		\
	calli @abort					\
F##op##r##t##r0##f0##f1##l:				\
	movi##t %f1 li					\
	op##i##t %r0 %f0 ri				\
	bnei F##op##i##t##r0##f0##f1##l %r0 1		\
	calli @abort					\
F##op##i##t##r0##f0##f1##l:
#define fcmp1(l, t, op, r0, li, ri)			\
	xfcmp(l, t, op, r0, f0, f1, li, ri)		\
	xfcmp(l, t, op, r0, f1, f2, li, ri)		\
	xfcmp(l, t, op, r0, f2, f3, li, ri)		\
	xfcmp(l, t, op, r0, f3, f4, li, ri)		\
	xfcmp(l, t, op, r0, f4, f5, li, ri)
#define fcmp0(l, t, op, li, ri)				\
	fcmp1(l, t, op, v0, li, ri)			\
	fcmp1(l, t, op, v1, li, ri)			\
	fcmp1(l, t, op, v2, li, ri)			\
	fcmp1(l, t, op, r0, li, ri)			\
	fcmp1(l, t, op, r1, li, ri)			\
	fcmp1(l, t, op, r2, li, ri)
#if __ia64__
#  define fcmp(l, op, li, ri)				\
	xfcmp(l, _f, op, r0, f0, f1, li, ri)		\
	xfcmp(l, _d, op, r0, f0, f1, li, ri)
#else
#  define fcmp(l, op, li, ri)				\
	 fcmp0(l, _f, op, li, ri)			\
	 fcmp0(l, _d, op, li, ri)
#endif

#define xf2w(l, f, r0, f0, iv, fv)			\
	movi##f %f0 fv					\
	truncr##f %r0 %f0				\
	beqi W##f##r0##f0##l %r0 iv			\
	calli @abort					\
W##f##r0##f0##l:
#define f2w1(l, t, r0, iv, fv)				\
	xf2w(l, t, r0, f0, iv, fv)			\
	xf2w(l, t, r0, f1, iv, fv)			\
	xf2w(l, t, r0, f2, iv, fv)			\
	xf2w(l, t, r0, f3, iv, fv)			\
	xf2w(l, t, r0, f4, iv, fv)			\
	xf2w(l, t, r0, f5, iv, fv)
#define f2w0(l, t, iv, fv)				\
	f2w1(l, t, v0, iv, fv)				\
	f2w1(l, t, v1, iv, fv)				\
	f2w1(l, t, v2, iv, fv)				\
	f2w1(l, t, r0, iv, fv)				\
	f2w1(l, t, r1, iv, fv)				\
	f2w1(l, t, r2, iv, fv)
#if __ia64__
#  define f2w(l, iv, fv)				\
	xf2w(l, _f, r0, f0, iv, fv)			\
	xf2w(l, _d, r0, f0, iv, fv)
#else
#  define f2w(l, iv, fv)				\
	f2w0(l, _f, iv, fv)				\
	f2w0(l, _d, iv, fv)
#endif

.code
	prolog

	tcmp(__LINE__, lt, 0, 1)
	tcmp(__LINE__, lt, $nInf, $pInf)
	tcmp(__LINE__, lt, $nInf, 0)
	tcmp(__LINE__, lt, 0, $pInf)
	fcmp(__LINE__, lt, $NaN, 0)
	fcmp(__LINE__, lt, $NaN, $NaN)
	fcmp(__LINE__, lt, $nInf, $NaN)
	fcmp(__LINE__, lt, 1, 0)
	fcmp(__LINE__, lt, 0, 0)
	fcmp(__LINE__, lt, $pInf, $nInf)
	fcmp(__LINE__, lt, 0, $nInf)
	fcmp(__LINE__, lt, 0, $NaN)

	tcmp(__LINE__, le, 0, 1)
	tcmp(__LINE__, le, 0, 0)
	tcmp(__LINE__, le, 1, 1)
	tcmp(__LINE__, le, $nInf, $pInf)
	tcmp(__LINE__, le, $nInf, 0)
	tcmp(__LINE__, le, 0, $pInf)
	fcmp(__LINE__, le, $NaN, 0)
	fcmp(__LINE__, le, $NaN, $NaN)
	fcmp(__LINE__, le, $nInf, $NaN)
	fcmp(__LINE__, le, 1, 0)
	fcmp(__LINE__, le, $pInf, $nInf)
	fcmp(__LINE__, le, 0, $nInf)
	fcmp(__LINE__, le, 0, $NaN)

	tcmp(__LINE__, eq, 0, 0)
	tcmp(__LINE__, eq, 1, 1)
	fcmp(__LINE__, eq, $NaN, 0)
	fcmp(__LINE__, eq, $NaN, $NaN)
	fcmp(__LINE__, eq, $nInf, $NaN)
	fcmp(__LINE__, eq, 0, 1)
	fcmp(__LINE__, eq, 1, 0)
	fcmp(__LINE__, eq, $pInf, $nInf)
	fcmp(__LINE__, eq, 0, $nInf)
	fcmp(__LINE__, eq, 0, $NaN)

	tcmp(__LINE__, ge, 1, 0)
	tcmp(__LINE__, ge, 0, 0)
	tcmp(__LINE__, ge, 1, 1)
	tcmp(__LINE__, ge, $pInf, $nInf)
	tcmp(__LINE__, ge, 0, $nInf)
	tcmp(__LINE__, ge, $pInf, 0)
	fcmp(__LINE__, ge, $NaN, 0)
	fcmp(__LINE__, ge, $NaN, $NaN)
	fcmp(__LINE__, ge, $nInf, $NaN)
	fcmp(__LINE__, ge, 0, 1)
	fcmp(__LINE__, ge, $nInf, $pInf)
	fcmp(__LINE__, ge, $nInf, 0)
	fcmp(__LINE__, ge, 0, $NaN)

	tcmp(__LINE__, gt, 1, 0)
	tcmp(__LINE__, gt, $pInf, $nInf)
	tcmp(__LINE__, gt, 0, $nInf)
	tcmp(__LINE__, gt, $pInf, 0)
	fcmp(__LINE__, gt, $NaN, 0)
	fcmp(__LINE__, gt, $NaN, $NaN)
	fcmp(__LINE__, gt, $nInf, $NaN)
	fcmp(__LINE__, gt, 0, 1)
	fcmp(__LINE__, gt, 0, 0)
	fcmp(__LINE__, gt, $nInf, $pInf)
	fcmp(__LINE__, gt, $nInf, 0)
	fcmp(__LINE__, gt, 0, $NaN)

	tcmp(__LINE__, ne, 0, 1)
	tcmp(__LINE__, ne, 1, 0)
	tcmp(__LINE__, ne, $NaN, $NaN)
	tcmp(__LINE__, ne, $nInf, $pInf)
	tcmp(__LINE__, ne, $NaN, 0)
	tcmp(__LINE__, ne, $nInf, $NaN)
	tcmp(__LINE__, ne, $pInf, $nInf)
	tcmp(__LINE__, ne, 0, $nInf)
	tcmp(__LINE__, ne, 0, $NaN)
	fcmp(__LINE__, ne, 0, 0)
	fcmp(__LINE__, ne, 1, 1)

	tcmp(__LINE__, unlt, 0, 1)
	tcmp(__LINE__, unlt, $nInf, $pInf)
	tcmp(__LINE__, unlt, $nInf, 0)
	tcmp(__LINE__, unlt, 0, $pInf)
	tcmp(__LINE__, unlt, $NaN, 0)
	tcmp(__LINE__, unlt, $NaN, $NaN)
	tcmp(__LINE__, unlt, $nInf, $NaN)
	tcmp(__LINE__, unlt, 0, $NaN)
	fcmp(__LINE__, unlt, 1, 0)
	fcmp(__LINE__, unlt, 0, 0)
	fcmp(__LINE__, unlt, $pInf, $nInf)
	fcmp(__LINE__, unlt, 0, $nInf)

	tcmp(__LINE__, unle, 0, 1)
	tcmp(__LINE__, unle, 0, 0)
	tcmp(__LINE__, unle, 1, 1)
	tcmp(__LINE__, unle, $nInf, $pInf)
	tcmp(__LINE__, unle, $nInf, 0)
	tcmp(__LINE__, unle, 0, $pInf)
	tcmp(__LINE__, unle, $NaN, 0)
	tcmp(__LINE__, unle, $NaN, $NaN)
	tcmp(__LINE__, unle, $nInf, $NaN)
	tcmp(__LINE__, unle, 0, $NaN)
	fcmp(__LINE__, unle, 1, 0)
	fcmp(__LINE__, unle, $pInf, $nInf)
	fcmp(__LINE__, unle, 0, $nInf)

	tcmp(__LINE__, uneq, 0, 0)
	tcmp(__LINE__, uneq, 1, 1)
	tcmp(__LINE__, uneq, $NaN, 0)
	tcmp(__LINE__, uneq, $NaN, $NaN)
	tcmp(__LINE__, uneq, $nInf, $NaN)
	tcmp(__LINE__, uneq, 0, $NaN)
	fcmp(__LINE__, uneq, 0, 1)
	fcmp(__LINE__, uneq, 1, 0)
	fcmp(__LINE__, uneq, $pInf, $nInf)
	fcmp(__LINE__, uneq, 0, $nInf)

	tcmp(__LINE__, unge, 1, 0)
	tcmp(__LINE__, unge, 0, 0)
	tcmp(__LINE__, unge, 1, 1)
	tcmp(__LINE__, unge, $pInf, $nInf)
	tcmp(__LINE__, unge, 0, $nInf)
	tcmp(__LINE__, unge, $pInf, 0)
	tcmp(__LINE__, unge, $NaN, 0)
	tcmp(__LINE__, unge, $NaN, $NaN)
	tcmp(__LINE__, unge, $nInf, $NaN)
	tcmp(__LINE__, unge, 0, $NaN)
	fcmp(__LINE__, unge, 0, 1)
	fcmp(__LINE__, unge, $nInf, $pInf)
	fcmp(__LINE__, unge, $nInf, 0)

	tcmp(__LINE__, ungt, 1, 0)
	tcmp(__LINE__, ungt, $pInf, $nInf)
	tcmp(__LINE__, ungt, 0, $nInf)
	tcmp(__LINE__, ungt, $pInf, 0)
	tcmp(__LINE__, ungt, $NaN, 0)
	tcmp(__LINE__, ungt, $NaN, $NaN)
	tcmp(__LINE__, ungt, $nInf, $NaN)
	tcmp(__LINE__, ungt, 0, $NaN)
	fcmp(__LINE__, ungt, 0, 1)
	fcmp(__LINE__, ungt, 0, 0)
	fcmp(__LINE__, ungt, $nInf, $pInf)
	fcmp(__LINE__, ungt, $nInf, 0)

	tcmp(__LINE__, ltgt, 0, 1)
	tcmp(__LINE__, ltgt, 1, 0)
	tcmp(__LINE__, ltgt, $nInf, $pInf)
	tcmp(__LINE__, ltgt, $pInf, $nInf)
	tcmp(__LINE__, ltgt, 0, $nInf)
	fcmp(__LINE__, ltgt, $NaN, $NaN)
	fcmp(__LINE__, ltgt, $NaN, 0)
	fcmp(__LINE__, ltgt, $nInf, $NaN)
	fcmp(__LINE__, ltgt, 0, $NaN)
	fcmp(__LINE__, ltgt, 0, 0)
	fcmp(__LINE__, ltgt, 1, 1)

	tcmp(__LINE__, ord, 0, 1)
	tcmp(__LINE__, ord, 1, 0)
	tcmp(__LINE__, ord, $nInf, $pInf)
	tcmp(__LINE__, ord, $pInf, $nInf)
	tcmp(__LINE__, ord, 0, $nInf)
	tcmp(__LINE__, ord, 0, 0)
	tcmp(__LINE__, ord, 1, 1)
	fcmp(__LINE__, ord, $NaN, $NaN)
	fcmp(__LINE__, ord, $NaN, 0)
	fcmp(__LINE__, ord, $nInf, $NaN)
	fcmp(__LINE__, ord, 0, $NaN)

	tcmp(__LINE__, unord, $NaN, $NaN)
	tcmp(__LINE__, unord, $NaN, 0)
	tcmp(__LINE__, unord, $nInf, $NaN)
	tcmp(__LINE__, unord, 0, $NaN)
	fcmp(__LINE__, unord, 0, 1)
	fcmp(__LINE__, unord, 1, 0)
	fcmp(__LINE__, unord, $nInf, $pInf)
	fcmp(__LINE__, unord, $pInf, $nInf)
	fcmp(__LINE__, unord, 0, $nInf)
	fcmp(__LINE__, unord, 0, 0)
	fcmp(__LINE__, unord, 1, 1)

	f2w(__LINE__, 0, 0)
	f2w(__LINE__, 1, 1)
        /* not all loongson agree on it */
#if !__mips__
	f2w(__LINE__, wninf, $nInf)
#endif
	f2w(__LINE__, wpinf, $pInf)
	f2w(__LINE__, wnan, $NaN)

	prepare
		pushargi ok
		ellipsis
	finishi @puts

	ret
	epilog
