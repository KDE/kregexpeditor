/*
   SPDX-FileCopyrightText: 2017 Montel Laurent <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QStandardPaths>
#include <QCommandLineParser>
#include <QApplication>
#include "regexpeditorwidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process(app);

    RegExpEditorWidget *w = new RegExpEditorWidget;
    w->resize(800, 600);
    w->show();
    app.exec();
    delete w;
    return 0;
}
