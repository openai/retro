.data	8
ok:
.c	"ok"

.code
	jmpi main

	name putr
putr:
	prolog
	frame 160
	arg $ac
	arg $auc
	arg $as
	arg $aus
	arg $ai
#if __WORDSIZE == 64
	arg $aui
	arg $al
#endif
	arg_f $af
	arg_d $ad
	arg $a
#if __WORDSIZE == 64
	arg $_l
	arg $_ui
#endif
	arg $_i
	arg $_us
	arg $_s
	arg $_uc
	arg $_c
	getarg_c %r0 $ac
	negr %r0 %r0
	putargr %r0 $ac
	getarg_uc %r0 $auc
	negr %r0 %r0
	putargr %r0 $auc
	getarg_s %r0 $as
	negr %r0 %r0
	putargr %r0 $as
	getarg_us %r0 $aus
	negr %r0 %r0
	putargr %r0 $aus
	getarg_i %r0 $ai
	negr %r0 %r0
	putargr %r0 $ai
#if __WORDSIZE == 64
	getarg_ui %r0 $aui
	negr %r0 %r0
	putargr %r0 $aui
	getarg_l %r0 $al
	negr %r0 %r0
	putargr %r0 $al
#endif
	getarg_f %f0 $af
	negr_f %f0 %f0
	putargr_f %f0 $af
	getarg_d %f0 $ad
	negr_d %f0 %f0
	putargr_d %f0 $ad
	getarg %r0 $a
	negr %r0 %r0
	putargr %r0 $a
#if __WORDSIZE == 64
	getarg_l %r0 $_l
	negr %r0 %r0
	putargr %r0 $_l
	getarg_ui %r0 $_ui
	negr %r0 %r0
	putargr %r0 $_ui
#endif
	getarg_i %r0 $_i
	negr %r0 %r0
	putargr %r0 $_i
	getarg_us %r0 $_us
	negr %r0 %r0
	putargr %r0 $_us
	getarg_s %r0 $_s
	negr %r0 %r0
	putargr %r0 $_s
	getarg_uc %r0 $_uc
	negr %r0 %r0
	putargr %r0 $_uc
	getarg_c %r0 $_c
	negr %r0 %r0
	putargr %r0 $_c
	jmpi _putr
rputr:
	putargi 17 $ac
	putargi 16 $auc
	putargi 15 $as
	putargi 14 $aus
	putargi 13 $ai
#if __WORDSIZE == 64
	putargi 12 $aui
	putargi 11 $al
#endif
	putargi_f 10 $af
	putargi_d 9 $ad
	putargi 8 $a
#if __WORDSIZE == 64
	putargi 7 $_l
	putargi 6 $_ui
#endif
	putargi 5 $_i
	putargi 4 $_us
	putargi 3 $_s
	putargi 2 $_uc
	putargi 1 $_c
	jmpi _puti
rputi:
	ret
	epilog

	name _putr
_putr:
	prolog
	tramp 160
	arg $ac
	arg $auc
	arg $as
	arg $aus
	arg $ai
#if __WORDSIZE == 64
	arg $aui
	arg $al
#endif
	arg_f $af
	arg_d $ad
	arg $a
#if __WORDSIZE == 64
	arg $_l
	arg $_ui
#endif
	arg $_i
	arg $_us
	arg $_s
	arg $_uc
	arg $_c
	getarg_c %r0 $ac
	beqi rac %r0 -1
	calli @abort
rac:
	getarg_uc %r0 $auc
	beqi rauc %r0 $(-2 & 0xff)
	calli @abort
rauc:
	getarg_s %r0 $as
	beqi ras %r0 -3
	calli @abort
ras:
	getarg_us %r0 $aus
	beqi raus %r0 $(-4 & 0xffff)
	calli @abort
raus:
	getarg_i %r0 $ai
	beqi rai %r0 -5
	calli @abort
rai:
#if __WORDSIZE == 64
	getarg_ui %r0 $aui
	beqi raui %r0 $(-6 & 0xffffffff)
	calli @abort
raui:
	getarg_l %r0 $al
	beqi ral %r0 -7
	calli @abort
ral:
#endif
	getarg_f %f0 $af
	beqi_f raf %f0 -8
	calli @abort
raf:
	getarg_d %f0 $ad
	beqi_d rad %f0 -9
	calli @abort
rad:
	getarg %r0 $a
	beqi ra %r0 -10
	calli @abort
ra:
#if __WORDSIZE == 64
	getarg %r0 $_l
	beqi r_l %r0 -11
	calli @abort
r_l:
	getarg_ui %r0 $_ui
	beqi r_ui %r0 $(-12 & 0xffffffff)
	calli @abort
r_ui:
#endif
	getarg_i %r0 $_i
	beqi r_i %r0 -13
	calli @abort
r_i:
	getarg_us %r0 $_us
	beqi r_us %r0 $(-14 & 0xffff)
	calli @abort
r_us:
	getarg_s %r0 $_s
	beqi r_s %r0 -15
	calli @abort
