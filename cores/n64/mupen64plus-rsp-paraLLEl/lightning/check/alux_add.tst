#include "alu.inc"

.code
	prolog

#define ADDX(N, I0, I1, V)	ALUX(N, add, I0, I1, V)

	/* nothing */
	ADDX(0,	0,		0,		0)
#if __WORDSIZE == 32
	/* carry */
	ADDX(1,	0xffffffff,	0xffffffff,	1)
	/* overflow */
	ADDX(2,	0x7fffffff,	1,		0)
	/* overflow */
	ADDX(3,	0x7fffffff,	0x7fffffff,	0)
	/* carry */
	ADDX(4,	0x7fffffff,	0x80000000,	0)
	/* carry+overflow */
	ADDX(5,	0x80000000,	0x80000000,	1)
#else
	/* nothing */
	ADDX(1,	0xffffffff,		0xffffffff,		0)
	/* nothing */
	ADDX(2,	0x7fffffff,		1,			0)
	/* nothing */
	ADDX(3,	0x7fffffff,		0x7fffffff,		0)
	/* nothing */
	ADDX(4,	0x7fffffff,		0x80000000,		0)
	/* nothing */
	ADDX(5,	0x80000000,		0x80000000,		0)
	/* carry */
	ADDX(6,	0xffffffffffffffff,	0xffffffffffffffff,	1)
	/* overflow */
	ADDX(7,	0x7fffffffffffffff,	1,			0)
	/* overflow */
	ADDX(8,	0x7fffffffffffffff,	0x7fffffffffffffff,	0)
	/* overflow */
	ADDX(9,	0x7fffffffffffffff,	0x8000000000000000,	0)
	/* carry+overflow */
	ADDX(10,0x8000000000000000,	0x8000000000000000,	1)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
