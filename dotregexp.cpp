#include "dotregexp.h"

DotRegExp::DotRegExp()
{
}

QString DotRegExp::toString() const
{
	return QString::fromLatin1(".");
}

QDomNode DotRegExp::toXml( QDomDocument* doc ) const
{
  return doc->createElement( QString::fromLocal8Bit("AnyChar") );
}

bool DotRegExp::load( QDomElement top, const QString& /*version*/ ) 
{
  // Nothing to do
  return true;
}

