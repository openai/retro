.text
.global main
main:
	li $a0, 17
	li $a1, 5
	li $a2, -17
	li $a3, -5
	sltu $t0, $a0, $a1
	sltu $t1, $a3, $a0
	sltu $t2, $a2, $a3
	sltu $t3, $a2, $a1
	break
