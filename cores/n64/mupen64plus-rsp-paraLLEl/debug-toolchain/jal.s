.text
.set noreorder
.global main
main:
	jal indelay
	nop

	jal die # Test to make sure this becomes a conditional branch.

indelay:
	jal notdie
	move $t2, $ra
	li $t0, 1

die:
	li $t0, 0x10000

notdie:
	li $t1, 2
	break

