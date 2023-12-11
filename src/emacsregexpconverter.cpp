/*
 *  SPDX-FileCopyrightText: 2002-2004 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "emacsregexpconverter.h"

#include <KLocalizedString>
#include <KMessageBox>

#include "altnregexp.h"
#include "compoundregexp.h"
#include "concregexp.h"
#include "positionregexp.h"
#include "regexp.h"
#include "repeatregexp.h"
#include "textrangeregexp.h"
#include "textregexp.h"

bool EmacsRegExpConverter::canParse()
{
    return false;
}

QString EmacsRegExpConverter::toString(AltnRegExp *regexp, bool markSelection)
{
    QString res;

    bool first = true;
    const RegExpList children = regexp->children();
    for (RegExp *child : children) {
        if (!first) {
            res += QLatin1String("\\|");
        }
        first = false;
        res += toStr(child, markSelection);
    }

    return res;
}

QString EmacsRegExpConverter::toString(ConcRegExp *regexp, bool markSelection)
{
    QString res;

    const RegExpList children = regexp->children();
    for (RegExp *child : children) {
        QString startPar;
        QString endPar;
        if (child->precedence() < regexp->precedence()) {
            startPar = QStringLiteral("\\(");
            endPar = QStringLiteral("\\)");
        }

        res += startPar + toStr(child, markSelection) + endPar;
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
    const auto range = regexp->range();
    for (const StringPair &elm : range) {
        txt.append(elm.first + QLatin1Char('-') + elm.second);
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
        res += QLatin1Char(' ') + QLatin1Char((char)9); // Tab char
    }

    if (regexp->wordChar()) {
        res += QStringLiteral("a-zA-Z");
    }

    if (foundCarrot) {
        res.append(QLatin1Char('^'));
    }

    res.append(QLatin1Char(']'));

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
    list << QLatin1Char('$') << QLatin1Char('^') << QLatin1Char('.') << QLatin1Char('*') << QLatin1Char('+') << QLatin1Char('?') << QLatin1Char('[')
         << QLatin1Char(']') << QLatin1Char('\\');

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
