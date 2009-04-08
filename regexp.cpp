/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#include "regexp.h"

#include "widgetfactory.h"
#include "kregexpeditorgui.h"
#include "errormap.h"

RegExp::RegExp( bool selected ) : _parent(0), _destructing( false ), _selected( selected )
{
  // Nothing to do
}

RegExp::~RegExp()
{
  _destructing = true;
  qDeleteAll(_children);
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
    _children.removeOne( child );
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
  doc.setContent( QString::fromLatin1( "<RegularExpression/>" ) );
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


