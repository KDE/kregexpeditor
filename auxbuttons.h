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
    void cut();
    void copy();
    void paste();
    void save();
  
public slots:
    void slotEnterWhatsThis();
    void slotCanUndo( bool );
    void slotCanRedo( bool );
    void slotCanCut( bool );
    void slotCanCopy( bool );
    void slotCanPaste( bool );
    void slotCanSave( bool );

private:
    QPushButton* _undo;
    QPushButton* _redo;
    QPushButton* _cut;
    QPushButton* _copy;
    QPushButton* _paste;
    QPushButton* _save;
};


#endif // __AUXBUTTONS_H
