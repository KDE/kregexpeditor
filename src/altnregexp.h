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

#ifndef ALTNREGEXP_H
#define ALTNREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `alternative' regular expression
   @internal
*/
class AltnRegExp : public RegExp
{
public:
    AltnRegExp(bool selected);

    void addRegExp(RegExp *);
    RegExpList children() const;

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return 1;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
    RegExpType type() const override
    {
        return ALTN;
    }

    bool operator==(const RegExp &other) const override;
    void replacePart(CompoundRegExp *replacement) override;

private:
    RegExpList list;
};

#endif // ALTNREGEXP_H
