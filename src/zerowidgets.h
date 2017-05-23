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

#ifndef zerowidget
#define zerowidget

#include "regexpwidget.h"

/**
   Represents any widget in the regular expression dialog, which do not have sub widget.
   @internal
*/
class ZeroWidget : public RegExpWidget
{
public:
    ZeroWidget(QString text, RegExpEditorWindow *editorWindow, QWidget *parent);
    virtual void addNewChild(DragAccepter *accepter, RegExpWidget *child);
    virtual QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *e);

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

    virtual RegExp *regExp() const;
    virtual RegExpType type() const
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
    virtual RegExp *regExp() const;
    virtual RegExpType type() const
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
    virtual RegExp *regExp() const;
    virtual RegExpType type() const
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
    virtual RegExp *regExp() const;
    virtual RegExpType type() const
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
    virtual RegExp *regExp() const;
    virtual RegExpType type() const
    {
        return NONWORDBOUNDARY;
    }
};

#endif // zerowidget
