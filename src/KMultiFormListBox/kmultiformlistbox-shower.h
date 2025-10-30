// Added by qt3to4:
#include <QList>
/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/
#ifndef __kmultiformlistboxshower
#define __kmultiformlistboxshower

#include <QWidget>

// -------------------------------- includes ------------------------------
class KMultiFormListBoxEntry;
//-------------------------------------------------------------------------
typedef QList<KMultiFormListBoxEntry *> KMultiFormListBoxEntryList;

/**
   Abstract class defining the interface for widgets showing a number of @ref KMultiFormListBoxEntry.

   @internal
*/
class KMultiFormListBoxShower
{
public:
    virtual ~KMultiFormListBoxShower()
    {
    }

    virtual KMultiFormListBoxEntryList elements() = 0;

    /** Appends the given argument to the list */
    virtual void append(KMultiFormListBoxEntry *) = 0;

    /** Return the element as a QWidget */
    virtual QWidget *qWidget() = 0;

    /** Adds an empty element. */
    virtual void addElement() = 0;

    /** Deletes an element at the given index. */
    virtual void delElement(QWidget *) = 0;

    /** Deletes any element from the list */
    virtual void delAnElement() = 0;
};

#endif /* kmultiformlistboxshower */
