.data
foobar:
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0

.text
.global main
main:
	li $t0, 0xaabbccdd
	la $t1, foobar
	sb $t0, 0($t1)
	sb $t0, 1($t1)
	sb $t0, 3($t1)
	break

