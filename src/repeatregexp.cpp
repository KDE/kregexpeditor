/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "repeatregexp.h"

#include <KMessageBox>
#include <KLocalizedString>

RepeatRegExp::RepeatRegExp(bool selected, int lower, int upper, RegExp *child) : RegExp(selected)
{
    _lower = lower;
    _upper = upper;
    _child = child;
    if (child) {
        addChild(child);
    }
}

bool RepeatRegExp::check(ErrorMap &map, bool first, bool last)
{
    _child->check(map, first, last);
    return _lower == 0;
}

QDomNode RepeatRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top = doc->createElement(QStringLiteral("Repeat"));
    top.setAttribute(QStringLiteral("lower"), _lower);
    top.setAttribute(QStringLiteral("upper"), _upper);
    top.appendChild(_child->toXml(doc));
    return top;
}

bool RepeatRegExp::load(const QDomElement &top, const QString &version)
{
    Q_ASSERT(top.tagName() == QStringLiteral("Repeat"));
    QString lower = top.attribute(QStringLiteral("lower"), QStringLiteral("0"));
    QString upper = top.attribute(QStringLiteral("upper"), QStringLiteral("0"));
    bool ok;
    _lower = lower.toInt(&ok);
    if (!ok) {
        KMessageBox::error(nullptr, i18n("<p>Value for attribute <b>%1</b> was not an integer for element "
                                         "<b>%2</b></p><p>It contained the value <b>%3</b></p>",
                                         QStringLiteral("lower"), QStringLiteral("Repeat"), lower),
                           i18n("Error While Loading From XML File"));
        _lower = 0;
    }
    _upper = upper.toInt(&ok);
    if (!ok) {
        KMessageBox::error(nullptr, i18n("<p>Value for attribute <b>%1</b> was not an integer for element "
                                         "<b>%2</b></p><p>It contained the value <b>%3</b></p>",
                                         QStringLiteral("upper"), QStringLiteral("Repeat"), upper),
                           i18n("Error While Loading From XML File"));
        _upper = -1;
    }

    _child = readRegExp(top, version);
    if (_child) {
        addChild(_child);
        return true;
    } else {
        return false;
    }
}

bool RepeatRegExp::operator==(const RegExp &other) const
{
    if (type() != other.type()) {
        return false;
    }

    const RepeatRegExp &theOther = dynamic_cast<const RepeatRegExp &>(other);
    if (_lower != theOther._lower || _upper != theOther._upper) {
        return false;
    }

    return *_child == *(theOther._child);
}
