#include "infopage.h"
#include <klocale.h>
#include <kurl.h>
#include <kapplication.h>
#include <kdebug.h>

InfoPage::InfoPage( QWidget* parent, const char* name )
  :KTextBrowser( parent, name )
{
  setText( i18n( "Translators, feel free to add yourself in the text above, asking for a postcard ;-), "
		 "also feel free to add a section saying <h2>Translators</h2>. "
		 "Kind regards, and thanks for your work - Jesper.",
		 "<qt>"
                 "<h1>Regular Expression Editor</h1>"
                 "<p>What you are currently looking at is an editor for <i>Regular Expressions</i>.</p>"
                 "<p>The upper part in the middle is the editing area, the lower part is a verification window where you can "
                 "try your regular expressions right away. The row of buttons is the editing actions. "
                 "This is actually very similar to common drawing programs. Select an editing tool to start "
                 "editing your regular expression, and press the mouse button in the editing area where you want "
                 "this item inserted.</p>"
                 "<p>For a more detailed description of this editor see the <a href=\"doc://\">info pages</a></p>"

                 "<h2>What is a regular expression?</h2>"
                 "If you do not know what a regular expression is, then it might be a good idea "
                 "to read <a href=\"doc://whatIsARegExp\">the introduction to regular expressions</a>.<p>"

                 "<h2>Send the author an electronic postcard</h2>"
                 "I don't get any money for my work on KDE. "
                 "I therefore appreciate it very much when users tell me what they think about my work. "
                 "I would therefore be very happy if you <a href=\"mailto:blackie@kde.org?subject=KRegExpEditor\">"
                 "sent me a short mail</a>, telling me that you use my regular expression editor."

                 "<h2>Author</h2>"
                 "<a href=\"http://www.blackie.dk/\">Jesper K. Pedersen</a> &lt;<a href=\"mailto:blackie@kde.org\">blackie@kde.org</a>&gt;"                 
                 "</qt>") );
}

void InfoPage::setSource ( const QString & name )
{
  if ( name.startsWith( QString::fromLocal8Bit("doc://") ) ) {
    kapp->invokeHelp( name.mid(6, name.length()-7), QString::fromLocal8Bit("KRegExpEditor") ); 
  }
  else {
    KTextBrowser::setSource( name ); // handle mailto and other links
  }
}
