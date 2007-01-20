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
#ifdef QT_ONLY
#include "compat.h"
#include <q3filedialog.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <QBoxLayout>
#include "images.h"
#else
#include <klocale.h>
#include "verifybuttons.moc"
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#endif

#include "verifybuttons.h"
#include <QToolTip>
#include <QLayout>

#include "qtregexpconverter.h"
#include "emacsregexpconverter.h"
#include <qtoolbutton.h>
#include "util.h"
#include <q3popupmenu.h>
#include <QActionGroup>
#include <qaction.h>

VerifyButtons::VerifyButtons( QWidget* parent, const char* name )
    :Q3DockWindow( Q3DockWindow::InDock, parent, name ), _configMenu( 0 )
{
    QBoxLayout* layout = boxLayout();

    _verify =  new QToolButton(this);
    QIcon icon = Util::getSystemIconSet( QString::fromLatin1("spellcheck"));
    _verify->setIcon( icon );
    _verify->setToolTip( i18n( "Verify regular expression" ) );
    _verify->setWhatsThis( i18n("Shows what part of the regular expression is being matched in the <i>verifier window</i>."
                                   "(The window below the graphical editor window)."));
    layout->addWidget( _verify );
    connect( _verify, SIGNAL( clicked() ), this, SIGNAL( verify() ) );

    QToolButton* button = new QToolButton(this);
    button->setIcon(static_cast<QIcon>( Util::getSystemIcon( QString::fromLatin1("fileopen")) ));
    layout->addWidget( button );
    connect(button, SIGNAL(clicked()), this, SLOT(loadText()));
    button->setToolTip( i18n("Load text in the verifier window") );

    button = new QToolButton(this);
    button->setIcon(static_cast<QIcon>( Util::getSystemIcon( QString::fromLatin1("package_settings")) ));
    layout->addWidget( button );
    connect(button, SIGNAL(clicked()), this, SLOT(configure()));
    button->setToolTip( i18n("Verification Settings") );

    // It is currently not possible to ask for the paragraph being highlighted, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // _first = new QToolButton( QString::fromLatin1("<<"), this);
    // layout->addWidget( _first );
    // connect(_first, SIGNAL(clicked()), this, SIGNAL( gotoFirst()));
    // _first->setFixedWidth( 25 );
    //
    // _prev = new QToolButton(QString::fromLatin1("<"), this);
    // layout->addWidget( _prev );
    // connect(_prev, SIGNAL(clicked()), this, SIGNAL( gotoPrev()));
    // _prev->setFixedWidth( 20 );
    //
    // _next = new QToolButton(QString::fromLatin1(">"), this);
    // layout->addWidget( _next );
    // connect(_next, SIGNAL(clicked()), this, SIGNAL( gotoNext()));
    // _next->setFixedWidth( 20 );
    //
    // _last = new QToolButton(QString::fromLatin1(">>"), this);
    // layout->addWidget( _last );
    // connect(_last, SIGNAL(clicked()), this, SIGNAL( gotoLast()));
    // _last->setFixedWidth( 25 );

    // Same as above
//  QLabel* label = new QLabel( i18n("Matches: "), this );
//  layout->addWidget( label );
//  _matches = new QLabel(i18n("-"), this );
//  layout->addWidget( _matches );
//  QString txt = i18n( "Shows number of times regular expression matches the text in the verifier window");
//  label->setToolTip( txt );
//  _matches->setToolTip( txt );

    _verify->setEnabled( false );

    // -------------------------------------------------- RegExp Converters

    // Qt
    RegExpConverter* converter = new QtRegExpConverter();
    _converters.append( qMakePair( converter, static_cast<QAction*>( 0 ) ) );
    QString qtConverterName = converter->name();

    // Emacs
    converter = new EmacsRegExpConverter();
    _converters.append( qMakePair( converter, static_cast<QAction*>( 0 )  ) );


    // -------------------------------------------------- Initialize the config menu
    _configMenu = new Q3PopupMenu( this, "config menu" );

    // Auto Verify
    QAction* autoVerify = new QAction( i18n("Verify on the Fly"), this );
    autoVerify->setCheckable( true );
    autoVerify->setChecked( true );
    connect( autoVerify, SIGNAL( toggled( bool ) ), this, SLOT( updateVerifyButton( bool ) ) );
    connect( autoVerify, SIGNAL( toggled( bool ) ), this, SIGNAL( autoVerify( bool ) ) );
    autoVerify->addTo( _configMenu );
    autoVerify->setToolTip( i18n( "Toggle on-the-fly verification of regular expression" ) );
    autoVerify->setWhatsThis( i18n( "Enabling this option will make the verifier update for each edit. "
                                    "If the verify window contains much text, or if the regular expression is either "
                                    "complex or matches a lot of time, this may be very slow."));

    QAction* matchGreedy = new QAction( i18n("Match Greedy"), this );
    matchGreedy->setCheckable( true );
    matchGreedy->setChecked( false );
    connect( matchGreedy, SIGNAL( toggled( bool ) ), this, SIGNAL( matchGreedy( bool ) ) );
    matchGreedy->addTo( _configMenu );
    matchGreedy->setToolTip( i18n("Toggle greedy matching when verifying the regular expression.") );
    matchGreedy->setWhatsThis( i18n( "When this option is enabled, the regular expression will be evaluated on a so-called greedy way." ) );

    // RegExp Languages
    Q3PopupMenu* languages = new Q3PopupMenu( _configMenu );
    _languageId = _configMenu->insertItem( i18n("RegExp Language"), languages );

    QActionGroup* grp = new QActionGroup( this );
    for( Q3ValueList< QPair<RegExpConverter*,QAction*> >::Iterator it = _converters.begin(); it != _converters.end(); ++it ) {
        QString name = (*it).first->name();
        QAction* action = new QAction( name, this );
        action->setCheckable( true );
        grp->addAction( action );
        (*it).second = action;
    }
    grp->addTo( languages );
    connect( grp, SIGNAL( selected( QAction* ) ), this, SLOT( slotChangeSyntax( QAction* ) ) );
    _configMenu->setItemEnabled( _languageId, false );

    // Select the Qt converter by default
    setSyntax( qtConverterName );
}



