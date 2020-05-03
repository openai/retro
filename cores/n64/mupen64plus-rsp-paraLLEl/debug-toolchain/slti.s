.text
.global main
main:
	li $a0, 100000
	li $a1, -100000
	slti $t0, $a0, -40
	slti $t1, $a0, 40
	slti $t2, $a1, -40
	slti $t3, $a1, 40
	break
