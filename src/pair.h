/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/
#pragma once

/**
   @internal
*/
template<class F, class S>
class Pair
{
public:
    Pair()
    {
    }

    Pair(F first, S second)
        : _first(first)
        , _second(second)
    {
    }

    F first()
    {
        return _first;
    }

    S second()
    {
        return _second;
    }

private:
    F _first;
    S _second;
};
