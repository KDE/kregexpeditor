#ifndef __DOTREGEXP_H
#define __DOTREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for "the dot" regular expression (i.e. any characters)
   @internal
*/
class DotRegExp :public RegExp
{
public:
	
	DotRegExp( bool selected );
	
	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 4;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    virtual RegExpType type() const { return DOT;}
};


#endif // __DOTREGEXP_H
