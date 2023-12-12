/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "kregexpeditorwindow.h"

#include <QActionGroup>
#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFrame>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>

#include <KActionCollection>
#include <KHelpClient>
#include <KLocalizedString>
#include <KSelectAction>

#include "emacsregexpconverter.h"
#include "kregexpeditorprivate.h"
#include "qtregexpconverter.h"

using namespace Qt::Literals::StringLiterals;

const QString KRegExpEditorWindow::version = u"1.0"_s;

KRegExpEditorWindow::KRegExpEditorWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    setWindowTitle(i18nc("@title:window", "Regular Expression Editor"));

    _editor = new KRegExpEditorPrivate(this);
    setCentralWidget(_editor);

    setupActions();

    // connect(_editor, &KRegExpEditorGUI::canUndo, this, &KRegExpEditorWindow::canUndo);
    // connect(_editor, &KRegExpEditorGUI::canRedo, this, &KRegExpEditorWindow::canRedo);
    // connect(_editor, &KRegExpEditorGUI::changes, this, &KRegExpEditorWindow::changes);

    // QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help);
    // QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    // okButton->setDefault(true);
    // okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    // layout->addWidget(buttonBox);
    // connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    // connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    // connect(buttonBox, &QDialogButtonBox::helpRequested, this, &KRegExpEditorWindow::showHelp);
    resize(640, 400);
}

KRegExpEditorWindow::~KRegExpEditorWindow()
{
}

void KRegExpEditorWindow::setupActions()
{
    // File
    KStandardAction::save(_editor, &KRegExpEditorPrivate::slotUndo, actionCollection());

    // Edit
    auto action = KStandardAction::undo(_editor, &KRegExpEditorPrivate::slotUndo, actionCollection());
    action->setEnabled(false);
    connect(_editor, &KRegExpEditorPrivate::canUndo, this, [action](bool canUndo) {
        action->setEnabled(canUndo);
    });
    action = KStandardAction::redo(_editor, &KRegExpEditorPrivate::slotRedo, actionCollection());
    action->setEnabled(false);
    connect(_editor, &KRegExpEditorPrivate::canRedo, this, [action](bool canRedo) {
        action->setEnabled(canRedo);
    });
    action = KStandardAction::save(_editor, &KRegExpEditorPrivate::slotSave, actionCollection());
    action->setEnabled(false);
    connect(_editor, &KRegExpEditorPrivate::canSave, this, [action](bool canSave) {
        action->setEnabled(canSave);
    });
    action = KStandardAction::copy(_editor, &KRegExpEditorPrivate::slotCopy, actionCollection());
    action->setEnabled(false);
    connect(_editor, &KRegExpEditorPrivate::anythingSelected, this, [action](bool anythingSelected) {
        action->setEnabled(anythingSelected);
    });
    action = KStandardAction::cut(_editor, &KRegExpEditorPrivate::slotCut, actionCollection());
    action->setEnabled(false);
    connect(_editor, &KRegExpEditorPrivate::anythingSelected, this, [action](bool anythingSelected) {
        action->setEnabled(anythingSelected);
    });
    action = KStandardAction::paste(_editor, &KRegExpEditorPrivate::slotPaste, actionCollection());
    action->setEnabled(false);

    // Verify
    auto verifyAction = new QAction(QIcon::fromTheme(QStringLiteral("tools-check-spelling")), i18n("Verify regular expression"), this);
    actionCollection()->addAction(u"kregexpeditor_verify"_s, verifyAction);
    connect(verifyAction, &QAction::triggered, this, [this](bool) {
        _editor->doVerify();
    });

    auto loadAction = new QAction(QIcon::fromTheme(QStringLiteral("document-open")), i18nc("@action:inmenu", "Load text"), this);
    loadAction->setToolTip(i18n("Load text in the verifier window"));
    actionCollection()->addAction(u"kregexpeditor_load"_s, loadAction);
    connect(loadAction, &QAction::triggered, this, [this](bool) {
        const QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), QString());
        if (!fileName.isNull()) {
            Q_EMIT _editor->setVerifyText(fileName);
        }
    });

    // Auto Verify
    QAction *autoVerify = new QAction(i18n("Verify on the Fly"), this);
    autoVerify->setCheckable(true);
    autoVerify->setChecked(true);
    // connect(autoVerify, &QAction::toggled, this, &VerifyButtons::updateVerifyButton);
    connect(autoVerify, &QAction::toggled, this, [this](bool checked) {
        _editor->setAutoVerify(checked);
    });
    actionCollection()->addAction(u"kregexpeditor_config_autoverify"_s, autoVerify);
    autoVerify->setToolTip(i18n("Toggle on-the-fly verification of regular expression"));
    autoVerify->setWhatsThis(
        i18n("Enabling this option will make the verifier update for each edit. "
             "If the verify window contains much text, or if the regular expression is either "
             "complex or matches a lot of time, this may be very slow."));

    // Match greedy
    QAction *matchGreedy = new QAction(i18n("Match Greedy"), this);
    matchGreedy->setCheckable(true);
    matchGreedy->setChecked(false);
    connect(matchGreedy, &QAction::toggled, this, [this](bool checked) {
        _editor->setMatchGreedy(checked);
    });
    actionCollection()->addAction(u"kregexpeditor_config_matchgreedy"_s, matchGreedy);
    matchGreedy->setToolTip(i18n("Toggle greedy matching when verifying the regular expression."));
    matchGreedy->setWhatsThis(i18n("When this option is enabled, the regular expression will be evaluated on a so-called greedy way."));

    // Converters
    auto languageMenu = new KSelectAction(i18nc("@title:menu", "RegExp Languages"), this);

    // Qt
    RegExpConverter *qtConverter = new QtRegExpConverter();
    m_converters.append(std::pair(qtConverter, static_cast<QAction *>(nullptr)));
    QString qtConverterName = qtConverter->name();
    _editor->setSyntax(qtConverter);

    // Emacs
    auto converter = new EmacsRegExpConverter();
    m_converters.append(std::pair(converter, static_cast<QAction *>(nullptr)));

    auto converterGroup = new QActionGroup(this);
    for (auto it = m_converters.begin(); it != m_converters.end(); ++it) {
        QString name = it->first->name();
        auto action = new QAction(name, this);
        action->setCheckable(true);
        converterGroup->addAction(action);
        languageMenu->addAction(action);
        it->second = action;

        if (it->first == qtConverter) {
            action->setEnabled(true);
        }
    }

    connect(converterGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        for (auto it = m_converters.begin(); it != m_converters.end(); ++it) {
            if (it->second == action) {
                _editor->setSyntax(it->first);
            }
        }
    });

    actionCollection()->addAction(u"kregexpeditor_config_languages"_s, languageMenu);

    setupGUI(ToolBar | Keys | Save | Create, u"kregexpeditorui.rc"_s);
}

void KRegExpEditorWindow::doSomething(const QString &method, void *arguments)
{
    if (method == QLatin1String("setCaseSensitive")) {
        _editor->setCaseSensitive((bool)arguments);
    } else if (method == QLatin1String("setMinimal")) {
        _editor->setMinimal((bool)arguments);
    } else if (method == QLatin1String("setSyntax")) {
        //_editor->setSyntax(*((QString *)arguments));
    } else {
        qFatal("%s", qPrintable(tr("Method '%1' is not valid!").arg(method)));
    }
}

void KRegExpEditorWindow::setMatchText(const QString &txt)
{
    _editor->setMatchText(txt);
}

void KRegExpEditorWindow::showHelp()
{
    KHelpClient::invokeHelp(QString(), QStringLiteral("kregexpeditor"));
}
