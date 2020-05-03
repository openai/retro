.data	16
ok:
.c	"ok\n"

#define us12_i		0x1234
#define us7f_i		0x7ff7
#define us80_i		0x8008
#define usff_i		0xffff
#define ui12_i		0x01234567
#define ui7f_i		0x7f7ff7f7
#define ui80_i		0x80800808
#define uiff_i		0xffffffff
#define ul12_i		0x0123456789abcdef
#define ul7f_i		0x7f7f7f7ff7f7f7f7
#define ul80_i		0x8080808008080808
#define ulff_i		0xffffffffffffffff

#if __WORDSIZE == 32
#  define xus12_i	0xffff1234
#  define xus7f_i	0x10107ff7
#  define xus80_i	0x81188008
#  define xusff_i	0xeaaeffff
#else
#  define xus12_i	0xffffffffffff1234
#  define xus7f_i	0x1010100101017ff7
#  define xus80_i	0x8181811818818008
#  define xusff_i	0xeaeaeaaeaeaeffff
#  define xui12_i	0xffffffff01234567
#  define xui7f_i	0x101001017f7ff7f7
#  define xui80_i	0x8181181880800808
#  define xuiff_i	0xeaeaaeaeffffffff
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#  define us12_o	0x3412
#  define us7f_o	0xf77f
#  define us80_o	0x0880
#  define usff_o	0xffff
#  define ui12_o	0x67452301
#  define ui7f_o	0xf7f77f7f
#  define ui80_o	0x08088080
#  define uiff_o	0xffffffff
#  define ul12_o	0xefcdab8967452301
#  define ul7f_o	0xf7f7f7f77f7f7f7f
#  define ul80_o	0x0808080880808080
#  define ulff_o	0xffffffffffffffff
#else
#  define us12_o	us12_i
#  define us7f_o	us7f_i
#  define us80_o	us80_i
#  define usff_o	usff_i
#  define ui12_o	ui12_i
#  define ui7f_o	ui7f_i
#  define ui80_o	ui80_i
#  define uiff_o	uiff_i
#  define ul12_o	ul12_i
#  define ul7f_o	ul7f_i
#  define ul80_o	ul80_i
#  define ulff_o	ulff_i
#endif

#define HTON4(I, O, T, R0, R1)				\
	movi %R0 I					\
	htonr_##T %R1 %R0				\
	beqi T##R0##R1##I %R1 O				\
	calli @abort					\
T##R0##R1##I:

#define HTON3(T, R0, R1)				\
	HTON4(T##12_i, T##12_o, T, R0, R1)		\
	HTON4(x##T##12_i, T##12_o, T, R0, R1)		\
	HTON4(T##7f_i, T##7f_o, T, R0, R1)		\
	HTON4(x##T##7f_i, T##7f_o, T, R0, R1)		\
	HTON4(T##80_i, T##80_o, T, R0, R1)		\
	HTON4(x##T##80_i, T##80_o, T, R0, R1)		\
	HTON4(T##ff_i, T##ff_o, T, R0, R1)		\
	HTON4(x##T##ff_i, T##ff_o, T, R0, R1)

#define HTON3x(T, R0, R1)				\
	HTON4(T##12_i, T##12_o, T, R0, R1)		\
	HTON4(T##7f_i, T##7f_o, T, R0, R1)		\
	HTON4(T##80_i, T##80_o, T, R0, R1)		\
	HTON4(T##ff_i, T##ff_o, T, R0, R1)

#define HTON2(T, V0, V1, V2, R0, R1, R2)		\
	HTON3(T, V0, V0)				\
	HTON3(T, V0, V1)				\
	HTON3(T, V0, V2)				\
	HTON3(T, V0, R0)				\
	HTON3(T, V0, R1)				\
	HTON3(T, V0, R2)				\

#define HTON2x(T, V0, V1, V2, R0, R1, R2)		\
	HTON3x(T, V0, V0)				\
	HTON3x(T, V0, V1)				\
	HTON3x(T, V0, V2)				\
	HTON3x(T, V0, R0)				\
	HTON3x(T, V0, R1)				\
	HTON3x(T, V0, R2)				\

#define HTON1(T, V0, V1, V2, R0, R1, R2)		\
	HTON2(T, V0, V1, V2, R0, R1, R2)		\
	HTON2(T, V1, V2, R0, R1, R2, V0)		\
	HTON2(T, V2, R0, R1, R2, V0, V1)		\
	HTON2(T, R0, R1, R2, V0, V1, V2)		\
	HTON2(T, R1, R2, V0, V1, V2, R0)		\
	HTON2(T, R2, V0, V1, V2, R0, R1)

#define HTON1x(T, V0, V1, V2, R0, R1, R2)		\
	HTON2x(T, V0, V1, V2, R0, R1, R2)		\
	HTON2x(T, V1, V2, R0, R1, R2, V0)		\
	HTON2x(T, V2, R0, R1, R2, V0, V1)		\
	HTON2x(T, R0, R1, R2, V0, V1, V2)		\
	HTON2x(T, R1, R2, V0, V1, V2, R0)		\
	HTON2x(T, R2, V0, V1, V2, R0, R1)

#if __WORDSIZE == 32
#  define HTON(V0, V1, V2, R0, R1, R2)			\
	HTON1(us, V0, V1, V2, R0, R1, R2)		\
	HTON1x(ui, V0, V1, V2, R0, R1, R2)
#else
#  define HTON(V0, V1, V2, R0, R1, R2)			\
	HTON1(us, V0, V1, V2, R0, R1, R2)		\
	HTON1(ui, V0, V1, V2, R0, R1, R2)		\
	HTON1x(ul, V0, V1, V2, R0, R1, R2)
#endif

.code
	prolog
	/* simple sequence for easier disassembly reading and encoding check */
	movi %r0 us12_i
	htonr_us %r1 %r0
	beqi us %r1 us12_o
	calli @abort
us:

	movi %r0 xus12_i
	htonr_us %r1 %r0
	beqi xus %r1 us12_o
	calli @abort
xus:
	movi %r0 ui12_i
	htonr_ui %r1 %r0
	beqi ui %r1 ui12_o
	calli @abort
ui:
#if __WORDSIZE == 64
	movi %r0 xui12_i
	htonr_ui %r1 %r0
	beqi xui %r1 ui12_o
	calli @abort
xui:
	movi %r0 ul12_i
	htonr_ul %r1 %r0
	beqi ul %r1 ul12_o
	calli @abort
ul:
#endif

	HTON(v0, v1, v2, r0, r1, r2)

	// just to know did not abort
	prepare
		pushargi ok
		ellipsis
	finishi @printf

	ret
	epilog
