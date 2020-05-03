.text
.set noreorder
.global main
main:
	li $t0, 1
	blez $t0, foo
	break

	nop
	nop
	nop
	nop

foo:
	break
