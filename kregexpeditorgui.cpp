#include "kregexpeditorgui.h"
#include "regexp.h"
#include <unistd.h>
#include <stdio.h>
#include "infopage.h"
#include "kregexpeditorprivate.h"
#include <klocale.h>
#include <kgenericfactory.h>
#include <kapplication.h>
#include <qlayout.h>

const QString KRegExpEditorGUI::version = QString::fromLocal8Bit("1.0");


KRegExpEditorGUI::KRegExpEditorGUI(QWidget *parent, const char *name,
	                           const QStringList & ) 
  : QWidget( parent, name)
{
  QHBoxLayout* layout = new QHBoxLayout( this );
  _editor = new KRegExpEditorPrivate( this, "_editor" );
  layout->addWidget( _editor );
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
                                                const char *name,
                                                const QStringList & )
  : KDialogBase( KDialogBase::Plain, i18n("Regular Expression Editor"),
                 KDialogBase::Ok | KDialogBase::Cancel | KDialogBase::Help, KDialogBase::Ok,
                 parent, name ? name : "KRegExpDialog" )
{
  QFrame* frame = plainPage();
  QVBoxLayout* layout = new QVBoxLayout( frame );
  layout->setAutoAdd( true );
  _editor = new KRegExpEditorGUI( frame );
  
  connect( _editor, SIGNAL( canUndo(bool) ), this, SIGNAL( canUndo(bool) ) );
  connect( _editor, SIGNAL( canRedo(bool) ), this, SIGNAL( canRedo(bool) ) );
  connect( _editor, SIGNAL( changes(bool) ), this, SIGNAL( changes(bool) ) );
  resize( 640, 400 );

  setHelp( QString::null, QString::fromLocal8Bit( "KRegExpEditor" ) );
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
    else {
        qFatal( QString::fromLatin1("Method '%1' is not valid!").arg(method).latin1() );
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

typedef K_TYPELIST_2( KRegExpEditorGUI, KRegExpEditorGUIDialog ) Products;
K_EXPORT_COMPONENT_FACTORY( libkregexpeditorgui, 
                            KGenericFactory<Products>( "kregexpeditor" ) );

#include "kregexpeditorgui.moc"
