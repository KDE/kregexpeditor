/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "indexWindow.h"

#include <QHBoxLayout>
#include <QListWidget>

indexWindow::indexWindow()
    : QWidget(nullptr, Qt::Popup)
{
    lb = new QListWidget(this);
    connect(lb, SIGNAL(selected(int)), this, SLOT(lbSelected(int)));
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
    // qApp->exit_loop();
}

void indexWindow::insertItem(const QString &txt)
{
    lb->addItem(txt);
}

void indexWindow::hideEvent(QHideEvent *h)
{
    QWidget::hideEvent(h);
    if (!lbFinish) {
        finish(-1);
    }
}

int indexWindow::exec(const QPoint & /*start*/, int /*width*/)
{
    // This code is not xinerama safe, on the other hand this part of the widget
    // is not used in KRegExpEditor. This is part of a widget which has
    // never been completed, but which is used partly by KRegExpEditor.
    // 23 Feb. 2003 11:28 -- Jesper K. Pedersen
    /*
        // calculate the height of all the elements together.
      // I need to do it this way, as sizeHint doesn't report the correct size
      // and itemHeight doesn't neither.
      int elm_h = lb->item(0)->height(lb) * lb->count();
      elm_h += 2*lb->frameWidth();

      QWidget *desktop = QApplication::desktop();
      int desktop_h = desktop->height();
      int rest_h = desktop_h - start.y();
      int below_h = qMax(rest_h, 200);

      int start_y = start.y();

      if (rest_h < 200 && elm_h > 200) {
        start_y = desktop_h-qMin(elm_h,200);
      }

      setGeometry(start.x(), start_y, width, qMin(elm_h, below_h));
      show();
      qApp->enter_loop();
      return itemSelected;
    */
    return 0;
}

#include "moc_indexWindow.cpp"
