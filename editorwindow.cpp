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

#ifdef QT_ONLY
  #include "compat.h"
  #include "images.h"
#else
  #include <klocale.h>
  #include <kmessagebox.h>
//   #include <kfiledialog.h>
  #include <kstandarddirs.h>
  #include <kiconloader.h>
  #include "editorwindow.moc"
  #include <klineeditdlg.h>
#endif

#include "editorwindow.h"
#include "concwidget.h"
#include "drag.h"
#include <qlayout.h>
#include <qpainter.h>
#include <qaccel.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qclipboard.h>
#include <qpopupmenu.h>
#include "regexp.h"
#include "userdefinedregexps.h"
#include <qregexp.h>
#include <qfileinfo.h>

RegExpEditorWindow::RegExpEditorWindow( QWidget *parent, const char *name)
    : QWidget(parent, name, Qt::WPaintUnclipped)
{
    _top = new ConcWidget(this, this);
    _layout = new QHBoxLayout( this);
    _layout->addWidget(_top);
    _top->setToplevel();
    _undrawSelection = false;
    _menu = 0;
    _insertInAction = false;
    _pasteInAction = false;
    _pasteData = 0;

    QAccel* accel = new QAccel( this );
    accel->connectItem( accel->insertItem( CTRL+Key_C ), this, SLOT( slotCopy() ) );
    accel->connectItem( accel->insertItem( CTRL+Key_X ), this, SLOT( slotCut() ) );
    accel->connectItem( accel->insertItem( Key_Delete ), this, SLOT( slotCut() ) );
    accel->connectItem( accel->insertItem( Key_BackSpace ), this, SLOT( slotCut() ) );
    accel->connectItem( accel->insertItem( CTRL+Key_V ), this, SLOT( slotStartPasteAction() ) );
    accel->connectItem( accel->insertItem( Key_Escape ), this, SLOT( slotEndActions() ) );
    accel->connectItem( accel->insertItem( CTRL+Key_S ), this, SLOT( slotSave() ) );

    connect( this, SIGNAL( change() ), this, SLOT( emitVerifyRegExp() ) );
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
    }
    else {
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
            if ( !regexp )
                return;
            QDragObject *d = new RegExpWidgetDrag( regexp, this );
            delete regexp;

            bool del = d->drag();
            if ( del )
                slotDeleteSelection();
            else {
                clearSelection( true );
            }
            releaseMouse();
            emit change();
            emit canSave( _top->hasAnyChildren() );
        }
    }
    else {
        QPainter p( this );
        p.setRasterOp( Qt::NotROP );
        p.setPen( Qt::DotLine );

        // remove last selection rectangle
        if ( ! _lastPoint.isNull() && _undrawSelection ) {
            p.drawRect(QRect(_start, _lastPoint));
        }

        // Note this line must come after the old rect has been removed
        // and before the new one is draw otherwise the update event which this
        // line invokes, will remove a line, which later will be drawn instead of
        // removed during NotROP.
        _top->updateSelection( false );
        emit scrolling( event->pos() );

        p.drawRect(QRect(_start, event->pos()));
        _undrawSelection = true;
        _lastPoint = event->pos();

        _selection = QRect(mapToGlobal(_start), mapToGlobal(_lastPoint)).normalize();
    }
}

void RegExpEditorWindow::mouseReleaseEvent( QMouseEvent *event)
{
    releaseMouse();
    QWidget::mouseReleaseEvent( event);

    // remove last selection rectangle
    QPainter p( this );
    p.setRasterOp( Qt::NotROP );
    p.setPen( Qt::DotLine );
    if ( ! _lastPoint.isNull() ) {
        p.drawRect(QRect(_start, _lastPoint));
    }
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
    if ( update )
        _top->updateAll();
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
    if ( _pasteData )
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
        KMessageBox::information(this, i18n( "There is no selection"), i18n("Missing Selection") );
    }
    else {
        _top->deleteSelection();
    }
    updateContent( 0 );
}

void RegExpEditorWindow::updateContent( QWidget* focusChild)
{
    QPoint p(0,0);
    if ( focusChild )
        p = focusChild->mapTo( this, QPoint(0,0) );

    _top->update();
    emit contentChanged( p );
}

