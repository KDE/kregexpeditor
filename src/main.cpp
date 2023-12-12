/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include <KAboutData>
#include <KCrash>

#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "kregexpeditorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KCrash::initialize();

    KLocalizedString::setApplicationDomain("kregexpeditor");

    KAboutData aboutData(QStringLiteral("kregexpeditor"),
                         i18n("RegExp Editor"),
                         QStringLiteral("1.0"),
                         i18n("Editor for Regular Expressions"),
                         KAboutLicense::GPL,
                         i18n("(c) 2002-2003 Jesper K. Pedersen"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Developer"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(i18nc("@info:credit", "Carl Schwan"),
                        i18nc("@info:credit", "Port to Qt6"),
                        QStringLiteral("carl@carlschwan.eu"),
                        QStringLiteral("https://carlschwan.eu"),
                        QUrl(QStringLiteral("https://carlschwan.eu/avatar.png")));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KRegExpEditorWindow window(nullptr);
    window.show();

    return app.exec();
}
