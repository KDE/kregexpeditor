#include "widgetwindow.h"
#include "drag.h"
#include <iostream>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qsignalmapper.h>
#include <qheader.h>
#include "regexp.h"
#include "flow.h"

RegExpWidgetWindow::RegExpWidgetWindow( QWidget *parent, const char *name )
  : QWidget(parent, name), _keepMode(false)
{
  QVBoxLayout* topLayout = new QVBoxLayout(this);
  SimpleFlow *flowLayout = new SimpleFlow( topLayout );
  
  _grp = new QButtonGroup(this);
  _grp->hide();
  _grp->setExclusive( true );
  
  _mapper = new QSignalMapper( this, "RegExpWidgetWindow::_mapper" );  
  connect( _mapper, SIGNAL( mapped(int) ), this, SIGNAL( clicked(int) ) );

  // The "select" button.
  _selectBut = new QPushButton( this);
  QPixmap pix = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/select.png") ), KIcon::Toolbar );
  _selectBut->setPixmap( pix );
  flowLayout->add( _selectBut );
  _grp->insert(_selectBut);
  _selectBut->setToggleButton( true );
  connect( _selectBut, SIGNAL(clicked()), SIGNAL(doSelect()));
  connect( _selectBut, SIGNAL(clicked()), this, SLOT(slotSetNonKeepMode()) );
  
  QToolTip::add( _selectBut, i18n("Selection tool"));
  QWhatsThis::add( _selectBut, i18n("<qml>This will change the state of the editor to <i>selection state</i>.<p>"
                             "In this state you will not be inserting <i>regexp items</i>, but instead select them. "
                             "To select a number of items, press down the left mouse button and drag it over the items.<p>"
                             "When you have selected a number of items, you may use cut/copy/paste. These functions are "
                             "found in the right mouse button menu.</qml>"));

  // Insert buttons.
  DoubleClickButton* but;
  
  but = insert(TEXT, "text", i18n("Text"), 
               i18n( "<qml>This will insert a text field, where you may write text. The text you write will "
                     "be matched literally. (I.e. you do not need to escape any characters)</qml>" ) );
  flowLayout->add( but );


  but = insert(CHARSET, "characters", i18n("A single character specified in a range"), 
               i18n("<qml>This will match a single character from a predefined range.<p>"
                    "When you insert this widget a dialog box will appear, which lets you specify "
                    "which characters this <i>regexp item</i> will match.</qml>") );
  flowLayout->add( but );


  but = insert(DOT, "anychar", i18n("Any character"), 
               i18n("<qml>This will match any single character</qml>") );
  flowLayout->add( but );

  
  but = insert(REPEAT, "repeat", i18n("Repeated content"), 
               i18n("<qml>This <i>regexp item</i> will repeat the <i>regexp items</i> it surrounds "
                    "a number of times you specify.<p>"
                    "The number of times to repeat may be specified using ranges. You may e.g. say "
                    "that it should match from 2 to 4 times, that it should match exactly 5 times, or "
                    "that it should match at least one time.<p>"
                    "Examples:<br>"
                    "If you specify that it should match <i>any</i> time, and the content it surrounds "
                    "is <tt>abc</tt>, then this <i>regexp item</i> will match the empty string, "
                    "the string <tt>abc</tt>, the string <tt>abcabc</tt>, the string <tt>abcabcabcabc</tt> "
                    "etc.</qml>") );
  flowLayout->add( but );


  but = insert(ALTN, "altn", i18n("Alternatives"), 
               i18n("<qml>This <i>regexp item</i> will match any of its alternatives.</p>"
                    "You specify alternatives by placing <i>regexp items</i> on top of "
                    "each other inside this widget.</qml>") );
  flowLayout->add( but );


  but = insert(COMPOUND,  "compound", i18n("Compound regexp"), 
               i18n("<qml>This <i>regexp item</i> serves two purposes:"
                    "<ul><li>It makes it possible for you to collapse a huge <i>regexp item</i> into "
                    "a small box. This makes it easier for you to get an overview of large "
                    "<i>regexp items</i>. This is especially useful if you load an pre-made <i>regexp item</i> "
                    "you perhaps don't care about the inner working of."
                    "<li>The whole <i>regexp item</i> may be referenced in a <i>replace widget</i>. "
                    "Imagine that this regular expression editor is used for search and replace, then "
                    "you may inserted all the text match by this <i>regexp item</i> "
                    "in the <i>replace</i> widget.</ul></qml>") );
  flowLayout->add( but );

  
  but = insert(BEGLINE,  "begline", i18n("Beginning of line"), 
               i18n("<qml>This will match the beginning of a line.</qml>") );
  flowLayout->add( but );


  but = insert(ENDLINE,  "endline", i18n("End of line"), 
               i18n("<qml>This will match the end of a line.</qml>") );
  flowLayout->add( but );


  but = insert(WORDBOUNDARY,  "wordboundary", i18n("Word boundary"), 
               i18n("<qml>This asserts a word boundary (This part do not actually match any characters)</qml>") );
  flowLayout->add( but );
  
  but = insert(NONWORDBOUNDARY,  "nonwordboundary", i18n("Non Word boundary"), 
               i18n("<qml>This asserts a non-word boundary (This part do not actually match any characters)</qml>") );
  flowLayout->add( but );

  but = insert(POSLOOKAHEAD,  "poslookahead", i18n("Positive Assertion"), 
               i18n("<qml>This asserts a regular expression (This part do not actually match any characters)</qml>") );
  flowLayout->add( but );

  but = insert(NEGLOOKAHEAD,  "neglookahead", i18n("Negatove Assertion"), 
               i18n("<qml>This asserts a regular expression that must not match (This part do not actually match any characters)</qml>") );
  flowLayout->add( but );

  // The whatsthis button
  KIconLoader loader;
  QPixmap whatsThisIM = loader.loadIcon(QString::fromLatin1("contexthelp"), KIcon::Toolbar);
  QPushButton* whatsThisBU = new QPushButton(this, "whatsThisLA");
  whatsThisBU->setFocusPolicy(NoFocus);

  connect(whatsThisBU, SIGNAL(clicked()), this, SLOT(slotEnterWhatsThis()));
  whatsThisBU->setPixmap( whatsThisIM );
  flowLayout->add(whatsThisBU );

  // The listview with loadable regexp
  _userDefined = new QListView( this );
  _userDefined->addColumn( QString::null );
  _userDefined->header()->hide();
  topLayout->addWidget( _userDefined );
  slotPopulateUserRegexps();
  connect( _userDefined, SIGNAL(clicked(QListViewItem*)), this, SLOT(slotLoad(QListViewItem*)) );
}

