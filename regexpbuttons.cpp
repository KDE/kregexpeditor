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
  #include <kiconloader.h>
  #include <klocale.h>
  #include <kstandarddirs.h>
  #include "regexpbuttons.moc"
#endif

#include "regexpbuttons.h"
#include "dcbutton.h"
#include <qtooltip.h>

#include <q3buttongroup.h>
#include <qlayout.h>
#include <qsignalmapper.h>
//Added by qt3to4:
#include <QPixmap>
#include <QBoxLayout>
#include "regexpconverter.h"

RegExpButtons::RegExpButtons( QWidget *parent, const char *name )
  : Q3DockWindow( Q3DockWindow::InDock, parent, name), _keepMode(false)
{
  QBoxLayout *layout = boxLayout();

  _grp = new Q3ButtonGroup(this);
  _grp->hide();
  _grp->setExclusive( true );

  _mapper = new QSignalMapper( this, "RegExpButtons::_mapper" );
  connect( _mapper, SIGNAL( mapped(int) ), this, SIGNAL( clicked(int) ) );

  // The "select" button.
  _selectBut = new QToolButton( this);

#ifdef QT_ONLY
  QPixmap pix;
  pix.convertFromImage( qembed_findImage( "select" ) );
#else
  QPixmap pix = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/select.png") ), KIcon::Toolbar );
#endif

  _selectBut->setPixmap( pix );
  layout->addWidget( _selectBut );
  _grp->insert(_selectBut);
  _selectBut->setToggleButton( true );
  connect( _selectBut, SIGNAL(clicked()), SIGNAL(doSelect()));
  connect( _selectBut, SIGNAL(clicked()), this, SLOT(slotSetNonKeepMode()) );

  QToolTip::add( _selectBut, i18n("Selection tool"));
  _selectBut->setWhatsThis( i18n("<qt>This will change the state of the editor to <i>selection state</i>.<p>"
                             "In this state you will not be inserting <i>regexp items</i>, but instead select them. "
                             "To select a number of items, press down the left mouse button and drag it over the items.<p>"
                             "When you have selected a number of items, you may use cut/copy/paste. These functions are "
                             "found in the right mouse button menu.</qt>"));

  // Insert buttons.
  DoubleClickButton* but;

  but = insert(TEXT, "text", i18n("Text"),
               i18n( "<qt>This will insert a text field, where you may write text. The text you write will "
                     "be matched literally. (i.e. you do not need to escape any characters)</qt>" ) );
  layout->addWidget( but );


  but = insert(CHARSET, "characters", i18n("A single character specified in a range"),
               i18n("<qt>This will match a single character from a predefined range.<p>"
                    "When you insert this widget a dialog box will appear, which lets you specify "
                    "which characters this <i>regexp item</i> will match.</qt>") );
  layout->addWidget( but );


  but = insert(DOT, "anychar", i18n("Any character"),
               i18n("<qt>This will match any single character</qt>") );
  layout->addWidget( but );


  but = insert(REPEAT, "repeat", i18n("Repeated content"),
               i18n("<qt>This <i>regexp item</i> will repeat the <i>regexp items</i> it surrounds "
                    "a specified number of times.<p>"
                    "The number of times to repeat may be specified using ranges. e.g. You may specify "
                    "that it should match from 2 to 4 times, that it should match exactly 5 times, or "
                    "that it should match at least one time.<p>"
                    "Examples:<br>"
                    "If you specify that it should match <i>any</i> time, and the content it surrounds "
                    "is <tt>abc</tt>, then this <i>regexp item</i> will match the empty string, "
                    "the string <tt>abc</tt>, the string <tt>abcabc</tt>, the string <tt>abcabcabcabc</tt>, "
                    "etc.</qt>") );
  layout->addWidget( but );


  but = insert(ALTN, "altn", i18n("Alternatives"),
               i18n("<qt>This <i>regexp item</i> will match any of its alternatives.</p>"
                    "You specify alternatives by placing <i>regexp items</i> on top of "
                    "each other inside this widget.</qt>") );
  layout->addWidget( but );


  but = insert(COMPOUND,  "compound", i18n("Compound regexp"),
               i18n("<qt>This <i>regexp item</i> serves two purposes:"
                    "<ul><li>It makes it possible for you to collapse a huge <i>regexp item</i> into "
                    "a small box. This makes it easier for you to get an overview of large "
                    "<i>regexp items</i>. This is especially useful if you load a predefined <i>regexp item</i> "
                    "you perhaps don't care about the inner workings of.") );
  layout->addWidget( but );


  but = insert(BEGLINE,  "begline", i18n("Beginning of line"),
               i18n("<qt>This will match the beginning of a line.</qt>") );
  layout->addWidget( but );


  but = insert(ENDLINE,  "endline", i18n("End of line"),
               i18n("<qt>This will match the end of a line.</qt>") );
  layout->addWidget( but );


  _wordBoundary = insert(WORDBOUNDARY,  "wordboundary", i18n("Word boundary"),
                         i18n("<qt>This asserts a word boundary (This part does not actually match any characters)</qt>") );
  layout->addWidget( _wordBoundary );

  _nonWordBoundary = insert(NONWORDBOUNDARY,  "nonwordboundary", i18n("Non Word boundary"),
                            i18n("<qt>This asserts a non-word boundary "
                                 "(This part does not actually match any characters)</qt>") );
  layout->addWidget( _nonWordBoundary );

  _posLookAhead = insert(POSLOOKAHEAD,  "poslookahead", i18n("Positive Look Ahead"),
                         i18n("<qt>This asserts a regular expression (This part does not actually match any characters). "
                              "You can only use this at the end of a regular expression.</qt>") );
  layout->addWidget( _posLookAhead );

  _negLookAhead = insert(NEGLOOKAHEAD,  "neglookahead", i18n("Negative Look Ahead"),
                         i18n("<qt>This asserts a regular expression that must not match "
                              "(This part does not actually match any characters). "
                              "You can only use this at the end of a regular expression.</qt>") );
  layout->addWidget( _negLookAhead );
}

