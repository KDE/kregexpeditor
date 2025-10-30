/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include "concregexp.h"

#include "compoundregexp.h"
#include "widgetfactory.h"

ConcRegExp::ConcRegExp(bool selected)
    : RegExp(selected)
{
}

void ConcRegExp::addRegExp(RegExp *regExp)
{
    list.append(regExp);
    addChild(regExp);
}

RegExpList ConcRegExp::children()
{
    return list;
}

bool ConcRegExp::check(ErrorMap &map, bool first, bool last)
{
    bool f = first;
    bool possibleEmpty = true;
    RegExpListIt it(list);
    while (it.hasNext()) {
        possibleEmpty = it.next()->check(map, f, last && !it.hasNext()) && possibleEmpty;
        if (!possibleEmpty) {
            f = false;
        }
    }
    return possibleEmpty;
}

RegExp *ConcRegExp::lastRegExp()
{
    if (list.isEmpty()) {
        return nullptr;
    } else {
        return list.at(list.count() - 1);
    }
}

QDomNode ConcRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top = doc->createElement(QStringLiteral("Concatenation"));
    for (RegExp *r : std::as_const(list)) {
        top.appendChild(r->toXml(doc));
    }
    return top;
}

bool ConcRegExp::load(const QDomElement &top, const QString &version)
{
    Q_ASSERT(top.tagName() == QStringLiteral("Concatenation"));

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

bool ConcRegExp::operator==(const RegExp &other) const
{
    // TODO: Merge with AltnRegExp::operator==
    if (list.count() == 1) {
        return other == *(const_cast<QList<RegExp *> &>(list).at(0));
    }

    if (other.type() != type()) {
        return false;
    }

    const ConcRegExp &theOther = dynamic_cast<const ConcRegExp &>(other);

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

void ConcRegExp::replacePart(CompoundRegExp *replacement)
{
    RegExp *otherChild = replacement->child();
    ConcRegExp *otherConc = dynamic_cast<ConcRegExp *>(otherChild);
    if (!otherConc) {
        // Watch out for garbage here!
        otherConc = new ConcRegExp(false);
        otherConc->addRegExp(otherChild);
    }

    RegExpList newList;
    RegExpListStlIt it1(list.begin());
    while (it1 != list.end()) {
        (*it1)->replacePart(replacement);
        RegExpListStlIt it2 = it1;
        RegExpListStlIt it3(otherConc->list.begin());
        bool match = true;
        int count = 0;

        // See if replacement is a sublist of list starting from what it1 points at
        for (; it2 != list.end() && it3 != otherConc->list.end() && match;) {
            if (!(**it2 == **it3)) {
                match = false;
            }
            ++it2;
            ++it3;
            ++count;
        }

        if (match && it3 == otherConc->list.end()) {
            // I found a match, delete the element in it1, which is common with it3
            while (*it1 != *it2) {
                RegExp *item = *it1;
                ++it1;
                delete item;
            }
            RegExp *theClone = replacement->clone();
            newList.append(theClone);
            addChild(theClone);
        } else {
            newList.append(*it1);
            ++it1;
        }
    }
    list = newList;
}
