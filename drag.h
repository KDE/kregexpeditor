#ifndef __drag_h
#define __drag_h
#include <qdragobject.h>
#include <qpoint.h>
#include <qsize.h>
#include <qwidget.h>
#include "widgetfactory.h"

class RegExpWidget;

/**
   Class used for drag and drop in the RegExp widget.
   @internal
*/
class RegExpWidgetDrag :public QStoredDrag
{
public:
  RegExpWidgetDrag(QByteArray data, QWidget* dragSource);
  static bool canDecode(QDragMoveEvent* event);
  static RegExpWidget* decode(QDropEvent* event, RegExpEditorWindow* window,
                              QWidget* parent);
};

#endif //__drag_h
