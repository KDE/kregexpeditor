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
#include "textwidget.h"
#include "textregexp.h"
#include "selectablelineedit.h"
#include <qlayout.h>
#include <qcursor.h>
#include <qapplication.h>
TextWidget::TextWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                       const char *name)
  :RegExpWidget(editorWindow, parent, name)
{
  init( QString::fromLocal8Bit("") );
}

TextWidget::TextWidget( TextRegExp* regexp,  RegExpEditorWindow* editorWindow,
            QWidget* parent, const char* name )
  : RegExpWidget( editorWindow, parent, name )
{
  init(regexp->text());
}

void TextWidget::init( const QString& txt )
{
  QHBoxLayout *lay = new QHBoxLayout(this);
  _edit = new SelectableLineEdit( this, this, "TextWidget::edit" );
  _edit->setDragEnabled( false ); //otherwise QLineEdit::mouseMoveEvent will set the cursor over and over again.
  lay->addWidget(_edit);

  _edit->setText( txt );

  connect( _edit, SIGNAL( parentPleaseUpdate() ), this, SLOT(slotUpdate()) );
  setFocusProxy( _edit );
  _edit->installEventFilter( this );
  connect( _edit, SIGNAL( textChanged( const QString & ) ), _editorWindow, SLOT( emitChange() ) );
}


void TextWidget::slotUpdate()
{
  // I need to force the parent to repaint, as the size change of this
  // widget may not be enough for the parent to change size, and in that
  // case the parent would not repaint, and the text widget would not be
  // resized.
  QWidget *p = dynamic_cast<QWidget*>(parent());
  if (p)
    p->repaint();
  _editorWindow->updateContent( this );
}

QSize TextWidget::sizeHint() const
{
  return _edit->sizeHint();
}

void TextWidget::paintEvent( QPaintEvent *e)
{
  RegExpWidget::paintEvent(e);
}

void TextWidget::selectWidget( bool sel )
{
    _edit->setSelected( sel );
}

bool TextWidget::updateSelection(bool parentSelected)
{
  bool changed = RegExpWidget::updateSelection( parentSelected );

  // I need to call this function all the time, else the rubber band will
  // not be correctly deleted in the line edit.
  _edit->setSelected( _isSelected );
  return changed;
}
void TextWidget::updateAll()
{
  _edit->update();
  update();
}

void TextWidget::clearSelection()
{
  _isSelected = false;
  _edit->setSelected( false );
}

RegExp* TextWidget::regExp() const
{
	return new TextRegExp( isSelected(), _edit->text() );
}

bool TextWidget::eventFilter( QObject*, QEvent* event)
{
    // This is an event filter (in contrast to methods in SelectableLineEdit),
    // otherwise lots of functions would need to be exported from TextWidget.
    if ( event->type() == QEvent::MouseButtonRelease ) {
        if ( _editorWindow->isInserting() ) {
            if ( acceptWidgetInsert( _editorWindow->insertType() ) ) {
                mouseReleaseEvent( dynamic_cast<QMouseEvent*>(event) );
            }
            return true;
        }
    }
    else if ( event->type() == QEvent::MouseButtonPress ) {
        if ( _editorWindow->isInserting() ) {
            return true;
        }
        else  if ( isSelected() ) {
            QMouseEvent* e = static_cast<QMouseEvent*>( event );
            QMouseEvent ev( event->type(), mapTo(_editorWindow, e->pos()),
                            e->button(), e->state());
            QApplication::sendEvent( _editorWindow, &ev );
            return true;
        }
    }

    else if ( event->type() == QEvent::Enter ) {
        if ( _editorWindow->isInserting() ) {
            if ( acceptWidgetInsert( _editorWindow->insertType() ) ) {
                _edit->setCursor(crossCursor);
            }
            else {
                _edit->setCursor(forbiddenCursor);
            }
        }
        else if (  isSelected() ) {
            _edit->setCursor( arrowCursor );
        }
        else {
            _edit->setCursor( ibeamCursor );
        }
    }
    else if ( event->type() == QEvent::MouseButtonDblClick &&  _editorWindow->isInserting() ) {
        return true;
    }
    return false;
}


#include "textwidget.moc"
