#include "regexphighlighter.h"

RegexpHighlighter::RegexpHighlighter( QTextEdit* edit )
    :QSyntaxHighlighter( edit ), _caseSensitive( false ), _minimal( false )
{
}

void RegexpHighlighter::setRegExp( const QString& regexp )
{
    _regexp = regexp;
}

void RegexpHighlighter::setCaseSensitive( bool b )
{
    _caseSensitive = b;
}

void RegexpHighlighter::setMinimal( bool b )
{
    _minimal = b;
}





