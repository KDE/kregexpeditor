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

#include <q3dockwindow.h>
#include "widgetfactory.h"
class DoubleClickButton;
class Q3ButtonGroup;
class QToolButton;
class QSignalMapper;


class RegExpButtons :public Q3DockWindow
{
    Q_OBJECT

public:
    explicit RegExpButtons( QWidget *parent, const char *name = 0 );
    void setFeatures( int features );

protected:
    DoubleClickButton* insert(RegExpType tp, const char* file, QString tooltip, QString whatsthis);

public slots:
    void slotSelectNewAction();
    void slotUnSelect();

protected slots:
    void slotSetKeepMode();
    void slotSetNonKeepMode();

signals:
    void clicked( int );
    void doSelect();

private:
    Q3ButtonGroup* _grp;
    QToolButton* _selectBut;
    QToolButton* _wordBoundary;
    QToolButton* _nonWordBoundary;
    QToolButton* _posLookAhead;
    QToolButton* _negLookAhead;
    QSignalMapper* _mapper; // single click Mapper.

    /**
       This variable is true if the use wishes to continue editing in the
       selected mode after the previous editing is completed (that is, if the
       user has double clicked the buttons).
    */
    bool _keepMode;
};


#endif // __REGEXPBUTTON_H
