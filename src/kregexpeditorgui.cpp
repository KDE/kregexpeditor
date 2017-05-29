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

#include "kregexpeditorgui.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QSettings>

#include <KLocalizedString>
#include <KHelpClient>
#include "kregexpeditorprivate.h"

const QString KRegExpEditorGUI::version = QString::fromLocal8Bit("1.0");

KRegExpEditorGUI::KRegExpEditorGUI(QWidget *parent, const QVariantList &)
    : QWidget(parent)
    , _editor(new KRegExpEditorPrivate(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
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
    if (method == QString::fromLatin1("setCaseSensitive")) {
        _editor->setCaseSensitive((bool)arguments);
    } else if (method == QString::fromLatin1("setMinimal")) {
        _editor->setMinimal((bool)arguments);
    } else if (method == QString::fromLatin1("setSyntax")) {
        _editor->setSyntax(*((QString *)arguments));
    } else if (method == QString::fromLatin1("setAllowNonQtSyntax")) {
        _editor->setAllowNonQtSyntax((bool)arguments);
    } else {
        qFatal("%s", qPrintable(tr("Method '%1' is not valid!").arg(method)));
    }
}

void KRegExpEditorGUI::setMatchText(const QString &txt)
{
    _editor->setMatchText(txt);
}

