#include "syntaxhighlighter.h"
SyntaxHighlighter::SyntaxHighlighter( Verifier* verifier )
    :QSyntaxHighlighter( verifier ), _verifier( verifier ), _caseSensitive( false ), _minimal( false )
{
}

int SyntaxHighlighter::highlightParagraph( const QString & text, int endStateOfLastPara )
{
    QRegExp regexp( _regexp );
    regexp.setCaseSensitive( _caseSensitive );
    regexp.setMinimal( _minimal );

    setFormat( 0, text.length(), _verifier->font(), Qt::black );

    if ( !regexp.isValid() || regexp.isEmpty() ) {
        return 0;
    }

    // ------------------------------ Process with the regular expression.
    QColor colors[] = { Qt::red, Qt::blue };
    int color = endStateOfLastPara;
    if ( color < 0 || color > 1 )
        color = 0;

    int index = 0;
    int start, length;
    while ( (index = regexp.search( text, index ) ) != -1 && index < (int) text.length()) {
        if ( regexp.pos(1) != -1 ) {
            start = regexp.pos(1);
            length = regexp.cap(1).length();
        }
        else {
            start = index;
            length = regexp.matchedLength();
        }

        if ( start != index )
            setFormat( index, start-index, colors[color] );

        QFont font = _verifier->font();
        font.setUnderline( true );
        font.setPointSize( font.pointSize() * 1.3 );
        setFormat( start, length, font, colors[color] );

        if ( length + (start-index) != regexp.matchedLength() )
            setFormat( start+length, regexp.matchedLength()-length-(start-index), colors[color] );

        index +=  QMAX( 1, regexp.matchedLength() ); // ensure progress when matching for example ^ or \b
        color = (color+1)%2;
    }
    return color;
}

void SyntaxHighlighter::setRegExp( const QString& regexp )
{
    _regexp = regexp;
}

void SyntaxHighlighter::setCaseSensitive( bool b )
{
    _caseSensitive = b;
}

void SyntaxHighlighter::setMinimal( bool b )
{
    _minimal = b;
}


