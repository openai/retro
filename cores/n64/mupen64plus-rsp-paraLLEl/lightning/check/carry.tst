
#define ix0		0
#define lx0		0
#define ix1		1
#define lx1		1
#define ix2		2
#define lx2		2
#define ix4		4
#define lx4		4
#if __WORDSIZE == 32
#  define ix7fe		0x7ffffffe
#  define ix7f		0x7fffffff
#  define ix80		0x80000000
#  define iff		0xffffffff
#  define ife		0xfffffffe
#  define ifd		0xfffffffd
#  define ifc		0xfffffffc
#else
#  define ix7fe		0x7ffffffffffffffe
#  define ix7f		0x7fffffffffffffff
#  define ix80		0x8000000000000000
#  define iff		0xffffffffffffffff
#  define ife		0xfffffffffffffffe
#  define ifd		0xfffffffffffffffd
#  define ifc		0xfffffffffffffffc
#endif

/* check jumps are taken and result value is correct */
#define bopr_t(l, u, op, r0, r1, il, ir, iv)			\
	movi %r0 il						\
	movi %r1 ir						\
	b##op##r##u op##u##r##l##r0##r1 %r0 %r1			\
	/* validate did jump */					\
	movi %r0 0x5a5a5a5a					\
op##u##r##l##r0##r1:						\
	beqi op##u##r##l##ok##r0##r1 %r0 iv			\
	calli @abort						\
op##u##r##l##ok##r0##r1:
#define bopi_t(l, u, op, r0, il, ir, iv)			\
	movi %r0 il						\
	b##op##i##u op##u##i##l##r0##r1 %r0 ir			\
	/* validate did jump */					\
	movi %r0 0x5a5a5a5a					\
op##u##i##l##r0##r1:						\
	beqi op##u##i##l##ok##r0##r1 %r0 iv			\
	calli @abort						\
op##u##i##l##ok##r0##r1:
#define bopr_f(l, u, op, r0, r1, il, ir, iv)			\
	movi %r0 il						\
	movi %r1 ir						\
	b##op##r##u op##u##r##l##r0##r1 %r0 %r1			\
	beqi op##u##r##l##ok##r0##r1 %r0 iv			\
op##u##r##l##r0##r1:						\
	calli @abort						\
op##u##r##l##ok##r0##r1:
#define bopi_f(l, u, op, r0, il, ir, iv)			\
	movi %r0 il						\
	b##op##i##u op##u##i##l##r0##r1 %r0 ir			\
	beqi op##u##i##l##ok##r0##r1 %r0 iv			\
op##u##i##l##r0##r1:						\
	calli @abort						\
op##u##i##l##ok##r0##r1:
#define ccop(cc, l, u, op, r0, r1, il, ir, iv)			\
	bopr##cc(l, u, op, r0, r1, i##il, i##ir, i##iv)		\
	bopi##cc(l, u, op, r0, i##il, i##ir, i##iv)
#define tadd(l, u, r0, r1, il, ir, iv)				\
	ccop(_t, l, u, oadd, r0, r1, il, ir, iv)		\
	ccop(_f, l, u, xadd, r0, r1, il, ir, iv)
#define fadd(l, u, r0, r1, il, ir, iv)				\
	ccop(_f, l, u, oadd, r0, r1, il, ir, iv)		\
	ccop(_t, l, u, xadd, r0, r1, il, ir, iv)
#define tsub(l, u, r0, r1, il, ir, iv)				\
	ccop(_t, l, u, osub, r0, r1, il, ir, iv)		\
	ccop(_f, l, u, xsub, r0, r1, il, ir, iv)
#define fsub(l, u, r0, r1, il, ir, iv)				\
	ccop(_f, l, u, osub, r0, r1, il, ir, iv)		\
	ccop(_t, l, u, xsub, r0, r1, il, ir, iv)

#define xopr6(l,op,r0,r1,r2,r3,r4,r5,llo,lhi,rlo,rhi,vlo,vhi)	\
	movi %r1 llo						\
	movi %r2 lhi						\
	movi %r4 rlo						\
	movi %r5 rhi						\
	op##cr %r0 %r1 %r4					\
	op##xr %r3 %r2 %r5					\
	beqi op##l##L##r0##r1##r2##r3##r4##r5 %r0 vlo		\
	calli @abort						\
op##l##L##r0##r1##r2##r3##r4##r5:				\
	beqi op##l##H##r0##r1##r2##r3##r4##r5 %r3 vhi		\
	calli @abort						\
op##l##H##r0##r1##r2##r3##r4##r5:
#define xopr4_(l,op,r0,r1,r2,r3,llo,lhi,rlo,rhi,vlo,vhi)	\
	movi %r0 llo						\
	movi %r1 lhi						\
	movi %r2 rlo						\
	movi %r3 rhi						\
	op##cr %r0 %r0 %r2					\
	op##xr %r1 %r1 %r3					\
	beqi op##l##L_##r0##r1##r2##r3 %r0 vlo			\
	calli @abort						\
