#include "regexp.h"
#include "widgetfactory.h"
#include "kregexpeditorgui.h"
#include "errormap.h"

RegExp::Syntax RegExp::_syntax = RegExp::Qt;

RegExp::RegExp( bool selected ) : _parent(0), _destructing( false ), _selected( selected )
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
  QString xmlString = QString::fromLocal8Bit("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<!DOCTYPE RegularExpression PUBLIC \"-//KDE//KRegexpEditor DTD 1.0//EN\" \"http://www.blackie.dk/kreg.dtd\">\n") + doc.toString();

  return xmlString;
}

RegExp* RegExp::clone() const
{
  return WidgetFactory::createRegExp( toXmlString() );
}

void RegExp::check( ErrorMap& map )
{
    map.start();
    check( map, true, true );
    map.end();
}

void RegExp::setSyntax( Syntax syntax )
{
    _syntax = syntax;
}

QString RegExp::openPar() const
{
    if ( _syntax == Qt )
        return QString::fromLatin1( "(" );
    else if ( _syntax == Emacs )
        return QString::fromLatin1( "\\(" );
    else {
        qFatal("What");
        return QString::null;
    }
}

QString RegExp::closePar() const
{
    if ( _syntax == Qt )
        return QString::fromLatin1( ")" );
    else if ( _syntax == Emacs )
        return QString::fromLatin1( "\\)" );
    else {
        qFatal("What");
        return QString::null;
    }
}
