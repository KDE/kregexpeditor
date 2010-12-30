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

#include "kregexpeditorguidialog.h"

#include <QVBoxLayout>
#include <QFrame>

#include <KLocale>
#include <KToolInvocation>

#include "kregexpeditorprivate.h"

KRegExpEditorGUIDialog::KRegExpEditorGUIDialog( QWidget *parent,
                                                const QVariantList & )
  : KDialog( parent )
{
    setCaption( i18n("Regular Expression Editor") );
    setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Help );
    setDefaultButton( KDialog::Ok );

  QFrame* frame = new QFrame(this);
  setMainWidget( frame );
  QVBoxLayout* layout = new QVBoxLayout( frame );
  layout->setSpacing( 6 );
  _editor = new KRegExpEditorGUI( frame );
  layout->addWidget(_editor);

  connect( _editor, SIGNAL( canUndo(bool) ), this, SIGNAL( canUndo(bool) ) );
  connect( _editor, SIGNAL( canRedo(bool) ), this, SIGNAL( canRedo(bool) ) );
  connect( _editor, SIGNAL( changes(bool) ), this, SIGNAL( changes(bool) ) );
  resize( 640, 400 );

  setHelp( QString(), QString::fromLocal8Bit( "KRegExpEditor" ) );
}


QString KRegExpEditorGUIDialog::regExp() const
{
    return _editor->regExp();
}

void KRegExpEditorGUIDialog::setRegExp( const QString &regexp )
{
    _editor->setRegExp( regexp );
}

void KRegExpEditorGUIDialog::redo()
{
  _editor->redo();
}

void KRegExpEditorGUIDialog::undo()
{
  _editor->undo();
}

void KRegExpEditorGUIDialog::doSomething( QString method, void* arguments )
{
    _editor->doSomething( method, arguments );
}

void KRegExpEditorGUIDialog::setMatchText( const QString& txt )
{
    _editor->setMatchText( txt );
}

#include "kregexpeditorguidialog.moc"