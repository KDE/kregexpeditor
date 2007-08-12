/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <Qt::blackie@kde.org>
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

#include "regexpwidget.h"

#ifdef QT_ONLY
  #include "compat.h"
#else
  #include "regexpwidget.moc"
#endif

#include <iostream>
#include <qpainter.h>
#include <qcursor.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QApplication>
#include <QEvent>
#include "concwidget.h"
#include "dragaccepter.h"

const int RegExpWidget::pw = 1;
const int RegExpWidget::bdSize = 5;
const int RegExpWidget::space = 5;

RegExpWidget::RegExpWidget(RegExpEditorWindow* editorWindow, QWidget *parent)
  : QWidget(parent, Qt::WNoMousePropagation ),
    _editorWindow( editorWindow ),  _isSelected( false ), _isToplevel( false )
{
}


void RegExpWidget::addNewChild(DragAccepter *, RegExpWidget *)
{
  qFatal("This widget should not expect any children");
}

void RegExpWidget::addNewConcChild(DragAccepter *, ConcWidget *)
{
  qFatal("This widget should not expect any children");
}

void RegExpWidget::setConcChild(ConcWidget *)
{
  qFatal("This widget should not expect any children");
}

bool RegExpWidget::updateSelection(bool parentSelected)
{
  bool selected = ( parentSelected ||
                    _editorWindow->selectionOverlap( mapToGlobal( QPoint(0,0) ), size() ) )
    && !_isToplevel;

  if ( _isSelected != selected ) {
    // Selection state changed
    _isSelected = selected;
    repaint();
    return true;
  }
  return false;
}

void RegExpWidget::drawPossibleSelection( QPainter& painter, QSize mySize )
{
  if ( _isSelected ) {
    painter.fillRect( 0, 0, mySize.width(), mySize.height(), QBrush( Qt::gray ) );
  }
}

bool RegExpWidget::isSelected() const
{
  return _isSelected;
}

bool RegExpWidget::hasSelection() const
{
  return _isSelected;
}

void RegExpWidget::clearSelection()
{
  _isSelected = false;
}

void RegExpWidget::applyRegExpToSelection( RegExpType )
{
  qFatal("This method should not be called for this widget");
}

void RegExpWidget::deleteSelection()
{
  qFatal("This method should be overridden if needed!");
}

RegExp* RegExpWidget::selection() const
{
  return regExp();
}

int RegExpWidget::edit()
{
  qFatal("This method should be overridden if needed!");
  return 0; // Compiler shut up
}

void RegExpWidget::mousePressEvent ( QMouseEvent* event )
{
  if ( _editorWindow->isPasteing() || _editorWindow->isInserting() )
    return;

  if ( event->button() == Qt::LeftButton ) {
    if ( ! _editorWindow->pointSelected( QCursor::pos() ) ) {
      _editorWindow->clearSelection( true );
      if ( dynamic_cast<DragAccepter*>(this) == 0 && dynamic_cast<ConcWidget*>(this) == 0 ) {
        selectWidget( true );
      }
    }

    QMouseEvent ev( event->type(), mapTo(_editorWindow, event->pos()),
                    event->button(), Qt::MouseButtons(event->state() & Qt::MouseButtonMask),
		    event->modifiers());
    QApplication::sendEvent( _editorWindow, &ev );
  }
  else if ( event->button() == Qt::RightButton ) {
    _editorWindow->showRMBMenu( true );
  }

  // currently (Qt3.0) it seems like qt do not accept that the accept flag is set,
  // and thus sends the event to the parent - given that the following line is in.
  // It doesn't make any change to leave it out.
  // 25 Oct. 2001 19:03 -- Jesper K. Pedersen
  //  QWidget::mousePressEvent( event );
}

void RegExpWidget::mouseReleaseEvent( QMouseEvent* )
{
  if ( _editorWindow->isInserting() && acceptWidgetInsert( _editorWindow->insertType() ) ) {
    if ( !_editorWindow->hasSelection() )
      _isSelected = true;

    _editorWindow->applyRegExpToSelection( _editorWindow->insertType() );
    _editorWindow->clearSelection( true );
    _editorWindow->updateContent( this );
    _editorWindow->slotEndActions();
    _editorWindow->updateCursorUnderPoint();
  }
}

QRect RegExpWidget::selectionRect() const
{
  return QRect( mapToGlobal( QPoint(0,0) ), size() );
}


void RegExpWidget::enterEvent( QEvent * )
{
  updateCursorShape();
}

void RegExpWidget::updateCursorShape()
{
  QCursor cursor;

  if ( _editorWindow->isPasteing() ) {
    if ( acceptWidgetPaste() )
      cursor =  Qt::CrossCursor;
    else
      cursor =  Qt::ForbiddenCursor;
  }
  else if ( _editorWindow->isInserting() ) {
    if ( acceptWidgetInsert( _editorWindow->insertType() ) )
      cursor = Qt::CrossCursor;
    else
      cursor = Qt::ForbiddenCursor;
  }
  else
    cursor = Qt::ArrowCursor;

  setCursor( cursor );
}

void RegExpWidget::updateCursorRecursively()
{
  updateCursorShape();
}


bool RegExpWidget::acceptWidgetPaste() const
{
  return false;
}

bool RegExpWidget::acceptWidgetInsert( RegExpType tp ) const
{
  return WidgetFactory::isContainer( tp );
}

RegExpWidget* RegExpWidget::widgetUnderPoint( QPoint globalPos, bool )
{
  if ( QRect(mapToGlobal( QPoint(0,0) ), size() ).contains( globalPos ) )
    return this;
  else
    return 0;
}

void RegExpWidget::selectWidget( bool sel)
{
  _isSelected = sel;
  update();
}

void RegExpWidget::updateAll()
{
  update();
}


