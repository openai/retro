#include "alu.inc"

.code
	prolog
#define ABS(N, T, I, V)		FUN(N, T, abs, I, V)
#define UABS(N, T, I, V)	UFUN(N, T, abs, I, V)
	ABS(0, _f,	-0.0,		 0.0)
	ABS(1, _f,	 0.5,		 0.5)
	ABS(2, _f,	-0.5,		 0.5)
	ABS(3, _f,	$Inf,		$Inf)
	ABS(4, _f,	$nInf,		$Inf)
	ABS(5, _f,	 1.25,		1.25)
	ABS(6, _f,	-1.25,		1.25)
	ABS(7, _f,	$nInf,		$Inf)
	UABS(0, _f,	$NaN,		$NaN)
	ABS(0, _d,	-0.0,		 0.0)
	ABS(1, _d,	 0.5,		 0.5)
	ABS(2, _d,	-0.5,		 0.5)
	ABS(3, _d,	$Inf,		$Inf)
	ABS(4, _d,	$nInf,		$Inf)
	ABS(5, _d,	 1.25,		 1.25)
	ABS(6, _d,	-1.25,		 1.25)
	ABS(7, _d,	$nInf,		$Inf)
	UABS(0, _d,	$NaN,		$NaN)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
