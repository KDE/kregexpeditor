#include "positionregexp.h"

PositionRegExp::PositionRegExp( PositionType tp )
{
	_tp = tp;
}

QString PositionRegExp::toString() const
{
	switch (_tp) {
	case BEGLINE:
		return QString::fromLatin1("^");
	case ENDLINE:
		return QString::fromLatin1("$");
	case WORDBOUNDARY:
		return QString::fromLatin1("\\b");
	case NONWORDBOUNDARY:
		return QString::fromLatin1("\\B");
	}
	Q_ASSERT( false );
	return QString::fromLatin1("");
}

QDomNode PositionRegExp::toXml( QDomDocument* doc ) const
{
  switch (_tp) {
  case BEGLINE: return doc->createElement(QString::fromLocal8Bit( "BegLine" ) );
  case ENDLINE: return doc->createElement(QString::fromLocal8Bit( "EndLine" ) );
  case WORDBOUNDARY: return doc->createElement(QString::fromLocal8Bit( "WordBoundary" ) );
  case NONWORDBOUNDARY: return doc->createElement(QString::fromLocal8Bit( "NonWordBoundary" ) );
  }
  return QDomNode();
}

bool PositionRegExp::load( QDomElement top, const QString& /*version*/ ) 
{
  // Nothing to do.
  return true;
}

