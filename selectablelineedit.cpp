#include "selectablelineedit.h"
#include "regexpwidget.h"
#include <qapplication.h>

SelectableLineEdit::SelectableLineEdit( RegExpWidget* owner, QWidget* parent, const char* name)
  : QLineEdit( parent, name ), _owner(owner)
{
  connect( this, SIGNAL( textChanged( const QString & ) ), this, 
           SLOT( slotKeyPressed() ) );
}

void SelectableLineEdit::setSelected( bool selected )
{
  if ( selected ) {
    QPalette pal = palette().copy();
    pal.setBrush( QColorGroup::Base, gray );
    setPalette( pal );
  }
  else {
    unsetPalette();
  }
  repaint();
}

void SelectableLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
  QApplication::sendEvent( _owner, event );
}

void SelectableLineEdit::dropEvent(QDropEvent *event)
{
  QApplication::sendEvent( _owner, event );
}

QSize SelectableLineEdit::sizeHint() const 
{
  int frameWidth = frame() ? 8 : 4; // from QLineEdit source
  QFontMetrics metrics = fontMetrics();
  int actualSize = metrics.width( text() );
  int charWidth = metrics.maxWidth();
  int height = QLineEdit::sizeHint().height();

  int width;
  if ( hasFocus() ) 
    width = actualSize+6*charWidth + frameWidth;
  else
    width = QMAX(actualSize, charWidth) + frameWidth;
  return QSize( width , height );
}

void SelectableLineEdit::slotKeyPressed() 
{
  int frameWidth = frame() ? 8 : 4; // from QLineEdit source

  QFontMetrics metrics = fontMetrics();
  int actualSize = metrics.width( text() );
  
  if ( actualSize > size().width()-frameWidth ) {
    repaint();
    emit parentPleaseUpdate();
  }    
}

#include "selectablelineedit.moc"
