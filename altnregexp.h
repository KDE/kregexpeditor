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
	AltnRegExp();
	
	void addRegExp( RegExp * );
  RegExpList children();
	
	virtual QString toString() const;
  virtual int precedence() const { return 1;}
  virtual QDomNode toXml( QDomDocument* doc ) const;
  virtual bool load( QDomElement, const QString& version );
  virtual void updateCI( CompoundInfo* ci );

private:
	RegExpList list;
};

#endif // __ALTNREGEXP_H
