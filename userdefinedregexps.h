#ifndef __USERDEFINEDREGEXPS_H
#define __USERDEFINEDREGEXPS_H
#include <qdockwindow.h>
#include <qlistview.h>
#include <qptrlist.h>
class QPoint;
class RegExp;
class CompoundRegExp;

class UserDefinedRegExps :public QDockWindow
{
Q_OBJECT

public:
  UserDefinedRegExps( QWidget *parent, const char *name = 0 );
  const QPtrList<CompoundRegExp> regExps() const;

public slots:
  void slotSelectNewAction();
  
protected slots:
  void slotLoad(QListViewItem* item);
  void slotEdit( QListViewItem* item, const QPoint& pos );
  void slotPopulateUserRegexps();
  void slotUnSelect();

signals:
  void load( RegExp* );
  
private:
  QListView* _userDefined;
  QPtrList<CompoundRegExp> _regExps;
};

class WidgetWinItem :public QListViewItem 
{
public:
  WidgetWinItem( QString name, RegExp* regexp, QListView* parent );
  QString fileName() const;
  RegExp* regExp() const;
  QString name() const;
  void setName( const QString& );
  static QString path();
  
private:
  QString _name;
  RegExp* _regexp;
};


#endif // __USERDEFINEDREGEXPS_H
