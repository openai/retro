.data	32
fmt:
.c	"%d\n"
.code
	prolog
	movi %r0 1
	jmpi L1				/* should not generate this */
	align $(__WORDSIZE / 8)		/* possible nops */
L1:
	bgei L4 %r0 10
	addi %r0 %r0 1
	jmpi L2
	movr %r1 %r0			/* to force jump generation */
	align $(__WORDSIZE / 8)		/* possible nops */
L2:
	bgti L4 %r0 10			/* never executed */
	align $(__WORDSIZE / 8)		/* possible nops */
L3:
	jmpi L1
	align $(__WORDSIZE / 8)		/* possible nops */
L4:
	prepare
		pushargi fmt
		ellipsis
		pushargr %r0
	finishi @printf
	ret
	epilog
