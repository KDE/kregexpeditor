/* A Bison parser, made from qregexpparser.y, by GNU bison 1.75.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef BISON_GEN_QREGEXPPARSER_HH
# define BISON_GEN_QREGEXPPARSER_HH

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_Dot = 258,
     TOK_Dollar = 259,
     TOK_Carat = 260,
     TOK_MagicLeftParent = 261,
     TOK_PosLookAhead = 262,
     TOK_NegLookAhead = 263,
     TOK_LeftParen = 264,
     TOK_RightParent = 265,
     TOK_Bar = 266,
     TOK_Quantifier = 267,
     TOK_BackRef = 268,
     TOK_CharClass = 269,
     TOK_Char = 270,
     TOK_EscapeChar = 271,
     TOK_PosWordChar = 272,
     TOK_PosNonWordChar = 273
   };
#endif
#define TOK_Dot 258
#define TOK_Dollar 259
#define TOK_Carat 260
#define TOK_MagicLeftParent 261
#define TOK_PosLookAhead 262
#define TOK_NegLookAhead 263
#define TOK_LeftParen 264
#define TOK_RightParent 265
#define TOK_Bar 266
#define TOK_Quantifier 267
#define TOK_BackRef 268
#define TOK_CharClass 269
#define TOK_Char 270
#define TOK_EscapeChar 271
#define TOK_PosWordChar 272
#define TOK_PosNonWordChar 273




#ifndef YYSTYPE
#line 49 "qregexpparser.y"
typedef union {
  struct {
    int min;
    int max;
  } range;
  int backRef;
  RegExp* regexp;
  char ch;
} yystype;
/* Line 1281 of /usr/share/bison/yacc.c.  */
#line 86 "gen_qregexpparser.hh"
# define YYSTYPE yystype
#endif

extern YYSTYPE qregexplval;


#endif /* not BISON_GEN_QREGEXPPARSER_HH */

