#include "lookaheadregexp.h"
#include "errormap.h"

LookAheadRegExp::LookAheadRegExp( bool selected, TYPE tp, RegExp* child )
    : RegExp( selected ), _child( child ), _tp( tp )
{
    if ( child )
        addChild( child );
}

bool LookAheadRegExp::check( ErrorMap& map, bool , bool last )
{
    if ( !last ) {
        map.lookAheadError();
    }
    return true;
}

QString LookAheadRegExp::toString( bool markSelection ) const
{
    if ( _syntax == Qt ) {
        if ( _tp == POSITIVE )
            return QString::fromLatin1( "(?=" ) + _child->toString( markSelection ) + QString::fromLocal8Bit( ")" );
        else
            return QString::fromLatin1( "(?!" ) + _child->toString( markSelection ) + QString::fromLocal8Bit( ")" );
    }
    else {
        // PENDING(blackie) enhance error handling
        qWarning( "Look ahead not supported");
        return QString::null;
    }
}


QDomNode LookAheadRegExp::toXml( QDomDocument* doc ) const
{
    QDomElement top;
    if ( _tp == POSITIVE )
        top = doc->createElement( QString::fromLocal8Bit("PositiveLookAhead") );
    else
        top = doc->createElement( QString::fromLocal8Bit("NegativeLookAhead") );

    top.appendChild( _child->toXml( doc ) );
    return top;
}

bool LookAheadRegExp::load( QDomElement top, const QString& version )
{
    _child = readRegExp( top, version );
    if ( _child ) {
        addChild( _child );
        return true;
    }
    else
        return false;
}


bool LookAheadRegExp::operator==( const RegExp& other ) const
{
    if ( type() != other.type() )
        return false;

    const LookAheadRegExp& theOther = dynamic_cast<const LookAheadRegExp&>( other );

    if ( lookAheadType() != theOther.lookAheadType() )
        return false;

    return ( *_child == *(theOther._child) );
}
