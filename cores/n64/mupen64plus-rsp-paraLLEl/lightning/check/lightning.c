/*
 * Copyright (C) 2012-2019  Free Software Foundation, Inc.
 *
 * This file is part of GNU lightning.
 *
 * GNU lightning is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU lightning is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * Authors:
 *	Paulo Cesar Pereira de Andrade
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#if HAVE_GETOPT_H
#  include <getopt.h>
#else
#  include <unistd.h>
#endif
#include <stdio.h>
#include <stdarg.h>
#include <lightning.h>
#include <dlfcn.h>

#if defined(__linux__) && (defined(__i386__) || defined(__x86_64__))
#  include <fpu_control.h>
#endif

/* The label_t identifier clashes with a system definitions */
#if defined(_AIX) || defined(__sun__) || defined(__osf__)
#  define label_t		l_label_t
#endif

#if defined(__hpux)
#  define DL_HANDLE		RTLD_NEXT
#elif defined(__sgi)
static void			*DL_HANDLE;
#elif defined(__osf__)
#  define DL_HANDLE		NULL
#else
#  define DL_HANDLE		RTLD_DEFAULT
#endif

#if defined(__GNUC__)
#  define noreturn		__attribute__ ((noreturn))
#  define printf_format(f, v)	__attribute__ ((format (printf, f, v)))
#  define maybe_unused		__attribute__ ((unused))
#else
#  define noreturn		/**/
#  define printf_format(f, v)	/**/
#  define maybe_unused		/**/
#endif

#define check_data(length)						\
    do {								\
	if (data_offset + length > data_length)				\
	    error(".data too small (%ld < %ld)",			\
		  data_length, data_offset + length);			\
    } while (0)

#define get_label_by_name(name)	((label_t *)get_hash(labels, name))

#define PARSING_NONE		0
#define PARSING_DATA		1
#define PARSING_CODE		2
#define MAX_IDENTIFIER		256

/*
 * Types
 */
typedef struct instr		  instr_t;
typedef union value		  value_t;
typedef struct parser		  parser_t;
typedef struct label	  	  label_t;
typedef struct patch	  	  patch_t;
typedef struct symbol	  	  symbol_t;
typedef struct hash		  hash_t;
typedef struct entry		  entry_t;
typedef int			(*function_t)(int argc, char *argv[]);

typedef enum {
    tok_eof = -1,
    tok_symbol,
    tok_char,
    tok_int,
    tok_float,
    tok_pointer,
    tok_string,
    tok_register,
    tok_dot,
    tok_newline,
    tok_semicollon,
} token_t;

typedef enum {
    skip_none,
    skip_ws,
    skip_nl,
} skip_t;

typedef enum {
    type_none,
    type_c,
    type_s,
    type_i,
    type_l,
    type_f,
    type_d,
    type_p,
} type_t;

#define compose(a, b)		(((a) << 8) | b)
typedef enum {
    expr_inc	 = compose('+', '+'),
    expr_dec	 = compose('-', '-'),
    expr_not	 = '!',
    expr_com	 = '~',
    expr_mul	 = '*',
    expr_div	 = '/',
    expr_rem	 = '%',
    expr_add	 = '+',
    expr_sub	 = '-',
    expr_lsh	 = compose('<', '<'),
    expr_rsh	 = compose('>', '>'),
    expr_and	 = '&',
    expr_or	 = '|',
    expr_xor	 = '^',
    expr_set	 = '=',
    expr_mulset	 = compose('*', '='),
    expr_divset	 = compose('/', '='),
    expr_remset	 = compose('%', '='),
    expr_addset	 = compose('+', '='),
    expr_subset	 = compose('-', '='),
    expr_lshset	 = compose(expr_lsh, '='),
    expr_rshset	 = compose(expr_rsh, '='),
    expr_andset	 = compose('&', '='),
    expr_orset	 = compose('|', '='),
    expr_xorset	 = compose('^', '='),
    expr_lt	 = '<',
    expr_le	 = compose('<', '='),
    expr_eq	 = compose('=', '='),
    expr_ne	 = compose('!', '='),
    expr_gt	 = '>',
    expr_ge	 = compose('>', '='),
    expr_andand	 = compose('&', '&'),
    expr_oror	 = compose('|', '|'),
    expr_lparen	 = '(',
    expr_rparen	 = ')',
    expr_int	 = '0',
    expr_float	 = '.',
    expr_pointer = '@',
    expr_symbol  = '$',
} expr_t;
#undef compose

struct instr {
    instr_t		 *next;
    const char		 *name;
    void		(*function)(void);
    int			  flag;
};

union value {
    jit_word_t		 i;
    jit_uword_t		 ui;
    float		 f;
    double		 d;
    void		*p;
    char		*cp;
    label_t		*label;
    patch_t		*patch;
};

struct parser {
    FILE		*fp;
    char		 name[256];
    int			 line;
    int			 regval;
    type_t		 regtype;
    expr_t		 expr;
    type_t		 type;
    value_t		 value;

    /* variable length string buffer */
    char		*string;
    int			 length;
    int			 offset;

    int			 newline;
    expr_t		 putback;
    int			 short_circuit;
    int			 parsing;

    struct {
	unsigned char	 buffer[4096];
	int		 offset;
	int		 length;
    } data;
};

typedef enum {
    label_kind_data,
    label_kind_code,
    label_kind_code_forward,
    label_kind_dynamic,
} label_kind_t;

struct hash {
    entry_t		**entries;
    int			  size;
    int			  count;
};

struct entry {
    entry_t		 *next;
    char		 *name;
    void		 *value;
    int			  flag;
};

struct label {
    label_t		*next;
    char		*name;
    void		*value;
    label_kind_t	 kind;
};

typedef enum {
    patch_kind_jmp,
    patch_kind_mov,
    patch_kind_call,
} patch_kind_t;

struct patch {
    patch_t		*next;
    label_t		*label;
    void		*value;
    patch_kind_t	 kind;
};

/* minor support for expressions */
struct symbol {
    symbol_t		*next;
    char		*name;
    value_t		 value;
    type_t		 type;
};

/*
 * Prototypes
 */
static jit_gpr_t get_ireg(void);
static jit_fpr_t get_freg(void);
static symbol_t *get_symbol(void);
static void jmp_forward(void *value, label_t *label);
static void mov_forward(void *value, label_t *label);
static void call_forward(void *value, label_t *label);
static void make_arg(void *value);
static jit_pointer_t get_arg(void);
static jit_word_t get_imm(void);
static void live(void);
static void align(void);	static void name(void);
static void prolog(void);
static void frame(void);	static void tramp(void);
static void ellipsis(void);
static void allocai(void);	static void allocar(void);
static void arg(void);
static void getarg_c(void);	static void getarg_uc(void);
static void getarg_s(void);	static void getarg_us(void);
static void getarg_i(void);
#if __WORDSIZE == 64
static void getarg_ui(void);	static void getarg_l(void);
#endif
static void getarg(void);
static void putargr(void);	static void putargi(void);
static void addr(void);		static void addi(void);
static void addxr(void);	static void addxi(void);
static void addcr(void);	static void addci(void);
static void subr(void);		static void subi(void);
static void subxr(void);	static void subxi(void);
static void subcr(void);	static void subci(void);
static void rsbr(void);		static void rsbi(void);
static void mulr(void);		static void muli(void);
static void qmulr(void);	static void qmuli(void);
static void qmulr_u(void);	static void qmuli_u(void);
static void divr(void);		static void divi(void);
static void divr_u(void);	static void divi_u(void);
static void qdivr(void);	static void qdivi(void);
static void qdivr_u(void);	static void qdivi_u(void);
static void remr(void);		static void remi(void);
static void remr_u(void);	static void remi_u(void);
static void andr(void);		static void andi(void);
static void orr(void);		static void ori(void);
static void xorr(void);		static void xori(void);
static void lshr(void);		static void lshi(void);
static void rshr(void);		static void rshi(void);
static void rshr_u(void);	static void rshi_u(void);
static void negr(void);		static void comr(void);
static void ltr(void);		static void lti(void);
static void ltr_u(void);	static void lti_u(void);
static void ler(void);		static void lei(void);
static void ler_u(void);	static void lei_u(void);
static void eqr(void);		static void eqi(void);
static void ger(void);		static void gei(void);
static void ger_u(void);	static void gei_u(void);
static void gtr(void);		static void gti(void);
static void gtr_u(void);	static void gti_u(void);
static void ner(void);		static void nei(void);
static void movr(void);		static void movi(void);
static void extr_c(void);	static void extr_uc(void);
static void extr_s(void);	static void extr_us(void);
#if __WORDSIZE == 64
static void extr_i(void);	static void extr_ui(void);
#endif
static void htonr_us(void);	static void ntohr_us(void);
static void htonr_ui(void);	static void ntohr_ui(void);
#if __WORDSIZE == 64
static void htonr_ul(void);	static void ntohr_ul(void);
#endif
static void htonr(void);	static void ntohr(void);
static void ldr_c(void);	static void ldi_c(void);
static void ldr_uc(void);	static void ldi_uc(void);
static void ldr_s(void);	static void ldi_s(void);
static void ldr_us(void);	static void ldi_us(void);
static void ldr_i(void);	static void ldi_i(void);
#if __WORDSIZE == 64
static void ldr_ui(void);	static void ldi_ui(void);
static void ldr_l(void);	static void ldi_l(void);
#endif
static void ldr(void);		static void ldi(void);
static void ldxr_c(void);	static void ldxi_c(void);
static void ldxr_uc(void);	static void ldxi_uc(void);
static void ldxr_s(void);	static void ldxi_s(void);
static void ldxr_us(void);	static void ldxi_us(void);
static void ldxr_i(void);	static void ldxi_i(void);
#if __WORDSIZE == 64
static void ldxr_ui(void);	static void ldxi_ui(void);
static void ldxr_l(void);	static void ldxi_l(void);
#endif
static void ldxr(void);		static void ldxi(void);
static void str_c(void);	static void sti_c(void);
static void str_s(void);	static void sti_s(void);
static void str_i(void);	static void sti_i(void);
#if __WORDSIZE == 64
static void str_l(void);	static void sti_l(void);
#endif
static void str(void);		static void sti(void);
static void stxr_c(void);	static void stxi_c(void);
static void stxr_s(void);	static void stxi_s(void);
static void stxr_i(void);	static void stxi_i(void);
#if __WORDSIZE == 64
static void stxr_l(void);	static void stxi_l(void);
#endif
static void stxr(void);		static void stxi(void);
static void bltr(void);		static void blti(void);
static void bltr_u(void);	static void blti_u(void);
static void bler(void);		static void blei(void);
static void bler_u(void);	static void blei_u(void);
static void beqr(void);		static void beqi(void);
static void bger(void);		static void bgei(void);
static void bger_u(void);	static void bgei_u(void);
static void bgtr(void);		static void bgti(void);
static void bgtr_u(void);	static void bgti_u(void);
static void bner(void);		static void bnei(void);
static void bmsr(void);		static void bmsi(void);
static void bmcr(void);		static void bmci(void);
static void boaddr(void);	static void boaddi(void);
static void boaddr_u(void);	static void boaddi_u(void);
static void bxaddr(void);	static void bxaddi(void);
static void bxaddr_u(void);	static void bxaddi_u(void);
static void bosubr(void);	static void bosubi(void);
static void bosubr_u(void);	static void bosubi_u(void);
static void bxsubr(void);	static void bxsubi(void);
static void bxsubr_u(void);	static void bxsubi_u(void);
static void jmpr(void);		static void jmpi(void);
static void callr(void);	static void calli(void);
static void prepare(void);
static void pushargr(void);	static void pushargi(void);
static void finishr(void);	static void finishi(void);
static void ret(void);
static void retr(void);		static void reti(void);
static void retval_c(void);	static void retval_uc(void);
static void retval_s(void);	static void retval_us(void);
static void retval_i(void);
#if __WORDSIZE == 64
static void retval_ui(void);	static void retval_l(void);
#endif
static void retval(void);
static void epilog(void);
static void arg_f(void);	static void getarg_f(void);
static void putargr_f(void);	static void putargi_f(void);
static void addr_f(void);	static void addi_f(void);
static void subr_f(void);	static void subi_f(void);
static void rsbr_f(void);	static void rsbi_f(void);
static void mulr_f(void);	static void muli_f(void);
static void divr_f(void);	static void divi_f(void);
static void negr_f(void);	static void absr_f(void);
static void sqrtr_f(void);
static void ltr_f(void);	static void lti_f(void);
static void ler_f(void);	static void lei_f(void);
static void eqr_f(void);	static void eqi_f(void);
static void ger_f(void);	static void gei_f(void);
static void gtr_f(void);	static void gti_f(void);
static void ner_f(void);	static void nei_f(void);
static void unltr_f(void);	static void unlti_f(void);
static void unler_f(void);	static void unlei_f(void);
static void uneqr_f(void);	static void uneqi_f(void);
static void unger_f(void);	static void ungei_f(void);
static void ungtr_f(void);	static void ungti_f(void);
static void ltgtr_f(void);	static void ltgti_f(void);
static void ordr_f(void);	static void ordi_f(void);
static void unordr_f(void);	static void unordi_f(void);
static void truncr_f_i(void);
#if __WORDSIZE == 64
static void truncr_f_l(void);
#endif
static void truncr_f(void);
static void extr_f(void);	static void extr_d_f(void);
static void movr_f(void);	static void movi_f(void);
static void ldr_f(void);	static void ldi_f(void);
static void ldxr_f(void);	static void ldxi_f(void);
static void str_f(void);	static void sti_f(void);
static void stxr_f(void);	static void stxi_f(void);
static void bltr_f(void);	static void blti_f(void);
static void bler_f(void);	static void blei_f(void);
static void beqr_f(void);	static void beqi_f(void);
static void bger_f(void);	static void bgei_f(void);
static void bgtr_f(void);	static void bgti_f(void);
static void bner_f(void);	static void bnei_f(void);
static void bunltr_f(void);	static void bunlti_f(void);
static void bunler_f(void);	static void bunlei_f(void);
static void buneqr_f(void);	static void buneqi_f(void);
static void bunger_f(void);	static void bungei_f(void);
static void bungtr_f(void);	static void bungti_f(void);
static void bltgtr_f(void);	static void bltgti_f(void);
static void bordr_f(void);	static void bordi_f(void);
static void bunordr_f(void);	static void bunordi_f(void);
static void pushargr_f(void);	static void pushargi_f(void);
static void retr_f(void);	static void reti_f(void);
static void retval_f(void);
static void arg_d(void);	static void getarg_d(void);
static void putargr_d(void);	static void putargi_d(void);
static void addr_d(void);	static void addi_d(void);
static void subr_d(void);	static void subi_d(void);
static void rsbr_d(void);	static void rsbi_d(void);
static void mulr_d(void);	static void muli_d(void);
static void divr_d(void);	static void divi_d(void);
static void negr_d(void);	static void absr_d(void);
static void sqrtr_d(void);
static void ltr_d(void);	static void lti_d(void);
static void ler_d(void);	static void lei_d(void);
static void eqr_d(void);	static void eqi_d(void);
static void ger_d(void);	static void gei_d(void);
static void gtr_d(void);	static void gti_d(void);
static void ner_d(void);	static void nei_d(void);
static void unltr_d(void);	static void unlti_d(void);
static void unler_d(void);	static void unlei_d(void);
static void uneqr_d(void);	static void uneqi_d(void);
static void unger_d(void);	static void ungei_d(void);
static void ungtr_d(void);	static void ungti_d(void);
static void ltgtr_d(void);	static void ltgti_d(void);
static void ordr_d(void);	static void ordi_d(void);
static void unordr_d(void);	static void unordi_d(void);
static void truncr_d_i(void);
#if __WORDSIZE == 64
static void truncr_d_l(void);
#endif
static void truncr_d(void);
static void extr_d(void);	static void extr_f_d(void);
static void movr_d(void);	static void movi_d(void);
static void ldr_d(void);	static void ldi_d(void);
static void ldxr_d(void);	static void ldxi_d(void);
static void str_d(void);	static void sti_d(void);
static void stxr_d(void);	static void stxi_d(void);
static void bltr_d(void);	static void blti_d(void);
static void bler_d(void);	static void blei_d(void);
static void beqr_d(void);	static void beqi_d(void);
static void bger_d(void);	static void bgei_d(void);
static void bgtr_d(void);	static void bgti_d(void);
static void bner_d(void);	static void bnei_d(void);
static void bunltr_d(void);	static void bunlti_d(void);
static void bunler_d(void);	static void bunlei_d(void);
static void buneqr_d(void);	static void buneqi_d(void);
static void bunger_d(void);	static void bungei_d(void);
static void bungtr_d(void);	static void bungti_d(void);
static void bltgtr_d(void);	static void bltgti_d(void);
static void bordr_d(void);	static void bordi_d(void);
static void bunordr_d(void);	static void bunordi_d(void);
static void pushargr_d(void);	static void pushargi_d(void);
static void retr_d(void);	static void reti_d(void);
static void retval_d(void);
static void vastart(void);	static void vapush(void);
static void vaarg(void);	static void vaarg_d(void);
static void vaend(void);

