/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

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

protected Q_SLOTS:
    void slotUpdate();

private:
    QString text;
    SelectableLineEdit *_edit = nullptr;
    QSize textSize, boxSize, editSize;
};
