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

#include "errormap.h"

#include <KMessageBox>
#include <KLocalizedString>

ErrorMap::ErrorMap() : _prevLineStartError(false)
    , _prevLineEndError(false)
    , _prevLookAHeadError(false)
{
}

void ErrorMap::start()
{
    _lineStartError = false;
    _lineEndError = false;
    _lookAHeadError = false;
}

void ErrorMap::end()
{
    _prevLineStartError = _lineStartError;
    _prevLineEndError = _lineEndError;
    _prevLookAHeadError = _lookAHeadError;
}

void ErrorMap::lineStartError()
{
    if (!_prevLineStartError) {
        KMessageBox::information(nullptr, i18n("Your regular expression is invalid, due to something preceding a 'line start'."),
                                 i18n("Regular Expression Error"), QStringLiteral("KRegExpEditorLineStartError"));
    }
    _lineStartError = true;
}

void ErrorMap::lineEndError()
{
    if (!_prevLineEndError) {
        KMessageBox::information(nullptr, i18n("Your regular expression is invalid, due to something following a 'line end'."),
                                 i18n("Regular Expression Error"), QStringLiteral("KRegExpEditorLineEndError"));
    }
    _lineEndError = true;
}

void ErrorMap::lookAheadError()
{
    if (!_prevLookAHeadError) {
        KMessageBox::information(nullptr, i18n("Your regular expression is invalid. 'Look Ahead' regular expression must be the last sub expression."),
                                 i18n("Regular Expression Error"), QStringLiteral("KRegExpEditorLookAHeadError"));
    }
    _lookAHeadError = true;
}
