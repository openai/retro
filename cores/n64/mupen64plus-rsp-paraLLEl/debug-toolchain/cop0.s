.text
.set noreorder
.global main
main:
	li $t0, 100
	mtc0 $t0, $1
	bgez $t0, bar
	mfc0 $t1, $1
	li $t1, 200 # Should not execute.
	break

bar:
	li $t2, 400
	break
