#ifndef __AUXBUTTONS_H
#define __AUXBUTTONS_H

#include <qdockwindow.h>
#include "regexp.h"
class QPushButton;
class QComboBox;
class QLabel;


class AuxButtons :public QDockWindow
{
    Q_OBJECT

public:
    AuxButtons( QWidget* parent, const char* name );
    void setShowSyntaxCombo( bool );
    void setSyntax( RegExp::Syntax );

signals:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void save();
    void changeSyntax( RegExp::Syntax );

public slots:
    void slotEnterWhatsThis();
    void slotCanUndo( bool );
    void slotCanRedo( bool );
    void slotCanCut( bool );
    void slotCanCopy( bool );
    void slotCanPaste( bool );
    void slotCanSave( bool );
    void slotChangeSyntax( int );

private:
    QPushButton* _undo;
    QPushButton* _redo;
    QPushButton* _cut;
    QPushButton* _copy;
    QPushButton* _paste;
    QPushButton* _save;
    QComboBox* _syntax;
    QLabel* _syntaxLabel;
};


#endif // __AUXBUTTONS_H
