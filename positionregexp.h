#ifndef __POSITIONREGEXP_H
#define __POSITIONREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `positions' regular expression
   @internal
*/
class PositionRegExp :public RegExp
{
public:
	enum PositionType { BEGLINE, ENDLINE, WORDBOUNDARY, NONWORDBOUNDARY };

	PositionRegExp( bool selected, PositionType tp );
    PositionType position() const { return _tp; }

	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 4;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    virtual RegExpType type() const { return POSITION ;}

private:
	PositionType _tp;

};

#endif // __POSITIONREGEXP_H
