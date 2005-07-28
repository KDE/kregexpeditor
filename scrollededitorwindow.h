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
#ifndef __scrolledEditorWindow
#define __scrolledEditorWindow

#include <q3hbox.h>
//Added by qt3to4:
#include <QResizeEvent>
class Q3ScrollView;
class RegExpEditorWindow;
class RegExp;


/**
   Regular Expression editor window.
*/
class RegExpScrolledEditorWindow :public QWidget
{
    Q_OBJECT
public:
    RegExpScrolledEditorWindow( QWidget *parent = 0, const char* name = 0);
    RegExp* regExp();

private:
    RegExpScrolledEditorWindow() {}; // Dont use!


protected:
    virtual void resizeEvent( QResizeEvent* );

public slots:
    void slotSetRegExp( RegExp* );
    void slotInsertRegExp( int );
    void slotInsertRegExp( RegExp* );
    void slotDeleteSelection();
    void slotDoSelect();
    void slotCut();
    void slotCopy();
    void slotSave();
    void slotPaste();

protected slots:
    void slotUpdateContentSize( QPoint focusPoint );
    void slotScroll( QPoint focusPoint );

    signals:
    /**
       This signal is emited when the user has completed an editing
       action. The application may chose to call @ref slotDoSelect as a
       consequence to reset to selection mode.
    */
    void doneEditing();

    /**
       This signal is emited whenever a change has taked place in the editor widget
    */
    void change();

    /**
       This signal is emitted when the user saves a regular expression.
    */
    void savedRegexp();

    void anythingSelected( bool );
    void anythingOnClipboard( bool );
    void canSave( bool );

    /**
       This signal is emitted when the regular expression has changed, or
       when the selection has changed.
     */
    void verifyRegExp();

private:
    RegExpEditorWindow* _editorWindow;
    Q3ScrollView* _scrollView;
};

#endif // __scrolledEditorWindow
