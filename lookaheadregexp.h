#ifndef __LOOKAHEADREGEXP_H
#define __LOOKAHEADREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `repeated content' regular expression
   @internal
*/
class LookAheadRegExp :public RegExp
{
public:
    enum TYPE { POSITIVE, NEGATIVE };
  
	LookAheadRegExp( bool selected, TYPE tp, RegExp* child = 0);
	
	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 4;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    RegExp* child() const { return _child; }
    TYPE lookAheadType() const { return _tp; }
    virtual RegExpType type() const { return LOOKAHEAD;}
    virtual bool operator==( const RegExp& other ) const;
    virtual void replacePart( CompoundRegExp* replacement ) { _child->replacePart( replacement ); }
  
private:
	RegExp* _child;
    TYPE _tp;
};

#endif // __LOOKAHEADREGEXP_H
