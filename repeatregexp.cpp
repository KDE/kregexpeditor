#include "repeatregexp.h"
#include "kmessagebox.h"
#include "klocale.h"

RepeatRegExp::RepeatRegExp(int lower, int upper, RegExp* child)
{
	_lower = lower;
	_upper = upper;
	_child = child;
  if (child)
    addChild( child );
}

QString RepeatRegExp::toString() const
{
	QString cText = _child->toString();
	
  QString startPar = QString::fromLocal8Bit("");
  QString endPar = QString::fromLocal8Bit("");
  
  if ( _child->precedence() < precedence() ) {
    startPar = QString::fromLocal8Bit("(");
    endPar = QString::fromLocal8Bit(")");
  }
  
	if (_lower == 0 && _upper == -1) {
		return startPar + cText +endPar + QString::fromLocal8Bit("*");
	}
	else if ( _lower == 0 && _upper == 1 ) {
		return startPar + cText + endPar + QString::fromLocal8Bit("?");
	}
	else if ( _lower == 1 && _upper == -1 ) {
		return startPar + cText + endPar + QString::fromLocal8Bit("+");
	}
	else {
    return startPar + cText + endPar + QString::fromLocal8Bit("{") +
      QString::number( _lower ) + QString::fromLocal8Bit(",") +
      QString::number( _upper ) + QString::fromLocal8Bit("}");
	}
}

QDomNode RepeatRegExp::toXml( QDomDocument* doc ) const
{
  QDomElement top = doc->createElement( QString::fromLocal8Bit("Repeat") );
  top.setAttribute( QString::fromLocal8Bit("lower"), _lower );
  top.setAttribute( QString::fromLocal8Bit("upper"), _upper );
  top.appendChild( _child->toXml( doc ) );
  return top;
}

bool RepeatRegExp::load( QDomElement top, const QString& version ) 
{
  Q_ASSERT( top.tagName() == QString::fromLocal8Bit( "Repeat" ) );
  QString lower = top.attribute( QString::fromLocal8Bit("lower"), QString::fromLocal8Bit("0") );
  QString upper = top.attribute( QString::fromLocal8Bit("upper"), QString::fromLocal8Bit("0") );
  bool ok;
  _lower = lower.toInt( &ok );
  if ( !ok ) {
    KMessageBox::sorry( 0, i18n("value for attribute <b>lower</b> was not an integer for element "
                                "<b>Repeat</b><br>It contained the value <b>%1</b>").arg(lower),
                        i18n("Error while loading from XML file") ) ;
    _lower = 0;
  }
  _upper = upper.toInt( &ok );
  if ( !ok ) {
    KMessageBox::sorry( 0, i18n("value for attribute <b>upper</b> was not an integer for element "
                                "<b>Repeat</b><br>It contained the value <b>%1</b>").arg(upper),
                        i18n("Error while loading from XML file") ) ;
    _upper = -1;
  }
  
  _child = readRegExp( top, version );
  if ( _child ) {
    addChild( _child );
    return true;
  }
  else
    return false;
}

bool RepeatRegExp::operator==( const RegExp& other ) const
{
  if ( type() != other.type() )
    return false;

  const RepeatRegExp& theOther = dynamic_cast<const RepeatRegExp&>( other );
  if ( _lower != theOther._lower || _upper != theOther._upper )
    return false;
  
  return (*_child == *(theOther._child) );
}

