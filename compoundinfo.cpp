#include "compoundinfo.h"

void CompoundInfo::clear()
{
  _regexpMap.clear();
  _indexMap.clear();
  _count = 0;
}

void CompoundInfo::add( const QString& regexp, const QString& title, const QString& description, bool hidden )
{
  _regexpMap.insert( regexp, StringTriple( title, description, hidden ) );
  _indexMap.insert( ++_count, StringTriple( title, description, hidden ) );
}

bool CompoundInfo::lookUp( const QString& regexp, int index, QString* title, QString* description, bool* hidden ) 
{
  QMap<QString, StringTriple >::Iterator it = _regexpMap.find( regexp );
  if ( it == _regexpMap.end() ) {
    QMap<int, StringTriple>::Iterator it = _indexMap.find( index );
    if ( it == _indexMap.end() ) 
      return false;
    else {
      *title = it.data().first();
      *description = it.data().second();
      *hidden = it.data().third();      
      return true;
    }
  }
  else {
    *title = it.data().first();
    *description = it.data().second();
    *hidden = it.data().third();
    return true;
  }
}
