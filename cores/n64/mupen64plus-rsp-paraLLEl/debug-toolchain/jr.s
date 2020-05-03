.text
.set noreorder
.global main
main:
	la $t5, notdie
	la $ra, indelay
	jr $ra
	nop

	j die # Test to make sure this becomes a conditional branch.

indelay:
	jr $t5
	li $t0, 1

die:
	li $t0, 0x10000

notdie:
	li $t1, 2
	break

