#ifndef _REGEXP_H
#define _REGEXP_H

#include <qstring.h>
#include <qstringlist.h>
#include <qptrlist.h>
#include <qdom.h>
class CompoundRegExp;
class ErrorMap;


/**
   Abstract syntax tree for regular expressions.
   @internal
*/
class RegExp
{
public:
    RegExp( bool selected );
	virtual ~RegExp();
	
	virtual QString toString( bool markSelection ) const = 0;
    virtual int precedence() const = 0;
    virtual QDomNode toXml( QDomDocument* doc ) const = 0;
    virtual bool load( QDomElement, const QString& version ) = 0;
    QString toXmlString() const;
    
    void check( ErrorMap& );
    virtual bool check( ErrorMap&, bool first, bool last ) = 0;

    void addChild( RegExp* child );
    void removeChild( RegExp* child );
    void setParent( RegExp* parent );
    RegExp* clone() const;
    virtual bool operator==( const RegExp& other ) const { return ( type() == other.type() ); }  

    enum RegExpType { CONC, TEXT, DOT, POSITION, REPEAT, ALTN, COMPOUND, LOOKAHEAD, TEXTRANGE };
    virtual RegExpType type() const = 0;
    virtual void replacePart( CompoundRegExp* /* replacement */ ) {};
    bool isSelected() const { return _selected; }
    void setSelected( bool b ) { _selected = b; }
  
protected:
    RegExp* readRegExp( QDomElement top, const QString& version );

private:
    RegExp() {}; // disable
    QPtrList<RegExp> _children;
    RegExp* _parent;
    bool _destructing;
    bool _selected;
};

typedef QPtrList<RegExp> RegExpList;
typedef QPtrListIterator<RegExp> RegExpListIt;




#endif // _REGEXP_H
