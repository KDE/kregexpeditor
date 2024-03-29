/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "singlecontainerwidget.h"

#include "concwidget.h"

SingleContainerWidget::SingleContainerWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : RegExpWidget(editorWindow, parent)
{
}

bool SingleContainerWidget::updateSelection(bool parentSelected)
{
    bool changed = RegExpWidget::updateSelection(parentSelected);
    changed = _child->updateSelection(_isSelected) && changed;
    if (changed) {
        repaint();
    }

    return changed;
}

bool SingleContainerWidget::hasSelection() const
{
    return _isSelected || _child->hasSelection();
}

void SingleContainerWidget::clearSelection()
{
    _isSelected = false;
    _child->clearSelection();
}

void SingleContainerWidget::deleteSelection()
{
    _child->deleteSelection();
    update();
}

void SingleContainerWidget::applyRegExpToSelection(RegExpType type)
{
    _child->applyRegExpToSelection(type);
}

RegExp *SingleContainerWidget::selection() const
{
    if (isSelected()) {
        return regExp();
    } else {
        return _child->selection();
    }
}

bool SingleContainerWidget::validateSelection() const
{
    return _child->validateSelection();
}

QRect SingleContainerWidget::selectionRect() const
{
    if (_isSelected) {
        return QRect(mapToGlobal(QPoint(0, 0)), size());
    } else {
        return _child->selectionRect();
    }
}

RegExpWidget *SingleContainerWidget::widgetUnderPoint(QPointF globalPos, bool justVisibleWidgets)
{
    RegExpWidget *wid = _child->widgetUnderPoint(globalPos, justVisibleWidgets);
    if (wid) {
        return wid;
    } else {
        // The child didn't contain the point, then lets see if this widget
        // itself does.
        return RegExpWidget::widgetUnderPoint(globalPos, justVisibleWidgets);
    }
}

RegExpWidget *SingleContainerWidget::findWidgetToEdit(QPointF globalPos)
{
    RegExpWidget *wid = _child->findWidgetToEdit(globalPos);
    if (wid) {
        return wid;
    } else if (QRectF(mapToGlobal(QPointF(0, 0)), size()).contains(globalPos)) {
        return this;
    } else {
        return nullptr;
    }
}

void SingleContainerWidget::setConcChild(ConcWidget *conc)
{
    delete _child;
    _child = conc;
}

void SingleContainerWidget::selectWidget(bool sel)
{
    RegExpWidget::selectWidget(sel);
    _child->selectWidget(sel);
    update();
}

void SingleContainerWidget::updateAll()
{
    _child->updateAll();
    RegExpWidget::updateAll();
}

void SingleContainerWidget::updateCursorRecursively()
{
    _child->updateCursorRecursively();
    updateCursorShape();
}

#include "moc_singlecontainerwidget.cpp"
