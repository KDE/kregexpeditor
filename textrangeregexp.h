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
#ifndef __TEXTRANGEREGEXP_H
#define __TEXTRANGEREGEXP_H

#include "regexp.h"
#include "pair.h"

typedef Pair<QString,QString> StringPair;

/**
   Abstract syntax node for `text range' regular expression

   @internal
*/
class TextRangeRegExp :public RegExp
{
public:
	TextRangeRegExp( bool selected );
	virtual ~TextRangeRegExp();

	void addCharacter( QString ch );
    QStringList chars() const { return _chars; }
    void clearChars() { _chars.clear(); }

	void addRange( QString from, QString to );
    QPtrList<StringPair> range() const { return _ranges; }
    void clearRange() { _ranges.clear(); }

    void setNegate( bool set ) { _negate = set; }
    void setDigit( bool set ) { _digit = set; }
    void setNonDigit( bool set ) { _nonDigit = set; }
    void setSpace( bool set ) { _space = set; }
    void setNonSpace( bool set ) { _nonSpace = set; }
    void setWordChar( bool set ) { _wordChar = set; }
    void setNonWordChar( bool set ) { _nonWordChar = set; }

    bool negate() const { return _negate; }
    bool digit() const { return _digit; }
    bool nonDigit() const { return _nonDigit; }
    bool space() const { return _space; }
    bool nonSpace() const { return _nonSpace; }
    bool wordChar() const { return _wordChar; }
    bool nonWordChar() const { return _nonWordChar; }

    virtual bool check( ErrorMap&, bool first, bool last );
    virtual int precedence() const { return 4;}
    virtual QDomNode toXml( QDomDocument* doc ) const;
    virtual bool load( QDomElement, const QString& version );
    virtual RegExpType type() const { return TEXTRANGE;}
    virtual bool operator==( const RegExp& other ) const;

private:
	bool _negate, _digit, _nonDigit, _space, _nonSpace, _wordChar, _nonWordChar;
	QStringList _chars;
	QPtrList<StringPair> _ranges;
};

#endif // __TEXTRANGEREGEXP_H