static void error(const char *format, ...) noreturn printf_format(1, 2);
static void warn(const char *format, ...) printf_format(1, 2) maybe_unused;
static void message(const char *kind, const char *format, va_list ap);

static int getch(void);
static int getch_noeof(void);
static int ungetch(int ch);
static int skipws(void);
static int skipnl(void);
static int skipct(void);
static int skipcp(void);
static jit_word_t get_int(skip_t skip);
static jit_uword_t get_uint(skip_t skip);
static double get_float(skip_t skip);
static float make_float(double d);
static void *get_pointer(skip_t skip);
static label_t *get_label(skip_t skip);
static token_t regname(void);
static token_t identifier(int ch);
static void get_data(type_t type);
static void dot(void);
static token_t number(int ch);
static int escape(int ch);
static token_t string(void);
static token_t dynamic(void);
static token_t character(void);
static void expression_prim(void);
static void expression_inc(int pre);
static void expression_dec(int pre);
static void expression_unary(void);
static void expression_mul(void);
static void expression_add(void);
static void expression_shift(void);
static void expression_bit(void);
static void expression_rel(void);
static void expression_cond(void);
static token_t expression(void);
static token_t primary(skip_t skip);
static void parse(void);
static int execute(int argc, char *argv[]);

static void *xmalloc(size_t size);
static void *xrealloc(void *pointer, size_t size);
static void *xcalloc(size_t nmemb, size_t size);

static label_t *new_label(label_kind_t kind, char *name, void *value);
static patch_t *new_patch(patch_kind_t kind, label_t *label, void *value);
static int bcmp_symbols(const void *left, const void *right);
static int qcmp_symbols(const void *left, const void *right);
static symbol_t *new_symbol(char *name);
static symbol_t *get_symbol_by_name(char *name);

static hash_t *new_hash(void);
static int hash_string(char *name);
static void put_hash(hash_t *hash, entry_t *entry);
static entry_t *get_hash(hash_t *hash, char *name);
static void rehash(hash_t *hash);

/*
 * Initialization
 */
static jit_state_t	 *_jit;
static int		  flag_verbose;
static int		  flag_data;
static int		  flag_disasm;
static char		 *progname;
static parser_t		  parser;
static hash_t		 *labels;
static int		  label_offset;
static patch_t		 *patches;
static symbol_t		**symbols;
static int		  symbol_length;
static int		  symbol_offset;
static hash_t		 *instrs;
static char		 *data;
static size_t		  data_offset, data_length;
static instr_t		  instr_vector[] = {
#define entry(value)	{ NULL, #value, value }
#define entry2(name, function)	{ NULL, name, function }
    entry(live),
    entry(align),	entry(name),
    entry(prolog),
    entry(frame),	entry(tramp),
    entry(ellipsis),
    entry(allocai),	entry(allocar),
    entry(arg),
    entry(getarg_c),	entry(getarg_uc),
    entry(getarg_s),	entry(getarg_us),
    entry(getarg_i),
#if __WORDSIZE == 64
    entry(getarg_ui),	entry(getarg_l),
#endif
    entry(getarg),
    entry(putargr),	entry(putargi),
    entry(addr),	entry(addi),
    entry(addxr),	entry(addxi),
    entry(addcr),	entry(addci),
    entry(subr),	entry(subi),
    entry(subxr),	entry(subxi),
    entry(subcr),	entry(subci),
    entry(rsbr),	entry(rsbi),
    entry(mulr),	entry(muli),
    entry(qmulr),	entry(qmuli),
    entry(qmulr_u),	entry(qmuli_u),
    entry(divr),	entry(divi),
    entry(divr_u),	entry(divi_u),
    entry(qdivr),	entry(qdivi),
    entry(qdivr_u),	entry(qdivi_u),
    entry(remr),	entry(remi),
    entry(remr_u),	entry(remi_u),
    entry(andr),	entry(andi),
    entry(orr),		entry(ori),
    entry(xorr),	entry(xori),
    entry(lshr),	entry(lshi),
    entry(rshr),	entry(rshi),
    entry(rshr_u),	entry(rshi_u),
    entry(negr),	entry(comr),
    entry(ltr),		entry(lti),
    entry(ltr_u),	entry(lti_u),
    entry(ler),		entry(lei),
    entry(ler_u),	entry(lei_u),
    entry(eqr),		entry(eqi),
    entry(ger),		entry(gei),
    entry(ger_u),	entry(gei_u),
    entry(gtr),		entry(gti),
    entry(gtr_u),	entry(gti_u),
    entry(ner),		entry(nei),
    entry(movr),	entry(movi),
    entry(extr_c),	entry(extr_uc),
    entry(extr_s),	entry(extr_us),
#if __WORDSIZE == 64
    entry(extr_i),	entry(extr_ui),
#endif
    entry(htonr_us),	entry(ntohr_us),
    entry(htonr_ui),	entry(ntohr_ui),
#if __WORDSIZE == 64
    entry(htonr_ul),	entry(ntohr_ul),
#endif
    entry(htonr),	entry(ntohr),
    entry(ldr_c),	entry(ldi_c),
    entry(ldr_uc), 	entry(ldi_uc),
    entry(ldr_s),	entry(ldi_s),
    entry(ldr_us), 	entry(ldi_us),
    entry(ldr_i),	entry(ldi_i),
#if __WORDSIZE == 64
    entry(ldr_ui), 	entry(ldi_ui),
    entry(ldr_l),	entry(ldi_l),
#endif
    entry(ldr),		entry(ldi),
    entry(ldxr_c),	entry(ldxi_c),
    entry(ldxr_uc),	entry(ldxi_uc),
    entry(ldxr_s),	entry(ldxi_s),
    entry(ldxr_us),	entry(ldxi_us),
    entry(ldxr_i),	entry(ldxi_i),
#if __WORDSIZE == 64
    entry(ldxr_ui),	entry(ldxi_ui),
    entry(ldxr_l),	entry(ldxi_l),
#endif
    entry(ldxr),	entry(ldxi),
    entry(str_c),	entry(sti_c),
    entry(str_s),	entry(sti_s),
    entry(str_i),	entry(sti_i),
#if __WORDSIZE == 64
    entry(str_l),	entry(sti_l),
#endif
    entry(str),		entry(sti),
    entry(stxr_c),	entry(stxi_c),
    entry(stxr_s),	entry(stxi_s),
    entry(stxr_i),	entry(stxi_i),
#if __WORDSIZE == 64
    entry(stxr_l),	entry(stxi_l),
#endif
    entry(stxr),	entry(stxi),
    entry(bltr),	entry(blti),
    entry(bltr_u),	entry(blti_u),
    entry(bler),	entry(blei),
    entry(bler_u),	entry(blei_u),
    entry(beqr),	entry(beqi),
    entry(bger),	entry(bgei),
    entry(bger_u),	entry(bgei_u),
    entry(bgtr),	entry(bgti),
    entry(bgtr_u),	entry(bgti_u),
    entry(bner),	entry(bnei),
    entry(bmsr),	entry(bmsi),
    entry(bmcr),	entry(bmci),
    entry(boaddr),	entry(boaddi),
    entry(boaddr_u),	entry(boaddi_u),
    entry(bxaddr),	entry(bxaddi),
    entry(bxaddr_u),	entry(bxaddi_u),
    entry(bosubr),	entry(bosubi),
    entry(bosubr_u),	entry(bosubi_u),
    entry(bxsubr),	entry(bxsubi),
    entry(bxsubr_u),	entry(bxsubi_u),
    entry(jmpr),	entry(jmpi),
    entry(callr),	entry(calli),
    entry(prepare),
    entry(pushargr),	entry(pushargi),
    entry(finishr),	entry(finishi),
    entry(ret),
    entry(retr),	entry(reti),
    entry(retval_c),	entry(retval_uc),
    entry(retval_s),	entry(retval_us),
    entry(retval_i),
#if __WORDSIZE == 64
    entry(retval_ui),	entry(retval_l),
#endif
    entry(retval),
    entry(epilog),
    entry(arg_f),	entry(getarg_f),
    entry(putargr_f),	entry(putargi_f),
    entry(addr_f),	entry(addi_f),
    entry(subr_f),	entry(subi_f),
    entry(rsbr_f),	entry(rsbi_f),
    entry(mulr_f),	entry(muli_f),
    entry(divr_f),	entry(divi_f),
    entry(negr_f),	entry(absr_f),
    entry(sqrtr_f),
    entry(ltr_f),	entry(lti_f),
    entry(ler_f),	entry(lei_f),
    entry(eqr_f),	entry(eqi_f),
    entry(ger_f),	entry(gei_f),
    entry(gtr_f),	entry(gti_f),
    entry(ner_f),	entry(nei_f),
    entry(unltr_f),	entry(unlti_f),
    entry(unler_f),	entry(unlei_f),
    entry(uneqr_f),	entry(uneqi_f),
    entry(unger_f),	entry(ungei_f),
    entry(ungtr_f),	entry(ungti_f),
    entry(ltgtr_f),	entry(ltgti_f),
    entry(ordr_f),	entry(ordi_f),
    entry(unordr_f),	entry(unordi_f),
    entry(truncr_f_i),
#if __WORDSIZE == 64
    entry(truncr_f_l),
#endif
    entry(truncr_f),
    entry(extr_f),	entry(extr_d_f),
    entry(movr_f),	entry(movi_f),
    entry(ldr_f),	entry(ldi_f),
    entry(ldxr_f),	entry(ldxi_f),
    entry(str_f),	entry(sti_f),
    entry(stxr_f),	entry(stxi_f),
    entry(bltr_f),	entry(blti_f),
    entry(bler_f),	entry(blei_f),
    entry(beqr_f),	entry(beqi_f),
    entry(bger_f),	entry(bgei_f),
    entry(bgtr_f),	entry(bgti_f),
    entry(bner_f),	entry(bnei_f),
    entry(bunltr_f),	entry(bunlti_f),
    entry(bunler_f),	entry(bunlei_f),
    entry(buneqr_f),	entry(buneqi_f),
    entry(bunger_f),	entry(bungei_f),
    entry(bungtr_f),	entry(bungti_f),
    entry(bltgtr_f),	entry(bltgti_f),
    entry(bordr_f),	entry(bordi_f),
    entry(bunordr_f),	entry(bunordi_f),
    entry(pushargr_f),	entry(pushargi_f),
    entry(retr_f),	entry(reti_f),
    entry(retval_f),
    entry(arg_d),	entry(getarg_d),
    entry(putargr_d),	entry(putargi_d),
    entry(addr_d),	entry(addi_d),
    entry(subr_d),	entry(subi_d),
    entry(rsbr_d),	entry(rsbi_d),
    entry(mulr_d),	entry(muli_d),
    entry(divr_d),	entry(divi_d),
    entry(negr_d),	entry(absr_d),
    entry(sqrtr_d),
    entry(ltr_d),	entry(lti_d),
    entry(ler_d),	entry(lei_d),
    entry(eqr_d),	entry(eqi_d),
    entry(ger_d),	entry(gei_d),
    entry(gtr_d),	entry(gti_d),
    entry(ner_d),	entry(nei_d),
    entry(unltr_d),	entry(unlti_d),
    entry(unler_d),	entry(unlei_d),
    entry(uneqr_d),	entry(uneqi_d),
    entry(unger_d),	entry(ungei_d),
    entry(ungtr_d),	entry(ungti_d),
    entry(ltgtr_d),	entry(ltgti_d),
    entry(ordr_d),	entry(ordi_d),
    entry(unordr_d),	entry(unordi_d),
    entry(truncr_d_i),
#if __WORDSIZE == 64
    entry(truncr_d_l),
#endif
    entry(truncr_d),
    entry(extr_d),	entry(extr_f_d),
    entry(movr_d),	entry(movi_d),
    entry(ldr_d),	entry(ldi_d),
    entry(ldxr_d),	entry(ldxi_d),
    entry(str_d),	entry(sti_d),
    entry(stxr_d),	entry(stxi_d),
    entry(bltr_d),	entry(blti_d),
    entry(bler_d),	entry(blei_d),
    entry(beqr_d),	entry(beqi_d),
    entry(bger_d),	entry(bgei_d),
    entry(bgtr_d),	entry(bgti_d),
    entry(bner_d),	entry(bnei_d),
    entry(bunltr_d),	entry(bunlti_d),
    entry(bunler_d),	entry(bunlei_d),
    entry(buneqr_d),	entry(buneqi_d),
    entry(bunger_d),	entry(bungei_d),
    entry(bungtr_d),	entry(bungti_d),
    entry(bltgtr_d),	entry(bltgti_d),
    entry(bordr_d),	entry(bordi_d),
    entry(bunordr_d),	entry(bunordi_d),
    entry(pushargr_d),	entry(pushargi_d),
    entry(retr_d),	entry(reti_d),
    entry(retval_d),
    entry2("va_start", vastart),
    entry2("va_push", vapush),
    entry2("va_arg", vaarg),
    entry2("va_arg_d", vaarg_d),
    entry2("va_end", vaend),
#undef entry
};

/*
 * Implementation
 */
