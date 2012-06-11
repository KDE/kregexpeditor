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

#include "userdefinedregexps.h"

#include <QMenu>
#include <QDir>
#include <QTextStream>
#include <QVBoxLayout>
#include <QAction>
#include <QHeaderView>

#include <KInputDialog>
#include <KLocale>
#include <KMessageBox>
#include <KStandardDirs>

#include "widgetfactory.h"
#include "compoundregexp.h"

UserDefinedRegExps::UserDefinedRegExps( QWidget *parent, const QString &title )
  : QDockWidget(title, parent)
{
  QWidget* top = new QWidget( this );
  QVBoxLayout* lay = new QVBoxLayout( top );
  lay->setSpacing( 6 );

  //QLabel* label = new QLabel( i18n("Compound regular expression:"), top );

  // This is to avoid that the label set the minimum width for the window.
  //label->setMinimumSize(1,0);
  //lay->addWidget(label);

  _userDefined = new QTreeWidget( top/*, "UserDefinedRegExps::_userDefined"*/ );
  //_userDefined->addColumn( QString() );
  _userDefined->header()->hide();
  _userDefined->setRootIsDecorated( true );
  _userDefined->setContextMenuPolicy(Qt::CustomContextMenu);
  lay->addWidget(_userDefined);
  setWidget( top );
  slotPopulateUserRegexps();

  connect( _userDefined, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(slotLoad(QTreeWidgetItem*)) );
  connect( _userDefined, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotContextMenuTriggered(const QPoint&)));
}

void UserDefinedRegExps::slotPopulateUserRegexps()
{
  _userDefined->clear();
  _regExps.clear();

  createItems( i18n("User Defined"), WidgetWinItem::path(), true );

  QStringList dirs = KGlobal::dirs()->findDirs( "data", QString::fromLocal8Bit("kregexpeditor/predefined/") );

  for ( QStringList::iterator it1 = dirs.begin(); it1 != dirs.end(); ++it1 ) {
    QDir dir( *it1, QString(), QDir::Name, QDir::Dirs );
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

  QTreeWidgetItem* lvItem = new QTreeWidgetItem( (QTreeWidget*)0, QStringList(title) );
  lvItem->setExpanded( true );
  _userDefined->addTopLevelItem(lvItem);

  QDir directory( dir );
  QStringList files = directory.entryList( QStringList(QString::fromLocal8Bit("*.regexp")) );
  for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
    QString fileName = dir + QString::fromLocal8Bit("/") + *it;

    QFile file( fileName );
    if ( ! file.open(QIODevice::ReadOnly) ) {
      KMessageBox::sorry( this, i18n("Could not open file for reading: %1", fileName) );
      continue;
    }

    QTextStream stream( &file );
    QString data = stream.readAll();
    file.close();

    RegExp* regexp = WidgetFactory::createRegExp( data );
    if ( ! regexp ) {
      KMessageBox::sorry( this, i18n("File %1 containing user defined regular expression contained an error", fileName ) );
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

const QList<CompoundRegExp *> UserDefinedRegExps::regExps() const
{
  return _regExps;
}


void UserDefinedRegExps::slotUnSelect()
{
  _userDefined->clearSelection();
}

void UserDefinedRegExps::slotLoad(QTreeWidgetItem* item)
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

void UserDefinedRegExps::slotContextMenuTriggered( const QPoint& pos )
{
  QMenu menu;
  QAction *deleteAction = menu.addAction(i18n("Delete"), this, SLOT(slotDeleteUserRegexp()) );
  QAction *renameAction = menu.addAction(i18n("Rename"), this, SLOT(slotRenameUserRegexp()) );
  
  QTreeWidgetItem* item = _userDefined->itemAt(pos);
  
  if ( !item || ! dynamic_cast<WidgetWinItem *>( item ) ) {
    // menu not selected on an item
    deleteAction->setEnabled(false);
    renameAction->setEnabled(false);
  }
  else {
    // Only allow rename and delete of users own regexps.
    WidgetWinItem* winItem = dynamic_cast<WidgetWinItem *>( item );
    if ( winItem ) {
      if ( ! winItem->isUsersRegExp() ) {
        deleteAction->setEnabled(false);
        renameAction->setEnabled(false);
      }
      else {
        QVariant var = QVariant::fromValue<void *>((void *)(winItem));
        deleteAction->setData(var);
        renameAction->setData(var);
      }
    }
  }
  
  menu.exec( _userDefined->mapToGlobal(pos) );
}

void UserDefinedRegExps::slotSelectNewAction()
{
  slotUnSelect();
}

void UserDefinedRegExps::slotRenameUserRegexp() 
{
    QAction* const action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);
    
    WidgetWinItem* winItem = static_cast<WidgetWinItem *>(action->data().value<void *>());
    Q_ASSERT( winItem );

    QString oldFile = winItem->fileName();
    QString oldName = winItem->name();

    QString txt;
    txt = KInputDialog::getText( i18n("Rename Item"), i18n("New name:"), oldName );
    
    if ( !txt.isNull() && oldName != txt ) {
      QString fileName = WidgetWinItem::path() + QString::fromLocal8Bit("/") + txt + QString::fromLocal8Bit(".regexp");
      QFileInfo finfo( fileName );
      if ( finfo.exists() ) {
        int answer = KMessageBox::warningYesNo( this, i18n("<p>Overwrite named regular expression <b>%1</b>?</p>", txt), QString(), KStandardGuiItem::overwrite(), KGuiItem(i18n("Do Not Overwrite")) );
        if ( answer != KMessageBox::Yes )
          return;

        // An item with this name already exists.
        delete winItem;
      }
      else
        winItem->setName( txt );
      QDir dir;
      dir.remove(fileName);
    }
}

void UserDefinedRegExps::slotDeleteUserRegexp() 
{
    QAction* const action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);
    
    WidgetWinItem* winItem = static_cast<WidgetWinItem *>(action->data().value<void *>());
    Q_ASSERT( winItem );
    
    QFile file( winItem->fileName() );
    Q_ASSERT( file.exists() );
    
    file.remove();
    delete winItem;
}

WidgetWinItem::WidgetWinItem( QString fileName, RegExp* regexp, bool usersRegExp, QTreeWidgetItem* parent )
  :QTreeWidgetItem( parent ), _regexp( regexp ), _usersRegExp ( usersRegExp )
{
  int index = fileName.lastIndexOf(QLatin1String(".regexp"));
  _name = fileName.left(index);

  setText( 0, _name );
}

QString WidgetWinItem::fileName() const
{
  return path() + QLatin1Char('/') +_name + QLatin1String(".regexp");
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
    return KStandardDirs::locateLocal("data", QString::fromLocal8Bit("KRegExpEditor/"));
}

#include "userdefinedregexps.moc"
