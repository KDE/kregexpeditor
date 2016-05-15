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

#include "regexpeditorwindow.h"

#include <QMenu>
#include <QFileInfo>
#include <QPainter>
#include <QShortcut>
#include <QClipboard>
#include <QTextStream>
#include <QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

#include <KLocale>
#include <KMessageBox>
#include <KInputDialog>
#include <KDebug>
#include <KIcon>

#include "regexp.h"
#include "userdefinedregexps.h"
#include "concwidget.h"
#include "regexpconverter.h"

RegExpEditorWindow::RegExpEditorWindow( QWidget *parent)
    : QWidget(parent /*, Qt::WPaintUnclipped*/)
{
    _top = new ConcWidget(this, this);
    _layout = new QHBoxLayout( this);
    _layout->addWidget(_top);
    _top->setToplevel();
    _menu = 0;
    _insertInAction = false;
    _pasteInAction = false;
    _pasteData = 0;

    _PosEdit= QPoint ( 0, 0 );

    (void) new QShortcut( Qt::CTRL+Qt::Key_C , this, SLOT( slotCopy() ) );
    (void) new QShortcut( Qt::CTRL+Qt::Key_X , this, SLOT( slotCut() ) );
    (void) new QShortcut( Qt::Key_Delete , this, SLOT( slotCut() ) );
    (void) new QShortcut( Qt::Key_Backspace , this, SLOT( slotCut() ) );
    (void) new QShortcut( Qt::CTRL+Qt::Key_V , this, SLOT( slotStartPasteAction() ) );
    (void) new QShortcut( Qt::Key_Escape , this, SLOT( slotEndActions() ) );
    (void) new QShortcut( Qt::CTRL+Qt::Key_S , this, SLOT( slotSave() ) );

    connect( this, SIGNAL( change() ), this, SLOT( emitVerifyRegExp() ) );
}

RegExpEditorWindow::~RegExpEditorWindow()
{
    delete _pasteData;
}

RegExp* RegExpEditorWindow::regExp() const
{
	return _top->regExp();
}

void  RegExpEditorWindow::mousePressEvent ( QMouseEvent* event )
{
    setFocus();
    updateContent( 0 );

    _start = event->pos();
    _lastPoint = QPoint(0,0);

    if ( pointSelected( event->globalPos() ) ) {
        _isDndOperation = true;
    } else {
        _isDndOperation = false;
        _selection = QRect();
        _top->updateSelection( false );

        QWidget::mousePressEvent( event );
    }
    grabMouse();
}

bool RegExpEditorWindow::pointSelected( QPoint p ) const
{
    QRect rect = _top->selectionRect();
    return rect.contains(p);
}

void RegExpEditorWindow::mouseMoveEvent ( QMouseEvent* event )
{
    if ( _isDndOperation ) {
        if ( ( _start - event->pos() ).manhattanLength() > QApplication::startDragDistance() ) {
            RegExp* regexp = _top->selection();
            if ( !regexp ) {
                return;
            }

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            
            mimeData->setText(RegExpConverter::current()->toStr( regexp, false));
            mimeData->setData("KRegExpEditor/widgetdrag", regexp->toXmlString().toAscii());
            delete regexp;
            
            drag->setMimeData(mimeData);
            
            Qt::DropAction dropAction = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
            if ( dropAction == Qt::MoveAction ) {
                slotDeleteSelection();
            }
            else {
                clearSelection( true );
            }
            
            releaseMouse();
            emit change();
            emit canSave( _top->hasAnyChildren() );
        }
    }
    else {
        
        _top->updateSelection( false );
        
        emit scrolling( event->pos() );
        
        _lastPoint = event->pos();
        
        update();

        _selection = QRect(mapToGlobal(_start), mapToGlobal(_lastPoint)).normalized();
    }
}

void RegExpEditorWindow::mouseReleaseEvent( QMouseEvent *event)
{
    releaseMouse();
    QWidget::mouseReleaseEvent( event);
    
    _lastPoint = QPoint(0, 0);

    _top->validateSelection();
    _top->updateAll();
    emit anythingSelected( hasSelection() );
    if ( hasSelection() ) {
        emit verifyRegExp();
    }
}

bool RegExpEditorWindow::selectionOverlap( QPoint pos, QSize size ) const
{
    QRect child(pos, size);

    return (_selection.intersects(child) && ! child.contains(_selection));
}

