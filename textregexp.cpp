#include "textregexp.h"
#include <kmessagebox.h>
#include <klocale.h>

TextRegExp::TextRegExp(QString text)
{
	_text = text;
}

QString TextRegExp::toString() const
{
	QPtrList<QChar> list;
	list.append(new QChar('$'));
	list.append(new QChar('^'));
	list.append(new QChar('.'));
	list.append(new QChar('*'));
	list.append(new QChar('+'));
	list.append(new QChar('?'));
	list.append(new QChar('['));
	list.append(new QChar(']'));
	list.append(new QChar('\\'));
	QString res = escape( _text, list, QChar('\\') );
	return res;
}

QString TextRegExp::escape( QString text, QPtrList<QChar> chars, QChar escapeChar) const
{
	QString res;
	for (unsigned int i=0; i<text.length(); i++) {
		for (unsigned int j=0; j<chars.count(); j++) {
			if ( text.at(i) == *chars.at(j) ) {
				res.append( escapeChar );
				break;
			}
		}
		res.append( text.at(i) );
	}

	return res;
	
}

void TextRegExp::append( QString str )
{
  _text.append( str );
}

QDomNode TextRegExp::toXml( QDomDocument* doc ) const
{
  QDomElement top = doc->createElement(QString::fromLocal8Bit("Text"));
  QDomText text = doc->createTextNode( _text );
  top.appendChild( text );
  return top;
}

bool TextRegExp::load( QDomElement top, const QString& /*version*/) 
{
  Q_ASSERT( top.tagName() == QString::fromLocal8Bit( "Text" ) );
  QDomNode child = top.firstChild();
  if ( ! child.isText() ) {
    KMessageBox::sorry( 0, i18n("Element <b>Text</b> didn't contain any textual data"),
                        i18n("Error while loading from XML file") ) ;
    return false;
  }
  QDomText txtNode = child.toText();
  
  _text = txtNode.data();
  
  return true;
}

bool TextRegExp::operator==( const RegExp& other ) const { 
  if ( other.type() != type() )
    return false;
  
  const TextRegExp& theOther = dynamic_cast<const TextRegExp&>( other );
  if ( text() == theOther.text() )
    return true;
  
  return false;
}

