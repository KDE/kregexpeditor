/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
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
    explicit KRegExpEditorPrivate(QWidget *parent);
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
    RegExpScrolledEditorWindow *_scrolledEditorWindow = nullptr;
    RegExpButtons *_regExpButtons = nullptr;
    VerifyButtons *_verifyButtons = nullptr;
    AuxButtons *_auxButtons = nullptr;
    InfoPage *_info = nullptr;
    QLineEdit *_regexpEdit = nullptr;
    QSplitter *_editor = nullptr;
    bool _updating;
    QLabel *_error;
    QStack<RegExp *> _undoStack;
    QStack<RegExp *> _redoStack;
    UserDefinedRegExps *_userRegExps;
    QTimer *_timer = nullptr;
    Verifier *_verifier = nullptr;
    bool _autoVerify;
    ErrorMap _errorMap;
    bool _matchGreedy;
};

#endif
