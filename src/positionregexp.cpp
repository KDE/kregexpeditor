/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include "positionregexp.h"

#include "errormap.h"

PositionRegExp::PositionRegExp(bool selected, PositionType tp)
    : RegExp(selected)
{
    _tp = tp;
}

bool PositionRegExp::check(ErrorMap &map, bool first, bool last)
{
    if (_tp == BEGLINE && !first) {
        map.lineStartError();
    } else if (_tp == ENDLINE && !last) {
        map.lineEndError();
    }
    return true;
}

QDomNode PositionRegExp::toXml(QDomDocument *doc) const
{
    switch (_tp) {
    case BEGLINE:
        return doc->createElement(QStringLiteral("BegLine"));
    case ENDLINE:
        return doc->createElement(QStringLiteral("EndLine"));
    case WORDBOUNDARY:
        return doc->createElement(QStringLiteral("WordBoundary"));
    case NONWORDBOUNDARY:
        return doc->createElement(QStringLiteral("NonWordBoundary"));
    }
    return QDomNode();
}

bool PositionRegExp::load(const QDomElement & /* top */, const QString & /*version*/)
{
    // Nothing to do.
    return true;
}
