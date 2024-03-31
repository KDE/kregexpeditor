/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include <QLineEdit>

/**
   QLineEdit which only accepts a prespecified number of character.
   @internal
*/
class LimitedCharLineEdit : public QLineEdit
{
public:
    enum Mode { NORMAL = 0, HEX = 1, OCT = 2 };

    LimitedCharLineEdit(Mode mode, QWidget *parent, const QString &name = QString());

protected:
    void keyPressEvent(QKeyEvent *) override;

private:
    int _count;
};
