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

#include "regexpbuttons.h"

#include <QButtonGroup>
#include <QSignalMapper>

#include <KIconLoader>
#include <KLocalizedString>
#include <QStandardPaths>

#include "dcbutton.h"
#include "regexpconverter.h"

RegExpButtons::RegExpButtons(QWidget *parent, const QString &name)
    : QToolBar(name, parent)
    , _keepMode(false)
{
    _grp = new QButtonGroup(this);
    _grp->setExclusive(true);

    _mapper = new QSignalMapper(this);
    _mapper->setObjectName(QStringLiteral("RegExpButtons::_mapper"));
    connect(_mapper, SIGNAL(mapped(int)), this, SIGNAL(clicked(int)));

    // The "select" button.
    _selectBut = new QToolButton(this);

    QPixmap pix = KIconLoader::global()->loadIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString::fromLatin1("kregexpeditor/pics/select.png")), KIconLoader::Toolbar);

    _selectBut->setIcon(static_cast<QIcon>(pix));
    addWidget(_selectBut);
    _grp->addButton(_selectBut);
    _selectBut->setCheckable(true);
    connect(_selectBut, SIGNAL(clicked()), SIGNAL(doSelect()));
    connect(_selectBut, SIGNAL(clicked()), this, SLOT(slotSetNonKeepMode()));

    _selectBut->setToolTip(i18n("Selection tool"));
    _selectBut->setWhatsThis(i18n("<p>This will change the state of the editor to <i>selection state</i>.</p>"
                                  "<p>In this state you will not be inserting <i>regexp items</i>, but instead select them. "
                                  "To select a number of items, press down the left mouse button and drag it over the items.</p>"
                                  "<p>When you have selected a number of items, you may use cut/copy/paste. These functions are "
                                  "found in the right mouse button menu.</p>"));

    // Insert buttons.
    DoubleClickButton *but;

    but = insert(TEXT, "text", i18n("Text"),
                 i18n("<qt>This will insert a text field, where you may write text. The text you write will "
                      "be matched literally. (i.e. you do not need to escape any characters)</qt>"));
    addWidget(but);

    but = insert(CHARSET, "characters", i18n("A single character specified in a range"),
                 i18n("<p>This will match a single character from a predefined range.</p>"
                      "<p>When you insert this widget a dialog box will appear, which lets you specify "
                      "which characters this <i>regexp item</i> will match.</p>"));
    addWidget(but);

    but = insert(DOT, "anychar", i18n("Any character"),
                 i18n("<qt>This will match any single character</qt>"));
    addWidget(but);

    but = insert(REPEAT, "repeat", i18n("Repeated content"),
                 i18n("<qt>This <i>regexp item</i> will repeat the <i>regexp items</i> it surrounds "
                      "a specified number of times.<br />"
                      "The number of times to repeat may be specified using ranges; e.g. it could be specified "
                      "that it should match from 2 to 4 times, that it should match exactly 5 times, or "
                      "that it should match at least one time.<br />"
                      "Examples:<br />"
                      "If you specify that it should match <i>any</i> time, and the content it surrounds "
                      "is <tt>abc</tt>, then this <i>regexp item</i> will match the empty string, "
                      "the string <tt>abc</tt>, the string <tt>abcabc</tt>, the string <tt>abcabcabcabc</tt>, "
                      "etc.</qt>"));
    addWidget(but);

    but = insert(ALTN, "altn", i18n("Alternatives"),
                 i18n("<p>This <i>regexp item</i> will match any of its alternatives.</p>"
                      "<p>Alternatives are specified by placing <i>regexp items</i> on top of "
                      "each other inside this widget.</p>"));
    addWidget(but);

    but = insert(COMPOUND, "compound", i18n("Compound regexp"),
                 i18n("<qt>This <i>regexp item</i> serves two purposes:"
                      "<ul><li>It makes it possible for you to collapse a huge <i>regexp item</i> into "
                      "a small box. This makes it easier for you to get an overview of large "
                      "<i>regexp items</i>. This is especially useful if you load a predefined <i>regexp item</i> "
                      "that you perhaps do not care about the inner workings of.</qt>"));
    addWidget(but);

    but = insert(BEGLINE, "begline", i18n("Beginning of line"),
                 i18n("<qt>This will match the beginning of a line.</qt>"));
    addWidget(but);

    but = insert(ENDLINE, "endline", i18n("End of line"),
                 i18n("<qt>This will match the end of a line.</qt>"));
    addWidget(but);

    _wordBoundary = insert(WORDBOUNDARY, "wordboundary", i18n("Word boundary"),
                           i18n("<qt>This asserts a word boundary (This part does not actually match any characters)</qt>"));
    addWidget(_wordBoundary);

    _nonWordBoundary = insert(NONWORDBOUNDARY, "nonwordboundary", i18n("Non Word boundary"),
                              i18n("<qt>This asserts a non-word boundary "
                                   "(This part does not actually match any characters)</qt>"));
    addWidget(_nonWordBoundary);

    _posLookAhead = insert(POSLOOKAHEAD, "poslookahead", i18n("Positive Look Ahead"),
                           i18n("<qt>This asserts a regular expression (This part does not actually match any characters). "
                                "You can only use this at the end of a regular expression.</qt>"));
    addWidget(_posLookAhead);

    _negLookAhead = insert(NEGLOOKAHEAD, "neglookahead", i18n("Negative Look Ahead"),
                           i18n("<qt>This asserts a regular expression that must not match "
                                "(This part does not actually match any characters). "
                                "You can only use this at the end of a regular expression.</qt>"));
    addWidget(_negLookAhead);
}

DoubleClickButton *RegExpButtons::insert(RegExpType tp, const char *name, const QString &tooltip, const QString &whatsthis)
{
    QPixmap pix
        = KIconLoader::global()->loadIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation, QString::fromLatin1("kregexpeditor/pics/") + QString::fromLatin1(name) + QString::fromLatin1(
                                                                     ".png")), KIconLoader::Toolbar);

    DoubleClickButton *but = new DoubleClickButton(pix, this, QStringLiteral("RegExpButtons::but"));

    _mapper->setMapping(but, tp);

    connect(but, SIGNAL(clicked()), _mapper, SLOT(map()));
    connect(but, SIGNAL(clicked()), this, SLOT(slotSetNonKeepMode()));
    connect(but, SIGNAL(doubleClicked()), this, SLOT(slotSetKeepMode()));

    _grp->addButton(but);
    but->setCheckable(true);
    but->setToolTip(tooltip);
    but->setWhatsThis(whatsthis);

    return but;
}

void RegExpButtons::slotUnSelect()
{
    if (_grp->checkedId() != -1) {
        QToolButton *pb = static_cast<QToolButton *>(_grp->checkedButton());
        if (pb) {
            pb->setChecked(false);
        }
    }
}

void RegExpButtons::slotSetKeepMode()
{
    _keepMode = true;
}

void RegExpButtons::slotSetNonKeepMode()
{
    _keepMode = false;
}

void RegExpButtons::slotSelectNewAction()
{
    if (!_keepMode) {
        emit doSelect();
        _selectBut->click();
    }
}

void RegExpButtons::setFeatures(int features)
{
    _wordBoundary->setVisible(features & RegExpConverter::WordBoundary);
    _nonWordBoundary->setVisible(features & RegExpConverter::NonWordBoundary);
    _posLookAhead->setVisible(features & RegExpConverter::PosLookAhead);
    _negLookAhead->setVisible(features & RegExpConverter::NegLookAhead);
}
