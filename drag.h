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
#ifndef __drag_h
#define __drag_h
#include <qdragobject.h>
#include <qpoint.h>
#include <qsize.h>
#include <qwidget.h>
#include "widgetfactory.h"
class RegExp;

class RegExpWidget;

/**
   Class used for drag and drop in the RegExp widget.
   @internal
*/
class RegExpWidgetDrag :public QDragObject
{
public:
  RegExpWidgetDrag( RegExp* regexp , QWidget* dragSource);
  ~RegExpWidgetDrag();
  virtual const char * format ( int i = 0 ) const;
  virtual QByteArray encodedData ( const char * ) const;
  static bool canDecode(QDragMoveEvent* event);
  static RegExpWidget* decode(QDropEvent* event, RegExpEditorWindow* window,
                              QWidget* parent);
private:
  RegExp* _regexp;
};

#endif //__drag_h
