.text
.set noreorder
.global main
.align 10
main:
	li $t0, 0
	.rept 126
		nop
	.endr
	bgtz $t0, foobar
	# New block begins here,
	# last instruction will be bgtz which needs to promote its delay slot
	# into a new block.
	# First instruction of any block have to handle potential latent delay slots like these.
	li $t1, 1
	li $t2, 2 # This should execute

foobar:
	break
