/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "kmultiformlistboxfactory.h"

#include <QFrame>

QWidget *KMultiFormListBoxFactory::separator(QWidget *parent)
{
    QFrame *sep = new QFrame(parent);
    sep->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    sep->setLineWidth(1);
    return sep;
}
