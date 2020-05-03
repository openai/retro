.disasm		// only disassemble
.code
	prolog
	allocai 32 $buf
	arg $c
	arg $uc
	arg $s
	arg $us
	arg $i
#if __WORDSIZE == 64
	arg $ui
	arg $l
#endif
	getarg_c %r0 $c
	getarg_uc %r0 $uc
	getarg_s %r0 $s
	getarg_us %r0 $us
	getarg_i %r0 $i
#if __WORDSIZE == 64
	getarg_ui %r0 $ui
	getarg_l %r0 $l
#endif
	addr %r0 %r1 %r2
	addi %r0 %r1 2
	addcr %r0 %r1 %r2
	addci %r0 %r1 2
	addxr %r0 %r1 %r2
	addxi %r0 %r1 2
	subr %r0 %r1 %r2
	subi %r0 %r1 2
	subcr %r0 %r1 %r2
	subci %r0 %r1 2
	subxr %r0 %r1 %r2
	subxi %r0 %r1 2
	mulr %r0 %r1 %r2
	muli %r0 %r1 2
	qmulr %r0 %r1 %r2 %v0
	qmuli %r0 %r1 %r2 3
	qmulr_u %r0 %r1 %r2 %v0
	qmuli_u %r0 %r1 %r2 3
	divr %r0 %r1 %r2
	divi %r0 %r1 2
	divr_u %r0 %r1 %r2
	divi_u %r0 %r1 2
	qdivr %r0 %r1 %r2 %v0
	qdivi %r0 %r1 %r2 3
	qdivr_u %r0 %r1 %r2 %v0
	qdivi_u %r0 %r1 %r2 3
	remr %r0 %r1 %r2
	remi %r0 %r1 2
	remr_u %r0 %r1 %r2
	remi_u %r0 %r1 2
	andr %r0 %r1 %r2
	andi %r0 %r1 2
	orr %r0 %r1 %r2
	ori %r0 %r1 2
	xorr %r0 %r1 %r2
	xori %r0 %r1 2
	lshr %r0 %r1 %r2
	lshi %r0 %r1 2
	rshr %r0 %r1 %r2
	rshi %r0 %r1 2
	rshr_u %r0 %r1 %r2
	rshi_u %r0 %r1 2
	negr %r0 %r1
	comr %r0 %r1
	ltr %r0 %r1 %r2
	lti %r0 %r1 2
	ltr_u %r0 %r1 %r2
	lti_u %r0 %r1 2
	ler %r0 %r1 %r2
	lei %r0 %r1 2
	ler_u %r0 %r1 %r2
	lei_u %r0 %r1 2
	eqr %r0 %r1 %r2
	eqi %r0 %r1 2
	ger %r0 %r1 %r2
	gei %r0 %r1 2
	ger_u %r0 %r1 %r2
	gei_u %r0 %r1 2
	gtr %r0 %r1 %r2
	gti %r0 %r1 2
	gtr_u %r0 %r1 %r2
	gti_u %r0 %r1 2
	ner %r0 %r1 %r2
	nei %r0 %r1 2
	movr %r0 %r1
	movi %r0 1
	extr_c %r0 %r1
	extr_uc %r0 %r1
	extr_s %r0 %r1
	extr_us %r0 %r1
#if __WORDSIZE == 64
	extr_i %r0 %r1
	extr_ui %r0 %r1
#endif
	htonr %r0 %r1
	ntohr %r0 %r1
	ldr_c %r0 %r1
	ldi_c %r0 0x80000000
	ldr_uc %r0 %r1
	ldi_uc %r0 0x80000000
	ldr_s %r0 %r1
	ldi_s %r0 0x80000000
	ldr_us %r0 %r1
	ldi_us %r0 0x80000000
	ldr_i %r0 %r1
	ldi_i %r0 0x80000000
#if __WORDSIZE == 64
	ldr_ui %r0 %r1
	ldi_ui %r0 0x80000000
	ldr_l %r0 %r1
	ldi_l %r0 0x80000000
#endif
	ldxr_c %r0 %r1 %r2
	ldxi_c %r0 %r1 1
	ldxr_uc %r0 %r1 %r2
	ldxi_uc %r0 %r1 1
	ldxr_s %r0 %r1 %r2
	ldxi_s %r0 %r1 2
	ldxr_us %r0 %r1 %r2
	ldxi_us %r0 %r1 2
	ldxr_i %r0 %r1 %r2
	ldxi_i %r0 %r1 4
#if __WORDSIZE == 64
	ldxr_ui %r0 %r1 %r2
	ldxi_ui %r0 %r1 4
	ldxr_l %r0 %r1 %r2
	ldxi_l %r0 %r1 8
#endif
	str_c %r1 %r0
	sti_c 0x80000000 %r1
	str_s %r1 %r0
	sti_s 0x80000000 %r1
	str_i %r1 %r0
	sti_i 0x80000000 %r1
#if __WORDSIZE == 64
	str_l %r1 %r0
	sti_l 0x80000000 %r1
