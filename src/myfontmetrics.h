/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef __MYFONTMETRICS_H
#define __MYFONTMETRICS_H

#include <QFontMetrics>

// QFontMetrics::size is broken, therefore I need this hack until it works in QT3

QSize HackCalculateFontSize(const QFontMetrics &fm, const QString &str);

#endif
