/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
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
class RegExpConverter;
class RegExpButtons;

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
    static RegExpConverter* converter();

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
    void setSyntax( const QString& );
    void showHelp();

signals:
    void canUndo( bool );
    void canRedo( bool );
    void changes( bool );

protected:
    void recordUndoInfo();
    void emitUndoRedoSignals();

private:
	RegExpScrolledEditorWindow* _scrolledEditorWindow;
    RegExpButtons* _regExpButtons;
    VerifyButtons* _verifyButtons;
    AuxButtons *_auxButtons;
    InfoPage* _info;
    QLineEdit* _regexpEdit;
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
    static RegExpConverter* _converter;
};

#endif

