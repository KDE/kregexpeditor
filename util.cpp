#include "util.h"
#include <kiconloader.h>
#include <kglobal.h>
#include <kstandarddirs.h>
QPixmap Util::getKRegExpEditorIcon( const QString& name )
{
#ifdef QT_ONLY
    QPixmap pix;
    pix.convertFromImage( qembed_findImage(name) );
    return pix;
#else
  return KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/") +name ),
                                         KIcon::Toolbar );
#endif
}

QPixmap Util::getSystemIcon( const QString& name )
{
#ifdef QT_ONLY
    QPixmap pix;
    pix.convertFromImage( qembed_findImage( name ) );
    return pix;
#else
  KIconLoader loader;
  return loader.loadIcon( name, KIcon::Toolbar);
#endif

}
