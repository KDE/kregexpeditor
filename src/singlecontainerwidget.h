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

#ifndef __singlecontainer
#define __singlecontainer

#include "regexpwidget.h"

/**
   Abstract class representing RegExp widgets having a single child.
*/
class SingleContainerWidget : public RegExpWidget
{
    Q_OBJECT

public:
    explicit SingleContainerWidget(RegExpEditorWindow *editorWindow, QWidget *parent);

    bool updateSelection(bool parentSelected) override;
    bool hasSelection() const override;
    void clearSelection() override;
    void deleteSelection() override;
    void applyRegExpToSelection(RegExpType type) override;
    RegExp *selection() const override;
    bool validateSelection() const override;
    QRect selectionRect() const override;
    RegExpWidget *widgetUnderPoint(QPointF globalPos, bool justVisibleWidgets) override;
    RegExpWidget *findWidgetToEdit(QPointF globalPos) override;
    void setConcChild(ConcWidget *child) override;
    void selectWidget(bool sel) override;
    void updateAll() override;
    void updateCursorRecursively() override;

protected:
    ConcWidget *_child = nullptr;
};

#endif // __singlecontainer
