#ifndef UTIL_H
#define UTIL_H
#include <qpixmap.h>
#include <qstring.h>

class Util
{
public:
    static QPixmap getKRegExpEditorIcon( const QString& name );
    static QPixmap getSystemIcon( const QString& name );
};

#endif /* UTIL_H */

