#include "charselector.h"
#include <qwidgetstack.h>
#include <limitedcharlineedit.h>
#include <klocale.h>
#include <qcombobox.h>
#include <qstringlist.h>

/**
   In the class CharSelector, three LimitedCharLineEdit are used.
   These widgets are all used in a QWidgetStack. The LimitedCharLineEdit
   class is basicly a QLineEdit, which is limited to a certain
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
      child->reparent( this, QPoint(0,0), false );
      layout->addWidget( child );
      layout->addStretch( 1 );
    }
};
      
CharSelector::CharSelector( QWidget* parent, const char* name )
  :QWidget( parent, name ), _oldIndex(0)
{
  QStringList items;
  QHBoxLayout* layout = new QHBoxLayout( this, 0, 6 );

  _type = new QComboBox( this, "_type" );  
  items << i18n("Normal Character") 
        << i18n("Unicode char in hex.") 
        << i18n("Unicode char in oct.")
        << QString::fromLatin1("----")
        << i18n("The bell character (\\a)")
        << i18n("The form feed character (\\f)")
        << i18n("The line feed character (\\n)")
        << i18n("The carriage return character (\\r)")
        << i18n("The horizontal tab character (\\t)")
        << i18n("The vertical tab character (\\v)");
  _type->insertStringList( items );
  layout->addWidget( _type );
  
  _stack = new QWidgetStack( this, "_stack" );
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
  _type->setCurrentItem( which );
  if ( which <= 2 ) {
    _stack->raiseWidget( which );
    _normal->setEnabled( true );
    _hex->setEnabled( true );
    _oct->setEnabled( true );    
  }
  else if ( which == 3 ) {
    _type->setCurrentItem( _oldIndex );
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
      qWarning("Warning %s:%d Unknown escape %s", __FILE__, __LINE__, text.latin1() );
    }
  }
  else {
    slotNewItem(0);
    _normal->setText(text);
  }
}

bool CharSelector::isEmpty() const
{
  return ( _type->currentItem() == 0 && _normal->text().isEmpty() ) ||
    ( _type->currentItem() == 1 && _hex->text().isEmpty() ) ||
    ( _type->currentItem() == 2 && _oct->text().isEmpty() );
}

QString CharSelector::text() const
{
  switch ( _type->currentItem() ) {
  case 0: // Normal Character
    return _normal->text();
  case 1: // Hex
    return QString::fromLocal8Bit("\\x") + _hex->text();
  case 2: // Oct
      return QString::fromLocal8Bit("\\0") + _oct->text();
  case 3: // The seperator
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
  return QString::null;
}

#include "charselector.moc"
