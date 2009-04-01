//Added by qt3to4:
#include <QList>
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
#ifndef __kmultiformlistboxshower
#define __kmultiformlistboxshower

// -------------------------------- includes ------------------------------
//-------------------------------------------------------------------------
typedef QList<KMultiFormListBoxEntry *> KMultiFormListBoxEntryList ;

/**
   Abstract class defining the interface for widgets showing a number of @ref KMultiFormListBoxEntry.

   @internal
*/
class KMultiFormListBoxShower {

public:
  virtual ~KMultiFormListBoxShower(){}
  virtual KMultiFormListBoxEntryList elements() = 0;

  /** Appends the given argument to the list */
  virtual void append(KMultiFormListBoxEntry *) = 0;

  /** Return the element as a QWidget */
	virtual QWidget* qWidget() = 0;

  /** Adds an empty element. */
  virtual void addElement() = 0;

  /** Deletes an element at the given index. */
	virtual void delElement(QWidget *) = 0;

  /** Deletes any element from the list */
	virtual void delAnElement() = 0;
};


#endif /* kmultiformlistboxshower */

