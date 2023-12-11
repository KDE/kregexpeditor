/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "altnregexp.h"
#include "widgetfactory.h"

AltnRegExp::AltnRegExp(bool selected)
    : RegExp(selected)
{
    // Nothing to do
}

void AltnRegExp::addRegExp(RegExp *elm)
{
    list.append(elm);
    addChild(elm);
}

RegExpList AltnRegExp::children() const
{
    return list;
}

bool AltnRegExp::check(ErrorMap &map, bool first, bool last)
{
    bool possibleEmpty = false;
    for (RegExp *r : std::as_const(list)) {
        possibleEmpty = r->check(map, first, last) || possibleEmpty;
    }
    return possibleEmpty;
}

QDomNode AltnRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top = doc->createElement(QStringLiteral("Alternatives"));
    for (RegExp *r : std::as_const(list)) {
        top.appendChild(r->toXml(doc));
    }
    return top;
}

bool AltnRegExp::load(const QDomElement &top, const QString &version)
{
    Q_ASSERT(top.tagName() == QStringLiteral("Alternatives"));

    for (QDomNode child = top.firstChild(); !child.isNull(); child = child.nextSibling()) {
        if (!child.isElement()) {
            continue; // User might have added a comment.
        }

        RegExp *regexp = WidgetFactory::createRegExp(child.toElement(), version);
        if (regexp == nullptr) {
            return false;
        }
        addRegExp(regexp);
    }
    return true;
}

bool AltnRegExp::operator==(const RegExp &other) const
{
    // TODO: merge with ConcRegExp::operator==

    if (other.type() != type()) {
        return false;
    }

    const AltnRegExp &theOther = dynamic_cast<const AltnRegExp &>(other);

    if (list.count() != theOther.list.count()) {
        return false;
    }

    RegExpListIt it1(list);
    RegExpListIt it2(theOther.list);

    while (it1.hasNext() && it2.hasNext()) {
        if (!(it1.next() == it2.next())) {
            return false;
        }
    }
    return true;
}

void AltnRegExp::replacePart(CompoundRegExp *replacement)
{
    for (RegExp *r : std::as_const(list)) {
        r->replacePart(replacement);
    }
}
