.data
buffer:
.hword 10, 20, 30, 40, 50, 60, 70, 80
.hword 20, 30, 40, 50, 60, 70, 80, 90

.text
.set noreorder
.global main
main:
	la $t0, buffer
	# lqv 2, 0, 1(t0)
	.word (0x32 << 26) | (4 << 11) | (2 << 16) | (0 << 7) | (1 << 0) | (8 << 21)
	# sqv 2, 0, 2(t0)
	.word (0x3a << 26) | (4 << 11) | (2 << 16) | (0 << 7) | (2 << 0) | (8 << 21)
	break
