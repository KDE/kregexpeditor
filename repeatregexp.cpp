#include "repeatregexp.h"
#include "kmessagebox.h"
#include "klocale.h"

RepeatRegExp::RepeatRegExp( bool selected, int lower, int upper, RegExp* child) : RegExp( selected )
{
	_lower = lower;
	_upper = upper;
	_child = child;
    if (child)
        addChild( child );
}

bool RepeatRegExp::check( ErrorMap& map, bool first, bool last )
{
    _child->check( map, first, last );
    return ( _lower == 0 );
}

QString RepeatRegExp::toString( bool markSelection) const
{
    QString cText = _child->toString( markSelection );
    QString startPar;
    QString endPar;

    if ( markSelection && _syntax == Qt ) {
        if ( !isSelected() && _child->isSelected()) {
            startPar = QString::fromLatin1( "(" );
            endPar = QString::fromLatin1( ")" );
        }
        else if ( _child->precedence() < precedence() ) {
            startPar = QString::fromLatin1( "(?:" );
            endPar = QString::fromLatin1( ")" );
        }
    }
    else if ( _child->precedence() < precedence() ) {
        startPar = openPar();
        endPar = closePar();
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
        if ( _syntax == Qt ) {
            return startPar + cText + endPar + QString::fromLocal8Bit("{") +
                QString::number( _lower ) + QString::fromLocal8Bit(",") +
                QString::number( _upper ) + QString::fromLocal8Bit("}");
        }
        else if ( _syntax == Emacs ) {
            QString res = QString::fromLatin1("");
            for ( int i = 0; i < _lower; ++i ) {
                res += QString::fromLatin1( "(" ) + cText + QString::fromLatin1( ")" );
            }
            if ( _upper != -1 ) {
                for ( int i = _lower; i < _upper; ++i ) {
                    res += QString::fromLatin1("(") + cText + QString::fromLatin1(")?");
                }
            }
            else
                res += QString::fromLatin1("+");

            return startPar + res + endPar;
        }
        else {
            qFatal("What?!");
            return QString::null;
        }
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
        KMessageBox::sorry( 0, i18n("<p>Value for attribute <b>%1</b> was not an integer for element "
                                    "<b>%2</b></p><p>It contained the value <b>%3</b></p>")
                            .arg(QString::fromLatin1("lower")).arg(QString::fromLatin1("Repeat")).arg(lower),
                            i18n("Error While Loading From XML File") ) ;
        _lower = 0;
    }
    _upper = upper.toInt( &ok );
    if ( !ok ) {
        KMessageBox::sorry( 0, i18n("<p>Value for attribute <b>%1</b> was not an integer for element "
                                    "<b>%2</b></p><p>It contained the value <b>%3</b></p>")
                            .arg(QString::fromLatin1("upper")).arg(QString::fromLatin1("Repeat")).arg(upper),
                            i18n("Error While Loading From XML File") ) ;
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

