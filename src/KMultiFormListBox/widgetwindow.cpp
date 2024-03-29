/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "widgetwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <QListWidget>

#include <QDialogButtonBox>

#include "kmultiformlistboxentry.h"
#include "kmultiformlistboxfactory.h"
#include "windowlistboxitem.h"

WidgetWindow::WidgetWindow(KMultiFormListBoxFactory *factory, QListWidget *lb)
    : QDialog(nullptr)
{
    init(factory, lb);
}

WidgetWindow::WidgetWindow(KMultiFormListBoxFactory *factory, KMultiFormListBoxEntry *widget, QListWidget *lb)
    : QDialog(nullptr)
{
    init(factory, lb, widget);
}

void WidgetWindow::init(KMultiFormListBoxFactory *factory, QListWidget *lb, KMultiFormListBoxEntry *widget)
{
    setWindowTitle(i18n("Widget Configuration"));
    auto mainLayout = new QVBoxLayout(this);

    listbox = lb;
    myFact = factory;

    QFrame *frame = new QFrame(this);
    QHBoxLayout *lay = new QHBoxLayout;
    frame->setLayout(lay);
    lay->setObjectName(QStringLiteral("WidgetWindow::init::lay"));
    lay->setSpacing(-1);
    lay->setContentsMargins(0, 0, 0, 0);

    if (widget != nullptr) {
        myWidget = widget;
        widget->setParent(frame);
    } else {
        myWidget = factory->create(frame);
    }
    QDataStream stream(&_backup, QIODeviceBase::WriteOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    myFact->toStream(myWidget, stream);

    lay->addWidget(myWidget);

    if (widget != nullptr) {
        initialShow = false;
        myListboxItem = new WindowListboxItem(listbox, myWidget->idxString(), this);
    } else {
        initialShow = true;
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &WidgetWindow::slotOk);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &WidgetWindow::slotCancel);
    mainLayout->addWidget(frame);
    mainLayout->addWidget(buttonBox);
}

WidgetWindow::~WidgetWindow()
{
    delete myWidget;
}

void WidgetWindow::slotOk()
{
    if (initialShow) {
        myListboxItem = new WindowListboxItem(listbox, myWidget->idxString(), this);
    } else {
        myListboxItem->setText(myWidget->idxString());
    }
    initialShow = false;
    QDialog::accept();
}

void WidgetWindow::slotCancel()
{
    if (initialShow) {
        deleteLater();
    } else {
        QDataStream stream(&_backup, QIODeviceBase::ReadOnly);

        stream.setVersion(QDataStream::Qt_3_1);
        myFact->fromStream(stream, myWidget);
    }
    QDialog::reject();
}

WidgetWindow *WidgetWindow::clone()
{
    WidgetWindow *item = new WidgetWindow(myFact, listbox);
    QByteArray data;
    QDataStream ws(&data, QIODeviceBase::WriteOnly);

    ws.setVersion(QDataStream::Qt_3_1);
    myFact->toStream(myWidget, ws);
    QDataStream rs(&data, QIODeviceBase::ReadOnly);

    rs.setVersion(QDataStream::Qt_3_1);
    myFact->fromStream(rs, item->myWidget);

    item->slotOk();
    return item;
}

void WidgetWindow::display()
{
    QDataStream stream(&_backup, QIODeviceBase::WriteOnly);

    stream.setVersion(QDataStream::Qt_3_1);
    myFact->toStream(myWidget, stream);
    show();
}

KMultiFormListBoxEntry *WidgetWindow::entry()
{
    return myWidget;
}
