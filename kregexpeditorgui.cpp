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
#include <qlayout.h>

const QString KRegExpEditorGUI::version = QString::fromLocal8Bit("1.0");


KRegExpEditorGUI::KRegExpEditorGUI(QWidget *parent, const char *name) 
  : KRegExpEditor(parent, name)
{
  QHBoxLayout* layout = new QHBoxLayout( this );
  _editor = new KRegExpEditorPrivate( this, "_editor" );
  layout->addWidget( _editor );
}

QString KRegExpEditorGUI::regexp()
{
  return _editor->regexp();
}

bool KRegExpEditorGUI::canUndo() const
{
  return _editor->canUndo();
}

bool KRegExpEditorGUI::canRedo() const
{
  return _editor->canRedo();
}

void KRegExpEditorGUI::slotRedo()
{
  _editor->slotRedo();
}

void KRegExpEditorGUI::slotUndo()
{
  _editor->slotUndo();
}


extern "C" {
  void* init_libkregexpeditorgui() 
  {
    return new KRegExpEditorFactoryImpl;
  }
}

#include "kregexpeditorgui.moc"
