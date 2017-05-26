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

#ifndef __REPEATREGEXP_H
#define __REPEATREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `repeated content' regular expression
   @internal
*/
class RepeatRegExp : public RegExp
{
public:
    explicit RepeatRegExp(bool selected, int lower = 0, int upper = 0, RegExp *child = 0);

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return 3;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(QDomElement, const QString &version) override;
    int min() const
    {
        return _lower;
    }

    int max() const
    {
        return _upper;
    }

    RegExp *child() const
    {
        return _child;
    }

    RegExpType type() const override
    {
        return REPEAT;
    }

    bool operator==(const RegExp &other) const override;
    void replacePart(CompoundRegExp *replacement) override
    {
        _child->replacePart(replacement);
    }

private:
    int _lower;
    int _upper;
    RegExp *_child;
};

#endif // __REPEATREGEXP_H
