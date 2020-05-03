.text
.set noreorder
.global main
main:
	jal foo
	break

	nop
	nop
	nop
	nop

foo:
	break
