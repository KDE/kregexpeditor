/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "dcbutton.h"

DoubleClickButton::DoubleClickButton(const QPixmap &pixmap, QWidget *parent, const QString &name)
    : QToolButton(parent)
{
    setObjectName(name);
    setIcon(static_cast<QIcon>(pixmap));
}

void DoubleClickButton::mouseDoubleClickEvent(QMouseEvent *)
{
    Q_EMIT doubleClicked();
}

#include "moc_dcbutton.cpp"