bool RegExpEditorWindow::hasSelection() const
{
    return _top->hasSelection();
}

void RegExpEditorWindow::clearSelection( bool update )
{
    _top->clearSelection();
    if ( update ) {
        _top->updateAll();
    }
    emit anythingSelected(false);
}

void RegExpEditorWindow::slotInsertRegExp( RegExpType type )
{
    _insertInAction = true;
    _insertTp = type;

    updateCursorUnderPoint();
    setFocus();
}

void RegExpEditorWindow::slotInsertRegExp( RegExp* regexp )
{
    delete _pasteData;

    _pasteData = regexp->clone();
    _pasteInAction = true;
    updateCursorUnderPoint();
    setFocus();
}

void RegExpEditorWindow::slotDoSelect()
{
    _pasteInAction = false;
    _insertInAction = false;

    // I need to update the cursor recursively, as a repaint may not have been issued yet
    // when this method is invoked. This means that when the repaint comes, the cursor may
    // move to an other widget.
    _top->updateCursorRecursively();
}

void RegExpEditorWindow::slotDeleteSelection()
{
    if ( ! hasSelection() ) {
        KMessageBox::information(this, i18n( "There is no selection."), i18n("Missing Selection") );
    }
    else {
        _top->deleteSelection();
    }
    updateContent( 0 );
}

void RegExpEditorWindow::updateContent( QWidget* focusChild)
{
    QPoint p(0,0);
    if ( focusChild ) {
        p = focusChild->mapTo( this, QPoint(0,0) );
    }

    _top->update();
    emit contentChanged( p );
}

QSize RegExpEditorWindow::sizeHint() const
{
    return _top->sizeHint();
}

void RegExpEditorWindow::paintEvent( QPaintEvent* event )
{
    QPainter p(this);
    
    p.setPen(Qt::DotLine);
    
    if ( ! _lastPoint.isNull() ) {
        p.drawRect(QRect(_start, _lastPoint));
    }
    
    QWidget::paintEvent( event );
}

void RegExpEditorWindow::slotCut()
{
    cut( QCursor::pos() );
    emit change();
    emit canSave( _top->hasAnyChildren() );
}

void RegExpEditorWindow::cut( QPoint pos )
{
    cutCopyAux( pos );
    slotDeleteSelection();
}

void RegExpEditorWindow::slotCopy()
{
    copy( QCursor::pos() );
}

void RegExpEditorWindow::copy( QPoint pos )
{
    cutCopyAux( pos );
    clearSelection( true );
}


void RegExpEditorWindow::cutCopyAux( QPoint pos )
{
    if ( !hasSelection() ) {
        RegExpWidget* widget = _top->widgetUnderPoint( pos, true );
        if ( !widget ) {
            KMessageBox::information(this, i18n("There is no widget under cursor."), i18n("Invalid Operation") );
            return;
        }
        else {
            widget->updateSelection( true ); // HACK!
        }
    }

    RegExp* regexp = _top->selection();

    QMimeData *mimeData = new QMimeData; 
    mimeData->setText(RegExpConverter::current()->toStr( regexp, false));
    mimeData->setData("KRegExpEditor/widgetdrag", regexp->toXmlString().toAscii());

    delete regexp;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setMimeData( mimeData );
    emit anythingOnClipboard( true );
    emit canSave( _top->hasAnyChildren() );
}


void RegExpEditorWindow::slotStartPasteAction()
{
    QString str = qApp->clipboard()->mimeData()->data( "KRegExpEditor/widgetdrag" );
    if (str.isEmpty()) {
        return;
    }

    RegExp* regexp = WidgetFactory::createRegExp( str );
    if ( regexp ) {
        slotInsertRegExp( regexp );
    }
}

void RegExpEditorWindow::slotEndActions() {
    emit doneEditing();
    emit change();
    emit canSave( _top->hasAnyChildren() );
}

