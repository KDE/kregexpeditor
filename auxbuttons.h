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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
#ifndef AUXBUTTONS_H
#define AUXBUTTONS_H

#include <qdockwindow.h>
class QComboBox;
class QLabel;
class QToolButton;

class AuxButtons :public QDockWindow
{
    Q_OBJECT

public:
    AuxButtons( QWidget* parent, const char* name );

signals:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void save();

public slots:
    void slotEnterWhatsThis();
    void slotCanUndo( bool );
    void slotCanRedo( bool );
    void slotCanCut( bool );
    void slotCanCopy( bool );
    void slotCanPaste( bool );
    void slotCanSave( bool );

private:
    QToolButton* _undo;
    QToolButton* _redo;
    QToolButton* _cut;
    QToolButton* _copy;
    QToolButton* _paste;
    QToolButton* _save;
};


#endif // AUXBUTTONS_H
