#ifndef __selectablelineedit
#define __selectablelineedit
#include <qlineedit.h>
class RegExpWidget;

/**
   A specialized QLineEdit for use in @ref TextWidget.

   This widget has two main features:
   @li when the @ref TextWidget which contains it is selected, then the
        line edit is grayed like the rest of the widgets.
   @li When the widget does not have the focus, it is resized to only be
        large enough to contain the text written in it.

   @internal
*/
class SelectableLineEdit :public QLineEdit 
{
Q_OBJECT

public:
  SelectableLineEdit( RegExpWidget* owner, QWidget* parent = 0, const char* name = 0);
  void setSelected( bool selected );
  virtual QSize sizeHint() const;

protected slots:
  void slotKeyPressed();
  
signals:
  void parentPleaseUpdate();
  
private:
  RegExpWidget* _owner;
};

#endif
