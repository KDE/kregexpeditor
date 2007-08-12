//Added by qt3to4:
#include <QEvent>
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
#ifndef __ccp
#define __ccp


/**
   Helper class for @ref KMultiFormListBoxMultiVisible which is used to install EventFilters.

   When the user presses CTRL + right mouse button then a menu should
   appear which offers him cut and paste capabilities for the entries in
   the KMultiFormListBoxMultiVisible.

  To obtain this an event filter must be install for each subwidget of the
  KMultiFormListBoxMultiVisible. This event filter must catch the right mouse press event and
  post the menu. This requires a widget which has the method @ref
  eventFilter defined. We have this helper class exactly for this purpose.

  For each @ref KMultiFormListBoxEntry in the @ref KMultiFormListBoxMultiVisible widget an instance of
  this class is associated.

  CCP stand for Cut Copy and Paste

  @internal
**/
class CCP :public  QObject {
  Q_OBJECT
private:
  friend class KMultiFormListBoxMultiVisible;
  /**
   * Constructor is private so only the class @ref KMultiFormListBoxMultiVisible may create an
   * instance of this widget.
  **/
  CCP(KMultiFormListBoxMultiVisible *,KMultiFormListBoxEntry *);

  void install(QObject *);
  bool eventFilter(QObject *, QEvent *);

  // Instance variables.

  KMultiFormListBoxMultiVisible *ee;
  KMultiFormListBoxEntry *eee;
};

#endif /* ccp */

