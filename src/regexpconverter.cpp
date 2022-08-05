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

#include "regexpconverter.h"

#include <QTextEdit>

#include "altnregexp.h"
#include "concregexp.h"
#include "lookaheadregexp.h"
#include "textrangeregexp.h"
#include "compoundregexp.h"
#include "dotregexp.h"
#include "positionregexp.h"
#include "repeatregexp.h"
#include "textregexp.h"
#include "regexphighlighter.h"

RegExpConverter *RegExpConverter::_current = nullptr;
RegExp *RegExpConverter::parse(const QString &, bool *ok)
{
    *ok = false;
    return new DotRegExp(false);   // This method should never be called.
}

// This function needs to be called toStr rather than toString, as it is not possible to
// over load function across inheritance!
QString RegExpConverter::toStr(RegExp *regexp, bool markSelection)
{
    switch (regexp->type()) {
    case RegExp::CONC:
        return toString(static_cast<ConcRegExp *>(regexp), markSelection);
    case RegExp::TEXT:
        return toString(static_cast<TextRegExp *>(regexp), markSelection);
    case RegExp::DOT:
        return toString(static_cast<DotRegExp *>(regexp), markSelection);
    case RegExp::POSITION:
        return toString(static_cast<PositionRegExp *>(regexp), markSelection);
    case RegExp::REPEAT:
        return toString(static_cast<RepeatRegExp *>(regexp), markSelection);
    case RegExp::ALTN:
        return toString(static_cast<AltnRegExp *>(regexp), markSelection);
    case RegExp::COMPOUND:
        return toString(static_cast<CompoundRegExp *>(regexp), markSelection);
    case RegExp::LOOKAHEAD:
        return toString(static_cast<LookAheadRegExp *>(regexp), markSelection);
    case RegExp::TEXTRANGE:
        return toString(static_cast<TextRangeRegExp *>(regexp), markSelection);
    }
    qWarning("We shouldn't get here!");
    return QString();
}

QString RegExpConverter::escape(const QString &text, const QList<QChar> &chars, QChar escapeChar) const
{
    QString res;
    for (int i = 0; i < text.length(); i++) {
        for (int j = 0; j < chars.count(); j++) {
            if (text.at(i) == (chars[j])) {
                res.append(escapeChar);
                break;
            }
        }
        res.append(text.at(i));
    }

    return res;
}

/**
   Returns a QSyntaxHighlighter to be used in the verifyer widget.
*/
RegexpHighlighter *RegExpConverter::highlighter(QTextEdit *)
{
    return nullptr;
}

RegExpConverter *RegExpConverter::current()
{
    return _current;
}

void RegExpConverter::setCurrent(RegExpConverter *converter)
{
    _current = converter;
}
