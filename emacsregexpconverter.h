#ifndef EMACSREGEXPCONVERTER_H
#define EMACSREGEXPCONVERTER_H

#include "regexpconverter.h"

class EmacsRegExpConverter :public RegExpConverter
{
public:
    virtual bool canParse();
    virtual QString name();
    virtual int features();
    virtual QString toString( AltnRegExp*, bool markSelection );
    virtual QString toString( ConcRegExp*, bool markSelection );
    virtual QString toString( LookAheadRegExp*, bool markSelection );
    virtual QString toString( TextRangeRegExp*, bool markSelection );
    virtual QString toString( CompoundRegExp*, bool markSelection );
    virtual QString toString( DotRegExp*, bool markSelection );
    virtual QString toString( PositionRegExp*, bool markSelection );
    virtual QString toString( RepeatRegExp*, bool markSelection );
    virtual QString toString( TextRegExp*, bool markSelection );
};


#endif /* EMACSREGEXPCONVERTER_H */

