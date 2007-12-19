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
#include "charselector.h"

#ifdef QT_ONLY
#include "compat.h"
#else
#include <klocale.h>
#include "charselector.moc"
#endif

#include "limitedcharlineedit.h"
#include "regexpconverter.h"
#include <q3widgetstack.h>
#include <QComboBox>
//Added by qt3to4:
#include <QHBoxLayout>

/**
   In the class CharSelector, three LimitedCharLineEdit are used.
   These widgets are all used in a QWidgetStack. The LimitedCharLineEdit
   class is basically a QLineEdit, which is limited to a certain
   number of characters. This conflicts with the QWidgetStack, as this
   class expects the widgets on the stack to take up all space.
   StackContainer fills in this gab.
*/
class StackContainer :public QWidget
{
public:
    StackContainer( QWidget* child, QWidget* parent ) : QWidget( parent )
        {
            QHBoxLayout* layout = new QHBoxLayout( this );
            child->setParent( this );
            layout->addWidget( child );
            layout->addStretch( 1 );
        }
};

CharSelector::CharSelector( QWidget* parent )
    :QWidget( parent ), _oldIndex(0)
{
  QStringList items;
  QHBoxLayout* layout = new QHBoxLayout( this );
  layout->setSpacing( 6 );
  layout->setMargin( 0 );

  _type = new QComboBox( this );
  _type->setObjectName("_type");
  items << i18n("Normal Character")
        << i18n("Unicode Char in Hex.")
        << i18n("Unicode Char in Oct.")
        << QString::fromLatin1("----")
        << i18n("The Bell Character (\\a)")
        << i18n("The Form Feed Character (\\f)")
        << i18n("The Line Feed Character (\\n)")
        << i18n("The Carriage Return Character (\\r)")
        << i18n("The Horizontal Tab Character (\\t)")
        << i18n("The Vertical Tab Character (\\v)");
  _type->addItems( items );
  layout->addWidget( _type );

  _stack = new Q3WidgetStack( this, "_stack" );
  layout->addWidget( _stack );

  _normal = new LimitedCharLineEdit( LimitedCharLineEdit::NORMAL, 0, "_normal" );
  _stack->addWidget( new StackContainer( _normal, _stack ), 0 );

  _hex = new LimitedCharLineEdit( LimitedCharLineEdit::HEX, _stack, "_hex" );
  _stack->addWidget( new StackContainer( _hex, _stack ), 1 );

  _oct = new LimitedCharLineEdit( LimitedCharLineEdit::OCT, _stack, "_oct" );
  _stack->addWidget( new StackContainer( _oct, _stack ), 2 );

  _stack->raiseWidget( 0 );

  connect( _type, SIGNAL( activated( int ) ), this, SLOT(slotNewItem( int ) ) );
}

void CharSelector::slotNewItem( int which )
{
  _type->setCurrentIndex( which );
  if ( which <= 2 ) {
    _stack->raiseWidget( which );
    _normal->setEnabled( true );
    _hex->setEnabled( true );
    _oct->setEnabled( true );
  }
  else if ( which == 3 ) {
    _type->setCurrentIndex( _oldIndex );
    slotNewItem(_oldIndex);
    return;
  }
  else {
    _normal->setEnabled( false );
    _hex->setEnabled( false );
    _oct->setEnabled( false );
  }

    _oldIndex = which;
}

void CharSelector::setText( QString text )
{
    // This is the best I can do about missing character range features, unless all of
    // textrangeregexp is to be reworked. The problem is that textrangeregexp only allows to
    // get the characters, which would be something like \a, but \a does not work with say Emacs
    // style regexps -- ko28 Sep. 2003 10:55 -- Jesper K. Pedersen
    bool enabled = ( RegExpConverter::current()->features() & RegExpConverter::ExtRange );
    _type->setEnabled( enabled );

  if ( text.at(0) == QChar('\\') ) {
    if ( text.at(1) == QChar('x') ) {
      _hex->setText(text.mid(2));
      slotNewItem( 1 );
    }
    else if ( text.at(1) == QChar('0') ) {
      _oct->setText(text.mid(2));
      slotNewItem( 2 );
    }
    else if ( text.at(1) == QChar('a') )
      slotNewItem(4);
    else if ( text.at(1) == QChar('f') )
      slotNewItem(5);
    else if ( text.at(1) == QChar('n') )
      slotNewItem(6);
    else if ( text.at(1) == QChar('r') )
      slotNewItem(7);
    else if ( text.at(1) == QChar('t') )
      slotNewItem(8);
    else if ( text.at(1) == QChar('v') )
      slotNewItem(9);
    else {
      qWarning("Warning %s:%d Unknown escape %s", __FILE__, __LINE__, qPrintable(text) );
    }
  }
  else {
    slotNewItem(0);
    _normal->setText(text);
  }
}

bool CharSelector::isEmpty() const
{
  return ( _type->currentIndex() == 0 && _normal->text().isEmpty() ) ||
    ( _type->currentIndex() == 1 && _hex->text().isEmpty() ) ||
    ( _type->currentIndex() == 2 && _oct->text().isEmpty() );
}

QString CharSelector::text() const
{
  switch ( _type->currentIndex() ) {
  case 0: // Normal Character
    return _normal->text();
  case 1: // Hex
    return QString::fromLocal8Bit("\\x") + _hex->text();
  case 2: // Oct
      return QString::fromLocal8Bit("\\0") + _oct->text();
  case 3: // The separator
    break;
  case 4:
    return QString::fromLatin1("\\a");
  case 5:
    return QString::fromLatin1("\\f");
  case 6:
    return QString::fromLatin1("\\n");
  case 7:
    return QString::fromLatin1("\\r");
  case 8:
    return QString::fromLatin1("\\t");
  case 9:
    return QString::fromLatin1("\\v");
  }
  return QString();
}
