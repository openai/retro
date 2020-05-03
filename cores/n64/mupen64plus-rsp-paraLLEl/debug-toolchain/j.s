.text
.set noreorder
.global main
main:
	j indelay
	nop

	j die # Test to make sure this becomes a conditional branch.

indelay:
	j notdie
	li $t0, 1

die:
	li $t0, 0x10000

notdie:
	li $t1, 2
	break

