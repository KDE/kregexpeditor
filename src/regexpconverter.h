/*
 *  SPDX-FileCopyrightText: 2002-2004 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include <QList>

class QTextEdit;
class AltnRegExp;
class ConcRegExp;
class LookAheadRegExp;
class TextRangeRegExp;
class CompoundRegExp;
class DotRegExp;
class PositionRegExp;
class RepeatRegExp;
class TextRegExp;
class RegexpHighlighter;
class RegExp;
class QChar;

class RegExpConverter
{
public:
    enum Features {
        WordBoundary = 0x01,
        NonWordBoundary = 0x02,
        WordStart = 0x04,
        WordEnd = 0x08,
        PosLookAhead = 0x10,
        NegLookAhead = 0x20,
        CharacterRangeNonItems = 0x40,
        ExtRange = 0x80
    };
    virtual ~RegExpConverter()
    {
    }

    virtual bool canParse() = 0;
    virtual QString name() = 0;
    virtual int features() = 0;
    virtual RegExp *parse(const QString &, bool *ok);
    QString toStr(RegExp *, bool markSelection);
    virtual RegexpHighlighter *highlighter(QTextEdit *);

    static void setCurrent(RegExpConverter *);
    static RegExpConverter *current();

protected:
    virtual QString toString(AltnRegExp *, bool markSelection) = 0;
    virtual QString toString(ConcRegExp *, bool markSelection) = 0;
    virtual QString toString(LookAheadRegExp *, bool markSelection) = 0;
    virtual QString toString(TextRangeRegExp *, bool markSelection) = 0;
    virtual QString toString(CompoundRegExp *, bool markSelection) = 0;
    virtual QString toString(DotRegExp *, bool markSelection) = 0;
    virtual QString toString(PositionRegExp *, bool markSelection) = 0;
    virtual QString toString(RepeatRegExp *, bool markSelection) = 0;
    virtual QString toString(TextRegExp *, bool markSelection) = 0;
    QString escape(const QString &text, const QList<QChar> &chars, QChar escapeChar) const;

private:
    static RegExpConverter *_current;
};
