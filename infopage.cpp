#include "infopage.h"
#include <klocale.h>
#include <kurl.h>
#include <kapplication.h>
#include <kdebug.h>

InfoPage::InfoPage( QWidget* parent, const char* name )
  :QTextBrowser( parent, name )
{
  setText( i18n( "<qt>"
                 "<h1>Regular Expression Editor</h1>"
                 "What you are currently looking at is an editor for <i>Regular Expressions</i>.<p>"
                 "The window to the right is the editing area. The row of buttons is the editing actions. "
                 "This is actually very similar to common drawing programs. Select an editing tool to start "
                 "editing your regular expression.<p>"

                 "<h2>What is a regular expression?</h2>"
                 "If you do not know what a regular expression is, then it might be a good idea "
                 "to read <a href=\"doc://whatIsARegExp\">the introduction to regular expressions</a>.<p>"

                 "<h2>How does this editor work?</h2>"
                 "In case this is the first time you see this editor, then you might want to " 
                 "read an <a href=\"doc://theEditor\">introduction to the editor</a>.<p><p>"
                 "The following list might help you find exactly what you are searching for in the documentation:"
                 "<ul>"
                 "<li> <a href=\"doc://editingtools\">Description of the editing tools</a>."
                 "<li> <a href=\"doc://cutnpaste\">Cut and Past</a> and <a href=\"doc://draganddrop\">Drag and Drop</a>."
                 "<li> <a href=\"doc://userdefinedregexps\">Load and Save</a>"
                 "<li> <a href=\"doc://bugreport\">Reporting bugs or suggesting new features</a>"
                 "<li> <a href=\"doc://faq\">The FAQ</a>"
                 "</ul>"

                 "<h2>Send the author an electronic postcard</h2>"
                 "As everybody else in the KDE project I dont get any money for my work on KDE. "
                 "I therefore appreciate it very much when users tell me what they think about my work. "
                 "I would therefor be very happy if you <a href=\"mailto:blackie@kde.org?subject=KRegExpEditor\">"
                 "sent me a short mail</a>, telling me that you use my regular expression editor."

                 "<h2>Author</h2>"
                 "<a href=\"http://www.blackie.dk/\">Jesper K. Pedersen</a> &lt;<a href=\"mailto:blackie@kde.org\">blackie@kde.org</a>&gt;"
                 "</qt>") );
}

void InfoPage::setSource ( const QString & name )
{
  if ( name.left(6) == QString::fromLocal8Bit("doc://") ) {
    kapp->invokeHelp( name.mid(6, name.length()-7), QString::fromLocal8Bit("KRegExpEditor") ); 
  }
  else if ( name.left(7) == QString::fromLocal8Bit("mailto:") ) {
    kapp->invokeMailer( KURL(name) );
  }
  else {
    kapp->invokeBrowser( name );
  }
}
