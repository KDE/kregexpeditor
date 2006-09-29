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
#ifndef __repeatwidget
#define __repeatwidget

#include "singlecontainerwidget.h"
#include <q3vbox.h>
//Added by qt3to4:
#include <QPaintEvent>
class Q3ButtonGroup;
class KDialog;
class QCheckBox;
class QSpinBox;
class RepeatRegExp;

/**
   Widget containging the configuration for a @ref RepeatWidget
   @internal
*/
class RepeatRangeWindow :public Q3VBox
{
Q_OBJECT

public:
  enum REPEATTYPE {ANY, ATLEAST, ATMOST, EXACTLY, MINMAX};

  RepeatRangeWindow( QWidget* parent );
  QString text();
  int min();
  int max();
  void set( REPEATTYPE tp, int min, int max );

protected slots:
  void slotItemChange( int which );
  void slotUpdateMinVal( int minVal );
  void slotUpdateMaxVal( int minVal );


private:
  void createLine( QWidget* parent, QString text, QSpinBox** spin, REPEATTYPE tp );

  QSpinBox* _leastTimes;
  QSpinBox* _mostTimes;
  QSpinBox* _exactlyTimes;
  QSpinBox* _rangeFrom;
  QSpinBox* _rangeTo;
  Q3ButtonGroup* _group;
};





/**
   RegExp widget for `repeated content'
   @internal
*/
class RepeatWidget :public SingleContainerWidget
{
Q_OBJECT

public:
  RepeatWidget( RegExpEditorWindow* editorWindow, QWidget *parent);
  RepeatWidget( RepeatRegExp* regexp, RegExpEditorWindow* editorWindow,
              QWidget* parent);
  void init();
  virtual QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return REPEAT; }
  virtual int edit();

protected:
  virtual void paintEvent( QPaintEvent *e );

protected slots:
  void slotConfigCanceled();
  void slotConfigWindowClosed();

private:
  KDialog* _configWindow;
  RepeatRangeWindow* _content;

  mutable QSize _textSize;
  mutable QSize _childSize;
  QByteArray _backup;
};


#endif // __repeatwidget
