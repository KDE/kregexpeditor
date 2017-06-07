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

#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QList>

class CompoundRegExp;
class QPoint;
class RegExp;

class UserDefinedRegExps : public QDockWidget
{
    Q_OBJECT

public:
    explicit UserDefinedRegExps(QWidget *parent, const QString &title);
    const QList<CompoundRegExp *> regExps() const;

public Q_SLOTS:
    void slotSelectNewAction();

protected Q_SLOTS:
    void slotLoad(QTreeWidgetItem *item);
    void slotContextMenuTriggered(const QPoint &pos);
    void slotPopulateUserRegexps();
    void slotUnSelect();
    void slotRenameUserRegexp();
    void slotDeleteUserRegexp();

protected:
    void createItems(const QString &title, const QString &dir, bool usersRegExp);

Q_SIGNALS:
    void load(RegExp *);

private:
    QTreeWidget *_userDefined;
    QList<CompoundRegExp *> _regExps;
};

class WidgetWinItem : public QTreeWidgetItem
{
public:
    WidgetWinItem(const QString &name, RegExp *regexp, bool users, QTreeWidgetItem *parent);
    ~WidgetWinItem();
    static QString path();

    QString fileName() const;
    RegExp *regExp() const;
    QString name() const;
    void setName(const QString &);
    bool isUsersRegExp() const
    {
        return _usersRegExp;
    }

private:
    QString _name;
    RegExp *_regexp;
    bool _usersRegExp;
};

#endif // __USERDEFINEDREGEXPS_H
