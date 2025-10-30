/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include "regexp.h"

/**
   Abstract syntax node for "the dot" regular expression (i.e. any characters)
   @internal
*/
class DotRegExp : public RegExp
{
public:
    DotRegExp(bool selected);

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return 4;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
    RegExpType type() const override
    {
        return DOT;
    }
};