void RegExpWidgetWindow::slotPopulateUserRegexps()
{
  _userDefined->clear();
  QString dir = locateLocal("data", QString::fromLocal8Bit("KRegExpEditor/"));
  QDir directory(dir);
  QStringList files = directory.entryList( QString::fromLocal8Bit("*.regexp") );
  for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
    new WidgetWinItem( dir, *it, _userDefined );
  }
}


void RegExpWidgetWindow::slotLoad(QListViewItem* item)
{
  if ( !item || ! dynamic_cast<WidgetWinItem*>(item) ) {
    // Mouse pressed outside a widget.
    return;
  }
  
  QString fileName = dynamic_cast<WidgetWinItem*>(item)->fileName();

  QFile file( fileName );
  if ( ! file.open(IO_ReadOnly) ) {
    KMessageBox::sorry( this, i18n("Could not open file for reading: %1").arg(fileName) );
    return;
  }

  // Deselect any of the buttons.
  _grp->setButton( _grp->id(_selectBut) );
  _selectBut->setOn( false );

  QTextStream stream( &file );
  QString data = stream.read();
  file.close();

  RegExp* regexp = WidgetFactory::createRegExp( data );
  if ( regexp ) {
    emit load( regexp );
    delete regexp;
  }
}

DoubleClickButton* RegExpWidgetWindow::insert(RegExpType tp, const char* name, QString tooltip, QString whatsthis)
{  
  QPixmap pix = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/")+QString::fromLatin1(name) +
                                                       QString::fromLatin1(".png") ), KIcon::Toolbar );
  DoubleClickButton* but = new DoubleClickButton( pix, this, "RegExpWidgetWindow::but");
  
  _mapper->setMapping( but, tp );

  connect( but, SIGNAL( clicked() ), _mapper, SLOT( map() ) );
  connect( but, SIGNAL( clicked() ), this, SLOT( slotSetNonKeepMode() ) );
  connect( but, SIGNAL( doubleClicked() ), this, SLOT( slotSetKeepMode() ) );
  
  _grp->insert( but );
  but->setToggleButton( true );
  QToolTip::add( but, tooltip );
  QWhatsThis::add( but, whatsthis );
  
  return but;
}

void RegExpWidgetWindow::slotEnterWhatsThis() 
{
  QWhatsThis::enterWhatsThisMode ();
}

void RegExpWidgetWindow::slotSetKeepMode( )
{
  _keepMode = true;
  _userDefined->clearSelection();
}

void RegExpWidgetWindow::slotSetNonKeepMode( )
{
  _keepMode = false;
  _userDefined->clearSelection();
}

void RegExpWidgetWindow::slotSelectNewAction()
{
  if ( ! _keepMode ) {
    emit doSelect();
    _grp->setButton(_grp->id(_selectBut));
    _userDefined->clearSelection();
  }
}

WidgetWinItem::WidgetWinItem( QString path, QString fileName, QListView* parent ) :QListViewItem( parent )
{
  int index = fileName.findRev(QString::fromLocal8Bit(".regexp"));
  QString txt = fileName.left(index);
  setText( 0, txt );
  _fileName = path + QString::fromLocal8Bit("/") + fileName;
}

QString WidgetWinItem::fileName() const 
{
  return _fileName;
}


#include "widgetwindow.moc"
