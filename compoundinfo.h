#ifndef compoundinfo_h
#define compoundinfo_h

#include "triple.h"
#include <qmap.h>

typedef Triple<QString,QString,bool> StringTriple;
class CompoundInfo
{
public:
  void clear();
  void add( const QString& regexp, const QString& title, const QString& description, bool hidden );
  bool lookUp( const QString& regexp, int index, QString* title, QString* description, bool* hidden );
private:
  QMap<QString,StringTriple> _regexpMap;
  QMap<int,StringTriple> _indexMap;
  int _count;
};


#endif
