/*
 *  SPDX-FileCopyrightText: 2002-2004 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
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
