.text
.set noreorder
.global main
main:
	la $t5, notdie
	la $ra, indelay
	jalr $a0, $ra
	move $ra, $a0

	j die # Test to make sure this becomes a conditional branch.

indelay:
	jalr $a0, $t5
	move $t0, $a0

die:
	li $t0, 0x10000

notdie:
	li $t1, 2
	break

