#include "alu.inc"

.code
	prolog

#define SUBX(N, I0, I1, V)	ALUX(N, sub, I0, I1, V)

	/* nothing */
	SUBX(0,	 0,		0,		0)
#if __WORDSIZE == 32
	/* carry */
	SUBX(1,	0x7fffffff,	0xffffffff,	0xffffffff)
	/* overflow */
	SUBX(2,	0x80000000,	1,		0)
	/* carry */
	SUBX(3,	0x7fffffff,	0x80000000,	0xffffffff)
	/* overflow */
	SUBX(4,	0x80000000,	0x7fffffff,	0)
	/* carry+overflow */
	SUBX(5,	1,		0x80000000,	0xffffffff)
#else
	/* carry */
	SUBX(1,	0x7fffffff,		0xffffffff,		-1)
	/* nothing */
	SUBX(2,	0x80000000,		1,			0)
	/* carry */
	SUBX(3,	0x7fffffff,		0x80000000,		-1)
	/* nothing */
	SUBX(4,	0x80000000,		0x7fffffff,		0)
	/* carry */
	SUBX(5,	1,			0x80000000,		-1)
	/* carry */
	SUBX(6,	0x7fffffffffffffff,	0xffffffffffffffff,	-1)
	/* overflow */
	SUBX(7,	0x8000000000000000,	1,			0)
	/* carry */
	SUBX(8,	0x7fffffffffffffff,	0x8000000000000000,	-1)
	/* overflow */
	SUBX(9,	0x8000000000000000,	0x7fffffffffffffff,	0)
	/* carry+overflow */
	SUBX(10,1,			0x8000000000000000,	-1)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
