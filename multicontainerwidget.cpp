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
#include "multicontainerwidget.h"
#include "dragaccepter.h"

MultiContainerWidget::MultiContainerWidget( RegExpEditorWindow* editorWindow,
                                            QWidget* parent, const char* name)
  :RegExpWidget( editorWindow, parent, name )
{
}

void MultiContainerWidget::append( RegExpWidget* child )
{
  child->reparent( this, QPoint(0,0), false );
  _children.append( child );
  _children.append( new DragAccepter( _editorWindow, this ) );
}

bool MultiContainerWidget::hasSelection() const
{
  if ( _isSelected )
    return true;

  QPtrListIterator<RegExpWidget> it(_children);
  ++it; // Move past the first dragAccepter
	for ( ; *it;  it += 2 ) {
		if ( (*it)->hasSelection() ) {
      return true;
    }
  }
  return false;
}

void MultiContainerWidget::clearSelection()
{
  _isSelected = false;
	for ( unsigned int i = 0; i< _children.count(); i++ ) {
		_children.at(i)->clearSelection();
  }
}


void MultiContainerWidget::deleteSelection()
{
  // run from the back to the front (which we do since we delete items on the run)
  // When deleting children, delete the drag accepter to its right.
  for ( int i = (int) _children.count()-2; i > 0; i -=2 ) {

    RegExpWidget* child = _children.at( i );
    if ( child->isSelected() ) {
      delete _children.at( i+1 );
      _children.remove( i+1 );
      delete child;
      _children.remove(i);
    }
    else if ( child->hasSelection() ) {
      child->deleteSelection();
    }
  }
  _isSelected = false;
  update();
}

void MultiContainerWidget::addNewChild(DragAccepter *accepter, RegExpWidget *child)
{
  for ( unsigned int i=0; i<_children.count(); i+= 2 ) {
    RegExpWidget *ch = _children.at( i );
    if ( ch == accepter ) {
      // Insert the new child
      _children.insert( i+1, child );

      // Insert an accepter as the next element.
      DragAccepter *accepter = new DragAccepter( _editorWindow, this );

      _children.insert( i+2, accepter );

      // These two show's must come here otherwise a paintevent
      // will be invoked, where the invariant, that a accepter is located at
      // every second element.
      child->show();
      accepter->show();

      update();

      return;
    }
  }
  qFatal("Accepter not found in list");
}

bool MultiContainerWidget::updateSelection(bool parentSelected)
{
  bool changed = false;
  bool isSel = _isSelected;
  QMemArray<bool> oldState(_children.count());
  QMemArray<bool> newState(_children.count());

  for (int i = 0; i< (int)_children.count();i++) {
    oldState[i] = _children.at(i)->isSelected();
  }

 RegExpWidget::updateSelection( parentSelected );

  int first;
  int last;

  // scan for the first selected item.
  for (first = 1; first < (int) _children.count(); first+= 2 ) {
    RegExpWidget* child = _children.at(first);
    changed = child->updateSelection( _isSelected ) || changed;
    newState[first] = child->isSelected();
    if ( child->isSelected() )
      break;
  }

  // scan for the last selected item
  for (last = _children.count()-2; last>first; last -= 2) {
    RegExpWidget* child = _children.at(last);
    changed = child->updateSelection( _isSelected ) || changed;
    newState[last] = child->isSelected();
    if ( child->isSelected() )
      break;
  }

  // everything between first and last must be selected.
  for (int j = first+2; j<last; j+=2) {
    RegExpWidget* child = _children.at(j);

   changed = child->updateSelection( true ) || changed;
    newState[j] = true;
  }

  // update drag accepters.
  for (int k = 0; k< (int) _children.count(); k+=2) {
    RegExpWidget* child = _children.at(k);
    bool select;
    if ( k == 0 || k == (int)_children.count()-1) {
      // The elements at the border is only selected if the parent is selected.
      select = _isSelected;
    }
    else {
      // Drag accepters in the middle is selected if the elements at both
      // sides are selected.
      select = newState[k-1] && newState[k+1];
    }

    bool isChildSel = child->isSelected();
    DragAccepter *accepter = dynamic_cast<DragAccepter*>(child);
    if (accepter)
      accepter->_isSelected = select;
    if ( select != isChildSel )
      child->repaint();
  }

  changed = changed || isSel != _isSelected;
  if ( changed ) {
    repaint();
  }

  return changed;
}



QRect MultiContainerWidget::selectionRect() const
{
  if ( _isSelected )
    return QRect( mapToGlobal( QPoint(0,0) ), size() );
  else {
    QRect res;
    QPtrListIterator<RegExpWidget> it(_children);
    ++it; // Move past the first dragAccepter
    for ( ; *it; it +=2 ) {
      if ( (*it)->hasSelection() ) {
        QRect childSel = (*it)->selectionRect();
        if ( res.isNull() )
          res = childSel;
        else {
          QRect newRes;
          newRes.setLeft( QMIN( res.left(), childSel.left() ) );
          newRes.setTop( QMIN( res.top(), childSel.top() ) );
          newRes.setRight( QMAX( res.right(), childSel.right() ) );
          newRes.setBottom( QMAX( res.bottom(), childSel.bottom() ) );
          res = newRes;
        }
      }
    }
    return res;
  }
}

RegExpWidget* MultiContainerWidget::widgetUnderPoint( QPoint globalPos, bool justVisibleWidgets )
{
  unsigned int start, incr;
  if ( justVisibleWidgets ) {
    start = 1;
    incr = 2;
  }
  else {
    start = 0;
    incr = 1;
  }

  for ( unsigned int i = start; i < _children.count(); i+=incr ) {
    RegExpWidget* wid = _children.at(i)->widgetUnderPoint( globalPos, justVisibleWidgets );
    if ( wid )
      return wid;
  }
  if ( justVisibleWidgets )
    return 0;
  else {
    return RegExpWidget::widgetUnderPoint( globalPos, justVisibleWidgets );
  }
}

RegExpWidget* MultiContainerWidget::findWidgetToEdit( QPoint globalPos )
{
  for ( unsigned int i = 1; i < _children.count(); i+=2 ) {
    RegExpWidget* wid = _children.at(i)->findWidgetToEdit( globalPos );
    if ( wid )
      return wid;
  }
  return 0;
}

void MultiContainerWidget::selectWidget( bool sel )
{
  RegExpWidget::selectWidget( sel );
  QPtrListIterator<RegExpWidget> it(_children);
  for ( ; *it ; ++it ) {
    (*it)->selectWidget( sel );
  }
  update();
}

void MultiContainerWidget::updateAll()
{
  for ( QPtrListIterator<RegExpWidget> it(_children); *it ; ++it ) {
    (*it)->updateAll();
  }
  RegExpWidget::updateAll();
}

void MultiContainerWidget::updateCursorRecursively()
{
  for ( QPtrListIterator<RegExpWidget> it(_children); *it ; ++it ) {
    (*it)->updateCursorRecursively();
  }
  updateCursorShape();
}
