/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "kmultiformlistboxentry.h"

#include <QPushButton>

//----------------------------------------------------------------------
// This function is needed to signal which of the KMultiFormListBox entries
// the Idx button was invoked from.
//----------------------------------------------------------------------
void KMultiFormListBoxEntry::acceptIndexButton()
{
    Q_EMIT gotoIndex(this);
}

void KMultiFormListBoxEntry::indexWindowPos(QPoint *start, int *width)
{
    // Calculate the position of the value widgets left-buttom border
    QPoint global_point = valueWidget()->mapToGlobal(QPoint(0, 0));
    start->setX(global_point.x());
    start->setY(global_point.y() + valueWidget()->height());

    // Calculate the width of the list.
    global_point = indexButton()->mapToGlobal(QPoint(0, 0));
    *width = global_point.x() + indexButton()->width() - start->x();
}

#include "moc_kmultiformlistboxentry.cpp"
