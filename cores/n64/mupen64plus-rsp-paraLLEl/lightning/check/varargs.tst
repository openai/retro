.data	1024
ifmt:
.c	"%d %d %d %d %d %d %d %d %d %d\n"
.align	4
ichk:
.i	9 8 7 6 5 4 3 2 1 0
dfmt:
.c	"%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n"
lfmt:
.c	"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n"
.align	8
dchk:
.d	9.0 8.0 7.0 6.0 5.0 4.0 3.0 2.0 1.0 0.0
idfmt:
.c	"%d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f\n"
ldfmt:
.c	"%d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf\n"
difmt:
.c	"%.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d %.1f %d\n"
dlfmt:
.c	"%lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d %lf %d\n"
.align	8
buff:
.size	256

.code
	prolog

	/*
		sprintf(buff, "%d %d %d %d %d %d %d %d %d %d\n",
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	 */
	prepare
		pushargi buff
		pushargi ifmt
		ellipsis
		pushargi 0
		pushargi 1
		pushargi 2
		pushargi 3
		pushargi 4
		pushargi 5
		pushargi 6
		pushargi 7
		pushargi 8
		pushargi 9
	finishi @sprintf

	/*
		sscanf(buff, "%d %d %d %d %d %d %d %d %d %d\n",
		       ichk+0, ichk+1, ichk+2, ichk+3, ichk+4,
		       ichk+5, ichk+6, ichk+7, ichk+8, ichk+9);
	 */
	movi %v0 ichk
	prepare
		pushargi buff
		pushargi ifmt
		ellipsis
		pushargr %v0		/* 0 */
		addi %v0 %v0 4
		pushargr %v0		/* 1 */
		addi %v0 %v0 4
		pushargr %v0		/* 2 */
		addi %v0 %v0 4
		pushargr %v0		/* 3 */
		addi %v0 %v0 4
		pushargr %v0		/* 4 */
		addi %v0 %v0 4
		pushargr %v0		/* 5 */
		addi %v0 %v0 4
		pushargr %v0		/* 6 */
		addi %v0 %v0 4
		pushargr %v0		/* 7 */
		addi %v0 %v0 4
		pushargr %v0		/* 8 */
		addi %v0 %v0 4
		pushargr %v0		/* 9 */
	finishi @sscanf

	movi %v0 ichk
	movi %r0 0
loopi:
	ldr_i %r1 %v0
	beqr nexti %r0 %r1
	calli @abort
nexti:
	addi %r0 %r0 1
	bgei outi %r0 10
	addi %v0 %v0 4
	jmpi loopi
outi:

	prepare
		pushargi buff
		ellipsis
	finishi @printf

	/*
		sprintf(buff,
			"%.1f %.1f %.1f %.1f %.1f "
			"%.1f %.1f %.1f %.1f %.1f\n",
			0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	 */
	prepare
		pushargi buff
		pushargi dfmt
		ellipsis
		pushargi_d 0.0
		pushargi_d 1.0
		pushargi_d 2.0
		pushargi_d 3.0
		pushargi_d 4.0
		pushargi_d 5.0
		pushargi_d 6.0
		pushargi_d 7.0
		pushargi_d 8.0
		pushargi_d 9.0
	finishi @sprintf

	/*
		sscanf(buff, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n",
		       dchk+0, dchk+1, dchk+2, dchk+3, dchk+4,
		       dchk+5, dchk+6, dchk+7, dchk+8, dchk+9);
	 */
	movi %v0 dchk
	prepare
		pushargi buff
		pushargi lfmt
		ellipsis
		pushargr %v0		/* 0 */
		addi %v0 %v0 8
		pushargr %v0		/* 1 */
		addi %v0 %v0 8
		pushargr %v0		/* 2 */
		addi %v0 %v0 8
		pushargr %v0		/* 3 */
		addi %v0 %v0 8
		pushargr %v0		/* 4 */
		addi %v0 %v0 8
		pushargr %v0		/* 5 */
		addi %v0 %v0 8
		pushargr %v0		/* 6 */
		addi %v0 %v0 8
		pushargr %v0		/* 7 */
		addi %v0 %v0 8
		pushargr %v0		/* 8 */
		addi %v0 %v0 8
		pushargr %v0		/* 9 */
	finishi @sscanf

	movi %v0 dchk
	movi_d %f0 0.0
loopd:
	ldr_d %f1 %v0
	beqr_d nextd %f0 %f1
	calli @abort
nextd:
	addi_d %f0 %f0 1.0
	bgei_d outd %f0 10.0
	addi %v0 %v0 8
	jmpi loopd
