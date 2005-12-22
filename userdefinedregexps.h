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
#ifndef __USERDEFINEDREGEXPS_H
#define __USERDEFINEDREGEXPS_H
#include <qdockwindow.h>
#include <qlistview.h>

#include "compoundregexp.h"

class QPoint;
class RegExp;

class UserDefinedRegExps :public QDockWindow
{
Q_OBJECT

public:
  UserDefinedRegExps( QWidget *parent, const char *name = 0 );
  const QPtrList<CompoundRegExp> regExps() const;

public slots:
  void slotSelectNewAction();

protected slots:
  void slotLoad(QListViewItem* item);
  void slotEdit( QListViewItem* item, const QPoint& pos );
  void slotPopulateUserRegexps();
  void slotUnSelect();

protected:
  void createItems( const QString& title, const QString& dir, bool usersRegExp );

signals:
  void load( RegExp* );

private:
  QListView* _userDefined;
  QPtrList<CompoundRegExp> _regExps;
};

class WidgetWinItem :public QListViewItem
{
public:
  WidgetWinItem( QString name, RegExp* regexp, bool users, QListViewItem* parent );
  static QString path();

  QString fileName() const;
  RegExp* regExp() const;
  QString name() const;
  void setName( const QString& );
  bool isUsersRegExp() const { return _usersRegExp; };

private:
  QString _name;
  RegExp* _regexp;
  bool _usersRegExp;
};


#endif // __USERDEFINEDREGEXPS_H
