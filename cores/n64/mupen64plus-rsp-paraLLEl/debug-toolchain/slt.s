.text
.global main
main:
	li $a0, 17
	li $a1, 5
	li $a2, -17
	li $a3, -5
	slt $t0, $a0, $a1
	slt $t1, $a3, $a0
	slt $t2, $a2, $a3
	slt $t3, $a2, $a1
	break
