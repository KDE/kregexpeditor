#include "compoundregexp.h"
#include "widgetfactory.h"

CompoundRegExp::CompoundRegExp( const QString& title, const QString& description, bool hidden, 
                                bool allowReplace, RegExp* child)
  : _title( title ), _description( description ), _hidden( hidden ), _allowReplace( allowReplace ), _child( child )
{
  if ( child )
    addChild( child );
}

QString CompoundRegExp::toString() const 
{
  return  _child->toString();
}

QDomNode CompoundRegExp::toXml( QDomDocument* doc ) const 
{
  QDomElement top = doc->createElement( QString::fromLocal8Bit( "Compound" ) );
  if (_hidden)
    top.setAttribute( QString::fromLocal8Bit("hidden"), true );
  if ( _allowReplace )
    top.setAttribute( QString::fromLocal8Bit("allowReplace"), true );

  QDomElement title = doc->createElement( QString::fromLocal8Bit( "Title" ) );
  QDomText titleTxt = doc->createTextNode( _title );
  title.appendChild( titleTxt );
  top.appendChild( title );

  QDomElement description = doc->createElement( QString::fromLocal8Bit( "Description" ) );
  QDomText descriptionTxt = doc->createTextNode( _description );
  description.appendChild( descriptionTxt );
  top.appendChild( description );
  
  top.appendChild( _child->toXml( doc ) );

  return top;
}
  
  
bool CompoundRegExp::load( QDomElement top, const QString& version ) 
{
  Q_ASSERT( top.tagName() == QString::fromLocal8Bit("Compound") );
  QString str = top.attribute( QString::fromLocal8Bit( "hidden" ), QString::fromLocal8Bit("0") );
  _hidden = true; // alway hidden. (str == QString::fromLocal8Bit("1") );
  
  str = top.attribute( QString::fromLocal8Bit( "allowReplace" ), QString::fromLocal8Bit("0") );
  _allowReplace = (str == QString::fromLocal8Bit("1") );
  
  for ( QDomNode node = top.firstChild(); !node.isNull(); node = node.nextSibling() ) {
    if ( !node.isElement() ) 
      continue; // skip past comments.
    QString txt;
    QDomElement child = node.toElement();
    QDomNode txtNode = child.firstChild();
    if ( txtNode.isText() )
      txt = txtNode.toText().data();
    if ( child.tagName() == QString::fromLocal8Bit( "Title" ) ) {
      _title = txt;
    }
    else if ( child.tagName() == QString::fromLocal8Bit( "Description" ) ) {
      _description = txt;
    }
    else {
      _child = WidgetFactory::createRegExp( child, version );
      if ( _child ) {
        addChild( _child );
        return true;
      }
      else {
        return false;
      }
    }
  }
  return false;
}

bool CompoundRegExp::operator==( const RegExp& other ) const
{
  // Note the order is important in the comparison below.
  // Using other as the first argument, means that
  // the following will be evaluated: other.operator== rather than (*child).operator==
  // This means that if other is a CompoundRegExp too, then this level will be striped.
  return ( other == *_child );
}
