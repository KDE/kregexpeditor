#include "drag.h"
#include "regexp.h"

RegExpWidgetDrag::RegExpWidgetDrag( QByteArray data, QWidget* dragSource ) 
  : QStoredDrag( "KRegExpDialog/widgetdrag", dragSource )
{
  setEncodedData( data );
}

bool RegExpWidgetDrag::canDecode( QDragMoveEvent* event ) 
{
  return event->provides( "KRegExpDialog/widgetdrag" );
}

RegExpWidget* RegExpWidgetDrag::decode(QDropEvent* event, RegExpEditorWindow* window,
                                       QWidget* parent) 
{
  QByteArray payload = event->encodedData("KRegExpDialog/widgetdrag" );
  QTextStream stream( payload, IO_ReadOnly );
  QString str = stream.read();
  RegExp* regexp = WidgetFactory::createRegExp( str );
  RegExpWidget* widget = WidgetFactory::createWidget( regexp, window, parent );
  delete regexp;
  return widget;
}
