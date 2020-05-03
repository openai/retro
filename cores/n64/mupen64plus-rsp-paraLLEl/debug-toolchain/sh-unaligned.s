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
	sh $t0, 0($t1)
	sh $t0, 3($t1)
	break

