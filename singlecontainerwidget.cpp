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
#ifndef QT_ONLY
  #include "singlecontainerwidget.moc"
#endif

#include "singlecontainerwidget.h"
#include "concwidget.h"

SingleContainerWidget::SingleContainerWidget(RegExpEditorWindow* editorWindow,
                                             QWidget* parent, const char* name)
  : RegExpWidget( editorWindow, parent, name )
{
}

bool SingleContainerWidget::updateSelection( bool parentSelected )
{
  bool changed = RegExpWidget::updateSelection( parentSelected );
  changed = _child->updateSelection( _isSelected ) && changed;
  if (changed)
    repaint();

  return changed;
}

bool SingleContainerWidget::hasSelection() const
{
  return _isSelected || _child->hasSelection();
}

void SingleContainerWidget::clearSelection()
{
  _isSelected = false;
  _child->clearSelection();
}

void SingleContainerWidget::deleteSelection()
{
  _child->deleteSelection();
  update();
}

void SingleContainerWidget::applyRegExpToSelection( RegExpType type )
{
  _child->applyRegExpToSelection( type );
}

RegExp* SingleContainerWidget::selection() const
{
  if ( isSelected() )
    return regExp();
  else
    return _child->selection();
}

bool SingleContainerWidget::validateSelection() const
{
  return _child->validateSelection();
}

QRect SingleContainerWidget::selectionRect() const
{
  if ( _isSelected )
    return QRect( mapToGlobal( QPoint(0,0) ), size() );
  else
    return _child->selectionRect();
}

RegExpWidget* SingleContainerWidget::widgetUnderPoint( QPoint globalPos, bool justVisibleWidgets )
{
  RegExpWidget* wid = _child->widgetUnderPoint( globalPos, justVisibleWidgets );
  if ( wid )
    return wid;
  else {
    // The child didn't contain the point, then lets see if this widget
    // itself does.
    return RegExpWidget::widgetUnderPoint( globalPos, justVisibleWidgets );
  }
}

RegExpWidget* SingleContainerWidget::findWidgetToEdit( QPoint globalPos )
{
  RegExpWidget* wid = _child->findWidgetToEdit( globalPos );
  if ( wid )
    return wid;
  else if ( QRect(mapToGlobal(QPoint(0,0)), size()).contains( globalPos ) )
    return this;
  else
    return 0;
}

void SingleContainerWidget::setConcChild( ConcWidget* conc )
{
  delete _child;
  _child = conc;
}

void SingleContainerWidget::selectWidget( bool sel )
{
  RegExpWidget::selectWidget( sel );
  _child->selectWidget( sel );
  update();
}

void SingleContainerWidget::updateAll()
{
  _child->updateAll();
  RegExpWidget::updateAll();
}

void SingleContainerWidget::updateCursorRecursively()
{
  _child->updateCursorRecursively();
  updateCursorShape();
}


