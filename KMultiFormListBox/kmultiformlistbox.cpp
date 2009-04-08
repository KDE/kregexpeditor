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

#include "kmultiformlistbox-multivisible.h"
#include "kmultiformlistbox-windowed.h"

#include <QHBoxLayout>

#include "kmultiformlistboxfactory.h"

KMultiFormListBox::KMultiFormListBox( KMultiFormListBoxFactory *factory, KMultiFormListBoxType tp, QWidget *parent,
                                      bool showUpDownButtons, bool showHelpButton, QString addButtonText )
    : QWidget( parent )
{
	switch ( tp ) {

	case MultiVisible:
    theWidget = new KMultiFormListBoxMultiVisible( factory, this );
		break;

	case Windowed:
    theWidget = new KMultiFormListBoxWindowed( factory, this, showUpDownButtons,
																							 showHelpButton, addButtonText);
		break;
	}

  QWidget *widget = theWidget->qWidget();

  QHBoxLayout *layout = new QHBoxLayout( this );
	_factory = factory;
  layout->addWidget( widget );
}

void KMultiFormListBox::append( KMultiFormListBoxEntry *element )
{
  theWidget->append( element );
}

void KMultiFormListBox::addElement()
{
  theWidget->addElement();
}

KMultiFormListBoxEntryList KMultiFormListBox::elements()
{
  return theWidget->elements();
}

const KMultiFormListBoxEntryList KMultiFormListBox::elements() const
{
  return const_cast<KMultiFormListBox*>(this)->elements();
}

void KMultiFormListBox::slotChangeFace( KMultiFormListBoxType /*newFace*/ )
{
	// TODO
	// kDebug() << "It's not possible yet to change the face on the fly." 
	//					<< "Please let me (blackie@kde.org) know that you need it, and I'll work on it" << endl;
}

void KMultiFormListBox::toStream( QDataStream& stream ) const
{
  const KMultiFormListBoxEntryList elms = elements();
  stream << elms.count();
  foreach ( KMultiFormListBoxEntry * entry, elms)
    _factory->toStream( entry, stream );
}

void KMultiFormListBox::fromStream( QDataStream& stream )
{
  unsigned int fromCount, toCount;
  stream >> fromCount;

  toCount = elements().count();

  // adds/remove elements in the to list, to make it have the correct length.
  for (unsigned int j=toCount; j< fromCount; ++j) {
    addElement();
  }
  for (unsigned int k=fromCount; k < toCount; ++k) {
    theWidget->delAnElement();
  }

  KMultiFormListBoxEntryList elms = elements();
  foreach ( KMultiFormListBoxEntry * entry, elms)
    _factory->fromStream( stream, entry );
}

#include "kmultiformlistbox.moc"
