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
	TextRegExp( bool selected, QString text = QString::null);

	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
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
    virtual RegExpType type() const { return TEXT;}
    virtual bool operator==( const RegExp& other ) const;


protected:
	QString escape( QString text, QValueList<QChar> chars, QChar escapeChar) const;

private:
	QString _text;
};

#endif // __textregexp_h
