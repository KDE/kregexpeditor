/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "limitedcharlineedit.h"

#include <QValidator>
#include <QKeyEvent>
#include <qregularexpression.h>

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
                && txt.indexOf(QRegularExpression(QStringLiteral("^[0-9A-Fa-f]*$"))) != -1)
            || (_mode == LimitedCharLineEdit::OCT
                && txt.indexOf(QRegularExpression(QStringLiteral("^[0-7]*$"))) != -1)) {
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
