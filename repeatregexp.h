#ifndef __REPEATREGEXP_H
#define __REPEATREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `repeated content' regular expression
   @internal
*/
class RepeatRegExp :public RegExp
{
public:
	RepeatRegExp( bool selected, int lower = 0, int upper = 0, RegExp* child = 0);

	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 3;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    int min() const { return _lower; }
    int max() const { return _upper; }
    RegExp* child() const { return _child; }
    virtual RegExpType type() const { return REPEAT;}
    virtual bool operator==( const RegExp& other ) const;
    virtual void replacePart( CompoundRegExp* replacement ) { _child->replacePart( replacement ); }

private:
	int _lower;
	int _upper;
	RegExp* _child;
};

#endif // __REPEATREGEXP_H
