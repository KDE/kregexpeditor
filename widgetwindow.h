#ifndef __widgetwindow
#define __widgetwindow
#include "drag.h"
#include <qmultilineedit.h>
#include <qstring.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include "dcbutton.h"
#include <qlistview.h>

class RegExpEditorWindow;
class QSignalMapper;


/**
   Widget containing button row which activates the different editing functions.
*/
class RegExpWidgetWindow :public QWidget
{
Q_OBJECT

public:
  RegExpWidgetWindow( QWidget *parent, const char *name = 0 );

protected:
  DoubleClickButton* insert(RegExpType tp, const char* file, QString tooltip, QString whatsthis);
  
public slots:
  void slotSelectNewAction();
  
protected slots:
  void slotLoad(QListViewItem* item);
  void slotEnterWhatsThis();
  void slotEditUserDefined( QListViewItem* item, const QPoint& pos );
  void slotSetKeepMode();
  void slotSetNonKeepMode();
  void slotPopulateUserRegexps();

signals:
  void clicked( int );
  void load( RegExp* );
  void doSelect();
  
private:
  QButtonGroup* _grp;
  QPushButton* _selectBut;
  QSignalMapper* _mapper; // single click Mapper.

  /**
     This variable is true if the use wishes to continue editing in the
     selected mode after the previous editing is completed (that is, if the
     user has double clicked the buttons).
  */
  bool _keepMode;
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
  
#endif /* __widgetwindow */
