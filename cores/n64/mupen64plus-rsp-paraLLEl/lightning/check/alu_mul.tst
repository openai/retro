#include "alu.inc"

.code
	prolog

#define MUL(N, I0, I1, V)	ALU(N, , mul, I0, I1, V)

	MUL(0,	0x7fffffff, 1,		0x7fffffff)
	MUL(1,	1,	    0x7fffffff, 0x7fffffff)
	MUL(2,	0x80000000, 1,		0x80000000)
	MUL(3,	1,	    0x80000000, 0x80000000)
	MUL(4,	0x7fffffff, 2,		0xfffffffe)
	MUL(5,	2,	    0x7fffffff,	0xfffffffe)
	MUL(6,	0x7fffffff, 0,		0)
	MUL(7,	0,	    0x7fffffff,	0)
#if __WORDSIZE == 32
	MUL(8,	0x80000000, 2,		0)
	MUL(9,	2,	    0x80000000, 0)
	MUL(10,	0x7fffffff, 0x80000000, 0x80000000)
	MUL(11,	0x80000000, 0x7fffffff, 0x80000000)
	MUL(12,	0x7fffffff, 0xffffffff, 0x80000001)
	MUL(13,	0xffffffff, 0x7fffffff, 0x80000001)
	MUL(14,	0xffffffff, 0xffffffff, 1)
#else
	MUL(8,	0x80000000,	    2,			0x100000000)
	MUL(9,	2,		    0x80000000, 	0x100000000)
	MUL(10,	0x7fffffff,	    0x80000000,		0x3fffffff80000000)
	MUL(11,	0x80000000,	    0x7fffffff,		0x3fffffff80000000)
	MUL(12,	0x7fffffff,	    0xffffffff,		0x7ffffffe80000001)
	MUL(13,	0xffffffff,	    0x7fffffff,		0x7ffffffe80000001)
	MUL(14,	0xffffffff,	    0xffffffff,		0xfffffffe00000001)
	MUL(15,	0x7fffffffffffffff, 1,			0x7fffffffffffffff)
	MUL(16,	1,		    0x7fffffffffffffff,	0x7fffffffffffffff)
	MUL(17,	0x8000000000000000, 1,			0x8000000000000000)
	MUL(18,	1,		    0x8000000000000000,	0x8000000000000000)
	MUL(19,	0x7fffffffffffffff, 2,			0xfffffffffffffffe)
	MUL(20,	2,		    0x7fffffffffffffff,	0xfffffffffffffffe)
	MUL(21,	0x8000000000000000, 2,			0)
	MUL(22,	2,		    0x8000000000000000,	0)
	MUL(23,	0x7fffffffffffffff, 0x8000000000000000,	0x8000000000000000)
	MUL(24,	0x8000000000000000, 0x7fffffffffffffff,	0x8000000000000000)
	MUL(25,	0x7fffffffffffffff, 0xffffffffffffffff,	0x8000000000000001)
	MUL(26,	0xffffffffffffffff, 0x7fffffffffffffff,	0x8000000000000001)
	MUL(27,	0xffffffffffffffff, 0xffffffffffffffff,	1)
#endif

#undef MUL
#define MUL(N, T, I0, I1, V)	FOP(N, T, mul, I0, I1, V)
	MUL(0, _f,	-0.5,	    0.5,	-0.25)
	MUL(1, _f,	 0.25,	    0.75,	 0.1875)
	MUL(0, _d,	-0.5,	    0.5,	-0.25)
	MUL(1, _d,	 0.25,	    0.75,	 0.1875)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
