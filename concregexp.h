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
#ifndef __CONCREGEXP_H
#define __CONCREGEXP_H

#include "regexp.h"


/**
   Abstract syntax node for `concatenated' regular expression
   @internal
*/
class ConcRegExp :public RegExp
{
public:
	ConcRegExp( bool selected );

	void addRegExp( RegExp *);
    RegExpList children();
    RegExp* lastRegExp();

	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 2;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    virtual RegExpType type() const {return CONC; }
    virtual bool operator==( const RegExp& other ) const;
    virtual void replacePart( CompoundRegExp* replacement );

private:
	RegExpList list;
};

#endif // __CONCREGEXP_H
