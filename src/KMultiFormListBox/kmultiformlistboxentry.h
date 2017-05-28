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

#ifndef __kmultiformlistboxentry
#define __kmultiformlistboxentry

#include <QtWidgets/QWidget>

class QPushButton;

/**
   This widget must be the base class for an entry widget used in the @ref
   KMultiFormListBox class. It is necessary for you to inherit this class to get any
   information attached to the elements in the KMultiFormListBox.

   The KMultiFormListBox widget features a fast scrolling mechanism through the Idx
   button. If you want to use this in you KMultiFormListBox, then you must do the
   following:
   @li Create a @ref QPushButton as a sub-widget to your KMultiFormListBoxEntry.
   @li Override the @ref indexButton method to return your QPushButton
   @li Override the @ref idxString to return a @ref QString
   with a textual representation of the content in this KMultiFormListBoxEntry. This
   string will be used in the drop-down box which the user gets when he
   presses the Idx button.
   @li The drop down window must be aligned horizontal to some widget
   (which should be next to the Idx button, to ensure a good looking GUI. The
   position of the drop down widget may be specified in two ways: (1)
   override the @ref valueWidget method to return a widget, to align with
   (that is the upper right corner of the drop down  window will be the
   same as the lower right corner of this widget) or (2) override the @ref
   indexWindowPos method to return a start point for the drop down window and
   a width.
 **/
class KMultiFormListBoxEntry : public QWidget
{
    Q_OBJECT

public:
    KMultiFormListBoxEntry(QWidget *parent) : QWidget(parent)
    {
    }

    virtual QPushButton *indexButton()
    {
        return nullptr;
    }

    virtual QWidget *valueWidget()
    {
        return nullptr;
    }

    virtual void indexWindowPos(QPoint *start, int *width); // both variables are return values.

    // This function must return a string representing the KMultiFormListBox. This is
    // used when showing the fast-search menu available from the `Idx' button.
    virtual QString idxString()
    {
        return QString::fromLatin1("");
    }

public Q_SLOTS:
    void acceptIndexButton();

Q_SIGNALS:
    void gotoIndex(KMultiFormListBoxEntry *);
};

#endif /* kmultiformlistboxentry */
