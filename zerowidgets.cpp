#include "zerowidgets.h"
#include "dotregexp.h"
#include "positionregexp.h"
#include <klocale.h>
#include <qpainter.h>
#include <qbrush.h>

//--------------------------------------------------------------------------------
//                                ZeroWidget
//--------------------------------------------------------------------------------
ZeroWidget::ZeroWidget(QString txt, RegExpEditorWindow* editorWindow, 
                       QWidget *parent, const char *name)
  : RegExpWidget(editorWindow, parent, name ? name : "ZeroWidget" )
{
  _text = txt;
}

void ZeroWidget::addNewChild(DragAccepter *, RegExpWidget *)
{
  qFatal("No children should be added to this widget!");
}

QSize ZeroWidget::sizeHint() const
{
  QFontMetrics metrics = fontMetrics();
  _textSize = metrics.size(0,_text);
  _boxSize = QSize(_textSize.width() + 2*space, _textSize.height() + 2 *space);
  return _boxSize;
}

void ZeroWidget::paintEvent( QPaintEvent *e)
{
  // So what is my Size?
  QSize mySize = sizeHint();

  QPainter painter(this);
  drawPossibleSelection( painter, mySize);
  
  // Write the text and the rectangle
  painter.drawText(space, space, _textSize.width(), _textSize.height(), 0, _text);
  painter.drawRoundRect(0, 0, _boxSize.width(), _boxSize.height());
  
  RegExpWidget::paintEvent(e);
}


//--------------------------------------------------------------------------------
//                                AnyCharWidget
//--------------------------------------------------------------------------------
AnyCharWidget::AnyCharWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                             const char *name)
  : ZeroWidget(i18n("Any\nCharacter"), editorWindow, parent, 
               name ? name : "AnyCharWidget")
{
}

RegExp* AnyCharWidget::regExp() const 
{
	return new DotRegExp();
}


//--------------------------------------------------------------------------------
//                                BegLineWidget
//--------------------------------------------------------------------------------
BegLineWidget::BegLineWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                             const char *name)
  : ZeroWidget(i18n("Beg.\nof\nline"), editorWindow, parent, 
               name ? name : "BegLineWidget")
{
}

RegExp* BegLineWidget::regExp() const
{
	return new PositionRegExp( PositionRegExp::BEGLINE );
	
}

//--------------------------------------------------------------------------------
//                                EndLineWidget
//--------------------------------------------------------------------------------
EndLineWidget::EndLineWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                             const char *name)
  : ZeroWidget(i18n("End\nof\nline"), editorWindow, parent, name)
{
}

RegExp* EndLineWidget::regExp() const
{
	return new PositionRegExp( PositionRegExp::ENDLINE );
}

//--------------------------------------------------------------------------------
//                                WordBoundaryWidget
//--------------------------------------------------------------------------------
WordBoundaryWidget::WordBoundaryWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                                       const char *name)
  : ZeroWidget(i18n("Word\nBoundary"), editorWindow, parent, 
               name ? name : "WordBoundaryWidget" )
{
}

RegExp* WordBoundaryWidget::regExp() const 
{
	return new PositionRegExp( PositionRegExp::WORDBOUNDARY );
}

//--------------------------------------------------------------------------------
//                                NonWordBoundaryWidget
//--------------------------------------------------------------------------------
NonWordBoundaryWidget::NonWordBoundaryWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                                             const char *name)
  : ZeroWidget(i18n("Non-word\nBoundary"), editorWindow, parent, 
               name ? name : "NonWordBoundaryWidget" )
{
}

RegExp* NonWordBoundaryWidget::regExp() const
{
	return new PositionRegExp( PositionRegExp::NONWORDBOUNDARY );
}

