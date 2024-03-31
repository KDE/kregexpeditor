/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include "regexp.h"

#include <QList>
#include <QPair>
#include <QStringList>

typedef QPair<QString, QString> StringPair;

/**
   Abstract syntax node for `text range' regular expression

   @internal
*/
class TextRangeRegExp : public RegExp
{
public:
    TextRangeRegExp(bool selected);
    ~TextRangeRegExp() override;

    void addCharacter(const QString &ch);
    QStringList chars() const
    {
        return _chars;
    }

    void clearChars()
    {
        _chars.clear();
    }

    void addRange(const QString &from, const QString &to);
    QList<StringPair> range() const
    {
        return _ranges;
    }

    void clearRange()
    {
        _ranges.clear();
    }

    void setNegate(bool set)
    {
        _negate = set;
    }

    void setDigit(bool set)
    {
        _digit = set;
    }

    void setNonDigit(bool set)
    {
        _nonDigit = set;
    }

    void setSpace(bool set)
    {
        _space = set;
    }

    void setNonSpace(bool set)
    {
        _nonSpace = set;
    }

    void setWordChar(bool set)
    {
        _wordChar = set;
    }

    void setNonWordChar(bool set)
    {
        _nonWordChar = set;
    }

    bool negate() const
    {
        return _negate;
    }

    bool digit() const
    {
        return _digit;
    }

    bool nonDigit() const
    {
        return _nonDigit;
    }

    bool space() const
    {
        return _space;
    }

    bool nonSpace() const
    {
        return _nonSpace;
    }

    bool wordChar() const
    {
        return _wordChar;
    }

    bool nonWordChar() const
    {
        return _nonWordChar;
    }

    bool check(ErrorMap &, bool first, bool last) override;
    int precedence() const override
    {
        return 4;
    }

    QDomNode toXml(QDomDocument *doc) const override;
    bool load(const QDomElement &, const QString &version) override;
    RegExpType type() const override
    {
        return TEXTRANGE;
    }

    bool operator==(const RegExp &other) const override;

private:
    bool _negate, _digit, _nonDigit, _space, _nonSpace, _wordChar, _nonWordChar;
    QStringList _chars;
    QList<StringPair> _ranges;
};
