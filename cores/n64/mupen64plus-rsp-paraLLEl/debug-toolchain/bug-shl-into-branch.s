.text
.global main
main:
	li $t1, ((0x50000000) << 5) & 0xffffffff
	li $t0, 0x50000000
	sll $t0, $t0, 5
	bne $t0, $t1, error
	nop

	li $t2, 1
	break

error:
	li $t2, 2
	break
