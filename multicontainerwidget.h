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
#ifndef __multicontainerwidget
#define __multicontainerwidget

#include "regexpwidget.h"
//Added by qt3to4:
#include <Q3PtrList>

/**
   Abstract RegExp widget class representing widgets having multiple children.
   @internal
*/
class MultiContainerWidget :public RegExpWidget
{
public:
  explicit MultiContainerWidget( RegExpEditorWindow* editorWindow, QWidget* parent = 0);
  virtual bool hasSelection() const;
  virtual void clearSelection();
  virtual void deleteSelection();
  virtual void addNewChild(DragAccepter *accepter, RegExpWidget *child);
  virtual bool updateSelection(bool parentSelected);
  virtual QRect selectionRect() const;
  virtual RegExpWidget* widgetUnderPoint( QPoint globalPos, bool justVisibleWidgets );
  virtual RegExpWidget* findWidgetToEdit( QPoint globalPos );
  virtual void selectWidget( bool sel );
  virtual void updateAll();
  virtual void updateCursorRecursively();

protected:
  void append( RegExpWidget* child );
  Q3PtrList<RegExpWidget> _children;
};


#endif // __multicontainerwidget
