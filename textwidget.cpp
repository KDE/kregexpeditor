#include "textwidget.h"
#include "textregexp.h"
#include "selectablelineedit.h"
#include <qlayout.h>
#include <qcursor.h>
TextWidget::TextWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                       const char *name)
  :RegExpWidget(editorWindow, parent, name)
{
  init( QString::fromLocal8Bit("") );
}

TextWidget::TextWidget( TextRegExp* regexp,  RegExpEditorWindow* editorWindow,
            QWidget* parent, const char* name )
  : RegExpWidget( editorWindow, parent, name )
{
  init(regexp->text());
}

void TextWidget::init( const QString& txt ) 
{
  QHBoxLayout *lay = new QHBoxLayout(this);
  _edit = new SelectableLineEdit( this, this, "TextWidget::edit" );
  lay->addWidget(_edit);

  _edit->setText( txt );

  connect( _edit, SIGNAL( parentPleaseUpdate() ), this, SLOT(slotUpdate()) );
  setFocusProxy( _edit );
  _edit->installEventFilter( this );
  connect( _edit, SIGNAL( textChanged( const QString & ) ), _editorWindow, SLOT( emitChange() ) );
}


void TextWidget::slotUpdate() 
{
  // I need to force the parent to repaint, as the size change of this
  // widget may not be enough for the parent to change size, and in that
  // case the parent would not repaint, and the text widget would not be
  // resized.
  dynamic_cast<QWidget*>(parent())->repaint();
  _editorWindow->updateContent( this );
}

QSize TextWidget::sizeHint() const
{
  return _edit->sizeHint();
}

void TextWidget::paintEvent( QPaintEvent *e)
{
  RegExpWidget::paintEvent(e);
}

bool TextWidget::updateSelection(bool parentSelected)
{
  bool changed = RegExpWidget::updateSelection( parentSelected );

  // I need to call this function all the time, else the rubber band will
  // not be correctly deleted in the line edit.
  _edit->setSelected( _isSelected );
  return changed;
}

void TextWidget::clearSelection( bool )
{
  _isSelected = false;
  _edit->setSelected( false );
}



RegExp* TextWidget::regExp() const
{
	return new TextRegExp( _edit->text() ); 
}

bool TextWidget::eventFilter( QObject*, QEvent* event)
{
  if ( event->type() == QEvent::MouseButtonRelease ) {
    if ( _editorWindow->isInserting() && acceptWidgetInsert( _editorWindow->insertType() ) ) {
      mouseReleaseEvent( dynamic_cast<QMouseEvent*>(event) );
      return true;
    }
  }
  else if ( event->type() == QEvent::Enter ) {
    if ( _editorWindow->isInserting() ) {
      QCursor cursor;
      if ( acceptWidgetInsert( _editorWindow->insertType() ) )
        cursor = CrossCursor;
      else
        cursor = ForbiddenCursor;
      _edit->setCursor( cursor );
      return true;
    }
    else {
      _edit->setCursor( ibeamCursor );
      return false;
    }  
  }
  return false;
}

#include "textwidget.moc"
