.text
.set noreorder
.global main
main:
	li $t0, 10
	li $t2, 10
	nop
	.word (0x12 << 26) | (4 << 21) | (2 << 11) | (8 << 16) | (4 << 7)
	.word (0x12 << 26) | (0 << 21) | (2 << 11) | (9 << 16) | (4 << 7)
	#mtc2 $t0, $2, 4
	#mfc2 $t1, $2, 4

	ctc2 $t2, $2
	cfc2 $t3, $2

	break
