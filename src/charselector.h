/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include <QWidget>

class QComboBox;
class QStackedWidget;
class LimitedCharLineEdit;

class CharSelector : public QWidget
{
    Q_OBJECT

public:
    explicit CharSelector(QWidget *parent);
    QString text() const;
    void setText(const QString &text);
    bool isEmpty() const;

protected:
    void fillComboBox();

private:
    void slotNewItem(int which);
    QComboBox *_type = nullptr;
    QStackedWidget *_stack = nullptr;
    LimitedCharLineEdit *_normal = nullptr;
    LimitedCharLineEdit *_hex = nullptr;
    LimitedCharLineEdit *_oct = nullptr;
    int _oldIndex = 0;
};
