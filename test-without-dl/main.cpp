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
  QFile file("/packages/kde-src/kdeutils/kregexpeditor/test/main.cpp");
  file.open(IO_ReadOnly);
  QTextStream stream( &file);
  QString txt = stream.read();
  iface->setMatchText( txt );
  
  iface->exec();
}

