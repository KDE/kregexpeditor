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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
#include "kmultiformlistbox-multivisible.h"
#include "kmultiformlistbox-windowed.h"
#include <kdebug.h>

KMultiFormListBox::KMultiFormListBox( KMultiFormListBoxFactory *factory, KMultiFormListBoxType tp, QWidget *parent,
																		  bool showUpDownButtons, bool showHelpButton, QString addButtonText,
																		  const char *name )  : QWidget( parent, name )
{
	switch ( tp ) {

	case MultiVisible:
    theWidget = new KMultiFormListBoxMultiVisible( factory, this, "KMultiFormListBox Widget" );
		break;

	case Windowed:
    theWidget = new KMultiFormListBoxWindowed( factory, this, showUpDownButtons,
																							 showHelpButton, addButtonText, "KMultiFormListBox Widget" );
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
	kdDebug() << "It's not possible yet to change the face on the fly." << endl
						<< "Please let me (blackie@kde.org) know that you need it, and I'll work on it" << endl;
}

void KMultiFormListBox::toStream( QDataStream& stream ) const
{
  const KMultiFormListBoxEntryList elms = elements();
  stream << elms.count();
  for ( QPtrListIterator<KMultiFormListBoxEntry> it(elms); *it; ++it)
    _factory->toStream( *it, stream );
}

void KMultiFormListBox::fromStream( QDataStream& stream )
{
  unsigned int fromCount, toCount;
  stream >> fromCount;

  toCount = elements().count();

  // adds/remove elements in the to list, to make it have the correct length.
  for (unsigned int i=toCount; i< fromCount; ++i) {
    addElement();
  }
  for (unsigned int i=fromCount; i < toCount; ++i) {
    theWidget->delAnElement();
  }

  KMultiFormListBoxEntryList elms = elements();
  for (QPtrListIterator<KMultiFormListBoxEntry> it(elms); *it; ++it)
    _factory->fromStream( stream, *it );
}



#include "kmultiformlistbox.moc"
