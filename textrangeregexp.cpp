#include "textrangeregexp.h"
#include "kdebug.h"
#include <kmessagebox.h>
#include <klocale.h>

TextRangeRegExp::TextRangeRegExp()
  :_negate(false), _digit(false), _nonDigit(false), _space(false), _nonSpace(false), _wordChar(false), _nonWordChar(false)
{
	carrot = new QChar( '^' );
	dash = new QChar( '-' );
	parenthesis = new QChar( ']' );
}

TextRangeRegExp::~TextRangeRegExp()
{
	delete carrot;
	delete dash;
	delete parenthesis;
}


void TextRangeRegExp::addCharacter( QString str )
{
	_chars.append( str );
}

void TextRangeRegExp::addRange(QString from, QString to)
{
	_ranges.append( new StringPair( from, to ) );
}

QString TextRangeRegExp::toString() const
{
	QString txt;

	bool foundCarrot = false;
	bool foundDash = false;
	bool foundParenthesis = false;

	// Now print the rest of the single characters, but keep "^" as the very
	// last element of the characters.
	for (unsigned int i = 0; i< _chars.count(); i++) {
		if ( *_chars.at(i) == *parenthesis ) {
			foundParenthesis = true;
		}
		else if ( *_chars.at(i) == *dash ) {
			foundDash = true;
		}
		else if ( *_chars.at(i) == *carrot ) {
			foundCarrot = true;
		}
		else {
			txt.append( *_chars.at(i) );
		}
	}

	// Now insert the ranges.
  for ( QListIterator<StringPair> it(_ranges); *it; ++it ) {
		txt.append((*it)->first()+ QString::fromLatin1("-")+ (*it)->second());
	}
	
	// Ok, its time to build each part of the regexp, here comes the rule:
	// if a ']' is one of the characters, then it must be the first one in the 
	// list (after then opening '[' and eventually negation '^')
	// Next if a '-' is one of the characters, then it must come
	// finally if '^' is one of the characters, then it must not be the first
	// one!

	QString res = QString::fromLatin1("[");
	
	if ( _negate )
		res.append(QString::fromLatin1("^"));


	// a ']' must be the first character in teh range.
	if ( foundParenthesis ) {
		res.append(QString::fromLatin1("]"));
	}
	
	// a '-' must be the first character ( only comming after a ']')
	if ( foundDash ) {
		res.append(QString::fromLatin1("-"));
	}

	res += txt;

	// Insert \s,\S,\d,\D,\w, and \W
  if ( _digit )
    res += QString::fromLocal8Bit("\\d");
  if ( _nonDigit )
    res += QString::fromLocal8Bit("\\D");
  if ( _space )
    res += QString::fromLocal8Bit("\\s");
  if ( _nonSpace )
    res += QString::fromLocal8Bit("\\S");
  if ( _wordChar )
    res += QString::fromLocal8Bit("\\w");
  if ( _nonWordChar )
    res += QString::fromLocal8Bit("\\W");
  

	if ( foundCarrot ) {
		res.append( QChar( '^' ) );
	}

	res.append(QString::fromLatin1("]"));

	return res;
}

QDomNode TextRangeRegExp::toXml( QDomDocument* doc ) const
{
  QDomElement top = doc->createElement( QString::fromLocal8Bit( "TextRange" ) );

  if ( _negate )
    top.setAttribute( QString::fromLocal8Bit("negate"), true );
  if ( _digit )
    top.setAttribute( QString::fromLocal8Bit("digit"), true );
  if ( _nonDigit )
    top.setAttribute( QString::fromLocal8Bit("nonDigit"), true );
  if ( _space )
    top.setAttribute( QString::fromLocal8Bit("space"), true );
  if ( _nonSpace )
    top.setAttribute( QString::fromLocal8Bit("nonSpace"), true );
  if ( _wordChar )
    top.setAttribute( QString::fromLocal8Bit("wordChar"), true );
  if ( _nonWordChar )
    top.setAttribute( QString::fromLocal8Bit("nonWordChar"), true );
  
  for ( QStringList::ConstIterator it = _chars.begin(); it != _chars.end(); ++it ) {
    QDomElement elm = doc->createElement( QString::fromLocal8Bit( "Character" ) );
    elm.setAttribute( QString::fromLocal8Bit( "char" ), *it );
    top.appendChild( elm );
  }
  
  for ( QListIterator<StringPair> it(_ranges); *it; ++it ) {
    QDomElement elm = doc->createElement( QString::fromLocal8Bit( "Range" ) );
    elm.setAttribute( QString::fromLocal8Bit( "from" ), (*it)->first() );
    elm.setAttribute( QString::fromLocal8Bit( "to" ), (*it)->second() );
    top.appendChild( elm );
  }
  return top;
}

bool TextRangeRegExp::load( QDomElement top, const QString& /*version*/ ) 
{
  ASSERT( top.tagName() == QString::fromLocal8Bit( "TextRange" ) );
  QString str;
  QString one = QString::fromLocal8Bit("1");
  QString zero = QString::fromLocal8Bit("0");
  
  str = top.attribute( QString::fromLocal8Bit("negate"), zero );
  _negate = ( str == one );

  str = top.attribute( QString::fromLocal8Bit("digit"), zero );
  _digit = ( str == one );

  str = top.attribute( QString::fromLocal8Bit("nonDigit"), zero );
  _nonDigit = ( str == one );

  str = top.attribute( QString::fromLocal8Bit("space"), zero );
  _space = ( str == one );

  str = top.attribute( QString::fromLocal8Bit("nonSpace"), zero );
  _nonSpace = ( str == one );

  str = top.attribute( QString::fromLocal8Bit("wordChar"), zero );
  _wordChar = ( str == one );

  str = top.attribute( QString::fromLocal8Bit("nonWordChar"), zero );
  _nonWordChar = ( str == one );

  for ( QDomNode node = top.firstChild(); !node.isNull(); node = node.nextSibling() ) {
    if ( !node.isElement() )
      continue; // Skip comments.
    QDomElement child = node.toElement();
    
    if ( child.tagName() == QString::fromLocal8Bit( "Character" ) ) {
      QString ch = child.attribute( QString::fromLocal8Bit( "char" ) );
      addCharacter( ch );
    }
    else if ( child.tagName() == QString::fromLocal8Bit( "Range" ) ) {
      QString from = child.attribute( QString::fromLocal8Bit( "from" ) );
      QString to = child.attribute( QString::fromLocal8Bit( "to" ) );
      addRange( from, to );
    }
    else {
      KMessageBox::sorry( 0, i18n("Invalid sub element to element <b>TextRange</b>. Tag was <b>%1</b>").arg(child.tagName()),
                        i18n("Error while loading from XML file") ) ;
      return false;
    }
  }
  return true;
}

