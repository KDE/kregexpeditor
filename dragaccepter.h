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
#ifndef __dragaccepter
#define __dragaccepter

#include "regexpwidget.h"
class MultiContainerWidget;

/**
   RegExp widget representing a "spot" in which new RegExp widgets can be inserted.
   @internal
*/
class DragAccepter :public RegExpWidget
{
  friend class MultiContainerWidget;

public:
  DragAccepter(RegExpEditorWindow* editorWindow, RegExpWidget *parent,
               const char *name = 0);
  QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return DRAGACCEPTER; }
  void setDrawLine( bool drawLine ) { _drawLine = drawLine; }

protected:
  virtual void mousePressEvent ( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dropEvent(QDropEvent *event);
  virtual void paintEvent(QPaintEvent *);
  virtual bool acceptWidgetInsert( RegExpType ) const { return true; }
  virtual bool acceptWidgetPaste() const { return true; }

private:
  bool _drawLine;
};

#endif // __dragaccepter
