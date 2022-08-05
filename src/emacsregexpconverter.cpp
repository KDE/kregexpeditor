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

#include "emacsregexpconverter.h"

#include <KMessageBox>
#include <KLocalizedString>

#include "regexp.h"
#include "altnregexp.h"
#include "concregexp.h"
#include "textrangeregexp.h"
#include "textregexp.h"
#include "compoundregexp.h"
#include "positionregexp.h"
#include "repeatregexp.h"

bool EmacsRegExpConverter::canParse()
{
    return false;
}

QString EmacsRegExpConverter::toString(AltnRegExp *regexp, bool markSelection)
{
    QString res;

    bool first = true;
    RegExpList list = regexp->children();
    foreach (RegExp *r, list) {
        if (!first) {
            res += QLatin1String("\\|");
        }
        first = false;
        res += toStr(r, markSelection);
    }

    return res;
}

QString EmacsRegExpConverter::toString(ConcRegExp *regexp, bool markSelection)
{
    QString res;

    RegExpList list = regexp->children();
    foreach (RegExp *r, list) {
        QString startPar = QString();
        QString endPar = QString();
        if (r->precedence() < regexp->precedence()) {
            startPar = QStringLiteral("\\(");
            endPar = QStringLiteral("\\)");
        }

        res += startPar + toStr(r, markSelection) + endPar;
    }

    return res;
}

QString EmacsRegExpConverter::toString(LookAheadRegExp * /*regexp*/, bool /*markSelection*/)
{
    static bool haveWarned = false;
    if (!haveWarned) {
        KMessageBox::error(nullptr, i18n("Look ahead regular expressions not supported in Emacs style"));
        haveWarned = true;
    }

    return QString();
}

QString EmacsRegExpConverter::toString(TextRangeRegExp *regexp, bool /*markSelection*/)
{
    QString txt;

    bool foundCarrot = false;
    bool foundDash = false;
    bool foundParenthesis = false;

    // print the single characters, but keep "^" as the very
    // last element of the characters.
    QStringList chars = regexp->chars();
    for (int i = 0; i < chars.count(); i++) {
        if (chars.at(i).at(0) == QLatin1Char(']')) {
            foundParenthesis = true;
        } else if (chars.at(i).at(0) == QLatin1Char('-')) {
            foundDash = true;
        } else if (chars.at(i).at(0) == QLatin1Char('^')) {
            foundCarrot = true;
        } else {
            txt.append(chars.at(i).at(0));
        }
    }

    // Now insert the ranges.
    foreach (const StringPair &elm, regexp->range()) {
        txt.append(elm.first + QStringLiteral("-") + elm.second);
    }

    // Ok, its time to build each part of the regexp, here comes the rule:
    // if a ']' is one of the characters, then it must be the first one in the
    // list (after then opening '[' and eventually negation '^')
    // Next if a '-' is one of the characters, then it must come
    // finally if '^' is one of the characters, then it must not be the first
    // one!

    QString res = QStringLiteral("[");

    if (regexp->negate()) {
        res.append(QLatin1String("^"));
    }

    // a ']' must be the first character in teh range.
    if (foundParenthesis) {
        res.append(QLatin1String("]"));
    }

    // a '-' must be the first character ( only coming after a ']')
    if (foundDash) {
        res.append(QLatin1String("-"));
    }

    res += txt;

    // Insert equivalents to \s,\S,\d,\D,\w, and \W
    // non-digit, non-space, and non-word is not supported in Emacs style
    if (regexp->digit()) {
        res += QStringLiteral("0-9");
    }

    if (regexp->space()) {
        res += QStringLiteral(" ") + QString(QLatin1Char((char)9));      // Tab char
    }

    if (regexp->wordChar()) {
        res += QStringLiteral("a-zA-Z");
    }

    if (foundCarrot) {
        res.append(QLatin1Char('^'));
    }

    res.append(QLatin1String("]"));

    return res;
}

QString EmacsRegExpConverter::toString(CompoundRegExp *regexp, bool markSelection)
{
    return toStr(regexp->child(), markSelection);
}

QString EmacsRegExpConverter::toString(DotRegExp * /*regexp*/, bool /*markSelection*/)
{
    return QStringLiteral(".");
}

QString EmacsRegExpConverter::toString(PositionRegExp *regexp, bool /*markSelection*/)
{
    static bool haveWarned = false;
    switch (regexp->position()) {
    case PositionRegExp::BEGLINE:
        return QStringLiteral("^");
    case PositionRegExp::ENDLINE:
        return QStringLiteral("$");
    case PositionRegExp::WORDBOUNDARY:
    case PositionRegExp::NONWORDBOUNDARY:
        if (!haveWarned) {
            KMessageBox::error(nullptr, i18n("Word boundary and non word boundary is not supported in Emacs syntax"));
            haveWarned = true;
            return QString();
        }
    }
    return QString();
}

QString EmacsRegExpConverter::toString(RepeatRegExp *regexp, bool markSelection)
{
    RegExp *child = regexp->child();
    QString cText = toStr(child, markSelection);
    QString startPar;
    QString endPar;

    if (child->precedence() < regexp->precedence()) {
        startPar = QStringLiteral("\\(");
        endPar = QStringLiteral("\\)");
    }

    if (regexp->min() == 0 && regexp->max() == -1) {
        return startPar + cText + endPar + QStringLiteral("*");
    } else if (regexp->min() == 0 && regexp->max() == 1) {
        return startPar + cText + endPar + QStringLiteral("?");
    } else if (regexp->min() == 1 && regexp->max() == -1) {
        return startPar + cText + endPar + QStringLiteral("+");
    } else {
        QString res = QString();
        for (int i = 0; i < regexp->min(); ++i) {
            res += QStringLiteral("\\(") + cText + QStringLiteral("\\)");
        }
        if (regexp->max() != -1) {
            for (int i = regexp->min(); i < regexp->max(); ++i) {
                res += QStringLiteral("\\(") + cText + QStringLiteral("\\)?");
            }
        } else {
            res += QLatin1String("+");
        }

        return res;
    }
}

QString EmacsRegExpConverter::toString(TextRegExp *regexp, bool /*markSelection*/)
{
    QList<QChar> list;
    list << QLatin1Char('$')
         << QLatin1Char('^')
         << QLatin1Char('.')
         << QLatin1Char('*')
         << QLatin1Char('+')
         << QLatin1Char('?')
         << QLatin1Char('[')
         << QLatin1Char(']')
         << QLatin1Char('\\');

    QString res = escape(regexp->text(), list, QLatin1Char('\\'));
    return res;
}

QString EmacsRegExpConverter::name()
{
    return QStringLiteral("Emacs");
}

int EmacsRegExpConverter::features()
{
    return WordStart | WordEnd;
}
