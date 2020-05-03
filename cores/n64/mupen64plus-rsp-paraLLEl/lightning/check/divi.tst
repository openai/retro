.data	128
small_ops:
.i	40 64 80
large_ops:
.i	98304 65536 163840
fmt:
.c	"%i/%i = %i (expected %i)\n"
x:
.c	"%d\n"
.code
	jmpi main

#define generate_divider(operand)	\
	name divider_##operand		\
divider_##operand:			\
	prolog				\
	arg $i				\
	getarg %r1 $i			\
	divi %r2 %r1 operand		\
	retr %r2			\
	epilog
generate_divider(8)
generate_divider(32768)

#define generate_test_divider(divisor)	\
	name test_divider_##divisor	\
test_divider_##divisor:			\
	prolog				\
	allocai 4 $loc			\
	arg $p				\
	arg $c				\
	getarg %v0 $p			\
	getarg %v1 $c			\
	muli %v1 %v1 4			\
	addr %v1 %v0 %v1		\
loop_##divisor:				\
	bger done_##divisor %v0 %v1	\
	ldr_i %v2 %v0			\
	prepare				\
		pushargr %v2		\
	finishi divider_##divisor	\
	retval %v2			\
	ldr_i %r2 %v0			\
	divi %r0 %r2 divisor		\
	/* save div result */		\
	stxi_i $loc %fp %r0		\
	prepare				\
		pushargi fmt		\
		ellipsis		\
		pushargr %r2		\
		pushargi divisor	\
		pushargr %v2		\
		pushargr %r0		\
	finishi @printf			\
	addi %v0 %v0 4			\
	/* reload div result */		\
	ldxi_i %r0 %fp $loc		\
	beqr loop_##divisor %r0 %v2	\
	/* return if failed */		\
	reti 1				\
done_##divisor:				\
	reti 0				\
	epilog
generate_test_divider(8)
generate_test_divider(32768)

	name main
main:
	prolog
	prepare
		pushargi small_ops
		pushargi 3
	finishi test_divider_8
	retval %r0
	bnei fail %r0 0	
	prepare
		pushargi large_ops
		pushargi 3
	finishi test_divider_32768
	retval %r0
	bnei fail %r0 0	
	reti 0
fail:
	reti 1
	epilog
