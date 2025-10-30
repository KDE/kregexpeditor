/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include "widgetfactory.h"
#include <QToolBar>

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
