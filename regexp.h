#ifndef _REGEXP_H
#define _REGEXP_H

#include <qstring.h>
#include <qstringlist.h>
#include <qptrlist.h>
#include <qdom.h>
#include "compoundinfo.h"

/**
   Abstract syntax tree for regular expressions.
   @internal
*/
class RegExp
{
public:
  RegExp();
	virtual ~RegExp();
	
	virtual QString toString() const = 0;
  virtual int precedence() const = 0;
  virtual QDomNode toXml( QDomDocument* doc ) const = 0;
  virtual bool load( QDomElement, const QString& version ) = 0;
  QString toXmlString() const;
  virtual void updateCI( CompoundInfo* ci );

  void addChild( RegExp* child );
  void removeChild( RegExp* child );
  void setParent( RegExp* parent );
  RegExp* clone() const;
  
protected:
  RegExp* readRegExp( QDomElement top, const QString& version );

private:
  QPtrList<RegExp> _children;
  RegExp* _parent;
  bool _destructing;
};

typedef QPtrList<RegExp> RegExpList;
typedef QPtrListIterator<RegExp> RegExpListIt;




#endif // _REGEXP_H
