%{
  #include <qstring.h>
  #include <stdlib.h>
  #include <kmessagebox.h>
  #include <klocale.h>

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

  extern int yylex();
  extern void setParseData( QString str );
  int yyerror (const char *);
  void setParseResult( RegExp* );
  RegExp* parseData();
  static RegExp* parseResult;
  static int _index;
%}

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

%%

empty           : /* nothing */ ;

regexp :  expression { setParseResult( $<regexp>1) ; }
       | empty { setParseResult( new ConcRegExp() ); }
       ;

expression : expression TOK_Bar term {
               if ( dynamic_cast<AltnRegExp*>( $<regexp>1 ) ) {
                 $<regexp>$ = $<regexp>1;
               }
               else {
                 $<regexp>$ = new AltnRegExp();
                 dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( $<regexp>1 );
               }
               dynamic_cast<AltnRegExp*>( $<regexp>$ )->addRegExp( $<regexp>3 );
             }
           | term { $<regexp>$ = $<regexp>1; }
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
         ConcRegExp* reg = new ConcRegExp();
         reg->addRegExp( $<regexp>1 );
         $<regexp>$ = reg;
       }
     ;

factor : atom TOK_Quantifier {
           $<regexp>$ = new RepeatRegExp( $<range>2.min, $<range>2.max, $<regexp>1 );
         }
       | atom { $<regexp>$ = $<regexp>1; }
       ;

atom : TOK_LeftParen expression TOK_RightParent { 
         $<regexp>$ = $<regexp>2; 
       }
     | TOK_MagicLeftParent expression TOK_RightParent { $<regexp>$ = $<regexp>2; }
     | TOK_PosLookAhead expression TOK_RightParent { 
         $<regexp>$ = new LookAheadRegExp( LookAheadRegExp::POSITIVE, $<regexp>2 );
       }
     | TOK_NegLookAhead expression TOK_RightParent {
         $<regexp>$ = new LookAheadRegExp( LookAheadRegExp::NEGATIVE, $<regexp>2 );
       }
     | TOK_CharClass { $<regexp>$ = $<regexp>1; }
     | char { $<regexp>$ = $<regexp>1; }
     | TOK_Dollar { $<regexp>$ = new PositionRegExp( PositionRegExp::ENDLINE ); }
     | TOK_Carat  { $<regexp>$ = new PositionRegExp( PositionRegExp::BEGLINE ); }
     | TOK_Dot { $<regexp>$ = new DotRegExp(); }
     | TOK_BackRef { 
        QString match = QString::fromLocal8Bit("\\%1").arg( $<backRef>1 );
        $<regexp>$ = new TextRegExp( match );
        KMessageBox::information(0,i18n("<qt>Back reference regular expressions are not supported.<p>"
                                        "<tt>\\1</tt>, <tt>\\2</tt>, ... are <i>back references</i>, meaning they refer to  "
                                        "previous matches. "
                                        "Unfortunately this is not supported in the current version of this editor.<p>"
                                        "In the graphical area the text <b>%1</b> has been inserted. This is however "
                                        "just a workaround to ensure that the application handles the regexp at all. "
                                        "Therefore, as soon as you edit the regular expression in the graphical area, "
                                        "the back reference will be replaced by matching the text <b>%2</b> literally.")
                                    .arg( match ).arg( match ),
                                 i18n("Back reference regular expressions not supported"), 
                                 QString::fromLocal8Bit("backReferenceNotSupported") );
      }
     | TOK_PosWordChar { $<regexp>$ = new PositionRegExp( PositionRegExp::WORDBOUNDARY ); }
     | TOK_PosNonWordChar { $<regexp>$ = new PositionRegExp( PositionRegExp::NONWORDBOUNDARY ); }
     ;

char : TOK_Char { $<regexp>$ = new TextRegExp( QString::fromLocal8Bit("%1").arg($<ch>1)); }
     | TOK_EscapeChar { $<regexp>$ = new TextRegExp( QString::fromLocal8Bit("%1").arg($<ch>1)); }
     ; 

%%

bool parse( QString qstr ) {
  _index = 0;
  parseResult = 0;
  setParseData( qstr );
  yyparse();
  return ( yynerrs == 0 );
}

void setParseResult( RegExp* regexp ) {
  parseResult = regexp;
}

RegExp* parseData() {
  return parseResult;
}

int yyerror(const char *) {
    yynerrs++;
  return 0;
}
