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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
#include "drag.h"
#include "regexp.h"

RegExpWidgetDrag::RegExpWidgetDrag( RegExp* regexp, QWidget* dragSource )
  : QDragObject( dragSource ), _regexp( regexp->clone() )
{
}

RegExpWidgetDrag::~RegExpWidgetDrag()
{
  delete _regexp;
}


bool RegExpWidgetDrag::canDecode( QDragMoveEvent* event )
{
  return event->provides( "KRegExpEditor/widgetdrag" );
}

RegExpWidget* RegExpWidgetDrag::decode(QDropEvent* event, RegExpEditorWindow* window,
                                       QWidget* parent)
{
  QByteArray payload = event->encodedData("KRegExpEditor/widgetdrag" );
  QTextStream stream( payload, IO_ReadOnly );
  QString str = stream.read();
  RegExp* regexp = WidgetFactory::createRegExp( str );
  RegExpWidget* widget = WidgetFactory::createWidget( regexp, window, parent );
  delete regexp;
  return widget;
}

const char * RegExpWidgetDrag::format ( int i ) const
{
  if ( i == 0 )
    return "KRegExpEditor/widgetdrag";
  else if ( i == 1 )
    return "text/plain";
  else
    return 0;
}

QByteArray RegExpWidgetDrag::encodedData ( const char* format ) const
{
  QByteArray data;
  QTextStream stream( data, IO_WriteOnly );
  if ( QString::fromLocal8Bit( format ) == QString::fromLocal8Bit( "KRegExpEditor/widgetdrag" ) ) {
    QString xml = _regexp->toXmlString();
    stream << xml;
  }
  else if ( QString::fromLocal8Bit( format ) == QString::fromLocal8Bit( "text/plain" ) ) {
    stream << _regexp->toString( false );
  }
  else {
    qWarning("Unexpected drag and drop format: %s", format );
  }
  return data;
}

