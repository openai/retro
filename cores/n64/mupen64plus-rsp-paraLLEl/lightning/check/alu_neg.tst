#include "alu.inc"

.code
	prolog

#define NEG(N, I, V)			UN(N, neg, I, V)

	NEG(0,	0,			0)
#if __WORDSIZE == 32
	NEG(1,	1,			0xffffffff)
	NEG(2,	0xffffffff,		1)
	NEG(3,	0x80000000,		0x80000000)
	NEG(4,	0x7fffffff,		0x80000001)
	NEG(5,	0x80000001,		0x7fffffff)
#else
	NEG(1,	1,			0xffffffffffffffff)
	NEG(2,	0xffffffff,		0xffffffff00000001)
	NEG(3,	0x80000000,		0xffffffff80000000)
	NEG(4,	0x7fffffff,		0xffffffff80000001)
	NEG(5,	0x80000001,		0xffffffff7fffffff)
	NEG(6,	0xffffffffffffffff,	1)
	NEG(7,	0x8000000000000000,	0x8000000000000000)
	NEG(8,	0x7fffffffffffffff,	0x8000000000000001)
#endif

#undef NEG
#define NEG(N, T, I, V)			FUN(N, T, neg, I, V)
	NEG(0, _f,	0.0,		-0.0)
	NEG(1, _f,	0.5,		-0.5)
	NEG(2, _f,	$(1 / 0.0),	$(-1.0 / 0))
	NEG(3, _f,	-1.25,		 1.25)
	NEG(0, _d,	 0.0,		-0.0)
	NEG(1, _d,	 0.5,		-0.5)
	NEG(2, _d,	 $(1.0 / 0),	$(-1 / 0.0))
	NEG(3, _d,	-1.25,		 1.25)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
