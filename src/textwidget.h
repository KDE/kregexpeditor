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

#ifndef __textwidget
#define __textwidget

#include "regexpwidget.h"

class SelectableLineEdit;
class TextRegExp;

/**
   RegExp widget representing text.

   @internal
*/
class TextWidget : public RegExpWidget
{
    Q_OBJECT

public:
    TextWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    TextWidget(TextRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent);
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return TEXT;
    }

    void updateAll() override;
    void selectWidget(bool) override;

protected:
    void init(const QString &text);
    void paintEvent(QPaintEvent *e) override;
    bool updateSelection(bool parentSelected) override;
    void clearSelection() override;
    bool eventFilter(QObject *, QEvent *) override;

protected slots:
    void slotUpdate();

private:
    QString text;
    SelectableLineEdit *_edit;
    QSize textSize, boxSize, editSize;
};

#endif // __textwidget
