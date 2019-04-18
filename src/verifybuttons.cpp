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

#include "verifybuttons.h"

#include <KLocalizedString>
#include <QIcon>

#include <QMenu>
#include <QToolButton>
#include <QFileDialog>

#include "qtregexpconverter.h"
#include "emacsregexpconverter.h"

VerifyButtons::VerifyButtons(QWidget *parent, const QString &name)
    : QToolBar(name, parent)
    , _configMenu(nullptr)
{
    _verify = new QToolButton(this);
    QIcon icon = QIcon::fromTheme(QStringLiteral("tools-check-spelling"));
    _verify->setIcon(icon);
    _verify->setToolTip(i18n("Verify regular expression"));
    _verify->setWhatsThis(i18n("Shows what part of the regular expression is being matched in the <i>verifier window</i>."
                               "(The window below the graphical editor window)."));
    addWidget(_verify);
    connect(_verify, SIGNAL(clicked()), this, SIGNAL(verify()));

    QToolButton *button = new QToolButton(this);
    button->setIcon(QIcon::fromTheme(QStringLiteral("document-open")));
    addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(loadText()));
    button->setToolTip(i18n("Load text in the verifier window"));

    button = new QToolButton(this);
    button->setIcon(QIcon::fromTheme(QStringLiteral("configure")));
    addWidget(button);
    button->setToolTip(i18n("Verification Settings"));
    button->setPopupMode(QToolButton::InstantPopup);

    // It is currently not possible to ask for the paragraph being highlighted, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // _first = new QToolButton( QString::fromLatin1("<<"), this);
    // layout->addWidget( _first );
    // connect(_first, SIGNAL(clicked()), this, SIGNAL(gotoFirst()));
    // _first->setFixedWidth( 25 );
    //
    // _prev = new QToolButton(QString::fromLatin1("<"), this);
    // layout->addWidget( _prev );
    // connect(_prev, SIGNAL(clicked()), this, SIGNAL(gotoPrev()));
    // _prev->setFixedWidth( 20 );
    //
    // _next = new QToolButton(QString::fromLatin1(">"), this);
    // layout->addWidget( _next );
    // connect(_next, SIGNAL(clicked()), this, SIGNAL(gotoNext()));
    // _next->setFixedWidth( 20 );
    //
    // _last = new QToolButton(QString::fromLatin1(">>"), this);
    // layout->addWidget( _last );
    // connect(_last, SIGNAL(clicked()), this, SIGNAL(gotoLast()));
    // _last->setFixedWidth( 25 );

    // Same as above
//  QLabel* label = new QLabel( i18n("Matches: "), this );
//  layout->addWidget( label );
//  _matches = new QLabel(i18n("-"), this );
//  layout->addWidget( _matches );
//  QString txt = i18n( "Shows number of times regular expression matches the text in the verifier window");
//  label->setToolTip( txt );
//  _matches->setToolTip( txt );

    _verify->setEnabled(false);

    // -------------------------------------------------- RegExp Converters

    // Qt
    RegExpConverter *converter = new QtRegExpConverter();
    _converters.append(qMakePair(converter, static_cast<QAction *>(nullptr)));
    QString qtConverterName = converter->name();

    // Emacs
    converter = new EmacsRegExpConverter();
    _converters.append(qMakePair(converter, static_cast<QAction *>(nullptr)));
    QString emacsConverterName = converter->name();

    // -------------------------------------------------- Initialize the config menu
    _configMenu = new QMenu(i18n("config menu"), this);

    // Auto Verify
    QAction *autoVerify = new QAction(i18n("Verify on the Fly"), this);
    autoVerify->setCheckable(true);
    autoVerify->setChecked(true);
    connect(autoVerify, SIGNAL(toggled(bool)), this, SLOT(updateVerifyButton(bool)));
    connect(autoVerify, SIGNAL(toggled(bool)), this, SIGNAL(autoVerify(bool)));
    _configMenu->addAction(autoVerify);
    autoVerify->setToolTip(i18n("Toggle on-the-fly verification of regular expression"));
    autoVerify->setWhatsThis(i18n("Enabling this option will make the verifier update for each edit. "
                                  "If the verify window contains much text, or if the regular expression is either "
                                  "complex or matches a lot of time, this may be very slow."));

    QAction *matchGreedy = new QAction(i18n("Match Greedy"), this);
    matchGreedy->setCheckable(true);
    matchGreedy->setChecked(false);
    connect(matchGreedy, SIGNAL(toggled(bool)), this, SIGNAL(matchGreedy(bool)));
    _configMenu->addAction(matchGreedy);
    matchGreedy->setToolTip(i18n("Toggle greedy matching when verifying the regular expression."));
    matchGreedy->setWhatsThis(i18n("When this option is enabled, the regular expression will be evaluated on a so-called greedy way."));

    // RegExp Languages
    _languages = new QMenu(i18n("RegExp Language"), _configMenu);
    _configMenu->addMenu(_languages);

    QActionGroup *grp = new QActionGroup(this);
    for (QLinkedList< QPair<RegExpConverter *, QAction *> >::Iterator it = _converters.begin(); it != _converters.end(); ++it) {
        QString name = (*it).first->name();
        QAction *action = new QAction(name, this);
        action->setCheckable(true);
        grp->addAction(action);
        (*it).second = action;
    }

    _languages->addActions(grp->actions());
    connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(slotChangeSyntax(QAction*)));
    _languages->setEnabled(false);

    button->setMenu(_configMenu);

    // Select the Qt converter by default
    setSyntax(qtConverterName);
}

void VerifyButtons::updateVerifyButton(bool b)
{
    _verify->setEnabled(!b);
}

void VerifyButtons::loadText()
{
    const QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), QString());
    if (!fileName.isNull()) {
        Q_EMIT loadVerifyText(fileName);
    }
}

// Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
// do not work. Enable this when they work.
// void VerifyButtons::enableBackwardButtons( bool b )
// {
//     _first->setEnabled( b );
//     _prev->setEnabled( b );
// }
//
// void VerifyButtons::enableForwardButtons( bool b )
// {
//     _next->setEnabled( b );
//     _last->setEnabled( b );
// }

void VerifyButtons::setMatchCount(int /*count*/)
{
// currently this is not possible due to limitation in QSyntaxHighlighter
    /*
      if ( count == -1 ) {
      _matches->setText( QString::fromLatin1("-") );
      } else {
      _matches->setText( QString::number( count ) );
      }
    */
}

void VerifyButtons::slotChangeSyntax(QAction *action)
{
    Q_EMIT changeSyntax(action->text());
}

RegExpConverter *VerifyButtons::setSyntax(const QString &which)
{
    QString noAmpersand = which;
    noAmpersand.remove(QLatin1Char('&')); // HACK, can probably be done more cleanly
    for (QLinkedList< QPair<RegExpConverter *, QAction *> >::Iterator it = _converters.begin(); it != _converters.end(); ++it) {
        QString name = (*it).first->name();
        if (name == noAmpersand) {
            (*it).second->setChecked(true);
            return (*it).first;
        }
    }
    qWarning("No such converter: '%s'", qPrintable(noAmpersand));
    return nullptr;
}

void VerifyButtons::setAllowNonQtSyntax(bool b)
{
    _languages->setEnabled(b);
}
