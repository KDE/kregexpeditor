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
#ifndef __altnwidget
#define __altnwidget

#include "multicontainerwidget.h"
class AltnRegExp;

/**
   RegExp widget for alternatives.

   @internal
*/
class AltnWidget :public MultiContainerWidget
{
public:
  AltnWidget( RegExpEditorWindow* editorWindow, QWidget *parent,
              const char *label = 0);
  AltnWidget( AltnRegExp* regexp, RegExpEditorWindow* editorWindow,
              QWidget* parent, const char* name = 0);
  virtual void addNewChild(DragAccepter *accepter, RegExpWidget *child);
  virtual QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual void applyRegExpToSelection( RegExpType type );
  virtual RegExpType type() const { return ALTN; }
  virtual RegExp* selection() const;
  virtual bool validateSelection() const;
  virtual void setConcChild(ConcWidget *child);
  virtual void addNewConcChild(DragAccepter *accepter, ConcWidget *child);
  virtual void deleteSelection();

protected:
  virtual void paintEvent( QPaintEvent* e );

private:
  /**
     This functions runs through all the dragaccepters and calls @ref
     DragAccepter::setDrawLine.
  */
  void updateDrawLineInfo();

  QString _text;
  mutable QSize _textSize;

  mutable int _maxSelectedWidth;
  mutable int _childrenWidth;
  mutable int _childrenHeight;
};

#endif // __altnwidget
