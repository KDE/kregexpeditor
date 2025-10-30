/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include "zerowidgets.h"

#include <QPainter>

#include <KLocalizedString>

#include "dotregexp.h"
#include "myfontmetrics.h"
#include "positionregexp.h"

//--------------------------------------------------------------------------------
//                                ZeroWidget
//--------------------------------------------------------------------------------
ZeroWidget::ZeroWidget(const QString &txt, RegExpEditorWindow *editorWindow, QWidget *parent)
    : RegExpWidget(editorWindow, parent)
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
    _textSize = HackCalculateFontSize(metrics, _text);
    //  _textSize = metrics.size(0,_text);
    _boxSize = QSize(_textSize.width() + 2 * space, _textSize.height() + 2 * space);
    return _boxSize;
}

void ZeroWidget::paintEvent(QPaintEvent *e)
{
    // So what is my Size?
    QSize mySize = sizeHint();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawPossibleSelection(painter, mySize);

    // Write the text and the rectangle
    painter.drawText(space, space, _textSize.width(), _textSize.height(), 0, _text);
    painter.drawRoundedRect(0, 0, _boxSize.width() - 1, _boxSize.height() - 1, 5, 5);

    RegExpWidget::paintEvent(e);
}

//--------------------------------------------------------------------------------
//                                AnyCharWidget
//--------------------------------------------------------------------------------
AnyCharWidget::AnyCharWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : ZeroWidget(i18n("Any\nCharacter"), editorWindow, parent)
{
}

RegExp *AnyCharWidget::regExp() const
{
    return new DotRegExp(isSelected());
}

//--------------------------------------------------------------------------------
//                                BegLineWidget
//--------------------------------------------------------------------------------
BegLineWidget::BegLineWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : ZeroWidget(i18n("Line\nStart"), editorWindow, parent)
{
}

RegExp *BegLineWidget::regExp() const
{
    return new PositionRegExp(isSelected(), PositionRegExp::BEGLINE);
}

//--------------------------------------------------------------------------------
//                                EndLineWidget
//--------------------------------------------------------------------------------
EndLineWidget::EndLineWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : ZeroWidget(i18n("Line\nEnd"), editorWindow, parent)
{
}

RegExp *EndLineWidget::regExp() const
{
    return new PositionRegExp(isSelected(), PositionRegExp::ENDLINE);
}

//--------------------------------------------------------------------------------
//                                WordBoundaryWidget
//--------------------------------------------------------------------------------
WordBoundaryWidget::WordBoundaryWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : ZeroWidget(i18n("Word\nBoundary"), editorWindow, parent)
{
}

RegExp *WordBoundaryWidget::regExp() const
{
    return new PositionRegExp(isSelected(), PositionRegExp::WORDBOUNDARY);
}

//--------------------------------------------------------------------------------
//                                NonWordBoundaryWidget
//--------------------------------------------------------------------------------
NonWordBoundaryWidget::NonWordBoundaryWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : ZeroWidget(i18n("Non-word\nBoundary"), editorWindow, parent)
{
}

RegExp *NonWordBoundaryWidget::regExp() const
{
    return new PositionRegExp(isSelected(), PositionRegExp::NONWORDBOUNDARY);
}
