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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
#include "limitedcharlineedit.h"
#include <qvalidator.h>

/**
   @internal
   A Validator for the @ref LimitedCharLineEdit
*/
class Validator :public QValidator
{
public:
  Validator( LimitedCharLineEdit::Mode mode, QWidget* parent )
    :QValidator( parent, "Validator" ), _mode(mode)
  {
  }

  virtual QValidator::State validate( QString& txt, int & /*pos*/ ) const
  {
    if ( _mode == LimitedCharLineEdit::NORMAL ||
         (_mode == LimitedCharLineEdit::HEX &&
          QRegExp(QString::fromLocal8Bit("^[0-9A-Ea-e]*$")).search( txt ) != -1) ||
         (_mode == LimitedCharLineEdit::OCT &&
          QRegExp(QString::fromLocal8Bit("^[0-7]*$")).search( txt ) != -1 ) ) {
      return QValidator::Acceptable;
    }
    else {
      return QValidator::Invalid;
    }
  }

private:
  LimitedCharLineEdit::Mode _mode;
};


void LimitedCharLineEdit::keyPressEvent ( QKeyEvent *event )
{
  QLineEdit::keyPressEvent( event );
  if ( text().length() == _count && !event->text().isNull() )
    focusNextPrevChild(true);
}

LimitedCharLineEdit::LimitedCharLineEdit( Mode mode, QWidget* parent, const char* name )
	:QLineEdit( parent, name ), _mode(mode)
{
  if ( mode == NORMAL )
    _count = 1;
  else if ( mode == HEX )
    _count = 4;
  else
    _count = 4;

  setMaxLength( _count );
  setFixedSize( fontMetrics().width('A')*5+5, sizeHint().height());

  setValidator( new Validator( mode, this ) );
}


