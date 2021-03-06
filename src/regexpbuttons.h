/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
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

#ifndef __REGEXPBUTTONS_H
#define __REGEXPBUTTONS_H

#include <QToolBar>
#include "widgetfactory.h"

class DoubleClickButton;
class QButtonGroup;
class QToolButton;

class RegExpButtons : public QToolBar
{
    Q_OBJECT

public:
    explicit RegExpButtons(QWidget *parent, const QString &name = QString());
    void setFeatures(int features);

protected:
    DoubleClickButton *insert(RegExpType tp, const QString &file, const QString &tooltip, const QString &whatsthis);

public Q_SLOTS:
    void slotSelectNewAction();
    void slotUnSelect();

protected Q_SLOTS:
    void slotSetKeepMode();
    void slotSetNonKeepMode();

Q_SIGNALS:
    void clicked(int);
    void doSelect();

private:
    QButtonGroup *_grp = nullptr;
    QToolButton *_selectBut = nullptr;
    QToolButton *_wordBoundary = nullptr;
    QToolButton *_nonWordBoundary = nullptr;
    QToolButton *_posLookAhead = nullptr;
    QToolButton *_negLookAhead = nullptr;

    /**
       This variable is true if the use wishes to continue editing in the
       selected mode after the previous editing is completed (that is, if the
       user has double clicked the buttons).
    */
    bool _keepMode;
};

#endif // __REGEXPBUTTON_H
