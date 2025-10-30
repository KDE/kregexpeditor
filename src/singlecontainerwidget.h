/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

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
