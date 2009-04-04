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

#include "infopage.h"

#ifdef QT_ONLY
  #include "compat.h"
#else
  #include <klocale.h>
  #include <kapplication.h>
#include <ktoolinvocation.h>
#endif

InfoPage::InfoPage( QWidget* parent )
  :KTextBrowser( parent )
{
    QString txt =
        QString::fromLatin1( "<qt>" ) +
        i18nc( "Translators, feel free to add yourself in the text below, asking for a postcard ;-), "
              "also feel free to add a section saying <h2>Translators</h2>. "
              "Kind regards, and thanks for your work - Jesper.",
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
              "to read <a href=\"doc://whatIsARegExp\">the introduction to regular expressions</a>.<br />");

#ifdef HOUSEOFFUSION
    txt += tr("<h2>House Of Fusion note</h2>"
              "If you want to donate money to continue development on the KRegExpEditor port for "
              "house of fusion then please contact Michael Dinowitz (mdinowit@houseoffusion.com).");
#endif
    txt += i18n( "<h2>Send the author an electronic postcard</h2>"
                 "I don't get any money for working on KRegExpEditor, "
                 "I therefore appreciate it very much when users tell me what they think about my work. "
                 "I would therefore be very happy if you <a href=\"mailto:blackie@kde.org?subject=KRegExpEditor\">"
                 "sent me a short mail</a>, telling me that you use my regular expression editor."

                 "<h2>Author</h2>"
                 "<a href=\"http://www.blackie.dk/\">Jesper K. Pedersen</a> &lt;<a href=\"mailto:blackie@kde.org\">blackie@kde.org</a>&gt;")
           + QString::fromLatin1( "</qt>" );
    setText( txt );
}

void InfoPage::setSource ( const QUrl& name )
{
    QString nm = name.toString();
#ifdef QT_ONLY
    mimeSourceFactory()->setFilePath( QStringList() << QString::fromLatin1("manual/"));
    if ( nm.endsWith('/') )
        nm = nm.left( nm.length()-1);

    if ( nm.startsWith("mailto:") ) {
        QMessageBox::information( this, tr("Support mail"), tr("Please send the mail to blackie@kde.org") );
        return;
    }
    if ( nm.startsWith( "http:" ) )
        return;


    if ( nm == "doc://" )
        nm = "doc://index";

    if ( nm.startsWith( QString::fromLocal8Bit("doc://") ) ) {
        nm = nm.mid(6) + ".html";
    }

    QTextBrowser::setSource( nm );
#else
  if ( nm.startsWith( QString::fromLocal8Bit("doc://") ) ) {
    KToolInvocation::invokeHelp( nm.mid(6, nm.length()-7), QString::fromLocal8Bit("KRegExpEditor") );
  }
  else {
    KTextBrowser::setSource( name ); // handle mailto and other links
  }
#endif
}
