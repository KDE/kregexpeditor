#include "concregexp.h"
#include "widgetfactory.h"
#include "compoundregexp.h"

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

bool ConcRegExp::operator==( const RegExp& other ) const
{
  // TODO: Merge with AltnRegExp::operator==
  if ( list.count() == 1 )
    return ( other == *(const_cast< QPtrList<RegExp>& >(list).at(0)) );
  
  if ( other.type() != type() ) 
    return false;
  
  const ConcRegExp& theOther = dynamic_cast<const ConcRegExp&>( other );
  
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

      
void ConcRegExp::replacePart( CompoundRegExp* replacement )
{
  RegExp* otherChild = replacement->child();
  ConcRegExp* otherConc = dynamic_cast<ConcRegExp*>( otherChild );
  if ( ! otherConc ) {
    // Watch out for garbage here!
    otherConc = new ConcRegExp();
    otherConc->addRegExp( otherChild );
  }

  RegExpList newList;
  RegExpListIt it1( list );
  while ( *it1 ) {
    (*it1)->replacePart( replacement );
    RegExpListIt it2 = it1;
    RegExpListIt it3( otherConc->list );
    bool match = true;
    int count = 0;
    
    // See if replacement is a sublist of list starting from what it1 points at
    for ( ; *it2 && * it3 && match ; ) {
      if (! ( **it2 == **it3 ) )
        match = false;
      ++it2;
      ++it3;
      ++count;
    }

    if ( match && ! *it3) {
      // I found a match, delete the element in it1, which is common with it3
      while ( *it1 != *it2 ) {
        RegExp* item = *it1;
        ++it1;
        delete item;
      }
      RegExp* theClone = replacement->clone();
      newList.append( theClone );
      addChild( theClone );
    }
    else {
      newList.append( *it1 );
      ++it1;
    }
  }
  list = newList;
}


