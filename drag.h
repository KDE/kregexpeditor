#ifndef __drag_h
#define __drag_h
#include <qdragobject.h>
#include <qpoint.h>
#include <qsize.h>
#include <qwidget.h>
#include "widgetfactory.h"
class RegExp;

class RegExpWidget;

/**
   Class used for drag and drop in the RegExp widget.
   @internal
*/
class RegExpWidgetDrag :public QDragObject
{
public:
  RegExpWidgetDrag( RegExp* regexp , QWidget* dragSource);
  ~RegExpWidgetDrag();
  virtual const char * format ( int i = 0 ) const;
  virtual QByteArray encodedData ( const char * ) const;
  static bool canDecode(QDragMoveEvent* event);
  static RegExpWidget* decode(QDropEvent* event, RegExpEditorWindow* window,
                              QWidget* parent);
private:
  RegExp* _regexp;
};

#endif //__drag_h
