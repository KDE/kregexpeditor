/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

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
