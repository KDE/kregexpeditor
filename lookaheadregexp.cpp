#include "lookaheadregexp.h"

LookAheadRegExp::LookAheadRegExp( TYPE tp, RegExp* child )
  : _child( child ), _tp( tp )
{
  if ( child )
    addChild( child );
}

QString LookAheadRegExp::toString() const
{
  if ( _tp == POSITIVE )
    return QString::fromLatin1( "(?=" ) + _child->toString() + QString::fromLocal8Bit( ")" );
  else
    return QString::fromLatin1( "(?!" ) + _child->toString() + QString::fromLocal8Bit( ")" );
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
