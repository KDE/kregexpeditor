#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kregexpeditorinterface.h>
#include <qdialog.h>
#include <qfile.h>
#include <qtextstream.h>
#include "../kregexpeditorgui.h"
int main( int argc, char* argv[] )
{
  KCmdLineArgs::init(argc, argv, "RegExp Example","","");
  KApplication myapp( argc, argv );

  KRegExpEditorGUIDialog* iface = new KRegExpEditorGUIDialog( 0, "_editor", QStringList() );
  iface->doSomething( QString::fromLatin1("setShowSyntaxCombo"), (bool*) true );

  iface->exec();
}

