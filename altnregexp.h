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
#ifndef __ALTNREGEXP_H
#define __ALTNREGEXP_H

#include "regexp.h"

/**
   Abstract syntax node for `alternative' regular expression
   @internal
*/
class AltnRegExp :public RegExp
{
public:
	AltnRegExp( bool selected );

	void addRegExp( RegExp * );
    RegExpList children() const;

    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 1;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    virtual RegExpType type() const { return ALTN;}
    virtual bool operator==( RegExp& other );
    virtual void replacePart( CompoundRegExp* replacement );

private:
	RegExpList list;
};

#endif // __ALTNREGEXP_H
