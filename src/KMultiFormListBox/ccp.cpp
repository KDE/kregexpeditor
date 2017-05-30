/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *  Copyright (c) 2011 Morten A. B. Sjøgren <m_abs@mabs.dk>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

//---------------------
// ccp = Cut-Copy-Paste
//---------------------

#include "ccp.h"

#include <QMenu>
#include <QEvent>
#include <QMouseEvent>

#include "kmultiformlistbox-multivisible.h"

CCP::CCP(KMultiFormListBoxMultiVisible *ee_, KMultiFormListBoxEntry *eee_) : QObject()
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
    if (event->type() != QEvent::MouseButtonPress
        || ((QMouseEvent *)event)->button() != Qt::RightButton
        || (((QMouseEvent *)event)->modifiers() & Qt::ControlModifier) == 0) {
        return false;
    }

    QPoint pos = ((QMouseEvent *)event)->globalPos();

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
