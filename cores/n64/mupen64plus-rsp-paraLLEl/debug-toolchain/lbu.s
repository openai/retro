.data
.byte 0, 0, 0
foobar:
.byte 0x72, 0x91

.text
.global main
main:
	la $t1, foobar
	lbu $t0, 1($t1)
	break
