#ifndef __singlecontainer
#define __singlecontainer
#include <regexpwidget.h>

/**
   Abstract class representing RegExp widgets having a single child.
*/
class SingleContainerWidget :public RegExpWidget 
{
Q_OBJECT

public:
  SingleContainerWidget( RegExpEditorWindow* editorWindow, QWidget* parent,
                         const char* name = 0);

  virtual bool updateSelection( bool parentSelected );
  virtual bool hasSelection() const;
  virtual void clearSelection();
  virtual void deleteSelection();
  virtual void applyRegExpToSelection( RegExpType type );  
  virtual RegExp* selection() const;
  virtual bool validateSelection() const;
  virtual QRect selectionRect() const;
  virtual RegExpWidget* widgetUnderPoint( QPoint globalPos, bool justVisibleWidgets );
  virtual RegExpWidget* findWidgetToEdit( QPoint globalPos );
  virtual void setConcChild( ConcWidget* child );
  virtual void selectWidget( bool sel );
  virtual void updateAll();

protected:
  ConcWidget *_child; 
};


#endif // __singlecontainer
