/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef __USERDEFINEDREGEXPS_H
#define __USERDEFINEDREGEXPS_H

#include <QDockWidget>
#include <QList>
#include <QTreeWidgetItem>

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
    QTreeWidget *_userDefined = nullptr;
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
    RegExp *_regexp = nullptr;
    bool _usersRegExp;
};

#endif // __USERDEFINEDREGEXPS_H
