#ifndef __CHARSELECTOR_H
#define __CHARSELECTOR_H
#include <qwidget.h>
#include <qlayout.h>

class QComboBox;
class QWidgetStack;
class LimitedCharLineEdit;

class CharSelector :public QWidget 
{
Q_OBJECT

public:
  CharSelector( QWidget* parent, const char* name = 0 );
  QString text() const;
  void setText( QString text );
  bool isEmpty() const;
  QString regexpStr() const;

private slots:
  void slotNewItem( int which );

private:
  QComboBox* _type;
  QWidgetStack* _stack;
  LimitedCharLineEdit* _normal;
  LimitedCharLineEdit* _hex;
  LimitedCharLineEdit* _oct;
  int _oldIndex;
};

#endif // __CHARSELECTOR_H
