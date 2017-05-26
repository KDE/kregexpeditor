/*
 *  Copyright (c) 2002-2004 Jesper K. Pedersen <blackie@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#ifndef EMACSREGEXPCONVERTER_H
#define EMACSREGEXPCONVERTER_H

#include "regexpconverter.h"

class EmacsRegExpConverter : public RegExpConverter
{
public:
    bool canParse() override;
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
};

#endif /* EMACSREGEXPCONVERTER_H */
