/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "textwidget.h"

#include <QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>

#include "textregexp.h"
#include "selectablelineedit.h"

TextWidget::TextWidget(RegExpEditorWindow *editorWindow, QWidget *parent)
    : RegExpWidget(editorWindow, parent)
{
    init(QString());
}

TextWidget::TextWidget(TextRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent)
    : RegExpWidget(editorWindow, parent)
{
    init(regexp->text());
}

void TextWidget::init(const QString &txt)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    _edit = new SelectableLineEdit(this, this, QStringLiteral("TextWidget::edit"));
    _edit->setDragEnabled(false);   //otherwise QLineEdit::mouseMoveEvent will set the cursor over and over again.
    lay->addWidget(_edit);

    _edit->setText(txt);

    connect(_edit, &SelectableLineEdit::parentPleaseUpdate, this, &TextWidget::slotUpdate);
    setFocusProxy(_edit);
    _edit->installEventFilter(this);
    connect(_edit, &QLineEdit::textChanged, _editorWindow, &RegExpEditorWindow::emitChange);
}

void TextWidget::slotUpdate()
{
    // I need to force the parent to repaint, as the size change of this
    // widget may not be enough for the parent to change size, and in that
    // case the parent would not repaint, and the text widget would not be
    // resized.
    QWidget *p = static_cast<QWidget *>(parent());
    if (p) {
        p->repaint();
    }
    _editorWindow->updateContent(this);
}

void TextWidget::paintEvent(QPaintEvent *e)
{
    RegExpWidget::paintEvent(e);
}

void TextWidget::selectWidget(bool sel)
{
    _edit->setSelected(sel);
}

bool TextWidget::updateSelection(bool parentSelected)
{
    bool changed = RegExpWidget::updateSelection(parentSelected);

    // I need to call this function all the time, else the rubber band will
    // not be correctly deleted in the line edit.
    _edit->setSelected(_isSelected);
    return changed;
}

void TextWidget::updateAll()
{
    _edit->update();
    update();
}

void TextWidget::clearSelection()
{
    _isSelected = false;
    _edit->setSelected(false);
}

RegExp *TextWidget::regExp() const
{
    return new TextRegExp(isSelected(), _edit->text());
}

bool TextWidget::eventFilter(QObject *, QEvent *event)
{
    // This is an event filter (in contrast to methods in SelectableLineEdit),
    // otherwise lots of functions would need to be exported from TextWidget.
    if (event->type() == QEvent::MouseButtonRelease) {
        if (_editorWindow->isInserting()) {
            if (acceptWidgetInsert(_editorWindow->insertType())) {
                mouseReleaseEvent(static_cast<QMouseEvent *>(event));
            }
            return true;
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        if (_editorWindow->isInserting()) {
            return true;
        } else if (isSelected()) {
            QMouseEvent *e = static_cast<QMouseEvent *>(event);
            QMouseEvent ev(event->type(), mapTo(_editorWindow, e->pos()),
                           e->button(), e->buttons(), e->modifiers());
            QApplication::sendEvent(_editorWindow, &ev);
            return true;
        }
    } else if (event->type() == QEvent::Enter) {
        if (_editorWindow->isInserting()) {
            if (acceptWidgetInsert(_editorWindow->insertType())) {
                _edit->setCursor(Qt::CrossCursor);
            } else {
                _edit->setCursor(Qt::ForbiddenCursor);
            }
        } else if (isSelected()) {
            _edit->setCursor(Qt::ArrowCursor);
        } else {
            _edit->setCursor(Qt::IBeamCursor);
        }
    } else if (event->type() == QEvent::MouseButtonDblClick && _editorWindow->isInserting()) {
        return true;
    }
    return false;
}
