#if __WORDSIZE == 64
#  define L0		0x8000000000000001
#  define LL0		L0
#  define LC0		0xffffffffffffff81
#  define LS0		0xffffffffffff8001
#  define LI0		0xffffffff80000001
#  define L1		0x8000000000000000
#  define LL1		L1
#  define LC1		0xffffffffffffff80
#  define LS1		0xffffffffffff8000
#  define LI1		0xffffffff80000000
#  define L2		0x7fffffffffffffff
#  define LL2		L2
#  define LC2		0x000000000000007f
#  define LS2		0x0000000000007fff
#  define LI2		0x000000007fffffff
#  define L3		0xffffffffffffffff
#  define LL3		L3
#  define LC3		0xffffffffffffffff
#  define LS3		0xffffffffffffffff
#  define LI3		0xffffffffffffffff
#  define XC		LC0
#  define XS		LS0
#  define XI		LI0
#else
#  define XC		IC0
#  define XS		IS0
#  define XI		II0
#endif
#define I0		0x80000001
#define II0		I0
#define IC0		0xffffff81
#define IS0		0xffff8001
#define I1		0x80000000
#define II1		I1
#define IC1		0xffffff80
#define IS1		0xffff8000
#define I2		0x7fffffff
#define II2		I2
#define IC2		0x0000007f
#define IS2		0x00007fff
#define I3		0xffffffff
#define II3		I3
#define IC3		0xffffffff
#define IS3		0xffffffff
#define S0		0x8001
#define S1		0x8000
#define S2		0x7fff
#define S3		0xffff
#define C0		0x81
#define C1		0x80
#define C2		0x7f
#define C3		0xff
#define F0		 0.25
#define F1		 0.75
#define F2		-0.25
#define F3		-0.75
#define D0		 0.25
#define D1		 0.75
#define D2		-0.25
#define D3		-0.75

.data	512
ok:
.c	"ok\n"
.align	8
t0:
c0:
.c	0
uc0:
.c	0
s0:
.s	0
us0:
.s	0
.align	4
i0:
.i	0
#if __WORDSIZE == 64
ui0:
.i	0
.align	8
l0:
.l	0
#endif
f0:
.f	0
.align	8
d0:
.d	0

.	$($offc  = c0  - t0)
.	$($offuc = uc0 - t0)
.	$($offs  = s0  - t0)
.	$($offus = us0 - t0)
.	$($offi  = i0  - t0)
#if __WORDSIZE == 64
.	$($offui = ui0 - t0)
.	$($offl  = l0  - t0)
#endif
.	$($offf  = f0  - t0)
.	$($offd  = d0  - t0)
