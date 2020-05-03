.text
.set noreorder
.global main
main:
	li $t0, 1
	li $t1, 1
	li $t2, 2
	li $t3, 0
	bne $t0, $t1, taken0 # This will not be taken.
	nop

	bne $t0, $t2, taken1 # Will be taken.
	li $t3, 3

taken0:
	li $t4, 5
	break
taken1:
	li $t4, 4
	break
