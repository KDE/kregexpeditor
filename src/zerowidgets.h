/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include "regexpwidget.h"

/**
   Represents any widget in the regular expression dialog, which do not have sub widget.
   @internal
*/
class ZeroWidget : public RegExpWidget
{
public:
    ZeroWidget(const QString &text, RegExpEditorWindow *editorWindow, QWidget *parent);
    void addNewChild(DragAccepter *accepter, RegExpWidget *child) override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QString _text;

    mutable QSize _textSize;
    mutable QSize _boxSize;
};

/**
   RegExp widget for `any single character'
   @internal
*/
class AnyCharWidget : public ZeroWidget
{
public:
    AnyCharWidget(RegExpEditorWindow *editorWindow, QWidget *parent);

    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return DOT;
    }
};

/**
   RegExp widget for `beginning of line'
   @internal
*/
class BegLineWidget : public ZeroWidget
{
public:
    BegLineWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return BEGLINE;
    }
};

/**
   RegExp widget for `end of line'
   @internal
*/
class EndLineWidget : public ZeroWidget
{
public:
    EndLineWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return ENDLINE;
    }
};

/**
   RegExp widget for `word boundary'
   @internal
*/
class WordBoundaryWidget : public ZeroWidget
{
public:
    WordBoundaryWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return WORDBOUNDARY;
    }
};

/**
   RegExp widget for `end of word'
   @internal
*/
class NonWordBoundaryWidget : public ZeroWidget
{
public:
    NonWordBoundaryWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return NONWORDBOUNDARY;
    }
};
