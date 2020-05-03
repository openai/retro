# This works on the Lightning JIT, but hangs the reference.

.text
.set noreorder
.global main
main:
	li $t0, 1
	li $t1, 1
	li $t2, 2
	li $t3, 0
	beq $t0, $t1, taken0 # This will be taken.
	beq $t0, $t1, taken1 # This will also be taken.
	li $t3, 3 # This will not be executed.

taken0:
	li $t6, 5 # This will be executed, the second branch then kicks in and runs taken1 path.
	break
taken1:
	li $t4, 4 # This will be executed.
	break
