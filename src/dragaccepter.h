/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include "regexpwidget.h"

class MultiContainerWidget;

/**
   RegExp widget representing a "spot" in which new RegExp widgets can be inserted.
   @internal
*/
class DragAccepter : public RegExpWidget
{
    friend class MultiContainerWidget;

public:
    DragAccepter(RegExpEditorWindow *editorWindow, RegExpWidget *parent);
    QSize sizeHint() const override;
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return DRAGACCEPTER;
    }

    void setDrawLine(bool drawLine)
    {
        _drawLine = drawLine;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    bool acceptWidgetInsert(RegExpType) const override
    {
        return true;
    }

    bool acceptWidgetPaste() const override
    {
        return true;
    }

private:
    bool _drawLine;
};
