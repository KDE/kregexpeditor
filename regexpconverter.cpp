#include "regexpconverter.h"

RegExpConverter* RegExpConverter::_current = 0;
RegExp* RegExpConverter::parse( const QString&, bool* ok )
{
    ok = false;
    return new DotRegExp( false ); // This method should never be called.
}

// This function needs to be called toStr rather than toString, as it is not possible to
// over load function across inheritance!
QString RegExpConverter::toStr( RegExp* regexp, bool markSelection )
{
    switch (regexp->type()) {
    case RegExp::CONC: return toString( static_cast<ConcRegExp*>( regexp ), markSelection);
    case RegExp::TEXT: return toString( static_cast<TextRegExp*>( regexp ), markSelection );
    case RegExp::DOT: return toString( static_cast<DotRegExp*>( regexp ), markSelection );
    case RegExp::POSITION: return toString( static_cast<PositionRegExp*>( regexp ), markSelection );
    case RegExp::REPEAT: return toString( static_cast<RepeatRegExp*>( regexp ), markSelection );
    case RegExp::ALTN: return toString( static_cast<AltnRegExp*>( regexp ), markSelection );
    case RegExp::COMPOUND: return toString( static_cast<CompoundRegExp*>( regexp ), markSelection );
    case RegExp::LOOKAHEAD: return toString( static_cast<LookAheadRegExp*>( regexp ), markSelection );
    case RegExp::TEXTRANGE: return toString( static_cast<TextRangeRegExp*>( regexp ), markSelection );
    }
    qWarning("We shouldn't get here!");
    return QString::fromLatin1( "" );
}


QString RegExpConverter::escape( QString text, QValueList<QChar> chars, QChar escapeChar) const
{
	QString res;
	for (unsigned int i=0; i<text.length(); i++) {
		for (unsigned int j=0; j<chars.count(); j++) {
			if ( text.at(i) == (chars[j]) ) {
				res.append( escapeChar );
				break;
			}
		}
		res.append( text.at(i) );
	}

	return res;
}

/**
   Returns a QSyntaxHighlighter to be used in the virifyer widget.
*/
RegexpHighlighter* RegExpConverter::highlighter( QTextEdit* )
{
    return 0;
}

RegExpConverter* RegExpConverter::current()
{
    return _current;
}

void RegExpConverter::setCurrent( RegExpConverter* converter)
{
    _current = converter;
}