static jit_gpr_t
get_ireg(void)
{
    if (primary(skip_ws) != tok_register)
	error("bad register");
    if (parser.regtype != type_l)
	error("bad int register");

    return ((jit_gpr_t)parser.regval);
}

static jit_fpr_t
get_freg(void)
{
    if (primary(skip_ws) != tok_register)
	error("bad register");
    if (parser.regtype != type_d)
	error("bad float register");

    return ((jit_fpr_t)parser.regval);
}

static symbol_t *
get_symbol(void)
{
    symbol_t	*symbol;
    int		 ch = skipws();

    if (ch != '$')
	error("expecting variable");
    (void)identifier('$');
    if (parser.string[1] == '\0')
	error("expecting variable");
    if ((symbol = get_symbol_by_name(parser.string)) == NULL)
	symbol = new_symbol(parser.string);

    return (symbol);
}

static void
jmp_forward(void *value, label_t *label)
{
    (void)new_patch(patch_kind_jmp, label, value);
}

static void
mov_forward(void *value, label_t *label)
{
    (void)new_patch(patch_kind_mov, label, value);
}

static void
call_forward(void *value, label_t *label)
{
    (void)new_patch(patch_kind_call, label, value);
}

static void
make_arg(void *value)
{
    symbol_t	*symbol = get_symbol();

    symbol->type = type_p;
    symbol->value.p = value;
}

static jit_pointer_t
get_arg(void)
{
    symbol_t	*symbol = get_symbol();

    if (symbol->type != type_p)
	error("bad argument %s type", symbol->name);

    return symbol->value.p;
}

static jit_word_t
get_imm(void)
{
    int		 ch;
    label_t	*label;
    jit_word_t	 value;
    ch = skipws();
    switch (ch) {
	case '+': case '-': case '0' ... '9':
	    ungetch(ch);
	    value = get_int(skip_none);
	    break;
	case '\'':
	    character();
	    value = parser.value.i;
	    break;
	case '$':
	    switch (expression()) {
		case tok_int:
		case tok_pointer:
		    value = parser.value.i;
		    break;
		default:
		    error("expecting immediate");
	    }
	    break;
	case '@':
	    dynamic();
	    value = (jit_word_t)parser.value.p;
	    break;
	default:
	    ungetch(ch);
	    label = get_label(skip_none);
	    if (label->kind == label_kind_data)
		value = (jit_word_t)label->value;
	    else
		error("expecting immediate");
	    break;
    }
    return (value);
}

