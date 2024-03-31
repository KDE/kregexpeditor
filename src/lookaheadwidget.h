/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include "singlecontainerwidget.h"

class LookAheadRegExp;

class LookAheadWidget : public SingleContainerWidget
{
    Q_OBJECT
public:
    LookAheadWidget(RegExpEditorWindow *editorWindow, RegExpType tp, QWidget *parent);
    LookAheadWidget(LookAheadRegExp *regexp, RegExpEditorWindow *editorWindow, RegExpType tp, QWidget *parent);

    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return _tp;
    }

    QSize sizeHint() const override;
    RegExpWidget *findWidgetToEdit(QPointF globalPos) override;

protected:
    void init();
    void paintEvent(QPaintEvent *e) override;

private:
    RegExpType _tp;
    QString _text;

    mutable QSize _textSize;
    mutable QSize _childSize;
};
