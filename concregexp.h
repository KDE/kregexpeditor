#ifndef __CONCREGEXP_H
#define __CONCREGEXP_H

#include "regexp.h"


/**
   Abstract syntax node for `concatenated' regular expression
   @internal
*/
class ConcRegExp :public RegExp
{
public:
	ConcRegExp();
	
	void addRegExp( RegExp *);
  RegExpList children();
  RegExp* lastRegExp();
	
	virtual QString toString() const;
  virtual int precedence() const { return 2;}
  virtual QDomNode toXml( QDomDocument* doc ) const;
  virtual bool load( QDomElement, const QString& version );
  virtual void updateCI( CompoundInfo* ci );
  
private:
	RegExpList list;
};

#endif // __CONCREGEXP_H
