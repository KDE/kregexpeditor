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
    QDomElement top = doc->createElement(QString::fromLocal8Bit("Repeat"));
    top.setAttribute(QString::fromLocal8Bit("lower"), _lower);
    top.setAttribute(QString::fromLocal8Bit("upper"), _upper);
    top.appendChild(_child->toXml(doc));
    return top;
}

bool RepeatRegExp::load(QDomElement top, const QString &version)
{
    Q_ASSERT(top.tagName() == QString::fromLocal8Bit("Repeat"));
    QString lower = top.attribute(QString::fromLocal8Bit("lower"), QString::fromLocal8Bit("0"));
    QString upper = top.attribute(QString::fromLocal8Bit("upper"), QString::fromLocal8Bit("0"));
    bool ok;
    _lower = lower.toInt(&ok);
    if (!ok) {
        KMessageBox::sorry(nullptr, i18n("<p>Value for attribute <b>%1</b> was not an integer for element "
                                         "<b>%2</b></p><p>It contained the value <b>%3</b></p>",
                                         QString::fromLatin1("lower"), QString::fromLatin1("Repeat"), lower),
                           i18n("Error While Loading From XML File"));
        _lower = 0;
    }
    _upper = upper.toInt(&ok);
    if (!ok) {
        KMessageBox::sorry(nullptr, i18n("<p>Value for attribute <b>%1</b> was not an integer for element "
                                         "<b>%2</b></p><p>It contained the value <b>%3</b></p>",
                                         QString::fromLatin1("upper"), QString::fromLatin1("Repeat"), upper),
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