#endif
	stxr_c %r2 %r1 %r0
	stxi_c 1 %r1 %r0
	stxr_s %r2 %r1 %r0
	stxi_s 2 %r1 %r0
	stxr_i %r2 %r1 %r0
	stxi_i 4 %r1 %r0
#if __WORDSIZE == 64
	stxr_l %r2 %r1 %r0
	stxi_l 8 %r1 %r0
#endif
cond:
	bltr cond %r0 %r1
condi:
	blti condi %r0 1
condu:
	bltr_u condu %r0 %r1
condiu:
	blti_u condiu %r0 1
	bler cond %r0 %r1
	blei condi %r0 1
	bler_u condu %r0 %r1
	blei_u condiu %r0 1
bool:
	beqr bool %r0 %r1
booli:
	beqi booli %r0 1
	bger cond %r0 %r1
	bgei condi %r0 1
	bger_u condu %r0 %r1
	bgei_u condiu %r0 1
	bgtr cond %r0 %r1
	bgti condi %r0 1
	bgtr_u condu %r0 %r1
	bgti_u condiu %r0 1
	bner bool %r0 %r1
	bnei booli %r0 1
mask:
	bmsr mask %r0 %r1
maski:
	bmsi maski %r0 1
	bmcr mask %r0 %r1
	bmci maski %r0 1
as:
	boaddr as %r0 %r1
asi:
	boaddi asi %r0 1
asu:
	boaddr_u as %r0 %r1
	boaddi_u asi %r0 1
	bxaddr as %r0 %r1
	bxaddi asi %r0 1
	bxaddr_u as %r0 %r1
	bxaddi_u asi %r0 1
	bosubr as %r0 %r1
	bosubi asi %r0 1
	bosubr_u as %r0 %r1
	bosubi_u asi %r0 1
	bxsubr as %r0 %r1
	bxsubi asi %r0 1
	bxsubr_u as %r0 %r1
	bxsubi_u asi %r0 1
label:
	jmpr %r0
	jmpi label
	callr %r0
	calli label
	prepare
	pushargr %r0
	finishr %r0
	prepare
	pushargi 1
	ellipsis
	finishi 0x80000000
	ret
	retr %r1
	reti 2
	retval_c %r1
	retval_uc %r1
	retval_s %r1
	retval_us %r1
	retval_i %r1
#if __WORDSIZE == 64
	retval_ui %r1
	retval_l %r1
#endif
	arg_f $f
	getarg_f %f1 $f
	addr_f %f0 %f1 %f2
	addi_f %f0 %f1 0.5
	subr_f %f0 %f1 %f2
	subi_f %f0 %f1 0.5
	mulr_f %f0 %f1 %f2
	muli_f %f0 %f1 0.5
	divr_f %f0 %f1 %f2
	divi_f %f0 %f1 0.5
	negr_f %f0 %f1
	absr_f %f0 %f1
	sqrtr_f %f0 %f1
	ltr_f %r0 %f0 %f1
	lti_f %r0 %f0 0.5
	ler_f %r0 %f0 %f1
	lei_f %r0 %f0 0.5
	eqr_f %r0 %f0 %f1
	eqi_f %r0 %f0 0.5
	ger_f %r0 %f0 %f1
	gei_f %r0 %f0 0.5
	gtr_f %r0 %f0 %f1
	gti_f %r0 %f0 0.5
	ner_f %r0 %f0 %f1
	nei_f %r0 %f0 0.5
	unltr_f %r0 %f0 %f1
	unlti_f %r0 %f0 0.5
	unler_f %r0 %f0 %f1
	unlei_f %r0 %f0 0.5
	uneqr_f %r0 %f0 %f1
	uneqi_f %r0 %f0 0.5
	unger_f %r0 %f0 %f1
	ungei_f %r0 %f0 0.5
	ungtr_f %r0 %f0 %f1
	ungti_f %r0 %f0 0.5
	ltgtr_f %r0 %f0 %f1
	ltgti_f %r0 %f0 0.5
	ordr_f %r0 %f0 %f1
	ordi_f %r0 %f0 0.5
	unordr_f %r0 %f0 %f1
	unordi_f %r0 %f0 0.5
	truncr_f_i %r0 %f0
#if __WORDSIZE == 64
	truncr_f_l %r0 %f0
#endif
	extr_f %f0 %r0
	extr_d_f %f0 %f1
	movr_f %f0 %f1
	movi_f %f0 1.5
	ldr_f %f0 %r0
	ldi_f %f0 0x80000000
	ldxr_f %f0 %r0 %r1
	ldxi_f %f0 %r0 4
	str_f %r0 %f0
	sti_f 0x80000000 %f0
	stxr_f %r1 %r0 %f0
	stxi_f 4 %r0 %f0
/* FIXME the bordr_d at the end will cause an assertion on riscv due to
 * too distant jump (does not fit in a 12 bit signed int) */
ord:
	bltr_f ord %f0 %f1
