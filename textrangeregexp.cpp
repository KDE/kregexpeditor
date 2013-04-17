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

#include "textrangeregexp.h"

#include <KLocale>
#include <KMessageBox>

#include "regexpconverter.h"

TextRangeRegExp::TextRangeRegExp( bool selected ) : RegExp( selected ),
    _negate(false), _digit(false), _nonDigit(false), _space(false), _nonSpace(false), _wordChar(false), _nonWordChar(false)
{
}

TextRangeRegExp::~TextRangeRegExp()
{
}


void TextRangeRegExp::addCharacter( QString str )
{
	_chars.append( str );
}

void TextRangeRegExp::addRange(QString from, QString to)
{
	_ranges.append( StringPair( from, to ) );
}

bool TextRangeRegExp::check( ErrorMap&, bool, bool )
{
    return false;
}

QDomNode TextRangeRegExp::toXml( QDomDocument* doc ) const
{
    QDomElement top = doc->createElement( QString::fromLocal8Bit( "TextRange" ) );

    if ( _negate ) {
        top.setAttribute( QString::fromLocal8Bit("negate"), true );
    }

    if ( _digit ) {
        top.setAttribute( QString::fromLocal8Bit("digit"), true );
    }

    if ( _nonDigit ) {
        top.setAttribute( QString::fromLocal8Bit("nonDigit"), true );
    }

    if ( _space ) {
        top.setAttribute( QString::fromLocal8Bit("space"), true );
    }

    if ( _nonSpace ) {
        top.setAttribute( QString::fromLocal8Bit("nonSpace"), true );
    }

    if ( _wordChar ) {
        top.setAttribute( QString::fromLocal8Bit("wordChar"), true );
    }

    if ( _nonWordChar ) {
        top.setAttribute( QString::fromLocal8Bit("nonWordChar"), true );
    }

    for ( QStringList::ConstIterator it = _chars.begin(); it != _chars.end(); ++it ) {
        QDomElement elm = doc->createElement( QString::fromLocal8Bit( "Character" ) );
        elm.setAttribute( QString::fromLocal8Bit( "char" ), *it );
        top.appendChild( elm );
    }

    foreach ( const StringPair & pair, _ranges ) {
        QDomElement elm = doc->createElement( QString::fromLocal8Bit( "Range" ) );
        elm.setAttribute( QString::fromLocal8Bit( "from" ), pair.first );
        elm.setAttribute( QString::fromLocal8Bit( "to" ), pair.second );
        top.appendChild( elm );
    }
    return top;
}

bool TextRangeRegExp::load( QDomElement top, const QString& /*version*/ )
{
    Q_ASSERT( top.tagName() == QString::fromLocal8Bit( "TextRange" ) );
    QString str;
    QString one = QString::fromLocal8Bit("1");
    QString zero = QString::fromLocal8Bit("0");

    str = top.attribute( QString::fromLocal8Bit("negate"), zero );
    _negate = ( str == one );

    str = top.attribute( QString::fromLocal8Bit("digit"), zero );
    _digit = ( str == one );

    str = top.attribute( QString::fromLocal8Bit("nonDigit"), zero );
    _nonDigit = ( str == one );

    str = top.attribute( QString::fromLocal8Bit("space"), zero );
    _space = ( str == one );

    str = top.attribute( QString::fromLocal8Bit("nonSpace"), zero );
    _nonSpace = ( str == one );

    str = top.attribute( QString::fromLocal8Bit("wordChar"), zero );
    _wordChar = ( str == one );

    str = top.attribute( QString::fromLocal8Bit("nonWordChar"), zero );
    _nonWordChar = ( str == one );

    for ( QDomNode node = top.firstChild(); !node.isNull(); node = node.nextSibling() ) {
        if ( !node.isElement() ) {
            continue; // Skip comments.
	}
        QDomElement child = node.toElement();

        if ( child.tagName() == QString::fromLocal8Bit( "Character" ) ) {
            QString ch = child.attribute( QString::fromLocal8Bit( "char" ) );
            addCharacter( ch );
        } else if ( child.tagName() == QString::fromLocal8Bit( "Range" ) ) {
            QString from = child.attribute( QString::fromLocal8Bit( "from" ) );
            QString to = child.attribute( QString::fromLocal8Bit( "to" ) );
            addRange( from, to );
        } else {
            KMessageBox::sorry( 0, i18n("<p>Invalid sub element to element <b>TextRange</b>. Tag was <b>%1</b></p>", child.tagName()),
                                i18n("Error While Loading From XML File") ) ;
            return false;
        }
    }
    return true;
}

bool TextRangeRegExp::operator==( const RegExp& other ) const
{
    return ( RegExpConverter::current()->toStr( const_cast<TextRangeRegExp*>( this ), false ) ==
             RegExpConverter::current()->toStr( const_cast<RegExp*>( &other ), false ) );
}
