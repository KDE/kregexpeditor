#ifndef __windowlistboxitem
#define __windowlistboxitem
#include <qlistbox.h>
#include <qstring.h>
class WidgetWindow;
class KMultiFormListBoxWindowed;
class KMultiFormListBoxEntry;


/**
   @internal
*/
class WindowListboxItem :public QListBoxText 
{
  friend WidgetWindow;
  friend KMultiFormListBoxWindowed;
public:
  WindowListboxItem(QListBox *, QString, WidgetWindow *);
  ~WindowListboxItem();
  void clone();
  void displayWidget();
  KMultiFormListBoxEntry *entry();

private:
  WidgetWindow *myItem;
};

#endif // __windowlistboxitem
