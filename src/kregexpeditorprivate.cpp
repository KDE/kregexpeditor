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

#include "kregexpeditorprivate.h"

#include <KLocalizedString>
#include <QIcon>
#include <KIconLoader>

#include <KMessageBox>
#include <QLineEdit>

#include <QApplication>
#include <QToolButton>
#include <QTextStream>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTimer>
#include <QFile>
#include <QShortcut>
#include <QLabel>
#include <QStandardPaths>

#include "auxbuttons.h"
#include "verifybuttons.h"
#include "regexpbuttons.h"
#include "userdefinedregexps.h"
#include "scrollededitorwindow.h"
#include "infopage.h"
#include "verifier.h"
#include "regexpconverter.h"
#include "regexp.h"

KRegExpEditorPrivate::KRegExpEditorPrivate(QWidget *parent)
    : QMainWindow(parent)
    , _updating(false)
    , _autoVerify(true)
    , _matchGreedy(false)
{
    setMinimumSize(730, 300);
    setWindowFlags(Qt::Widget);

    // The DockWindows.
    _regExpButtons = new RegExpButtons(this, QStringLiteral("RegExpButton"));
    addToolBar(Qt::TopToolBarArea, _regExpButtons);

    _verifyButtons = new VerifyButtons(this, QStringLiteral("VerifyButtons"));
    addToolBar(Qt::TopToolBarArea, _verifyButtons);

    _auxButtons = new AuxButtons(this, QStringLiteral("AuxButtons"));
    addToolBar(Qt::TopToolBarArea, _auxButtons);

    _userRegExps = new UserDefinedRegExps(/*verArea1*/ this, /*"KRegExpEditorPrivate::userRegExps"*/ i18n("Compound regular expression:"));
    _userRegExps->setWhatsThis(i18n("In this window you will find predefined regular expressions. Both regular expressions "
                                    "you have developed and saved, and regular expressions shipped with the system."));
    addDockWidget(Qt::LeftDockWidgetArea, _userRegExps);

    // Editor window
    _editor = new QSplitter(Qt::Vertical, this);
    _editor->setObjectName(QStringLiteral("KRegExpEditorPrivate::_editor"));

    _scrolledEditorWindow = new RegExpScrolledEditorWindow(_editor);
    _scrolledEditorWindow->setWhatsThis(i18n("In this window you will develop your regular expressions. "
                                             "Select one of the actions from the action buttons above, and click the mouse in this "
                                             "window to insert the given action."));

    _info = new InfoPage(this);
    _info->setObjectName(QStringLiteral("_info"));
    _verifier = new Verifier(_editor);
    connect(_verifier, &QTextEdit::textChanged, this, &KRegExpEditorPrivate::maybeVerify);
    _verifier->setWhatsThis(i18n("<p>Type in some text in this window, and see what the regular expression you have developed matches.</p>"
                                 "<p>Each second match will be colored in red and each other match will be colored blue, simply so you "
                                 "can distinguish them from each other.</p>"
                                 "<p>If you select part of the regular expression in the editor window, then this part will be "
                                 "highlighted - This allows you to <i>debug</i> your regular expressions</p>"));

    _editor->hide();
    _editor->setSizes(QList<int>() << _editor->height() / 2 << _editor->height() / 2);

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_editor);
    layout->addWidget(_info);
    setCentralWidget(centralWidget);

    // Connect the buttons
    connect(_regExpButtons, SIGNAL(clicked(int)), _scrolledEditorWindow, SLOT(slotInsertRegExp(int)));
    connect(_regExpButtons, &RegExpButtons::doSelect, _scrolledEditorWindow, &RegExpScrolledEditorWindow::slotDoSelect);
    connect(_userRegExps, SIGNAL(load(RegExp*)), _scrolledEditorWindow, SLOT(slotInsertRegExp(RegExp*)));

    connect(_regExpButtons, SIGNAL(clicked(int)), _userRegExps, SLOT(slotUnSelect()));
    connect(_regExpButtons, SIGNAL(doSelect()), _userRegExps, SLOT(slotUnSelect()));
    connect(_userRegExps, &UserDefinedRegExps::load, _regExpButtons, &RegExpButtons::slotUnSelect);

    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::doneEditing, _regExpButtons, &RegExpButtons::slotSelectNewAction);
    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::doneEditing, _userRegExps, &UserDefinedRegExps::slotSelectNewAction);

    connect(_regExpButtons, &RegExpButtons::clicked, this, &KRegExpEditorPrivate::slotShowEditor);
    connect(_userRegExps, &UserDefinedRegExps::load, this, &KRegExpEditorPrivate::slotShowEditor);
    connect(_regExpButtons, &RegExpButtons::doSelect, this, &KRegExpEditorPrivate::slotShowEditor);

    connect(_scrolledEditorWindow, SIGNAL(savedRegexp()), _userRegExps, SLOT(slotPopulateUserRegexps()));

    connect(_auxButtons, &AuxButtons::undo, this, &KRegExpEditorPrivate::slotUndo);
    connect(_auxButtons, &AuxButtons::redo, this, &KRegExpEditorPrivate::slotRedo);
    connect(_auxButtons, &AuxButtons::cut, _scrolledEditorWindow, &RegExpScrolledEditorWindow::slotCut);
    connect(_auxButtons, &AuxButtons::copy, _scrolledEditorWindow, &RegExpScrolledEditorWindow::slotCopy);
    connect(_auxButtons, &AuxButtons::paste, _scrolledEditorWindow, &RegExpScrolledEditorWindow::slotPaste);
    connect(_auxButtons, &AuxButtons::save, _scrolledEditorWindow, &RegExpScrolledEditorWindow::slotSave);
    connect(_verifyButtons, &VerifyButtons::autoVerify, this, &KRegExpEditorPrivate::setAutoVerify);
    connect(_verifyButtons, &VerifyButtons::verify, this, &KRegExpEditorPrivate::doVerify);
    connect(_verifyButtons, &VerifyButtons::changeSyntax, this, &KRegExpEditorPrivate::setSyntax);
    connect(_verifyButtons, &VerifyButtons::matchGreedy, this, &KRegExpEditorPrivate::setMatchGreedy);

    connect(this, &KRegExpEditorPrivate::canUndo, _auxButtons, &AuxButtons::slotCanUndo);
    connect(this, &KRegExpEditorPrivate::canRedo, _auxButtons, &AuxButtons::slotCanRedo);
    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::anythingSelected, _auxButtons, &AuxButtons::slotCanCut);
    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::anythingSelected, _auxButtons, &AuxButtons::slotCanCopy);
    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::anythingOnClipboard, _auxButtons, &AuxButtons::slotCanPaste);
    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::canSave, _auxButtons, &AuxButtons::slotCanSave);

    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::verifyRegExp, this, &KRegExpEditorPrivate::maybeVerify);

    connect(_verifyButtons, &VerifyButtons::loadVerifyText, this, &KRegExpEditorPrivate::setVerifyText);

    _auxButtons->slotCanPaste(false);
    _auxButtons->slotCanCut(false);
    _auxButtons->slotCanCopy(false);
    _auxButtons->slotCanSave(false);

    // Line Edit
    QDockWidget *editDock = new QDockWidget(i18n("ASCII syntax:"), this);
    editDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, editDock);

    QWidget *editDockWidget = new QWidget(editDock);
    editDock->setWidget(editDockWidget);
    QHBoxLayout *dockLayout = new QHBoxLayout(editDockWidget);
    dockLayout->setContentsMargins(0, 0, 0, 0);

    _regexpEdit = new QLineEdit(editDockWidget);
    dockLayout->addWidget(_regexpEdit);
    _regexpEdit->setFocus(Qt::OtherFocusReason);
    _regexpEdit->setClearButtonEnabled(true);
    _regexpEdit->setWhatsThis(i18n("<p>This is the regular expression in ASCII syntax. You are likely only "
                                   "to be interested in this if you are a programmer, and need to "
                                   "develop a regular expression using QRegExp.</p>"
                                   "<p>You may develop your regular expression both by using the graphical "
                                   "editor, and by typing the regular expression in this line edit.</p>"));

    QPixmap pix = KIconLoader::global()->loadIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kregexpeditor/pics/error.png")), KIconLoader::Toolbar);
    _error = new QLabel(editDockWidget);
    _error->setPixmap(pix);
    dockLayout->addWidget(_error);
    _error->hide();

    _timer = new QTimer(this);
    _timer->setSingleShot(true);

    connect(_scrolledEditorWindow, &RegExpScrolledEditorWindow::change, this, &KRegExpEditorPrivate::slotUpdateLineEdit);
    connect(_regexpEdit, &QLineEdit::textChanged, this, &KRegExpEditorPrivate::slotTriggerUpdate);
    connect(_timer, &QTimer::timeout, this, &KRegExpEditorPrivate::slotTimeout);

    // Push an initial empty element on the stack.
    _undoStack.push(_scrolledEditorWindow->regExp());

    (void)new QShortcut(Qt::CTRL | Qt::Key_Z, this, SLOT(slotUndo()));
    (void)new QShortcut(Qt::CTRL | Qt::Key_R, this, SLOT(slotRedo()));

    setSyntax(QStringLiteral("Qt"));
}

