/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "regexpwidget.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>

#include "concwidget.h"
#include "dragaccepter.h"

const int RegExpWidget::pw = 1;
const int RegExpWidget::bdSize = 5;
const int RegExpWidget::space = 5;

RegExpWidget::RegExpWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : QWidget(parent)
    , _editorWindow(editorWindow)
    , _isSelected(false)
    , _isToplevel(false)
{
    setAttribute(Qt::WA_NoMousePropagation);
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
    bool selected = (parentSelected || _editorWindow->selectionOverlap(mapToGlobal(QPoint(0, 0)), size())) && !_isToplevel;

    if (_isSelected != selected) {
        // Selection state changed
        _isSelected = selected;
        repaint();
        return true;
    }
    return false;
}

void RegExpWidget::drawPossibleSelection(QPainter &painter, QSize mySize)
{
    if (_isSelected) {
        painter.fillRect(0, 0, mySize.width(), mySize.height(), QBrush(Qt::gray));
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

void RegExpWidget::applyRegExpToSelection(RegExpType)
{
    qFatal("This method should not be called for this widget");
}

void RegExpWidget::deleteSelection()
{
    qFatal("This method should be overridden if needed!");
}

RegExp *RegExpWidget::selection() const
{
    return regExp();
}

int RegExpWidget::edit()
{
    qFatal("This method should be overridden if needed!");
    return 0; // Compiler shut up
}

void RegExpWidget::mousePressEvent(QMouseEvent *event)
{
    if (_editorWindow->isPasteing() || _editorWindow->isInserting()) {
        return;
    }

    if (event->button() == Qt::LeftButton) {
        if (!_editorWindow->pointSelected(QCursor::pos())) {
            _editorWindow->clearSelection(true);
            if (dynamic_cast<DragAccepter *>(this) == nullptr && dynamic_cast<ConcWidget *>(this) == nullptr) {
                selectWidget(true);
            }
        }

        QMouseEvent ev(event->type(), mapTo(_editorWindow, event->pos()), event->button(), event->buttons(), event->modifiers());
        QApplication::sendEvent(_editorWindow, &ev);
    } else if (event->button() == Qt::RightButton) {
        _editorWindow->showRMBMenu(true);
    }

    // currently (Qt3.0) it seems like qt do not accept that the accept flag is set,
    // and thus sends the event to the parent - given that the following line is in.
    // It doesn't make any change to leave it out.
    // 25 Oct. 2001 19:03 -- Jesper K. Pedersen
    //  QWidget::mousePressEvent( event );
}

void RegExpWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (_editorWindow->isInserting() && acceptWidgetInsert(_editorWindow->insertType())) {
        if (!_editorWindow->hasSelection()) {
            _isSelected = true;
        }

        _editorWindow->applyRegExpToSelection(_editorWindow->insertType());
        _editorWindow->clearSelection(true);
        _editorWindow->updateContent(this);
        _editorWindow->slotEndActions();
        _editorWindow->updateCursorUnderPoint();
    }
}

QRect RegExpWidget::selectionRect() const
{
    return QRect(mapToGlobal(QPoint(0, 0)), size());
}

void RegExpWidget::enterEvent(QEnterEvent *)
{
    updateCursorShape();
}

void RegExpWidget::updateCursorShape()
{
    QCursor cursor;

    if (_editorWindow->isPasteing()) {
        if (acceptWidgetPaste()) {
            cursor = Qt::CrossCursor;
        } else {
            cursor = Qt::ForbiddenCursor;
        }
    } else if (_editorWindow->isInserting()) {
        if (acceptWidgetInsert(_editorWindow->insertType())) {
            cursor = Qt::CrossCursor;
        } else {
            cursor = Qt::ForbiddenCursor;
        }
    } else {
        cursor = Qt::ArrowCursor;
    }

    setCursor(cursor);
}

void RegExpWidget::updateCursorRecursively()
{
    updateCursorShape();
}

bool RegExpWidget::acceptWidgetPaste() const
{
    return false;
}

bool RegExpWidget::acceptWidgetInsert(RegExpType tp) const
{
    return WidgetFactory::isContainer(tp);
}

RegExpWidget *RegExpWidget::widgetUnderPoint(QPointF globalPos, bool)
{
    if (QRectF(mapToGlobal(QPointF(0, 0)), size()).contains(globalPos)) {
        return this;
    } else {
        return nullptr;
    }
}

void RegExpWidget::selectWidget(bool sel)
{
    _isSelected = sel;
    update();
}

void RegExpWidget::updateAll()
{
    update();
}

#include "moc_regexpwidget.cpp"
