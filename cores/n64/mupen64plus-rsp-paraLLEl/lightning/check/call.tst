#define def_wi(i)			\
	name _w##i			\
_w##i:					\
	prolog				\
	arg $arg##i			\
	getarg##i %r0 $arg##i		\
	retr %r0			\
	epilog
#define def_wf(f)			\
	name _w##f			\
_w##f:					\
	prolog				\
	arg##f $arg##f			\
	getarg##f %f0 $arg##f		\
	truncr##f %r0 %f0		\
	retr %r0			\
	epilog
#define def_fi(f, i)			\
	name f##i			\
f##i:					\
	prolog				\
	arg $arg##i			\
	getarg##i %r0 $arg##i		\
	extr##f %f0 %r0			\
	retr##f %f0			\
	epilog
#define def_f(f)			\
	name f##f			\
f##f:					\
	prolog				\
	arg##f $arg##f			\
	getarg##f %f0 $arg##f		\
	retr##f %f0			\
	epilog
#define def_ff(f, g)			\
	name f##g			\
	name f##g			\
f##g:					\
	prolog				\
	arg##g $arg##g			\
	getarg##g %f0 $arg##g		\
	extr##g##f %f0 %f0		\
	retr##f %f0			\
	epilog

.data	32
fstr:
.c	"forward"
bstr:
.c	"backward"

.code
	jmpi main

	def_wi(_c)
	def_wi(_uc)
	def_wi(_s)
	def_wi(_us)
#if __WORDSIZE == 64
	def_wi(_i)
	def_wi(_ui)
#endif
	def_wf(_f)
	def_wf(_d)
	def_fi(_f, _c)
	def_fi(_f, _uc)
	def_fi(_f, _s)
	def_fi(_f, _us)
	def_fi(_f, _i)
#if __WORDSIZE == 64
	def_fi(_f, _ui)
	def_fi(_f, _l)
#endif
	def_fi(_d, _c)
	def_fi(_d, _uc)
	def_fi(_d, _s)
	def_fi(_d, _us)
	def_fi(_d, _i)
#if __WORDSIZE == 64
	def_fi(_d, _ui)
	def_fi(_d, _l)
#endif
	def_f(_f)
	def_f(_d)
	def_ff(_f, _d)
	def_ff(_d, _f)

	name main
main:
	prolog

#define _call_w(n, i, a, r)		\
	prepare				\
		pushargi a		\
	finishi _w##i			\
	retval %r0			\
	extr##i %r0 %r0			\
	beqi _w##i##_##n %r0 r		\
	calli @abort			\
_w##i##_##n:
#define call_w(n, i, a, r)		_call_w(n, i, a, r)
#define _call_wf(n, f, a, r)		\
	prepare				\
		pushargi##f a		\
	finishi _w##f			\
	retval %r0			\
	extr##f %f0 %r0			\
	beqi##f _w##f##_##n %f0 r	\
	calli @abort			\
_w##f##_##n:
#define call_wf(n, f, a, r)		_call_wf(n, f, a, r)
#define _call_fi(n, f, i, a, r)		\
	prepare				\
		pushargi a		\
	finishi f##i			\
	retval##f %f0			\
	beqi##f f##i##n %f0 r		\
	calli @abort			\
f##i##n:
#define call_fi(n, f, i, a, r)		_call_fi(n, f, i, a, r)
#define _call_f(n, f, a, r)		\
	prepare				\
		pushargi##f a		\
	finishi f##f			\
	retval##f %f0			\
	beqi##f f##f##n %f0 r		\
	calli @abort			\
f##f##n:
#define call_f(n, f, a, r)		_call_f(n, f, a, r)
#define _call_ff(n, f, g, a, r)		\
	prepare				\
		pushargi##g a		\
	finishi f##g			\
	retval##f %f0			\
	beqi##f f##g##n %f0 r		\
	calli @abort			\
f##g##n:
#define call_ff(n, f, g, a, r)		_call_ff(n, f, g, a, r)

