/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/
#ifndef __pair_h
#define __pair_h

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

#endif // __pair_h
