#include "compoundwidget.h"
#include <kdialogbase.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qpainter.h>
#include "concwidget.h"
#include "kwidgetstreamer.h"
#include "compoundregexp.h"

//================================================================================

CompoundDetailWindow::CompoundDetailWindow( QWidget* parent, const char* name ) 
  :QWidget( parent, name )
{
  QGridLayout* layout = new QGridLayout( this, 2, 2 );
  
  QLabel* label = new QLabel( i18n("&Title"), this);
  layout->addWidget( label, 0, 0 );
  layout->setSpacing( 6 );
  
  _title = new QLineEdit( this );
  layout->addWidget( _title, 0, 1 );
  label->setBuddy( _title );
  
  label = new QLabel( i18n("&Description"), this );
  layout->addWidget( label, 1, 0 );
  label->setAlignment( AlignTop );
  
  _description  = new QMultiLineEdit( this );
  layout->addWidget( _description, 1, 1 );
  label->setBuddy( _description );

  _title->setFocus();
  
}

QString CompoundDetailWindow::title() const
{
  return _title->text();
}

QString CompoundDetailWindow::description() const
{
  return _description->text();
}

void CompoundDetailWindow::setTitle( QString txt ) 
{
  _title->setText( txt );
}

void CompoundDetailWindow::setDescription( QString txt ) 
{
  _description->setText( txt );
}

//================================================================================

CompoundWidget::CompoundWidget( RegExpEditorWindow* editorWindow, QWidget* parent,
                                const char* name ) 
  :SingleContainerWidget( editorWindow, parent, name == 0 ? "CompoundWidget" : name )
{
  _child = new ConcWidget( editorWindow, this );
  init();
}

CompoundWidget::CompoundWidget( CompoundRegExp* regexp, RegExpEditorWindow* editorWindow, 
                                QWidget* parent, const char* name )
  : SingleContainerWidget( editorWindow, parent, name  == 0 ? "CompoundWidget" : name )
{
  init();
  _content->setTitle( regexp->title() );
  _content->setDescription( regexp->description() );
  RegExpWidget* child = WidgetFactory::createWidget( regexp->child(), _editorWindow, this );
  if ( !( _child = dynamic_cast<ConcWidget*>(child) ) ) {
    _child = new ConcWidget( _editorWindow, child, this );
  }
  
  _hidden = regexp->hidden();
}

void CompoundWidget::init( ) 
{
  _configWindow = new KDialogBase( this, "_configWindow", true,
                                   i18n("Configure Widget"), 
                                   KDialogBase::Ok | KDialogBase::Cancel );
  _content = new CompoundDetailWindow( _configWindow );
  _configWindow->setMainWidget( _content );

  connect( _configWindow, SIGNAL(cancelClicked()), this, SLOT(slotConfigCanceled())) ;
  connect(_configWindow, SIGNAL(finished()), this, SLOT(slotConfigWindowClosed()));

  _down = SmallIcon( QString::fromLocal8Bit( "1downarrow" ));
  _up = SmallIcon( QString::fromLocal8Bit( "1uparrow" ) );

  _hidden = false;
  _backRefId = -1;
}

QSize CompoundWidget::sizeHint() const
{
  QFontMetrics metrics = fontMetrics();
  _childSize = _child->sizeHint();  
  _textSize = metrics.size( 0, _content->title() );
  
  int width, height;
  
  if ( _hidden ) {
    _pixmapSize = _up.size();
    width = 2*pw + QMAX( 2*bdSize+_textSize.width(), 2*bdSize+_pixmapSize.width());
    height = _pixmapSize.height() + 2*bdSize + _textSize.height()+pw;
  }
  else {
    _pixmapSize = _down.size();
    int headerLineWidth = 2*pw + 2*bdSize + _pixmapSize.width();
    if ( _textSize.width() != 0)
      headerLineWidth += 3*bdSize + _textSize.width();
    
    width = QMAX( 2*pw + _childSize.width(), headerLineWidth );
    height = QMAX( _textSize.height(), _pixmapSize.height() ) +
      2*bdSize + _childSize.height() + pw;
  }
  return QSize( width, height );
  
}