void VerifyButtons::updateVerifyButton( bool b )
{
    _verify->setEnabled( !b );
}

void VerifyButtons::loadText()
{
    QString fileName = KFileDialog::getOpenFileName(KUrl(), QString::null, this);
    if ( !fileName.isNull() ) {
        emit loadVerifyText( fileName );
    }
}

// Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
// do not work. Enable this when they work.
// void VerifyButtons::enableBackwardButtons( bool b )
// {
//     _first->setEnabled( b );
//     _prev->setEnabled( b );
// }
//
// void VerifyButtons::enableForwardButtons( bool b )
// {
//     _next->setEnabled( b );
//     _last->setEnabled( b );
// }

void VerifyButtons::setMatchCount( int /*count*/ )
{
// currently this is not possible due to limitation in QSyntaxHighlighter
/*
  if ( count == -1 )
  _matches->setText( QString::fromLatin1("-") );
  else
  _matches->setText( QString::number( count ) );
*/
}

void VerifyButtons::slotChangeSyntax( QAction* action )
{
    emit changeSyntax( action->text()  );
}

RegExpConverter* VerifyButtons::setSyntax( const QString& which)
{
    for( Q3ValueList< QPair<RegExpConverter*, QAction*> >::Iterator it = _converters.begin(); it != _converters.end(); ++it ) {
        QString name = (*it).first->name();
        if ( name == which ) {
            (*it).second->setChecked( true );
            return (*it).first;
        }
    }
    qWarning( "No such converter: '%s'", qPrintable(which) );
    return 0;
}

void VerifyButtons::configure()
{
    _configMenu->exec( QCursor::pos() );
}

void VerifyButtons::setAllowNonQtSyntax( bool b )
{
    _configMenu->setItemEnabled( _languageId, b );
}
