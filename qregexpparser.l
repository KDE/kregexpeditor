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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
%option noyywrap

%{ 
  #include <qstring.h>
  #include "textrangeregexp.h"
  #include "gen_qregexpparser.hh"
#ifdef QT_ONLY
  #include "compat.h"
#endif
  void parseRange( char* txt, int* min, int* max );  
  RegExp* parseCharClass( char* match );
%}
  
Escape   \\.
BackRef  \\[1-9][0-9]*
CharClass \[^?\]?[^]]*\]
Range \{[0-9]*(,[0-9]*)?\}
HexChar \\x[0-9a-fA-F]{1,4}
OctChar \\0[0-7]{1,4}
SpecialEsc \\[afnrtv]
%%
"\\b"      return TOK_PosWordChar;
"\\B"      return TOK_PosNonWordChar;
"\\d"      { 
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->setDigit( true );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
"\\D"      { 
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->setNonDigit( true );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
"\\s"      { 
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->setSpace( true );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
"\\S"      { 
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->setNonSpace( true );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
"\\w"      { 
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->setWordChar( true );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
"\\W"      { 
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->setNonWordChar( true );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
{SpecialEsc} {
               TextRangeRegExp* regexp = new TextRangeRegExp( false );
               regexp->addCharacter( QString::fromLocal8Bit( yytext ) );
               qregexplval.regexp = regexp;
               return TOK_CharClass;
             }

{HexChar}  {
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->addCharacter( QString::fromLocal8Bit(yytext) );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
{OctChar}  {
             TextRangeRegExp* regexp = new TextRangeRegExp( false ); 
             regexp->addCharacter( QString::fromLocal8Bit(yytext) );
             qregexplval.regexp = regexp;
             return TOK_CharClass;
           }
"."        return TOK_Dot;
"$"        return TOK_Dollar;
"^"        return TOK_Carat;
"(?:"      return TOK_MagicLeftParent;
"(?="      return TOK_PosLookAhead;
"(?!"      return TOK_NegLookAhead;
"("        return TOK_LeftParen;
")"        return TOK_RightParent;
"|"        return TOK_Bar;
"*"        { qregexplval.range.min = 0; qregexplval.range.max=-1; return TOK_Quantifier; }
"?"        { qregexplval.range.min = 0; qregexplval.range.max=1;  return TOK_Quantifier; }
"+"        { qregexplval.range.min = 1; qregexplval.range.max=-1; return TOK_Quantifier; }
{Range}     { parseRange( yytext, &qregexplval.range.min, &qregexplval.range.max ); return TOK_Quantifier; }  
{CharClass} { qregexplval.regexp = parseCharClass(yytext); return TOK_CharClass; }
{BackRef}   { qregexplval.backRef = atoi( yytext+1 ); return TOK_BackRef; }
{Escape}    { qregexplval.ch = yytext[1]; return TOK_EscapeChar; }
.           { qregexplval.ch = yytext[0]; return TOK_Char; }

%%

void setParseData( QString qstr ) {  
  const char* cstr;
  if ( qstr.isNull() ) 
    cstr = "";
  else
    cstr = qstr.latin1();
  yy_switch_to_buffer( yy_scan_string( cstr ) );
}

/**
   This function parses a range in a form similar to "{3,4}", "{,7}"
   etc. and returns the value in the integers pointed to by min and max.
*/
void parseRange( char* txt, int* min, int* max ) 
{

  /*  
      case  txt   min  max
       1    {}     0   -1
       2    {,}    0   -1
       3    {5}    5    5
       4    {5,}   5   -1
       5    {,7}   0    7
       6    {5,7}  5    7
  */    
  char c;
  int i = 1;
  int minimum=0, maximum=0;
  int minFound=0, maxFound=0, commaFound = 0;

  while ( (c = txt[i++]) != ',' && c != '}') {
    minimum = minimum*10+ c-'0';
    minFound=1;
  }

  if ( c == ',' )
    commaFound = 1;

  if ( c != '}' ) {
    while ( (c = txt[i++]) != '}') {
      maximum = maximum*10+ c-'0';
      maxFound = 1;
    }
  }
  
  *min = minimum;
  if ( maxFound ) 
    *max = maximum;   /* case 5,6 */
  else if ( !minFound )
    *max = -1;        /* case 1,2 */
  else if ( commaFound )
    *max = -1;        /* case 4 */
  else
    *max = minimum;   /* case 3 */
}


/** 
    This function parses a character range like "[^ab1-4]".
*/
RegExp* parseCharClass( char* match )
{
  TextRangeRegExp* res = new TextRangeRegExp( false );
  QString txt = QString::fromLocal8Bit( match );
  txt = txt.mid(1,txt.length()-2);
  
  unsigned int i = 0;
  QChar ch = txt.at(i++);
  QString pendingChar;
  QString thisChar;
  bool charPending = false;
  bool rangePending = false;
  bool flushPending = false;
  
  if ( ch == QChar('^') ) {
    res->setNegate( true );
    ch = txt.at(i++);
  }

  do {
    // If a character is pending, and the next char is '-' then we are
    // possible looking at a range.
    if ( ch == QChar('-') && charPending ) {
      rangePending = true;
      ch = txt.at(i++);
      continue;
    }

    // If we have a pending character, but do not also have a pending
    // range, then the pending character was not part of a range, and
    // should therefore just be added as a single character.
    if ( charPending && !rangePending ) {
      res->addCharacter( pendingChar );
      charPending = false;
    }

    if ( ch == QChar('\\') ) {
      // Handle the cases where an escape character is specified.
      ch = txt.at(i++);
      
      if ( ch == QChar('a') || ch == QChar('f') || ch == QChar('n') || ch == QChar('r') || ch == QChar('t') || ch == QChar('v') ) {
        // These are just seen as normal characters.
        thisChar = QString::fromLocal8Bit("\\") + ch;
      }
      else if ( ch == QChar('d') ) {  
        // The following characters represent character groups. If any of
        // these are seen in a range, then the range is ignored, thus [a-\s]
        // matches an 'a', a '-', and a space (\s means space).
        res->setDigit( true );
        flushPending = true;
      }
      else if ( ch == QChar('D') ) {
        res->setNonDigit( true );
        flushPending = true;
      }
      else if ( ch == QChar('s') ) {
        res->setSpace( true );
        flushPending = true;
      }
      else if ( ch == QChar('S') ) {
        res->setNonSpace( true );
        flushPending = true;
      }
      else if ( ch == QChar('w') ) {
        res->setWordChar( true );
        flushPending = true;
      }
      else if ( ch == QChar('W') ) {
        res->setNonWordChar( true );
        flushPending = true;
      }
      else if ( ch == QChar('x') || ch == QChar('X') ) { 
        // This is a hexidecimal character: \xHHHH
        QString str;
        for ( int j=0; j<4; j++) {
          ch = txt.at(i++);
            if ( ch == 'a' || ch == 'A' || ch == 'b' || ch == 'B' || ch == 'c' || ch == 'C' || ch == 'd' || ch == 'D' || 
                 ch == 'e' || ch == 'E' || ch == 'f' || ch == 'F' ||
                 ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || 
                 ch == '8' || ch == '9' )
              str += ch;
            else
              i--;
        }
        thisChar = QString::fromLocal8Bit("\\x") + str;
      }
      else if ( ch == QChar('0') ) {
        // This is an octal character
        QString str;
        for ( int j=0; j<4; j++) {
          ch = txt.at(i++);
          if ( ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' )
            str += ch;
          else
            i--;
        }
        thisChar = QString::fromLocal8Bit("\\x") + str ;
      }
      else {
        // Anything else escaped just means the character itself.
        thisChar = ch;
      }
    }
    else {
      // A non escaped character.
      thisChar = ch;
    }
    
    // The characters \s,\S,\w,\W,\d or \D, can not be part of a range,
    // thus if they are meet in what looks like a range, then the
    // characters of the range is justed seen as normal non range
    // characters. thus [a-\s] matches an 'a', a '-', and a space (\s means
    // space). 
    if ( flushPending ) {
      if ( charPending ) 
        res->addCharacter( pendingChar );
      if ( rangePending ) 
        res->addCharacter( QString::fromLocal8Bit("-") );
      flushPending = false; 
      charPending = false;
      rangePending = false;
    }
    else {
      if ( rangePending ) {
        res->addRange( pendingChar, thisChar );
        charPending = false;
        rangePending = false;
      }
      else {
        pendingChar = thisChar;
        charPending = true;
      }
    }
    ch = txt.at(i++);
  }
  while ( ch != QChar(']') && i <= txt.length() );

  if ( charPending ) 
    res->addCharacter( pendingChar );
  if ( rangePending ) 
    res->addCharacter( QString::fromLocal8Bit("-") );

  return res;
}  
