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
#include "auxbuttons.h"
#include "auxbuttons.moc"
#include <qpushbutton.h>
#include <kiconloader.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <klocale.h>
#include <qcombobox.h>
#include <qlabel.h>

AuxButtons::AuxButtons( QWidget* parent, const char* name = 0)
  :QDockWindow( QDockWindow::InDock, parent, name)
{
  QBoxLayout* layout = boxLayout();
  KIconLoader loader;

  _undo = new QPushButton( this );
  _undo->setPixmap( loader.loadIcon(QString::fromLatin1("undo"), KIcon::Toolbar) );
  layout->addWidget( _undo );
  connect( _undo, SIGNAL(clicked()), this, SIGNAL(undo()) );
  QToolTip::add( _undo, i18n( "undo" ) );

  _redo = new QPushButton( this );
  _redo->setPixmap( loader.loadIcon(QString::fromLatin1("redo"), KIcon::Toolbar) );
  layout->addWidget( _redo );
  connect( _redo, SIGNAL(clicked()), this, SIGNAL(redo()) );
  QToolTip::add( _redo, i18n( "redo" ) );

  _cut = new QPushButton( this );
  _cut->setPixmap( loader.loadIcon(QString::fromLatin1("editcut"), KIcon::Toolbar) );
  layout->addWidget( _cut );
  connect( _cut, SIGNAL(clicked()), this, SIGNAL(cut()) );
  QToolTip::add( _cut, i18n( "cut" ) );

  _copy = new QPushButton( this );
  _copy->setPixmap( loader.loadIcon(QString::fromLatin1("editcopy"), KIcon::Toolbar) );
  layout->addWidget( _copy );
  connect( _copy, SIGNAL(clicked()), this, SIGNAL(copy()) );
  QToolTip::add( _copy, i18n( "copy" ) );

  _paste = new QPushButton( this );
  _paste->setPixmap( loader.loadIcon(QString::fromLatin1("editpaste"), KIcon::Toolbar) );
  layout->addWidget( _paste );
  connect( _paste, SIGNAL(clicked()), this, SIGNAL(paste()) );
  QToolTip::add( _paste, i18n( "paste" ) );

  _save = new QPushButton( this );
  _save->setPixmap( loader.loadIcon(QString::fromLatin1("filesave"), KIcon::Toolbar) );
  layout->addWidget( _save );
  connect( _save, SIGNAL(clicked()), this, SIGNAL(save()) );
  QToolTip::add( _save, i18n( "save" ) );


  QPushButton* button = new QPushButton(this);
  button->setPixmap( loader.loadIcon(QString::fromLatin1("contexthelp"), KIcon::Toolbar) );
  layout->addWidget( button );
  connect(button, SIGNAL(clicked()), this, SLOT(slotEnterWhatsThis()));

  _syntaxLabel = new QLabel( i18n("Language:"), this );
  layout->addWidget( _syntaxLabel );
  _syntax = new QComboBox( this );
  _syntax->insertStringList( QStringList() << i18n("Qt") << i18n("Emacs") );
  layout->addWidget( _syntax );
  connect( _syntax, SIGNAL( activated( int ) ), this, SLOT( slotChangeSyntax( int ) ) );
  _syntaxLabel->hide();
  _syntax->hide();

  _undo->setEnabled( false );
  _redo->setEnabled( false );
}

void AuxButtons::slotEnterWhatsThis()
{
  QWhatsThis::enterWhatsThisMode ();
}

void AuxButtons::slotCanUndo( bool b )
{
  _undo->setEnabled( b );
}

void AuxButtons::slotCanRedo( bool b )
{
  _redo->setEnabled( b );
}

void AuxButtons::slotCanCut( bool b )
{
  _cut->setEnabled( b );
}

void AuxButtons::slotCanCopy( bool b )
{
  _copy->setEnabled( b );
}

void AuxButtons::slotCanPaste( bool b )
{
  _paste->setEnabled( b );
}

void AuxButtons::slotCanSave( bool b )
{
  _save->setEnabled( b );
}

void AuxButtons::slotChangeSyntax( int syntax )
{
    emit changeSyntax( (RegExp::Syntax) syntax );
}

void AuxButtons::setShowSyntaxCombo( bool b )
{
    _syntaxLabel->setShown( b );
    _syntax->setShown( b );
}

void AuxButtons::setSyntax( RegExp::Syntax syntax )
{
    _syntax->setCurrentItem( (int) syntax );
}
