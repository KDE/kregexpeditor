#ifndef REGEXPLINEEDIT_H
#define REGEXPLINEEDIT_H
#include <qlineedit.h>
#include <qevent.h>

class RegExpLineEdit :public QLineEdit {

public:
    RegExpLineEdit( QWidget* parent, const char* name = 0 );
    void setEditable( bool b );
protected:
    bool eventFilter( QObject*, QEvent* ev );

private:
    bool _editable;
};


#endif /* REGEXPLINEEDIT_H */

