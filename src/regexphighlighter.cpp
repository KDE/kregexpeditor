/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

// krazy:excludeall=qclasses

#include "regexphighlighter.h"
#include <QTextEdit>

RegexpHighlighter::RegexpHighlighter(QTextEdit *edit)
    : QSyntaxHighlighter(edit)
    , _caseSensitive(false)
    , _minimal(false)
{
}

void RegexpHighlighter::setRegExp(const QString &regexp)
{
    _regexp = regexp;
}

void RegexpHighlighter::setCaseSensitive(bool b)
{
    _caseSensitive = b;
}

void RegexpHighlighter::setMinimal(bool b)
{
    _minimal = b;
}
