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

#include "lookaheadregexp.h"
#include "errormap.h"

LookAheadRegExp::LookAheadRegExp( bool selected, TYPE tp, RegExp* child )
    : RegExp( selected ), _child( child ), _tp( tp )
{
    if ( child ) {
        addChild( child );
    }
}

bool LookAheadRegExp::check( ErrorMap& map, bool , bool last )
{
    if ( !last ) {
        map.lookAheadError();
    }
    return true;
}


QDomNode LookAheadRegExp::toXml( QDomDocument* doc ) const
{
    QDomElement top;
    if ( _tp == POSITIVE ) {
        top = doc->createElement( QString::fromLocal8Bit("PositiveLookAhead") );
    } else {
        top = doc->createElement( QString::fromLocal8Bit("NegativeLookAhead") );
    }

    top.appendChild( _child->toXml( doc ) );
    return top;
}

bool LookAheadRegExp::load( QDomElement top, const QString& version )
{
    _child = readRegExp( top, version );
    if ( _child ) {
        addChild( _child );
        return true;
    } else {
        return false;
    }
}


bool LookAheadRegExp::operator==( const RegExp& other ) const
{
    if ( type() != other.type() ) {
        return false;
    }

    const LookAheadRegExp& theOther = dynamic_cast<const LookAheadRegExp&>( other );

    if ( lookAheadType() != theOther.lookAheadType() ) {
        return false;
    }

    return ( *_child == *(theOther._child) );
}
