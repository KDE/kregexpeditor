#ifndef __REGEXPBUTTONS_H
#define __REGEXPBUTTONS_H

#include <qdockwindow.h>
#include "widgetfactory.h"
class DoubleClickButton;
class QButtonGroup;
class QPushButton;
class QSignalMapper;


class RegExpButtons :public QDockWindow
{
Q_OBJECT

public:
  RegExpButtons( QWidget *parent, const char *name = 0 );

protected:
  DoubleClickButton* insert(RegExpType tp, const char* file, QString tooltip, QString whatsthis);
  
public slots:
  void slotSelectNewAction();
  void slotUnSelect();
  
protected slots:
  void slotSetKeepMode();
  void slotSetNonKeepMode();

signals:
  void clicked( int );
  void doSelect();
  
private:
  QButtonGroup* _grp;
  QPushButton* _selectBut;
  QSignalMapper* _mapper; // single click Mapper.

  /**
     This variable is true if the use wishes to continue editing in the
     selected mode after the previous editing is completed (that is, if the
     user has double clicked the buttons).
  */
  bool _keepMode;
};


#endif // __REGEXPBUTTON_H
