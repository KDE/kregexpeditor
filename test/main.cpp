#include <kapp.h>
#include <kcmdlineargs.h>
#include <kregexpeditorinterface.h>
#include <kparts/componentfactory.h>
#include <qdialog.h>

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

  int error = 0;
  QDialog* m_editorDialog = KParts::ComponentFactory::createInstanceFromQuery<QDialog>( "KRegExpEditor/KRegExpEditor", QString::null, 0, 0, QStringList(), &error );
  switch ( error )
  {
      case KParts::ComponentFactory::ErrNoServiceFound:
	  qDebug( "cannot find KRegExpEditor service." ); return 1;
      case KParts::ComponentFactory::ErrServiceProvidesNoLibrary:
	  qDebug( "service provides no library field." ); return 1;
      case KParts::ComponentFactory::ErrNoFactory:
	  qDebug( "shared library does not provide a factory." ); return 1;
      case KParts::ComponentFactory::ErrNoComponent:
	  qDebug( "factory does not provide a QDialog component." ); return 1;
      default: break;
  };

  Q_ASSERT( m_editorDialog );
  
  KRegExpEditorInterface *iface = dynamic_cast<KRegExpEditorInterface *>( m_editorDialog );

  Q_ASSERT( iface );

  iface->setRegExp( QString::fromLatin1( ".*" ) );
  
  m_editorDialog->exec();
  delete m_editorDialog;
}