QSize RegExpEditorWindow::sizeHint() const
{
    return _top->sizeHint();
}

void RegExpEditorWindow::paintEvent( QPaintEvent* event )
{
    QWidget::paintEvent( event );
    _undrawSelection = false;
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
            KMessageBox::information(this, i18n("There is no widget under cursor"), i18n("invalid operation") );
            return;
        }
        else {
            widget->updateSelection( true ); // HACK!
        }
    }

    RegExp* regexp = _top->selection();
    RegExpWidgetDrag *clipboardData = new RegExpWidgetDrag( regexp, this );
    delete regexp;

    QClipboard* clipboard = qApp->clipboard();
    clipboard->setData( clipboardData );
    emit anythingOnClipboard( true );
    emit canSave( _top->hasAnyChildren() );
}


void RegExpEditorWindow::slotStartPasteAction()
{
    QByteArray data = qApp->clipboard()->data()->encodedData( "KRegExpEditor/widgetdrag" );
    QTextStream stream( data, IO_ReadOnly );
    QString str = stream.read();

    RegExp* regexp = WidgetFactory::createRegExp( str );
    if ( regexp )
        slotInsertRegExp( regexp );
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
        _menu = new QPopupMenu( 0 );
        _menu->insertItem(getIcon(QString::fromLocal8Bit("editcut")),
                          i18n("C&ut"), CUT);
        _menu->insertItem(getIcon(QString::fromLocal8Bit("editcopy")),
                          i18n("&Copy"), COPY);
        _menu->insertItem(getIcon(QString::fromLocal8Bit("editpaste")),
                          i18n("&Paste"), PASTE);
        _menu->insertSeparator();
        _menu->insertItem(getIcon(QString::fromLocal8Bit("edit")),
                          i18n("&Edit"), EDIT);
        _menu->insertItem(getIcon(QString::fromLocal8Bit("filesave")),
                          i18n("&Save Regular Expression..."), SAVE);
    }

    _menu->setItemEnabled( CUT, enableCutCopy );
    _menu->setItemEnabled( COPY, enableCutCopy );

    if ( ! qApp->clipboard()->data()->provides( "KRegExpEditor/widgetdrag" ) )
        _menu->setItemEnabled( PASTE, false );
    else
        _menu->setItemEnabled( PASTE, true );

    _menu->setItemEnabled( SAVE, _top->hasAnyChildren() );

    RegExpWidget* editWidget = _top->findWidgetToEdit( QCursor::pos() );

    _menu->setItemEnabled( EDIT, editWidget  );

    QPoint pos = QCursor::pos();
    int choice = _menu->exec( pos );
    switch ( choice ) {
    case COPY: copy( pos ); break;
    case CUT: cut( pos ); break;
    case PASTE: slotStartPasteAction(); break;
    case SAVE: slotSave(); break;
    case EDIT: editWidget->edit(); break;
    }
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

#ifdef QT_ONLY
    txt = QInputDialog::getText( tr("Name for regexp"), tr("Enter name:") );
    if ( txt.isNull() )
        return;
#else
    KLineEditDlg dlg(i18n("Enter name:"), QString::null, this);
    dlg.setCaption(i18n("Name for regexp"));
    if (!dlg.exec()) return;
    txt = dlg.text();
#endif

    QString fileName = dir + QString::fromLocal8Bit("/") + txt + QString::fromLocal8Bit(".regexp");
    QFileInfo finfo( fileName );
    if ( finfo.exists() ) {
        int answer = KMessageBox::warningYesNo( this, i18n("<p>Overwrite named regular expression <b>%1</b></p>").arg(txt));
        if ( answer != KMessageBox::Yes )
            return;
    }

    QFile file( fileName );
    if ( ! file.open(IO_WriteOnly) ) {
        KMessageBox::sorry( this, i18n("Could not open file for writing: %1").arg(fileName) );
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
    if ( widget )
        widget->updateCursorShape();
}

void RegExpEditorWindow::emitVerifyRegExp()
{
    emit verifyRegExp();
}


QIconSet RegExpEditorWindow::getIcon( const QString& name )
{
#ifdef QT_ONLY
    QPixmap pix;
    pix.convertFromImage( qembed_findImage( name ) );
    return pix;
#else
        return SmallIconSet( name );
#endif
}

