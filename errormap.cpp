#include "errormap.h"
#include <kmessagebox.h>
#include <klocale.h>

ErrorMap::ErrorMap() : _prevLineStartError( false ), _prevLineEndError( false ), _prevLookAHeadError( false )
{
}

void ErrorMap::start()
{
    _lineStartError = false;
    _lineEndError = false;
    _lookAHeadError = false;
}

void ErrorMap::end()
{
    _prevLineStartError = _lineStartError;
    _prevLineEndError = _lineEndError;
    _prevLookAHeadError = _lookAHeadError;
}

void ErrorMap::lineStartError()
{
    if ( ! _prevLineStartError ) {
        KMessageBox::information( 0, i18n("Your regular expression is invalid, due to something preceding a 'line start'"),
                                  i18n("Regular expression error"), QString::fromLatin1("KRegExpEditorLineStartError") );
    }
    _lineStartError = true;
}

void ErrorMap::lineEndError()
{
    if ( !_prevLineEndError ) {
        KMessageBox::information( 0, i18n("Your regular expression is invalid, due to something following a 'line end'"),
                                  i18n("Regular expression error"), QString::fromLatin1("KRegExpEditorLineEndError") );
    }
    _lineEndError = true;
}


void ErrorMap::lookAheadError()
{
    if ( !_prevLookAHeadError ) {
        KMessageBox::information( 0, i18n("Your regular expression is invalid. 'Look Ahead' regular expression must be the last last sub expression."),
                                  i18n("Regular expression error"), QString::fromLatin1("KRegExpEditorLookAHeadError") );
    }
    _lookAHeadError = true;
}

