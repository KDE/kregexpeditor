#include "lookaheadwidget.h"
#include "lookaheadregexp.h"
#include "concwidget.h"
#include <klocale.h>
#include <qpainter.h>

LookAheadWidget::LookAheadWidget( RegExpEditorWindow* editorWindow, RegExpType tp, QWidget* parent, const char* name )
  :SingleContainerWidget(editorWindow, parent, name ? name : "LookAheadWidget" ), _tp(tp)
{
  _child = new ConcWidget( editorWindow, this );
  init();
}

LookAheadWidget::LookAheadWidget( LookAheadRegExp* regexp, RegExpEditorWindow* editorWindow, RegExpType tp, 
                                  QWidget* parent, const char* name )
  :SingleContainerWidget( editorWindow, parent, name ? name : "LookAheadWidget" ), _tp(tp)
{
  RegExpWidget* child = WidgetFactory::createWidget( regexp->child(), editorWindow, this );
  if ( ! (_child = dynamic_cast<ConcWidget*>( child ) ) ) 
    _child = new ConcWidget( editorWindow, child, this );

  init();
}

void LookAheadWidget::init()
{
  if ( _tp == POSLOOKAHEAD )
    _text = i18n("Positive Assertion");
  else
    _text = i18n("Negative Assertion");
}

RegExp* LookAheadWidget::regExp() const
{
  return new LookAheadRegExp( ( (_tp == POSLOOKAHEAD) ? LookAheadRegExp::POSITIVE : LookAheadRegExp::NEGATIVE),
                              _child->regExp() );
}

QSize LookAheadWidget::sizeHint() const
{
  // TODO: Merge with RepeatWidget::sizeHint
  QFontMetrics metrics = fontMetrics();
  _textSize = metrics.size( 0, _text );

  _childSize = _child->sizeHint();

  int height = _textSize.height() + bdSize + _childSize.height() + bdSize + 2*pw;
  int width  = 2 * pw + QMAX(_childSize.width(), 4*bdSize + _textSize.width());
  return QSize(width,height);
}

void LookAheadWidget::paintEvent( QPaintEvent *e )
{
  // TODO: Merge with RepeatWidget::paintEvent
  QSize mySize = sizeHint();
  QPainter painter(this);
  
  drawPossibleSelection( painter, mySize );
  
  // move the child to its position and resize it.
  _child->move(pw,_textSize.height()+bdSize);
  _child->resize(mySize.width() - 2*pw, _childSize.height());
  
  // Draw the border and the text.
  int startY = _textSize.height()/2;

  // Top lines and text
  painter.drawLine(pw,startY,bdSize,startY);
  painter.drawText(pw+2*bdSize,0,_textSize.width(), _textSize.height(),0, _text);
  int offset = pw + 3*bdSize + _textSize.width();
  painter.drawLine(offset,startY,mySize.width()-pw,startY);

  // horizontal lines
  painter.drawLine(0,startY,0,mySize.height()-pw);
  painter.drawLine(mySize.width()-pw,startY,mySize.width()-pw, mySize.height()-pw);

  // buttom line
  painter.drawLine(0, mySize.height()-pw, mySize.width()-pw, mySize.height()-pw);

  SingleContainerWidget::paintEvent(e);
}
