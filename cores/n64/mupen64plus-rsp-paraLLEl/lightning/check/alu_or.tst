#include "alu.inc"

.code
	prolog

#define OR(N, I0, I1, V)	ALU(N, , or, I0, I1, V)

	OR(0,	0x7fffffff, 1,		0x7fffffff)
	OR(1,	1,	    0x7fffffff, 0x7fffffff)
	OR(2,	0x80000000, 1,		0x80000001)
	OR(3,	1,	    0x80000000, 0x80000001)
	OR(4,	0x7fffffff, 0x80000000, 0xffffffff)
	OR(5,	0x80000000, 0x7fffffff, 0xffffffff)
	OR(6,	0x7fffffff, 0xffffffff, 0xffffffff)
	OR(7,	0xffffffff, 0x7fffffff, 0xffffffff)
	OR(8,	0xffffffff, 0xffffffff, 0xffffffff)
	OR(9,	0x7fffffff, 0,		0x7fffffff)
	OR(10,	0,	    0x7fffffff,	0x7fffffff)
#if __WORDSIZE == 64
	OR(11,	0x7fffffffffffffff, 1,			0x7fffffffffffffff)
	OR(12,	1,		    0x7fffffffffffffff,	0x7fffffffffffffff)
	OR(13,	0x8000000000000000, 1,			0x8000000000000001)
	OR(14,	1,		    0x8000000000000000,	0x8000000000000001)
	OR(15,	0x7fffffffffffffff, 0x8000000000000000,	0xffffffffffffffff)
	OR(16,	0x8000000000000000, 0x7fffffffffffffff,	0xffffffffffffffff)
	OR(17,	0x7fffffffffffffff, 0xffffffffffffffff,	0xffffffffffffffff)
	OR(18,	0xffffffffffffffff, 0x7fffffffffffffff,	0xffffffffffffffff)
	OR(19,	0xffffffffffffffff, 0xffffffffffffffff,	0xffffffffffffffff)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
