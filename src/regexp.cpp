/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "regexp.h"

#include "errormap.h"
#include "kregexpeditorwindow.h"
#include "widgetfactory.h"

RegExp::RegExp(bool selected)
    : _parent(nullptr)
    , _destructing(false)
    , _selected(selected)
{
    // Nothing to do
}

RegExp::~RegExp()
{
    _destructing = true;
    qDeleteAll(_children);
    if (_parent) {
        _parent->removeChild(this);
    }
    _parent = nullptr;
}

void RegExp::addChild(RegExp *child)
{
    _children.append(child);
    child->setParent(this);
}

void RegExp::removeChild(RegExp *child)
{
    if (!_destructing) {
        _children.removeOne(child);
    }
}

void RegExp::setParent(RegExp *parent)
{
    _parent = parent;
}

RegExp *RegExp::readRegExp(const QDomElement &top, const QString &version)
{
    for (QDomNode node = top.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement()) {
            continue; // skip past comments
        }

        RegExp *regexp = WidgetFactory::createRegExp(node.toElement(), version);
        return regexp;
    }
    return nullptr;
}

QString RegExp::toXmlString() const
{
    QDomDocument doc;
    doc.setContent(QStringLiteral("<RegularExpression/>"));
    QDomNode top = doc.documentElement();
    top.toElement().setAttribute(QStringLiteral("version"), KRegExpEditorWindow::version);

    QDomNode elm = toXml(&doc);

    top.appendChild(elm);
    QString xmlString = QStringLiteral(
                            "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<!DOCTYPE RegularExpression PUBLIC \"-//KDE//KRegexpEditor DTD 1.0//EN\" "
                            "\"http://www.blackie.dk/kreg.dtd\">\n")
        + doc.toString();

    return xmlString;
}

RegExp *RegExp::clone() const
{
    return WidgetFactory::createRegExp(toXmlString());
}

void RegExp::check(ErrorMap &map)
{
    map.start();
    check(map, true, true);
    map.end();
}