KRegExpEditorPrivate::~KRegExpEditorPrivate()
{
    qDeleteAll(_undoStack);
    qDeleteAll(_redoStack);
}

QString KRegExpEditorPrivate::regexp()
{
    RegExp *regexp = _scrolledEditorWindow->regExp();
    QString res = RegExpConverter::current()->toStr(regexp, false);
    delete regexp;
    return res;
}

void KRegExpEditorPrivate::slotUpdateEditor(const QString &txt)
{
    _updating = true;
    bool ok;
    if (!RegExpConverter::current()->canParse()) {
        // This can happend if the application set a text through the API.
        //qDebug("cannot parse");
    } else {
        RegExp *result = RegExpConverter::current()->parse(txt, &ok);
        if (ok) {
            const QList<CompoundRegExp *> list = _userRegExps->regExps();
            for (CompoundRegExp *regExp : list) {
                result->replacePart(regExp);
            }

            _scrolledEditorWindow->slotSetRegExp(result);
            _error->hide();
            maybeVerify();
            recordUndoInfo();
            result->check(_errorMap);
        } else {
            _error->show();
            if (_autoVerify) {
                _verifier->clearRegexp();
            }
        }
        delete result;
    }
    _updating = false;
}

void KRegExpEditorPrivate::slotUpdateLineEdit()
{
    if (_updating) {
        return;
    }
    _updating = true;

    RegExp *regexp = _scrolledEditorWindow->regExp();
    regexp->check(_errorMap);

    QString str = RegExpConverter::current()->toStr(regexp, false);
    _regexpEdit->setText(str);
    delete regexp;

    recordUndoInfo();

    _updating = false;
}

