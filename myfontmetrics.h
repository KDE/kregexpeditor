#ifndef __MYFONTMETRICS_H
#define __MYFONTMETRICS_H
#include <qfontmetrics.h>
#include <qstringlist.h>

// QFontMetrics::size is broken, therefore I need this hack until it works in QT3

QSize HackCalculateFontSize(QFontMetrics fm, QString str );

#endif
