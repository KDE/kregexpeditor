#ifndef __COMPOUNDREGEXP_H
#define __COMPOUNDREGEXP_H
#include "regexp.h"

/**
   Abstract syntax node for `compound content' regular expression
   @internal
*/
class CompoundRegExp :public RegExp
{
public:
	CompoundRegExp( const QString& title = QString::null, 
                  const QString& description = QString::null,
                  bool hidden = false, bool allowReplace = false, RegExp* child = 0);
	
	virtual QString toString() const;
  virtual int precedence() const { return _child->precedence();}
  virtual QDomNode toXml( QDomDocument* doc ) const;
  virtual bool load( QDomElement, const QString& version );
  QString title() const { return _title; }
  QString description() const { return _description; }
  RegExp* child() const { return _child; }
  bool hidden() const { return _hidden; }    
  bool allowReplace() const { return _allowReplace; }
  virtual RegExpType type() const { return COMPOUND;}
  virtual bool operator==( const RegExp& other ) const;

private:
  QString _title;
  QString _description;
  bool _hidden;
  bool _allowReplace;
	RegExp* _child;
};


#endif // __COMPOUNDREGEXP_H
