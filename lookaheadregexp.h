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
  
	LookAheadRegExp( TYPE tp, RegExp* child = 0);
	
	virtual QString toString() const;
  virtual int precedence() const { return 4;}
  virtual QDomNode toXml( QDomDocument* doc ) const;
  virtual bool load( QDomElement, const QString& version );
  RegExp* child() const { return _child; }
  TYPE type() const { return _tp; }
  virtual void updateCI( CompoundInfo* ci );

private:
	RegExp* _child;
  TYPE _tp;
};

#endif // __LOOKAHEADREGEXP_H