outd:

	prepare
		pushargi buff
		ellipsis
	finishi @printf

	/*
	    sprintf(buff,
		   "%d %.1f %d %.1f %d %.1f %d %.1f %d %.1f "
		   "%d %.1f %d %.1f %d %.1f %d %.1f %d %.1f\n",
		   0, 0.0, 1, 1.0, 2, 2.0, 3, 3.0, 4, 4.0,
		   5, 5.0, 6, 6.0, 7, 7.0, 8, 8.0, 9, 9.0);
	 */
	prepare
		pushargi buff
		pushargi idfmt
		ellipsis
		pushargi 0
		pushargi_d 0.0
		pushargi 1
		pushargi_d 1.0
		pushargi 2
		pushargi_d 2.0
		pushargi 3
		pushargi_d 3.0
		pushargi 4
		pushargi_d 4.0
		pushargi 5
		pushargi_d 5.0
		pushargi 6
		pushargi_d 6.0
		pushargi 7
		pushargi_d 7.0
		pushargi 8
		pushargi_d 8.0
		pushargi 9
		pushargi_d 9.0
	finishi @sprintf

	/*
		sscanf(buff,
		      "%d %lf %d %lf %d %lf %d %lf %d %lf "
		      "%d %lf %d %lf %d %lf %d %lf %d %lf\n",
		      ichk+0, dchk+0, ichk+1, dchk+1, ichk+2,
		      dchk+2, ichk+3, dchk+3, ichk+4, dchk+4,
		      ichk+5, dchk+5, ichk+6, dchk+6, ichk+7,
		      dchk+7, ichk+8, dchk+8, ichk+9, dchk+9);
	 */
	movi %v0 ichk
	movi %v1 dchk
	prepare
		pushargi buff
		pushargi ldfmt
		ellipsis
		pushargr %v0		/* 0 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 1 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 2 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 3 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 4 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 5 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 6 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 7 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 8 */
		addi %v0 %v0 4
		pushargr %v1
		addi %v1 %v1 8
		pushargr %v0		/* 9 */
		pushargr %v1
	finishi @sscanf

	movi %v0 ichk
	movi %v1 dchk
	movi %r0 0
	movi_d %f0 0.0
loopid:
	ldr_i %r1 %v0
	beqr checkd %r0 %r1
	calli @abort
checkd:
	ldr_d %f1 %v1
	beqr_d nextid %f0 %f1
	calli @abort
nextid:
	addi %r0 %r0 1
	addi_d %f0 %f0 1.0
	bgei outid %r0 10
	addi %v0 %v0 4
	addi %v1 %v1 8
	jmpi loopid
outid:

	prepare
		pushargi buff
		ellipsis
	finishi @printf

	/*
	    sprintf(buff,
		   "%.1f %d %.1f %d %.1f %d %.1f %d %.1f %d "
		   "%.1f %d %.1f %d %.1f %d %.1f %d %.1f %d\n",
		   0.0, 0, 1.0, 1, 2.0, 2, 3.0, 3, 4.0, 4,
		   5, 5.0, 6.0, 6, 7.0, 7, 8.0, 8, 9.0, 9);
	 */
	prepare
		pushargi buff
		pushargi difmt
		ellipsis
		pushargi_d 0.0
		pushargi 0
		pushargi_d 1.0
		pushargi 1
		pushargi_d 2.0
		pushargi 2
		pushargi_d 3.0
		pushargi 3
		pushargi_d 4.0
		pushargi 4
		pushargi_d 5.0
		pushargi 5
		pushargi_d 6.0
		pushargi 6
		pushargi_d 7.0
		pushargi 7
		pushargi_d 8.0
		pushargi 8
		pushargi_d 9.0
		pushargi 9
	finishi @sprintf

	/*
		sscanf(buff,
		      "%lf %d %lf %d %lf %d %lf %d %lf %d "
		      "%lf %d %lf %d %lf %d %lf %d %lf %d \n",
		      dchk+0, ichk+0, dchk+1, ichk+1, dchk+2,
		      ichk+2, dchk+3, ichk+3, dchk+4, ichk+4,
		      dchk+5, ichk+5, dchk+6, ichk+6, dchk+7,
		      ichk+7, dchk+8, ichk+8, dchk+9, ichk+9);
	 */
	movi %v0 dchk
	movi %v1 ichk
	prepare
		pushargi buff
		pushargi dlfmt
		ellipsis
		pushargr %v0		/* 0 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 1 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 2 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 3 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 4 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 5 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 6 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 7 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 8 */
		addi %v0 %v0 8
		pushargr %v1
		addi %v1 %v1 4
		pushargr %v0		/* 9 */
		pushargr %v1
	finishi @sscanf

	movi %v0 ichk
	movi %v1 dchk
	movi %r0 0
	movi_d %f0 0.0
loopdi:
	ldr_i %r1 %v0
	beqr check_d %r0 %r1
	calli @abort
check_d:
	ldr_d %f1 %v1
	beqr_d nextdi %f0 %f1
	calli @abort
nextdi:
	addi %r0 %r0 1
	addi_d %f0 %f0 1.0
	bgei outdi %r0 10
	addi %v0 %v0 4
	addi %v1 %v1 8
	jmpi loopdi
outdi:

	prepare
		pushargi buff
		ellipsis
	finishi @printf

	ret
	epilog
