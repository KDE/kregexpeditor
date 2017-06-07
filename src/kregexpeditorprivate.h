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
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#ifndef kregexpeditorprivate_h
#define kregexpeditorprivate_h

#include <QMainWindow>
#include <QStack>

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
class RegExpButtons;
class RegExp;
class QLabel;

/**
   Widget used to build a regular expression

   @author Jesper K. Pedersen <blackie@kde.org>
   @version 0.1
**/
class KRegExpEditorPrivate : public QMainWindow
{
    Q_OBJECT

public:
    KRegExpEditorPrivate(QWidget *parent);
    ~KRegExpEditorPrivate();
    QString regexp();
    void setMinimal(bool);
    void setCaseSensitive(bool);
    void setAllowNonQtSyntax(bool);

protected Q_SLOTS:
    void slotUpdateEditor(const QString &);
    void slotUpdateLineEdit();
    void slotShowEditor();
    void slotTriggerUpdate();
    void slotTimeout();
    void maybeVerify();
    void doVerify();
    void setAutoVerify(bool);
    void setVerifyText(const QString &fileName);
    void setMatchGreedy(bool);

public Q_SLOTS:
    void slotUndo();
    void slotRedo();
    void slotSetRegexp(const QString &regexp);
    void setMatchText(const QString &text);
    void setSyntax(const QString &);
    void showHelp();

Q_SIGNALS:
    void canUndo(bool);
    void canRedo(bool);
    void changes(bool);

protected:
    void recordUndoInfo();
    void emitUndoRedoSignals();

private:
    RegExpScrolledEditorWindow *_scrolledEditorWindow;
    RegExpButtons *_regExpButtons;
    VerifyButtons *_verifyButtons;
    AuxButtons *_auxButtons;
    InfoPage *_info;
    QLineEdit *_regexpEdit;
    QSplitter *_editor;
    bool _updating;
    QLabel *_error;
    QStack<RegExp *> _undoStack;
    QStack<RegExp *> _redoStack;
    UserDefinedRegExps *_userRegExps;
    QTimer *_timer;
    Verifier *_verifier;
    bool _autoVerify;
    ErrorMap _errorMap;
    bool _matchGreedy;
};

#endif
