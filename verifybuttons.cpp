#include "verifybuttons.h"
#include "verifybuttons.moc"
#include <klocale.h>
#include <kstandarddirs.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <kmessagebox.h>
#include <qpushbutton.h>
#include <kiconloader.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qwhatsthis.h>

VerifyButtons::VerifyButtons( QWidget* parent, const char* name ) :QDockWindow( QDockWindow::InDock, parent, name )
{
  QBoxLayout* layout = boxLayout();
  KIconLoader loader;

  _autoVerify = new QPushButton(this);
  _autoVerify->setToggleButton( true );
  QPixmap icon = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/autoverify.png") ),
                                                 KIcon::Toolbar );
  _autoVerify->setPixmap( icon );
  QToolTip::add( _autoVerify, i18n( "Toggle on-the-fly verification of regular expression" ) );
  QWhatsThis::add( _autoVerify, i18n( "Enabling this option will make the verifier update for each edit."
                                      "If the verify window contains much text, or if the regular expression is either"
                                      "complex or matches a lot of time, this may be very slow."));
  layout->addWidget( _autoVerify );
  connect( _autoVerify, SIGNAL( toggled( bool ) ), this, SIGNAL( autoVerify( bool ) ) );

  _verify =  new QPushButton(this);
  icon = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/verify.png") ), KIcon::Toolbar );
  _verify->setPixmap( icon );
  QToolTip::add( _verify, i18n( "Verify regular expression" ) );
  QWhatsThis::add( _verify, i18n("Shows what part of the regular expression is being matches in the <i>verifier window</i>."
                                  "(The window below the graphical editor window)."));
  layout->addWidget( _verify );
  connect( _autoVerify, SIGNAL( toggled( bool ) ), this, SLOT( updateVerifyButton( bool ) ) );
  connect( _verify, SIGNAL( clicked() ), this, SIGNAL( verify() ) );

  QPushButton* button = new QPushButton(this);
  button->setPixmap( loader.loadIcon(QString::fromLatin1("fileopen"), KIcon::Toolbar) );
  layout->addWidget( button );
  connect(button, SIGNAL(clicked()), this, SLOT(loadText()));
  QToolTip::add( button, i18n("Load text in the verifier window") );

  // It is currently not possible to ask for the paragraph being highlighted, thefore scrolling to next/prev match
  // do not work. Enable this when they work.
  // _first = new QPushButton( QString::fromLatin1("<<"), this);
  // layout->addWidget( _first );
  // connect(_first, SIGNAL(clicked()), this, SIGNAL( gotoFirst()));
  // _first->setFixedWidth( 25 );
  //
  // _prev = new QPushButton(QString::fromLatin1("<"), this);
  // layout->addWidget( _prev );
  // connect(_prev, SIGNAL(clicked()), this, SIGNAL( gotoPrev()));
  // _prev->setFixedWidth( 20 );
  //
  // _next = new QPushButton(QString::fromLatin1(">"), this);
  // layout->addWidget( _next );
  // connect(_next, SIGNAL(clicked()), this, SIGNAL( gotoNext()));
  // _next->setFixedWidth( 20 );
  //
  // _last = new QPushButton(QString::fromLatin1(">>"), this);
  // layout->addWidget( _last );
  // connect(_last, SIGNAL(clicked()), this, SIGNAL( gotoLast()));
  // _last->setFixedWidth( 25 );

  // Same as above
//  QLabel* label = new QLabel( i18n("Matches: "), this );
//  layout->addWidget( label );
//  _matches = new QLabel(i18n("-"), this );
//  layout->addWidget( _matches );
//  QString txt = i18n( "Shows number of times regular expression matches the text in the verifier window");
//  QToolTip::add( label, txt );
//  QToolTip::add( _matches, txt );

  _autoVerify->setOn( true );
//  _verify->setEnabled( false );
}



void VerifyButtons::updateVerifyButton( bool b )
{
    _verify->setEnabled( !b );
}

void VerifyButtons::loadText()
{
    QString fileName = QFileDialog::getOpenFileName(QString::null, QString::null, this, "VerifyButton::fileDialog");
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

void VerifyButtons::setMatchCount( int count )
{
// currently this is not possible due to limitation in QSyntaxHighlighter
/*
    if ( count == -1 )
        _matches->setText( QString::fromLatin1("-") );
    else
        _matches->setText( QString::number( count ) );
*/
}

