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
//---------------------
// ccp = Cut-Copy-Paste
//---------------------

#include "kmultiformlistbox-multivisible.h"
#include "ccp.h"
#include <qobjectlist.h>
#include <qobjectdict.h>
#include <qpopupmenu.h>
#include <klocale.h>

CCP::CCP(KMultiFormListBoxMultiVisible *ee_, KMultiFormListBoxEntry *eee_) : QObject() {
  ee = ee_;
  eee = eee_;
  install(eee);
}

void CCP::install(QObject *elm)
{
  elm->installEventFilter(this);
  const QObjectList *children = elm->children();
  if (children) {
    QObjectListIt it = QObjectListIt(*children);

    while (QObject *child=it.current()) {
      if (dynamic_cast<KMultiFormListBoxMultiVisible *>(child)) {
        // Stop if the widget is an KMultiFormListBox, as this widget has its own cut/copy/paste
      }
      else {
        install(child);
      }
      ++it;
    }
  }
}

// This function post the Cut/Copy/Paste menu
bool CCP::eventFilter(QObject *, QEvent *event)
{
  if (event->type() != QEvent::MouseButtonPress ||
      ((QMouseEvent *) event)->button() != RightButton ||
      ((QMouseEvent *) event)->state() != QEvent::ControlButton) {
    return false;
  }

  QPoint pos = ((QMouseEvent *) event)->globalPos();

  QPopupMenu *menu = new QPopupMenu();
  menu->insertItem(i18n("Cut"),1);
  menu->insertItem(i18n("Copy"),2);
  menu->insertItem(i18n("Paste"),3);
  menu->insertItem(i18n("Insert Blank"),4);

  int res=menu->exec(pos);
  switch (res) {
  case 1: ee->cut(eee); break;
  case 2: ee->copy(eee); break;
  case 3: ee->paste(eee); break;
  case 4: ee->addElement(eee); break;
  }
  return true;
}

