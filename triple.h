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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
#ifndef __triple_h
#define __triple_h

/**
   @internal
*/
template<class F, class S, class T> class Triple
{
public:
  Triple() {};
	Triple(F first, S second, T third) : _first(first), _second(second),
                                       _third(third) {}
	F first() { return _first; }
	S second() { return _second; }
	T third() { return _third; }
private:
	F _first;
	S _second;
	T _third;

};

#endif // __triple_h
