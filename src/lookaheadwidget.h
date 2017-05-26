/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#ifndef __lookaheadwidget
#define __lookaheadwidget

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
    RegExpWidget *findWidgetToEdit(QPoint globalPos) override;

protected:
    void init();
    void paintEvent(QPaintEvent *e) override;

private:
    RegExpType _tp;
    QString _text;

    mutable QSize _textSize;
    mutable QSize _childSize;
};

#endif // __lookaheadwidget
