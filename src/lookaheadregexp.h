/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include "regexp.h"

/**
   Abstract syntax node for `repeated content' regular expression
   @internal
*/
class LookAheadRegExp : public RegExp
{
public:
    enum TYPE {
        POSITIVE,
        NEGATIVE
    };

    LookAheadRegExp(bool selected, TYPE tp, RegExp *child = nullptr);

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return 4;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
    RegExp *child() const
    {
        return _child;
    }

    TYPE lookAheadType() const
    {
        return _tp;
    }

    RegExpType type() const override
    {
        return LOOKAHEAD;
    }

    bool operator==(const RegExp &other) const override;
    void replacePart(CompoundRegExp *replacement) override
    {
        _child->replacePart(replacement);
    }

private:
    RegExp *_child = nullptr;
    TYPE _tp;
};
