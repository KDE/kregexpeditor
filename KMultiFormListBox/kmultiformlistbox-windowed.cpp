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
#include "widgetwindow.h"
#include "windowlistboxitem.h"
#include <kdebug.h>
#include <kmessagebox.h>

KMultiFormListBoxWindowed::KMultiFormListBoxWindowed(KMultiFormListBoxFactory *factory, QWidget *parent,
																	 bool showUpDownButtons, bool showHelpButton,
																	 QString addButtonText,const char *name)
  : QWidget( parent, name )
{
  _layout = new QVBoxLayout(this);

	QHBoxLayout *innerLayout = new QHBoxLayout();
	_layout->addLayout(innerLayout);

  _listbox = new KListBox(this,"listbox");
  _listbox->setSelectionMode(QListBox::Single);
  innerLayout->addWidget(_listbox);

  QVBoxLayout *buttons = new QVBoxLayout();
  innerLayout->addLayout(buttons);

  QPushButton *but = new QPushButton(addButtonText, this,"Add Button");
  buttons->addWidget(but,0);
  connect(but, SIGNAL(clicked()), this, SLOT(addNewElement()));

  but = new QPushButton(i18n("Edit"), this,"Edit Button");
  buttons->addWidget(but,0);
  connect(but,SIGNAL(clicked()), this, SLOT(slotEditSelected()));
  connect(_listbox, SIGNAL(doubleClicked(QListBoxItem *)), this, SLOT(slotEditSelected(QListBoxItem *)));
	_buttonList.append(but);

  but = new QPushButton(i18n("Delete"), this, "Delete Button");
  buttons->addWidget(but,0);
  connect(but, SIGNAL(clicked()), this, SLOT(slotDeleteEntry()));
	_buttonList.append(but);

  but = new QPushButton(i18n("Copy"), this, "Copy Button");
  buttons->addWidget(but,0);
  connect(but, SIGNAL(clicked()), this, SLOT(slotCopySelected()));
	_buttonList.append(but);

	if (showUpDownButtons) {
		but = new QPushButton(i18n("Up"), this, "Up Button");
		buttons->addWidget(but, 0);
		connect(but, SIGNAL(clicked()), this, SLOT(slotMoveItemUp()));
		_buttonList.append(but);

		but = new QPushButton(i18n("Down"), this, "Down Button");
		buttons->addWidget(but, 0);
		connect(but, SIGNAL(clicked()), this, SLOT(slotMoveItemDown()));
		_buttonList.append(but);
	}

	if (showHelpButton) {
		but = new QPushButton(i18n("Help"), this, "Help Button");
		buttons->addWidget(but, 0);
		connect(but, SIGNAL(clicked()), this, SIGNAL(showHelp()));
	}

  buttons->addStretch(1);
  _factory = factory;
	slotUpdateButtonState();

}

KMultiFormListBoxEntryList KMultiFormListBoxWindowed::elements()
{
  KMultiFormListBoxEntryList list;
  for (unsigned int i=0; i < _listbox->count(); i++) {
    WindowListboxItem *item = (WindowListboxItem *) _listbox->item(i);
    list.append(item->entry());
  }
  return list;
}

void KMultiFormListBoxWindowed::delElement(QWidget */*elm*/)
{
  kdDebug() << "KMultiFormListBoxWindowed::delElement NOT YET IMPLEMENTED"<<endl;
	// TODO
}

void KMultiFormListBoxWindowed::delAnElement()
{
  kdDebug() << "KMultiFormListBoxWindowed::delAnElement NOT YET IMPLEMENTED"<<endl;
  // TODO
}


void KMultiFormListBoxWindowed::append(KMultiFormListBoxEntry *elm)
{
  (void) new WidgetWindow(_factory, elm, _listbox);
	slotUpdateButtonState();
}

void KMultiFormListBoxWindowed::addNewElement()
{
	kdDebug() << "addNewElement " << _factory << "," << _listbox << endl;

  QWidget *widget = new WidgetWindow(_factory, _listbox);
	widget->show();
	connect(widget, SIGNAL(finished()), this, SLOT(slotUpdateButtonState()));
}

void KMultiFormListBoxWindowed::addElement()
{
  new WidgetWindow(_factory, _listbox);
	slotUpdateButtonState();
}

void KMultiFormListBoxWindowed::slotEditSelected(QListBoxItem *item)
{
  ((WindowListboxItem *) item)->displayWidget();
}

void KMultiFormListBoxWindowed::slotEditSelected()
{
  WindowListboxItem *item = selected();
  if (item) {
    slotEditSelected(item);
  }
}

void KMultiFormListBoxWindowed::slotDeleteEntry()
{
  WindowListboxItem *item = selected();
  if (item) {
		int answer =
			KMessageBox::warningYesNo(0, i18n("Delete item \"%1\"?").arg(item->text()),i18n("Delete Item"));
		if (answer == KMessageBox::Yes) {
			delete item;
			slotUpdateButtonState();
		}
  }
}

void KMultiFormListBoxWindowed::slotCopySelected()
{
  WindowListboxItem *item = selected();
  if (item) {
    item->clone();
  }
}

WindowListboxItem *KMultiFormListBoxWindowed::selected()
{
  int i = _listbox->currentItem();
  if (i == -1) {
    return 0;
  } else {
    return (WindowListboxItem *) _listbox->item(i);
  }
}

void KMultiFormListBoxWindowed::slotMoveItemUp()
{
	WindowListboxItem *item = selected();
	if (item == 0)
		return;

	int index = _listbox->index(item);
	if (index != 0) {
		_listbox->takeItem(item);
		_listbox->insertItem(item, index-1);
		_listbox->setCurrentItem(item);
	}
}

void KMultiFormListBoxWindowed::slotMoveItemDown()
{
	WindowListboxItem *item = selected();
	if (item == 0)
		return;

	unsigned int index = _listbox->index(item);
	if (index < _listbox->count()) {
		_listbox->takeItem(item);
		_listbox->insertItem(item, index+1);
		_listbox->setCurrentItem(item);
	}
}

void KMultiFormListBoxWindowed::slotUpdateButtonState()
{
	bool on = (_listbox->count() != 0);
	for (unsigned int i=0; i<_buttonList.count(); i++) {
		_buttonList.at(i)->setEnabled(on);
	}
}
#include "kmultiformlistbox-windowed.moc"
