.data
.byte 0, 0, 0, 0
foobar:
.byte 0x92, 0x70

.text
.global main
main:
	la $t1, foobar
	lh $t0, 0($t1)
	break
