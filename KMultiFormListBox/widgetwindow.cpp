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
#include <iostream>

WidgetWindow::WidgetWindow(KMultiFormListBoxFactory *factory, KListBox *lb)
  :KDialogBase(Plain, i18n("Widget Configuration"), Ok | Cancel, Ok, 0, "ConfigWindow", false)
{
  init(factory, lb);
}

WidgetWindow::WidgetWindow(KMultiFormListBoxFactory *factory, KMultiFormListBoxEntry *widget, KListBox *lb)
  :KDialogBase(Plain, i18n("Widget Configuration"), Ok | Cancel, Ok, 0, "ConfigWindow", false)
{
  init(factory, lb, widget);
}

void WidgetWindow::init(KMultiFormListBoxFactory *factory, KListBox *lb, KMultiFormListBoxEntry *widget)
{
  listbox = lb;
  myFact = factory;

  QFrame *frame = plainPage();
  QHBoxLayout *lay = new QHBoxLayout(frame,0,-1,"WidgetWindow::init::lay");

  if (widget != 0) {
    myWidget = widget;
    widget->reparent(frame, 0, QPoint(0,0));
  }
  else {
    myWidget = factory->create(frame);
  }
  QDataStream stream( _backup, IO_WriteOnly );
  myFact->toStream( myWidget, stream );

  lay->addWidget(myWidget);

  if (widget != 0) {
    initialShow = false;
    myListboxItem = new WindowListboxItem(listbox,myWidget->idxString(), this);
  }
  else {
    initialShow = true;
  }
}



WidgetWindow::~WidgetWindow()
{
  delete myWidget;
}

void WidgetWindow::slotOk()
{
  if (initialShow) {
    myListboxItem = new WindowListboxItem(listbox,myWidget->idxString(), this);
  }
  else {
    myListboxItem->setText(myWidget->idxString());
  }
  initialShow = false;
  KDialogBase::slotOk();
}

void WidgetWindow::slotCancel()
{
  if (initialShow) {
    delayedDestruct();
  }
  else {
    QDataStream stream( _backup, IO_ReadOnly );
    myFact->fromStream( stream, myWidget );
  }
  KDialogBase::slotCancel();
}

WidgetWindow *WidgetWindow::clone()
{
  WidgetWindow *item = new WidgetWindow(myFact, listbox);
  QByteArray data;
  QDataStream ws( data, IO_WriteOnly );
  myFact->toStream( myWidget, ws );
  QDataStream rs( data, IO_ReadOnly );
  myFact->fromStream( rs, item->myWidget );

  item->slotOk();
  return item;
}

void WidgetWindow::display()
{
  QDataStream stream( _backup, IO_WriteOnly);
  myFact->toStream( myWidget, stream );
  show();
}

KMultiFormListBoxEntry *WidgetWindow::entry()
{
  return myWidget;
}
