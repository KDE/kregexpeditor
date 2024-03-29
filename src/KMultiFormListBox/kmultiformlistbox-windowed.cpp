/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "kmultiformlistbox-windowed.h"

#include <QVBoxLayout>

#include <KGuiItem>
#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardGuiItem>
#include <QListWidget>
#include <QPushButton>

#include "widgetwindow.h"
#include "windowlistboxitem.h"

KMultiFormListBoxWindowed::KMultiFormListBoxWindowed(KMultiFormListBoxFactory *factory,
                                                     QWidget *parent,
                                                     bool showUpDownButtons,
                                                     bool showHelpButton,
                                                     const QString &addButtonText)
    : QWidget(parent)
{
    _layout = new QVBoxLayout(this);

    QHBoxLayout *innerLayout = new QHBoxLayout();
    _layout->addLayout(innerLayout);

    _listbox = new QListWidget(this);
    _listbox->setObjectName(QStringLiteral("listbox"));
    _listbox->setSelectionMode(QAbstractItemView::SingleSelection);
    innerLayout->addWidget(_listbox);

    QVBoxLayout *buttons = new QVBoxLayout();
    innerLayout->addLayout(buttons);

    QPushButton *but = new QPushButton(addButtonText, this);
    but->setObjectName(QStringLiteral("Add Button"));
    buttons->addWidget(but, 0);
    connect(but, &QAbstractButton::clicked, this, &KMultiFormListBoxWindowed::addNewElement);

    but = new QPushButton(i18n("Edit"), this);
    but->setObjectName(QStringLiteral("Edit Button"));
    buttons->addWidget(but, 0);
    connect(but, SIGNAL(clicked()), this, SLOT(slotEditSelected()));
    connect(_listbox, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotEditSelected(QListWidgetItem *)));
    _buttonList.append(but);

    but = new QPushButton(i18n("Delete"), this);
    but->setObjectName(QStringLiteral("Delete Button"));
    buttons->addWidget(but, 0);
    connect(but, &QAbstractButton::clicked, this, &KMultiFormListBoxWindowed::slotDeleteEntry);
    _buttonList.append(but);

    but = new QPushButton(i18n("Copy"), this);
    but->setObjectName(QStringLiteral("Copy Button"));
    buttons->addWidget(but, 0);
    connect(but, &QAbstractButton::clicked, this, &KMultiFormListBoxWindowed::slotCopySelected);
    _buttonList.append(but);

    if (showUpDownButtons) {
        but = new QPushButton(i18n("Up"), this);
        but->setObjectName(QStringLiteral("Up Button"));
        buttons->addWidget(but, 0);
        connect(but, &QAbstractButton::clicked, this, &KMultiFormListBoxWindowed::slotMoveItemUp);
        _buttonList.append(but);

        but = new QPushButton(i18n("Down"), this);
        but->setObjectName(QStringLiteral("Down Button"));
        buttons->addWidget(but, 0);
        connect(but, &QAbstractButton::clicked, this, &KMultiFormListBoxWindowed::slotMoveItemDown);
        _buttonList.append(but);
    }

    if (showHelpButton) {
        but = new QPushButton(this);
        KGuiItem::assign(but, KStandardGuiItem::help());
        but->setObjectName(QStringLiteral("Help Button"));
        buttons->addWidget(but, 0);
        connect(but, &QAbstractButton::clicked, this, &KMultiFormListBoxWindowed::showHelp);
    }

    buttons->addStretch(1);
    _factory = factory;
    slotUpdateButtonState();
}

KMultiFormListBoxEntryList KMultiFormListBoxWindowed::elements()
{
    KMultiFormListBoxEntryList list;
    for (int i = 0; i < _listbox->count(); ++i) {
        WindowListboxItem *item = (WindowListboxItem *)_listbox->item(i);
        list.append(item->entry());
    }
    return list;
}

void KMultiFormListBoxWindowed::delElement(QWidget * /*elm*/)
{
    // kDebug() << "KMultiFormListBoxWindowed::delElement NOT YET IMPLEMENTED";
    // TODO
}

void KMultiFormListBoxWindowed::delAnElement()
{
    // kDebug() << "KMultiFormListBoxWindowed::delAnElement NOT YET IMPLEMENTED";
    // TODO
}

void KMultiFormListBoxWindowed::append(KMultiFormListBoxEntry *elm)
{
    (void)new WidgetWindow(_factory, elm, _listbox);
    slotUpdateButtonState();
}

void KMultiFormListBoxWindowed::addNewElement()
{
    // kDebug() << "addNewElement " << _factory << "," << _listbox ;

    QWidget *widget = new WidgetWindow(_factory, _listbox);
    widget->show();
    connect(widget, SIGNAL(finished()), this, SLOT(slotUpdateButtonState()));
}

void KMultiFormListBoxWindowed::addElement()
{
    new WidgetWindow(_factory, _listbox);
    slotUpdateButtonState();
}

void KMultiFormListBoxWindowed::slotEditSelected(QListWidgetItem *item)
{
    ((WindowListboxItem *)item)->displayWidget();
}

void KMultiFormListBoxWindowed::slotEditSelected()
{
    WindowListboxItem *item = selected();
    if (item) {
        slotEditSelected(item);
    }
}

void KMultiFormListBoxWindowed::slotDeleteEntry()
{
    WindowListboxItem *item = selected();
    if (item) {
        int answer = KMessageBox::warningContinueCancel(nullptr, i18n("Delete item \"%1\"?", item->text()), i18n("Delete Item"), KStandardGuiItem::del());
        if (answer == KMessageBox::Continue) {
            delete item;
            slotUpdateButtonState();
        }
    }
}

void KMultiFormListBoxWindowed::slotCopySelected()
{
    WindowListboxItem *item = selected();
    if (item) {
        item->cloneItem();
    }
}

WindowListboxItem *KMultiFormListBoxWindowed::selected()
{
    /*
    int i = _listbox->currentItem();
    if (i == -1) {
    return 0;
    } else {
    return (WindowListboxItem *) _listbox->item(i);
    }
    */
    return (WindowListboxItem *)_listbox->currentItem();
}

void KMultiFormListBoxWindowed::slotMoveItemUp()
{
    WindowListboxItem *item = selected();
    if (item == nullptr) {
        return;
    }

    int index = _listbox->row(item);
    if (index != 0) {
        _listbox->takeItem(index);
        _listbox->insertItem(index - 1, item);
        _listbox->setCurrentItem(item);
    }
}

void KMultiFormListBoxWindowed::slotMoveItemDown()
{
    WindowListboxItem *item = selected();
    if (item == nullptr) {
        return;
    }

    int index = _listbox->row(item);
    if (index < _listbox->count()) {
        _listbox->takeItem(index);
        _listbox->insertItem(index + 1, item);
        _listbox->setCurrentItem(item);
    }
}

void KMultiFormListBoxWindowed::slotUpdateButtonState()
{
    bool on = (_listbox->count() != 0);
    const int nbButton = _buttonList.count();
    for (int i = 0; i < nbButton; i++) {
        _buttonList.at(i)->setEnabled(on);
    }
}

#include "moc_kmultiformlistbox-windowed.cpp"
