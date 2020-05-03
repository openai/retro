.data
foo:
.byte 0
.byte 0xaa, 0xbb, 0xcc, 0xdd

.text
.set noreorder
.global main
main:
	li $t0, 1
	la $t1, foo
	bgtz $t0, taken1
	lw $t1, 1($t1)

taken0:
	li $t5, 100
	break

taken1:
	break
