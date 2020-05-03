.text
.set noreorder
.global main
main:
	li $t0, -1
	bgtz $t0, taken1 # This will not be taken.
	li $t2, 10

	li $t0, 0
	bgtz $t0, taken1 # This will not be taken.
	li $t2, 20

	li $t0, 1
	bgtz $t0, taken1 # This will be taken.
	li $t2, 30

taken0:
	li $t4, 5
	break
taken1:
	li $t4, 4
	break
