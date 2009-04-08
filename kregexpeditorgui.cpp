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

#include "kregexpeditorgui.h"

#include <QVBoxLayout>
#include <QFrame>

#include <KLocale>
#include <KToolInvocation>

#include "kregexpeditorprivate.h"

const QString KRegExpEditorGUI::version = QString::fromLocal8Bit("1.0");


KRegExpEditorGUI::KRegExpEditorGUI(QWidget *parent,
	                           const QStringList & )
  : QWidget( parent)
{
  QHBoxLayout* layout = new QHBoxLayout( this );
  layout->setSpacing( 6 );
  _editor = new KRegExpEditorPrivate( this );
  layout->addWidget( _editor );
  _editor->setVisible(true);
  connect( _editor, SIGNAL( canUndo(bool) ), this, SIGNAL( canUndo(bool) ) );
  connect( _editor, SIGNAL( canRedo(bool) ), this, SIGNAL( canRedo(bool) ) );
  connect( _editor, SIGNAL( changes(bool) ), this, SIGNAL( changes(bool) ) );
}

QString KRegExpEditorGUI::regExp() const
{
  return _editor->regexp();
}

void KRegExpEditorGUI::redo()
{
  _editor->slotRedo();
}

void KRegExpEditorGUI::undo()
{
  _editor->slotUndo();
}

void KRegExpEditorGUI::setRegExp( const QString &regexp )
{
  _editor->slotSetRegexp( regexp );
}

KRegExpEditorGUIDialog::KRegExpEditorGUIDialog( QWidget *parent,
                                                const QStringList & )
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

void KRegExpEditorGUI::doSomething( QString method, void* arguments )
{
    if ( method == QString::fromLatin1( "setCaseSensitive" ) ) {
        _editor->setCaseSensitive( (bool) arguments );
    }
    else if ( method == QString::fromLatin1("setMinimal") ) {
        _editor->setMinimal( (bool) arguments );
    }
    else if ( method == QString::fromLatin1("setSyntax") ) {
        _editor->setSyntax( *((QString*) arguments) );
    }
    else if ( method == QString::fromLatin1("setAllowNonQtSyntax") ) {
        _editor->setAllowNonQtSyntax( (bool) arguments );
    }
    else {
        qFatal( "%s", qPrintable(tr("Method '%1' is not valid!").arg(method)) );
    }
}

void KRegExpEditorGUIDialog::setMatchText( const QString& txt )
{
    _editor->setMatchText( txt );
}

void KRegExpEditorGUI::setMatchText( const QString& txt )
{
    _editor->setMatchText( txt );
}


void KRegExpEditorGUI::showHelp()
{
    KToolInvocation::invokeHelp( QString(), QString::fromLocal8Bit( "KRegExpEditor" ) );
}

#include "kregexpeditorgui.moc"
