#include "alu.inc"

.code
	prolog

#define AND(N, I0, I1, V)	ALU(N, , and, I0, I1, V)

	AND(0,	0x7fffffff, 1,		1)
	AND(1,	1,	    0x7fffffff, 1)
	AND(2,	0x80000000, 1,		0)
	AND(3,	1,	    0x80000000, 0)
	AND(4,	0x7fffffff, 0x80000000, 0)
	AND(5,	0x80000000, 0x7fffffff, 0)
	AND(6,	0x7fffffff, 0xffffffff, 0x7fffffff)
	AND(7,	0xffffffff, 0x7fffffff, 0x7fffffff)
	AND(8,	0xffffffff, 0xffffffff, 0xffffffff)
	AND(9,	0x7fffffff, 0,		0)
	AND(10,	0,	    0x7fffffff,	0)
#if __WORDSIZE == 64
	AND(11,	0x7fffffffffffffff, 1,			1)
	AND(12,	1,		    0x7fffffffffffffff,	1)
	AND(13,	0x8000000000000000, 1,			0)
	AND(14,	1,		    0x8000000000000000,	0)
	AND(15,	0x7fffffffffffffff, 0x8000000000000000,	0)
	AND(16,	0x8000000000000000, 0x7fffffffffffffff,	0)
	AND(17,	0x7fffffffffffffff, 0xffffffffffffffff,	0x7fffffffffffffff)
	AND(18,	0xffffffffffffffff, 0x7fffffffffffffff,	0x7fffffffffffffff)
	AND(19,	0xffffffffffffffff, 0xffffffffffffffff,	0xffffffffffffffff)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
