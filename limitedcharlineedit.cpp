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
  
  virtual QValidator::State validate( QString& txt, int &pos ) const
  {
    if ( _mode == LimitedCharLineEdit::NORMAL ||
         (_mode == LimitedCharLineEdit::HEX && 
          QRegExp(QString::fromLocal8Bit("^[0-9A-Ea-e]*$")).find( txt,0 ) != -1) ||
         (_mode == LimitedCharLineEdit::OCT && 
          QRegExp(QString::fromLocal8Bit("^[0-7]*$")).find( txt,0 ) != -1 ) ) {
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


