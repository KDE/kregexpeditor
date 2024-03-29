/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "infopage.h"

#include <KHelpClient>
#include <KLocalizedString>
#include <QDesktopServices>

InfoPage::InfoPage(QWidget *parent)
    : QTextBrowser(parent)
{
    setFrameShape(QFrame::NoFrame);

    QString txt = QStringLiteral("<qt>")
        + i18nc("Translators, feel free to add yourself in the text below, asking for a postcard ;-), "
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
                "to read <a href=\"doc://whatIsARegExp\">the introduction to regular expressions</a>.<br />")
        + QStringLiteral("</qt>");
    setText(txt);
}

void InfoPage::doSetSource(const QUrl &name, QTextDocument::ResourceType type)
{
    Q_UNUSED(type);

    QString nm = name.toString();

    if (nm.startsWith(QStringLiteral("doc://"))) {
        KHelpClient::invokeHelp(nm.mid(6, nm.length() - 7), QStringLiteral("kregexpeditor"));
    } else {
        QDesktopServices::openUrl(name); // handle mailto and other links
    }
}
