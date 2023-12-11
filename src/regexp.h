/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef REGEXP_H
#define REGEXP_H

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QList>

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
    virtual bool load(const QDomElement &, const QString &version) = 0;
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
    RegExp *readRegExp(const QDomElement &top, const QString &version);

private:
    RegExp()
    {
    }           // disable

    QList<RegExp *> _children;
    RegExp *_parent = nullptr;
    bool _destructing;
    bool _selected;
};

typedef QList<RegExp *> RegExpList;
typedef QListIterator<RegExp *> RegExpListIt;
typedef QList<RegExp *>::iterator RegExpListStlIt;

#endif // REGEXP_H
