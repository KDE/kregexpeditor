/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "userdefinedregexps.h"

#include <QAction>
#include <QDir>
#include <QHeaderView>
#include <QMenu>
#include <QTextStream>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <KMessageBox>
#include <QInputDialog>
#include <QStandardPaths>

#include "compoundregexp.h"
#include "widgetfactory.h"

UserDefinedRegExps::UserDefinedRegExps(QWidget *parent, const QString &title)
    : QDockWidget(title, parent)
{
    QWidget *top = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(top);
    lay->setContentsMargins(0, 0, 0, 0);

    // QLabel* label = new QLabel( i18n("Compound regular expression:"), top );

    // This is to avoid that the label set the minimum width for the window.
    // label->setMinimumSize(1,0);
    // lay->addWidget(label);

    _userDefined = new QTreeWidget(top /*, "UserDefinedRegExps::_userDefined"*/);
    //_userDefined->addColumn( QString() );
    _userDefined->header()->hide();
    _userDefined->setRootIsDecorated(true);
    _userDefined->setContextMenuPolicy(Qt::CustomContextMenu);
    lay->addWidget(_userDefined);
    setWidget(top);
    slotPopulateUserRegexps();

    connect(_userDefined, &QTreeWidget::itemClicked, this, &UserDefinedRegExps::slotLoad);
    connect(_userDefined, &QWidget::customContextMenuRequested, this, &UserDefinedRegExps::slotContextMenuTriggered);
}

void UserDefinedRegExps::slotPopulateUserRegexps()
{
    _userDefined->clear();
    _regExps.clear();

    createItems(i18n("User Defined"), WidgetWinItem::path(), true);

    const QStringList dirs =
        QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("kregexpeditor/predefined/"), QStandardPaths::LocateDirectory);

    for (QStringList::ConstIterator it1 = dirs.constBegin(); it1 != dirs.constEnd(); ++it1) {
        QDir dir(*it1, QString(), QDir::Name, QDir::Dirs);
        QStringList subdirs = dir.entryList();
        for (QStringList::iterator it2 = subdirs.begin(); it2 != subdirs.end(); ++it2) {
            if (*it2 == QStringLiteral(".") || *it2 == QStringLiteral("..")) {
                continue;
            }
            createItems(*it2, *it1 + QLatin1Char('/') + *it2, false);
        }
    }
}

