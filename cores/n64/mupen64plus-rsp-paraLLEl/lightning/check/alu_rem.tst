#include "alu.inc"

.code
	prolog

#define REM(N, I0, I1, V)	ALU(N, , rem, I0, I1, V)
#define UREM(N, I0, I1, V)	ALU(N, _u, rem, I0, I1, V)

	REM(0,	0x7fffffff, 1,		0)
	REM(1,	1,	    0x7fffffff, 1)
	REM(2,	0x80000000, 1,		0)
	REM(3,	1,	    0x80000000, 1)
	REM(4,	0x7fffffff, 2,		1)
	REM(5,	2,	    0x7fffffff,	2)
	REM(6,	0x80000000, 2,		0)
	REM(7,	2,	    0x80000000, 2)
	REM(8,	0x7fffffff, 0x80000000, 0x7fffffff)
	REM(9,	0,	    0x7fffffff,	0)
	REM(10,	0xffffffff, 0xffffffff, 0)
	UREM(0,	0x7fffffff, 1,		0)
	UREM(1,	1,	    0x7fffffff, 1)
	UREM(2,	0x80000000, 1,		0)
	UREM(3,	1,	    0x80000000, 1)
	UREM(4,	0x7fffffff, 2,		1)
	UREM(5,	2,	    0x7fffffff,	2)
	UREM(6,	0x80000000, 2,		0)
	UREM(7,	2,	    0x80000000, 2)
	UREM(8,	0x7fffffff, 0x80000000, 0x7fffffff)
	UREM(9,	0x80000000, 0x7fffffff, 1)
	UREM(10,0,	    0x7fffffff,	0)
	UREM(11,0x7fffffff, 0xffffffff, 0x7fffffff)
	UREM(12,0xffffffff, 0x7fffffff, 1)
	UREM(13,0xffffffff, 0xffffffff, 0)

#if __WORDSIZE == 32
	REM(11,	0x80000000, 0x7fffffff, 0xffffffff)
	REM(12,	0x7fffffff, 0xffffffff, 0)
	REM(13,	0xffffffff, 0x7fffffff, 0xffffffff)
#else
	REM(11,	0x80000000,	    0x7fffffff,		1)
	REM(12,	0x7fffffff,	    0xffffffff,		0x7fffffff)
	REM(13,	0xffffffff,	    0x7fffffff,		1)
	REM(14,	0x7fffffffffffffff, 1,			0)
	REM(15,	1,		    0x7fffffffffffffff,	1)
	REM(16,	0x8000000000000000, 1,			0)
	REM(17,	1,		    0x8000000000000000,	1)
	REM(18,	0x7fffffffffffffff, 2,			1)
	REM(19,	2,		    0x7fffffffffffffff,	2)
	REM(20,	0x8000000000000000, 2,			0)
	REM(21,	2,		    0x8000000000000000,	2)
	REM(22,	0x7fffffffffffffff, 0x8000000000000000,	0x7fffffffffffffff)
	REM(23,	0x8000000000000000, 0x7fffffffffffffff,	0xffffffffffffffff)
	REM(24,	0x7fffffffffffffff, 0xffffffffffffffff,	0)
	REM(25,	0xffffffffffffffff, 0x7fffffffffffffff,	0xffffffffffffffff)
	REM(26,	0xffffffffffffffff, 0xffffffffffffffff,	0)
	UREM(14,0x7fffffffffffffff, 1,			0)
	UREM(15,1,		    0x7fffffffffffffff,	1)
	UREM(16,0x8000000000000000, 1,			0)
	UREM(17,1,		    0x8000000000000000,	1)
	UREM(18,0x7fffffffffffffff, 2,			1)
	UREM(19,2,		    0x7fffffffffffffff,	2)
	UREM(20,0x8000000000000000, 2,			0)
	UREM(21,2,		    0x8000000000000000,	2)
	UREM(22,0x7fffffffffffffff, 0x8000000000000000,	0x7fffffffffffffff)
	UREM(23,0x8000000000000000, 0x7fffffffffffffff,	1)
	UREM(24,0x7fffffffffffffff, 0xffffffffffffffff,	0x7fffffffffffffff)
	UREM(25,0xffffffffffffffff, 0x7fffffffffffffff,	1)
	UREM(26,0xffffffffffffffff, 0xffffffffffffffff,	0)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
