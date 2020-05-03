.data
.byte 0, 0, 0, 0
foobar:
.byte 0
.byte 0x92, 0x70

.text
.global main
main:
	la $t1, foobar
	lhu $t0, 1($t1)
	break
