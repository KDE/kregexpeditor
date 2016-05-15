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
    Q_PROPERTY( QString regexp READ regExp WRITE setRegExp )
public:
    KRegExpEditorGUIDialog( QWidget *parent, const QVariantList & = QVariantList() );

    virtual QString regExp() const;

signals:
  /** This signal tells whether undo is available. */
  void canRedo( bool );
  void canUndo( bool );
  void changes( bool );

public slots:
  virtual void redo();
  virtual void undo();
  virtual void setRegExp( const QString &regexp );
  virtual void doSomething( const QString &method, void* arguments );
  virtual void setMatchText( const QString& );

private:
    KRegExpEditorGUI *_editor;
};

#endif
