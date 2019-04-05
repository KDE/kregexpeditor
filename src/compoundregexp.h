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

#ifndef COMPOUNDREGEXP_H
#define COMPOUNDREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `compound content' regular expression
   @internal
*/
class CompoundRegExp : public RegExp
{
public:
    explicit CompoundRegExp(bool selected, const QString &title = QString(), const QString &description = QString(), bool hidden = false, bool allowReplace = false, RegExp *child = nullptr);

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return _child->precedence();
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
    QString title() const
    {
        return _title;
    }

    QString description() const
    {
        return _description;
    }

    RegExp *child() const
    {
        return _child;
    }

    bool hidden() const
    {
        return _hidden;
    }

    bool allowReplace() const
    {
        return _allowReplace;
    }

    RegExpType type() const override
    {
        return COMPOUND;
    }

    bool operator==(const RegExp &other) const override;

private:
    QString _title;
    QString _description;
    bool _hidden;
    bool _allowReplace;
    RegExp *_child = nullptr;
};

#endif // COMPOUNDREGEXP_H
