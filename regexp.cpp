#include "regexp.h"
#include "widgetfactory.h"
#include "kregexpeditorgui.h"

RegExp::RegExp() : _parent(0), _destructing( false )
{
  // Nothing to do
}

RegExp::~RegExp()
{
  _destructing = true;
  for ( QPtrListIterator<RegExp> it(_children); *it; ++it ) {
    delete *it;
  }
  if ( _parent )
    _parent->removeChild( this );
  _parent = 0;
}

void RegExp::addChild( RegExp* child )
{
  _children.append( child );
  child->setParent( this );
}

void RegExp::removeChild( RegExp* child )
{
  if ( ! _destructing ) {
    _children.remove( child );
  }
}

void RegExp::setParent( RegExp* parent )
{
  _parent = parent;
}

RegExp* RegExp::readRegExp( QDomElement top, const QString& version )
{
  for ( QDomNode node = top.firstChild(); !node.isNull(); node = node.nextSibling() ) {
    if (!node.isElement() )
      continue; // skip past comments
    RegExp* regexp = WidgetFactory::createRegExp(node.toElement(), version );
    return regexp;
  }
  return 0;
}

QString RegExp::toXmlString() const
{
  QDomDocument doc;
  doc.setContent("<RegularExpression/>");
  QDomNode top = doc.documentElement();
  top.toElement().setAttribute(QString::fromLocal8Bit("version"), KRegExpEditorGUI::version);

  QDomNode elm = toXml( &doc );
  
  top.appendChild( elm );
  QString xmlString = doc.toString();

  return xmlString;
}

RegExp* RegExp::clone() const 
{
  return WidgetFactory::createRegExp( toXmlString() );
}

void RegExp::updateCI( CompoundInfo* ci )
{
  // Default for non recursive items is to do nothing.
}
