/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include <QSyntaxHighlighter>

// krazy:excludeall=qclasses

class QTextEdit;

class RegexpHighlighter : public QSyntaxHighlighter
{
public:
    explicit RegexpHighlighter(QTextEdit *edit);
    void setRegExp(const QString &regexp);
    void setCaseSensitive(bool);
    void setMinimal(bool);

protected:
    QString _regexp;
    bool _caseSensitive, _minimal;
};
