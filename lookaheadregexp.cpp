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


void LookAheadRegExp::updateCI( CompoundInfo* ci )
{
  _child->updateCI( ci );
}

