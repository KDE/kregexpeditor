#include "widgetfactory.h"
#include "regexpwidget.h"
#include "dragaccepter.h"
#include "repeatwidget.h"
#include "textwidget.h"
#include "characterswidget.h"
#include "altnwidget.h"
#include "zerowidgets.h"
#include "compoundwidget.h"
#include "editorwindow.h"
#include "concwidget.h"
#include "lookaheadwidget.h"
#include <qdialog.h>

#include "regexp.h"
#include "textregexp.h"
#include "textrangeregexp.h"
#include "repeatregexp.h"
#include "lookaheadregexp.h"
#include "concregexp.h"
#include "altnregexp.h"
#include "positionregexp.h"
#include "dotregexp.h"
#include "compoundregexp.h"
#include "kregexpeditorgui.h"
#include <kmessagebox.h>
#include <klocale.h>

bool WidgetFactory::isContainer( RegExpType tp )
{
  return ( tp == REPEAT || tp == ALTN || tp == COMPOUND );
}

RegExpWidget* WidgetFactory::createWidget( RegExpEditorWindow* win, QWidget* parent, 
                                           RegExpType type )
{
  RegExpWidget* widget = 0;
  
  switch (type) {
  case TEXT: 
    return new TextWidget( win, parent ); break;
  case ALTN: 
    return  new AltnWidget( win, parent ); break;
  case DOT: 
    return  new AnyCharWidget( win, parent ); break;
  case BEGLINE: 
    return new BegLineWidget( win, parent ); break;
  case ENDLINE: 
    return new EndLineWidget( win, parent ); break;
  case WORDBOUNDARY:
    return new WordBoundaryWidget( win, parent ); break;
  case NONWORDBOUNDARY:
    return new NonWordBoundaryWidget( win, parent ); break;
  case POSLOOKAHEAD:
  case NEGLOOKAHEAD:
    return new LookAheadWidget( win, type, parent ); break;
  case REPEAT: 
    widget = new RepeatWidget( win, parent ); break;
  case CHARSET: 
    widget = new CharactersWidget( win, parent ); break;
  case COMPOUND: 
    widget = new CompoundWidget( win, parent ); break;
  default:
    qFatal("It should not be possible to get here!");
    return 0;
  }

  if ( widget->edit() == QDialog::Rejected ) {
    delete widget;
    return 0;
  }
  return widget;
}

RegExpWidget* WidgetFactory::createWidget( RegExp* regexp, RegExpEditorWindow* editorWindow,
                                           QWidget* parent )
{
  if ( TextRegExp* reg = dynamic_cast<TextRegExp*>( regexp ) )
    return new TextWidget( reg, editorWindow, parent );
  else if ( TextRangeRegExp* reg = dynamic_cast<TextRangeRegExp*>( regexp ) )
    return new CharactersWidget( reg, editorWindow, parent );
  else if ( RepeatRegExp* reg = dynamic_cast<RepeatRegExp*>( regexp ) )
    return new RepeatWidget( reg, editorWindow, parent );
  else if ( LookAheadRegExp* reg = dynamic_cast<LookAheadRegExp*>( regexp ) ) {
    if ( reg->type() == LookAheadRegExp::POSITIVE )
      return new LookAheadWidget( reg, editorWindow, POSLOOKAHEAD, parent );
    else
      return new LookAheadWidget( reg, editorWindow, NEGLOOKAHEAD, parent );
  }
  else if ( ConcRegExp* reg = dynamic_cast<ConcRegExp*>( regexp ) )
    return new ConcWidget( reg, editorWindow, parent );
  else if ( AltnRegExp* reg = dynamic_cast<AltnRegExp*>( regexp ) )
    return new AltnWidget( reg, editorWindow, parent );
  else if ( PositionRegExp* reg = dynamic_cast<PositionRegExp*>( regexp ) ) {
    switch ( reg->position() ) {
    case PositionRegExp::BEGLINE:
      return new BegLineWidget( editorWindow, parent );      
    case PositionRegExp::ENDLINE:
      return new EndLineWidget( editorWindow, parent );
    case PositionRegExp::WORDBOUNDARY:
      return new WordBoundaryWidget( editorWindow, parent );      
    case PositionRegExp::NONWORDBOUNDARY:
      return new NonWordBoundaryWidget( editorWindow, parent );
    }
  }
  else if ( DotRegExp* reg = dynamic_cast<DotRegExp*>( regexp ) ) 
    return new AnyCharWidget( editorWindow, parent );
  else if ( CompoundRegExp* reg = dynamic_cast<CompoundRegExp*>( regexp ) )
    return new CompoundWidget( reg, editorWindow, parent );
  else {
    qFatal("%s:%d Internal Error: Unknown RegExp type", __FILE__, __LINE__);
  }
}

