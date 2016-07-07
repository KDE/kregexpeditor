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

#ifndef VERIFYBUTTONS_H
#define VERIFYBUTTONS_H

#include <QToolBar>
#include <QLinkedList>

class QToolButton;
class QLabel;
class QAction;
class QMenu;
class RegExpConverter;

class VerifyButtons : public QToolBar
{
    Q_OBJECT

public:
    VerifyButtons(QWidget *parent, const char *name);
    RegExpConverter *setSyntax(const QString &);
    void setAllowNonQtSyntax(bool);

signals:
    void verify();
    void autoVerify(bool);
    void loadVerifyText(const QString &);
    void matchGreedy(bool);

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void gotoFirst();
    // void gotoPrev();
    // void gotoNext();
    // void gotoLast();

    void changeSyntax(const QString &);

public slots:
    //     void enableForwardButtons( bool );
    //     void enableBackwardButtons( bool );
    void setMatchCount(int);

protected slots:
    void updateVerifyButton(bool);
    void loadText();
    void slotChangeSyntax(QAction *action);

private:
    QToolButton *_verify;
    QLabel *_matches;
    QMenu *_configMenu;
    QMenu *_languages;

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // QToolButton* _first;
    // QToolButton* _prev;
    // QToolButton* _next;
    // QToolButton* _last;

    QLinkedList< QPair<RegExpConverter *, QAction *> > _converters;
};

#endif // VERIFYBUTTONS_H
