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

#ifndef __kmultiformlistboxwindowed
#define __kmultiformlistboxwindowed

#include <QWidget>
#include "kmultiformlistbox-shower.h"

class QVBoxLayout;
class QListWidgetItem;
class QListWidget;
class WindowListboxItem;
class KMultiFormListBoxFactory;
class QPushButton;

/**
   This class implements the windowed look for a @ref KMultiFormListBox

   @internal
*/
class KMultiFormListBoxWindowed : public QWidget, KMultiFormListBoxShower
{
    Q_OBJECT

    friend class KMultiFormListBox;

private:
    KMultiFormListBoxWindowed(KMultiFormListBoxFactory *factory, QWidget *parent, bool
                              showUpDownButtons, bool
                              showHelpButton, const QString &addButtonText);

    KMultiFormListBoxEntryList elements() override;
    const KMultiFormListBoxEntryList elements() const;
    void append(KMultiFormListBoxEntry *) override;
    WindowListboxItem *selected();
    QWidget *qWidget() override
    {
        return this;
    }

    QVBoxLayout *_layout;
    KMultiFormListBoxFactory *_factory;
    QList<QPushButton *> _buttonList;
    QListWidget *_listbox;
    void delElement(QWidget *) override; // Deletes the given element
    void delAnElement() override;
    void addElement() override;

public Q_SLOTS:
    void addNewElement();

Q_SIGNALS:
    void showHelp();

private Q_SLOTS:
    void slotEditSelected();
    void slotEditSelected(QListWidgetItem *item);
    void slotCopySelected();
    void slotMoveItemUp();
    void slotMoveItemDown();
    void slotDeleteEntry();
    void slotUpdateButtonState();
};

#endif /* kmultiformlistboxwindowed */
