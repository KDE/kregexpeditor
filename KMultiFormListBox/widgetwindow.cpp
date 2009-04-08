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

#include "widgetwindow.h"

#include <QHBoxLayout>

#include <KLocale>
#include <KListWidget>

#include "kmultiformlistboxentry.h"
#include "kmultiformlistboxfactory.h"
#include "windowlistboxitem.h"

WidgetWindow::WidgetWindow(KMultiFormListBoxFactory *factory, KListWidget *lb)
  :KDialog(0)
{
    setCaption( i18n("Widget Configuration") );
    setButtons( Ok | Cancel );
    setDefaultButton( Ok );
  init(factory, lb);
}

WidgetWindow::WidgetWindow(KMultiFormListBoxFactory *factory, KMultiFormListBoxEntry *widget, KListWidget *lb)
  :KDialog(0)
{
    setCaption( i18n("Widget Configuration") );
    setButtons( Ok | Cancel );
    setDefaultButton( Ok );
  init(factory, lb, widget);
}

void WidgetWindow::init(KMultiFormListBoxFactory *factory, KListWidget *lb, KMultiFormListBoxEntry *widget)
{
  listbox = lb;
  myFact = factory;

  QFrame *frame = new QFrame(this);
  setMainWidget( frame );
  QHBoxLayout *lay = new QHBoxLayout(frame);
  lay->setObjectName("WidgetWindow::init::lay");
  lay->setSpacing(-1);
  lay->setMargin(0);

  if (widget != 0) {
    myWidget = widget;
    widget->setParent( frame );
  }
  else {
    myWidget = factory->create(frame);
  }
  QDataStream stream( &_backup, QIODevice::WriteOnly );

  stream.setVersion(QDataStream::Qt_3_1);
  myFact->toStream( myWidget, stream );

  lay->addWidget(myWidget);

  if (widget != 0) {
    initialShow = false;
    myListboxItem = new WindowListboxItem(listbox,myWidget->idxString(), this);
  }
  else {
    initialShow = true;
  }
  connect(this,SIGNAL(okClicked()),this,SLOT(slotOk()));
  connect(this,SIGNAL(cancelClicked()),this,SLOT(slotCancel()));
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
  KDialog::accept();
}

void WidgetWindow::slotCancel()
{
  if (initialShow) {
      deleteLater();
  }
  else {
    QDataStream stream( &_backup, QIODevice::ReadOnly );

    stream.setVersion(QDataStream::Qt_3_1);
    myFact->fromStream( stream, myWidget );
  }
  KDialog::reject();
}

WidgetWindow *WidgetWindow::clone()
{
  WidgetWindow *item = new WidgetWindow(myFact, listbox);
  QByteArray data;
  QDataStream ws( &data, QIODevice::WriteOnly );

  ws.setVersion(QDataStream::Qt_3_1);
  myFact->toStream( myWidget, ws );
  QDataStream rs( &data, QIODevice::ReadOnly );

  rs.setVersion(QDataStream::Qt_3_1);
  myFact->fromStream( rs, item->myWidget );

  item->slotOk();
  return item;
}

void WidgetWindow::display()
{
  QDataStream stream( &_backup, QIODevice::WriteOnly);

  stream.setVersion(QDataStream::Qt_3_1);
  myFact->toStream( myWidget, stream );
  show();
}

KMultiFormListBoxEntry *WidgetWindow::entry()
{
  return myWidget;
}
