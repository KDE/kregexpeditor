#ifndef __multicontainerwidget
#define __multicontainerwidget

#include "regexpwidget.h"
#include <qptrlist.h>

/**
   Abstract RegExp widget class representing widgets having multible children.
   @internal
*/
class MultiContainerWidget :public RegExpWidget
{
public:
  MultiContainerWidget( RegExpEditorWindow* editorWindow, QWidget* parent = 0,
                   const char* name = 0);
  virtual bool hasSelection() const;
  virtual void clearSelection();
  virtual void deleteSelection();
  virtual void addNewChild(DragAccepter *accepter, RegExpWidget *child);
  virtual bool updateSelection(bool parentSelected);
  virtual QRect selectionRect() const;  
  virtual RegExpWidget* widgetUnderPoint( QPoint globalPos, bool justVisibleWidgets );
  virtual RegExpWidget* findWidgetToEdit( QPoint globalPos );
  virtual void selectWidget( bool sel );
  virtual void updateAll();
  
protected:
  void append( RegExpWidget* child );    
  QPtrList<RegExpWidget> _children;
};


#endif // __multicontainerwidget
