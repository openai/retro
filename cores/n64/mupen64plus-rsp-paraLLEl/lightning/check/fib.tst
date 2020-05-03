.data	32
format:
.c	"nfibs(%d) = %d\n"

.code
	jmpi main

	name nfibs
nfibs:
	prolog
	arg $in
	getarg %r0 $in		// R0 = n
	beqi ref %r0 0
	movr %r1 %r0
	movi %r0 1
	blei_u ref %r1 2
	subi %r2 %r1 2
	movr %r1 %r0
loop:
	subi %r2 %r2 1		// decr. counter
	movr %v0 %r0		// V0 = R0
	addr %r0 %r0 %r1	// R0 = R0 + R1
	movr %r1 %v0		// R1 = V0
	bnei loop %r2 0		// if (R2) goto loop
ref:
	retr %r0		// RET = R0
	epilog

	name main
main:
	prolog
	arg $argc
	arg $argv

	getarg_i %r0 $argc
	blei default %r0 1
	getarg %r0 $argv
	addi %r0 %r0 $(__WORDSIZE >> 3)
	ldr %r0 %r0
	prepare
		pushargr %r0
	finishi @atoi
	retval %r0
	jmpi call

default:
	movi %r0 32

call:
	movr %v0 %r0
	prepare
		pushargr %r0
	finishi nfibs
	retval %r0
	prepare
		pushargi format
		ellipsis
		pushargr %v0
		pushargr %r0
	finishi @printf
	ret
	epilog
