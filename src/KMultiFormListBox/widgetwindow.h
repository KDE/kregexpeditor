#pragma once
/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include <QDialog>

class QListWidget;
class KMultiFormListBoxFactory;
class KMultiFormListBoxEntry;
class WindowListboxItem;

/**
   @internal
*/
class WidgetWindow : public QDialog
{
    friend class KMultiFormListBoxWindowed;
    friend class WindowListboxItem;

    WidgetWindow(KMultiFormListBoxFactory *, QListWidget *);
    WidgetWindow(KMultiFormListBoxFactory *, KMultiFormListBoxEntry *widget, QListWidget *);
    void init(KMultiFormListBoxFactory *, QListWidget *, KMultiFormListBoxEntry *widget = nullptr);
    void display();
    KMultiFormListBoxEntry *entry();
    WidgetWindow *clone();
    ~WidgetWindow();

protected Q_SLOTS:
    void slotOk();
    void slotCancel();

private:
    KMultiFormListBoxFactory *myFact = nullptr;
    KMultiFormListBoxEntry *myWidget = nullptr;
    QByteArray _backup;
    QListWidget *listbox = nullptr;
    WindowListboxItem *myListboxItem = nullptr;
    bool initialShow;
};