DoubleClickButton* RegExpButtons::insert(RegExpType tp, const char* name, QString tooltip, QString whatsthis)
{
#ifdef QT_ONLY
    QPixmap pix;
    pix.convertFromImage( qembed_findImage( QString::fromLatin1( name ) ) );
#else
    QPixmap pix = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/")+QString::fromLatin1(name) +
                                                       QString::fromLatin1(".png") ), KIcon::Toolbar );
#endif

  DoubleClickButton* but = new DoubleClickButton( pix, this, "RegExpButtons::but");

  _mapper->setMapping( but, tp );

  connect( but, SIGNAL( clicked() ), _mapper, SLOT( map() ) );
  connect( but, SIGNAL( clicked() ), this, SLOT( slotSetNonKeepMode() ) );
  connect( but, SIGNAL( doubleClicked() ), this, SLOT( slotSetKeepMode() ) );

  _grp->insert( but );
  but->setToggleButton( true );
  QToolTip::add( but, tooltip );
  but->setWhatsThis( whatsthis );

  return but;
}

void RegExpButtons::slotUnSelect()
{
  if ( _grp->selected() ) {
    QToolButton *pb = static_cast<QToolButton*>(_grp->selected());
    if (pb) {
      pb->setOn( false );
    }
  }
}

void RegExpButtons::slotSetKeepMode( )
{
  _keepMode = true;
}

void RegExpButtons::slotSetNonKeepMode( )
{
  _keepMode = false;
}

void RegExpButtons::slotSelectNewAction()
{
  if ( ! _keepMode ) {
    emit doSelect();
    _grp->setButton(_grp->id(_selectBut));
  }
}

void RegExpButtons::setFeatures( int features )
{
    _wordBoundary->setShown( features & RegExpConverter::WordBoundary );
    _nonWordBoundary->setShown( features & RegExpConverter::NonWordBoundary );
    _posLookAhead->setShown( features & RegExpConverter::PosLookAhead );
    _negLookAhead->setShown( features & RegExpConverter::NegLookAhead );

}

