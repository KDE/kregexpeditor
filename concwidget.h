#ifndef __concwidget
#define __concwidget

#include "multicontainerwidget.h"
class ConcRegExp;

/**
   RegExp widget which can have several sub widget inside it.
   This widget is invisible to the user, but act as a container around
   other RegExp widgets
   @internal
*/
class ConcWidget :public MultiContainerWidget
{
public:
  ConcWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
             const char *name = 0);
  ConcWidget(RegExpEditorWindow* editorWindow, RegExpWidget *child,
             QWidget *parent, const char *name=0);
  ConcWidget( RegExpEditorWindow* editorWindow, ConcWidget* origConc, 
              unsigned int start, unsigned int end);
  ConcWidget( ConcRegExp* regexp, RegExpEditorWindow* editorWindow,
              QWidget* parent, const char* name = 0);
  void init();

  virtual QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual bool updateSelection(bool parentSelected);
  virtual bool isSelected() const;

  virtual void applyRegExpToSelection( RegExpType type );  
  virtual RegExpType type() const { return CONC; }
  virtual RegExp* selection() const;
  virtual void addNewConcChild(DragAccepter *accepter, ConcWidget *child);
  virtual bool validateSelection() const;
  virtual bool acceptWidgetInsert( RegExpType ) const { return false; }
  virtual bool acceptWidgetPaste() const { return false; } 
  bool hasAnyChildren() { return _children.count() > 1; }
  
protected:
  virtual void paintEvent( QPaintEvent *e );
  virtual void mousePressEvent ( QMouseEvent* event );
  void sizeAccepter( DragAccepter* accepter, int height, int totHeight );
  void getSelectionIndexes( int* start, int* end );
  virtual void dragEnterEvent(QDragEnterEvent* event) { event->accept( false ); }

private:
  int _maxSelectedHeight;
};


#endif // __concwidget
