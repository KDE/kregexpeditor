#include "concregexp.h"
#include "kdebug.h"
#include "widgetfactory.h"

ConcRegExp::ConcRegExp()
{
}

void ConcRegExp::addRegExp( RegExp *regExp )
{
	list.append(regExp);
  addChild( regExp );
}

RegExpList ConcRegExp::children() 
{
  return list;
}

	
QString ConcRegExp::toString() const
{
	QString res;

	for ( RegExpListIt it(list); *it; ++it ) {
    QString startPar = QString::fromLocal8Bit("");
    QString endPar = QString::fromLocal8Bit("");
    if ( (*it)->precedence() < precedence() ) {
      startPar = QString::fromLocal8Bit("(");
      endPar = QString::fromLocal8Bit(")");
    }

		res += startPar + (*it)->toString() + endPar;
	}
	return res;
}

RegExp* ConcRegExp::lastRegExp()
{
  if ( list.count() == 0)
    return 0;
  else
    return list.at( list.count()-1);
}

QDomNode ConcRegExp::toXml( QDomDocument* doc ) const
{
  QDomElement top = doc->createElement( QString::fromLocal8Bit("Concatenation") );
  for ( RegExpListIt it(list); *it; ++it ) {
    top.appendChild( (*it)->toXml( doc ) );
  }
  return top;
}

bool ConcRegExp::load( QDomElement top, const QString& version ) 
{
  Q_ASSERT( top.tagName() == QString::fromLocal8Bit( "Concatenation" ) );
  
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

void ConcRegExp::updateCI( CompoundInfo* ci )
{
	for ( RegExpListIt it(list); *it; ++it ) {
    (*it)->updateCI( ci );
  }
}
