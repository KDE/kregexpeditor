#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H
#include "verifier.h"
#include <qsyntaxhighlighter.h>

class SyntaxHighlighter :public QSyntaxHighlighter
{
public:
    SyntaxHighlighter( Verifier* verifier );
    void setRegExp( const QString& regexp );
    virtual int highlightParagraph ( const QString & text, int endStateOfLastPara );
    void setCaseSensitive( bool );
    void setMinimal( bool );

private:
    Verifier* _verifier;
    QString _regexp;
    bool _caseSensitive, _minimal;
};

#endif /* SYNTAXHIGHLIGHTER_H */

