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

#ifndef __kmultiformlistbox
#define __kmultiformlistbox

#include "kmultiformlistboxentry.h"

#include <KLocalizedString>

class QDataStream;
class KMultiFormListBoxFactory;
class KMultiFormListBoxShower;

typedef QList<KMultiFormListBoxEntry *> KMultiFormListBoxEntryList;
typedef QList<QWidget *> WidgetList;

/**
 The main class used to get an KMultiFormListBox widget.

 The KMultiFormListBox widget consist of a sub-widget which is repeated a
 number of times, it is up to the end user to determine the number of times
 the sub widget is repeated, and he may require an additional copy simply
 by pressing a ``Add'' or ``More Entries'' button.  The KMultiFormListBox
 widget has two different faces (i.e. two different end user
 interfaces). One (Windowed) will show a listbox from which the end user
 can access each subwidget containing data by pressing the LMB on a name
 for the element. The other face (MultiVisible) shows all the subwidgets in
 one huge ``Listbox''.

 To use the KMultiFormListBox widget you must create a class which is inherited
 from the @ref KMultiFormListBoxFactory class. This new class must override the
 function `create'. This function must return a freshly made instance of
 the class @ref KMultiFormListBoxEntry (or a subclass of this). The KMultiFormListBoxEntry
 instance is the one inserted into the KMultiFormListBox widget (one instance for
 each sub widget in the KMultiFormListBox widget).

 @author Jesper Kjær Pedersen <blackie@kde.org>
 **/
class KMultiFormListBox : public QWidget
{
    Q_OBJECT

public:

    enum KMultiFormListBoxType {
        MultiVisible, Windowed
    };

    /**
       @param factory A factory used to generate the instances of
       KMultiFormListBoxEntry class which is repeated in the KMultiFormListBox
       @param parent A pointer to the parent widget
     **/
    explicit KMultiFormListBox(KMultiFormListBoxFactory *factory, KMultiFormListBoxType tp = Windowed, QWidget *parent = nullptr, bool showUpDownButtons = true, bool showHelpButton = true,
                               QString addButtonText = i18n("Add"));

    /**
       @return The elements in the KMultiFormListBox.
     **/
    KMultiFormListBoxEntryList elements();
    const KMultiFormListBoxEntryList elements() const;

    /**
         TODO.
    **/
    void append(KMultiFormListBoxEntry *);

    /** write data out to stream */
    void toStream(QDataStream &stream) const;

    /** reads data in from stream */
    void fromStream(QDataStream &stream);

public Q_SLOTS:

    /**
       Adds an empty element to the KMultiFormListBox.

       This slot is only required for the @ref MultiVisible face. It should
       be connected to a button which lets the user know that he may get more
       elements in this KMultiFormListBox by pressing it.  The button should
       be labeled ``More Entries'' or something similar.
     **/
    void addElement(); // Adds an empty element to the KMultiFormListBox

    /**
         Changes the face of the KMultiFormListBox.
         @param face The new face of the KMultiFormListBox
    **/
    void slotChangeFace(KMultiFormListBoxType newFace);

private:
    KMultiFormListBoxShower *theWidget;
    KMultiFormListBoxFactory *_factory;
};

#endif /* kmultiformlistbox */
