#include "kregexpeditorgui.h"
#include "scrollededitorwindow.h"
#include "regexp.h"
#include <unistd.h>
#include <stdio.h>
#include "infopage.h"
#include "kregexpeditorprivate.h"
#include <klocale.h>
#include <kgenericfactory.h>
#include <kapp.h>
#include <qlayout.h>

const QString KRegExpEditorGUI::version = QString::fromLocal8Bit("1.0");


KRegExpEditorGUI::KRegExpEditorGUI(QObject *parent, const char *name) 
  : QWidget( dynamic_cast<QWidget *>( parent ), name)
{
  QHBoxLayout* layout = new QHBoxLayout( this );
  _editor = new KRegExpEditorPrivate( this, "_editor" );
  layout->addWidget( _editor );
  connect( _editor, SIGNAL( canUndo(bool) ), this, SIGNAL( canUndo(bool) ) );
  connect( _editor, SIGNAL( canRedo(bool) ), this, SIGNAL( canRedo(bool) ) );
  connect( _editor, SIGNAL( changes(bool) ), this, SIGNAL( changes(bool) ) );  
}

QString KRegExpEditorGUI::regexp() const
{
  return _editor->regexp();
}

void KRegExpEditorGUI::slotRedo()
{
  _editor->slotRedo();
}

void KRegExpEditorGUI::slotUndo()
{
  _editor->slotUndo();
}

void KRegExpEditorGUI::slotSetRegExp( const QString &regexp )
{
  _editor->slotSetRegexp( regexp );
}

KRegExpEditorGUIDialog::KRegExpEditorGUIDialog( QObject *parent, 
	                                        const char *name )
  : KDialogBase( KDialogBase::Plain, i18n("Regular Expression Editor"),
	         KDialogBase::Ok | KDialogBase::Cancel | KDialogBase::Help, KDialogBase::Ok,
		 dynamic_cast<QWidget *>( parent ) , name ? name : "KRegExpDialog" )
{
    QFrame* frame = plainPage();
    QVBoxLayout* layout = new QVBoxLayout( frame );
    layout->setAutoAdd( true );
    _editor = new KRegExpEditorGUI( frame );
}

QWidget *KRegExpEditorGUIDialog::regExpEditor() const
{
    return _editor;
}

void KRegExpEditorGUIDialog::slotHelp()
{
    kapp->invokeHelp( QString::null, QString::fromLatin1( "KRegExpEditor" ) );
}

typedef K_TYPELIST_2( KRegExpEditorGUI, KRegExpEditorGUIDialog ) Products;
K_EXPORT_COMPONENT_FACTORY( libkregexpeditorgui, KGenericFactory<Products> );

#include "kregexpeditorgui.moc"
