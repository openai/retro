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
	sw $t0, 5($t1)
	sw $t0, 10($t1)
	sw $t0, 15($t1)
	break

