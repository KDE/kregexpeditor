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
#ifndef __configwindow
#define __configwindow

#ifdef QT_ONLY
  #include "compat.h"
  #include <q3listbox.h>
#else
  #include <kdialog.h>
  #include <k3listbox.h>
#endif

#include "kmultiformlistbox-windowed.h"

/**
   @internal
*/
class WidgetWindow :public KDialog
{
  friend class KMultiFormListBoxWindowed;
  friend class WindowListboxItem;

  WidgetWindow(KMultiFormListBoxFactory *, K3ListBox *);
  WidgetWindow(KMultiFormListBoxFactory *, KMultiFormListBoxEntry *widget, K3ListBox *);
  void init(KMultiFormListBoxFactory *, K3ListBox *, KMultiFormListBoxEntry *widget = 0);
  void display();
  KMultiFormListBoxEntry *entry();
  WidgetWindow *clone();
  ~WidgetWindow();

protected slots:
  void slotOk();
  void slotCancel();

private:
  KMultiFormListBoxFactory *myFact;
  KMultiFormListBoxEntry *myWidget;
  QByteArray _backup;
  K3ListBox *listbox;
  WindowListboxItem *myListboxItem;
  bool initialShow;
};

#endif // configwindow