RegExp* WidgetFactory::createRegExp( QDomElement node, const QString& version )
{
  QString tag = node.tagName();
  RegExp* regexp;
  if ( tag == QString::fromLocal8Bit( "TextRange" ) )
    regexp = new TextRangeRegExp();
  else if ( tag == QString::fromLocal8Bit( "Text" ) ) 
    regexp = new TextRegExp();
  else if ( tag == QString::fromLocal8Bit( "Concatenation" ) )
    regexp = new ConcRegExp();
  else if ( tag == QString::fromLocal8Bit( "Alternatives" ) )
    regexp = new AltnRegExp();
  else if ( tag == QString::fromLocal8Bit( "BegLine" ) )
    regexp = new PositionRegExp( PositionRegExp::BEGLINE );
  else if ( tag == QString::fromLocal8Bit( "EndLine" ) )
    regexp = new PositionRegExp( PositionRegExp::ENDLINE );
  else if ( tag == QString::fromLocal8Bit( "WordBoundary" ) )
    regexp = new PositionRegExp( PositionRegExp::WORDBOUNDARY );
  else if ( tag == QString::fromLocal8Bit( "NonWordBoundary" ) )
    regexp = new PositionRegExp( PositionRegExp::NONWORDBOUNDARY );
  else if ( tag == QString::fromLocal8Bit( "PositiveLookAhead" ) )
    regexp = new LookAheadRegExp( LookAheadRegExp::POSITIVE );
  else if ( tag == QString::fromLocal8Bit( "NegativeLookAhead" ) )
    regexp = new LookAheadRegExp( LookAheadRegExp::NEGATIVE );
  else if ( tag == QString::fromLocal8Bit( "Compound" ) )
    regexp = new CompoundRegExp();
  else if ( tag == QString::fromLocal8Bit( "AnyChar" ) )
    regexp = new DotRegExp();
  else if ( tag == QString::fromLocal8Bit( "Repeat" ) )
    regexp = new RepeatRegExp();
  else {
    KMessageBox::sorry( 0, i18n("Unknown tag while reading XML, tag was <b>%1</b>").arg(tag),
                        i18n("Error while loading from XML file") ) ;

    return 0;
  }
  
  bool ok = regexp->load( node, version );
  if (ok)
    return regexp;
  else {
    delete regexp;
    return 0;
  }
}

RegExp* WidgetFactory::createRegExp( QString str )
{
  QDomDocument doc;
  bool ok = doc.setContent( str );
  if ( !ok ) {
    KMessageBox::sorry( 0, i18n("Error while loading XML file. The file did most likely have unmatched tags."),
                        i18n("Error while loading from XML file") ) ;
  }
  

  // Read the RegularExpression element, and extract the version.
  QDomElement top = doc.documentElement();
  if (! (top.tagName() == QString::fromLocal8Bit("RegularExpression")) ) {
    KMessageBox::sorry( 0, i18n("XML file didn't contain a <b>RegularExpression</b> tag"),
                        i18n("Error while loading from XML file") ) ;
  }
  QString version = top.attribute( QString::fromLocal8Bit("version"), KRegExpEditorGUI::version );
  QDomNode child = top.firstChild();
  if ( ! child.isElement() ) {
    KMessageBox::sorry( 0, i18n("Error while reading XML file, the element just below the tag "
                                "<b>RegularExpression</b> was not an element"),
                        i18n("Error while loading from XML file") ) ;
  }

  RegExp* regexp = WidgetFactory::createRegExp( child.toElement(), version );

  return regexp;
}