r_s:
	getarg_uc %r0 $_uc
	beqi r_uc %r0 $(-16 & 0xff)
	calli @abort
r_uc:
	getarg_c %r0 $_c
	beqi r_c %r0 -17
	calli @abort
r_c:
	jmpi rputr
	epilog

	name _puti
_puti:
	prolog
	tramp 160
	arg $ac
	arg $auc
	arg $as
	arg $aus
	arg $ai
#if __WORDSIZE == 64
	arg $aui
	arg $al
#endif
	arg_f $af
	arg_d $ad
	arg $a
#if __WORDSIZE == 64
	arg $_l
	arg $_ui
#endif
	arg $_i
	arg $_us
	arg $_s
	arg $_uc
	arg $_c
	getarg_c %r0 $ac
	beqi iac %r0 17
	calli @abort
iac:
	getarg_uc %r0 $auc
	beqi iauc %r0 16
	calli @abort
iauc:
	getarg_s %r0 $as
	beqi ias %r0 15
	calli @abort
ias:
	getarg_us %r0 $aus
	beqi iaus %r0 14
	calli @abort
iaus:
	getarg_i %r0 $ai
	beqi iai %r0 13
	calli @abort
iai:
#if __WORDSIZE == 64
	getarg_ui %r0 $aui
	beqi iaui %r0 12
	calli @abort
iaui:
	getarg_l %r0 $al
	beqi ial %r0 11
	calli @abort
ial:
#endif
	getarg_f %f0 $af
	beqi_f iaf %f0 10
	calli @abort
iaf:
	getarg_d %f0 $ad
	beqi_d iad %f0 9
	calli @abort
iad:
	getarg %r0 $a
	beqi ia %r0 8
	calli @abort
ia:
#if __WORDSIZE == 64
	getarg %r0 $_l
	beqi i_l %r0 7
	calli @abort
i_l:
	getarg_ui %r0 $_ui
	beqi i_ui %r0 6
	calli @abort
i_ui:
#endif
	getarg_i %r0 $_i
	beqi i_i %r0 5
	calli @abort
i_i:
	getarg_us %r0 $_us
	beqi i_us %r0 4
	calli @abort
i_us:
	getarg_s %r0 $_s
	beqi i_s %r0 3
	calli @abort
i_s:
	getarg_uc %r0 $_uc
	beqi i_uc %r0 2
	calli @abort
i_uc:
	getarg_c %r0 $_c
	beqi i_c %r0 1
	calli @abort
i_c:
	jmpi rputi
	epilog

	name putf
putf:
	prolog
	frame 56
	arg $i1
	arg_d $d1
	arg_f $f1
	arg_f $f2
	arg_f $f3
	arg $i2
	arg_d $d2
	getarg %r0 $i1
	addi %r0 %r0 1
	putargr %r0 $i1
	getarg_d %f0 $d1
	addi_d %f0 %f0 1
	putargr_d %f0 $d1
	getarg_f %f0 $f1
	addi_f %f0 %f0 1
	putargr_f %f0 $f1
	getarg_f %f0 $f2
	subi_f %f0 %f0 1
	putargr_f %f0 $f2
	putargi_f -5 $f3
	putargi -6 $i2
	putargi_d -7 $d2
	jmpi _putf
rputf:
	ret
	epilog

	name _putf
_putf:
	prolog
	tramp 56
	arg $i1
	arg_d $d1
	arg_f $f1
	arg_f $f2
	arg_f $f3
	arg $i2
	arg_d $d2
	getarg %r0 $i1
	beqi fi1 %r0 2
	calli @abort
fi1:
	getarg_d %f0 $d1
	beqi_d fd1 %f0 3
	calli @abort
fd1:
	getarg_f %f0 $f1
	beqi_f ff1 %f0 4
	calli @abort
ff1:
	getarg_f %f0 $f2
	beqi_f ff2 %f0 3
	calli @abort
ff2:
	getarg_f %f0 $f3
	beqi_f ff3 %f0 -5
	calli @abort
ff3:
	getarg %r0 $i2
	beqi fi2 %r0 -6
	calli @abort
fi2:
	getarg_d %f0 $d2
	beqi_d fd2 %f0 -7
	calli @abort
fd2:
	jmpi rputf
	epilog

	name main
main:
	prolog
	prepare
		pushargi 1
		pushargi 2
		pushargi 3
		pushargi 4
		pushargi 5
#if __WORDSIZE == 64
		pushargi 6
		pushargi 7
#endif
		pushargi_f 8
		pushargi_d 9
		pushargi 10
#if __WORDSIZE == 64
		pushargi 11
		pushargi 12
#endif
		pushargi 13
		pushargi 14
		pushargi 15
		pushargi 16
		pushargi 17
	finishi putr
	prepare
		pushargi 1
		pushargi_d 2
		pushargi_f 3
		pushargi_f 4
		pushargi_f 5
		pushargi 6
		pushargi_d 7
	finishi putf
	prepare
		pushargi ok
	finishi @puts
	ret
	epilog
