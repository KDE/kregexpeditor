/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include "kmultiformlistbox-shower.h"
#include <QWidget>

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
    KMultiFormListBoxWindowed(KMultiFormListBoxFactory *factory, QWidget *parent, bool showUpDownButtons, bool showHelpButton, const QString &addButtonText);

    KMultiFormListBoxEntryList elements() override;
    const KMultiFormListBoxEntryList elements() const;
    void append(KMultiFormListBoxEntry *) override;
    WindowListboxItem *selected();
    QWidget *qWidget() override
    {
        return this;
    }

    QVBoxLayout *_layout = nullptr;
    KMultiFormListBoxFactory *_factory = nullptr;
    QList<QPushButton *> _buttonList;
    QListWidget *_listbox = nullptr;
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
