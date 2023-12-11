/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "myfontmetrics.h"

#include <QStringList>

QSize HackCalculateFontSize(const QFontMetrics &fm, const QString &str)
{
    QStringList list;
    int maxWidth = 0;
    int height = 0;

    if (str.isEmpty()) {
        return QSize(0, 0);
    }
    list = str.split(QStringLiteral("\n"), Qt::SkipEmptyParts);

    const QStringList::const_iterator end(list.constEnd());
    for (QStringList::const_iterator it = list.constBegin(); it != end; ++it) {
        QSize size = fm.size(0, *it);
        maxWidth = qMax(maxWidth, size.width());
        height += size.height();
    }
    return QSize(maxWidth, height);
}
