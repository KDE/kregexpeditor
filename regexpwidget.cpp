#include "regexpwidget.h"
#include "drag.h"
#include "regexp.h"
#include "editorwindow.h"
#include <iostream>
#include <qpainter.h>
#include <qapplication.h>
#include <qcursor.h>
#include "concwidget.h"
#include "dragaccepter.h"

RegExpWidget::RegExpWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                           const char *name)
  : QWidget(parent, name ? name : "RegExpWidget", WNoMousePropagation ), 
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
    painter.fillRect( 0, 0, mySize.width(), mySize.height(), QBrush( gray ) );
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
  qFatal("This method should be overriden if needed!");
}

RegExp* RegExpWidget::selection() const
{
  return regExp();
}

int RegExpWidget::edit()
{
  qFatal("This method should be overriden if needed!");
  return 0; // Compiler shut up
}

void RegExpWidget::mousePressEvent ( QMouseEvent* event )
{  
  if ( _editorWindow->isPasteing() || _editorWindow->isInserting() )
    return;
  
  if ( event->button() == LeftButton ) {
    if ( ! _editorWindow->pointSelected( QCursor::pos() ) ) {
      _editorWindow->clearSelection( true );
      if ( dynamic_cast<DragAccepter*>(this) == 0 && dynamic_cast<ConcWidget*>(this) == 0 ) {
        selectWidget( true );
      }
    }
    
    QMouseEvent ev( event->type(), mapTo(_editorWindow, event->pos()),
                    event->button(), event->state());
    QApplication::sendEvent( _editorWindow, &ev );
  }
  else if ( event->button() == RightButton ) {
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
      cursor =  CrossCursor;
    else
      cursor =  ForbiddenCursor;
  }
  else if ( _editorWindow->isInserting() ) {
    if ( acceptWidgetInsert( _editorWindow->insertType() ) )
      cursor = CrossCursor;
    else
      cursor = ForbiddenCursor;
  }
  else
    cursor = arrowCursor;

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


#include "regexpwidget.moc"
