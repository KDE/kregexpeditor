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
#ifndef __indexWindow
#define __indexWindow

#include <qlistbox.h>

/**
   Post a toplevel listbox synchronously.

   When the user presses the Idx button in the @ref KMultiFormListBox, then a
   listbox with the elements from the KMultiFormListBox must be shown. From this
   listbox the user should chose the element he wants to scroll to.
   This widget takes care of posting this listbox, and ensuring that the
   user can not do anything else before he has chosen an element.

   This widget resembles the behavior of @ref QPopupMenu, the difference
   is, however, that the QPopupMenu can not handle that the elements in the
   menu exceed the size of the screen. This widget can.

   In the future this widget may be replaced with the QPopupMenu if the
   QPopupMenu can handle this situation. But for now - it works!

   @internal
**/
class indexWindow : public QWidget{

Q_OBJECT

public:
  indexWindow();

  /**
     This method inserts an element into the listbox which is shown when
     the @ref exec method is invoked.
  **/
  void insertItem(QString txt);

  /**
     This function shows the index window with the elements inserted using
     the @ref insertItem function. The function will not return before the
     user has chosen an element in the listbox, or have pressed the right
     mouse button outside the window. As a result of returning from this
     function, the listbox is hidden.

     @param start The upper left corner of the pop-up window.
     @param width The width of the window
     @return The index of the element chosen, or -1 if no element has been
     chosen.
  **/
  int exec(const QPoint &start, int width);

protected:
  void finish(int retVal);
  virtual void hideEvent(QHideEvent *h);

protected slots:
  void lbSelected(int);

private:
  QListBox *lb;
  bool lbFinish;
  int itemSelected;

};

#endif /* indexWindow */

