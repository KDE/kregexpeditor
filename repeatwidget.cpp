#include "repeatwidget.h"
#include "concwidget.h"
#include "repeatregexp.h"
#include "selectablelineedit.h"
#include <qpainter.h>
#include <qcursor.h>
#include <klocale.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qgrid.h>
#include <kdialogbase.h>
#include <qvbuttongroup.h>
#include "kwidgetstreamer.h"

RepeatWidget::RepeatWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                           const char *name)
  : SingleContainerWidget(editorWindow, parent, name ? name : "RepeatWidget")
{
  _child = new ConcWidget(editorWindow, this);
  init();
}

RepeatWidget::RepeatWidget( RepeatRegExp* regexp, RegExpEditorWindow* editorWindow,
                            QWidget* parent, const char* name )
  :SingleContainerWidget( editorWindow, parent, name )
{
  init();
  RegExpWidget* child = WidgetFactory::createWidget( regexp->child(), editorWindow, this );
  if ( ! (_child = dynamic_cast<ConcWidget*>( child ) ) ) 
    _child = new ConcWidget( editorWindow, child, this );

  if ( regexp->max() == -1 ) {
    if ( regexp->min() == 0 ) {
      _content->set( RepeatRangeWindow::ANY, regexp->min(), regexp->max() );
    }
    else {
      _content->set( RepeatRangeWindow::ATLEAST, regexp->min(), regexp->max() );
    }
  }
  else {
    if ( regexp->min() == 0 ) {
      _content->set( RepeatRangeWindow::ATMOST, regexp->min(), regexp->max() );
    }
    else if ( regexp->min() == regexp->max() ) {
      _content->set( RepeatRangeWindow::EXACTLY, regexp->min(), regexp->max() );
    }
    else {
      _content->set( RepeatRangeWindow::MINMAX, regexp->min(), regexp->max() );
    }
  }
}

void RepeatWidget::init()
{
  _configWindow = new KDialogBase( this, "_configWindow", true,
                                   i18n("Number of Times to Repeat Content"),
                                   KDialogBase::Ok | KDialogBase::Cancel);
  _content = new RepeatRangeWindow( _configWindow );
  _configWindow->setMainWidget( _content );
  connect( _configWindow, SIGNAL( cancelClicked() ), this, SLOT( slotConfigCanceled() ) );
  connect(_configWindow, SIGNAL(finished()), this, SLOT(slotConfigWindowClosed()));
}


QSize RepeatWidget::sizeHint() const
{
  // TODO: Merge with LookAheadWidget::sizeHint
  QFontMetrics metrics = fontMetrics();
  _textSize = metrics.size( 0, _content->text() );

  _childSize = _child->sizeHint();

  int height = _textSize.height() + bdSize + _childSize.height() + bdSize + 2*pw;
  int width  = 2 * pw + QMAX(_childSize.width(), 4*bdSize + _textSize.width());
  return QSize(width,height);
}

void RepeatWidget::paintEvent( QPaintEvent *e )
{
  // TODO: Merge with LookAheadWidget::paintEvent
  QSize mySize = sizeHint();
  QPainter painter(this);
  
  drawPossibleSelection( painter, mySize );
  
  // move the child to its position and resize it.
  _child->move(pw,_textSize.height()+bdSize);
  QSize curChildSize = _child->size();
  QSize newChildSize = QSize(mySize.width() - 2*pw, _childSize.height());
  if ( curChildSize != newChildSize ) {
    _child->resize(newChildSize);
    // I resized the child, so give it a chance to relect thus.
    _child->update();
  }
  
  // Draw the border and the text.
  int startY = _textSize.height()/2;

  // Top lines and text
  painter.drawLine(pw,startY,bdSize,startY);
  painter.drawText(pw+2*bdSize,0,_textSize.width(), _textSize.height(),0,
                   _content->text());
  int offset = pw + 3*bdSize + _textSize.width();
  painter.drawLine(offset,startY,mySize.width()-pw,startY);

  // horizontal lines
  painter.drawLine(0,startY,0,mySize.height()-pw);
  painter.drawLine(mySize.width()-pw,startY,mySize.width()-pw, mySize.height()-pw);

  // buttom line
  painter.drawLine(0, mySize.height()-pw, mySize.width()-pw, mySize.height()-pw);

  SingleContainerWidget::paintEvent(e);
}

RegExp* RepeatWidget::regExp() const 
{
	return new RepeatRegExp( _content->min(), _content->max(), _child->regExp() );
}

void RepeatWidget::slotConfigWindowClosed()
{
  _editorWindow->updateContent( 0 );
  update();
}

void RepeatWidget::slotConfigCanceled()
{
  QDataStream stream( _backup, IO_ReadOnly );
  KWidgetStreamer streamer;
  streamer.fromStream( stream, _content );
  repaint();
}

int RepeatWidget::edit() 
{
  _configWindow->move(QCursor::pos() - QPoint(_configWindow->sizeHint().width()/2,
                                              _configWindow->sizeHint().height()/2)  );
  QDataStream stream( _backup, IO_WriteOnly );
  KWidgetStreamer streamer;
  streamer.toStream( _content, stream );
  
  return _configWindow->exec();
}

