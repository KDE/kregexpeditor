#include "kwidgetstreamer.h"
#include "kmultiformlistbox.h"
#include <qobjectlist.h>
#include <qobjectdict.h>
#include <qvariant.h>


void KWidgetStreamer::toStream(const QObject* from, QDataStream& stream )
{
  if ( from->inherits("KMultiFormListBox") ) {
    dynamic_cast<const KMultiFormListBox*>(from)->toStream( stream );
  }

  propertyToStream( from, stream );
}

void KWidgetStreamer::fromStream( QDataStream& stream, QObject* to )
{
  if ( to->inherits("KMultiFormListBox") ) {
    dynamic_cast<KMultiFormListBox*>(to)->fromStream( stream );
  }
  
  propertyFromStream( stream, to );
}


void KWidgetStreamer::propertyToStream( const QObject* from, QDataStream& stream )
{
  // Only handle widgets. Alternatives to widgets are layouts, validators, timers, etc.
  if ( ! from->inherits("QWidget") )
    return;

  // Serializing all the children (if any).
  const QObjectList* children = from->children();
  if ( children ) {
    stream <<  children->count();
    for ( QObjectListIt it = QObjectListIt(*children); *it; ++it ) {
      toStream( *it, stream );
    }
  }
  else {
    stream << (unsigned int) 0;
  }
  
  // Now stream out properties
  for ( PropertyMapIt mapIt = _map.begin(); mapIt != _map.end(); mapIt++ ) {
    QString tp = mapIt.key();
    PropertyList list = mapIt.data();
    if ( from->inherits( tp.latin1() ) ) {
      for ( PropertyListIt it = list.begin(); it != list.end(); ++it ) {
        QVariant prop = from->property( (*it).latin1() );
        Q_ASSERT( prop.isValid() );
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
  const QObjectList* children = to->children();
  unsigned int count;
  
  stream >> count;
  if ( children ) {
    Q_ASSERT( count == children->count() );
    for ( QObjectListIt it = QObjectListIt(*children); *it; ++it )
      fromStream( stream, *it );
  }
  else {
    Q_ASSERT( count == 0 );
  }

  // Now stream in properties
  for ( PropertyMapIt mapIt = _map.begin(); mapIt != _map.end(); mapIt++ ) {
    QString tp = mapIt.key();
    PropertyList list = mapIt.data();
    if ( to->inherits( tp.latin1() ) ) {
      for ( PropertyListIt it = list.begin(); it != list.end(); ++it ) {
         QVariant value;
        stream >> value;
        to->setProperty((*it).latin1(), value);
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
    << QString::fromLatin1("text") << QString::fromLatin1("maxLength") << QString::fromLatin1("maxLines") 
    << QString::fromLatin1("overWriteMode") 
    << QString::fromLatin1("echoMode") << QString::fromLatin1("alignment");
  _map.insert(QString::fromLatin1("QMultiLineEdit"), l);

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