void KRegExpEditorPrivate::recordUndoInfo()
{
    Q_ASSERT(_updating);

    // Update undo/redo stacks
    RegExp *regexp = _scrolledEditorWindow->regExp();
    if (regexp->toXmlString() != _undoStack.top()->toXmlString()) {
        _undoStack.push(regexp);
        qDeleteAll(_redoStack);
        _redoStack = QStack<RegExp *>();
        emitUndoRedoSignals();
    }
}

void KRegExpEditorPrivate::slotRedo()
{
    if (!_redoStack.isEmpty()) {
        _undoStack.push(_redoStack.pop());
        _scrolledEditorWindow->slotSetRegExp(_undoStack.top());
        slotUpdateLineEdit();
        emitUndoRedoSignals();
        maybeVerify();
    }
}

void KRegExpEditorPrivate::slotUndo()
{
    if (_undoStack.count() > 1) {
        _redoStack.push(_undoStack.pop());
        _scrolledEditorWindow->slotSetRegExp(_undoStack.top());
        slotUpdateLineEdit();
        emitUndoRedoSignals();
        maybeVerify();
    }
}

void KRegExpEditorPrivate::slotShowEditor()
{
    _info->hide();
    _editor->show();
}

void KRegExpEditorPrivate::emitUndoRedoSignals()
{
    Q_EMIT canUndo(_undoStack.count() > 1);
    Q_EMIT changes(_undoStack.count() > 1);
    Q_EMIT canRedo(_redoStack.count() > 0);
}

