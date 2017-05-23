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

#ifndef REGEXP_H
#define REGEXP_H

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <QtCore/QList>

class CompoundRegExp;
class ErrorMap;

/**
   Abstract syntax tree for regular expressions.
   @internal
*/
class RegExp
{
public:
    RegExp(bool selected);
    virtual ~RegExp();

    virtual int precedence() const = 0;
    virtual QDomNode toXml(QDomDocument *doc) const = 0;
    virtual bool load(QDomElement, const QString &version) = 0;
    QString toXmlString() const;

    void check(ErrorMap &);
    virtual bool check(ErrorMap &, bool first, bool last) = 0;

    void addChild(RegExp *child);
    void removeChild(RegExp *child);
    void setParent(RegExp *parent);
    RegExp *clone() const;
    virtual bool operator==(const RegExp &other) const
    {
        return type() == other.type();
    }

    enum RegExpType {
        CONC, TEXT, DOT, POSITION, REPEAT, ALTN, COMPOUND, LOOKAHEAD, TEXTRANGE
    };
    virtual RegExpType type() const = 0;
    virtual void replacePart(CompoundRegExp * /* replacement */)
    {
    }

    bool isSelected() const
    {
        return _selected;
    }

    void setSelected(bool b)
    {
        _selected = b;
    }

protected:
    RegExp *readRegExp(QDomElement top, const QString &version);

private:
    RegExp()
    {
    }           // disable

    QList<RegExp *> _children;
    RegExp *_parent;
    bool _destructing;
    bool _selected;
};

typedef QList<RegExp *> RegExpList;
typedef QListIterator<RegExp *> RegExpListIt;
typedef QList<RegExp *>::iterator RegExpListStlIt;

#endif // REGEXP_H
