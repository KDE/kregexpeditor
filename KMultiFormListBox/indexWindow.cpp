#include "indexWindow.h"
#include <iostream.h>
#include <qlayout.h>
#include <qapplication.h>

indexWindow::indexWindow() : QWidget(0,"", WStyle_Customize | WType_Popup)
{
  lb = new QListBox(this);
  connect(lb,SIGNAL(selected(int)), this, SLOT(lbSelected(int)));
  QHBoxLayout *lay = new QHBoxLayout(this);
  lay->addWidget(lb);
  lbFinish = false;
}

void indexWindow::lbSelected(int index)
{
  lbFinish = true;
  hide();
  lbFinish = false;
  finish(index);
}

void indexWindow::finish(int index)
{
  itemSelected = index;
  qApp->exit_loop();
}


void indexWindow::insertItem(QString txt)
{
  lb->insertItem(txt);
}

void indexWindow::hideEvent(QHideEvent *h)
{
  QWidget::hideEvent(h);
  if (!lbFinish)
    finish(-1);
}


int indexWindow::exec(const QPoint &start, int width) 
{
  // calculate the height of all the elements together.
  // I need to do it this way, as sizeHint doesn't report the correct size
  // and itemHeight doesn't neither.
  int elm_h = lb->item(0)->height(lb) * lb->count();
  elm_h += 2*lb->frameWidth();
  
  QWidget *desktop = QApplication::desktop();
  int desktop_h = desktop->height();
  int rest_h = desktop_h - start.y();
  int below_h = QMAX(rest_h, 200);

  int start_y = start.y();

  if (rest_h < 200 && elm_h > 200) {
    start_y = desktop_h-QMIN(elm_h,200);
  }
  
  setGeometry(start.x(), start_y, width, QMIN(elm_h, below_h));
  show();
  qApp->enter_loop();
  return itemSelected;
}

#include "indexWindow.moc"
