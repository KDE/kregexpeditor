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
#ifndef __limitedcharlineedit
#define __limitedcharlineedit

#include <qlineedit.h>

/**
   QLineEdit which only accepts a prespecified number of character.
   @internal
*/
class LimitedCharLineEdit :public QLineEdit
{
public:
  enum Mode { NORMAL = 0, HEX = 1, OCT = 2 };

	LimitedCharLineEdit(Mode mode, QWidget* parent, const char *name = 0);

protected:
  virtual void keyPressEvent ( QKeyEvent * );

private:
  Mode _mode;
  unsigned int _count;
};

#endif