void RegExpEditorWindow::showRMBMenu( bool enableCutCopy )
{
    enum CHOICES { CUT, COPY, PASTE, SAVE, EDIT };

    if ( !_menu ) {
        _menu = new QMenu( 0 );
        
        _cutAction = _menu->addAction(getIcon(QString::fromLocal8Bit("edit-cut")),
                          i18n("C&ut"));
        connect(_cutAction, SIGNAL(triggered()), this, SLOT(slotCut()));
        
        _copyAction = _menu->addAction(getIcon(QString::fromLocal8Bit("edit-copy")),
                          i18n("&Copy"));
        connect(_copyAction, SIGNAL(triggered()), this, SLOT(slotCopy()));
        
        _pasteAction = _menu->addAction(getIcon(QString::fromLocal8Bit("edit-paste")),
                          i18n("&Paste"));
        connect(_pasteAction, SIGNAL(triggered()), this, SLOT(slotStartPasteAction()));
        
        _menu->addSeparator();
        
        _editAction = _menu->addAction(getIcon(QString::fromLocal8Bit("document-properties")),
                          i18n("&Edit"));
        connect(_editAction, SIGNAL(triggered()), this, SLOT(editWidget()));
        
        _saveAction = _menu->addAction(getIcon(QString::fromLocal8Bit("document-save")),
                          i18n("&Save Regular Expression..."));
        connect(_saveAction, SIGNAL(triggered()), this, SLOT(slotSave()));
    }

    _cutAction->setEnabled( enableCutCopy );
    _copyAction->setEnabled( enableCutCopy );

    if ( ! qApp->clipboard()->mimeData()->hasFormat( "KRegExpEditor/widgetdrag" ) ) {
        _pasteAction->setEnabled( false );
    } else {
        _pasteAction->setEnabled( true );
    }

    _saveAction->setEnabled( _top->hasAnyChildren() );

    _PosEdit = QCursor::pos();

    RegExpWidget* editWidget = _top->findWidgetToEdit( _PosEdit );

    _editAction->setEnabled( editWidget  );

    _menu->exec( _PosEdit );

    _PosEdit = QPoint ( 0, 0 );

    emit change();
    emit canSave( _top->hasAnyChildren() );
}

void RegExpEditorWindow::applyRegExpToSelection( RegExpType tp )
{
    _top->applyRegExpToSelection( tp );
}

void RegExpEditorWindow::slotSave()
{
    QString dir = WidgetWinItem::path();
    QString txt;

    QString tmp = KInputDialog::getText( i18n("Name for Regular Expression"), i18n("Enter name:"));
    if( tmp.isEmpty()) {
      return;
    }
    txt = tmp;

    QString fileName = dir + QString::fromLocal8Bit("/") + txt + QString::fromLocal8Bit(".regexp");
    QFileInfo finfo( fileName );
    if ( finfo.exists() ) {
        int answer = KMessageBox::warningContinueCancel( this, i18n("<p>Overwrite named regular expression <b>%1</b></p>", txt), QString(), KStandardGuiItem::overwrite() );
        if ( answer != KMessageBox::Continue ) {
            return;
	}
    }

    QFile file( fileName );
    if ( ! file.open(QIODevice::WriteOnly) ) {
        KMessageBox::sorry( this, i18n("Could not open file for writing: %1", fileName) );
        return;
    }

    // Convert to XML.
    RegExp* regexp = _top->regExp();
    QString xml = regexp->toXmlString();
    delete regexp;

    QTextStream stream(&file);
    stream << xml;

    file.close();
    emit savedRegexp();
}


void RegExpEditorWindow::slotSetRegExp( RegExp* regexp )
{
    // I have no clue why the following line is necesarry, but if it is not here
    // then the editor area is messed up when calling slotSetRegExp before starting the eventloop.
    qApp->processEvents();

    delete _top;
    RegExpWidget* widget = WidgetFactory::createWidget( regexp, this, this );
    if ( ! (_top = dynamic_cast<ConcWidget*>( widget ) ) ) {
        // It was not a ConcWidget
        _top = new ConcWidget( this, widget, this );
    }
    _top->setToplevel();

    _top->show();
    _layout->addWidget( _top );
    clearSelection( true ); // HACK?
    emit canSave( _top->hasAnyChildren() );
}

void RegExpEditorWindow::updateCursorUnderPoint()
{
    RegExpWidget* widget = _top->widgetUnderPoint( QCursor::pos(), false );
    if ( widget ) {
        widget->updateCursorShape();
    }
}

void RegExpEditorWindow::emitVerifyRegExp()
{
    emit verifyRegExp();
}

void RegExpEditorWindow::editWidget() 
{
    QPoint EditPos = _PosEdit.isNull() ? QCursor::pos() : _PosEdit;
    RegExpWidget* editWidget = _top->findWidgetToEdit( EditPos );
    if ( editWidget ) {
      editWidget->edit();
    }
}

QIcon RegExpEditorWindow::getIcon( const QString& name )
{
    return KIcon( name );
}

