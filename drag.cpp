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

const char * RegExpWidgetDrag::format ( int i = 0 ) const 
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
    stream << _regexp->toString();
  }
  else {
    qWarning("Unexpected drag and drop format: %s", format );
  }
  return data;
}    
       
