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
#ifndef __VERIFYBUTTONS_H
#define __VERIFYBUTTONS_H

#include <qdockwindow.h>
class QPushButton;
class QLabel;

class VerifyButtons :public QDockWindow
{
    Q_OBJECT

public:
    VerifyButtons( QWidget* parent, const char* name );

signals:
    void verify();
    void autoVerify( bool );
    void loadVerifyText( const QString& );

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void gotoFirst();
    // void gotoPrev();
    // void gotoNext();
    // void gotoLast();

public slots:
    //     void enableForwardButtons( bool );
    //     void enableBackwardButtons( bool );
    void setMatchCount( int );

protected slots:
    void updateVerifyButton( bool );
    void loadText();

private:
    QPushButton* _autoVerify;
    QPushButton* _verify;
    QLabel* _matches;

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // QPushButton* _first;
    // QPushButton* _prev;
    // QPushButton* _next;
    // QPushButton* _last;
};


#endif // __VERIFYBUTTONS_H
