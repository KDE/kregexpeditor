/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef __CONCREGEXP_H
#define __CONCREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `concatenated' regular expression
   @internal
*/
class ConcRegExp : public RegExp
{
public:
    ConcRegExp(bool selected);

    void addRegExp(RegExp *);
    RegExpList children();
    RegExp *lastRegExp();

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return 2;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
    RegExpType type() const override
    {
        return CONC;
    }

    bool operator==(const RegExp &other) const override;
    void replacePart(CompoundRegExp *replacement) override;

private:
    RegExpList list;
};

#endif // __CONCREGEXP_H
