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
#include "regexpconverter.h"
#include <qcombobox.h>
#include <qlabel.h>
#include <qvaluelist.h>
class QToolButton;
class QLabel;

class VerifyButtons :public QDockWindow
{
    Q_OBJECT

public:
    VerifyButtons( QWidget* parent, const char* name );
    void setShowSyntaxCombo( bool );
    RegExpConverter* setSyntax( const QString& );

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

    void changeSyntax( const QString& );

public slots:
    //     void enableForwardButtons( bool );
    //     void enableBackwardButtons( bool );
    void setMatchCount( int );

protected slots:
    void updateVerifyButton( bool );
    void loadText();
    void slotChangeSyntax( int );

protected:
    QPixmap getIcon( const QString& name );

private:
    QToolButton* _autoVerify;
    QToolButton* _verify;
    QLabel* _matches;

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // QToolButton* _first;
    // QToolButton* _prev;
    // QToolButton* _next;
    // QToolButton* _last;

    QComboBox* _syntax;
    QLabel* _syntaxLabel;
    QValueList<RegExpConverter*> _converters;
};


#endif // __VERIFYBUTTONS_H
