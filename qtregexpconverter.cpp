#include "qtregexpconverter.h"

extern RegExp* parseQtRegExp( QString str, bool *ok );
extern RegExp* parseDataQtRegExp();

bool QtRegExpConverter::canParse()
{
    return true;
}


RegExp* QtRegExpConverter::parse( const QString& txt, bool* ok )
{
    return parseQtRegExp( txt, ok );
}

QString QtRegExpConverter::toString( AltnRegExp* regexp, bool markSelection )
{
    QString res;

	bool first = true;
    RegExpList list = regexp->children();
    for ( RegExpListIt it(list); *it; ++it ) {
		if ( !first ) {
			res += QString::fromLatin1( "|" );
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

QString QtRegExpConverter::toString( ConcRegExp* regexp, bool markSelection )
{
    QString res;
    bool childSelected = false;

    RegExpList list = regexp->children();
	for ( RegExpListIt it(list); *it; ++it ) {
        QString startPar = QString::fromLocal8Bit("");
        QString endPar = QString::fromLocal8Bit("");
        if ( (*it)->precedence() < regexp->precedence() ) {
            if ( markSelection )
                startPar = QString::fromLocal8Bit("(?:");
            else
                startPar = QString::fromLatin1( "(" );
            endPar = QString::fromLatin1( ")" );
        }

        // Note these two have different tests! They are activated in each their iteration of the loop.
        if ( markSelection && !childSelected && !regexp->isSelected() && (*it)->isSelected() ) {
            res += QString::fromLatin1("(");
            childSelected = true;
        }

        if ( markSelection && childSelected && !regexp->isSelected() && !(*it)->isSelected() ) {
            res += QString::fromLatin1(")");
            childSelected= false;
        }

		res += startPar + toStr( *it, markSelection ) + endPar;
	}
    if ( markSelection && childSelected && !regexp->isSelected() ) {
        res += QString::fromLatin1(")");
    }
	return res;
}

QString QtRegExpConverter::toString( LookAheadRegExp* regexp, bool markSelection )
{
    if ( regexp->lookAheadType() == LookAheadRegExp::POSITIVE )
        return QString::fromLatin1( "(?=" ) + toStr( regexp->child(), markSelection ) + QString::fromLocal8Bit( ")" );
    else
        return QString::fromLatin1( "(?!" ) + toStr( regexp->child(), markSelection ) + QString::fromLocal8Bit( ")" );
}

QString QtRegExpConverter::toString( TextRangeRegExp* regexp, bool /*markSelection*/ )
{
	QString txt;

	bool foundCarrot = false;
	bool foundDash = false;
	bool foundParenthesis = false;

	// Now print the rest of the single characters, but keep "^" as the very
	// last element of the characters.
    QStringList chars = regexp->chars();
	for (unsigned int i = 0; i< chars.count(); i++) {
		if ( *chars.at(i) == QChar( ']' ) ) {
			foundParenthesis = true;
		}
		else if ( *chars.at(i) == QChar( '-' ) ) {
			foundDash = true;
		}
		else if ( *chars.at(i) == QChar( '^' ) ) {
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

QString QtRegExpConverter::toString( CompoundRegExp* regexp, bool markSelection )
{
    if ( markSelection && !regexp->isSelected() && regexp->child()->isSelected() )
        return QString::fromLatin1( "(" ) + toStr( regexp->child(), markSelection ) + QString::fromLatin1( ")" );
    else
        return  toStr( regexp->child(), markSelection );
}

QString QtRegExpConverter::toString( DotRegExp* /*regexp*/, bool /*markSelection*/ )
{
    return QString::fromLatin1( "." );
}

QString QtRegExpConverter::toString( PositionRegExp* regexp, bool /*markSelection*/ )
{
	switch (regexp->position()) {
	case PositionRegExp::BEGLINE:
		return QString::fromLatin1("^");
	case PositionRegExp::ENDLINE:
		return QString::fromLatin1("$");
	case PositionRegExp::WORDBOUNDARY:
        return QString::fromLatin1("\\b");
	case PositionRegExp::NONWORDBOUNDARY:
        return QString::fromLatin1("\\B");
    }
	Q_ASSERT( false );
	return QString::fromLatin1("");
}

QString QtRegExpConverter::toString( RepeatRegExp* regexp, bool markSelection )
{
    RegExp* child = regexp->child();
    QString cText = toStr( child, markSelection );
    QString startPar;
    QString endPar;

    if ( markSelection ) {
        if ( !regexp->isSelected() && child->isSelected()) {
            startPar = QString::fromLatin1( "(" );
            endPar = QString::fromLatin1( ")" );
        }
        else if ( child->precedence() < regexp->precedence() ) {
            startPar = QString::fromLatin1( "(?:" );
            endPar = QString::fromLatin1( ")" );
        }
    }
    else if ( child->precedence() < regexp->precedence() ) {
        startPar = QString::fromLatin1( "(" );
        endPar = QString::fromLatin1( ")" );
    }

    if ( regexp->min() == 0 && regexp->max() == -1) {
        return startPar + cText +endPar + QString::fromLocal8Bit("*");
    }
    else if ( regexp->min() == 0 && regexp->max() == 1 ) {
        return startPar + cText + endPar + QString::fromLocal8Bit("?");
    }
    else if ( regexp->min() == 1 && regexp->max() == -1 ) {
        return startPar + cText + endPar + QString::fromLocal8Bit("+");
    }
    else if ( regexp->max() == -1 ) {
        return startPar + cText + endPar + QString::fromLocal8Bit("{") +
            QString::number( regexp->min() ) + QString::fromLocal8Bit(",") +
            QString::fromLocal8Bit("}");
    }
    else {
        return startPar + cText + endPar + QString::fromLocal8Bit("{") +
            QString::number( regexp->min() ) + QString::fromLocal8Bit(",") +
            QString::number( regexp->max() ) + QString::fromLocal8Bit("}");
    }
}

QString QtRegExpConverter::toString( TextRegExp* regexp, bool /*markSelection*/ )
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
         << QChar('\\')
         << QChar('{')
         << QChar('}')
         << QChar('(')
         << QChar(')');

	QString res = escape( regexp->text(), list, QChar('\\') );
	return res;
}

QString QtRegExpConverter::name()
{
    return QString::fromLatin1( "Qt" );
}

int QtRegExpConverter::features()
{
    return WordBoundary | NonWordBoundary | PosLookAhead | NegLookAhead;
}
