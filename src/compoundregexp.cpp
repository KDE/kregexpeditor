/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "compoundregexp.h"

#include <KLocalizedString>

#include "widgetfactory.h"

CompoundRegExp::CompoundRegExp(bool selected, const QString &title, const QString &description, bool hidden, bool allowReplace, RegExp *child)
    : RegExp(selected)
    , _title(title)
    , _description(description)
    , _hidden(hidden)
    , _allowReplace(allowReplace)
    , _child(child)
{
    if (child) {
        addChild(child);
    }
}

bool CompoundRegExp::check(ErrorMap &map, bool first, bool last)
{
    return _child->check(map, first, last);
}

QDomNode CompoundRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top = doc->createElement(QStringLiteral("Compound"));
    if (_hidden) {
        top.setAttribute(QStringLiteral("hidden"), true);
    }

    if (_allowReplace) {
        top.setAttribute(QStringLiteral("allowReplace"), true);
    }

    QDomElement title = doc->createElement(QStringLiteral("Title"));
    QDomText titleTxt = doc->createTextNode(_title);
    title.appendChild(titleTxt);
    top.appendChild(title);

    QDomElement description = doc->createElement(QStringLiteral("Description"));
    QDomText descriptionTxt = doc->createTextNode(_description);
    description.appendChild(descriptionTxt);
    top.appendChild(description);

    top.appendChild(_child->toXml(doc));

    return top;
}

bool CompoundRegExp::load(const QDomElement &top, const QString &version)
{
    Q_ASSERT(top.tagName() == QStringLiteral("Compound"));
    QString str = top.attribute(QStringLiteral("hidden"), QStringLiteral("0"));
    _hidden = true; // alway hidden. (str == QStringLiteral("1") );

    str = top.attribute(QStringLiteral("allowReplace"), QStringLiteral("0"));
    _allowReplace = (str == QStringLiteral("1"));

    for (QDomNode node = top.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement()) {
            continue; // skip past comments.
        }

        QString txt;
        QDomElement child = node.toElement();
        QDomNode txtNode = child.firstChild();
        if (txtNode.isText()) {
            txt = txtNode.toText().data();
        }

        if (child.tagName() == QStringLiteral("Title")) {
            if (txt.isEmpty()) {
                _title = txt;
            } else {
                _title = i18n(txt.toUtf8().data());
            }
        } else if (child.tagName() == QStringLiteral("Description")) {
            if (txt.isEmpty()) {
                _description = txt;
            } else {
                _description = i18n(txt.toUtf8().data());
            }
        } else {
            _child = WidgetFactory::createRegExp(child, version);
            if (_child) {
                addChild(_child);
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

bool CompoundRegExp::operator==(const RegExp &other) const
{
    // Note the order is important in the comparison below.
    // Using other as the first argument, means that
    // the following will be evaluated: other.operator== rather than (*child).operator==
    // This means that if other is a CompoundRegExp too, then this level will be striped.
    return other == *_child;
}
