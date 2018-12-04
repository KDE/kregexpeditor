/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/
%{
#ifndef YYENABLE_NLS
#  define YYENABLE_NLS 0
#endif

#ifndef YYLTYPE_IS_TRIVIAL
#  define YYLTYPE_IS_TRIVIAL 0
#endif

  #include <KLocalizedString>
  #include <QMessageBox>

  
  #include <stdlib.h>

  #include "regexp.h"
  #include "textregexp.h"
  #include "textrangeregexp.h"
  #include "repeatregexp.h"
  #include "lookaheadregexp.h"
  #include "concregexp.h"
  #include "altnregexp.h"
  #include "positionregexp.h"
  #include "dotregexp.h"
  #include "compoundregexp.h"
  #include "gen_qregexpparser.hh"

#define YY_EXTRA_TYPE struct parse_context *

  extern int yylex( YYSTYPE* yylval, yyscan_t scanner );
  extern void scannerInit( yyscan_t* scanner, struct parse_context* context, const QString& qstr );
  extern void scannerDestroy( yyscan_t scanner );
  int yyerror( yyscan_t scanner, struct parse_context* context, const char * );
  void setParseResult( RegExp*, struct parse_context* );
  RegExp* parseQtRegExp( const QString &qstr, bool* ok );
%}

%code requires {
#include "qregexpparsercommon.h"
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif
}

%union {
  struct {
    int min;
    int max;
  } range;
  int backRef;
  RegExp* regexp;
  char ch;
}

%token  TOK_Dot
%token  TOK_Dollar
%token  TOK_Carat
%token  TOK_MagicLeftParent
%token  TOK_PosLookAhead
%token  TOK_NegLookAhead
%token  TOK_LeftParen
%token  TOK_RightParent
%token  TOK_Bar 
%token  TOK_Quantifier
%token  TOK_BackRef
%token  TOK_CharClass
%token  TOK_Char
%token  TOK_EscapeChar
%token  TOK_PosWordChar
%token  TOK_PosNonWordChar

%start regexp

%pure-parser

%param { yyscan_t scanner }
%parse-param { struct parse_context *context }

%%

empty           : /* nothing */ ;

regexp :  expression { setParseResult( $<regexp>1, context ) ; }
       | empty { setParseResult( new ConcRegExp( false ), context ); }
       ;

expression : expression TOK_Bar term {
               if ( dynamic_cast<AltnRegExp*>( $<regexp>1 ) ) {
                 $<regexp>$ = $<regexp>1;
               }
               else {
                 $<regexp>$ = new AltnRegExp( false );
                 dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( $<regexp>1 );
               }
               dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( $<regexp>3 );
             }
           | term { $<regexp>$ = $<regexp>1; }
           | expression TOK_Bar { 
               if ( dynamic_cast<AltnRegExp*>( $<regexp>1 ) ) {
                 $<regexp>$ = $<regexp>1;
               }
               else {
                 $<regexp>$ = new AltnRegExp( false );
                 dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( $<regexp>1 );
               }
               dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( new TextRegExp( false, QString::fromLatin1("") ) );
             }
           | TOK_Bar term {  
               $<regexp>$ = new AltnRegExp( false );
               dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( new TextRegExp( false, QString::fromLatin1("") ) );
               dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( $<regexp>2 );
             }
           | TOK_Bar { $<regexp>$ = new AltnRegExp( false ); }
           ;

term : term factor {
       RegExp* last = dynamic_cast<ConcRegExp*>( $<regexp>1 )->lastRegExp();
       TextRegExp *reg1, *reg2;
       
       if ( last && ( reg1 = dynamic_cast<TextRegExp*>( last ) ) &&
            ( reg2 = dynamic_cast<TextRegExp*>( $<regexp>2 ) ) ) {
         reg1->append( reg2->text() );
         delete reg2;
       }
       else {
         dynamic_cast<ConcRegExp*>($<regexp>$)->addRegExp( $<regexp>2 );
       }
       $<regexp>$ = $<regexp>1;
     }
     | factor { 
         ConcRegExp* reg = new ConcRegExp( false );
         reg->addRegExp( $<regexp>1 );
         $<regexp>$ = reg;
       }
     ;

