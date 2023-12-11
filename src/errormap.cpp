/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "errormap.h"

#include <KLocalizedString>
#include <KMessageBox>

ErrorMap::ErrorMap()
    : _prevLineStartError(false)
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
        KMessageBox::information(nullptr,
                                 i18n("Your regular expression is invalid, due to something preceding a 'line start'."),
                                 i18n("Regular Expression Error"),
                                 QStringLiteral("KRegExpEditorLineStartError"));
    }
    _lineStartError = true;
}

void ErrorMap::lineEndError()
{
    if (!_prevLineEndError) {
        KMessageBox::information(nullptr,
                                 i18n("Your regular expression is invalid, due to something following a 'line end'."),
                                 i18n("Regular Expression Error"),
                                 QStringLiteral("KRegExpEditorLineEndError"));
    }
    _lineEndError = true;
}

void ErrorMap::lookAheadError()
{
    if (!_prevLookAHeadError) {
        KMessageBox::information(nullptr,
                                 i18n("Your regular expression is invalid. 'Look Ahead' regular expression must be the last sub expression."),
                                 i18n("Regular Expression Error"),
                                 QStringLiteral("KRegExpEditorLookAHeadError"));
    }
    _lookAHeadError = true;
}
