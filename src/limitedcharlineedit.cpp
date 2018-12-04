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

#include "limitedcharlineedit.h"

#include <QValidator>
#include <QKeyEvent>
#include <QRegularExpression>

/**
   @internal
   A Validator for the @ref LimitedCharLineEdit
*/
class Validator : public QValidator
{
public:
    Validator(LimitedCharLineEdit::Mode mode, QWidget *parent)
        : QValidator(parent)
        , _mode(mode)
    {
        setObjectName(QStringLiteral("Validator"));
    }

    QValidator::State validate(QString &txt, int & /*pos*/) const override
    {
        if (_mode == LimitedCharLineEdit::NORMAL
            || (_mode == LimitedCharLineEdit::HEX
                && txt.contains(QRegularExpression(QStringLiteral("^[0-9A-Fa-f]*$"))))
            || (_mode == LimitedCharLineEdit::OCT
                && txt.contains(QRegularExpression(QStringLiteral("^[0-7]*$"))))) {
            return QValidator::Acceptable;
        } else {
            return QValidator::Invalid;
        }
    }

private:
    LimitedCharLineEdit::Mode _mode;
};

void LimitedCharLineEdit::keyPressEvent(QKeyEvent *event)
{
    QLineEdit::keyPressEvent(event);
    if (text().length() == _count && !event->text().isNull()) {
        focusNextPrevChild(true);
    }
}

LimitedCharLineEdit::LimitedCharLineEdit(Mode mode, QWidget *parent, const QString &name)
    : QLineEdit(parent)
{
    setObjectName(name);

    if (mode == NORMAL) {
        _count = 1;
    } else if (mode == HEX) {
        _count = 4;
    } else {
        _count = 4;
    }

    setMaxLength(_count);
    setFixedSize(fontMetrics().boundingRect(QLatin1Char('A')).width() * 5 + 5, sizeHint().height());

    setValidator(new Validator(mode, this));
}
