/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include "regexphighlighter.h"

class QTextEdit;

// krazy:excludeall=qclasses

class QtRegexpHighlighter : public RegexpHighlighter
{
public:
    QtRegexpHighlighter(QTextEdit *verifier);
    void highlightBlock(const QString &text) override;

private:
    QTextEdit *_editor = nullptr;
};
