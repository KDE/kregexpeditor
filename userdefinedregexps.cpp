#include "userdefinedregexps.h"
#include <qheader.h>
#include <qpopupmenu.h>
#include <qinputdialog.h>
#include "regexp.h"
#include <kmessagebox.h>
#include <qdir.h>
#include <klocale.h>
#include "widgetfactory.h"
#include <kstandarddirs.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "compoundregexp.h"
#include <kdebug.h>
#include <qlayout.h>
#include <qlabel.h>

UserDefinedRegExps::UserDefinedRegExps( QWidget *parent, const char *name )
  : QDockWindow( QDockWindow::InDock, parent, name)
{
  QWidget* top = new QWidget( this );
  QVBoxLayout* lay = new QVBoxLayout( top, 6 );
  lay->setAutoAdd( true );

  QLabel* label = new QLabel( i18n("Compound Regular Expression:"), top );
  
  // This is to avoid that the label set the minimum width for the window.
  label->setMinimumSize(1,0);

  _userDefined = new QListView( top, "UserDefinedRegExps::_userDefined" );
  _userDefined->addColumn( QString::null );
  _userDefined->header()->hide();
  //  _userDefined->setRootIsDecorated( true );
  setWidget( top );
  slotPopulateUserRegexps();
  
  connect( _userDefined, SIGNAL(clicked(QListViewItem*)), this, SLOT(slotLoad(QListViewItem*)) );
  connect( _userDefined, SIGNAL(rightButtonPressed(QListViewItem*,const QPoint&, int )),
           this, SLOT( slotEdit( QListViewItem*, const QPoint& ) ) );
}

void UserDefinedRegExps::slotPopulateUserRegexps()
{
  _userDefined->clear();
  _regExps.clear();

  createItems( i18n("User Defined"), WidgetWinItem::path(), true );
  
  QStringList dirs = KGlobal::dirs()->findDirs( "data", QString::fromLocal8Bit("kregexpeditor/predefined/") );
  for ( QStringList::iterator it1 = dirs.begin(); it1 != dirs.end(); ++it1 ) {
    QDir dir( *it1, QString::null, QDir::Name, QDir::Dirs );
    QStringList subdirs = dir.entryList();
    for ( QStringList::iterator it2 = subdirs.begin(); it2 != subdirs.end(); ++it2 ) {
      if ( *it2 == QString::fromLocal8Bit(".") || *it2 == QString::fromLocal8Bit("..") )
        continue;
      createItems( *it2, *it1 + QString::fromLocal8Bit("/") + *it2, false );
    }
  }
  
}

void UserDefinedRegExps::createItems( const QString& _title, const QString& dir, bool usersRegExp ) 
{
  QString title = _title;
  if (_title == QString::fromLatin1("general"))
	  title = i18n("general");

  QListViewItem* lvItem = new QListViewItem( _userDefined, title );
  lvItem->setOpen( true );
  
  QDir directory( dir );
  QStringList files = directory.entryList( QString::fromLocal8Bit("*.regexp") );
  for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
    QString fileName = dir + QString::fromLocal8Bit("/") + *it;

    QFile file( fileName );
    if ( ! file.open(IO_ReadOnly) ) {
      KMessageBox::sorry( this, i18n("Could not open file for reading: %1").arg(fileName) );
      continue;
    }

    QTextStream stream( &file );
    QString data = stream.read();
    file.close();

    RegExp* regexp = WidgetFactory::createRegExp( data );
    if ( ! regexp ) {
      KMessageBox::sorry( this, i18n("File %1 containing user defined regular expression contained an error").arg( fileName ) );
      continue;
    }

    new WidgetWinItem( *it, regexp, usersRegExp, lvItem );

    // Inserth the regexp into the list of compound regexps
    if ( regexp->type() == RegExp::COMPOUND ) {
      CompoundRegExp* cregexp = dynamic_cast<CompoundRegExp*>( regexp );
      if ( cregexp->allowReplace() )
        _regExps.append( cregexp );
    }
  }
}

