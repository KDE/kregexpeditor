/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef __multicontainerwidget
#define __multicontainerwidget

#include "regexpwidget.h"

#include <QList>

/**
   Abstract RegExp widget class representing widgets having multiple children.
   @internal
*/
class MultiContainerWidget : public RegExpWidget
{
public:
    explicit MultiContainerWidget(RegExpEditorWindow *editorWindow, QWidget *parent = nullptr);
    bool hasSelection() const override;
    void clearSelection() override;
    void deleteSelection() override;
    void addNewChild(DragAccepter *accepter, RegExpWidget *child) override;
    bool updateSelection(bool parentSelected) override;
    QRect selectionRect() const override;
    RegExpWidget *widgetUnderPoint(QPointF globalPos, bool justVisibleWidgets) override;
    RegExpWidget *findWidgetToEdit(QPointF globalPos) override;
    void selectWidget(bool sel) override;
    void updateAll() override;
    void updateCursorRecursively() override;

protected:
    void append(RegExpWidget *child);
    QList<RegExpWidget *> _children;
};

#endif // __multicontainerwidget
