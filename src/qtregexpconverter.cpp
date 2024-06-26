/*
 *  SPDX-FileCopyrightText: 2002-2004 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "qtregexpconverter.h"

#include "altnregexp.h"
#include "compoundregexp.h"
#include "concregexp.h"
#include "dotregexp.h"
#include "lookaheadregexp.h"
#include "positionregexp.h"
#include "qtregexphighlighter.h"
#include "regexp.h"
#include "repeatregexp.h"
#include "textrangeregexp.h"
#include "textregexp.h"

extern RegExp *parseQtRegExp(const QString &qstr, bool *ok);

bool QtRegExpConverter::canParse()
{
    return true;
}

RegExp *QtRegExpConverter::parse(const QString &txt, bool *ok)
{
    return parseQtRegExp(txt, ok);
}

QString QtRegExpConverter::toString(AltnRegExp *regexp, bool markSelection)
{
    QString res;

    bool first = true;
    const RegExpList list = regexp->children();
    for (RegExp *r : list) {
        if (!first) {
            res += QLatin1StringView("|");
        }

        first = false;
        if (markSelection && !regexp->isSelected() && r->isSelected()) {
            res += QStringLiteral("(") + toStr(r, markSelection) + QStringLiteral(")");
        } else {
            res += toStr(r, markSelection);
        }
    }
    return res;
}

QString QtRegExpConverter::toString(ConcRegExp *regexp, bool markSelection)
{
    QString res;
    bool childSelected = false;

    const auto children = regexp->children();
    for (const auto child : children) {
        QString startPar;
        QString endPar;
        if (child->precedence() < regexp->precedence()) {
            if (markSelection) {
                startPar = QStringLiteral("(?:");
            } else {
                startPar = QStringLiteral("(");
            }
            endPar = QStringLiteral(")");
        }

        // Note these two have different tests! They are activated in each their iteration of the loop.
        if (markSelection && !childSelected && !regexp->isSelected() && child->isSelected()) {
            res += QLatin1StringView("(");
            childSelected = true;
        }

        if (markSelection && childSelected && !regexp->isSelected() && !child->isSelected()) {
            res += QLatin1StringView(")");
            childSelected = false;
        }

        res += startPar + toStr(child, markSelection) + endPar;
    }
    if (markSelection && childSelected && !regexp->isSelected()) {
        res += QLatin1StringView(")");
    }
    return res;
}

QString QtRegExpConverter::toString(LookAheadRegExp *regexp, bool markSelection)
{
    if (regexp->lookAheadType() == LookAheadRegExp::POSITIVE) {
        return QStringLiteral("(?=") + toStr(regexp->child(), markSelection) + QStringLiteral(")");
    } else {
        return QStringLiteral("(?!") + toStr(regexp->child(), markSelection) + QStringLiteral(")");
    }
}

QString QtRegExpConverter::toString(TextRangeRegExp *regexp, bool /*markSelection*/)
{
    QString txt;

    bool foundCarrot = false;
    bool foundDash = false;
    bool foundParenthesis = false;

    // Now print the rest of the single characters, but keep "^" as the very
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
        res.append(QLatin1StringView("^"));
    }

    // a ']' must be the first character in teh range.
    if (foundParenthesis) {
        res.append(QLatin1StringView("]"));
    }

    // a '-' must be the first character ( only coming after a ']')
    if (foundDash) {
        res.append(QLatin1StringView("-"));
    }

    res += txt;

    // Insert \s,\S,\d,\D,\w, and \W
    if (regexp->digit()) {
        res += QStringLiteral("\\d");
    }

    if (regexp->nonDigit()) {
        res += QStringLiteral("\\D");
    }

    if (regexp->space()) {
        res += QStringLiteral("\\s");
    }

    if (regexp->nonSpace()) {
        res += QStringLiteral("\\S");
    }

    if (regexp->wordChar()) {
        res += QStringLiteral("\\w");
    }

    if (regexp->nonWordChar()) {
        res += QStringLiteral("\\W");
    }

    if (foundCarrot) {
        res.append(QLatin1Char('^'));
    }

    res.append(QLatin1StringView("]"));

    return res;
}

QString QtRegExpConverter::toString(CompoundRegExp *regexp, bool markSelection)
{
    if (markSelection && !regexp->isSelected() && regexp->child()->isSelected()) {
        return QStringLiteral("(") + toStr(regexp->child(), markSelection) + QStringLiteral(")");
    } else {
        return toStr(regexp->child(), markSelection);
    }
}

QString QtRegExpConverter::toString(DotRegExp * /*regexp*/, bool /*markSelection*/)
{
    return QStringLiteral(".");
}

QString QtRegExpConverter::toString(PositionRegExp *regexp, bool /*markSelection*/)
{
    switch (regexp->position()) {
    case PositionRegExp::BEGLINE:
        return QStringLiteral("^");
    case PositionRegExp::ENDLINE:
        return QStringLiteral("$");
    case PositionRegExp::WORDBOUNDARY:
        return QStringLiteral("\\b");
    case PositionRegExp::NONWORDBOUNDARY:
        return QStringLiteral("\\B");
    }
    Q_ASSERT(false);
    return QString();
}

QString QtRegExpConverter::toString(RepeatRegExp *regexp, bool markSelection)
{
    RegExp *child = regexp->child();
    QString cText = toStr(child, markSelection);
    QString startPar;
    QString endPar;
    QString quantity;

    if (markSelection) {
        if (!regexp->isSelected() && child->isSelected()) {
            startPar = QStringLiteral("(");
            endPar = QStringLiteral(")");
        } else if (child->precedence() < regexp->precedence()) {
            startPar = QStringLiteral("(?:");
            endPar = QStringLiteral(")");
        }
    } else if (child->precedence() < regexp->precedence()) {
        startPar = QStringLiteral("(");
        endPar = QStringLiteral(")");
    }

    if (regexp->min() == 0 && regexp->max() == -1) {
        quantity = QStringLiteral("*");
    } else if (regexp->min() == 0 && regexp->max() == 1) {
        quantity = QStringLiteral("?");
    } else if (regexp->min() == 1 && regexp->max() == -1) {
        quantity = QStringLiteral("+");
    } else if (regexp->max() == -1) {
        quantity = QStringLiteral("{") + QString::number(regexp->min()) + QStringLiteral(",") + QStringLiteral("}");
    } else {
        quantity = QStringLiteral("{") + QString::number(regexp->min()) + QStringLiteral(",") + QString::number(regexp->max()) + QStringLiteral("}");
    }

    return startPar + cText + endPar + quantity;
}

QString QtRegExpConverter::toString(TextRegExp *regexp, bool /*markSelection*/)
{
    QList<QChar> list;
    list << QLatin1Char('$') << QLatin1Char('^') << QLatin1Char('.') << QLatin1Char('*') << QLatin1Char('+') << QLatin1Char('?') << QLatin1Char('[')
         << QLatin1Char(']') << QLatin1Char('\\') << QLatin1Char('{') << QLatin1Char('}') << QLatin1Char('(') << QLatin1Char(')') << QLatin1Char('|');

    QString res = escape(regexp->text(), list, QLatin1Char('\\'));
    return res;
}

QString QtRegExpConverter::name()
{
    return QStringLiteral("Qt");
}

int QtRegExpConverter::features()
{
    return WordBoundary | NonWordBoundary | PosLookAhead | NegLookAhead | CharacterRangeNonItems | ExtRange;
}

RegexpHighlighter *QtRegExpConverter::highlighter(QTextEdit *edit) // krazy:exclude=qclasses
{
    return new QtRegexpHighlighter(edit);
}
