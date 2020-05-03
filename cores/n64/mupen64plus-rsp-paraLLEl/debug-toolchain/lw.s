.data
.byte 0, 0, 0, 0
foobar:
.word 12345678
.word 30

.text
.global main
main:
	la $t1, foobar
	lw $t0, 4($t1)
	break