void KRegExpEditorPrivate::slotSetRegexp(const QString &regexp)
{
    _regexpEdit->setText(regexp);
}

void KRegExpEditorPrivate::slotTriggerUpdate()
{
    /* ### Guess this timeout value should be configurable somewhere, or (even
     * better: do some kind of benchmark each time the editor view gets updated
     * to measure how long it takes on the client system to render the editor
     * with the current complexity. That way we'd get good response times for
     * simple regexps, and flicker-free display for complex regexps.
     * - Frerich
     */
    if (!_updating) {
        _timer->start(300);
        slotShowEditor();
    }
}

void KRegExpEditorPrivate::slotTimeout()
{
    slotUpdateEditor(_regexpEdit->text());
}

void KRegExpEditorPrivate::setMatchText(const QString &text)
{
    bool autoVerify = _autoVerify;
    _autoVerify = false;
    _verifier->setText(text);
    _autoVerify = autoVerify;
}

void KRegExpEditorPrivate::maybeVerify()
{
    if (_autoVerify) {
        doVerify();
    } else {
        _verifyButtons->setMatchCount(-1);
    }
}

void KRegExpEditorPrivate::doVerify()
{
    bool autoVerify = _autoVerify; // prevent loop due to verify emit changed, which calls maybeVerify
    _autoVerify = false;
    RegExp *regexp = _scrolledEditorWindow->regExp();

    _verifier->verify(RegExpConverter::current()->toStr(regexp, true));
    delete regexp;
    _autoVerify = autoVerify;
}

void KRegExpEditorPrivate::setAutoVerify(bool on)
{
    _autoVerify = on;
    if (!_autoVerify) {
        _verifier->clearRegexp();
    } else {
        doVerify();
    }
}

void KRegExpEditorPrivate::setVerifyText(const QString &fileName)
{
    bool autoVerify = _autoVerify;
    _autoVerify = false;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        KMessageBox::error(nullptr, i18n("Could not open file '%1' for reading", fileName));
    } else {
        QTextStream s(&file);
        QString txt = s.readAll();
        file.close();
        RegExp *regexp = _scrolledEditorWindow->regExp();
        _verifier->setText(txt);
        _verifier->verify(RegExpConverter::current()->toStr(regexp, true));
        delete regexp;
    }
    _autoVerify = autoVerify;
}

void KRegExpEditorPrivate::setCaseSensitive(bool b)
{
    _verifier->setCaseSensitive(b);
}

void KRegExpEditorPrivate::setMinimal(bool b)
{
    _verifier->setMinimal(b);
}

void KRegExpEditorPrivate::setSyntax(const QString &syntax)
{
    RegExpConverter *converter = _verifyButtons->setSyntax(syntax);
    RegExpConverter::setCurrent(converter);
    if (converter->canParse()) {
        _regexpEdit->setReadOnly(false);
        _regexpEdit->setBackgroundRole(QPalette::Base);
    } else {
        _regexpEdit->setReadOnly(true);
        _regexpEdit->setBackgroundRole(QPalette::Window);
    }
    _regExpButtons->setFeatures(converter->features());
    _verifier->setHighlighter(converter->highlighter(_verifier));
    slotUpdateLineEdit();
}

void KRegExpEditorPrivate::showHelp()
{
    _info->show();
    _editor->hide();
}

void KRegExpEditorPrivate::setAllowNonQtSyntax(bool b)
{
    _verifyButtons->setAllowNonQtSyntax(b);
}

void KRegExpEditorPrivate::setMatchGreedy(bool b)
{
    _matchGreedy = b;
    _verifier->setMinimal(!b);
    doVerify();
}
