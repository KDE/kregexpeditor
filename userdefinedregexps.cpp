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

UserDefinedRegExps::UserDefinedRegExps( QWidget *parent, const char *name )
  : QDockWindow( QDockWindow::InDock, parent, name)
{
  _userDefined = new QListView( this, "UserDefinedRegExps::_userDefined" );
  _userDefined->addColumn( QString::null );
  _userDefined->header()->hide();
  setWidget( _userDefined );
  slotPopulateUserRegexps();
  
  connect( _userDefined, SIGNAL(clicked(QListViewItem*)), this, SLOT(slotLoad(QListViewItem*)) );
  connect( _userDefined, SIGNAL(rightButtonPressed(QListViewItem*,const QPoint&, int )),
           this, SLOT( slotEdit( QListViewItem*, const QPoint& ) ) );
}

void UserDefinedRegExps::slotPopulateUserRegexps()
{
  _userDefined->clear();
  _regExps.clear();
  QDir directory( WidgetWinItem::path());
  QStringList files = directory.entryList( QString::fromLocal8Bit("*.regexp") );
  for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
    QString fileName = WidgetWinItem::path() + QString::fromLocal8Bit("/") + *it;

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

    new WidgetWinItem( *it, regexp, _userDefined );

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
        int answer = KMessageBox::warningYesNo( this, i18n("Override named regular expression <b>%1</b>").arg(newName) );
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

WidgetWinItem::WidgetWinItem( QString fileName, RegExp* regexp, QListView* parent ) :QListViewItem( parent ), _regexp( regexp )
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
