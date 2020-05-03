.text
.set noreorder
.global main
main:
	li $t0, 1
	bgtz $0, dummy
	jal callsite
	li $t5, 10

	jr $s5
callsite:
	lw $t2, 184($0)
	li $t1, 10
	break

dummy:
	nop
	break

