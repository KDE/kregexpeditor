/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include "lookaheadregexp.h"
#include "errormap.h"

LookAheadRegExp::LookAheadRegExp(bool selected, TYPE tp, RegExp *child)
    : RegExp(selected)
    , _child(child)
    , _tp(tp)
{
    if (child) {
        addChild(child);
    }
}

bool LookAheadRegExp::check(ErrorMap &map, bool, bool last)
{
    if (!last) {
        map.lookAheadError();
    }
    return true;
}

QDomNode LookAheadRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top;
    if (_tp == POSITIVE) {
        top = doc->createElement(QStringLiteral("PositiveLookAhead"));
    } else {
        top = doc->createElement(QStringLiteral("NegativeLookAhead"));
    }

    top.appendChild(_child->toXml(doc));
    return top;
}

bool LookAheadRegExp::load(const QDomElement &top, const QString &version)
{
    _child = readRegExp(top, version);
    if (_child) {
        addChild(_child);
        return true;
    } else {
        return false;
    }
}

bool LookAheadRegExp::operator==(const RegExp &other) const
{
    if (type() != other.type()) {
        return false;
    }

    const LookAheadRegExp &theOther = dynamic_cast<const LookAheadRegExp &>(other);

    if (lookAheadType() != theOther.lookAheadType()) {
        return false;
    }

    return *_child == *(theOther._child);
}
