/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

// ------------------------------- Description ----------------------------
// This is the KMultiFormListBox Widget. You do not need to inherit from this class,
// your interface to it will be through the methods `append` and `elements'.
//
// To make an instance of the widget, you need to inherit the classes
// `KMultiFormListBoxFactory' and `KMultiFormListBoxEntry'.
// ------------------------------------------------------------------------

// -------------------------------- includes ------------------------------
#include "kmultiformlistbox-shower.h"
#include "kmultiformlistbox.h"
#include <QScrollArea>
//-------------------------------------------------------------------------
class QVBoxLayout;
class KMultiFormListBoxFactory;

/**
   @internal
*/
class KMultiFormListBoxMultiVisible : public QScrollArea, KMultiFormListBoxShower
{
    Q_OBJECT

    friend class KMultiFormListBox;

private:
    /**
       @param factory A factory used to generate the instances of KMultiFormListBoxEntry
       class which is repeated in the KMultiFormListBox
       @param parent A pointer to the parent widget
    */
    explicit KMultiFormListBoxMultiVisible(KMultiFormListBoxFactory *factory, QWidget *parent = nullptr);

    /**
       @return The elements in the KMultiFormListBox
    */
    KMultiFormListBoxEntryList elements() override;
    void append(KMultiFormListBoxEntry *) override;

    QWidget *qWidget() override
    {
        return this;
    }

public Q_SLOTS:

    /**
       This slot should be connected to a button which lets the user know that
       he may get more elements in this KMultiFormListBox by pressing it.
       The button should be labeled ``More Entries'' or something similar.
    */
    void addElement() override; // Adds an empty element to the KMultiFormListBox

protected Q_SLOTS:

    /**
       When this slot is invoked then the Idx menu will be shown.
       @param elm The KMultiFormListBoxEntry element where the Idx button was
       pressed. This information is necessary to be able to calculate the
       location where the drop down menu should be shown
    */
    void showIndexList(KMultiFormListBoxEntry *elm);

protected:
    /**
       Copies the element pointed to by which to the clipboard and removes it
       from the interface.
       This function is accessed from the class @ref CCP.
       @param which A pointer to the element to cut.
    */
    void cut(KMultiFormListBoxEntry *which);

    /**
       Copies the element pointed to by which to the clipboard.
       This function is accessed from the class @ref CCP.
       @param which A pointer to the element to copied.
    */
    void copy(KMultiFormListBoxEntry *which);

    /**
       Paste the element from the clipboard to the KMultiFormListBox
       This function is accessed from the class @ref CCP.
       @param which A pointer to the KMultiFormListBoxEntry element which the
       clipboard element should be inserted before,.
    */
    void paste(KMultiFormListBoxEntry *which);

    /**
       Inserts an empty KMultiFormListBoxEntry into the widget.
       @param which A pointer to the element which the new empty widget
       should be inserted before.
     */

    void addElement(KMultiFormListBoxEntry *);

    friend class CCP;

    void resizeEvent(QResizeEvent *) override;
    void addElemBefore(KMultiFormListBoxEntry *newElm, QWidget *existing);
    void insertElmIntoWidget(KMultiFormListBoxEntry *elm, KMultiFormListBoxEntry *after);
    void showWidget(KMultiFormListBoxEntry *elm);
    void delElement(QWidget *) override;
    void delAnElement() override;
    void addElemBefore(KMultiFormListBoxEntry *newElm, KMultiFormListBoxEntry *existing);
    void updateClipperContent();
    int countElements(WidgetList *);

private:
    // Instance variables

    KMultiFormListBoxFactory *factory;
    WidgetList *elms;
    QPushButton *addBut;
    QByteArray clipboard;
    QVBoxLayout *layout;
};
