#ifndef __textregexp_h
#define __textregexp_h

#include "regexp.h"

/**
   Abstract syntax node for `text' regular expression
   @internal
*/
class TextRegExp :public RegExp
{
public:
	TextRegExp(QString text = QString::null);
	
	virtual QString toString() const;
  virtual int precedence() const { 
    if ( _text.length() > 1 ) 
      return 2;
    else
      return 4;
  }
  QString text() const { return _text; }
  virtual QDomNode toXml( QDomDocument* doc ) const;
  virtual bool load( QDomElement, const QString& version );
  void append( QString str);

protected:
	QString escape( QString text, QList<QChar> chars, QChar escapeChar) const; 
	
private:
	QString _text;
};

#endif // __textregexp_h