void UserDefinedRegExps::createItems(const QString &_title, const QString &dir, bool usersRegExp)
{
    QString title = _title;
    if (_title == QLatin1String("general")) {
        title = i18n("general");
    }

    QTreeWidgetItem *lvItem = new QTreeWidgetItem((QTreeWidget *)nullptr, QStringList(title));
    lvItem->setExpanded(true);
    _userDefined->addTopLevelItem(lvItem);

    QDir directory(dir);
    QStringList files = directory.entryList(QStringList(QStringLiteral("*.regexp")));
    for (QStringList::Iterator it = files.begin(), total = files.end(); it != total; ++it) {
        const QString fileName = dir + QLatin1Char('/') + *it;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            KMessageBox::error(this, i18n("Could not open file for reading: %1", fileName));
            continue;
        }

        QTextStream stream(&file);
        QString data = stream.readAll();
        file.close();

        RegExp *regexp = WidgetFactory::createRegExp(data);
        if (!regexp) {
            KMessageBox::error(this, i18n("File %1 containing user defined regular expression contained an error", fileName));
            continue;
        }

        new WidgetWinItem(*it, regexp, usersRegExp, lvItem);

        // Inserth the regexp into the list of compound regexps
        if (regexp->type() == RegExp::COMPOUND) {
            CompoundRegExp *cregexp = dynamic_cast<CompoundRegExp *>(regexp);
            if (cregexp && cregexp->allowReplace()) {
                _regExps.append(cregexp);
            }
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

void UserDefinedRegExps::slotLoad(QTreeWidgetItem *item)
{
    if (!item || !dynamic_cast<WidgetWinItem *>(item)) {
        // Mouse pressed outside a widget.
        return;
    }

    WidgetWinItem *wwi = dynamic_cast<WidgetWinItem *>(item);
    if (wwi) {
        Q_EMIT load(wwi->regExp());
    }
}

void UserDefinedRegExps::slotContextMenuTriggered(const QPoint &pos)
{
    QMenu menu;
    QAction *deleteAction = menu.addAction(i18n("Delete"), this, &UserDefinedRegExps::slotDeleteUserRegexp);
    QAction *renameAction = menu.addAction(i18n("Rename"), this, &UserDefinedRegExps::slotRenameUserRegexp);

    QTreeWidgetItem *item = _userDefined->itemAt(pos);

    if (!item || !dynamic_cast<WidgetWinItem *>(item)) {
        // menu not selected on an item
        deleteAction->setEnabled(false);
        renameAction->setEnabled(false);
    } else {
        // Only allow rename and delete of users own regexps.
        WidgetWinItem *winItem = dynamic_cast<WidgetWinItem *>(item);
        if (winItem) {
            if (!winItem->isUsersRegExp()) {
                deleteAction->setEnabled(false);
                renameAction->setEnabled(false);
            } else {
                QVariant var = QVariant::fromValue<void *>((void *)(winItem));
                deleteAction->setData(var);
                renameAction->setData(var);
            }
        }
    }

    menu.exec(_userDefined->mapToGlobal(pos));
}

void UserDefinedRegExps::slotSelectNewAction()
{
    slotUnSelect();
}

void UserDefinedRegExps::slotRenameUserRegexp()
{
    QAction *const action = qobject_cast<QAction *>(sender());
    Q_ASSERT(action);

    WidgetWinItem *winItem = static_cast<WidgetWinItem *>(action->data().value<void *>());
    Q_ASSERT(winItem);

    QString oldName = winItem->name();

    QString txt = QInputDialog::getText(this, i18n("Rename Item"), i18n("New name:"), QLineEdit::Normal, oldName);

    if (!txt.isNull() && oldName != txt) {
        const QString fileName = WidgetWinItem::path() + QLatin1Char('/') + txt + QStringLiteral(".regexp");
        QFileInfo finfo(fileName);
        if (finfo.exists()) {
            int answer = KMessageBox::warningTwoActions(this,
                                                        i18n("<p>Overwrite named regular expression <b>%1</b>?</p>", txt),
                                                        QString(),
                                                        KStandardGuiItem::overwrite(),
                                                        KGuiItem(i18n("Do Not Overwrite")));
            if (answer != KMessageBox::PrimaryAction) {
                return;
            }

            // An item with this name already exists.
            delete winItem;
        } else {
            winItem->setName(txt);
        }
        QDir dir;
        dir.remove(fileName);

        const QString oldFileName = WidgetWinItem::path() + QLatin1Char('/') + oldName + QStringLiteral(".regexp");
        dir.rename(oldFileName, fileName);
    }
}

void UserDefinedRegExps::slotDeleteUserRegexp()
{
    QAction *const action = qobject_cast<QAction *>(sender());
    Q_ASSERT(action);

    WidgetWinItem *winItem = static_cast<WidgetWinItem *>(action->data().value<void *>());
    Q_ASSERT(winItem);

    QFile file(winItem->fileName());
    Q_ASSERT(file.exists());

    file.remove();
    delete winItem;
}

WidgetWinItem::WidgetWinItem(const QString &fileName, RegExp *regexp, bool usersRegExp, QTreeWidgetItem *parent)
    : QTreeWidgetItem(parent)
    , _regexp(regexp)
    , _usersRegExp(usersRegExp)
{
    int index = fileName.lastIndexOf(QLatin1String(".regexp"));
    _name = fileName.left(index);

    setText(0, _name);
}

WidgetWinItem::~WidgetWinItem()
{
    delete _regexp;
}

QString WidgetWinItem::fileName() const
{
    return path() + QLatin1Char('/') + _name + QStringLiteral(".regexp");
}

RegExp *WidgetWinItem::regExp() const
{
    return _regexp;
}

QString WidgetWinItem::name() const
{
    return _name;
}

void WidgetWinItem::setName(const QString &nm)
{
    _name = nm;
    setText(0, nm);
}

QString WidgetWinItem::path()
{
    const QString regexppath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + QStringLiteral("KRegExpEditor/");
    QDir().mkpath(regexppath);
    return regexppath;
}

#include "moc_userdefinedregexps.cpp"
