#include "myfontmetrics.h"

QSize HackCalculateFontSize(QFontMetrics fm, QString str ) 
{
  QStringList list = QStringList::split( QString::fromLatin1("\n"), str );
  int maxWidth = 0;
  int height = 0;
  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
    QSize size = fm.size( 0, *it );
    maxWidth = QMAX( maxWidth, size.width() );
    height += size.height();
  }
  return QSize( maxWidth, height );
}
