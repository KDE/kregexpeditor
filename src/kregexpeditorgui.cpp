/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "kregexpeditorgui.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QSettings>

#include <KLocalizedString>
#include <KHelpClient>
#include "kregexpeditorprivate.h"

const QString KRegExpEditorGUI::version = QStringLiteral("1.0");

KRegExpEditorGUI::KRegExpEditorGUI(QWidget *parent, const QVariantList &)
    : QWidget(parent)
    , _editor(new KRegExpEditorPrivate(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_editor);

    connect(_editor, &KRegExpEditorPrivate::canUndo, this, &KRegExpEditorGUI::canUndo);
    connect(_editor, &KRegExpEditorPrivate::canRedo, this, &KRegExpEditorGUI::canRedo);
    connect(_editor, &KRegExpEditorPrivate::changes, this, &KRegExpEditorGUI::changes);

    QSettings settings;
    restoreGeometry(settings.value(QStringLiteral("geometry")).toByteArray());
}

KRegExpEditorGUI::~KRegExpEditorGUI()
{
    delete _editor;
}

QString KRegExpEditorGUI::regExp() const
{
    return _editor->regexp();
}

void KRegExpEditorGUI::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue(QStringLiteral("geometry"), saveGeometry());

    QWidget::closeEvent(event);
}

void KRegExpEditorGUI::redo()
{
    _editor->slotRedo();
}

void KRegExpEditorGUI::undo()
{
    _editor->slotUndo();
}

void KRegExpEditorGUI::setRegExp(const QString &regexp)
{
    _editor->slotSetRegexp(regexp);
}

void KRegExpEditorGUI::doSomething(const QString &method, void *arguments)
{
    if (method == QLatin1String("setCaseSensitive")) {
        _editor->setCaseSensitive((bool)arguments);
    } else if (method == QLatin1String("setMinimal")) {
        _editor->setMinimal((bool)arguments);
    } else if (method == QLatin1String("setSyntax")) {
        _editor->setSyntax(*((QString *)arguments));
    } else if (method == QLatin1String("setAllowNonQtSyntax")) {
        _editor->setAllowNonQtSyntax((bool)arguments);
    } else {
        qFatal("%s", qPrintable(tr("Method '%1' is not valid!").arg(method)));
    }
}

void KRegExpEditorGUI::setMatchText(const QString &txt)
{
    _editor->setMatchText(txt);
}
