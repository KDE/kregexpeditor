#include "infopage.h"
#include <klocale.h>
#include <kapplication.h>
#include <kurl.h>

InfoPage::InfoPage( QWidget* parent, const char* name )
  :QTextBrowser( parent, name )
{
  setText( i18n( "<qt>"
                 "<h1>Regular Expression Editor</h1>"
                 "What you are currently looking at is an editor for <i>Regular Expressions</i>.<p>"
                 "You start editing by pressing one of the buttons at the left, and then fill out "
                 "the area containing the text you are looking at, by pressing the left mouse button at the location "
                 "you want the items to apear.<p>"

                 "<h2>What is a regular expression?</h2>"
                 "If you do not know what a regular expression is, then it might be a good idea "
                 "to read <a href=\"doc:whatIsARegExp\">the introduction</a>.<p>"

                 "<h2>How does this editor work?</h2>"
                 "In case this is the first time you see this editor, then you might want to " 
                 "read an <a href=\"doc:theEditor\">introduction to the editor</a>.<p><p>"
                 "The following list might help you find exactly what you are searching for in the documentation:"
                 "<ul>"
                 "<li> <a href=\"doc:widgets\">Description of the Widgets</a>."
                 "<li> <a href=\"doc:cutnpaste\">Cut and Past</a> and <a href=\"doc:draganddrop\">Drag and Drop</a>."
                 "<li> <a href=\"doc:loadandsave\">Load and Save</a>"
                 "<li> <a href=\"doc:graphicalvsascii\">The Graphical Widget v.s. the Ascii Syntax.</a>"
                 "</ul>"

                 "<h2>Authors</h2>"
                 "<a href=\"http://www.blackie.dk/\">Jesper K. Pedersen</a> &lt;<a href=\"mailto:blackie@kde.org\">blackie@kde.org</a>&gt;"
                 "</qt>") );
}

void InfoPage::setSource ( const QString & name )
{
  if ( name.left(4) == QString::fromLocal8Bit("doc:") ) {
    kapp->invokeHelp( name.mid(4), QString::fromLocal8Bit("KRegExpEditor") ); 
  }
  else if ( name.left(7) == QString::fromLocal8Bit("mailto:") ) {
    kapp->invokeMailer( KURL(name) );
  }
  else {
    kapp->invokeBrowser( name );
  }
}
