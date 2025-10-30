/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include "regexp.h"

/**
   Abstract syntax node for `compound content' regular expression
   @internal
*/
class CompoundRegExp : public RegExp
{
public:
    explicit CompoundRegExp(bool selected,
                            const QString &title = QString(),
                            const QString &description = QString(),
                            bool hidden = false,
                            bool allowReplace = false,
                            RegExp *child = nullptr);

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