void CompoundWidget::paintEvent( QPaintEvent *e )
{
  QSize mySize = sizeHint();
  
  QPainter painter(this);
  drawPossibleSelection( painter, mySize);

  int horLineY, childY;

  // draw top line
  if ( _hidden ) {
    horLineY = _pixmapSize.height()/2;
    childY = _pixmapSize.height() + bdSize;
    _pixmapPos = QPoint( mySize.width()-pw-bdSize-_pixmapSize.width(), 0 );
    painter.drawLine( pw, horLineY, _pixmapPos.x(), horLineY );
    painter.drawLine( mySize.width() - bdSize - pw, horLineY,
                      mySize.width(), horLineY);
    painter.drawPixmap( _pixmapPos, _up );
  }
  else {
    int maxH = QMAX( _textSize.height(), _pixmapSize.height() );
    int offset = 0;
    horLineY = maxH/2;
    childY = maxH+bdSize;
    
    painter.drawLine(pw, horLineY, bdSize, horLineY);
    if ( _textSize.width() != 0 ) {
      offset += pw + 2*bdSize;
    
      painter.drawText(offset, horLineY-_textSize.height()/2,
                       bdSize+_textSize.width(), horLineY+_textSize.height()/2,
                       0, _content->title());
      offset += _textSize.width() + bdSize;
    }
    
    _pixmapPos = QPoint( mySize.width()-pw-bdSize-_pixmapSize.width(),
                         horLineY - _pixmapSize.height()/2 );
    
    painter.drawLine( offset, horLineY, _pixmapPos.x(), horLineY );
    painter.drawPixmap( _pixmapPos, _down );
    
    painter.drawLine( mySize.width()-bdSize-pw, horLineY, mySize.width(), horLineY );
  }

  // draw rest frame
  painter.drawLine(0, horLineY, 0, mySize.height() );
  painter.drawLine( mySize.width()-pw, horLineY, mySize.width()-pw, mySize.height() );
  painter.drawLine( 0, mySize.height()-pw, mySize.width(), mySize.height()-pw );

  // place/size child
  if ( _hidden ) {
    _child->hide();
    painter.drawText( pw+bdSize, childY, 
                      pw+bdSize+_textSize.width(), childY+_textSize.height(), 0,
                      _content->title() );
  }
  else {
    QSize curSize = _child->size();
    QSize newSize = QSize( QMAX( _child->sizeHint().width(), mySize.width()-2*pw),
                           _child->sizeHint().height());
    
    _child->move( pw, childY );
    if ( curSize != newSize ) {
      _child->resize(newSize);
      // I resized the child, so give it a chance to relect thus.
      _child->update();
    }

    _child->show();
  }

  RegExpWidget::paintEvent( e );
}

void CompoundWidget::slotConfigWindowClosed()
{
  _editorWindow->updateContent( 0 );
  update();
}

void CompoundWidget::slotConfigCanceled()
{
  QDataStream stream( _backup, IO_ReadOnly );
  KWidgetStreamer streamer;
  streamer.fromStream( stream, _content );
  repaint();
}

RegExp* CompoundWidget::regExp() const
{
  return new CompoundRegExp( _content->title(), _content->description(), _hidden, 
                             _child->regExp() );
}

void CompoundWidget::mousePressEvent( QMouseEvent* event )
{
  if ( event->button() == LeftButton && 
       QRect( _pixmapPos, _pixmapSize ).contains( event->pos() ) ) {
    // Skip otherwise we will never see the mouse release
    // since it is eaten by Editor window.
  }
  else
    SingleContainerWidget::mousePressEvent( event );
}

void CompoundWidget::mouseReleaseEvent( QMouseEvent* event)
{
  if ( event->button() == LeftButton && 
       QRect( _pixmapPos, _pixmapSize ).contains( event->pos() ) ) {
    _hidden = !_hidden;
    _editorWindow->updateContent( 0 );
    repaint(); // is this necesary?
    _editorWindow->emitChange();
  }
  else
    SingleContainerWidget::mouseReleaseEvent( event );
}

int CompoundWidget::edit()
{
  _configWindow->move(QCursor::pos() - QPoint(_configWindow->sizeHint().width()/2, 
                                              _configWindow->sizeHint().height()/2)  );
  QDataStream stream( _backup, IO_WriteOnly );
  KWidgetStreamer streamer;
  streamer.toStream( _content, stream );
  return _configWindow->exec();
}

int nextId() 
{
  static int counter = 0;
  return ++counter;
}


#include "compoundwidget.moc"
