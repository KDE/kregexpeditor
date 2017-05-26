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

#ifndef __concwidget
#define __concwidget

#include "multicontainerwidget.h"

class ConcRegExp;

/**
   RegExp widget which can have several sub widget inside it.
   This widget is invisible to the user, but act as a container around
   other RegExp widgets
   @internal
*/
class ConcWidget : public MultiContainerWidget
{
public:
    ConcWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    ConcWidget(RegExpEditorWindow *editorWindow, RegExpWidget *child, QWidget *parent);
    ConcWidget(RegExpEditorWindow *editorWindow, ConcWidget *origConc, unsigned int start, unsigned int end);
    ConcWidget(ConcRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent);
    void init();

    QSize sizeHint() const override;
    RegExp *regExp() const override;
    bool updateSelection(bool parentSelected) override;
    bool isSelected() const override;

    void applyRegExpToSelection(RegExpType type) override;
    RegExpType type() const override
    {
        return CONC;
    }

    RegExp *selection() const override;
    void addNewConcChild(DragAccepter *accepter, ConcWidget *child) override;
    bool validateSelection() const override;
    bool acceptWidgetInsert(RegExpType) const override
    {
        return false;
    }

    bool acceptWidgetPaste() const override
    {
        return false;
    }

    bool hasAnyChildren()
    {
        return _children.count() > 1;
    }

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void sizeAccepter(DragAccepter *accepter, int height, int totHeight);
    void getSelectionIndexes(int *start, int *end);
    //virtual void dragEnterEvent(QDragEnterEvent* event) { event->setAccepted( false ); }

private:
    int _maxSelectedHeight;
};

#endif // __concwidget
