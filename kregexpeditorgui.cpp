#include <qlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <klocale.h>
#include "kregexpeditorgui.h"
#include "scrollededitorwindow.h"
#include "widgetwindow.h"
#include "regexp.h"
#include <unistd.h>
#include <stdio.h>
#include <kiconloader.h>
#include "infopage.h"
#include <kapp.h>
#include "kregexpeditorprivate.h"
#include <kregexpeditor.h>

const QString KRegExpEditorGUI::version = QString::fromLocal8Bit("1.0");


KRegExpEditorGUI::KRegExpEditorGUI(QWidget *parent, const char *name) 
  : KRegExpEditor(parent, name)
{
  QHBoxLayout* layout = new QHBoxLayout( this );
  _editor = new KRegExpEditorPrivate( this, "_editor" );
  layout->addWidget( _editor );
  connect( _editor, SIGNAL( canUndo(bool) ), this, SIGNAL( canUndo(bool) ) );
  connect( _editor, SIGNAL( canRedo(bool) ), this, SIGNAL( canRedo(bool) ) );
  connect( _editor, SIGNAL( changes(bool) ), this, SIGNAL( changes(bool) ) );  
}

QString KRegExpEditorGUI::regexp()
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

void KRegExpEditorGUI::slotSetRegExp( QString regexp )
{
  _editor->slotSetRegexp( regexp );
}

extern "C" {
  void* init_libkregexpeditorgui() 
  {
    return new KRegExpEditorFactoryImpl;
  }
}

#include "kregexpeditorgui.moc"
