#ifndef __altnwidget
#define __altnwidget

#include "multicontainerwidget.h"
class AltnRegExp;

/**
   RegExp widget for alternatives.

   @internal
*/
class AltnWidget :public MultiContainerWidget
{
public:
  AltnWidget( RegExpEditorWindow* editorWindow, QWidget *parent,
              const char *label = 0);
  AltnWidget( AltnRegExp* regexp, RegExpEditorWindow* editorWindow,
              QWidget* parent, const char* name = 0);
  virtual void addNewChild(DragAccepter *accepter, RegExpWidget *child);
  virtual QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual void applyRegExpToSelection( RegExpType type );  
  virtual RegExpType type() const { return ALTN; }
  virtual RegExp* selection() const;
  virtual bool validateSelection() const;
  virtual void setConcChild(ConcWidget *child);
  virtual void addNewConcChild(DragAccepter *accepter, ConcWidget *child);
  virtual void deleteSelection();

protected:
  virtual void paintEvent( QPaintEvent* e );

private:
  /**
     This functions runs through all the dragaccepters and calls @ref
     DragAccepter::setDrawLine.
  */
  void updateDrawLineInfo();
  
  QString _text;
  mutable QSize _textSize;
  
  mutable int _maxSelectedWidth;  
  mutable int _childrenWidth;
  mutable int _childrenHeight;
};

#endif // __altnwidget
