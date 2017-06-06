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

#include "auxbuttons.h"

#include <QToolButton>
#include <QWhatsThis>

#include <QIcon>
#include <KIconLoader>
#include <KLocalizedString>

AuxButtons::AuxButtons(QWidget *parent, const QString &name)
    : QToolBar(name, parent)
{
    _undo = new QToolButton(this);
    _undo->setIcon(QIcon::fromTheme(QStringLiteral("edit-undo")));
    addWidget(_undo);
    connect(_undo, &QAbstractButton::clicked, this, &AuxButtons::undo);
    _undo->setToolTip(i18n("Undo"));

    _redo = new QToolButton(this);
    _redo->setIcon(QIcon::fromTheme(QStringLiteral("edit-redo")));
    addWidget(_redo);
    connect(_redo, &QAbstractButton::clicked, this, &AuxButtons::redo);
    _redo->setToolTip(i18n("Redo"));

    _cut = new QToolButton(this);
    _cut->setIcon(QIcon::fromTheme(QStringLiteral("edit-cut")));
    addWidget(_cut);
    connect(_cut, &QAbstractButton::clicked, this, &AuxButtons::cut);
    _cut->setToolTip(i18n("Cut"));

    _copy = new QToolButton(this);
    _copy->setIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    addWidget(_copy);
    connect(_copy, &QAbstractButton::clicked, this, &AuxButtons::copy);
    _copy->setToolTip(i18n("Copy"));

    _paste = new QToolButton(this);
    _paste->setIcon(QIcon::fromTheme(QStringLiteral("edit-paste")));
    addWidget(_paste);
    connect(_paste, &QAbstractButton::clicked, this, &AuxButtons::paste);
    _paste->setToolTip(i18n("Paste"));

    _save = new QToolButton(this);
    _save->setIcon(QIcon::fromTheme(QStringLiteral("document-save")));
    addWidget(_save);
    connect(_save, &QAbstractButton::clicked, this, &AuxButtons::save);
    _save->setToolTip(i18n("Save"));

    QToolButton *button = new QToolButton(this);
    button->setIcon(QIcon::fromTheme(QStringLiteral("help-contextual")));
    addWidget(button);
    connect(button, &QAbstractButton::clicked, this, &AuxButtons::slotEnterWhatsThis);

    _undo->setEnabled(false);
    _redo->setEnabled(false);
}

void AuxButtons::slotEnterWhatsThis()
{
    QWhatsThis::enterWhatsThisMode();
}

void AuxButtons::slotCanUndo(bool b)
{
    _undo->setEnabled(b);
}

void AuxButtons::slotCanRedo(bool b)
{
    _redo->setEnabled(b);
}

void AuxButtons::slotCanCut(bool b)
{
    _cut->setEnabled(b);
}

void AuxButtons::slotCanCopy(bool b)
{
    _copy->setEnabled(b);
}

void AuxButtons::slotCanPaste(bool b)
{
    _paste->setEnabled(b);
}

void AuxButtons::slotCanSave(bool b)
{
    _save->setEnabled(b);
}
