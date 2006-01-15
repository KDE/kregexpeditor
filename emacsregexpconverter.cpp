/*
 *  Copyright (c) 2002-2004 Jesper K. Pedersen <blackie@kde.org>
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

#include "emacsregexpconverter.h"
#include "regexp.h"
#include "altnregexp.h"
#include "concregexp.h"
#include "lookaheadregexp.h"
#include "textrangeregexp.h"
#include "textregexp.h"
#include "compoundregexp.h"
#include "dotregexp.h"
#include "positionregexp.h"
#include "repeatregexp.h"

#include <kmessagebox.h>
#include <klocale.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <Q3PtrList>
bool EmacsRegExpConverter::canParse()
{
    return false;
}

QString EmacsRegExpConverter::toString( AltnRegExp* regexp, bool markSelection )
{
    QString res;

	bool first = true;
    RegExpList list = regexp->children();
    for ( RegExpListIt it(list); *it; ++it ) {
		if ( !first ) {
			res += QString::fromLatin1("\\|");
		}
		first = false;
        res += toStr( *it, markSelection );
	}
	return res;

}

QString EmacsRegExpConverter::toString( ConcRegExp* regexp, bool markSelection )
{
	QString res;

    RegExpList list = regexp->children();
	for ( RegExpListIt it(list); *it; ++it ) {
        QString startPar = QString::fromLocal8Bit("");
        QString endPar = QString::fromLocal8Bit("");
        if ( (*it)->precedence() < regexp->precedence() ) {
            startPar = QString::fromLatin1( "\\(" );
            endPar = QString::fromLatin1( "\\)" );
        }

		res += startPar + toStr( *it, markSelection ) + endPar;
	}

	return res;

}

QString EmacsRegExpConverter::toString( LookAheadRegExp* /*regexp*/, bool /*markSelection*/ )
{
    static bool haveWarned = false;
    if ( ! haveWarned ) {
        KMessageBox::sorry( 0, i18n("Look ahead regular expressions not supported in Emacs style") );
        haveWarned = true;
    }

    return QString::null;
}

QString EmacsRegExpConverter::toString( TextRangeRegExp* regexp, bool /*markSelection*/ )
{
	QString txt;

	bool foundCarrot = false;
	bool foundDash = false;
	bool foundParenthesis = false;

	// print the single characters, but keep "^" as the very
	// last element of the characters.
    QStringList chars = regexp->chars();
	for (int i = 0; i< chars.count(); i++) {
		if ( chars.at(i).at(0) == QChar(']') ) {
			foundParenthesis = true;
		}
		else if ( chars.at(i).at(0) == QChar('-') ) {
			foundDash = true;
		}
		else if ( chars.at(i).at(0) == QChar('^') ) {
			foundCarrot = true;
		}
		else {
			txt.append( chars.at(i).at(0) );
		}
	}

	// Now insert the ranges.
    Q3PtrList<StringPair> ranges = regexp->range();
    for ( Q3PtrListIterator<StringPair> it(ranges); *it; ++it ) {
		txt.append((*it)->first()+ QString::fromLatin1("-")+ (*it)->second());
	}

	// Ok, its time to build each part of the regexp, here comes the rule:
	// if a ']' is one of the characters, then it must be the first one in the
	// list (after then opening '[' and eventually negation '^')
	// Next if a '-' is one of the characters, then it must come
	// finally if '^' is one of the characters, then it must not be the first
	// one!

	QString res = QString::fromLatin1("[");

	if ( regexp->negate() )
		res.append(QString::fromLatin1("^"));


	// a ']' must be the first character in teh range.
	if ( foundParenthesis ) {
		res.append(QString::fromLatin1("]"));
	}

	// a '-' must be the first character ( only coming after a ']')
	if ( foundDash ) {
		res.append(QString::fromLatin1("-"));
	}

	res += txt;

	// Insert equivalents to \s,\S,\d,\D,\w, and \W
    // non-digit, non-space, and non-word is not supported in Emacs style
    if ( regexp->digit() )
        res += QString::fromLocal8Bit("0-9");
    if ( regexp->space() )
        res += QString::fromLocal8Bit(" ") + QString( QChar( (char) 9 ) ); // Tab char
    if ( regexp->wordChar() )
        res += QString::fromLocal8Bit("a-zA-Z");

	if ( foundCarrot ) {
		res.append( QChar( '^' ) );
	}

	res.append(QString::fromLatin1("]"));

	return res;
}

QString EmacsRegExpConverter::toString( CompoundRegExp* regexp, bool markSelection )
{
    return  toStr( regexp->child(), markSelection );
}

QString EmacsRegExpConverter::toString( DotRegExp* /*regexp*/, bool /*markSelection*/ )
{
    return QString::fromLatin1( "." );
}

QString EmacsRegExpConverter::toString( PositionRegExp* regexp, bool /*markSelection*/ )
{
    static bool haveWarned = false;
    switch ( regexp->position()) {
	case PositionRegExp::BEGLINE:
		return QString::fromLatin1("^");
	case PositionRegExp::ENDLINE:
		return QString::fromLatin1("$");
	case PositionRegExp::WORDBOUNDARY:
    case PositionRegExp::NONWORDBOUNDARY:
        if ( ! haveWarned ) {
            KMessageBox::sorry( 0, i18n( "Word boundary and non word boundary is not supported in Emacs syntax" ) );
            haveWarned = true;
            return QString::fromLatin1("");
        }
    }
    return QString::fromLatin1("");
}

QString EmacsRegExpConverter::toString( RepeatRegExp* regexp, bool markSelection )
{
    RegExp* child = regexp->child();
    QString cText = toStr( child, markSelection );
    QString startPar;
    QString endPar;

    if ( child->precedence() < regexp->precedence() ) {
        startPar = QString::fromLatin1( "\\(" );
        endPar = QString::fromLatin1( "\\)" );
    }

    if (regexp->min() == 0 && regexp->max() == -1) {
        return startPar + cText +endPar + QString::fromLocal8Bit("*");
    }
    else if ( regexp->min() == 0 && regexp->max() == 1 ) {
        return startPar + cText + endPar + QString::fromLocal8Bit("?");
    }
    else if ( regexp->min() == 1 && regexp->max() == -1 ) {
        return startPar + cText + endPar + QString::fromLocal8Bit("+");
    }
    else {
        QString res = QString::fromLatin1("");
        for ( int i = 0; i < regexp->min(); ++i ) {
            res += QString::fromLatin1( "\\(" ) + cText + QString::fromLatin1( "\\)" );
        }
        if ( regexp->max() != -1 ) {
            for ( int i = regexp->min(); i < regexp->max(); ++i ) {
                res += QString::fromLatin1("\\(") + cText + QString::fromLatin1("\\)?");
            }
        }
        else
            res += QString::fromLatin1("+");

        return res;
    }
}

QString EmacsRegExpConverter::toString( TextRegExp* regexp, bool /*markSelection*/ )
{
    Q3ValueList<QChar> list;
    list << QChar('$')
         << QChar('^')
         << QChar('.')
         << QChar('*')
         << QChar('+')
         << QChar('?')
         << QChar('[')
         << QChar(']')
         << QChar('\\');

	QString res = escape( regexp->text(), list, QChar('\\') );
	return res;
}

QString EmacsRegExpConverter::name()
{
    return QString::fromLatin1( "Emacs" );
}

int EmacsRegExpConverter::features()
{
    return WordStart | WordEnd;
}
