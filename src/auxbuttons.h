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

#ifndef AUXBUTTONS_H
#define AUXBUTTONS_H

#include <QToolBar>

class QToolButton;

class AuxButtons : public QToolBar
{
    Q_OBJECT

public:
    explicit AuxButtons(QWidget *parent, const QString &name = QString());

Q_SIGNALS:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void save();

public Q_SLOTS:
    void slotEnterWhatsThis();
    void slotCanUndo(bool);
    void slotCanRedo(bool);
    void slotCanCut(bool);
    void slotCanCopy(bool);
    void slotCanPaste(bool);
    void slotCanSave(bool);

private:
    QToolButton *_undo = nullptr;
    QToolButton *_redo = nullptr;
    QToolButton *_cut = nullptr;
    QToolButton *_copy = nullptr;
    QToolButton *_paste = nullptr;
    QToolButton *_save = nullptr;
};

#endif // AUXBUTTONS_H
