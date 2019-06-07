
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER = 258,
     ERR = 259,
     EQUATE = 260,
     CART_METHOD = 261,
     CPU_METHOD = 262,
     TIA_METHOD = 263,
     FUNCTION = 264,
     LOG_OR = 265,
     LOG_AND = 266,
     LOG_NOT = 267,
     SHL = 268,
     SHR = 269,
     EQ = 270,
     NE = 271,
     LTE = 272,
     GTE = 273,
     DEREF = 274,
     UMINUS = 275
   };
#endif
/* Tokens.  */
#define NUMBER 258
#define ERR 259
#define EQUATE 260
#define CART_METHOD 261
#define CPU_METHOD 262
#define TIA_METHOD 263
#define FUNCTION 264
#define LOG_OR 265
#define LOG_AND 266
#define LOG_NOT 267
#define SHL 268
#define SHR 269
#define EQ 270
#define NE 271
#define LTE 272
#define GTE 273
#define DEREF 274
#define UMINUS 275




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 28 "stella.y"

	int val;
	char *equate;
	CARTDEBUG_INT_METHOD cartMethod;
	CPUDEBUG_INT_METHOD  cpuMethod;
	TIADEBUG_INT_METHOD  tiaMethod;
	Expression *exp;
	char *function;



/* Line 1676 of yacc.c  */
#line 104 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


