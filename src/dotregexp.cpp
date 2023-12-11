/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "dotregexp.h"

DotRegExp::DotRegExp(bool selected)
    : RegExp(selected)
{
}

bool DotRegExp::check(ErrorMap &, bool, bool)
{
    return false;
}

QDomNode DotRegExp::toXml(QDomDocument *doc) const
{
    return doc->createElement(QStringLiteral("AnyChar"));
}

bool DotRegExp::load(const QDomElement & /* top */, const QString & /*version*/)
{
    // Nothing to do
    return true;
}
