.text
.global main
main:
	li $a0, 100000
	li $a1, -100000
	sltiu $t0, $a0, -40
	sltiu $t1, $a0, 40
	sltiu $t2, $a1, -40
	sltiu $t3, $a1, 40
	break
