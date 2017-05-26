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

#ifndef altnwidget
#define altnwidget

#include "multicontainerwidget.h"

class QPaintEvent;
class AltnRegExp;

/**
   RegExp widget for alternatives.

   @internal
*/
class AltnWidget : public MultiContainerWidget
{
public:
    AltnWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    AltnWidget(AltnRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent);
    void addNewChild(DragAccepter *accepter, RegExpWidget *child) override;
    QSize sizeHint() const override;
    RegExp *regExp() const override;
    void applyRegExpToSelection(RegExpType type) override;
    RegExpType type() const override
    {
        return ALTN;
    }

    RegExp *selection() const override;
    bool validateSelection() const override;
    void setConcChild(ConcWidget *child) override;
    void addNewConcChild(DragAccepter *accepter, ConcWidget *child) override;
    void deleteSelection() override;

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    /**
       This functions runs through all the dragaccepters and calls @ref
       DragAccepter::setDrawLine.
    */
    void updateDrawLineInfo();

    QString _text;
    mutable QSize _textSize;

    mutable int _childrenWidth;
    mutable int _childrenHeight;
};

#endif // altnwidget