//--------------------------------------------------------------------------------
RepeatRangeWindow::RepeatRangeWindow( QWidget* parent, const char* name )
  : QVBox( parent, name ? name : "RepeatRangeWindow" )
{
  setSpacing( 6 );
  
  _group = new QVButtonGroup( i18n("Times to match"), this, "groupbox" );
  
  // Any number of times
  QRadioButton* radioBut = new QRadioButton(i18n("Any number of times (including zero times)"),
                                            _group,
                                            "RepeatRangeWindow::choice any times");
  
  _group->insert(radioBut, ANY);

  QWidget* container = new QWidget( _group );
  QHBoxLayout* lay = new QHBoxLayout( container );
  QGrid* grid = new QGrid( 3, container );
  grid->setSpacing( 5 );
  
  lay->addWidget( grid );
  lay->addStretch( 1 );
  
  createLine( grid, i18n( "At least" ), &_leastTimes, ATLEAST );
  createLine( grid, i18n( "At most" ), &_mostTimes, ATMOST );
  createLine( grid, i18n( "Exactly" ), &_exactlyTimes, EXACTLY );

  // from ___ to ___ times
  radioBut = new QRadioButton(i18n( "From" ), grid, "RepeatRangeWindow::from");
  _group->insert( radioBut, MINMAX );
  
  _rangeFrom = new QSpinBox( 1, 999, 1, grid);

  QHBox* box = new QHBox( grid );
  box->setSpacing( 5 );

  (void) new QLabel(i18n( "to" ), box);
  _rangeTo = new QSpinBox( 1, 999, 1, box );
  (void) new QLabel( i18n( "time(s)" ), box );

  connect( _rangeFrom, SIGNAL( valueChanged( int ) ), this, SLOT( slotUpdateMaxVal( int ) ) );
  connect( _rangeTo, SIGNAL( valueChanged( int ) ), this, SLOT( slotUpdateMinVal( int ) ) );

  // set a default button.
  _group->setButton(ANY);
  slotItemChange( ANY );
  

  connect( _group, SIGNAL( clicked( int ) ), this, SLOT( slotItemChange( int ) ) );
}


void RepeatRangeWindow::createLine( QWidget* parent, QString text, QSpinBox** spin, REPEATTYPE tp ) 
{

  QRadioButton* radioBut = new QRadioButton(text, parent);
  *spin = new QSpinBox( 1, 999, 1, parent);
  (*spin)->setValue(1);
  
  (void) new QLabel(i18n("time(s)"), parent);
  _group->insert(radioBut, tp);
}

void RepeatRangeWindow::slotItemChange( int which ) 
{
  _leastTimes->setEnabled( false );
  _mostTimes->setEnabled( false );
  _exactlyTimes->setEnabled( false );
  _rangeFrom->setEnabled( false );
  _rangeTo->setEnabled( false );
  
  switch ( which ) {
  case ANY: break;
  case ATLEAST: _leastTimes->setEnabled( true ); break;
  case ATMOST: _mostTimes->setEnabled( true ); break;
  case EXACTLY: _exactlyTimes->setEnabled( true ); break;
  case MINMAX:
    _rangeFrom->setEnabled( true );
    _rangeTo->setEnabled( true );
    break;
  }
}

void RepeatRangeWindow::slotUpdateMinVal( int maxVal ) 
{
  if ( _rangeFrom->value() > maxVal ) {
    _rangeFrom->setValue( maxVal );
  }
}

void RepeatRangeWindow::slotUpdateMaxVal( int minVal ) 
{
  if ( _rangeTo->value() < minVal ) {
    _rangeTo->setValue( minVal );
  }
}

QString RepeatRangeWindow::text()
{
  switch ( _group->id(_group->selected()) ) {
  case ANY: return i18n("Repeated any number of times");
  case ATLEAST: return i18n("Repeated at least %1 times").arg( _leastTimes->value() );
  case ATMOST: return i18n("Repeated at most %1 times").arg( _mostTimes->value() );
  case EXACTLY: return i18n("Repeated exactly %1 times").arg( _exactlyTimes->value() );
  case MINMAX: return i18n("Repeated from %1 to %2 times")
                 .arg( _rangeFrom->value() ).arg( _rangeTo->value() );
  }
  qFatal("Fall through!");
  return QString::fromLocal8Bit("");
}
  
int RepeatRangeWindow::min() 
{
  switch ( _group->id(_group->selected()) ) {
  case ANY: return 0;
  case ATLEAST: return _leastTimes->value();
  case ATMOST: return 0;
  case EXACTLY: return _exactlyTimes->value();
  case MINMAX: return _rangeFrom->value();
  }
  qFatal("Fall through!");
  return -1;
}

int RepeatRangeWindow::max() 
{
  switch ( _group->id(_group->selected()) ) {
  case ANY: return -1;
  case ATLEAST: return -1;
  case ATMOST: return _mostTimes->value();
  case EXACTLY: return _exactlyTimes->value();
  case MINMAX:  return _rangeTo->value();
  }
  qFatal("Fall through!");
  return -1;
}

void RepeatRangeWindow::set( REPEATTYPE tp, int min, int max ) 
{
  _group->setButton( tp );
  switch ( tp ) {
  case ANY:
    break;
  case ATLEAST: 
    _leastTimes->setValue( min );
    break;
  case ATMOST:
    _mostTimes->setValue( max );
    break;
  case EXACTLY:
    _exactlyTimes->setValue( min );
    break;
  case MINMAX:
    _rangeFrom->setValue( min );
    _rangeTo->setValue( max );
    break;
  }
}

    
#include "repeatwidget.moc"
