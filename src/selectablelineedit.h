/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/
#pragma once

#include <QLineEdit>

class RegExpWidget;

/**
   A specialized QLineEdit for use in @ref TextWidget.

   This widget has two main features:
   @li when the @ref TextWidget which contains it is selected, then the
        line edit is grayed like the rest of the widgets.
   @li When the widget does not have the focus, it is resized to only be
        large enough to contain the text written in it.

   @internal
*/
class SelectableLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit SelectableLineEdit(RegExpWidget *owner, QWidget *parent = nullptr, const QString &name = QString());
    void setSelected(bool selected);
    QSize sizeHint() const override;

protected Q_SLOTS:
    void slotKeyPressed();

Q_SIGNALS:
    void parentPleaseUpdate();

private:
    RegExpWidget *_owner = nullptr;
};
