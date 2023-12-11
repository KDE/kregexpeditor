/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
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
