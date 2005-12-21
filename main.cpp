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

#ifdef QT_ONLY
  #include "compat.h"
  #include <qapplication.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#else
  #include <kaboutdata.h>
  #include <kapplication.h>
  #include <kcmdlineargs.h>
  #include <klocale.h>
  #include <kpushbutton.h>
#endif

#include "kregexpeditorgui.h"
#include <qlayout.h>

int main( int argc, char* argv[] )
{
#ifdef QT_ONLY
    QApplication myapp( argc, argv );
#else
    KAboutData aboutData( "kregexpeditor", I18N_NOOP("RegExp Editor"),
                          "1.0", I18N_NOOP("Editor for Regular Expressions"),
			  KAboutData::License_GPL,
                          "(c) 2002-2003 Jesper K. Pedersen");
    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication myapp;
#endif

    QDialog* top = new QDialog( 0 );
    QVBoxLayout* lay = new QVBoxLayout( top, 6 );

    KRegExpEditorGUI* iface = new KRegExpEditorGUI( top, "_editor", QStringList() );
    iface->doSomething( QString::fromLatin1("setAllowNonQtSyntax"), (bool*) true );
    lay->addWidget( iface );

    QHBoxLayout* lay2 = new QHBoxLayout( lay, 6 );
    KPushButton* help = new KPushButton( KStdGuiItem::help(), top );
    KPushButton* quit = new KPushButton( KStdGuiItem::quit(), top );

    lay2->addWidget( help );
    lay2->addStretch(1);
    lay2->addWidget( quit );

    QObject::connect( help, SIGNAL( clicked() ), iface, SLOT( showHelp() ) );
    QObject::connect( quit, SIGNAL( clicked() ), qApp, SLOT( quit() ) );

    top->show();
    QObject::connect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );
    myapp.exec();
}

