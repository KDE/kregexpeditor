#include "auxbuttons.h"
#include <qpushbutton.h>
#include <kiconloader.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <klocale.h>

AuxButtons::AuxButtons( QWidget* parent, const char* name = 0)
  :QDockWindow( QDockWindow::InDock, parent, name)
{
  QBoxLayout* layout = boxLayout();
  
  KIconLoader loader;

  QPushButton* button = new QPushButton(this);
  button->setPixmap( loader.loadIcon(QString::fromLatin1("contexthelp"), KIcon::Toolbar) );
  layout->addWidget( button );
  connect(button, SIGNAL(clicked()), this, SLOT(slotEnterWhatsThis()));

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


