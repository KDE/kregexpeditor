#ifndef __ALTNREGEXP_H
#define __ALTNREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `alternative' regular expression
   @internal
*/
class AltnRegExp :public RegExp
{
public:
	AltnRegExp( bool selected );
	
	void addRegExp( RegExp * );
    RegExpList children();
	
	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 1;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    virtual RegExpType type() const { return ALTN;}
    virtual bool operator==( const RegExp& other ) const;
    virtual void replacePart( CompoundRegExp* replacement );
  
private:
	RegExpList list;
};

#endif // __ALTNREGEXP_H
