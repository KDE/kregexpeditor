#include "widgetfactory.h"
#include "dragaccepter.h"
#include "concwidget.h"
#include "widgetfactory.h"

#include <qpainter.h>

DragAccepter::DragAccepter(RegExpEditorWindow* editorWindow, RegExpWidget *parent,
                           const char *name)
  : RegExpWidget(editorWindow, parent, name == 0 ? "dragaccepter" : name ),
    _drawLine( false )
{
  setAcceptDrops(TRUE);  
}

QSize DragAccepter::sizeHint() const
{
  return QSize(10,10);
}

void DragAccepter::paintEvent(QPaintEvent *e)
{
  QPainter painter(this);
  drawPossibleSelection( painter, size() );

  if ( _drawLine )
    painter.drawLine( 0, height()/2, width(), height()/2 );

  RegExpWidget::paintEvent(e);
}

void DragAccepter::mousePressEvent ( QMouseEvent* event ) 
{
  if ( event->button() == RightButton ) {
    _editorWindow->showRMBMenu( _editorWindow->hasSelection() );
  }
  else {
    RegExpWidget::mousePressEvent( event );
  }
}

void DragAccepter::mouseReleaseEvent( QMouseEvent* event )
{
  if ( _editorWindow->isPasteing() && event->button() == LeftButton ) {
    RegExp* regexp = _editorWindow->pasteData();

    RegExpWidget *newElm = WidgetFactory::createWidget( regexp, _editorWindow, 0 );
    if ( newElm ) {
      ConcWidget* elm;
      if (! (elm = dynamic_cast<ConcWidget*>( newElm ) ) ) {
        elm = new ConcWidget( _editorWindow, newElm, 0 );
      }
      
      Q_ASSERT( elm );
      
      dynamic_cast<RegExpWidget*>(parent())->addNewConcChild(this, elm);
      _editorWindow->updateContent( this );
      _editorWindow->clearSelection( true );
    }
  }
  else if ( _editorWindow->isInserting() && event->button() == LeftButton  ) {
    if ( WidgetFactory::isContainer( _editorWindow->insertType() ) &&
         _editorWindow->pointSelected( mapToGlobal( event->pos() ) ) ) {
      RegExpWidget::mouseReleaseEvent( event );
    }
    else {
      RegExpWidget *child = WidgetFactory::createWidget( _editorWindow, 
                                                         dynamic_cast<QWidget*>(parent()),
                                                         _editorWindow->insertType() );
      if ( child ) {
        dynamic_cast<RegExpWidget*>(parent())->addNewChild(this, child);
        _editorWindow->updateContent( child );
        child->setFocus();
        _editorWindow->clearSelection( true );
      }
    }
  }
  _editorWindow->slotEndActions();
}


void DragAccepter::dragEnterEvent(QDragEnterEvent *event) 
{
  bool selfDrag = (  event->source() && event->source()->topLevelWidget() == topLevelWidget() && _isSelected );
  event->accept(RegExpWidgetDrag::canDecode( event ) && !selfDrag );
}

void DragAccepter::dropEvent(QDropEvent *event)
{
  // The widget will be reparent afterward or part of it will, so no need to give
  // it a parent here.
  RegExpWidget *newElm = RegExpWidgetDrag::decode( event, _editorWindow, 0 );
  ConcWidget* elm;
  if ( !(elm = dynamic_cast<ConcWidget*>( newElm ) ) ) {
    elm = new ConcWidget( _editorWindow, newElm, 0 );
  }
       
  Q_ASSERT( elm );
  
  dynamic_cast<RegExpWidget*>(parent())->addNewConcChild(this, elm);
  dynamic_cast<QWidget*>(parent())->update();
  _editorWindow->updateContent( this );
  
  bool selfDrag = (  event->source() && event->source()->topLevelWidget() == topLevelWidget() );
  if ( ! selfDrag )
    _editorWindow->clearSelection( true );
  else {
    // selection should not be cleared here, since we might want to delete it.
  }
}

RegExp* DragAccepter::regExp() const
{
	return 0;
}
