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
  #include <kmessagebox.h>
#endif

#include "altnwidget.h"
#include "dragaccepter.h"
#include "concwidget.h"
#include "altnregexp.h"
#include <qpainter.h>

AltnWidget::AltnWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                       const char *name)
  :MultiContainerWidget(editorWindow, parent, name == 0 ? "AltnWidget" : name)
{
  DragAccepter *accepter = new DragAccepter(editorWindow, this);
  accepter->resize(0,0); // See note (1) in Comments
  _children.append(accepter);
  _text = i18n("Alternatives");
}

AltnWidget::AltnWidget( AltnRegExp* regexp, RegExpEditorWindow* editorWindow,
            QWidget* parent, const char* name)
  :MultiContainerWidget( editorWindow, parent, name )
{
  DragAccepter *accepter = new DragAccepter(editorWindow, this);
  accepter->resize(0,0); // See note (1) in Comments
  _children.append(accepter);
  _text = i18n("Alternatives");

  RegExpList list = regexp->children();
  for ( RegExpListIt it(list); *it; ++it ) {
    RegExpWidget* child = WidgetFactory::createWidget( *it, editorWindow, this );
    ConcWidget* conc;
    if (  ! (conc = dynamic_cast<ConcWidget*>( child ) ) ) {
      conc = new ConcWidget( editorWindow, child, parent );
    }
    append( conc );
  }
  updateDrawLineInfo();
}


void AltnWidget::addNewChild(DragAccepter *accepter, RegExpWidget *child)
{
  ConcWidget *conc = new ConcWidget(_editorWindow, child,this);
  MultiContainerWidget::addNewChild( accepter, conc );
  updateDrawLineInfo();
  repaint();
}

void AltnWidget::setConcChild(ConcWidget *child)
{
  addNewConcChild( dynamic_cast<DragAccepter*>(_children.at(0)), child );
}

void AltnWidget::addNewConcChild(DragAccepter *accepter, ConcWidget *child)
{
  child->reparent(this, QPoint(0,0), false);
  MultiContainerWidget::addNewChild( accepter, child );
  updateDrawLineInfo();
}


QSize AltnWidget::sizeHint() const
{
  QPtrListIterator<RegExpWidget> it(_children);
  // Skip the first child, as we only need half of the size of the first and the
  // last drag accepter. Does, however, not apply when there only is onw child.
  if ( _children.count() != 1 )
    ++it;

  _childrenWidth = 0;
  _childrenHeight = 0;

  for ( ; *it ; ++it) {
    QSize thisChildSize = (*it)->sizeHint();
    _childrenWidth = QMAX(_childrenWidth, thisChildSize.width());
    _childrenHeight += thisChildSize.height();
  }

  // Now add the size of the header
  QFontMetrics metrics = fontMetrics();
  _textSize = metrics.size(0,_text);

  int headerWidth = _textSize.width() + 2 * bdSize + 2;

  _childrenWidth = QMAX(_childrenWidth, headerWidth);

  return QSize(_childrenWidth + 2*pw, _childrenHeight + _textSize.height() + 1*pw );
}

void AltnWidget::paintEvent( QPaintEvent *e)
{
  Q_ASSERT( dynamic_cast<DragAccepter*>(_children.at(0)) );
  // if this fails, then I should check the location of the show()
  Q_ASSERT( _children.count() == 1 ||
          ( _children.count() >=3 &&
            dynamic_cast<DragAccepter*>(_children.at(_children.count()-1)) ) );

  int offset = 0;
  QSize mySize = sizeHint();

  QPainter painter(this);
  drawPossibleSelection( painter, mySize );

  //-------------------------------------- First draw the border and text
  int startY = _textSize.height()/2;

  painter.drawLine(0, startY, bdSize, startY);
  int xOffset = bdSize +1;

  painter.drawText(xOffset,0,_textSize.width(), _textSize.height(),0,_text);
  xOffset += _textSize.width() +1;
  painter.drawLine(xOffset,startY,mySize.width(), startY);
  painter.drawLine(0,startY,0,mySize.height());
  painter.drawLine(mySize.width()-pw, startY, mySize.width()-pw, mySize.height());
  painter.drawLine(0,mySize.height()-pw, mySize.width()-pw, mySize.height()-pw);

  //---- Run through all the children and place them at the correct location.
  offset = _textSize.height();
  xOffset = pw;

  for (unsigned int i = 0; i < _children.count(); i++ ) {

    RegExpWidget* child = _children.at(i);

    QSize childSize = child->sizeHint();
    QSize curChildSize = child->size();

    //-------------------------------------- place the child
    int x = xOffset;
    int y = offset;
    int h = childSize.height();
    if (  ( _children.count() != 1 ) && ( i == 0 || i == _children.count() -1 ) ) {
      // first and last DragAccepter should only be half size.
      h /= 2;
    }

    int w = _childrenWidth;
    child->setGeometry( x, y, w, h );
    if ( w != curChildSize.width() || h != curChildSize.height()  ) {
      // I resized the child, so give it a chance to relect thus.
      child->update();
    }

    offset += h;
  }
  MultiContainerWidget::paintEvent( e );
}

RegExp* AltnWidget::regExp() const
{
	AltnRegExp *regexp = new AltnRegExp( isSelected() );

  QPtrListIterator<RegExpWidget> it(_children);
  ++it; // start with the second element
	for ( ; *it; it+=2 ) {
    regexp->addRegExp( (*it)->regExp() );
	}

	return regexp;
}

void AltnWidget::applyRegExpToSelection( RegExpType type )
{
  for ( unsigned int i=1; i < _children.count(); i += 2 ) {
    RegExpWidget* child = _children.at( i );
    if ( child->hasSelection() ) {
      child->applyRegExpToSelection( type );
    }
  }
}


RegExp* AltnWidget::selection() const
{
  if ( isSelected() )
    return regExp();
  else {
    QPtrListIterator<RegExpWidget> it(_children);
    ++it; // Skip past DragAccepter
    for ( ; *it; it+=2 ) {
      if ( (*it)->hasSelection() ) {
        return (*it)->selection();
      }
    }
  }
  qFatal("Selection not found");
  return 0; // compiler shut up.
}

bool AltnWidget::validateSelection() const
{
  if ( _isSelected ) {
    return true;
  }

  bool foundASelection = false;
  QPtrListIterator<RegExpWidget> it(_children);
  ++it; // Skip past DragAccepter
  for ( ; *it; it+=2 ) {
    if ( (*it)->hasSelection() ) {
      if ( foundASelection ) {
          KMessageBox::information( const_cast<AltnWidget*>(this),
                                    i18n( "Selecting several alternatives is "
                                          "currently not supported." ),
                                    i18n( "Selection Invalid" ) );
        _editorWindow->clearSelection( true );
        return false;
      }
      else {
        foundASelection = true;
      }
    }
  }
  return true;
}

void AltnWidget::updateDrawLineInfo()
{
  for ( unsigned int i=0; i < _children.count(); i+=2 ) {
    bool line = ( i != 0 && i!= _children.count()-1 );
    DragAccepter *accepter = dynamic_cast<DragAccepter*>(_children.at(i));
    if (accepter)
      accepter->setDrawLine( line );
  }
}

void AltnWidget::deleteSelection()
{
  MultiContainerWidget::deleteSelection();
  updateDrawLineInfo();
}

