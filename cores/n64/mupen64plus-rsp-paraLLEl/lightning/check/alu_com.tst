#include "alu.inc"

.code
	prolog

#define COM(N, I0, V)		UN(N, com, I0, V)

#if __WORDSIZE == 32
	COM(0,	0,			0xffffffff)
	COM(1,	1,			0xfffffffe)
	COM(2,	0xffffffff,		0)
	COM(3,	0x80000000,		0x7fffffff)
	COM(4,	0x7fffffff,		0x80000000)
	COM(5,	0x80000001,		0x7ffffffe)
#else
	COM(0,	0,			0xffffffffffffffff)
	COM(1,	1,			0xfffffffffffffffe)
	COM(2,	0xffffffff,		0xffffffff00000000)
	COM(3,	0x80000000,		0xffffffff7fffffff)
	COM(4,	0x7fffffff,		0xffffffff80000000)
	COM(5,	0x80000001,		0xffffffff7ffffffe)
	COM(6,	0xffffffffffffffff,	0)
	COM(7,	0x8000000000000000,	0x7fffffffffffffff)
	COM(8,	0x7fffffffffffffff,	0x8000000000000000)
	COM(9,	0x8000000000000001,	0x7ffffffffffffffe)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
