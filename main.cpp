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
#include <KApplication>
#include <KCmdLineArgs>
#include <KLocale>
#include <KDialog>
#include <KPushButton>

#include "kregexpeditorgui.h"

int main( int argc, char* argv[] )
{
    KAboutData aboutData( "kregexpeditor", 0, ki18n("RegExp Editor"),
                          "1.0", ki18n("Editor for Regular Expressions"),
			  KAboutData::License_GPL,
                          ki18n("(c) 2002-2003 Jesper K. Pedersen"));
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication myapp;

    KDialog* top = new KDialog( 0 );
    top->setButtons(KDialog::Help | KDialog::Close);

    KRegExpEditorGUI* iface = new KRegExpEditorGUI( top );
    iface->doSomething( QString::fromLatin1("setAllowNonQtSyntax"), (bool*) true );
    top->setMainWidget(iface);

    QObject::connect( top, SIGNAL( helpClicked() ), iface, SLOT( showHelp() ) );

    top->show();
    QObject::connect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );
    myapp.exec();
}

