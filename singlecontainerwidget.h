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
#ifndef __singlecontainer
#define __singlecontainer
#include "regexpwidget.h"

/**
   Abstract class representing RegExp widgets having a single child.
*/
class SingleContainerWidget :public RegExpWidget
{
Q_OBJECT

public:
  SingleContainerWidget( RegExpEditorWindow* editorWindow, QWidget* parent);

  virtual bool updateSelection( bool parentSelected );
  virtual bool hasSelection() const;
  virtual void clearSelection();
  virtual void deleteSelection();
  virtual void applyRegExpToSelection( RegExpType type );
  virtual RegExp* selection() const;
  virtual bool validateSelection() const;
  virtual QRect selectionRect() const;
  virtual RegExpWidget* widgetUnderPoint( QPoint globalPos, bool justVisibleWidgets );
  virtual RegExpWidget* findWidgetToEdit( QPoint globalPos );
  virtual void setConcChild( ConcWidget* child );
  virtual void selectWidget( bool sel );
  virtual void updateAll();
  virtual void updateCursorRecursively();

protected:
  ConcWidget *_child;
};


#endif // __singlecontainer
