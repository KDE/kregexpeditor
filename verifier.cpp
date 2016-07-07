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

#include "verifier.h"

#include "regexphighlighter.h"

Verifier::Verifier(QWidget *parent) : KTextEdit(parent)
    /* QT_ANCHOR_DO_NOT_WORK: ,_current( 0 ) */
{
    setCheckSpellingEnabled(false);

    _highlighter = 0;
    setMinimumSize(1, 1);
}

/**
   Update text edit to show matches of regular expression
*/
void Verifier::verify(const QString &reg)
{
    if (_highlighter) {
        _highlighter->setRegExp(reg);
        _highlighter->rehighlight();
    }
}

/**
   Make the text edit display the text without any matches.
*/
void Verifier::clearRegexp()
{
    if (_highlighter) {
        _highlighter->setRegExp(QString());
        _highlighter->rehighlight();
    }
}

/**
   Set case sensitive matching
*/
void Verifier::setCaseSensitive(bool b)
{
    if (_highlighter) {
        _highlighter->setCaseSensitive(b);
    }
}

/**
   set minimal mathcing
*/
void Verifier::setMinimal(bool b)
{
    if (_highlighter) {
        _highlighter->setMinimal(b);
    }
}

// Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
// do not work. Enable this when they work.
// void Verifier::gotoFirst()
// {
//     gotoNum(1);
// }
//
// void Verifier::gotoLast()
// {
//     gotoNum( _count-1 );
// }
//
// void Verifier::gotoPrev()
// {
//     gotoNum( _current-1 );
// }
//
// void Verifier::gotoNext()
// {
//     gotoNum( _current+1 );
// }
//
// void Verifier::gotoNum( int which )
// {
//     QString anchor = QString::fromLatin1("match%1").arg(which);
//     scrollToAnchor( anchor );
//     _current = which;
//     emit currentChanged( _current );
//     emit goBackwardPossible( which != 0 );
//     emit goForwardPossible( which != _count -1 );
// }

void Verifier::setHighlighter(RegexpHighlighter *highlighter)
{
    if (_highlighter) {
        _highlighter->deleteLater();
    }
    _highlighter = highlighter;
    if (_highlighter) {
        setEnabled(true);
    } else {
        setEnabled(false);
    }
}

