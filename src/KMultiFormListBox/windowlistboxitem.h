/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef __windowlistboxitem
#define __windowlistboxitem

#include <QListWidgetItem>

class WidgetWindow;
class KMultiFormListBoxWindowed;
class KMultiFormListBoxEntry;

/**
   @internal
*/
class WindowListboxItem : public QListWidgetItem
{
    friend class WidgetWindow;
    friend class KMultiFormListBoxWindowed;
public:
    WindowListboxItem(QListWidget *, const QString &, WidgetWindow *);
    ~WindowListboxItem();
    void cloneItem();
    void displayWidget();
    KMultiFormListBoxEntry *entry();

private:
    WidgetWindow *myItem = nullptr;
};

#endif // __windowlistboxitem
