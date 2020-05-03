#include "alu.inc"

.code
	prolog

#define SUB(N, I0, I1, V)	ALU(N, , sub, I0, I1, V)

	SUB(0,	0x7fffffff, 1,		0x7ffffffe)
	SUB(2,	0x80000000, 1,		0x7fffffff)
	SUB(3,	0x80000000, 0x7fffffff,	1)
	SUB(4,	0xffffffff, 0xffffffff,	0)
	SUB(5,	0xffffffff, 0x7fffffff, 0x80000000)
	SUB(6,	0x7fffffff, 0,		0x7fffffff)
#if __WORDSIZE == 32
	SUB(7,	1,	    0x7fffffff, 0x80000002)
	SUB(8,	1,	    0x80000000, 0x80000001)
	SUB(9,	0x7fffffff, 0x80000000, 0xffffffff)
	SUB(10,	0x7fffffff, 0xffffffff, 0x80000000)
	SUB(11,	0,	    0x7fffffff,	0x80000001)
#else
	SUB(7,	1,		    0x7fffffff,		0xffffffff80000002)
	SUB(8,	1,		    0xffffffff80000000,		0x80000001)
	SUB(9,	0x7fffffff,	    0xffffffff80000000,		0xffffffff)
	SUB(10,	0xffffffff7fffffff, 0xffffffffffffffff,	0xffffffff80000000)
	SUB(11,	0,		    0x7fffffff,		0xffffffff80000001)
	SUB(12,	0x7fffffffffffffff, 1,			0x7ffffffffffffffe)
	SUB(13,	1,		    0x7fffffffffffffff, 0x8000000000000002)
	SUB(14,	0x8000000000000000, 1,			0x7fffffffffffffff)
	SUB(15,	1,		    0x8000000000000000, 0x8000000000000001)
	SUB(16,	0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff)
	SUB(17,	0x8000000000000000, 0x7fffffffffffffff,	1)
	SUB(18,	0x7fffffffffffffff, 0xffffffffffffffff, 0x8000000000000000)
	SUB(19,	0xffffffffffffffff, 0x7fffffffffffffff, 0x8000000000000000)
	SUB(20,	0xffffffffffffffff, 0xffffffffffffffff,	0)
#endif

#undef SUB
#define SUB(N, T, I0, I1, V)	FOP(N, T, sub, I0, I1, V)
	SUB(0, _f,	-0.5,	    0.5,	-1.0)
	SUB(1, _f,	 0.25,	    0.75,	-0.5)
	SUB(0, _d,	-0.5,	    0.5,	-1.0)
	SUB(1, _d,	 0.25,	    0.75,	-0.5)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
