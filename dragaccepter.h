#ifndef __dragaccepter
#define __dragaccepter

#include "regexpwidget.h"
class MultiContainerWidget;

/**
   RegExp widget representing a "spot" in which new RegExp widgets can be inserted.
   @internal
*/
class DragAccepter :public RegExpWidget
{
  friend MultiContainerWidget;
  
public:
  DragAccepter(RegExpEditorWindow* editorWindow, RegExpWidget *parent,
               const char *name = 0);
  QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return DRAGACCEPTER; }
  void setDrawLine( bool drawLine ) { _drawLine = drawLine; }    
  
protected:
  virtual void mousePressEvent ( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dropEvent(QDropEvent *event);
  virtual void paintEvent(QPaintEvent *);
  virtual bool acceptWidgetInsert( RegExpType ) const { return true; }
  virtual bool acceptWidgetPaste() const { return true; }

private:
  bool _drawLine;
};

#endif // __dragaccepter
