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

#include "altnregexp.h"
#include "widgetfactory.h"

AltnRegExp::AltnRegExp(bool selected) : RegExp(selected)
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
    foreach (RegExp *r, list) {
        possibleEmpty = r->check(map, first, last) || possibleEmpty;
    }
    return possibleEmpty;
}

QDomNode AltnRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top = doc->createElement(QString::fromLocal8Bit("Alternatives"));
    foreach (RegExp *r, list) {
        top.appendChild(r->toXml(doc));
    }
    return top;
}

bool AltnRegExp::load(QDomElement top, const QString &version)
{
    Q_ASSERT(top.tagName() == QString::fromLocal8Bit("Alternatives"));

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
    foreach (RegExp *r, list) {
        r->replacePart(replacement);
    }
}
