/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *  Copyright (c) 2011 Morten A. B. Sjøgren <m_abs@mabs.dk>
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

#include "qtregexphighlighter.h"

#include <QTextEdit>
#include <QRegularExpression>

// krazy:excludeall=qclasses

QtRegexpHighlighter::QtRegexpHighlighter(QTextEdit *editor)
    : RegexpHighlighter(editor)
    , _editor(editor)
{
}

void QtRegexpHighlighter::highlightBlock(const QString &text)
{
    QRegularExpression regexp(_regexp);
    QRegularExpression::PatternOptions options;
    if (!_caseSensitive) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    if (!_minimal) {
        options |= QRegularExpression::InvertedGreedinessOption;
    }
    regexp.setPatternOptions(options);

    QTextCharFormat format;
    format.setForeground(Qt::black);
    format.setFont(_editor->font());
    setFormat(0, text.length(), format);

    if (!regexp.isValid() || regexp.pattern().isEmpty()) {
        return;
    }

    // ------------------------------ Process with the regular expression.
    QColor colors[] = { Qt::red, Qt::blue };
    int color = previousBlockState();
    if (color < 0 || color > 1) {
        color = 0;
    }

    int index = 0;
    int start, length;
    QRegularExpressionMatch match;
    while ((index = text.indexOf(regexp, index, &match)) != -1 && index < (int)text.length()) {
        if (match.capturedStart(1) != -1) {
            start = match.capturedStart(1);
            length = match.captured(1).length();
        } else {
            start = index;
            length = match.capturedLength();
        }

        if (start != index) {
            setFormat(index, start - index, colors[color]);
        }

        QFont font = _editor->font();
        font.setUnderline(true);
        font.setPointSize((int)(font.pointSize() * 1.3));
        QTextCharFormat format;
        format.setFont(font);
        format.setForeground(colors[color]);
        setFormat(start, length, format);

        if (length + (start - index) != match.capturedLength()) {
            setFormat(start + length, match.capturedLength() - length - (start - index), colors[color]);
        }

        index += qMax(1, match.capturedLength());    // ensure progress when matching for example ^ or \b
        color = (color + 1) % 2;
    }
    setCurrentBlockState(color);
}
