.data	32
dfmt:
.c	"%1.0f\n"
ifmt:
.c	"%d\n"

.code
	jmpi main

#define def_test_double(a, b, c)		\
	name test_double_##a##_##b##_##c	\
test_double_##a##_##b##_##c:			\
	prolog					\
	arg_d $d0				\
	arg_d $d1				\
	getarg_d %b $d0				\
	getarg_d %c $d1				\
	subr_d %a %b %c				\
	retr_d %a				\
	epilog
#define test_double(a, b, c, x, y)		\
	prepare					\
		pushargi_d x			\
		pushargi_d y			\
	finishi test_double_##a##_##b##_##c	\
	retval_d %f0				\
	prepare					\
		pushargi dfmt			\
		ellipsis			\
		pushargr_d %f0			\
	finishi @printf

#define def_test_int(a, b, c)			\
	name test_int_##a##_##b##_##c		\
test_int_##a##_##b##_##c:			\
	prolog					\
	arg $i0					\
	arg $i1					\
	getarg %b $i0				\
	getarg %c $i1				\
	subr %a %b %c				\
	retr %a					\
	epilog
#define test_int(a, b, c, x, y)			\
	prepare					\
		pushargi x			\
		pushargi y			\
	finishi test_int_##a##_##b##_##c	\
	retval %r0				\
	prepare					\
		pushargi ifmt			\
		ellipsis			\
		pushargr %r0			\
	finishi @printf

def_test_double(f0, f0, f0)
def_test_double(f0, f0, f1)
def_test_double(f0, f1, f0)
def_test_double(f0, f1, f2)

def_test_double(f3, f3, f3)
def_test_double(f3, f3, f1)
def_test_double(f3, f1, f3)
def_test_double(f3, f1, f2)

def_test_double(f3, f0, f0)
def_test_double(f3, f0, f3)
def_test_double(f3, f3, f0)

def_test_int(r0, r0, r0)
def_test_int(r0, r0, r1)
def_test_int(r0, r1, r0)
def_test_int(r0, r1, r2)

def_test_int(v0, v0, v0)
def_test_int(v0, v0, r1)
def_test_int(v0, r1, v0)
def_test_int(v0, r1, r2)

def_test_int(v0, r0, r0)
def_test_int(v0, r0, v0)
def_test_int(v0, v0, r0)


	name main
main:
	prolog

	test_double(f0, f0, f0, 3.0, 2.0)
	test_double(f0, f0, f1, 3.0, 2.0)
	test_double(f0, f1, f0, 3.0, 2.0)
	test_double(f0, f1, f2, 3.0, 2.0)

	test_double(f3, f3, f3, 3.0, 2.0)
	test_double(f3, f3, f1, 3.0, 2.0)
	test_double(f3, f1, f3, 3.0, 2.0)
	test_double(f3, f1, f2, 3.0, 2.0)

	test_double(f3, f0, f0, 3.0, 2.0)
	test_double(f3, f0, f3, 3.0, 2.0)
	test_double(f3, f3, f0, 3.0, 2.0)

	test_int(r0, r0, r0, 3, 2)
	test_int(r0, r0, r1, 3, 2)
	test_int(r0, r1, r0, 3, 2)
	test_int(r0, r1, r2, 3, 2)

	test_int(v0, v0, v0, 3, 2)
	test_int(v0, v0, r1, 3, 2)
	test_int(v0, r1, v0, 3, 2)
	test_int(v0, r1, r2, 3, 2)

	test_int(v0, r0, r0, 3, 2)
	test_int(v0, r0, v0, 3, 2)
	test_int(v0, v0, r0, 3, 2)

	ret
	epilog
