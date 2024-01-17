/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "lookaheadwidget.h"

#include <QPainter>

#include <KLocalizedString>

#include "concwidget.h"
#include "lookaheadregexp.h"

LookAheadWidget::LookAheadWidget(RegExpEditorWindow *editorWindow, RegExpType tp, QWidget *parent)
    : SingleContainerWidget(editorWindow, parent)
    , _tp(tp)
{
    _child = new ConcWidget(editorWindow, this);
    init();
}

LookAheadWidget::LookAheadWidget(LookAheadRegExp *regexp, RegExpEditorWindow *editorWindow, RegExpType tp, QWidget *parent)
    : SingleContainerWidget(editorWindow, parent)
    , _tp(tp)
{
    RegExpWidget *child = WidgetFactory::createWidget(regexp->child(), editorWindow, this);
    if (!(_child = dynamic_cast<ConcWidget *>(child))) {
        _child = new ConcWidget(editorWindow, child, this);
    }

    init();
}

void LookAheadWidget::init()
{
    if (_tp == POSLOOKAHEAD) {
        _text = i18n("Pos. Look Ahead");
    } else {
        _text = i18n("Neg. Look Ahead");
    }
}

RegExp *LookAheadWidget::regExp() const
{
    return new LookAheadRegExp(isSelected(), ((_tp == POSLOOKAHEAD) ? LookAheadRegExp::POSITIVE : LookAheadRegExp::NEGATIVE), _child->regExp());
}

QSize LookAheadWidget::sizeHint() const
{
    // TODO: Merge with RepeatWidget::sizeHint
    QFontMetrics metrics = fontMetrics();
    _textSize = metrics.size(0, _text);

    _childSize = _child->sizeHint();

    int height = _textSize.height() + bdSize + _childSize.height() + bdSize + 2 * pw;
    int width = 2 * pw + qMax(_childSize.width(), 4 * bdSize + _textSize.width());
    return QSize(width, height);
}

void LookAheadWidget::paintEvent(QPaintEvent *e)
{
    // TODO: Merge with RepeatWidget::paintEvent
    QSize mySize = sizeHint();
    QPainter painter(this);

    drawPossibleSelection(painter, mySize);

    // move the child to its position and resize it.
    _child->move(pw, _textSize.height() + bdSize);
    _child->resize(mySize.width() - 2 * pw, _childSize.height());

    // Draw the border and the text.
    int startY = _textSize.height() / 2;

    // Top lines and text
    painter.drawLine(pw, startY, bdSize, startY);
    painter.drawText(pw + 2 * bdSize, 0, _textSize.width(), _textSize.height(), 0, _text);
    int offset = pw + 3 * bdSize + _textSize.width();
    painter.drawLine(offset, startY, mySize.width() - pw, startY);

    // horizontal lines
    painter.drawLine(0, startY, 0, mySize.height() - pw);
    painter.drawLine(mySize.width() - pw, startY, mySize.width() - pw, mySize.height() - pw);

    // buttom line
    painter.drawLine(0, mySize.height() - pw, mySize.width() - pw, mySize.height() - pw);

    SingleContainerWidget::paintEvent(e);
}

RegExpWidget *LookAheadWidget::findWidgetToEdit(QPointF globalPos)
{
    return _child->findWidgetToEdit(globalPos);
}

#include "moc_lookaheadwidget.cpp"
