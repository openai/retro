.text
.global main
main:
	li $a0, 17
	addi $t0, $a0, -8
	addi $t1, $a0, 8
	addiu $t2, $a0, -8
	addiu $t3, $a0, 8
	break
