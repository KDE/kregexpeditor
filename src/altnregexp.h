/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

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
