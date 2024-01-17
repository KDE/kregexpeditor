/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *  SPDX-FileCopyrightText: 2011 Morten A. B. Sjøgren <m_abs@mabs.dk>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

//---------------------
// ccp = Cut-Copy-Paste
//---------------------

#include "ccp.h"

#include <QEvent>
#include <QMenu>
#include <QMouseEvent>

#include "kmultiformlistbox-multivisible.h"

CCP::CCP(KMultiFormListBoxMultiVisible *ee_, KMultiFormListBoxEntry *eee_)
    : QObject()
{
    ee = ee_;
    eee = eee_;
    install(eee);
}

void CCP::install(QObject *elm)
{
    elm->installEventFilter(this);
    const QList<QObject *> children = elm->children();
    if (!children.isEmpty()) {
        for (int i = 0; i < children.size(); ++i) {
            if (children.at(i)->inherits("KMultiFormListBoxMultiVisible")) {
                // Stop if the widget is an KMultiFormListBox, as this widget has its own cut/copy/paste
            } else {
                install(children.at(i));
            }
        }
    }
}

// This function post the Cut/Copy/Paste menu
bool CCP::eventFilter(QObject *, QEvent *event)
{
    if (event->type() != QEvent::MouseButtonPress) {
        return false;
    }

    auto mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->button() != Qt::RightButton || (mouseEvent->modifiers() & Qt::ControlModifier) == 0) {
        return false;
    }

    QPoint pos = mouseEvent->globalPos();

    QMenu menu;
    QAction *cutAction = menu.addAction(i18n("Cut"));
    QAction *copyAction = menu.addAction(i18n("Copy"));
    QAction *pasteAction = menu.addAction(i18n("Paste"));
    QAction *blankAction = menu.addAction(i18n("Insert Blank"));

    QAction *res = menu.exec(pos);
    if (res) {
        if (res == cutAction) {
            ee->cut(eee);
        } else if (res == copyAction) {
            ee->copy(eee);
        } else if (res == pasteAction) {
            ee->paste(eee);
        } else if (res == blankAction) {
            ee->addElement(eee);
        }
    }
    return true;
}

#include "moc_ccp.cpp"