const QPtrList<CompoundRegExp> UserDefinedRegExps::regExps() const 
{
  return _regExps;
}


void UserDefinedRegExps::slotUnSelect()
{
  _userDefined->clearSelection();
}

void UserDefinedRegExps::slotLoad(QListViewItem* item)
{
  if ( !item || ! dynamic_cast<WidgetWinItem*>(item) ) {
    // Mouse pressed outside a widget.
    return;
  }
  
  RegExp* regexp = dynamic_cast<WidgetWinItem*>(item)->regExp();
  emit load( regexp );
}

void UserDefinedRegExps::slotEdit( QListViewItem* item, const QPoint& pos )
{
  QPopupMenu* menu = new QPopupMenu( this );
  menu->insertItem(i18n("Delete"), 1 );
  menu->insertItem(i18n("Rename"), 2 );
  if ( !item || ! dynamic_cast<WidgetWinItem*>( item ) ) {
    // menu not selected on an item
    menu->setItemEnabled( 1, false );
    menu->setItemEnabled( 2, false );
  }
  else {
    // Only allow rename and delete of users own regexps.
    WidgetWinItem* winItem = dynamic_cast<WidgetWinItem*>( item );
    if ( ! winItem->isUsersRegExp() ) {
      menu->setItemEnabled( 1, false );
      menu->setItemEnabled( 2, false );
    }
  }
  
  int which = menu->exec( pos );
  
  if ( which == 1 ) { // Delete
    WidgetWinItem* winItem = dynamic_cast<WidgetWinItem*>( item );
    Q_ASSERT( winItem );
    QFile file( winItem->fileName() );
    Q_ASSERT( file.exists() );
    file.remove();
    delete item;
  }
  else if ( which == 2 ) { // Rename
    WidgetWinItem* winItem = dynamic_cast<WidgetWinItem*>( item );
    Q_ASSERT( winItem );

    bool ok = false;
    QString oldFile = winItem->fileName();
    QString oldName = winItem->name();
    QString newName = QInputDialog::getText(i18n("Rename Item"), i18n("New Name"), QLineEdit::Normal, winItem->name(), &ok, this );
    if ( ok && oldName != newName ) {
      QString fileName = WidgetWinItem::path() + QString::fromLocal8Bit("/") + newName + QString::fromLocal8Bit(".regexp");
      QFileInfo finfo( fileName );
      if ( finfo.exists() ) {
        int answer = KMessageBox::warningYesNo( this, i18n("<p>Overwrite named regular expression <b>%1</b> ?</p>").arg(newName) );
        if ( answer != KMessageBox::Yes )
          return;

        // An item with this name already exists.
        delete winItem;
      }
      else 
        winItem->setName( newName );
      QDir dir;
      dir.rename( oldFile, fileName  );
    }
  }
  

  delete menu;
}

void UserDefinedRegExps::slotSelectNewAction()
{
  slotUnSelect();
}

WidgetWinItem::WidgetWinItem( QString fileName, RegExp* regexp, bool usersRegExp, QListViewItem* parent ) 
  :QListViewItem( parent ), _regexp( regexp ), _usersRegExp ( usersRegExp )
{
  int index = fileName.findRev(QString::fromLocal8Bit(".regexp"));
  _name = fileName.left(index);

  setText( 0, _name );
}

QString WidgetWinItem::fileName() const 
{
  return path() + QString::fromLocal8Bit("/") +_name + QString::fromLocal8Bit(".regexp");
}

RegExp* WidgetWinItem::regExp() const
{
  return _regexp;
}

QString WidgetWinItem::name() const 
{
  return _name;
}

void WidgetWinItem::setName( const QString& nm ) 
{
  _name = nm;
  setText( 0, nm );
}

QString WidgetWinItem::path() 
{
  return locateLocal("data", QString::fromLocal8Bit("KRegExpEditor/"));
}



#include "userdefinedregexps.moc"
