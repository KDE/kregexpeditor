#ifndef __dcbutton
#define __dcbutton
#include <qpushbutton.h>
#include <qpixmap.h>
class QMouseEvent;

/**
   QPushButton extended to emit a signal on double click.
   @internal
*/
class DoubleClickButton :public QPushButton 
{
Q_OBJECT

public:
  DoubleClickButton( QPixmap pix, QWidget* parent, const char* name = 0);

protected:
  virtual void mouseDoubleClickEvent ( QMouseEvent * );
  
signals:
  void doubleClicked();
};

#endif // __dcbutton
