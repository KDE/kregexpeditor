#ifndef REGEXPCONVERTER_H
#define REGEXPCONVERTER_H
#include "regexp.h"
#include <qstring.h>
#include "altnregexp.h"
#include "concregexp.h"
#include "lookaheadregexp.h"
#include "textrangeregexp.h"
#include "compoundregexp.h"
#include "dotregexp.h"
#include "positionregexp.h"
#include "repeatregexp.h"
#include "textregexp.h"
#include <qtextedit.h>
#include <qsyntaxhighlighter.h>
#include "regexphighlighter.h"

class RegExpConverter
{
public:
    enum Features {
        WordBoundary = 0x01,
        NonWordBoundary = 0x02,
        WordStart = 0x04,
        WordEnd = 0x08,
        PosLookAhead = 0x10,
        NegLookAhead = 0x20
    };

    virtual bool canParse() = 0;
    virtual QString name() = 0;
    virtual int features() = 0;
    virtual RegExp* parse( const QString&, bool* ok );
    QString toStr( RegExp*, bool markSelection );
    virtual RegexpHighlighter* highlighter( QTextEdit* );

protected:
    virtual QString toString( AltnRegExp*, bool markSelection ) = 0;
    virtual QString toString( ConcRegExp*, bool markSelection ) = 0;
    virtual QString toString( LookAheadRegExp*, bool markSelection ) = 0;
    virtual QString toString( TextRangeRegExp*, bool markSelection ) = 0;
    virtual QString toString( CompoundRegExp*, bool markSelection ) = 0;
    virtual QString toString( DotRegExp*, bool markSelection ) = 0;
    virtual QString toString( PositionRegExp*, bool markSelection ) = 0;
    virtual QString toString( RepeatRegExp*, bool markSelection ) = 0;
    virtual QString toString( TextRegExp*, bool markSelection ) = 0;
    QString escape( QString text, QValueList<QChar> chars, QChar escapeChar) const;
};

#endif /* REGEXPCONVERTER_H */

