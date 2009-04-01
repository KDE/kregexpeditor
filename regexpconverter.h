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

#ifndef REGEXPCONVERTER_H
#define REGEXPCONVERTER_H

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
    virtual ~RegExpConverter(){}
    virtual bool canParse() = 0;
    virtual QString name() = 0;
    virtual int features() = 0;
    virtual RegExp* parse( const QString&, bool* ok );
    QString toStr( RegExp*, bool markSelection );
    virtual RegexpHighlighter* highlighter( QTextEdit* );

    static void setCurrent( RegExpConverter* );
    static RegExpConverter* current();

protected:
    virtual QString toString( AltnRegExp*, bool markSelection ) = 0;
    virtual QString toString( ConcRegExp*, bool markSelection ) = 0;
    virtual QString toString( LookAheadRegExp*, bool markSelection ) = 0;
    virtual QString toString( TextRangeRegExp*, bool markSelection ) = 0;
    virtual QString toString( CompoundRegExp*, bool markSelection ) = 0;
    virtual QString toString( DotRegExp*, bool markSelection ) = 0;
    virtual QString toString( PositionRegExp*, bool markSelection ) = 0;
    virtual QString toString( RepeatRegExp*, bool markSelection ) = 0;
    virtual QString toString( TextRegExp*, bool markSelection ) = 0;
    QString escape( QString text, QList<QChar> chars, QChar escapeChar) const;

private:
    static RegExpConverter* _current;
};

#endif /* REGEXPCONVERTER_H */

