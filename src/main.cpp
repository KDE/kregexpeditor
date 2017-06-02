/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
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

#include <KAboutData>
#include <KCrash>

#include <KLocalizedString>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCommandLineParser>

#include "kregexpeditorguidialog.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    KCrash::initialize();


    KLocalizedString::setApplicationDomain("kregexpeditor");

    KAboutData aboutData(QStringLiteral("kregexpeditor"), i18n("RegExp Editor"),
                         QStringLiteral("1.0"), i18n("Editor for Regular Expressions"),
                         KAboutLicense::GPL,
                         i18n("(c) 2002-2003 Jesper K. Pedersen"));
    aboutData.addAuthor(i18n("Laurent Montel"), i18n("Developper"), QStringLiteral("montel@kde.org"));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KRegExpEditorGUIDialog iface(0);

    iface.show();

    app.setQuitOnLastWindowClosed(true);
    return app.exec();
}
