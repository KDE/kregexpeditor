/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "kregexpeditorguidialog.h"

#include <QVBoxLayout>
#include <QFrame>

#include <KLocalizedString>
#include <KHelpClient>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>

#include "kregexpeditorprivate.h"

KRegExpEditorGUIDialog::KRegExpEditorGUIDialog(QWidget *parent, const QVariantList &)
    : QDialog(parent)
{
    //qDebug() << "KRegExpEditorGUIDialog::KRegExpEditorGUIDialog(QWidget *parent, const QVariantList &) "<<this;
    setWindowTitle(i18n("Regular Expression Editor"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    _editor = new KRegExpEditorGUI(this);
    layout->addWidget(_editor);

    connect(_editor, &KRegExpEditorGUI::canUndo, this, &KRegExpEditorGUIDialog::canUndo);
    connect(_editor, &KRegExpEditorGUI::canRedo, this, &KRegExpEditorGUIDialog::canRedo);
    connect(_editor, &KRegExpEditorGUI::changes, this, &KRegExpEditorGUIDialog::changes);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::helpRequested, this, &KRegExpEditorGUIDialog::showHelp);
    resize(640, 400);
}

KRegExpEditorGUIDialog::~KRegExpEditorGUIDialog()
{
    //qDebug() << "KRegExpEditorGUIDialog::~KRegExpEditorGUIDialog(QWidget *parent, const QVariantList &) "<<this;
}

QString KRegExpEditorGUIDialog::regExp() const
{
    return _editor->regExp();
}

void KRegExpEditorGUIDialog::setRegExp(const QString &regexp)
{
    _editor->setRegExp(regexp);
}

void KRegExpEditorGUIDialog::redo()
{
    _editor->redo();
}

void KRegExpEditorGUIDialog::undo()
{
    _editor->undo();
}

void KRegExpEditorGUIDialog::doSomething(const QString &method, void *arguments)
{
    _editor->doSomething(method, arguments);
}

void KRegExpEditorGUIDialog::setMatchText(const QString &txt)
{
    _editor->setMatchText(txt);
}

void KRegExpEditorGUIDialog::showHelp()
{
    KHelpClient::invokeHelp(QString(), QStringLiteral("kregexpeditor"));
}
