#ifndef REGEXPHIGHLIGHTER_H
#define REGEXPHIGHLIGHTER_H
#include <qsyntaxhighlighter.h>
#include <qstring.h>

class RegexpHighlighter :public QSyntaxHighlighter
{
public:
    RegexpHighlighter( QTextEdit* edit );
    void setRegExp( const QString& regexp );
    void setCaseSensitive( bool );
    void setMinimal( bool );

protected:
    QString _regexp;
    bool _caseSensitive, _minimal;

};

#endif /* REGEXPHIGHLIGHTER_H */