factor : atom TOK_Quantifier {
           $<regexp>$ = new RepeatRegExp( false, $<range>2.min, $<range>2.max, $<regexp>1 );
         }
       | atom { $<regexp>$ = $<regexp>1; }
       ;

atom : TOK_LeftParen expression TOK_RightParent { 
         $<regexp>$ = $<regexp>2; 
       }
     | TOK_MagicLeftParent expression TOK_RightParent { $<regexp>$ = $<regexp>2; }
     | TOK_PosLookAhead expression TOK_RightParent { 
         $<regexp>$ = new LookAheadRegExp( false, LookAheadRegExp::POSITIVE, $<regexp>2 );
       }
     | TOK_NegLookAhead expression TOK_RightParent {
         $<regexp>$ = new LookAheadRegExp( false, LookAheadRegExp::NEGATIVE, $<regexp>2 );
       }
     | TOK_CharClass { $<regexp>$ = $<regexp>1; }
     | char { $<regexp>$ = $<regexp>1; }
     | TOK_Dollar { $<regexp>$ = new PositionRegExp( false, PositionRegExp::ENDLINE ); }
     | TOK_Carat  { $<regexp>$ = new PositionRegExp( false, PositionRegExp::BEGLINE ); }
     | TOK_Dot { $<regexp>$ = new DotRegExp( false ); }
     | TOK_BackRef { 
        QString match = QString::fromLocal8Bit("\\%1").arg( $<backRef>1 );
        $<regexp>$ = new TextRegExp( false, match );
        context->backrefs << match;
      }
     | TOK_PosWordChar { $<regexp>$ = new PositionRegExp( false, PositionRegExp::WORDBOUNDARY ); }
     | TOK_PosNonWordChar { $<regexp>$ = new PositionRegExp( false, PositionRegExp::NONWORDBOUNDARY ); }
     ;

char : TOK_Char { 
       if ( $<ch>1 == '{' || $<ch>1 == '}' || $<ch>1 == '[' || $<ch>1 == ']' || $<ch>1 == '\\' ) {
          yyerror( scanner, context, "illigal character - needs escaping" );
       }
       $<regexp>$ = new TextRegExp( false, QString::fromLocal8Bit("%1").arg($<ch>1)); 
     }
     | TOK_EscapeChar { $<regexp>$ = new TextRegExp( false, QString::fromLocal8Bit("%1").arg($<ch>1)); }
     ; 

%%

RegExp* parseQtRegExp( const QString &qstr, bool* ok ) {
  struct parse_context context;
  yyscan_t scanner;
  context.result = 0;
  context.nerrs = 0;
  scannerInit( &scanner, &context, qstr );
  (void) yyparse( scanner, &context );
  scannerDestroy( scanner );
  foreach ( const QString &match, context.backrefs ) {
    QMessageBox::information(nullptr,
                             i18n("Back reference regular expressions not supported"),
                             i18n("<p>Back reference regular expressions are not supported.</p>"
                                    "<p><tt>\\1</tt>, <tt>\\2</tt>, ... are <i>back references</i>, meaning they "
                                    "refer to previous matches. "
                                    "Unfortunately this is not supported in the current version of this editor.</p>"
                                    "<p>In the graphical area the text <b>%1</b> has been inserted. This is however "
                                    "just a workaround to ensure that the application handles the regexp at all. "
                                    "Therefore, as soon as you edit the regular expression in the graphical area, "
                                    "the back reference will be replaced by matching the text <b>%2</b> literally.</p>",
                                    match, match ));
  }
  *ok = ( context.nerrs == 0 );
  return context.result;
}

void setParseResult( RegExp* regexp, struct parse_context* context ) {
  context->result = regexp;
}

int yyerror( yyscan_t, struct parse_context* context, const char * ) {
  context->nerrs++;
  return 0;
}
