#include "positionregexp.h"
#include "errormap.h"

PositionRegExp::PositionRegExp( bool selected, PositionType tp ) :RegExp( selected )
{
	_tp = tp;
}

bool PositionRegExp::check( ErrorMap& map, bool first , bool last )
{
    if ( _tp == BEGLINE && !first) {
        map.lineStartError();
    }
    else if ( _tp == ENDLINE && !last ) {
        map.lineEndError();
    }
    return true;
}

QString PositionRegExp::toString( bool ) const
{
	switch (_tp) {
	case BEGLINE:
		return QString::fromLatin1("^");
	case ENDLINE:
		return QString::fromLatin1("$");
	case WORDBOUNDARY:
    {
        if ( _syntax == Qt )
            return QString::fromLatin1("\\b");
        else {
            // PENDING(blackie) enhance error handling
            qWarning("Word boundary is not supported in Emacs syntax");
            return QString::null;
        }
    }
	case NONWORDBOUNDARY:
    {
        if ( _syntax == Qt )
            return QString::fromLatin1("\\B");
        else {
            // PENDING(blackie) enhance error handling
            qWarning("Non Word boundary is not supported in Emacs syntax");
            return QString::null;
        }
	}
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

bool PositionRegExp::load( QDomElement /* top */, const QString& /*version*/ )
{
    // Nothing to do.
    return true;
}

