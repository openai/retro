.text
.set noreorder
.global main
.align 10
main:
	li $t0, 1
	.rept 126
		nop
	.endr
	bgtz $t0, foobar
	# New block begins here,
	j foobar2
	li $t2, 2 # This should not execute

foobar:
	li $t3, 3 # This should execute, then we move to foobar2
	break

foobar2:
	li $t4, 4 # This executes.
	break
