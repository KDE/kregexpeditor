#ifndef __AUXBUTTONS_H
#define __AUXBUTTONS_H

#include <qdockwindow.h>
class QPushButton;


class AuxButtons :public QDockWindow
{
Q_OBJECT

public:
  AuxButtons( QWidget* parent, const char* name );
  
signals:
  void undo();
  void redo();
  
protected slots:
  void slotEnterWhatsThis();
  void slotCanUndo( bool );
  void slotCanRedo( bool );
  
private:
  QPushButton* _undo;
  QPushButton* _redo;
};


#endif // __AUXBUTTONS_H