#define c7f		0x7f
#define c80		0x80
#define c81		0x81
#define cff		0xff
#define s7f		0x7fff
#define s80		0x8000
#define s81		0x8001
#define i7f		0x7fffffff
#define i80		0x80000000
#define i81		0x80000001
#define iff		0xffffffff
#define l7f		0x7fffffffffffffff
#define l80		0x8000000000000000
#define l81		0x8000000000000001
#define f7f		 127.0
#define f80		-128.0
#define f81		-127.0
#define uf80		 128.0
#define uf81		 127.0
#if __WORDSIZE == 32
#  define wc80		0xffffff80
#  define wc81		0xffffff81
#  define ws80		0xffff8000
#  define ws81		0xffff8001
#else
#  define wc80		0xffffffffffffff80
#  define wc81		0xffffffffffffff81
#  define ws80		0xffffffffffff8000
#  define ws81		0xffffffffffff8001
#  define wi80		0xffffffff80000000
#  define wi81		0xffffffff80000001
#endif

	call_w(__LINE__, _c,  c7f, c7f)
	call_w(__LINE__, _c,  c80, wc80)
	call_w(__LINE__, _c,  c81, wc81)
	call_w(__LINE__, _uc, c7f, c7f)
	call_w(__LINE__, _uc, c80, c80)
	call_w(__LINE__, _uc, c81, c81)
	call_w(__LINE__, _s,  s7f, s7f)
	call_w(__LINE__, _s,  s80, ws80)
	call_w(__LINE__, _s,  s81, ws81)
	call_w(__LINE__, _us, s7f, s7f)
	call_w(__LINE__, _us, s80, s80)
	call_w(__LINE__, _us, s81, s81)
#if __WORDSIZE == 64
	call_w(__LINE__, _i,  i7f, i7f)
	call_w(__LINE__, _i,  i80, wi80)
	call_w(__LINE__, _i,  i81, wi81)
	call_w(__LINE__, _ui, i7f, i7f)
	call_w(__LINE__, _ui, i80, i80)
	call_w(__LINE__, _ui, i81, i81)
#endif
	call_wf(__LINE__, _f, c7f, f7f)
	call_wf(__LINE__, _f, wc80, f80)
	call_wf(__LINE__, _f, wc81, f81)
	call_wf(__LINE__, _d, c7f, f7f)
	call_wf(__LINE__, _d, wc80, f80)
	call_wf(__LINE__, _d, wc81, f81)
	call_fi(__LINE__, _f, _c, c7f, f7f)
	call_fi(__LINE__, _f, _c, c80, f80)
	call_fi(__LINE__, _f, _uc, c7f, f7f)
	call_fi(__LINE__, _f, _uc, c80, uf80)
	call_fi(__LINE__, _f, _s, c7f, f7f)
	call_fi(__LINE__, _f, _s, c80, uf80)
	call_fi(__LINE__, _f, _us, c7f, f7f)
	call_fi(__LINE__, _f, _us, c80, uf80)
	call_fi(__LINE__, _f, _i, c7f, f7f)
	call_fi(__LINE__, _f, _i, c80, uf80)
#if __WORDSIZE == 64
	call_fi(__LINE__, _f, _ui, c7f, f7f)
	call_fi(__LINE__, _f, _ui, c80, uf80)
	call_fi(__LINE__, _f, _l, c7f, f7f)
	call_fi(__LINE__, _f, _l, c80, uf80)
#endif
	call_fi(__LINE__, _d, _c, c7f, f7f)
	call_fi(__LINE__, _d, _c, c80, f80)
	call_fi(__LINE__, _d, _uc, c7f, f7f)
	call_fi(__LINE__, _d, _uc, c80, uf80)
	call_fi(__LINE__, _d, _s, c7f, f7f)
	call_fi(__LINE__, _d, _s, c80, uf80)
	call_fi(__LINE__, _d, _us, c7f, f7f)
	call_fi(__LINE__, _d, _us, c80, uf80)
	call_fi(__LINE__, _d, _i, c7f, f7f)
	call_fi(__LINE__, _d, _i, c80, uf80)
#if __WORDSIZE == 64
	call_fi(__LINE__, _d, _ui, c7f, f7f)
	call_fi(__LINE__, _d, _ui, c80, uf80)
	call_fi(__LINE__, _d, _l, c7f, f7f)
	call_fi(__LINE__, _d, _l, c80, uf80)
#endif
	call_f(__LINE__, _f, f7f, f7f)
	call_f(__LINE__, _d, f7f, f7f)
	call_ff(__LINE__, _f, _d, f80, f80)
	call_ff(__LINE__, _d, _f, f81, f81)

	movi %r0 forward
	callr %r0

	calli iforward

	ret
	epilog

	name backward
backward:
	prolog
	prepare
		pushargi bstr
	finishi @puts
	ret
	epilog

	name forward
forward:
	prolog
	prepare
		pushargi fstr
	finishi @puts
	movi %r0 backward
	callr %r0
	ret
	epilog

	name iforward
iforward:
	prolog
	prepare
		pushargi fstr
	finishi @puts
	calli backward
	ret
	epilog
