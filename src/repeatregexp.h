/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include "regexp.h"

/**
   Abstract syntax node for `repeated content' regular expression
   @internal
*/
class RepeatRegExp : public RegExp
{
public:
    explicit RepeatRegExp(bool selected, int lower = 0, int upper = 0, RegExp *child = nullptr);

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return 3;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
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
    RegExp *_child = nullptr;
};
