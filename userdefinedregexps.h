#ifndef __USERDEFINEDREGEXPS_H
#define __USERDEFINEDREGEXPS_H
#include <qdockwindow.h>
#include <qlistview.h>
class QPoint;
class RegExp;

class UserDefinedRegExps :public QDockWindow
{
Q_OBJECT

public:
  UserDefinedRegExps( QWidget *parent, const char *name = 0 );

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
};

class WidgetWinItem :public QListViewItem 
{
public:
  WidgetWinItem( QString name, QListView* parent );
  QString fileName() const;
  QString name() const;
  void setName( const QString& );
  static QString path();
  
private:
  QString _name;
};


#endif // __USERDEFINEDREGEXPS_H
