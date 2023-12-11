/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "windowlistboxitem.h"
#include "widgetwindow.h"

WindowListboxItem::WindowListboxItem(QListWidget *lb, const QString &text, WidgetWindow *windowWidget)
    : QListWidgetItem(text, lb)
    , myItem(windowWidget)
{
}

WindowListboxItem::~WindowListboxItem()
{
    delete myItem;
}

void WindowListboxItem::displayWidget()
{
    myItem->display();
}

void WindowListboxItem::cloneItem()
{
    myItem->clone();
}

KMultiFormListBoxEntry *WindowListboxItem::entry()
{
    return myItem->entry();
}
