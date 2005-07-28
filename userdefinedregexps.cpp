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
#else
  #include <klineeditdlg.h>
  #include <klocale.h>
  #include <kmessagebox.h>
  #include <kstandarddirs.h>
  #include <kdebug.h>
  #include "userdefinedregexps.moc"
#endif

#include "userdefinedregexps.h"
#include <q3header.h>
#include <q3popupmenu.h>
#include <qdir.h>
//Added by qt3to4:
#include <QTextStream>
#include <Q3PtrList>
#include <QVBoxLayout>
#include "widgetfactory.h"
#include "compoundregexp.h"
#include <qlayout.h>
#include <qlabel.h>

UserDefinedRegExps::UserDefinedRegExps( QWidget *parent, const char *name )
  : Q3DockWindow( Q3DockWindow::InDock, parent, name)
{
  QWidget* top = new QWidget( this );
  QVBoxLayout* lay = new QVBoxLayout( top, 6 );
  lay->setAutoAdd( true );

  QLabel* label = new QLabel( i18n("Compound regular expression:"), top );

  // This is to avoid that the label set the minimum width for the window.
  label->setMinimumSize(1,0);

  _userDefined = new Q3ListView( top, "UserDefinedRegExps::_userDefined" );
  _userDefined->addColumn( QString::null );
  _userDefined->header()->hide();
  //  _userDefined->setRootIsDecorated( true );
  setWidget( top );
  slotPopulateUserRegexps();

  connect( _userDefined, SIGNAL(clicked(Q3ListViewItem*)), this, SLOT(slotLoad(Q3ListViewItem*)) );
  connect( _userDefined, SIGNAL(rightButtonPressed(Q3ListViewItem*,const QPoint&, int )),
           this, SLOT( slotEdit( Q3ListViewItem*, const QPoint& ) ) );
}

void UserDefinedRegExps::slotPopulateUserRegexps()
{
  _userDefined->clear();
  _regExps.clear();

  createItems( i18n("User Defined"), WidgetWinItem::path(), true );

#ifdef QT_ONLY
  QStringList dirs;
  dirs << QString::fromLatin1( "predefined" );
#else
  QStringList dirs = KGlobal::dirs()->findDirs( "data", QString::fromLocal8Bit("kregexpeditor/predefined/") );
#endif

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

  Q3ListViewItem* lvItem = new Q3ListViewItem( _userDefined, title );
  lvItem->setOpen( true );

  QDir directory( dir );
  QStringList files = directory.entryList( QString::fromLocal8Bit("*.regexp") );
  for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
    QString fileName = dir + QString::fromLocal8Bit("/") + *it;

    QFile file( fileName );
    if ( ! file.open(QIODevice::ReadOnly) ) {
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
      if ( cregexp && cregexp->allowReplace() )
        _regExps.append( cregexp );
    }
  }
}

const Q3PtrList<CompoundRegExp> UserDefinedRegExps::regExps() const
{
  return _regExps;
}


void UserDefinedRegExps::slotUnSelect()
{
  _userDefined->clearSelection();
}

void UserDefinedRegExps::slotLoad(Q3ListViewItem* item)
{
  if ( !item || ! dynamic_cast<WidgetWinItem*>(item) ) {
    // Mouse pressed outside a widget.
    return;
  }

  WidgetWinItem* wwi = dynamic_cast<WidgetWinItem*>(item);
  if (wwi) {
    emit load( wwi->regExp() );
  }
}

void UserDefinedRegExps::slotEdit( Q3ListViewItem* item, const QPoint& pos )
{
  Q3PopupMenu* menu = new Q3PopupMenu( this );
  menu->insertItem(i18n("Delete"), 1 );
  menu->insertItem(i18n("Rename..."), 2 );
  if ( !item || ! dynamic_cast<WidgetWinItem*>( item ) ) {
    // menu not selected on an item
    menu->setItemEnabled( 1, false );
    menu->setItemEnabled( 2, false );
  }
  else {
    // Only allow rename and delete of users own regexps.
    WidgetWinItem* winItem = dynamic_cast<WidgetWinItem*>( item );
    if ( winItem && ! winItem->isUsersRegExp() ) {
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

    QString oldFile = winItem->fileName();
    QString oldName = winItem->name();

    QString txt;
#ifdef QT_ONLY
    txt = QInputDialog::getText( tr("Rename Regular Expression"), tr("New name:") );
#else
    KLineEditDlg dlg(i18n("New name:"), oldName, this);
    dlg.setCaption(i18n("Rename Item"));
    bool ok = dlg.exec();
    if ( ok )
        txt = dlg.text();
#endif
    if ( !txt.isNull() && oldName != txt ) {
      QString fileName = WidgetWinItem::path() + QString::fromLocal8Bit("/") + txt + QString::fromLocal8Bit(".regexp");
      QFileInfo finfo( fileName );
      if ( finfo.exists() ) {
        int answer = KMessageBox::warningYesNo( this, i18n("<p>Overwrite named regular expression <b>%1</b>?</p>").arg(txt), QString::null, i18n("Overwrite"), i18n("Do Not Overwrite") );
        if ( answer != KMessageBox::Yes )
          return;

        // An item with this name already exists.
        delete winItem;
      }
      else
        winItem->setName( txt );
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

WidgetWinItem::WidgetWinItem( QString fileName, RegExp* regexp, bool usersRegExp, Q3ListViewItem* parent )
  :Q3ListViewItem( parent ), _regexp( regexp ), _usersRegExp ( usersRegExp )
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
#ifdef QT_ONLY
    return QString::fromLatin1( "predefined" );
#else
  return locateLocal("data", QString::fromLocal8Bit("KRegExpEditor/"));
#endif
}



