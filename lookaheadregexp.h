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

#ifndef __LOOKAHEADREGEXP_H
#define __LOOKAHEADREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `repeated content' regular expression
   @internal
*/
class LookAheadRegExp : public RegExp
{
public:
    enum TYPE { POSITIVE, NEGATIVE };

    LookAheadRegExp(bool selected, TYPE tp, RegExp *child = 0);

    virtual bool check(ErrorMap &, bool first, bool last);
    virtual int precedence() const
    {
        return 4;
    }
    virtual QDomNode toXml(QDomDocument *doc) const;
    virtual bool load(QDomElement, const QString &version);
    RegExp *child() const
    {
        return _child;
    }
    TYPE lookAheadType() const
    {
        return _tp;
    }
    virtual RegExpType type() const
    {
        return LOOKAHEAD;
    }
    virtual bool operator==(const RegExp &other) const;
    virtual void replacePart(CompoundRegExp *replacement)
    {
        _child->replacePart(replacement);
    }

private:
    RegExp *_child;
    TYPE _tp;
};

#endif // __LOOKAHEADREGEXP_H
