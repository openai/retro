.text
.global main
main:
	li $t0, 40
	li $t1, -20
	add $t2, $t0, $t1
	addu $t3, $t0, $t1

	li $t0, -5
	li $t1, -4
	add $t2, $t0, $t1
	addu $t3, $t0, $t1
	break
