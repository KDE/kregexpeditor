#include "windowlistboxitem.h"
#include "widgetwindow.h"

WindowListboxItem::WindowListboxItem(QListBox *lb, QString text, WidgetWindow *windowWidget) 
  :QListBoxText(lb, text), myItem(windowWidget)
{
}

WindowListboxItem::~WindowListboxItem()
{
  delete myItem;
}

void WindowListboxItem::displayWidget() 
{
  myItem->display();
}

void WindowListboxItem::clone() 
{
  myItem->clone();
}

KMultiFormListBoxEntry *WindowListboxItem::entry()
{
  return myItem->entry();
}

