#ifndef __configwindow
#define __configwindow

#include "kmultiformlistboxfactory.h"
#include "kmultiformlistbox-windowed.h"
#include "windowlistboxitem.h"
#include <kdialogbase.h>
#include <klistbox.h>

/**
   @internal
*/
class WidgetWindow :public KDialogBase
{
  friend class KMultiFormListBoxWindowed;
  friend class WindowListboxItem;
  
  WidgetWindow(KMultiFormListBoxFactory *, KListBox *);
  WidgetWindow(KMultiFormListBoxFactory *, KMultiFormListBoxEntry *widget, KListBox *);
  void init(KMultiFormListBoxFactory *, KListBox *, KMultiFormListBoxEntry *widget = 0);
  void display();
  KMultiFormListBoxEntry *entry();
  WidgetWindow *clone();
  ~WidgetWindow();

protected slots:
  void slotOk();
  void slotCancel();

private:
  KMultiFormListBoxFactory *myFact;
  KMultiFormListBoxEntry *myWidget;
  QByteArray _backup;
  KListBox *listbox;
  WindowListboxItem *myListboxItem;
  bool initialShow;
};

#endif // configwindow
