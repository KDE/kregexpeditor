/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include "scrollededitorwindow.h"

#include <QDebug>
#include <QResizeEvent>
#include <QScrollArea>

#include "regexpeditorwindow.h"

RegExpScrolledEditorWindow::RegExpScrolledEditorWindow(QWidget *parent)
    : QWidget(parent)
{
    _scrollArea = new QScrollArea(this);
    _editorWindow = new RegExpEditorWindow(this);
    _scrollArea->setWidget(_editorWindow);
    _scrollArea->setWidgetResizable(true); // Morten Sjøgren: This is for some reason required for pasting to work.
    _scrollArea->ensureWidgetVisible(_editorWindow);

    connect(_editorWindow, &RegExpEditorWindow::contentChanged, this, &RegExpScrolledEditorWindow::slotUpdateContentSize);

    connect(_editorWindow, &RegExpEditorWindow::scrolling, this, &RegExpScrolledEditorWindow::slotScroll);

    connect(_editorWindow, &RegExpEditorWindow::doneEditing, this, &RegExpScrolledEditorWindow::doneEditing);

    connect(_editorWindow, &RegExpEditorWindow::change, this, &RegExpScrolledEditorWindow::change);
    connect(_editorWindow, &RegExpEditorWindow::savedRegexp, this, &RegExpScrolledEditorWindow::savedRegexp);

    connect(_editorWindow, &RegExpEditorWindow::anythingSelected, this, &RegExpScrolledEditorWindow::anythingSelected);
    connect(_editorWindow, &RegExpEditorWindow::anythingOnClipboard, this, &RegExpScrolledEditorWindow::anythingOnClipboard);
    connect(_editorWindow, &RegExpEditorWindow::canSave, this, &RegExpScrolledEditorWindow::canSave);
    connect(_editorWindow, &RegExpEditorWindow::verifyRegExp, this, &RegExpScrolledEditorWindow::verifyRegExp);
}

void RegExpScrolledEditorWindow::slotSetRegExp(RegExp *regexp)
{
    _editorWindow->slotSetRegExp(regexp);
    slotUpdateContentSize(QPoint());
}

void RegExpScrolledEditorWindow::slotInsertRegExp(int tp)
{
    _editorWindow->slotInsertRegExp((RegExpType)tp);
}

void RegExpScrolledEditorWindow::slotInsertRegExp(RegExp *regexp)
{
    _editorWindow->slotInsertRegExp(regexp);
}

void RegExpScrolledEditorWindow::slotDeleteSelection()
{
    _editorWindow->slotDeleteSelection();
}

void RegExpScrolledEditorWindow::slotDoSelect()
{
    _editorWindow->slotDoSelect();
}

void RegExpScrolledEditorWindow::slotCut()
{
    _editorWindow->slotCut();
}

void RegExpScrolledEditorWindow::slotCopy()
{
    _editorWindow->slotCopy();
}

void RegExpScrolledEditorWindow::slotPaste()
{
    _editorWindow->slotStartPasteAction();
}

void RegExpScrolledEditorWindow::slotSave()
{
    _editorWindow->slotSave();
}

RegExp *RegExpScrolledEditorWindow::regExp()
{
    return _editorWindow->regExp();
}

void RegExpScrolledEditorWindow::resizeEvent(QResizeEvent *event)
{
    _scrollArea->resize(event->size());
    slotUpdateContentSize(QPointF());
}

void RegExpScrolledEditorWindow::slotUpdateContentSize(QPointF focusPoint)
{
    _editorWindow->resize(_editorWindow->sizeHint());

    if (!focusPoint.isNull()) {
        _scrollArea->ensureVisible(focusPoint.x(), focusPoint.y(), 250, 250);
    }
}

// TODO: add timers, which will make the widget scroll when mouse is located
// outside the QScrollView.
void RegExpScrolledEditorWindow::slotScroll(QPointF focusPoint)
{
    _scrollArea->ensureVisible(focusPoint.x(), focusPoint.y());
}

#include "moc_scrollededitorwindow.cpp"
