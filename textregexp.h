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
#ifndef __textregexp_h
#define __textregexp_h

#include "regexp.h"

/**
   Abstract syntax node for `text' regular expression
   @internal
*/
class TextRegExp :public RegExp
{
public:
	TextRegExp( bool selected, QString text = QString::null);

	virtual QString toString( bool markSelection ) const;
    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const {
        if ( _text.length() > 1 )
            return 2;
        else
            return 4;
    }
    QString text() const { return _text; }
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    void append( QString str);
    virtual RegExpType type() const { return TEXT;}
    virtual bool operator==( const RegExp& other ) const;


protected:
	QString escape( QString text, QValueList<QChar> chars, QChar escapeChar) const;

private:
	QString _text;
};

#endif // __textregexp_h
