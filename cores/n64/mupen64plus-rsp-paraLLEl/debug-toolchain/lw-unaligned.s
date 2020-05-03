.data
.byte 0, 0, 0, 0
foobar:
.word 12345678
.word 30

.text
.global main
main:
	la $t1, foobar
	lw $t0, 0($t1)
	lw $t1, 1($t1)
	lw $t2, 2($t1)
	lw $t3, 3($t1)
	break
