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
#ifndef __POSITIONREGEXP_H
#define __POSITIONREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `positions' regular expression
   @internal
*/
class PositionRegExp :public RegExp
{
public:
	enum PositionType { BEGLINE, ENDLINE, WORDBOUNDARY, NONWORDBOUNDARY };

	PositionRegExp( bool selected, PositionType tp );
    PositionType position() const { return _tp; }

    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 4;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    virtual RegExpType type() const { return POSITION ;}

private:
	PositionType _tp;

};

#endif // __POSITIONREGEXP_H
