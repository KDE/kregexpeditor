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
#include "kwidgetstreamer.h"
#include "kmultiformlistbox.h"
#include <QObject>
#include <QVariant>


void KWidgetStreamer::toStream(const QObject* from, QDataStream& stream )
{
  if ( from->inherits("KMultiFormListBox") ) {
    // Hmm, we'll trust Qt that this dynamic_cast won't fail!
    dynamic_cast<const KMultiFormListBox*>(from)->toStream( stream );
  }

  propertyToStream( from, stream );
}

void KWidgetStreamer::fromStream( QDataStream& stream, QObject* to )
{
  if ( to->inherits("KMultiFormListBox") ) {
    // Hmm, we'll trust Qt that this dynamic_cast won't fail!
    dynamic_cast<KMultiFormListBox*>(to)->fromStream( stream );
  }

  propertyFromStream( stream, to );
}


void KWidgetStreamer::propertyToStream( const QObject* from, QDataStream& stream )
{
  // Only handle widgets. Alternatives to widgets are layouts, validators, timers, etc.
  if ( ! from->inherits("QWidget") )
    return;


  // Stream in all the children (if any)
  const QList<QObject*> children= from->children();
  unsigned int count;

  stream >> count;
  if ( children.count() >0 )
  {
    stream <<  children.count();
  	for (int i = 0; i < children.size(); ++i) {
    	toStream( children.at(i), stream );
    	}
  }
  else
	stream << (unsigned int) 0;
  
  // Now stream out properties
  for ( PropertyMapIt mapIt = _map.begin(); mapIt != _map.end(); mapIt++ ) {
    QString tp = mapIt.key();
    PropertyList list = mapIt.value();
    if ( from->inherits( tp.toAscii() ) ) {
      for ( PropertyListIt it = list.begin(); it != list.end(); ++it ) {
        QVariant prop = from->property( (*it).toAscii() );
        if ( ! prop.isValid() )
          qWarning("Invalid property: %s:%s", qPrintable(tp), qPrintable(*it) );

        stream <<  prop ;
      }
    }
  }
}


void KWidgetStreamer::propertyFromStream( QDataStream& stream, QObject* to )
{
  // Only handle widgets. Alternatives to widgets are layouts, validators, timers, etc.
  if ( ! to->inherits("QWidget") )
    return;

  // Stream in all the children (if any)
  const QList<QObject*> children= to->children();
  //const QObjectList* children = to->children();
  unsigned int count;

  stream >> count;
  Q_ASSERT( count == 0 );
  for (int i = 0; i < children.size(); ++i) {
	fromStream( stream, children.at(i) );
	} 
  
  // Now stream in properties
  for ( PropertyMapIt mapIt = _map.begin(); mapIt != _map.end(); mapIt++ ) {
    QString tp = mapIt.key();
    PropertyList list = mapIt.value();
    if ( to->inherits( tp.toAscii() ) ) {
      for ( PropertyListIt it = list.begin(); it != list.end(); ++it ) {
         QVariant value;
        stream >> value;
        to->setProperty((*it).toAscii(), value);
      }
    }
  }
}

KWidgetStreamer::KWidgetStreamer ()
{
  QStringList l;

  // QCheckBox
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("checked") << QString::fromLatin1("tristate");
  _map.insert(QString::fromLatin1("QCheckBox"), l);

  // QComboBox
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("editable") << QString::fromLatin1("currentItem")
    << QString::fromLatin1("maxCount") << QString::fromLatin1("insertionPolicy")
    << QString::fromLatin1("autoCompletion");
  _map.insert(QString::fromLatin1("QComboBox"), l);

  // QDial
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("tracking") << QString::fromLatin1("wrapping")
    << QString::fromLatin1("value");
  _map.insert(QString::fromLatin1("QDial"), l);

  // QLCDNumber
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("numDigits") << QString::fromLatin1("mode")
    << QString::fromLatin1("segmentStyle") << QString::fromLatin1("value");
  _map.insert(QString::fromLatin1("QLCDNumber"), l);

  // QLineEdit
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("text") << QString::fromLatin1("maxLength")
    << QString::fromLatin1("echoMode") << QString::fromLatin1("alignment");
  _map.insert(QString::fromLatin1("QLineEdit"), l);

  // QMultiLineEdit
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("text")
    << QString::fromLatin1("alignment");
  _map.insert(QString::fromLatin1("QTextEdit"), l);

  // QRadioButton
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("checked");
  _map.insert(QString::fromLatin1("QRadioButton"), l);

  // QSlider
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("value");
  _map.insert(QString::fromLatin1("QSlider"), l);

  // QSpinBox
  l.clear();
  l << QString::fromLatin1("enabled")
    << QString::fromLatin1("value");
  _map.insert(QString::fromLatin1("QSpinBox"), l);
}
