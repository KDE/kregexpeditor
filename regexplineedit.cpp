#include "regexplineedit.h"

RegExpLineEdit::RegExpLineEdit( QWidget* parent, const char* name )
    :QLineEdit( parent, name ), _editable( true )
{
    installEventFilter( this );
}

void RegExpLineEdit::setEditable( bool b )
{
    _editable = b;
}

bool RegExpLineEdit::eventFilter( QObject*, QEvent* ev )
{
    if ( !_editable && ( ev->type() >= QEvent::MouseButtonPress && ev->type() <= QEvent::FocusOut ||
                         ev->type() == QEvent::ContextMenu ) )
        return true;
    else
        return false;
}
