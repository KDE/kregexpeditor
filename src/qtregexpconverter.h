/*
 *  SPDX-FileCopyrightText: 2002-2004 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include "regexpconverter.h"

class QtRegExpConverter : public RegExpConverter
{
public:
    bool canParse() override;
    RegExp *parse(const QString &, bool *ok) override;
    QString name() override;
    int features() override;
    QString toString(AltnRegExp *, bool markSelection) override;
    QString toString(ConcRegExp *, bool markSelection) override;
    QString toString(LookAheadRegExp *, bool markSelection) override;
    QString toString(TextRangeRegExp *, bool markSelection) override;
    QString toString(CompoundRegExp *, bool markSelection) override;
    QString toString(DotRegExp *, bool markSelection) override;
    QString toString(PositionRegExp *, bool markSelection) override;
    QString toString(RepeatRegExp *, bool markSelection) override;
    QString toString(TextRegExp *, bool markSelection) override;
    RegexpHighlighter *highlighter(QTextEdit *edit) override; // krazy:exclude=qclasses
};
