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
#ifndef QT_ONLY
  #include "scrollededitorwindow.moc"
#endif

#include "scrollededitorwindow.h"
#include "editorwindow.h"

RegExpScrolledEditorWindow::RegExpScrolledEditorWindow( QWidget* parent, const char* name)
    : QWidget(parent, name)
{
    _scrollView = new QScrollView( this );
    _editorWindow = new RegExpEditorWindow( _scrollView->viewport());
    _scrollView->addChild( _editorWindow );
    _scrollView->setResizePolicy( QScrollView::Manual );

    connect( _editorWindow, SIGNAL( contentChanged( QPoint ) ),
             this, SLOT( slotUpdateContentSize( QPoint ) ) );

    connect( _editorWindow, SIGNAL( scrolling( QPoint ) ),
             this, SLOT( slotScroll( QPoint ) ) );

    connect( _editorWindow, SIGNAL( doneEditing() ), this, SIGNAL( doneEditing() ) );

    connect( _editorWindow, SIGNAL( change() ), this, SIGNAL( change() ) );
    connect( _editorWindow, SIGNAL( savedRegexp() ), this, SIGNAL( savedRegexp() ) );

    connect( _editorWindow, SIGNAL( anythingSelected(bool) ), this, SIGNAL( anythingSelected(bool) ) );
    connect( _editorWindow, SIGNAL( anythingOnClipboard(bool) ), this, SIGNAL( anythingOnClipboard(bool) ) );
    connect( _editorWindow, SIGNAL( canSave(bool) ), this, SIGNAL( canSave(bool) ) );
    connect( _editorWindow, SIGNAL( verifyRegExp() ), this, SIGNAL( verifyRegExp() ) );
}

void RegExpScrolledEditorWindow::slotSetRegExp( RegExp* regexp )
{
    _editorWindow->slotSetRegExp( regexp );
    slotUpdateContentSize(QPoint());
}

void RegExpScrolledEditorWindow::slotInsertRegExp( int tp)
{
    _editorWindow->slotInsertRegExp( (RegExpType) tp );
}

void RegExpScrolledEditorWindow::slotInsertRegExp( RegExp* regexp)
{
    _editorWindow->slotInsertRegExp( regexp );
}

void RegExpScrolledEditorWindow::slotDeleteSelection()
{
    _editorWindow->slotDeleteSelection();
}

void RegExpScrolledEditorWindow::slotDoSelect()
{
    _editorWindow->slotDoSelect();
}

void RegExpScrolledEditorWindow::slotCut()
{
    _editorWindow->slotCut();
}

void RegExpScrolledEditorWindow::slotCopy()
{
    _editorWindow->slotCopy();
}

void RegExpScrolledEditorWindow::slotPaste()
{
    _editorWindow->slotStartPasteAction();
}

void RegExpScrolledEditorWindow::slotSave()
{
    _editorWindow->slotSave();
}

RegExp* RegExpScrolledEditorWindow::regExp()
{
    return _editorWindow->regExp();
}

void RegExpScrolledEditorWindow::resizeEvent( QResizeEvent *event )
{
    _scrollView->resize( event->size() );
    slotUpdateContentSize(QPoint());
}

void RegExpScrolledEditorWindow::slotUpdateContentSize( QPoint focusPoint )
{
    QSize childSize = _editorWindow->sizeHint();
    QSize vpSize = _scrollView->viewportSize(10,10);

    bool change = false;

    if ( childSize.width() < vpSize.width() ) {
        childSize.setWidth( vpSize.width() );
        change = true;
    }

    if ( childSize.height() < vpSize.height() ) {
        childSize.setHeight( vpSize.height() );
        change = true;
    }

    if ( change ||
         _scrollView->contentsWidth() != childSize.width() ||
         _scrollView->contentsHeight() != childSize.height() ) {
        _editorWindow->resize( childSize );
        _scrollView->resizeContents( childSize.width(), childSize.height() );
    }

    if ( !focusPoint.isNull() ) {
        _scrollView->ensureVisible ( focusPoint.x(), focusPoint.y(), 250,250 );
    }

}


// TODO: add timers, which will make the widget scroll when mouse is located
// outside the QScrollView.
void RegExpScrolledEditorWindow::slotScroll( QPoint focusPoint )
{
    _scrollView->ensureVisible( focusPoint.x(), focusPoint.y() );
}

