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

#ifndef kregexpeditorguidialog_h
#define kregexpeditorguidialog_h

#include <QDialog>
#include <kregexpeditorgui.h>
#include <kregexpeditorinterface.h>

class Q_DECL_EXPORT KRegExpEditorGUIDialog : public QDialog, public KRegExpEditorInterface
{
    Q_OBJECT
    Q_INTERFACES(KRegExpEditorInterface)
    Q_PROPERTY(QString regexp READ regExp WRITE setRegExp)
public:
    explicit KRegExpEditorGUIDialog(QWidget *parent, const QVariantList & = QVariantList());
    ~KRegExpEditorGUIDialog() override;

    QString regExp() const override;

Q_SIGNALS:
    /** This signal tells whether undo is available. */
    void canRedo(bool) override;
    void canUndo(bool) override;
    void changes(bool) override;

public Q_SLOTS:
    void redo() override;
    void undo() override;
    void setRegExp(const QString &regexp) override;
    void doSomething(const QString &method, void *arguments) override;
    void setMatchText(const QString &) override;

private:
    void showHelp();
    KRegExpEditorGUI *_editor = nullptr;
};

#endif
