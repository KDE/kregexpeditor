#include "altnregexp.h"
#include "widgetfactory.h"

AltnRegExp::AltnRegExp()
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

QString AltnRegExp::toString() const
{
	QString res;
	bool first = true;
  for ( RegExpListIt it(list); *it; ++it ) {
		if ( !first ) {
			res += QString::fromLatin1("|");
		}
		first = false;
		res += (*it)->toString();
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
  ASSERT( top.tagName() == QString::fromLocal8Bit( "Alternatives" ) );
  
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

void AltnRegExp::updateCI( CompoundInfo* ci )
{
  for ( RegExpListIt it(list); *it; ++it ) {
    (*it)->updateCI( ci );
  }
}

