/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

class ErrorMap
{
public:
    ErrorMap();
    void start();
    void end();
    void lineStartError();
    void lineEndError();
    void lookAheadError();

private:
    bool _lineStartError, _prevLineStartError;
    bool _lineEndError, _prevLineEndError;
    bool _lookAHeadError, _prevLookAHeadError;
};
