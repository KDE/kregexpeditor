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

#ifdef QT_ONLY
  #include "compat.h"
#else
  #include <klocale.h>
#endif

#include "zerowidgets.h"
#include "dotregexp.h"
#include "positionregexp.h"
#include <qpainter.h>
//Added by qt3to4:
#include <QPaintEvent>
#include "myfontmetrics.h"
//--------------------------------------------------------------------------------
//                                ZeroWidget
//--------------------------------------------------------------------------------
ZeroWidget::ZeroWidget(QString txt, RegExpEditorWindow* editorWindow,
                       QWidget *parent)
  : RegExpWidget(editorWindow, parent )
{
  _text = txt;
}

void ZeroWidget::addNewChild(DragAccepter *, RegExpWidget *)
{
  qFatal("No children should be added to this widget!");
}

QSize ZeroWidget::sizeHint() const
{
  QFontMetrics metrics = fontMetrics();
  _textSize = HackCalculateFontSize( metrics, _text );
  //  _textSize = metrics.size(0,_text);
  _boxSize = QSize(_textSize.width() + 2*space, _textSize.height() + 2 *space);
  return _boxSize;
}

void ZeroWidget::paintEvent( QPaintEvent *e)
{
  // So what is my Size?
  QSize mySize = sizeHint();

  QPainter painter(this);
  drawPossibleSelection( painter, mySize);

  // Write the text and the rectangle
  painter.drawText(space, space, _textSize.width(), _textSize.height(), 0, _text);
  painter.drawRoundRect(0, 0, _boxSize.width(), _boxSize.height());

  RegExpWidget::paintEvent(e);
}


//--------------------------------------------------------------------------------
//                                AnyCharWidget
//--------------------------------------------------------------------------------
AnyCharWidget::AnyCharWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                             const char *name)
  : ZeroWidget(i18n("Any\nCharacter"), editorWindow, parent,
               name ? name : "AnyCharWidget")
{
}

RegExp* AnyCharWidget::regExp() const
{
	return new DotRegExp( isSelected() );
}


//--------------------------------------------------------------------------------
//                                BegLineWidget
//--------------------------------------------------------------------------------
BegLineWidget::BegLineWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                             const char *name)
  : ZeroWidget(i18n("Line\nStart"), editorWindow, parent,
               name ? name : "BegLineWidget")
{
}

RegExp* BegLineWidget::regExp() const
{
	return new PositionRegExp( isSelected(), PositionRegExp::BEGLINE );

}

//--------------------------------------------------------------------------------
//                                EndLineWidget
//--------------------------------------------------------------------------------
EndLineWidget::EndLineWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                             const char *name)
  : ZeroWidget(i18n("Line\nEnd"), editorWindow, parent, name)
{
}

RegExp* EndLineWidget::regExp() const
{
	return new PositionRegExp( isSelected(), PositionRegExp::ENDLINE );
}

//--------------------------------------------------------------------------------
//                                WordBoundaryWidget
//--------------------------------------------------------------------------------
WordBoundaryWidget::WordBoundaryWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                                       const char *name)
  : ZeroWidget(i18n("Word\nBoundary"), editorWindow, parent,
               name ? name : "WordBoundaryWidget" )
{
}

RegExp* WordBoundaryWidget::regExp() const
{
	return new PositionRegExp( isSelected(), PositionRegExp::WORDBOUNDARY );
}

//--------------------------------------------------------------------------------
//                                NonWordBoundaryWidget
//--------------------------------------------------------------------------------
NonWordBoundaryWidget::NonWordBoundaryWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                                             const char *name)
  : ZeroWidget(i18n("Non-word\nBoundary"), editorWindow, parent,
               name ? name : "NonWordBoundaryWidget" )
{
}

RegExp* NonWordBoundaryWidget::regExp() const
{
	return new PositionRegExp( isSelected(), PositionRegExp::NONWORDBOUNDARY );
}

