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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kregexpeditorinterface.h>
#include <qdialog.h>
#include <qfile.h>
#include <qtextstream.h>
#include "../kregexpeditorgui.h"
class ShootABug :public QObject
{
public:
  virtual bool eventFilter( QObject* recv, QEvent* event )
  {
    if ( event->type() == QEvent::MouseButtonPress &&
         dynamic_cast<QMouseEvent*>(event)->state() == Qt::ControlButton ) {
      // Ctrl + left mouse click.

      qDebug("----------------------------------------------------");
      qDebug((QString("Widget name : ") + QString( recv->name() )).latin1() );
      qDebug((QString("Widget class: ") + QString( recv->className() )).latin1() );
      qDebug("\nObject info:");
      recv->dumpObjectInfo();
      qDebug("\nObject tree:");
      recv->dumpObjectTree();
      qDebug("----------------------------------------------------");
      return false;
    }
    return false;
  }
};

int main( int argc, char* argv[] )
{
  KCmdLineArgs::init(argc, argv, "RegExp Example","","");
  KApplication myapp( argc, argv );

  qApp->installEventFilter( new ShootABug() );

  KRegExpEditorGUIDialog* iface = new KRegExpEditorGUIDialog( 0, "_editor", QStringList() );
  iface->setRegExp( QString::fromLatin1( "#include" ) );
  iface->doSomething( "setMinimal", (void*) false );
  iface->doSomething( "setSyntax", (void*) new QString( QString::fromLatin1( "Emacs" ) ) );
  iface->doSomething( "setShowSyntaxCombo", (bool*) true );

  QFile file("/packages/kde-src/kdeutils/kregexpeditor/test/main.cpp");
  file.open(IO_ReadOnly);
  QTextStream stream( &file);
  QString txt = stream.read();
  iface->setMatchText( txt );

  iface->exec();
}

