#include "emacsregexpconverter.h"
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
        if ( markSelection && !regexp->isSelected() && (*it)->isSelected() ) {
            res += QString::fromLatin1("(") + toStr( *it, markSelection ) + QString::fromLatin1(")");
        }
        else {
            res += toStr( *it, markSelection );
        }
	}
	return res;

}

QString EmacsRegExpConverter::toString( ConcRegExp* regexp, bool markSelection )
{
	QString res;
    //bool childSelected = false;

    RegExpList list = regexp->children();
	for ( RegExpListIt it(list); *it; ++it ) {
        QString startPar = QString::fromLocal8Bit("");
        QString endPar = QString::fromLocal8Bit("");
        if ( (*it)->precedence() < regexp->precedence() ) {
            startPar = QString::fromLatin1( "\\(" );
            endPar = QString::fromLatin1( "\\)" );
        }

        // We currently do not support verification for Emacs style regexps.
        // Note these two have different tests! They are activated in each their iteration of the loop.
        // if ( markSelection && !childSelected && !isSelected() && (*it)->isSelected() ) {
        //     res += QString::fromLatin1("\\(");
        //     childSelected = true;
        // }
        //
        // if ( markSelection && childSelected && !regexp->isSelected() && !(*it)->isSelected() ) {
        //     res += QString::fromLatin1("\\)");
        //     childSelected= false;
        // }

		res += startPar + toStr( *it, markSelection ) + endPar;
	}

    // We currently do not support verification for Emacs style regexps.
    //if ( markSelection && childSelected && !regexp->isSelected() ) {
    //    res += QString::fromLatin1(")");
    //}
	return res;

}

QString EmacsRegExpConverter::toString( LookAheadRegExp* /*regexp*/, bool /*markSelection*/ )
{
    // PENDING(blackie) enhance error handling
    qWarning( "Look ahead not supported");
    return QString::null;
}

QString EmacsRegExpConverter::toString( TextRangeRegExp* regexp, bool /*markSelection*/ )
{
    // PENDING(blackie) This code is from the Qt Converter, it needs an update I guess.
	QString txt;

	bool foundCarrot = false;
	bool foundDash = false;
	bool foundParenthesis = false;

	// Now print the rest of the single characters, but keep "^" as the very
	// last element of the characters.
    QStringList chars = regexp->chars();
	for (unsigned int i = 0; i< chars.count(); i++) {
		if ( *chars.at(i) == QChar(']') ) {
			foundParenthesis = true;
		}
		else if ( *chars.at(i) == QChar('-') ) {
			foundDash = true;
		}
		else if ( *chars.at(i) == QChar('^') ) {
			foundCarrot = true;
		}
		else {
			txt.append( *chars.at(i) );
		}
	}

	// Now insert the ranges.
    QPtrList<StringPair> ranges = regexp->range();
    for ( QPtrListIterator<StringPair> it(ranges); *it; ++it ) {
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

	// a '-' must be the first character ( only comming after a ']')
	if ( foundDash ) {
		res.append(QString::fromLatin1("-"));
	}

	res += txt;

	// Insert \s,\S,\d,\D,\w, and \W
    if ( regexp->digit() )
        res += QString::fromLocal8Bit("\\d");
    if ( regexp->nonDigit() )
        res += QString::fromLocal8Bit("\\D");
    if ( regexp->space() )
        res += QString::fromLocal8Bit("\\s");
    if ( regexp->nonSpace() )
        res += QString::fromLocal8Bit("\\S");
    if ( regexp->wordChar() )
        res += QString::fromLocal8Bit("\\w");
    if ( regexp->nonWordChar() )
        res += QString::fromLocal8Bit("\\W");


	if ( foundCarrot ) {
		res.append( QChar( '^' ) );
	}

	res.append(QString::fromLatin1("]"));

	return res;
}

QString EmacsRegExpConverter::toString( CompoundRegExp* regexp, bool markSelection )
{
    if ( markSelection && !regexp->isSelected() && regexp->child()->isSelected() )
        return QString::fromLatin1( "\\(" ) + toStr( regexp->child(), markSelection ) + QString::fromLatin1( "\\)" );
    else
        return  toStr( regexp->child(), markSelection );

}

QString EmacsRegExpConverter::toString( DotRegExp* /*regexp*/, bool /*markSelection*/ )
{
    return QString::fromLatin1( "." );
}

QString EmacsRegExpConverter::toString( PositionRegExp* regexp, bool /*markSelection*/ )
{
    switch ( regexp->position()) {
	case PositionRegExp::BEGLINE:
		return QString::fromLatin1("^");
	case PositionRegExp::ENDLINE:
		return QString::fromLatin1("$");
	case PositionRegExp::WORDBOUNDARY:
    case PositionRegExp::NONWORDBOUNDARY:
        // PENDING(blackie) enhance error handling
        qWarning("Word boundary and non word boundary is not supported in Emacs syntax");
        return QString::null;
    }
	Q_ASSERT( false );
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

        return startPar + res + endPar;
    }
}

QString EmacsRegExpConverter::toString( TextRegExp* regexp, bool /*markSelection*/ )
{
    QValueList<QChar> list;
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
