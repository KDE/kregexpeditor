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

#ifndef kregexpeditorgui_h
#define kregexpeditorgui_h

#include <QDialog>
#include <QVariant>

#ifdef ENABLE_KTEXTWIDGETS
#include <kregexpeditorinterface.h>
#endif

class KRegExpEditorPrivate;

/**
   Regular Expression Editor.

   @author Jesper Kjær Pedersen <blackie@kde.org>
   @version 0.1
 **/
class Q_DECL_EXPORT KRegExpEditorGUI : public QWidget
#ifdef ENABLE_KTEXTWIDGETS
                                       , public KRegExpEditorInterface
#endif
{
    Q_OBJECT
#ifdef ENABLE_KTEXTWIDGETS
    Q_INTERFACES(KRegExpEditorInterface)
#endif
    Q_PROPERTY(QString regexp READ regExp WRITE setRegExp)
public:
    KRegExpEditorGUI(QWidget *parent = nullptr, const QVariantList & = QVariantList());
    ~KRegExpEditorGUI();
    QString regExp() const;

    static const QString version;

protected:
    void closeEvent(QCloseEvent *event);

Q_SIGNALS:
    /** This signal tells whether undo is available. */
    void canRedo(bool);
    void canUndo(bool);
    void changes(bool);

public Q_SLOTS:
    void redo();
    void undo();
    void setRegExp(const QString &regexp);
    void doSomething(const QString &method, void *arguments);
    void setMatchText(const QString &);

private:
    KRegExpEditorPrivate *_editor;
};

#endif