#define entry(name)							\
static void								\
name(void)								\
{									\
    jit_##name();							\
}
#define entry_ca(name)							\
static void								\
name(void)								\
{									\
    make_arg(jit_##name());						\
}
#define entry_ia(name)							\
static void								\
name(void)								\
{									\
    jit_gpr_t		r0 = get_ireg();				\
    jit_pointer_t	ac = get_arg();					\
    jit_##name(r0, ac);							\
}
#define entry_im(name)							\
static void								\
name(void)								\
{									\
    jit_word_t	 im = get_imm();					\
    jit_##name(im);							\
}
#define entry_ir(name)							\
static void								\
name(void)								\
{									\
    jit_gpr_t	 r0 = get_ireg();					\
    jit_##name(r0);							\
}
#define entry_ima(name)							\
static void								\
name(void)								\
{									\
    jit_word_t		im = get_imm();					\
    jit_pointer_t	ac = get_arg();					\
    jit_##name(im, ac);							\
}
#define entry_ir_ir_ir(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg(), r2 = get_ireg();	\
    jit_##name(r0, r1, r2);						\
}
#define entry_ir_ir_im(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg();			\
    jit_word_t	im = get_imm();						\
    jit_##name(r0, r1, im);						\
}
#define entry_ir_ir_ir_ir(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg(),			\
		r2 = get_ireg(), r3 = get_ireg();			\
    jit_##name(r0, r1, r2, r3);						\
}
#define entry_ir_ir_ir_im(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg(), r2 = get_ireg();	\
    jit_word_t	im = get_imm();						\
    jit_##name(r0, r1, r2, im);						\
}
#define entry_ir_ir(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg();			\
    jit_##name(r0, r1);							\
}
#define entry_ir_im(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg();					\
    jit_word_t	im = get_imm();						\
    jit_##name(r0, im);							\
}
#define entry_ir_pm(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	 r0 = get_ireg();					\
    void	*pm = get_pointer(skip_ws);				\
    jit_##name(r0, pm);							\
}
#define entry_pm_ir(name)						\
static void								\
name(void)								\
{									\
    void	*pm = get_pointer(skip_ws);				\
    jit_gpr_t	 r0 = get_ireg();					\
    jit_##name(pm, r0);							\
}
#define entry_im_ir_ir(name)						\
static void								\
name(void)								\
{									\
    jit_word_t	im = get_imm();						\
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg();			\
    (void)jit_##name(im, r0, r1);					\
}
#define entry_lb_ir_ir(name)						\
static void								\
name(void)								\
{									\
    jit_node_t	*jmp;							\
    label_t	*label = get_label(skip_ws);				\
    jit_gpr_t	 r0 = get_ireg(), r1 = get_ireg();			\
    if (label->kind == label_kind_code_forward)				\
	jmp_forward((void *)jit_##name(r0, r1), label);			\
    else {								\
	jmp = jit_##name(r0, r1);					\
	jit_patch_at(jmp, (jit_node_t *)label->value);			\
    }									\
}
#define entry_lb_ir_im(name)						\
static void								\
name(void)								\
{									\
    jit_node_t	*jmp;							\
    label_t	*label = get_label(skip_ws);				\
    jit_gpr_t	 r0 = get_ireg();					\
    jit_word_t	 im = get_imm();					\
    if (label->kind == label_kind_code_forward)				\
	jmp_forward((void *)jit_##name(r0, im), label);			\
    else {								\
	jmp = jit_##name(r0, im);					\
	jit_patch_at(jmp, (jit_node_t *)label->value);			\
    }									\
}
#define entry_lb(name)							\
static void								\
name(void)								\
{									\
    jit_node_t	*jmp;							\
    label_t	*label = get_label(skip_ws);				\
    if (label->kind == label_kind_code_forward)				\
	jmp_forward((void *)jit_##name(), label);			\
    else {								\
	jmp = jit_##name();						\
	jit_patch_at(jmp, (jit_node_t *)label->value);			\
    }									\
}
#define entry_pm(name)							\
static void								\
name(void)								\
{									\
    void	*pm = get_pointer(skip_ws);				\
    jit_##name(pm);							\
}
#define entry_fa(name)							\
static void								\
name(void)								\
{									\
    jit_fpr_t		r0 = get_freg();				\
    jit_pointer_t	ac = get_arg();					\
    jit_##name(r0, ac);							\
}
#define entry_fma(name)							\
static void								\
name(void)								\
{									\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_pointer_t	ac = get_arg();					\
    jit_##name(im, ac);							\
}
#define entry_fr_fr_fr(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t	r0 = get_freg(), r1 = get_freg(), r2 = get_freg();	\
    jit_##name(r0, r1, r2);						\
}
#define entry_fr_fr_fm(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t		r0 = get_freg(), r1 = get_freg();		\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(r0, r1, make_float(im));					\
}
#define entry_fr_fr_dm(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t		r0 = get_freg(), r1 = get_freg();		\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(r0, r1, im);						\
}
#define entry_fr_fr(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t	r0 = get_freg(), r1 = get_freg();			\
    jit_##name(r0, r1);							\
}
#define entry_ir_fr_fr(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg();					\
    jit_fpr_t	r1 = get_freg(), r2 = get_freg();			\
    jit_##name(r0, r1, r2);						\
}
#define entry_ir_fr_fm(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t		r0 = get_ireg();				\
    jit_fpr_t		r1 = get_freg();				\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(r0, r1, make_float(im));					\
}
#define entry_ir_fr_dm(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t		r0 = get_ireg();				\
    jit_fpr_t		r1 = get_freg();				\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(r0, r1, im);						\
}
#define entry_ir_fr(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg();					\
    jit_fpr_t	r1 = get_freg();					\
    jit_##name(r0, r1);							\
}
#define entry_fr_ir(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t	r0 = get_freg();					\
    jit_gpr_t	r1 = get_ireg();					\
    jit_##name(r0, r1);							\
}
#define entry_fr_fm(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t		r0 = get_freg();				\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(r0, make_float(im));					\
}
#define entry_fr_dm(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t		r0 = get_freg();				\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(r0, im);							\
}
#define entry_fr_pm(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t	 r0 = get_freg();					\
    void	*pm = get_pointer(skip_ws);				\
    jit_##name(r0, pm);							\
}
#define entry_fr_ir_ir(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t	r0 = get_freg();					\
    jit_gpr_t	r1 = get_ireg(), r2 = get_ireg();			\
    jit_##name(r0, r1, r2);						\
}
#define entry_fr_ir_im(name)						\
static void								\
name(void)								\
{									\
    jit_fpr_t	r0 = get_freg();					\
    jit_gpr_t	r1 = get_ireg();					\
    jit_word_t	im = get_imm();						\
    jit_##name(r0, r1, im);						\
}
#define entry_pm_fr(name)						\
static void								\
name(void)								\
{									\
    void	*pm = get_pointer(skip_ws);				\
    jit_fpr_t	 r0 = get_freg();					\
    jit_##name(pm, r0);							\
}
#define entry_ir_ir_fr(name)						\
static void								\
name(void)								\
{									\
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg();			\
    jit_fpr_t	r2 = get_freg();					\
    jit_##name(r0, r1, r2);						\
}
#define entry_im_ir_fr(name)						\
static void								\
name(void)								\
{									\
    jit_word_t	im = get_imm();						\
    jit_gpr_t	r0 = get_ireg();					\
    jit_fpr_t	r1 = get_freg();					\
    jit_##name(im, r0, r1);						\
}
#define entry_lb_fr_fr(name)						\
static void								\
name(void)								\
{									\
    jit_node_t	*jmp;							\
    label_t	*label = get_label(skip_ws);				\
    jit_fpr_t	 r0 = get_freg(), r1 = get_freg();			\
    if (label->kind == label_kind_code_forward)				\
	jmp_forward((void *)jit_##name(r0, r1), label);			\
    else {								\
	jmp = jit_##name(r0, r1);					\
	jit_patch_at(jmp, (jit_node_t *)label->value);			\
    }									\
}
#define entry_lb_fr_fm(name)						\
static void								\
name(void)								\
{									\
    jit_node_t		*jmp;						\
    label_t		*label = get_label(skip_ws);			\
    jit_fpr_t		 r0 = get_freg();				\
    jit_float64_t	 im = get_float(skip_ws);			\
    if (label->kind == label_kind_code_forward)				\
	jmp_forward((void *)jit_##name(r0, make_float(im)), label);	\
    else {								\
	jmp = jit_##name(r0, make_float(im));				\
	jit_patch_at(jmp, (jit_node_t *)label->value);			\
    }									\
}
#define entry_lb_fr_dm(name)						\
static void								\
name(void)								\
{									\
    jit_node_t		*jmp;						\
    label_t		*label = get_label(skip_ws);			\
    jit_fpr_t		 r0 = get_freg();				\
    jit_float64_t	 im = get_float(skip_ws);			\
    if (label->kind == label_kind_code_forward)				\
	jmp_forward((void *)jit_##name(r0, im), label);			\
    else {								\
	jmp = jit_##name(r0, im);					\
	jit_patch_at(jmp, (jit_node_t *)label->value);			\
    }									\
}
#define entry_fr(name)							\
static void								\
name(void)								\
{									\
    jit_fpr_t	r0 = get_freg();					\
    jit_##name(r0);							\
}
#define entry_fm(name)							\
static void								\
name(void)								\
{									\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(make_float(im));						\
}
#define entry_dm(name)							\
static void								\
name(void)								\
{									\
    jit_float64_t	im = get_float(skip_ws);			\
    jit_##name(im);							\
}
#define entry_fn(name)							\
static void								\
name(void)								\
{									\
    int		 ch;							\
    label_t	*label;							\
    void	*value;							\
    ch = skipws();							\
    switch (ch) {							\
	case '0' ... '9':						\
	    ungetch(ch);						\
	    value = (void *)(jit_word_t)get_uint(skip_none);		\
	    break;							\
	case '$':							\
	    switch (expression()) {					\
		case tok_int:						\
		    value = (void *)parser.value.i;			\
		    break;						\
		case tok_pointer:					\
		    value = parser.value.p;				\
		    break;						\
		default:						\
		    error("expecting pointer");				\
	    }								\
	    break;							\
	case '@':							\
	    dynamic();							\
	    value = parser.value.p;					\
	    break;							\
	default:							\
	    ungetch(ch);						\
	    label = get_label(skip_none);				\
	    if (label->kind == label_kind_code_forward)			\
		call_forward((void *)jit_##name(NULL), label);		\
	    else							\
		jit_patch_at(jit_##name(NULL), label->value);		\
	    return;							\
    }									\
    jit_##name(value);							\
}
static void
name(void) {
    int		 ch = skipws();
    (void)identifier(ch);
    jit_name(parser.string);
}
static void
live(void) {
    if (primary(skip_ws) != tok_register)
	error("bad register");
    jit_live(parser.regval);
}
entry_im(align)
entry(prolog)
entry_im(frame)			entry_im(tramp)
entry(ellipsis)
void
allocai(void) {
    symbol_t	*symbol;
    jit_word_t	 i, im = get_imm();
    i = jit_allocai(im);
    symbol = get_symbol();
    symbol->type = type_l;
    symbol->value.i = i;
}
entry_ir_ir(allocar)
entry_ca(arg)
entry_ia(getarg_c)		entry_ia(getarg_uc)
entry_ia(getarg_s)		entry_ia(getarg_us)
entry_ia(getarg_i)
#if __WORDSIZE == 64
entry_ia(getarg_ui)		entry_ia(getarg_l)
#endif
entry_ia(getarg)
entry_ia(putargr)		entry_ima(putargi)
entry_ir_ir_ir(addr)		entry_ir_ir_im(addi)
entry_ir_ir_ir(addxr)		entry_ir_ir_im(addxi)
entry_ir_ir_ir(addcr)		entry_ir_ir_im(addci)
entry_ir_ir_ir(subr)		entry_ir_ir_im(subi)
entry_ir_ir_ir(subxr)		entry_ir_ir_im(subxi)
entry_ir_ir_ir(subcr)		entry_ir_ir_im(subci)
entry_ir_ir_ir(rsbr)		entry_ir_ir_im(rsbi)
entry_ir_ir_ir(mulr)		entry_ir_ir_im(muli)
entry_ir_ir_ir_ir(qmulr)	entry_ir_ir_ir_im(qmuli)
entry_ir_ir_ir_ir(qmulr_u)	entry_ir_ir_ir_im(qmuli_u)
entry_ir_ir_ir(divr)		entry_ir_ir_im(divi)
entry_ir_ir_ir(divr_u)		entry_ir_ir_im(divi_u)
entry_ir_ir_ir_ir(qdivr)	entry_ir_ir_ir_im(qdivi)
entry_ir_ir_ir_ir(qdivr_u)	entry_ir_ir_ir_im(qdivi_u)
entry_ir_ir_ir(remr)		entry_ir_ir_im(remi)
entry_ir_ir_ir(remr_u)		entry_ir_ir_im(remi_u)
entry_ir_ir_ir(andr)		entry_ir_ir_im(andi)
entry_ir_ir_ir(orr)		entry_ir_ir_im(ori)
entry_ir_ir_ir(xorr)		entry_ir_ir_im(xori)
entry_ir_ir_ir(lshr)		entry_ir_ir_im(lshi)
entry_ir_ir_ir(rshr)		entry_ir_ir_im(rshi)
entry_ir_ir_ir(rshr_u)		entry_ir_ir_im(rshi_u)
entry_ir_ir(negr)		entry_ir_ir(comr)
entry_ir_ir_ir(ltr)		entry_ir_ir_im(lti)
entry_ir_ir_ir(ltr_u)		entry_ir_ir_im(lti_u)
entry_ir_ir_ir(ler)		entry_ir_ir_im(lei)
entry_ir_ir_ir(ler_u)		entry_ir_ir_im(lei_u)
entry_ir_ir_ir(eqr)		entry_ir_ir_im(eqi)
entry_ir_ir_ir(ger)		entry_ir_ir_im(gei)
entry_ir_ir_ir(ger_u)		entry_ir_ir_im(gei_u)
entry_ir_ir_ir(gtr)		entry_ir_ir_im(gti)
entry_ir_ir_ir(gtr_u)		entry_ir_ir_im(gti_u)
entry_ir_ir_ir(ner)		entry_ir_ir_im(nei)
entry_ir_ir(movr)
static void
movi(void)
{
    int		 ch;
    label_t	*label;
    void	*value;
    jit_gpr_t	 r0 = get_ireg();
    ch = skipws();
    switch (ch) {
	case '+': case '-':
	case '0' ... '9':
	    ungetch(ch);
	    value = (void *)(jit_word_t)get_uint(skip_none);
	    break;
	case '\'':
	    character();
	    value = (void *)parser.value.i;
	    break;
	case '$':
	    switch (expression()) {
		case tok_int:
		    value = (void *)parser.value.i;
		    break;
		case tok_pointer:
		    value = parser.value.p;
		    break;
		default:
		    error("expecting pointer");
	    }
	    break;
	case '@':
	    dynamic();
	    value = parser.value.p;
	    break;
	default:
	    ungetch(ch);
	    label = get_label(skip_none);
	    if (label->kind == label_kind_code ||
		label->kind == label_kind_code_forward) {
		mov_forward((void *)jit_movi(r0, 0), label);
		return;
	    }
	    value = label->value;
	    break;
    }
    jit_movi(r0, (jit_word_t)value);
}
entry_ir_ir(extr_c)		entry_ir_ir(extr_uc)
entry_ir_ir(extr_s)		entry_ir_ir(extr_us)
#if __WORDSIZE == 64
entry_ir_ir(extr_i)		entry_ir_ir(extr_ui)
#endif
entry_ir_ir(htonr_us)		entry_ir_ir(ntohr_us)
entry_ir_ir(htonr_ui)		entry_ir_ir(ntohr_ui)
#if __WORDSIZE == 64
entry_ir_ir(htonr_ul)		entry_ir_ir(ntohr_ul)
#endif
entry_ir_ir(htonr)		entry_ir_ir(ntohr)
entry_ir_ir(ldr_c)		entry_ir_pm(ldi_c)
entry_ir_ir(ldr_uc)		entry_ir_pm(ldi_uc)
entry_ir_ir(ldr_s)		entry_ir_pm(ldi_s)
entry_ir_ir(ldr_us)		entry_ir_pm(ldi_us)
entry_ir_ir(ldr_i)		entry_ir_pm(ldi_i)
#if __WORDSIZE == 64
entry_ir_ir(ldr_ui)		entry_ir_pm(ldi_ui)
entry_ir_ir(ldr_l)		entry_ir_pm(ldi_l)
#endif
entry_ir_ir(ldr)		entry_ir_pm(ldi)
entry_ir_ir_ir(ldxr_c)		entry_ir_ir_im(ldxi_c)
entry_ir_ir_ir(ldxr_uc)		entry_ir_ir_im(ldxi_uc)
entry_ir_ir_ir(ldxr_s)		entry_ir_ir_im(ldxi_s)
entry_ir_ir_ir(ldxr_us)		entry_ir_ir_im(ldxi_us)
entry_ir_ir_ir(ldxr_i)		entry_ir_ir_im(ldxi_i)
#if __WORDSIZE == 64
entry_ir_ir_ir(ldxr_ui)		entry_ir_ir_im(ldxi_ui)
entry_ir_ir_ir(ldxr_l)		entry_ir_ir_im(ldxi_l)
#endif
entry_ir_ir_ir(ldxr)		entry_ir_ir_im(ldxi)
entry_ir_ir(str_c)		entry_pm_ir(sti_c)
entry_ir_ir(str_s)		entry_pm_ir(sti_s)
entry_ir_ir(str_i)		entry_pm_ir(sti_i)
#if __WORDSIZE == 64
entry_ir_ir(str_l)		entry_pm_ir(sti_l)
#endif
entry_ir_ir(str)		entry_pm_ir(sti)
entry_ir_ir_ir(stxr_c)		entry_im_ir_ir(stxi_c)
entry_ir_ir_ir(stxr_s)		entry_im_ir_ir(stxi_s)
entry_ir_ir_ir(stxr_i)		entry_im_ir_ir(stxi_i)
#if __WORDSIZE == 64
entry_ir_ir_ir(stxr_l)		entry_im_ir_ir(stxi_l)
#endif
entry_ir_ir_ir(stxr)		entry_im_ir_ir(stxi)
entry_lb_ir_ir(bltr)		entry_lb_ir_im(blti)
entry_lb_ir_ir(bltr_u)		entry_lb_ir_im(blti_u)
entry_lb_ir_ir(bler)		entry_lb_ir_im(blei)
entry_lb_ir_ir(bler_u)		entry_lb_ir_im(blei_u)
entry_lb_ir_ir(beqr)		entry_lb_ir_im(beqi)
entry_lb_ir_ir(bger)		entry_lb_ir_im(bgei)
entry_lb_ir_ir(bger_u)		entry_lb_ir_im(bgei_u)
entry_lb_ir_ir(bgtr)		entry_lb_ir_im(bgti)
entry_lb_ir_ir(bgtr_u)		entry_lb_ir_im(bgti_u)
entry_lb_ir_ir(bner)		entry_lb_ir_im(bnei)
entry_lb_ir_ir(bmsr)		entry_lb_ir_im(bmsi)
entry_lb_ir_ir(bmcr)		entry_lb_ir_im(bmci)
entry_lb_ir_ir(boaddr)		entry_lb_ir_im(boaddi)
entry_lb_ir_ir(boaddr_u)	entry_lb_ir_im(boaddi_u)
entry_lb_ir_ir(bxaddr)		entry_lb_ir_im(bxaddi)
entry_lb_ir_ir(bxaddr_u)	entry_lb_ir_im(bxaddi_u)
entry_lb_ir_ir(bosubr)		entry_lb_ir_im(bosubi)
entry_lb_ir_ir(bosubr_u)	entry_lb_ir_im(bosubi_u)
entry_lb_ir_ir(bxsubr)		entry_lb_ir_im(bxsubi)
entry_lb_ir_ir(bxsubr_u)	entry_lb_ir_im(bxsubi_u)
entry_ir(jmpr)			entry_lb(jmpi)
entry_ir(callr)			entry_fn(calli)
entry(prepare)
entry_ir(pushargr)		entry_im(pushargi)
entry_ir(finishr)		entry_fn(finishi)
entry(ret)
entry_ir(retr)			entry_im(reti)
entry_ir(retval_c)		entry_ir(retval_uc)
entry_ir(retval_s)		entry_ir(retval_us)
entry_ir(retval_i)
#if __WORDSIZE == 64
entry_ir(retval_ui)		entry_ir(retval_l)
#endif
entry_ir(retval)
entry(epilog)
entry_ca(arg_f)			entry_fa(getarg_f)
entry_fa(putargr_f)		entry_fma(putargi_f)
entry_fr_fr_fr(addr_f)		entry_fr_fr_fm(addi_f)
entry_fr_fr_fr(subr_f)		entry_fr_fr_fm(subi_f)
entry_fr_fr_fr(rsbr_f)		entry_fr_fr_fm(rsbi_f)
entry_fr_fr_fr(mulr_f)		entry_fr_fr_fm(muli_f)
entry_fr_fr_fr(divr_f)		entry_fr_fr_fm(divi_f)
entry_fr_fr(negr_f)		entry_fr_fr(absr_f)
entry_fr_fr(sqrtr_f)
entry_ir_fr_fr(ltr_f)		entry_ir_fr_fm(lti_f)
entry_ir_fr_fr(ler_f)		entry_ir_fr_fm(lei_f)
entry_ir_fr_fr(eqr_f)		entry_ir_fr_fm(eqi_f)
entry_ir_fr_fr(ger_f)		entry_ir_fr_fm(gei_f)
entry_ir_fr_fr(gtr_f)		entry_ir_fr_fm(gti_f)
entry_ir_fr_fr(ner_f)		entry_ir_fr_fm(nei_f)
entry_ir_fr_fr(unltr_f)		entry_ir_fr_fm(unlti_f)
entry_ir_fr_fr(unler_f)		entry_ir_fr_fm(unlei_f)
entry_ir_fr_fr(uneqr_f)		entry_ir_fr_fm(uneqi_f)
entry_ir_fr_fr(unger_f)		entry_ir_fr_fm(ungei_f)
entry_ir_fr_fr(ungtr_f)		entry_ir_fr_fm(ungti_f)
entry_ir_fr_fr(ltgtr_f)		entry_ir_fr_fm(ltgti_f)
entry_ir_fr_fr(ordr_f)		entry_ir_fr_fm(ordi_f)
entry_ir_fr_fr(unordr_f)	entry_ir_fr_fm(unordi_f)
entry_ir_fr(truncr_f_i)
#if __WORDSIZE == 64
entry_ir_fr(truncr_f_l)
#endif
entry_ir_fr(truncr_f)
entry_fr_ir(extr_f)		entry_fr_fr(extr_d_f)
entry_fr_fr(movr_f)		entry_fr_fm(movi_f)
entry_fr_ir(ldr_f)		entry_fr_pm(ldi_f)
entry_fr_ir_ir(ldxr_f)		entry_fr_ir_im(ldxi_f)
entry_ir_fr(str_f)		entry_pm_fr(sti_f)
entry_ir_ir_fr(stxr_f)		entry_im_ir_fr(stxi_f)
entry_lb_fr_fr(bltr_f)		entry_lb_fr_fm(blti_f)
entry_lb_fr_fr(bler_f)		entry_lb_fr_fm(blei_f)
entry_lb_fr_fr(beqr_f)		entry_lb_fr_fm(beqi_f)
entry_lb_fr_fr(bger_f)		entry_lb_fr_fm(bgei_f)
entry_lb_fr_fr(bgtr_f)		entry_lb_fr_fm(bgti_f)
entry_lb_fr_fr(bner_f)		entry_lb_fr_fm(bnei_f)
entry_lb_fr_fr(bunltr_f)	entry_lb_fr_fm(bunlti_f)
entry_lb_fr_fr(bunler_f)	entry_lb_fr_fm(bunlei_f)
entry_lb_fr_fr(buneqr_f)	entry_lb_fr_fm(buneqi_f)
entry_lb_fr_fr(bunger_f)	entry_lb_fr_fm(bungei_f)
entry_lb_fr_fr(bungtr_f)	entry_lb_fr_fm(bungti_f)
entry_lb_fr_fr(bltgtr_f)	entry_lb_fr_fm(bltgti_f)
entry_lb_fr_fr(bordr_f)		entry_lb_fr_fm(bordi_f)
entry_lb_fr_fr(bunordr_f)	entry_lb_fr_fm(bunordi_f)
entry_fr(pushargr_f)		entry_fm(pushargi_f)
entry_fr(retr_f)		entry_fm(reti_f)
entry_fr(retval_f)
entry_ca(arg_d)			entry_fa(getarg_d)
entry_fa(putargr_d)		entry_fma(putargi_d)
entry_fr_fr_fr(addr_d)		entry_fr_fr_dm(addi_d)
entry_fr_fr_fr(subr_d)		entry_fr_fr_dm(subi_d)
entry_fr_fr_fr(rsbr_d)		entry_fr_fr_dm(rsbi_d)
entry_fr_fr_fr(mulr_d)		entry_fr_fr_dm(muli_d)
entry_fr_fr_fr(divr_d)		entry_fr_fr_dm(divi_d)
entry_fr_fr(negr_d)		entry_fr_fr(absr_d)
entry_fr_fr(sqrtr_d)
entry_ir_fr_fr(ltr_d)		entry_ir_fr_dm(lti_d)
entry_ir_fr_fr(ler_d)		entry_ir_fr_dm(lei_d)
entry_ir_fr_fr(eqr_d)		entry_ir_fr_dm(eqi_d)
entry_ir_fr_fr(ger_d)		entry_ir_fr_dm(gei_d)
entry_ir_fr_fr(gtr_d)		entry_ir_fr_dm(gti_d)
entry_ir_fr_fr(ner_d)		entry_ir_fr_dm(nei_d)
entry_ir_fr_fr(unltr_d)		entry_ir_fr_dm(unlti_d)
entry_ir_fr_fr(unler_d)		entry_ir_fr_dm(unlei_d)
entry_ir_fr_fr(uneqr_d)		entry_ir_fr_dm(uneqi_d)
entry_ir_fr_fr(unger_d)		entry_ir_fr_dm(ungei_d)
entry_ir_fr_fr(ungtr_d)		entry_ir_fr_dm(ungti_d)
entry_ir_fr_fr(ltgtr_d)		entry_ir_fr_dm(ltgti_d)
entry_ir_fr_fr(ordr_d)		entry_ir_fr_dm(ordi_d)
entry_ir_fr_fr(unordr_d)	entry_ir_fr_dm(unordi_d)
entry_ir_fr(truncr_d_i)
#if __WORDSIZE == 64
entry_ir_fr(truncr_d_l)
#endif
entry_ir_fr(truncr_d)
entry_fr_ir(extr_d)		entry_fr_fr(extr_f_d)
entry_fr_fr(movr_d)		entry_fr_dm(movi_d)
entry_fr_ir(ldr_d)		entry_fr_pm(ldi_d)
entry_fr_ir_ir(ldxr_d)		entry_fr_ir_im(ldxi_d)
entry_ir_fr(str_d)		entry_pm_fr(sti_d)
entry_ir_ir_fr(stxr_d)		entry_im_ir_fr(stxi_d)
entry_lb_fr_fr(bltr_d)		entry_lb_fr_dm(blti_d)
entry_lb_fr_fr(bler_d)		entry_lb_fr_dm(blei_d)
entry_lb_fr_fr(beqr_d)		entry_lb_fr_dm(beqi_d)
entry_lb_fr_fr(bger_d)		entry_lb_fr_dm(bgei_d)
entry_lb_fr_fr(bgtr_d)		entry_lb_fr_dm(bgti_d)
entry_lb_fr_fr(bner_d)		entry_lb_fr_dm(bnei_d)
entry_lb_fr_fr(bunltr_d)	entry_lb_fr_dm(bunlti_d)
entry_lb_fr_fr(bunler_d)	entry_lb_fr_dm(bunlei_d)
entry_lb_fr_fr(buneqr_d)	entry_lb_fr_dm(buneqi_d)
entry_lb_fr_fr(bunger_d)	entry_lb_fr_dm(bungei_d)
entry_lb_fr_fr(bungtr_d)	entry_lb_fr_dm(bungti_d)
entry_lb_fr_fr(bltgtr_d)	entry_lb_fr_dm(bltgti_d)
entry_lb_fr_fr(bordr_d)		entry_lb_fr_dm(bordi_d)
entry_lb_fr_fr(bunordr_d)	entry_lb_fr_dm(bunordi_d)
entry_fr(pushargr_d)		entry_dm(pushargi_d)
entry_fr(retr_d)		entry_dm(reti_d)
entry_fr(retval_d)
static void
vastart(void)
{
    jit_gpr_t	r0 = get_ireg();
    jit_va_start(r0);
}
static void
vapush(void)
{
    jit_gpr_t	r0 = get_ireg();
    jit_va_push(r0);
}
static void
vaarg(void)
{
    jit_gpr_t	r0 = get_ireg(), r1 = get_ireg();
    jit_va_arg(r0, r1);
}
static void
vaarg_d(void)
{
    jit_fpr_t	r0 = get_freg();
    jit_gpr_t	r1 = get_ireg();
    jit_va_arg_d(r0, r1);
}
static void
vaend(void)
{
    jit_gpr_t	r0 = get_ireg();
    jit_va_end(r0);
}
#undef entry_fn
#undef entry_fm
#undef entry_dm
#undef entry_lb_fr_fm
#undef entry_lb_fr_dm
#undef entry_lb_fr_fr
#undef entry_im_ir_fr
#undef entry_ir_ir_fr
#undef entry_pm_fr
#undef entry_fr_ir_ir
#undef entry_fr_ir_im
#undef entry_fr_pm
#undef entry_fr_fm
#undef entry_fr_dm
#undef entry_fr_ir
#undef entry_ir_fr
#undef entry_ir_fr_fm
#undef entry_ir_fr_dm
#undef entry_ir_fr_fr
#undef entry_fr_fr
#undef entry_fr_fr_fm
#undef entry_fr_fr_dm
#undef entry_fr_fr_fr
#undef entry_fma
#undef entry_fa
#undef entry_pm
#undef entry_lb
#undef entry_lb_ir_im
#undef entry_lb_ir_ir
#undef entry_im_ir_ir
#undef entry_pm_ir
#undef entry_ir_pm
#undef entry_ir_im
#undef entry_ir_ir
#undef entry_ir_ir_im
#undef entry_ir_ir_ir
#undef entry_ima
#undef entry_ir
#undef entry_im
#undef entry_ia
#undef entry_ca
#undef entry

static void
error(const char *format, ...)
{
    va_list	 ap;
    int		 length;
    char	*string;

    va_start(ap, format);
    message("error", format, ap);
    va_end(ap);
    length = parser.data.length - parser.data.offset;
    string = (char *)(parser.data.buffer + parser.data.offset - 1);
    if (length > 77)
	strcpy(string + 74, "...");
    else
	parser.data.buffer[parser.data.length - 1] = '\0';
    fprintf(stderr, "(%s)\n", string);
    exit(-1);
}

static void
warn(const char *format, ...)
{
    va_list	ap;
    va_start(ap, format);
    message("warning", format, ap);
    va_end(ap);
}

static void
message(const char *kind, const char *format, va_list ap)
{
    fprintf(stderr, "%s:%d: %s: ", parser.name,
	    parser.line - parser.newline, kind);
    vfprintf(stderr, format, ap);
    fputc('\n', stderr);
}

static int
getch(void)
{
    int		ch;

    if (parser.data.offset < parser.data.length)
	ch = parser.data.buffer[parser.data.offset++];
    else {
	/* keep first offset for ungetch */
	if ((parser.data.length = fread(parser.data.buffer + 1, 1,
					sizeof(parser.data.buffer) - 1,
					parser.fp) + 1) <= 1) {
	    ch = EOF;
	    parser.data.offset = 1;
	}
	else {
	    ch = parser.data.buffer[1];
	    parser.data.offset = 2;
	}
    }
    if ((parser.newline = ch == '\n'))
	++parser.line;

    return (ch);
}

static int
getch_noeof(void)
{
    int		ch = getch();

    if (ch == EOF)
	error("unexpected end of file");

    return (ch);
}

static int
ungetch(int ch)
{
    if ((parser.newline = ch == '\n'))
	--parser.line;

    if (parser.data.offset)
	parser.data.buffer[--parser.data.offset] = ch;
    else
	/* overwrite */
	parser.data.buffer[0] = ch;

    return (ch);
}

static int
skipws(void)
{
    int		ch;

    for (ch = getch();; ch = getch()) {
	switch (ch) {
	    case '/':
		ch = skipct();
		break;
	    case '#':
		ch = skipcp();
		break;
	}
	switch (ch) {
	    case ' ': case '\f': case '\r': case '\t':
		break;
	    default:
		return (ch);
	}
    }
}

static int
skipnl(void)
{
    int		ch;

    for (ch = getch();; ch = getch()) {
	switch (ch) {
	    case '/':
		ch = skipct();
		break;
	    case '#':
		ch = skipcp();
		break;
	}
	switch (ch) {
	    case ' ': case '\f': case '\n': case '\r': case '\t':
		break;
		/* handle as newline */
	    case ';':
		break;
	    default:
		return (ch);
	}
    }
}

static int
skipct(void)
{
    int		ch;

    ch = getch();
    switch (ch) {
	case '/':
	    for (ch = getch(); ch != '\n' && ch != EOF; ch = getch())
		;
	    return (ch);
	case '*':
	    for (; ch != '/';) {
		while (getch_noeof() != '*')
		    ;
		while ((ch = getch_noeof()) == '*')
		    ;
	    }
	    return (getch());
	default:
	    ungetch(ch);
	    return ('/');
    }
}

static int
skipcp(void)
{
    int		ch;

    for (ch = getch(); ch != '\n' && ch != EOF; ch = getch()) {
	switch (ch) {
	    case '0' ... '9':
		if ((number(ch)) == tok_int)
		    parser.line = parser.value.i - 1;
		break;
	    case '"':
		string();
		if (parser.offset >= (int)sizeof(parser.name)) {
		    strncpy(parser.name, parser.string, sizeof(parser.name));
		    parser.name[sizeof(parser.name) - 1] = '\0';
		}
		else
		    strcpy(parser.name, parser.string);
		break;
	    default:
		break;
	}
    }

    return (ch);
}

static jit_word_t
get_int(skip_t skip)
{
    switch (primary(skip)) {
	case tok_int:
	    break;
	case tok_pointer:
	    parser.type = type_l;
	    parser.value.i = (jit_word_t)parser.value.p;
	    break;
	default:
	    error("expecting integer");
    }

    return (parser.value.i);
}

static jit_uword_t
get_uint(skip_t skip)
{
    switch (primary(skip)) {
	case tok_char:		case tok_int:
	    break;
	case tok_pointer:
	    parser.type = type_l;
	    parser.value.ui = (jit_uword_t)parser.value.p;
	    break;
	default:
	    error("expecting integer");
    }

    return (parser.value.ui);
}

static double
get_float(skip_t skip)
{
    switch (primary(skip)) {
	case tok_char:
	case tok_int:
	    parser.type = type_d;
	    parser.value.d = parser.value.i;
	    break;
	case tok_float:
	    break;
	default:
	    error("expecting float");
    }

    return (parser.value.d);
}

/* Workaround gcc not converting unordered values from double to
 * float (as done in other architectures) on s390 */
static float
make_float(double d)
{
    /* This is an workaround to a bug in Hercules s390 emulator,
     * and at least HP-UX ia64 not have these */
#if defined(HAVE_ISNAN) && defined(HAVE_ISINF)
    if (isnan(d))	return ( 0.0f/0.0f);
    if (isinf(d)) {
	if (d > 0.0)	return ( 1.0f/0.0f);
	else		return (-1.0f/0.0f);
    }
#endif
    return ((float)d);
}

static void *
get_pointer(skip_t skip)
{
    label_t	*label;
    token_t	 token = primary(skip);

    switch (token) {
	case tok_symbol:
	    label = get_label_by_name(parser.string);
	    if (label == NULL)
		error("bad identifier %s", parser.string);
	    switch (label->kind) {
		case label_kind_data:
		case label_kind_code:
		    break;
		case label_kind_code_forward:
		    /* as expression arguments */
		    error("forward references not implemented");
		    break;
		case label_kind_dynamic:
		    break;
	    }
	    parser.type = type_p;
	    return (parser.value.p = label->value);
	case tok_int:
	    parser.type = type_p;
	    return (parser.value.p = (void *)parser.value.ui);
	case tok_pointer:
	    return (parser.value.p);
	default:		error("bad pointer");
    }
}

static label_t *
get_label(skip_t skip)
{
    label_t	*label;
    int		 ch = skipws();

    switch (ch) {
	case '@':
	    (void)dynamic();
	    break;
	case 'a' ... 'z': case 'A' ... 'Z': case '_':
	    (void)identifier(ch);
	    break;
	default:
	    error("expecting label/immediate");
    }
    if ((label = get_label_by_name(parser.string)) == NULL)
	label = new_label(label_kind_code_forward,
			  parser.string, jit_forward());

    return (label);
}

static token_t
regname(void)
{
    jit_word_t	num;
    int		check = 1, ch = getch();

    switch (ch) {
	case 'r':
	    parser.regtype = type_l;
	    switch (ch = getch()) {
		case '0':	parser.regval = JIT_R0;		break;
		case '1':	parser.regval = JIT_R1;		break;
		case '2':	parser.regval = JIT_R2;		break;
		case '(':
		    num = get_int(skip_none);
		    if (num < 0 || num >= JIT_R_NUM)		goto fail;
		    parser.regval = JIT_R(num);
		    if (getch() != ')')				goto fail;
		    check = 0;
		    break;
		default:					goto fail;
	    }
	    break;
	case 'v':
	    parser.regtype = type_l;
	    switch (ch = getch()) {
		case '0':	parser.regval = JIT_V0;		break;
		case '1':	parser.regval = JIT_V1;		break;
		case '2':	parser.regval = JIT_V2;		break;
		default:					goto fail;
		case '(':
		    num = get_int(skip_none);
		    if (num < 0 || num >= JIT_V_NUM)		goto fail;
		    parser.regval = JIT_V(num);
		    if (getch() != ')')				goto fail;
		    check = 0;
		    break;
	    }
	    break;
	case 'f':
	    parser.regtype = type_d;
	    switch (ch = getch()) {
		case '0':	parser.regval = JIT_F0;		break;
		case '1':	parser.regval = JIT_F1;		break;
		case '2':	parser.regval = JIT_F2;		break;
		case '3':	parser.regval = JIT_F3;		break;
		case '4':	parser.regval = JIT_F4;		break;
		case '5':	parser.regval = JIT_F5;		break;
		case 'p':
		    parser.regtype = type_l;	/* oops */
		    parser.regval = JIT_FP;			break;
		case '(':
		    num = get_int(skip_none);
		    if (num < 0 || num >= JIT_F_NUM)		goto fail;
		    parser.regval = JIT_F(num);
		    if (getch() != ')')				goto fail;
		    check = 0;
		    break;
		default:					goto fail;
	    }
	    break;
	default:
	fail:
	    error("bad register");
    }
    if (check) {
	ch = getch();
	if ((ch >= 'a' && ch <= 'z') ||
	    (ch >= 'A' && ch <= 'Z') ||
	    (ch >= '0' && ch <= '9') ||
	    ch == '_')
	    goto fail;
	ungetch(ch);
    }

    return (tok_register);
}

static token_t
identifier(int ch)
{
    parser.string[0] = ch;
    for (parser.offset = 1;;) {
	switch ((ch = getch())) {
	    case 'a' ... 'z': case 'A' ... 'Z': case '0' ... '9' :  case '_':
		if (parser.offset + 1 >= MAX_IDENTIFIER) {
		    parser.string[parser.offset] = '\0';
		    error("bad identifier %s", parser.string);
		}
		parser.string[parser.offset++] = ch;
		break;
	    default:
		parser.string[parser.offset] = '\0';
		ungetch(ch);
		return (tok_symbol);
	}
    }
}

static void
get_data(type_t type)
{
    int		 ch;
    token_t	 token;
    char	*test = data;

    for (;;) {
	switch (type) {
	    case type_c:
		switch (token = primary(skip_ws)) {
		    case tok_char: case tok_int:
			check_data(sizeof(signed char));
			*(signed char *)(data + data_offset) = parser.value.i;
			data_offset += sizeof(char);
			break;
		    case tok_string:
			check_data(parser.offset);
			memcpy(data + data_offset, parser.string,
			       parser.offset);
			data_offset += parser.offset;
			break;
		    case tok_newline:
		    case tok_semicollon:
			if (test == data)	error("syntax error");
			return;
		    default:			error("bad initializer");
		}
		break;
	    case type_s:
		check_data(sizeof(signed short));
		*(signed short *)(data + data_offset) = get_int(skip_ws);
		data_offset += sizeof(short);
		break;
	    case type_i:
		check_data(sizeof(signed int));
		*(signed int *)(data + data_offset) = get_int(skip_ws);
		data_offset += sizeof(int);
		break;
	    case type_l:
		check_data(sizeof(jit_word_t));
		*(jit_word_t *)(data + data_offset) = get_int(skip_ws);
		data_offset += sizeof(jit_word_t);
		break;
	    case type_f:
		check_data(sizeof(float));
		*(float *)(data + data_offset) = get_float(skip_ws);
		data_offset += sizeof(float);
		break;
	    case type_d:
		check_data(sizeof(double));
		*(double *)(data + data_offset) = get_float(skip_ws);
		data_offset += sizeof(double);
		break;
	    case type_p:
		/* FIXME **patch if realloc** */
		check_data(sizeof(void*));
		*(void **)(data + data_offset) = get_pointer(skip_ws);
		data_offset += sizeof(void*);
		break;
	    default:
		abort();
	}
	ch = skipws();
	if (ch == '\n' || ch == ';' || ch == EOF)
	    break;
	ungetch(ch);
    }
}

static void
dot(void)
{
    int		ch;
    size_t	offset, length;

    switch (ch = getch_noeof()) {
	case '$':
	    /* use .$(expression) for non side effects expression */
	    (void)expression();
	    return;
	case 'a' ... 'z': case 'A' ... 'Z': case '_':
	    (void)identifier(ch);
	    break;
	default:
	    ungetch(ch);
	    if (skipws() != '$')
		error("expecting symbol");
	    /* allow spaces before an expression */
	    (void)expression();
	    return;
    }
    if (parser.string[1] == '\0') {
	switch (parser.string[0]) {
	    case 'c':	get_data(type_c);	break;
	    case 's':	get_data(type_s);	break;
	    case 'i':	get_data(type_i);	break;
	    case 'l':	get_data(type_l);	break;
	    case 'f':	get_data(type_f);	break;
	    case 'd':	get_data(type_d);	break;
	    case 'p':	get_data(type_p);	break;
	    default:	error("bad type .%c", parser.string[0]);
	}
    }
    else if (strcmp(parser.string, "data") == 0) {
	if (parser.parsing != PARSING_NONE)
	    error(".data must be specified once and be the first section");
	parser.parsing = PARSING_DATA;
	data_length = get_int(skip_ws);
	data = (char *)xcalloc(1, data_length);
    }
    else if (strcmp(parser.string, "code") == 0) {
	if (parser.parsing != PARSING_NONE &&
	    parser.parsing != PARSING_DATA)
	    error(".code must be specified once only");
	parser.parsing = PARSING_CODE;
    }
    else if (strcmp(parser.string, "align") == 0) {
	length = get_int(skip_ws);
	if (parser.parsing != PARSING_DATA)
	    error(".align must be in .data");
	if (length > 1 && length <= 4096 && !(length & (length - 1))) {
	    offset = data_offset;
	    offset += length - ((offset + length) % length);
	    check_data(offset - data_offset);
	    data_offset = offset;
	}
	else
	    error("bad .align %ld (must be a power of 2, >= 2 && <= 4096)",
		  (jit_word_t)length);
    }
    else if (strcmp(parser.string, "size") == 0) {
	length = get_int(skip_ws);
	if (parser.parsing != PARSING_DATA)
	    error(".size must be in .data");
	check_data(length);
	data_offset += length;
    }
    else if (strcmp(parser.string, "disasm") == 0)
	flag_disasm = 1;
    else
	error("unknown command .%s", parser.string);
}

static token_t
number(int ch)
{
    char	buffer[1024], *endptr;
    int		integer = 1, offset = 0, neg = 0, e = 0, d = 0, base = 10;

    for (;; ch = getch()) {
	switch (ch) {
	    case '-':
		if (offset == 0) {
		    neg = 1;
		    continue;
		}
		if (offset && buffer[offset - 1] != 'e') {
		    ungetch(ch);
		    goto done;
		}
		break;
	    case '+':
		if (offset == 0)
		    continue;
		if (offset && buffer[offset - 1] != 'e') {
		    ungetch(ch);
		    goto done;
		}
		break;
	    case '.':
		if (d)
		    goto fail;
		d = 1;
		base = 10;
		integer = 0;
		break;
	    case '0':
		if (offset == 0 && base == 10) {
		    base = 8;
		    continue;
		}
		break;
	    case 'b':
		if (offset == 0 && base == 8) {
		    base = 2;
		    continue;
		}
		if (base != 16)
		    goto fail;
		break;
	    case '1':
		break;
	    case '2' ... '7':
		if (base < 8)
		    goto fail;
		break;
	    case '8': case '9':
		if (base < 10)
		    goto fail;
		break;
	    case 'x':
		if (offset == 0 && base == 8) {
		    base = 16;
		    continue;
		}
		goto fail;
	    case 'a': case 'c': case 'd': case 'f':
		if (base < 16)
		    goto fail;
		break;
	    case 'e':
		if (e)
		    goto fail;
		if (base != 16) {
		    e = 1;
		    base = 10;
		    integer = 0;
		}
		break;
	    case '_': case 'g' ... 'w': case 'y': case 'z': case 'A' ... 'Z':
	    fail:
		buffer[offset++] = '\0';
		error("bad constant %s", buffer);
	    default:
		ungetch(ch);
		goto done;
	}
	if (offset + 1 >= (int)sizeof(buffer))
	    goto fail;
	buffer[offset++] = ch;
    }
done:
    /* check for literal 0 */
    if (offset == 0 && base == 8)	buffer[offset++] = '0';
    buffer[offset] = '\0';
    if (integer) {
#if _WIN32
#  define STRTOUL	strtoull
#else
#  define STRTOUL	strtoul
#endif
	parser.value.ui = STRTOUL(buffer, &endptr, base);
	parser.type = type_l;
	if (neg)
	    parser.value.i = -parser.value.i;
    }
    else {
	parser.type = type_d;
	parser.value.d = strtod(buffer, &endptr);
	if (neg)
	    parser.value.d = -parser.value.d;
    }
    if (*endptr)
	goto fail;

    return (integer ? tok_int : tok_float);
}

static int
escape(int ch)
{
    switch (ch) {
	case 'a':	ch = '\a';	break;
	case 'b':	ch = '\b';	break;
	case 'f':	ch = '\f';	break;
	case 'n':	ch = '\n';	break;
	case 'r':	ch = '\r';	break;
	case 't':	ch = '\t';	break;
	case 'v':	ch = '\v';	break;
	default:			break;
    }

    return (ch);
}

static token_t
string(void)
{
    int		ch, esc = 0;

    for (parser.offset = 0;;) {
	switch (ch = getch_noeof()) {
	    case '\\':
		if (esc)		goto append;
		esc = 1;
		break;
	    case '"':
		if (!esc) {
		    parser.string[parser.offset++] = '\0';
		    parser.value.p = parser.string;
		    parser.type = type_p;
		    return (tok_string);
		}
		/* FALLTHROUGH */
	    default:
	    append:
		if (esc) {
		    ch = escape(ch);
		    esc = 0;
		}
		if (parser.offset + 1 >= parser.length) {
		    parser.length += 4096;
		    parser.string = (char *)xrealloc(parser.string,
						     parser.length);
		}
		parser.string[parser.offset++] = ch;
		break;
	}
    }
}

static token_t
character(void)
{
    int		ch, esc = 0;

    if ((ch = getch_noeof()) == '\\') {
	esc = 1;
	ch = getch();
    }
    if (getch_noeof() != '\'')
	error("bad single byte char");
    if (esc)
	ch = escape(ch);
    parser.type = type_l;
    parser.value.i = ch & 0xff;

    return (tok_char);
}

static token_t
dynamic(void)
{
    label_t	*label;
    void	*value;
    char	*string;
    (void)identifier('@');
    if ((label = get_label_by_name(parser.string)) == NULL) {
#if __CYGWIN__ ||_WIN32
	/* FIXME kludge to pass varargs test case, otherwise,
	 * will not print/scan float values */
	if (strcmp(parser.string + 1, "sprintf") == 0)
	    value = sprintf;
	else if (strcmp(parser.string + 1, "sscanf") == 0)
	    value = sscanf;
	else
#endif
	{
	    value = dlsym(DL_HANDLE, parser.string + 1);
	    if ((string = dlerror()))
		error("%s", string);
	}
	label = new_label(label_kind_dynamic, parser.string, value);
    }
    parser.type = type_p;
    parser.value.p = label->value;

    return (tok_pointer);
}

static void
expression_prim(void)
{
    int		 ch;
    token_t	 token;
    label_t	*label;
    symbol_t	*symbol;

    if (parser.putback) {
	parser.expr = parser.putback;
	parser.putback = (expr_t)0;
	return;
    }
    switch (ch = skipws()) {
	case '!':
	    if ((ch = getch_noeof()) == '=')	parser.expr = expr_ne;
	    else {
		ungetch(ch);			parser.expr = expr_not;
	    }
	    break;
	case '~':				parser.expr = expr_com;
	    break;
	case '*':
	    if ((ch = getch_noeof()) == '=')	parser.expr = expr_mulset;
	    else {
		ungetch(ch);			parser.expr = expr_mul;
	    }
	    break;
	case '/':
	    if ((ch = getch_noeof()) == '=')	parser.expr = expr_divset;
	    else {
		ungetch(ch);			parser.expr = expr_div;
	    }
	    break;
	case '%':
	    if ((ch = getch_noeof()) == '=')	parser.expr = expr_remset;
	    else {
		ungetch(ch);			parser.expr = expr_rem;
	    }
	    break;
	case '+':
	    switch (ch = getch_noeof()) {
		case '+':			parser.expr = expr_inc;
		    break;
		case '=':			parser.expr = expr_addset;
		    break;
		default:	ungetch(ch);	parser.expr = expr_add;
		    break;
	    }
	    break;
	case '-':
	    switch (ch = getch_noeof()) {
		case '-':			parser.expr = expr_dec;
		    break;
		case '=':			parser.expr = expr_subset;
		    break;
		default:	ungetch(ch);	parser.expr = expr_sub;
		    break;
	    }
	    break;
	case '<':
	    switch (ch = getch_noeof()) {
		case '=':			parser.expr = expr_le;
		    break;
		case '<':			ch = getch_noeof();
		    if (ch == '=')		parser.expr = expr_lshset;
		    else {
			ungetch(ch);		parser.expr = expr_lsh;
		    }
		    break;
		default:	ungetch(ch);	parser.expr = expr_lt;
		    break;
	    }
	    break;
	case '>':
	    switch (ch = getch_noeof()) {
		case '=':			parser.expr = expr_ge;
		    break;
		case '>':			ch = getch_noeof();
		    if (ch == '=')		parser.expr = expr_rshset;
		    else {
			ungetch(ch);		parser.expr = expr_rsh;
		    }
		    break;
		default:	ungetch(ch);	parser.expr = expr_gt;
		    break;
	    }
	    break;
	case '&':
	    switch (ch = getch_noeof()) {
		case '=':			parser.expr = expr_andset;
		    break;
		case '&':			parser.expr = expr_andand;
		    break;
		default:	ungetch(ch);	parser.expr = expr_and;
		    break;
	    }
	    break;
	case '|':
	    switch (ch = getch_noeof()) {
		case '=':			parser.expr = expr_orset;
		    break;
		case '|':			parser.expr = expr_oror;
		    break;
		default:	ungetch(ch);	parser.expr = expr_or;
		    break;
	    }
	    break;
	case '^':
	    if ((ch = getch_noeof()) == '=')	parser.expr = expr_xorset;
	    else {
		ungetch(ch);			parser.expr = expr_xor;
	    }
	    break;
	case '=':
	    if ((ch = getch_noeof()) == '=')	parser.expr = expr_eq;
	    else {
		ungetch(ch);			parser.expr = expr_set;
	    }
	    break;
	case '(':				parser.expr = expr_lparen;
	    break;
	case ')':				parser.expr = expr_rparen;
	    break;
	case '0' ... '9':
	    token = number(ch);
	    parser.expr = token == tok_int ? expr_int : expr_float;
	    break;
	case '@':
	    (void)dynamic();
	    parser.expr = expr_pointer;
	    break;
	case '$':
	    identifier('$');
	    /* no support for nested expressions */
	    if (parser.string[0] == '\0')
		error("syntax error");
	    parser.expr = expr_symbol;
	    if ((symbol = get_symbol_by_name(parser.string)) != NULL) {
		parser.type = symbol->type;
		parser.value = symbol->value;
	    }
	    else
		/* only create symbol on assignment */
		parser.type = type_none;
	    break;
	case 'a' ... 'z': case 'A' ... 'Z': case '_':
	    identifier(ch);
	    if ((label = get_label_by_name(parser.string))) {
		if (label->kind == label_kind_code_forward)
		    error("forward value for %s not supported",
			  parser.string);
		parser.expr = expr_pointer;
		parser.type = type_p;
		parser.value.p = label->value;
	    }
	    else
		error("invalid identifier %s", parser.string);
	    break;
	case '\'':
	    character();
	    parser.expr = expr_int;
	    break;
	case '"':
	    /* not smart enough to put it in data and/or relocate it, etc */
	    error("must declare strings as data");
	default:
	    error("syntax error");
    }
}

static void
expression_inc(int pre)
{
    symbol_t	*symbol;

    if (pre) {
	expression_prim();
	if (parser.expr != expr_symbol)
	    error("syntax error");
    }
    if ((symbol = get_symbol_by_name(parser.string)) == NULL) {
	if (!parser.short_circuit)
	    error("undefined symbol %s", symbol->name);
    }
    if (!parser.short_circuit) {
	parser.type = symbol->type;
	if (!pre)
	    parser.value = symbol->value;
	switch (symbol->type) {
	    case type_l:
		++symbol->value.i;
		break;
	    case type_d:
		/* should really be an error */
		symbol->value.d += 1.0;
		break;
	    default:
		++parser.value.cp;
		break;
	}
	if (pre)
	    parser.value = symbol->value;
    }
    expression_prim();
}

static void
expression_dec(int pre)
{
    symbol_t	*symbol;

    if (pre) {
	expression_prim();
	if (parser.expr != expr_symbol)
	    error("syntax error");
    }
    if ((symbol = get_symbol_by_name(parser.string)) == NULL) {
	if (!parser.short_circuit)
	    error("undefined symbol %s", symbol->name);
    }
    if (!parser.short_circuit) {
	parser.type = symbol->type;
	if (!pre)
	    parser.value = symbol->value;
	switch (symbol->type) {
	    case type_l:
		--symbol->value.i;
		break;
	    case type_d:
		/* should really be an error */
		symbol->value.d -= 1.0;
		break;
	    default:
		--parser.value.cp;
		break;
	}
	if (pre)
	    parser.value = symbol->value;
    }
    expression_prim();
}

static void
expression_unary(void)
{
    symbol_t	*symbol;
    char	 buffer[256];

    expression_prim();
    switch (parser.expr) {
	case expr_add:
	    expression_unary();
	    switch (parser.type) {
		case type_l:
		case type_d:
		    break;
		default:
		    error("syntax error");
	    }
	    break;
	case expr_sub:
	    expression_unary();
	    switch (parser.type) {
		case type_l:
		    parser.value.i = -parser.value.i;
		    break;
		case type_d:
		    parser.value.d = -parser.value.d;
		    break;
		default:
		    error("syntax error");
	    }
	    break;
	case expr_inc:
	    expression_inc(1);
	    break;
	case expr_dec:
	    expression_dec(1);
	    break;
	case expr_not:
	    expression_unary();
	    switch (parser.type) {
		case type_l:
		    parser.value.i = !parser.value.i;
		    break;
		case type_d:
		    parser.value.i = parser.value.d != 0;
		    break;
		case type_p:
		    parser.value.i = parser.value.p != NULL;
		    break;
		default:
		    error("syntax error");
	    }
	    parser.type = type_l;
	    break;
	case expr_com:
	    expression_unary();
	    if (parser.type != type_l)
		error("syntax error");
	    parser.value.i = ~parser.value.i;
	    break;
	case expr_lparen:
	    expression_cond();
	    if (parser.expr != expr_rparen)
		error("syntax error");
	    expression_prim();
	    break;
	case expr_symbol:
	    strcpy(buffer, parser.string);
	    expression_prim();
	    switch (parser.expr) {
		case expr_set:
		    if ((symbol = get_symbol_by_name(buffer)) == NULL) {
			if (!parser.short_circuit)
			    symbol = new_symbol(buffer);
		    }
		    expression_cond();
		set:
		    if (!parser.short_circuit) {
			if (symbol == NULL)
			    error("syntax error");
			symbol->type = parser.type;
			symbol->value = parser.value;
		    }
		    break;
		case expr_mulset:		parser.putback = expr_mul;
		    goto check;
		case expr_divset:		parser.putback = expr_div;
		    goto check;
		case expr_remset:		parser.putback = expr_rem;
		    goto check;
		case expr_addset:		parser.putback = expr_add;
		    goto check;
		case expr_subset:		parser.putback = expr_sub;
		    goto check;
		case expr_lshset:		parser.putback = expr_lsh;
		    goto check;
		case expr_rshset:		parser.putback = expr_rsh;
		    goto check;
		case expr_andset:		parser.putback = expr_and;
		    goto check;
		case expr_orset:		parser.putback = expr_or;
		    goto check;
		case expr_xorset:		parser.putback = expr_xor;
		check:
		    if ((symbol = get_symbol_by_name(buffer)) == NULL) {
			if (!parser.short_circuit)
			    error("undefined symbol %s", buffer);
			parser.type = type_l;
			parser.value.i = 1;
		    }
		    switch (parser.putback) {
			case expr_mul:	case expr_div:	case expr_rem:
			    expression_mul();
			    break;
			case expr_add:	case expr_sub:
			    expression_add();
			    break;
			case expr_lsh:	case expr_rsh:
			    expression_shift();
			    break;
			case expr_and:	case expr_or:  case expr_xor:
			    expression_bit();
			    break;
			default:
			    abort();
		    }
		    goto set;
		case expr_inc:
		    expression_inc(0);
		    break;
		case expr_dec:
		    expression_dec(0);
		    break;
		default:
		    break;
	    }
	    break;
	case expr_int:
	case expr_float:
	case expr_pointer:
	    /* make next token available */
	    expression_prim();
	default:
	    break;
    }
}

static void
expression_mul(void)
{
    type_t	type;
    value_t	value;

    expression_unary();
    switch (parser.type) {
	case type_l:	case type_d:	case type_p:	break;
	default:					return;
    }
    for (;;) {
	switch (parser.expr) {
	    case expr_mul:
		type = parser.type, value = parser.value;
		expression_unary();
		switch (parser.type) {
		    case type_l:
			if (type == type_l)
			    value.i *= parser.value.i;
			else
			    value.d *= parser.value.i;
			break;
		    case type_d:
			if (type == type_l) {
			    type = type_d;
			    value.d = value.i;
			}
			value.d *= parser.value.d;
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type, parser.value = value;
		break;
	    case expr_div:
		type = parser.type, value = parser.value;
		expression_unary();
		switch (parser.type) {
		    case type_l:
			if (type == type_l) {
			    if (parser.value.i == 0)
				error("divide by zero");
			    value.i /= parser.value.i;
			}
			else
			    value.d /= parser.value.i;
			break;
		    case type_d:
			if (type == type_l) {
			    type = type_d;
			    value.d = value.i;
			}
			value.d /= parser.value.d;
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type, parser.value = value;
		break;
	    case expr_rem:
		type = parser.type, value = parser.value;
		expression_unary();
		switch (parser.type) {
		    case type_l:
			if (type == type_l) {
			    if (parser.value.i == 0)
				error("divide by zero");
			    value.i %= parser.value.i;
			}
			else
			    error("invalid operand");
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type, parser.value = value;
		break;
	    default:
		return;
	}
    }
}

static void
expression_add(void)
{
    type_t	type;
    value_t	value;

    expression_mul();
    switch (parser.type) {
	case type_l:	case type_d:	case type_p:	break;
	default:					return;
    }
    for (;;) {
	switch (parser.expr) {
	    case expr_add:
		type = parser.type, value = parser.value;
		expression_mul();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i += parser.value.i;
				break;
			    case type_d:
				value.d += parser.value.i;
				break;
			    default:
				value.cp += parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				type = type_d;
				value.d = value.i;
				break;
			    case type_d:
				break;
			    default:
				error("invalid operand");
			}
			value.d += parser.value.d;
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				type = type_p;
				value.cp = value.i + parser.value.cp;
				break;
			    default:
				error("invalid operand");
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type, parser.value = value;
		break;
	    case expr_sub:
		type = parser.type, value = parser.value;
		expression_mul();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i -= parser.value.i;
				break;
			    case type_d:
				value.d -= parser.value.i;
				break;
			    default:
				value.cp -= parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				type = type_d;
				value.d = value.i;
				break;
			    case type_d:
				break;
			    default:
				error("invalid operand");
			}
			value.d -= parser.value.d;
			break;
		    case type_p:
			switch (type) {
			    case type_p:
				type = type_l;
				value.i = value.cp - parser.value.cp;
				break;
			    default:
				error("invalid operand");
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type, parser.value = value;
		break;
	    default:
		return;
	}
    }
}

static void
expression_shift(void)
{
    jit_word_t	value;
    expression_add();

    switch (parser.type) {
	case type_l:	case type_d:	case type_p:	break;
	default:					return;
    }
    for (;;) {
	switch (parser.expr) {
	    case expr_lsh:
		value = parser.value.i;
		if (parser.type != type_l)
		    error("invalid operand");
		expression_add();
		if (parser.type != type_l)
		    error("invalid operand");
		value <<= parser.value.i;
		parser.value.i = value;
		break;
	    case expr_rsh:
		value = parser.value.i;
		if (parser.type != type_l)
		    error("invalid operand");
		expression_add();
		if (parser.type != type_l)
		    error("invalid operand");
		value >>= parser.value.i;
		parser.value.i = value;
		break;
	    default:
		return;
	}
    }
}

static void
expression_bit(void)
{
    jit_word_t	i;

    expression_shift();
    switch (parser.type) {
	case type_l:	case type_d:	case type_p:	break;
	default:					return;
    }
    for (;;) {
	switch (parser.expr) {
	    case expr_and:
		if (parser.type != type_l)
		    error("invalid operand");
		i = parser.value.i;
		expression_shift();
		if (parser.type != type_l)
		    error("invalid operand");
		i &= parser.value.i;
		parser.value.i = i;
		break;
	    case expr_or:
		if (parser.type != type_l)
		    error("invalid operand");
		i = parser.value.i;
		expression_shift();
		if (parser.type != type_l)
		    error("invalid operand");
		i |= parser.value.i;
		parser.value.i = i;
		break;
	    case expr_xor:
		if (parser.type != type_l)
		    error("invalid operand");
		i = parser.value.i;
		expression_shift();
		if (parser.type != type_l)
		    error("invalid operand");
		i ^= parser.value.i;
		parser.value.i = i;
		break;
	    default:
		return;
	}
    }
}

static void
expression_rel(void)
{
    type_t	type;
    value_t	value;

    expression_bit();
    switch (parser.type) {
	case type_l:	case type_d:	case type_p:	break;
	default:					return;
    }
    for (;;) {
	switch (parser.expr) {
	    case expr_lt:
		type = parser.type, value = parser.value;
		expression_bit();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i < parser.value.i;
				break;
			    case type_d:
				value.i = value.d < parser.value.i;
				break;
			    default:
				value.i = (jit_word_t)value.p < parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i < parser.value.d;
				break;
			    case type_d:
				value.i = value.d < parser.value.d;
				break;
			    default:
				error("invalid operand");
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i < (jit_word_t)parser.value.p;
				break;
			    case type_d:
				error("invalid operand");
			    default:
				value.i = (jit_word_t)value.p < (jit_word_t)parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value = value;
		break;
	    case expr_le:
		type = parser.type, value = parser.value;
		expression_bit();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i <= parser.value.i;
				break;
			    case type_d:
				value.i = value.d <= parser.value.i;
				break;
			    default:
				value.i = (jit_word_t)value.p <= parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i <= parser.value.d;
				break;
			    case type_d:
				value.i = value.d <= parser.value.d;
				break;
			    default:
				value.i = (jit_word_t)value.p <= parser.value.d;
				break;
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i <= (jit_word_t)parser.value.p;
				break;
			    case type_d:
				error("invalid operand");
			    default:
				value.i = (jit_word_t)value.p <= (jit_word_t)parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value = value;
		break;
	    case expr_eq:
		type = parser.type, value = parser.value;
		expression_bit();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i == parser.value.i;
				break;
			    case type_d:
				value.i = value.d == parser.value.i;
				break;
			    default:
				value.i = (jit_word_t)value.p == parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i == parser.value.d;
				break;
			    case type_d:
				value.i = value.d == parser.value.d;
				break;
			    default:
				error("invalid operand");
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i == (jit_word_t)parser.value.p;
				break;
			    case type_d:
				error("invalid operand");
			    default:
				value.i = value.p == parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value = value;
		break;
	    case expr_ge:
		type = parser.type, value = parser.value;
		expression_bit();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i >= parser.value.i;
				break;
			    case type_d:
				value.i = value.d >= parser.value.i;
				break;
			    default:
				value.i = (jit_word_t)value.p >= parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i >= parser.value.d;
				break;
			    case type_d:
				value.i = value.d >= parser.value.d;
				break;
			    default:
				error("invalid operand");
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i >= (jit_word_t)parser.value.p;
				break;
			    case type_d:
				error("invalid operand");
			    default:
				value.i = (jit_word_t)value.p >= (jit_word_t)parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value = value;
		break;
	    case expr_gt:
		type = parser.type, value = parser.value;
		expression_bit();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i > parser.value.i;
				break;
			    case type_d:
				value.i = value.d > parser.value.i;
				break;
			    default:
				value.i = (jit_word_t)value.p > parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i > parser.value.d;
				break;
			    case type_d:
				value.i = value.d > parser.value.d;
				break;
			    default:
				error("invalid operand");
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i > (jit_word_t)parser.value.p;
				break;
			    case type_d:
				error("invalid operand");
			    default:
				value.i = (jit_word_t)value.p > (jit_word_t)parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value = value;
		break;
	    case expr_ne:
		type = parser.type, value = parser.value;
		expression_bit();
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i != parser.value.i;
				break;
			    case type_d:
				value.i = value.d != parser.value.i;
				break;
			    default:
				value.i = (jit_word_t)value.p != parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i != parser.value.d;
				break;
			    case type_d:
				value.i = value.d != parser.value.d;
				break;
			    default:
				error("invalid operand");
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i != (jit_word_t)parser.value.p;
				break;
			    case type_d:
				error("invalid operand");
			    default:
				value.i = value.p != parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value = value;
		break;
	    default:
		return;
	}
    }
}

static void
expression_cond(void)
{
    type_t	type;
    value_t	value;
    int		short_circuit;

    expression_rel();
    switch (parser.type) {
	case type_l:	case type_d:	case type_p:	break;
	default:					return;
    }
    for (;;) {
	switch (parser.expr) {
	    case expr_andand:
		type = parser.type, value = parser.value;
		switch (type) {
		    case type_l:
			short_circuit = value.i == 0;
			break;
		    case type_d:
			short_circuit = value.d == 0.0;
			break;
		    default:
			short_circuit = value.p == NULL;
			break;
		}
		parser.short_circuit += short_circuit;
		expression_rel();
		parser.short_circuit -= short_circuit;
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i && parser.value.i;
				break;
			    case type_d:
				value.i = value.d && parser.value.i;
				break;
			    default:
				value.i = value.p && parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i && parser.value.d;
				break;
			    case type_d:
				value.i = value.d && parser.value.d;
				break;
			    default:
				value.i = value.p && parser.value.d;
				break;
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i && parser.value.p;
				break;
			    case type_d:
				value.i = value.d && parser.value.p;
				break;
			    default:
				value.i = value.p && parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value.i = value.i;
		break;
	    case expr_oror:
		type = parser.type, value = parser.value;
		switch (type) {
		    case type_l:
			short_circuit = value.i != 0;
			break;
		    case type_d:
			short_circuit = value.d != 0.0;
			break;
		    default:
			short_circuit = value.p != NULL;
			break;
		}
		parser.short_circuit += short_circuit;
		expression_rel();
		parser.short_circuit -= short_circuit;
		switch (parser.type) {
		    case type_l:
			switch (type) {
			    case type_l:
				value.i = value.i || parser.value.i;
				break;
			    case type_d:
				value.i = value.d || parser.value.i;
				break;
			    default:
				value.i = value.p || parser.value.i;
				break;
			}
			break;
		    case type_d:
			switch (type) {
			    case type_l:
				value.i = value.i || parser.value.d;
				break;
			    case type_d:
				value.i = value.d || parser.value.d;
				break;
			    default:
				value.i = value.p || parser.value.d;
				break;
			}
			break;
		    case type_p:
			switch (type) {
			    case type_l:
				value.i = value.i || parser.value.p;
				break;
			    case type_d:
				value.i = value.d || parser.value.p;
				break;
			    default:
				value.i = value.p || parser.value.p;
				break;
			}
			break;
		    default:
			error("invalid operand");
		}
		parser.type = type_l, parser.value.i = value.i;
		break;
	    default:
		return;
	}
    }
}

static token_t
expression(void)
{
    symbol_t	*symbol;

    (void)identifier('$');
    if (parser.string[1] == '\0') {
	if (getch_noeof() != '(')
	    error("bad symbol or expression");
	parser.type = type_none;
	expression_cond();
	if (parser.expr != expr_rparen)
	    error("bad expression");
	switch (parser.type) {
	    case type_l:
		return (tok_int);
	    case type_d:
		return (tok_float);
	    case type_p:
		return (tok_pointer);
	    default:
		error("bad expression");
	}
    }
    else if ((symbol = get_symbol_by_name(parser.string))) {
	switch (parser.type = symbol->type) {
	    case type_l:
		parser.value.i = symbol->value.i;
		return (tok_int);
	    case type_d:
		parser.value.d = symbol->value.d;
		return (tok_float);
	    default:
		parser.value.p = symbol->value.p;
		return (tok_pointer);
	}
    }
    else
	error("undefined symbol %s", parser.string);
}

static token_t
primary(skip_t skip)
{
    int		ch;

    switch (skip) {
	case skip_none:	ch = getch();	break;
	case skip_ws:	ch = skipws();	break;
	case skip_nl:	ch = skipnl();	break;
	default:			abort();
    }
    switch (ch) {
	case '%':
	    return (regname());
	case 'a' ... 'z': case 'A' ... 'Z': case '_':
	    return (identifier(ch));
	case '0' ... '9': case '+': case '-':
	    return (number(ch));
	case '.':
	    return (tok_dot);
	case '"':
	    return (string());
	case '\'':
	    return (character());
	case '@':
	    return (dynamic());
	case '$':
	    return (expression());
	case EOF:
	    return (tok_eof);
	case '\n':
	    return (tok_newline);
	case ';':
	    return (tok_semicollon);
	default:
	    error("syntax error");
    }
}

static void
parse(void)
{
    int		 ch;
    token_t	 token;
    instr_t	*instr;
    label_t	*label;
    void	*value;

    for (;;) {
	switch (token = primary(skip_nl)) {
	    case tok_symbol:
		ch = getch_noeof();
		if (ch == ':') {
		    if ((label = get_label_by_name(parser.string))) {
			if (label->kind == label_kind_code_forward) {
			    label->kind = label_kind_code;
			    jit_link(label->value);
			    jit_note(parser.name, parser.line);
			}
			else
			    error("label %s: redefined", parser.string);
		    }
		    else {
			if (parser.parsing == PARSING_DATA) {
			    value = data + data_offset;
			    label = new_label(label_kind_data,
					      parser.string, value);
			}
			else if (parser.parsing == PARSING_CODE) {
			    value = jit_label();
			    jit_note(parser.name, parser.line);
			    label = new_label(label_kind_code,
					      parser.string, value);
			}
			else
			    error("label not in .code or .data");
		    }
		    break;
		}
		ungetch(ch);
		if ((instr =
		     (instr_t *)get_hash(instrs, parser.string)) == NULL)
		    error("unhandled symbol %s", parser.string);
		if (parser.parsing != PARSING_CODE)
		    error(".code must be specified before instructions");
		(*instr->function)();
		break;
	    case tok_dot:
		dot();
		break;
	    case tok_eof:
		return;
	    default:
		error("syntax error");
	}
    }
}

static int
execute(int argc, char *argv[])
{
    int		 result;
    label_t	*label;
    function_t	 function;
    patch_t	*patch, *next;

    for (patch = patches; patch; patch = next) {
	next = patch->next;
	label = patch->label;
	if (label->kind == label_kind_code_forward)
	    error("undefined label %s", label->name);
	switch (patch->kind) {
	    case patch_kind_jmp:
	    case patch_kind_mov:
	    case patch_kind_call:
		jit_patch_at(patch->value, label->value);
		break;
	    default:
		abort();
	}
	free(patch);
	patch = next;
    }

    if (flag_data == 0) {
	jit_realize();
	jit_set_data(NULL, 0, JIT_DISABLE_DATA | JIT_DISABLE_NOTE);
    }

    function = jit_emit();
    if (flag_verbose > 1 || flag_disasm) {
	jit_print();
	fprintf(stdout, "  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    }
    if (flag_verbose > 0 || flag_disasm) {
	jit_disassemble();
	fprintf(stdout, "  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    }

    jit_clear_state();
    if (flag_disasm)
	result = 0;
    else
	result = (*function)(argc, argv);
    jit_destroy_state();

    return (result);
}

static void *
xmalloc(size_t size)
{
    void	*pointer = malloc(size);

    if (pointer == NULL)
	error("out of memory");

    return (pointer);
}

static void *
xrealloc(void *pointer, size_t size)
{
    pointer = realloc(pointer, size);

    if (pointer == NULL)
	error("out of memory");

    return (pointer);
}

static void *
xcalloc(size_t nmemb, size_t size)
{
    void	*pointer = calloc(nmemb, size);

    if (pointer == NULL)
	error("out of memory");

    return (pointer);
}

static label_t *
new_label(label_kind_t kind, char *name, void *value)
{
    label_t	*label;

    label = (label_t *)xmalloc(sizeof(label_t));
    label->kind = kind;
    label->name = strdup(name);
    label->value = value;
    put_hash(labels, (entry_t *)label);
    label_offset++;
    return (label);
}

static patch_t *
new_patch(patch_kind_t kind, label_t *label, void *value)
{
    patch_t	*patch = (patch_t *)xmalloc(sizeof(patch_t));
    patch->kind = kind;
    patch->label = label;
    patch->value = value;
    patch->next = patches;
    patches = patch;

    return (patch);
}

static int
bcmp_symbols(const void *left, const void *right)
{
    return (strcmp((char *)left, (*(symbol_t **)right)->name));
}

static int
qcmp_symbols(const void *left, const void *right)
{
    return (strcmp((*(symbol_t **)left)->name, (*(symbol_t **)right)->name));
}

static symbol_t *
new_symbol(char *name)
{
    symbol_t	*symbol;

    if ((symbol_offset & 15) == 0) {
	if ((symbol_length += 16) == 16)
	    symbols = (symbol_t **)xmalloc(sizeof(symbol_t *) *
					   symbol_length);
	else
	    symbols = (symbol_t **)xrealloc(symbols, sizeof(symbol_t *) *
					    symbol_length);
    }
    symbol = (symbol_t *)xmalloc(sizeof(symbol_t));
    symbol->name = strdup(name);
    symbols[symbol_offset++] = symbol;
    qsort(symbols, symbol_offset, sizeof(symbol_t *), qcmp_symbols);

    return (symbol);
}

static symbol_t *
get_symbol_by_name(char *name)
{
    symbol_t	**symbol_pointer;

    if (symbols == NULL)
	return (NULL);
    symbol_pointer = (symbol_t **)bsearch(name, symbols, symbol_offset,
					  sizeof(symbol_t *), bcmp_symbols);

    return (symbol_pointer ? *symbol_pointer : NULL);
}

static hash_t *
new_hash(void)
{
    hash_t	*hash;

    hash = (hash_t *)xmalloc(sizeof(hash_t));
    hash->count = 0;
    hash->entries = (entry_t **)xcalloc(hash->size = 32, sizeof(void *));

    return (hash);
}

static int
hash_string(char *name)
{
    char	*ptr;
    int		 key;

    for (key = 0, ptr = name; *ptr; ptr++)
	key = (key << (key & 1)) ^ *ptr;

    return (key);
}

static void
put_hash(hash_t *hash, entry_t *entry)
{
    entry_t	*prev, *ptr;
    int		 key = hash_string(entry->name) & (hash->size - 1);

    for (prev = ptr = hash->entries[key]; ptr; prev = ptr, ptr = ptr->next) {
	if (strcmp(entry->name, ptr->name) == 0)
	    error("duplicated entry %s", entry->name);
    }
    if (prev == NULL)
	hash->entries[key] = entry;
    else
	prev->next = entry;
    entry->next = NULL;
    ++hash->count;
    if (hash->count > hash->size * 0.75)
	rehash(hash);
}

static entry_t *
get_hash(hash_t *hash, char *name)
{
    entry_t	*entry;
    int		 key = hash_string(name) & (hash->size - 1);

    for (entry = hash->entries[key]; entry; entry = entry->next) {
	if (strcmp(entry->name, name) == 0)
	    return (entry);
    }
    return (NULL);
}

static void
rehash(hash_t *hash)
{
    int		 i, size, key;
    entry_t	*entry, *next, **entries;

    entries = (entry_t **)xcalloc(size = hash->size * 2, sizeof(void *));
    for (i = 0; i < hash->size; i++) {
	for (entry = hash->entries[i]; entry; entry = next) {
	    next = entry->next;
	    key = hash_string(entry->name) & (size - 1);
	    entry->next = entries[key];
	    entries[key] = entry;
	}
    }
    free(hash->entries);
    hash->entries = entries;
    hash->size = size;
}

static void
usage(void)
{
#if HAVE_GETOPT_LONG_ONLY
    fprintf(stderr, "\
Usage: %s [jit assembler options] file [jit program options]\n\
Jit assembler options:\n\
  -help                    Display this information\n\
  -v[0-3]                  Verbose output level\n\
  -d                       Do not use a data buffer\n\
  -D<macro>[=<val>]        Preprocessor options\n"
#  if defined(__i386__) && __WORDSIZE == 32
"  -mx87=1                  Force using x87 when sse2 available\n"
#  endif
#  if defined(__i386__) || defined(__x86_64__)
"  -msse4_1=0               Do not use sse4_1 instructions when available\n"
#  endif
#  if defined(__arm__)
"  -mcpu=<val>              Force cpu version (4, 5, 6 or 7)\n\
  -mthumb[=0|1]            Enable or disable thumb\n\
  -mvfp=<val>              Set vpf version (0 to disable)\n\
  -mneon[=0|1]             Enable or disable neon\n"
#  endif
	    , progname);
#else
    fprintf(stderr, "\
Usage: %s [jit assembler options] file [jit program options]\n\
Jit assembler options:\n\
  -h                       Display this information\n\
  -v                       Verbose output level\n\
  -D<macro>[=<val>]        Preprocessor options\n", progname);
#endif
    finish_jit();
    exit(1);
}

int
main(int argc, char *argv[])
{
#if HAVE_GETOPT_LONG_ONLY
    static const char	*short_options = "dv::";
    static struct option long_options[] = {
	{ "help",		0, 0, 'h' },
	{ "data",		2, 0, 'd' },
#  if defined(__i386__) && __WORDSIZE == 32
	{ "mx87",		2, 0, '7' },
#  endif
#  if defined(__i386__) || defined(__x86_64__)
	{ "msse4_1",		2, 0, '4' },
#  endif
#  if defined(__arm__)
	{ "mcpu",		2, 0, 'c' },
	{ "mthumb",		2, 0, 't' },
	{ "mvfp",		2, 0, 'f' },
	{ "mneon",		2, 0, 'n' },
#  endif
	{ 0,			0, 0, 0   }
    };
#else
#endif	/* HAVE_GETOPT_LONG_ONLY */
    int			 offset;
    char		*endptr;
    int			 opt_index;
    int			 opt_short;
    char		 cmdline[8192];

#if defined(__CYGWIN__)
    /* Cause a compile warning about redefinition without dllimport
     * attribute, *but* cause correct linkage if liblightning.a is
     * linked to binutils (that happens to have an internal
     * getopt* implementation and an apparently conflicting
     * optind global variable) */
    extern int		 optind;
    optind = 1;
#endif

    progname = argv[0];

    init_jit(progname);

#if defined(__sgi)
    DL_HANDLE = dlopen(NULL, RTLD_LAZY);
#endif

    flag_data = 1;
#if HAVE_GETOPT_LONG_ONLY
    for (;;) {
	if ((opt_short = getopt_long_only(argc, argv, short_options,
					  long_options, &opt_index)) < 0)
	    break;
	switch (opt_short) {
	    case 'h':
	    default:
		usage();
		break;
	    case 'v':
		if (optarg) {
		    flag_verbose = strtol(optarg, &endptr, 10);
		    if (*endptr || flag_verbose < 0)
			usage();
		}
		else
		    flag_verbose = 1;
		break;
	    case 'd':
		flag_data = 0;
		break;
#if defined(__i386__) && __WORDSIZE == 32
	    case '7':
		if (optarg) {
		    if (strcmp(optarg, "") == 0 || strcmp(optarg, "1") == 0)
			jit_cpu.sse2 = 0;
		    else if (strcmp(optarg, "0"))
			usage();
		}
		else
		    jit_cpu.sse2 = 0;
		break;
#endif
#if defined(__i386__) || defined(__x86_64__)
	    case '4':
		if (optarg) {
		    if (strcmp(optarg, "0") == 0)
			jit_cpu.sse4_2 = 0;
		    else if (strcmp(optarg, "1"))
			usage();
		}
		break;
#endif
#if defined(__arm__)
	    case 'c':
		if (optarg) {
		    offset = strtol(optarg, &endptr, 10);
		    if (*endptr || offset < 0)
			usage();
		    if (offset < jit_cpu.version)
			jit_cpu.version = offset;
		}
		break;
	    case 't':
		if (optarg) {
		    if (strcmp(optarg, "0") == 0)
			jit_cpu.thumb = 0;
		    else if (strcmp(optarg, "1") && strcmp(optarg, "2"))
			usage();
		}
		break;
	    case 'f':
#  if !defined(__ARM_PCS_VFP)
		/* Do not allow overrinding hard float abi */
		if (optarg) {
		    offset = strtol(optarg, &endptr, 10);
		    if (*endptr || offset < 0)
			usage();
		    if (offset < jit_cpu.vfp)
			jit_cpu.vfp = offset;
		}
#  endif
		break;
	    case 'n':
		if (optarg) {
		    if (strcmp(optarg, "0") == 0)
			jit_cpu.neon = 0;
		    else if (strcmp(optarg, "1"))
			usage();
		}
		break;
#endif
	}
    }
#else
    while ((opt_short = getopt(argc, argv, "hvd")) >= 0) {
	if (opt_short == 'v')
	    ++flag_verbose;
	else if (opt_short == 'd')
	    flag_data = 0;
	else
	    usage();
    }
#endif

    opt_index = optind;
#if defined(__hpux)
    /* Workaround */
    if (opt_index < argc && argv[opt_index][0] == '-')
	++opt_index;
#endif
    if (opt_index < 0 || opt_index >= argc)
	usage();
    if (strcmp(argv[opt_index], "-") == 0)
	strcpy(parser.name, "<stdin>");
    else {
	if ((endptr = strrchr(argv[opt_index], '/')) == NULL)
	    endptr = argv[opt_index];
	else
	    ++endptr;
	strncpy(parser.name, endptr, sizeof(parser.name));
	parser.name[sizeof(parser.name) - 1] = '\0';
    }
#if __clang__
#  define cc "clang"
#else
#  define cc "gcc"
#endif
    opt_short = snprintf(cmdline, sizeof(cmdline), cc " -E -x c %s", argv[opt_index]);
    for (++opt_index; opt_index < argc; opt_index++) {
	if (argv[opt_index][0] == '-')
	    opt_short += snprintf(cmdline + opt_short,
				  sizeof(cmdline) - opt_short,
				  " %s", argv[opt_index]);
	else {
	    --opt_index;
	    break;
	}
    }
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__WORDSIZE=%d", __WORDSIZE);
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__LITTLE_ENDIAN=%d", __LITTLE_ENDIAN);
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__BIG_ENDIAN=%d", __BIG_ENDIAN);
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__BYTE_ORDER=%d", __BYTE_ORDER);
#if defined(__i386__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__i386__=1");
#endif
#if defined(__x86_64__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__x86_64__=1");
#endif
#if defined(__mips__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__mips__=1");
#endif
#if defined(__arm__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__arm__=1");
#endif
#if defined(__powerpc__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__ppc__=1");
#endif
#if defined(__sparc__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__sparc__=1");
#endif
#if defined(__ia64__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__ia64__=1");
#endif
#if defined(__hppa__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__hppa__=1");
#endif
#if defined(_AIX)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D_AIX=1");
#endif
#if defined(__sgi__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__sgi__=1");
#endif
#if defined(__aarch64__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__aarch64__=1");
#endif
#if defined(__s390__) || defined(__s390x__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__s390__=1");
#endif
#if defined(__alpha__)
    opt_short += snprintf(cmdline + opt_short,
			  sizeof(cmdline) - opt_short,
			  " -D__alpha__=1");
#endif
    if ((parser.fp = popen(cmdline, "r")) == NULL)
	error("cannot execute %s", cmdline);

    parser.line = 1;
    parser.string = (char *)xmalloc(parser.length = 4096);

#if defined(__linux__) && (defined(__i386__) || defined(__x86_64__))
    /*	double precision		0x200
     *	round nearest			0x000
     *	invalid operation mask		0x001
     *	denormalized operand mask	0x002
     *	zero divide mask		0x004
     *	precision (inexact) mask	0x020
     */
    {
	fpu_control_t fpu_control = 0x027f;
	_FPU_SETCW(fpu_control);
    }
#endif

    _jit = jit_new_state();

    instrs = new_hash();
    for (offset = 0;
	 offset < (int)(sizeof(instr_vector) / sizeof(instr_vector[0]));
	 offset++)
	put_hash(instrs, (entry_t *)(instr_vector + offset));

    labels = new_hash();

    parse();
    pclose(parser.fp);
    parser.fp = NULL;

    for (opt_short = 0; opt_index < argc; opt_short++, opt_index++)
	argv[opt_short] = argv[opt_index];
    argv[opt_short] = NULL;
    argc = opt_short;
    execute(argc, argv);

    finish_jit();

    return (0);
}
