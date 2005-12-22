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
#include "positionregexp.h"
#include "errormap.h"

PositionRegExp::PositionRegExp( bool selected, PositionType tp ) :RegExp( selected )
{
	_tp = tp;
}

bool PositionRegExp::check( ErrorMap& map, bool first , bool last )
{
    if ( _tp == BEGLINE && !first) {
        map.lineStartError();
    }
    else if ( _tp == ENDLINE && !last ) {
        map.lineEndError();
    }
    return true;
}

QDomNode PositionRegExp::toXml( QDomDocument* doc ) const
{
    switch (_tp) {
    case BEGLINE: return doc->createElement(QString::fromLocal8Bit( "BegLine" ) );
    case ENDLINE: return doc->createElement(QString::fromLocal8Bit( "EndLine" ) );
    case WORDBOUNDARY: return doc->createElement(QString::fromLocal8Bit( "WordBoundary" ) );
    case NONWORDBOUNDARY: return doc->createElement(QString::fromLocal8Bit( "NonWordBoundary" ) );
    }
    return QDomNode();
}

bool PositionRegExp::load( QDomElement /* top */, const QString& /*version*/ )
{
    // Nothing to do.
    return true;
}