op##l##L_##r0##r1##r2##r3:					\
	beqi op##l##H_##r0##r1##r2##r3 %r1 vhi			\
	calli @abort						\
op##l##H_##r0##r1##r2##r3:
#define xopr_4(l,op,r0,r1,r2,r3,llo,lhi,rlo,rhi,vlo,vhi)	\
	movi %r0 rlo						\
	movi %r1 rhi						\
	movi %r2 llo						\
	movi %r3 lhi						\
	op##cr %r0 %r2 %r0					\
	op##xr %r1 %r3 %r1					\
	beqi op##l##_L##r0##r1##r2##r3 %r0 vlo			\
	calli @abort						\
op##l##_L##r0##r1##r2##r3:					\
	beqi op##l##_H##r0##r1##r2##r3 %r1 vhi			\
	calli @abort						\
op##l##_H##r0##r1##r2##r3:

#define xaddr(l,llo,lhi,rlo,rhi,vlo,vhi)						\
	xopr6(l,add,r0,r1,r2,v0,v1,v2,i##llo,i##lhi,i##rlo,i##rhi,i##vlo,i##vhi)	\
	xopr4_(l,add,r0,r1,r2,v0,i##llo,i##lhi,i##rlo,i##rhi,i##vlo,i##vhi)		\
	xopr_4(l,add,r0,r1,r2,v0,i##llo,i##lhi,i##rlo,i##rhi,i##vlo,i##vhi)
#define xsubr(l,llo,lhi,rlo,rhi,vlo,vhi)						\
	xopr6(l,sub,r0,r1,r2,v0,v1,v2,i##llo,i##lhi,i##rlo,i##rhi,i##vlo,i##vhi)	\
	xopr4_(l,sub,r0,r1,r2,v0,i##llo,i##lhi,i##rlo,i##rhi,i##vlo,i##vhi)		\
	xopr_4(l,sub,r0,r1,r2,v0,i##llo,i##lhi,i##rlo,i##rhi,i##vlo,i##vhi)

.data	16
ok:
.c	"ok\n"

.code
	prolog

	tadd(__LINE__,   , r0, r1, x7f,  x1, x80)
	fadd(__LINE__,   , r0, r1, x7fe, x1, x7f)
	tsub(__LINE__,   , r0, r1, x80,  x1, x7f)
	fsub(__LINE__,   , r0, r1, x7f,  x1, x7fe)
	tadd(__LINE__, _u, r0, r1, ff,   x1, x0)
	fadd(__LINE__, _u, r0, r1, x7f,  x1, x80)
	tsub(__LINE__, _u, r0, r1, x0,   x1, ff)
	fsub(__LINE__, _u, r0, r1, x80,  x1, x7f)

	/* 0xffffffffffffffff + 1 = 0x10000000000000000 */
	xaddr(__LINE__, ff, ff, x1, x0, x0, x0)

	/* 1 + 0xffffffffffffffff = 0x10000000000000000 */
	xaddr(__LINE__, x1, x0, ff, ff, x0, x0)

	/* 0xfffffffeffffffff + 1 = 0xffffffff00000000 */
	xaddr(__LINE__, ff, fe, x1, x0, x0, ff)

	/* 1 + 0xfffffffeffffffff = 0xffffffff00000000 */
	xaddr(__LINE__, x1, x0, ff, fe, x0, ff)

	/* 0xfffffffefffffffe + 2 = 0xffffffff00000000 */
	xaddr(__LINE__, fe, fe, x2, x0, x0, ff)

	/* 2 + 0xfffffffefffffffe = 0xffffffff00000000 */
	xaddr(__LINE__, x2, x0, fe, fe, x0, ff)

	/* 0xffffffffffffffff - 1 = 0xfffffffffffffffe */
	xsubr(__LINE__, ff, ff, x1, x0, fe, ff)

	/* 1 - 0xffffffffffffffff = -0xfffffffffffffffe */
	xsubr(__LINE__, x1, x0, ff, ff, x2, x0)

	/* 0xfffffffeffffffff - 1 = 0xfffffffefffffffe */
	xsubr(__LINE__, ff, fe, x1, x0, fe, fe)

	/* 1 - 0xfffffffeffffffff = -0xfffffffefffffffe */
	xsubr(__LINE__, x1, x0, ff, fe, x2, x1)

	/* 0xfffffffefffffffe - 2 = 0xfffffffefffffffc */
	xsubr(__LINE__, fe, fe, x2, x0, fc, fe)

	/* 2 + 0xfffffffefffffffe = -0xfffffffefffffffc */
	xsubr(__LINE__, x2, x0, fe, fe, x4, x1)

	prepare
		pushargi ok
		ellipsis
	finishi @printf

	ret
	epilog