ordi:
	blti_f ordi %f0 0.5
	bler_f ord %f0 %f1
	blei_f ordi %f0 0.5
	beqr_f ord %f0 %f1
	beqi_f ordi %f0 0.5
	bger_f ord %f0 %f1
	bgei_f ordi %f0 0.5
	bgtr_f ord %f0 %f1
	bgti_f ordi %f0 0.5
	bner_f ord %f0 %f1
	bnei_f ordi %f0 0.5
unord:
	bunltr_f unord %f0 %f1
unordi:
	bunlti_f unordi %f0 0.5
	bunler_f unord %f0 %f1
	bunlei_f unordi %f0 0.5
	buneqr_f unord %f0 %f1
	buneqi_f unordi %f0 0.5
	bunger_f unord %f0 %f1
	bungei_f unordi %f0 0.5
	bungtr_f unord %f0 %f1
	bungti_f unordi %f0 0.5
	bltgtr_f unord %f0 %f1
	bltgti_f unordi %f0 0.5
	bordr_f unord %f0 %f1
	bordi_f unordi %f0 0.5
	bunordr_f unord %f0 %f1
	bunordi_f unordi %f0 0.5
	prepare
	pushargr_f %f1
	pushargi_f 0.5
	finishi 0x80000000
	retr_f %f1
	reti_f 0.5
	retval_f %f1
	arg_d $f
	getarg_d %f1 $f
	addr_d %f0 %f1 %f2
	addi_d %f0 %f1 0.5
	subr_d %f0 %f1 %f2
	subi_d %f0 %f1 0.5
	mulr_d %f0 %f1 %f2
	muli_d %f0 %f1 0.5
	divr_d %f0 %f1 %f2
	divi_d %f0 %f1 0.5
	negr_d %f0 %f1
	absr_d %f0 %f1
	sqrtr_d %f0 %f1
	ltr_d %r0 %f0 %f1
	lti_d %r0 %f0 0.5
	ler_d %r0 %f0 %f1
	lei_d %r0 %f0 0.5
	eqr_d %r0 %f0 %f1
	eqi_d %r0 %f0 0.5
	ger_d %r0 %f0 %f1
	gei_d %r0 %f0 0.5
	gtr_d %r0 %f0 %f1
	gti_d %r0 %f0 0.5
	ner_d %r0 %f0 %f1
	nei_d %r0 %f0 0.5
	unltr_d %r0 %f0 %f1
	unlti_d %r0 %f0 0.5
	unler_d %r0 %f0 %f1
	unlei_d %r0 %f0 0.5
	uneqr_d %r0 %f0 %f1
	uneqi_d %r0 %f0 0.5
	unger_d %r0 %f0 %f1
	ungei_d %r0 %f0 0.5
	ungtr_d %r0 %f0 %f1
	ungti_d %r0 %f0 0.5
	ltgtr_d %r0 %f0 %f1
	ltgti_d %r0 %f0 0.5
	ordr_d %r0 %f0 %f1
	ordi_d %r0 %f0 0.5
	unordr_d %r0 %f0 %f1
	unordi_d %r0 %f0 0.5
	truncr_d_i %r0 %f0
#if __WORDSIZE == 64
	truncr_d_l %r0 %f0
#endif
	extr_d %f0 %r0
	extr_f_d %f0 %f1
	movr_d %f0 %f1
	movi_d %f0 1.5
	ldr_d %f0 %r0
	ldi_d %f0 0x80000000
	ldxr_d %f0 %r0 %r1
	ldxi_d %f0 %r0 8
	str_d %r0 %f0
	sti_d 0x80000000 %f0
	stxr_d %r1 %r0 %f0
	stxi_d 8 %r0 %f0
	bltr_d ord %f0 %f1
	blti_d ordi %f0 0.5
	bler_d ord %f0 %f1
	blei_d ordi %f0 0.5
	beqr_d ord %f0 %f1
	beqi_d ordi %f0 0.5
	bger_d ord %f0 %f1
	bgei_d ordi %f0 0.5
	bgtr_d ord %f0 %f1
	bgti_d ordi %f0 0.5
	bner_d ord %f0 %f1
	bnei_d ordi %f0 0.5
	bunltr_d unord %f0 %f1
	bunlti_d unordi %f0 0.5
	bunler_d unord %f0 %f1
	bunlei_d unordi %f0 0.5
	buneqr_d unord %f0 %f1
	buneqi_d unordi %f0 0.5
	bunger_d unord %f0 %f1
	bungei_d unordi %f0 0.5
	bungtr_d unord %f0 %f1
	bungti_d unordi %f0 0.5
	bltgtr_d unord %f0 %f1
	bltgti_d unordi %f0 0.5
	bordr_d unord %f0 %f1
	bordi_d unordi %f0 0.5
	bunordr_d unord %f0 %f1
	bunordi_d unordi %f0 0.5
	prepare
	pushargr_d %f1
	pushargi_d 0.5
	finishi 0x80000000
	retr_d %f1
	reti_d 0.5
	retval_d %f1
