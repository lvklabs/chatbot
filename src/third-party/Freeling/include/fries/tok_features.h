/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     SENSOR = 258,
     TARG = 259,
     COMPRGF = 260,
     INT = 261,
     CONJUNCT = 262,
     WORD = 263,
     FLAG = 264,
     COLON = 265,
     LBRACK = 266,
     RBRACK = 267,
     LPAREN = 268,
     RPAREN = 269,
     NEWLINE = 270,
     COMMA = 271,
     AMP = 272,
     BAR = 273,
     SMALLX = 274,
     BIGX = 275,
     EQUALS = 276,
     SEMICOLON = 277,
     FLOC = 278,
     FINC = 279,
     FMARK = 280,
     ERROR = 281,
     QUOTE = 282
   };
#endif
/* Tokens.  */
#define SENSOR 258
#define TARG 259
#define COMPRGF 260
#define INT 261
#define CONJUNCT 262
#define WORD 263
#define FLAG 264
#define COLON 265
#define LBRACK 266
#define RBRACK 267
#define LPAREN 268
#define RPAREN 269
#define NEWLINE 270
#define COMMA 271
#define AMP 272
#define BAR 273
#define SMALLX 274
#define BIGX 275
#define EQUALS 276
#define SEMICOLON 277
#define FLOC 278
#define FINC 279
#define FMARK 280
#define ERROR 281
#define QUOTE 282




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 90 "features.y"
{
  int i;      /* For returning numbers  */
  SubRGF *s;  /* For returning SubRGF*  */
  RGF *r;     /* For returning RGF*     */
  char *c;    /* For returning char*    */
}
/* Line 1489 of yacc.c.  */
#line 110 "tok_features.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

