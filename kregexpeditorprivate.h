#ifndef kregexpeditorprivate_h
#define kregexpeditorprivate_h

#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qptrstack.h>
#include <qcstring.h>
#include "regexp.h"
#include "errormap.h"
class QTimer;

class RegExpScrolledEditorWindow;
class QLineEdit;
class InfoPage;
class UserDefinedRegExps;
class QSplitter;
class Verifier;
class VerifyButtons;
class AuxButtons;
class RegExpLineEdit;

/**
   Widget used to build a regular expression

   @author Jesper Kjær Pedersen <blackie@kde.org>
   @version 0.1
**/
class KRegExpEditorPrivate  :public QWidget
{
    Q_OBJECT

public:
    KRegExpEditorPrivate( QWidget *parent, const char *name = 0 );
    QString regexp();
    void setMinimal( bool );
    void setCaseSensitive( bool );
    void setShowSyntaxCombo( bool );

protected slots:
    void slotUpdateEditor( const QString & );
    void slotUpdateLineEdit();
    void slotShowEditor();
    void slotTriggerUpdate();
    void slotTimeout();
    void maybeVerify();
    void doVerify();
    void setAutoVerify( bool );
    void setVerifyText( const QString& fileName );

public slots:
    void slotUndo();
    void slotRedo();
    void slotSetRegexp( QString regexp );
    void setMatchText( const QString& text );
    void setSyntax( RegExp::Syntax );

signals:
    void canUndo( bool );
    void canRedo( bool );
    void changes( bool );

protected:
    void recordUndoInfo();
    void emitUndoRedoSignals();

private:
	RegExpScrolledEditorWindow* _scrolledEditorWindow;
    VerifyButtons* _verifyButtons;
    AuxButtons *_auxButtons;
    InfoPage* _info;
    RegExpLineEdit* _regexpEdit;
    QSplitter* _editor;
    bool _updating;
    QLabel* _error;
    QPtrStack<RegExp> _undoStack;
    QPtrStack<RegExp> _redoStack;
    UserDefinedRegExps*  _userRegExps;
    QTimer* _timer;
    Verifier* _verifier;
    bool _autoVerify;
    ErrorMap _errorMap;
};

#endif

