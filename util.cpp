/*
 *  Copyright (c) 2002-2004 Jesper K. Pedersen <blackie@kde.org>
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

#include "util.h"
#include <kiconloader.h>
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
