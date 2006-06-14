//Added by qt3to4:
#include <QMouseEvent>
#include <QPixmap>
#include <QPaintEvent>
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
#ifndef compoundwidget
#define compoundwidget
class QLineEdit;
class Q3MultiLineEdit;
class KDialog;
class QCheckBox;

#include "singlecontainerwidget.h"
#include "compoundregexp.h"

/**
   Widget containing configuration details for @ref CompoundWidget
   @internal
*/
class CompoundDetailWindow :public QWidget
{
public:
  CompoundDetailWindow(QWidget* parent, const char* name = 0);
  QString title() const;
  QString description() const;
  bool allowReplace() const;
  void setTitle( QString );
  void setDescription( QString );
  void setAllowReplace( bool );

private:
  QLineEdit* _title;
  Q3MultiLineEdit* _description;
  QCheckBox* _allowReplace;
};


/**
   Comopund RegExp widget.

   This widget has two purposes:
   @li To make it possible for a user to collapse a huge regular expression
   to take up less screen space.
   @li To act as back references for later use.

   @internal
*/
class CompoundWidget :public SingleContainerWidget
{
Q_OBJECT

public:
  CompoundWidget( RegExpEditorWindow* editorWindow, QWidget* parent,
                  const char* name = 0);
  CompoundWidget( CompoundRegExp* regexp, RegExpEditorWindow* editorWindow,
                  QWidget* parent, const char* name = 0);

  virtual bool updateSelection( bool parentSelected );
  virtual QSize sizeHint() const;
  virtual RegExp* regExp() const;
  virtual RegExpType type() const { return COMPOUND; }
  virtual int edit();

protected:
  virtual void paintEvent( QPaintEvent *e );
  virtual void mousePressEvent( QMouseEvent* e );
  virtual void mouseReleaseEvent( QMouseEvent* e);
  void init();
  QPixmap getIcon( const QString& name );

protected slots:
  void slotConfigCanceled();
  void slotConfigWindowClosed();

private:
  bool _hidden;
  QPixmap _up, _down;
  mutable QSize _pixmapSize;
  mutable QPoint _pixmapPos;

  KDialog* _configWindow;
  CompoundDetailWindow* _content;

  mutable QSize _textSize;
  mutable QSize _childSize;
  QByteArray _backup;

  int _backRefId;
};


#endif // compoundwidget
