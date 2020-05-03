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
	sw $t0, 0($t1)
	sw $t0, 4($t1)
	sw $t0, 12($t1)
	break

