#include "altnregexp.h"
#include "widgetfactory.h"

AltnRegExp::AltnRegExp( bool selected ) :RegExp( selected )
{
    // Nothing to do
}

void AltnRegExp::addRegExp( RegExp *elm )
{
	list.append( elm );
    addChild( elm );
}

RegExpList AltnRegExp::children()
{
    return list;
}

bool AltnRegExp::check( ErrorMap& map, bool first, bool last )
{
    bool possibleEmpty = false;
    for ( RegExpListIt it(list); *it; ++it ) {
        possibleEmpty = (*it)->check( map, first, last ) || possibleEmpty;
    }
    return possibleEmpty;
}

QString AltnRegExp::toString( bool markSelection ) const
{
	QString res;

    QString bar;
    if ( _syntax == Qt )
        bar = QString::fromLatin1("|");
    else if ( _syntax == Emacs )
        bar = QString::fromLatin1("\\|");
    else
        qFatal("WHAT!");

	bool first = true;
    for ( RegExpListIt it(list); *it; ++it ) {
		if ( !first ) {
			res += bar;
		}
		first = false;
        if ( markSelection && !isSelected() && (*it)->isSelected() ) {
            res += QString::fromLatin1("(") + (*it)->toString( markSelection ) + QString::fromLatin1(")");
        }
        else {
            res += (*it)->toString( markSelection );
        }
	}
	return res;
}

QDomNode AltnRegExp::toXml( QDomDocument* doc ) const
{
    QDomElement top = doc->createElement( QString::fromLocal8Bit( "Alternatives" ) );
    for ( RegExpListIt it(list); *it; ++it ) {
        top.appendChild( (*it)->toXml( doc ) );
    }
    return top;
}

bool AltnRegExp::load( QDomElement top, const QString& version )
{
    Q_ASSERT( top.tagName() == QString::fromLocal8Bit( "Alternatives" ) );

    for ( QDomNode child = top.firstChild(); !child.isNull(); child = child.nextSibling() ) {
        if ( ! child.isElement() )
            continue; // User might have added a comment.

        RegExp* regexp = WidgetFactory::createRegExp( child.toElement(), version );
        if ( regexp == 0 )
            return false;
        addRegExp( regexp );
    }
    return true;

}

bool AltnRegExp::operator==( const RegExp& other ) const
{
    // TODO: merge with ConcRegExp::operator==

    if ( other.type() != type() )
        return false;

    const AltnRegExp& theOther = dynamic_cast<const AltnRegExp&>( other );

    if ( list.count() != theOther.list.count() )
        return false;

    RegExpListIt it1( list );
    RegExpListIt it2( theOther.list );

    for ( ; *it1 && *it2 ; ) {
        if ( ! (**it1 == **it2) )
            return false;
        ++it1;
        ++it2;
    }
    return true;
}

void AltnRegExp::replacePart( CompoundRegExp* replacement )
{
    for ( RegExpListIt it( list ); *it; ++it ) {
        (*it)->replacePart( replacement );
    }
}

