/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef __textregexp_h
#define __textregexp_h

#include "regexp.h"

/**
   Abstract syntax node for `text' regular expression
   @internal
*/
class TextRegExp : public RegExp
{
public:
    explicit TextRegExp(bool selected, const QString &text = QString());

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        if (_text.length() > 1) {
            return 2;
        } else {
            return 4;
        }
    }

    QString text() const
    {
        return _text;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
    void append(const QString &str);
    RegExpType type() const override
    {
        return TEXT;
    }

    bool operator==(const RegExp &other) const override;

private:
    QString _text;
};

#endif // __textregexp_h
