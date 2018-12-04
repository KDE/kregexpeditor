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

#include "textrangeregexp.h"

#include <KLocalizedString>
#include <QMessageBox>

#include "regexpconverter.h"

TextRangeRegExp::TextRangeRegExp(bool selected) : RegExp(selected)
    , _negate(false)
    , _digit(false)
    , _nonDigit(false)
    , _space(false)
    , _nonSpace(false)
    , _wordChar(false)
    , _nonWordChar(false)
{
}

TextRangeRegExp::~TextRangeRegExp()
{
}

void TextRangeRegExp::addCharacter(const QString &str)
{
    _chars.append(str);
}

void TextRangeRegExp::addRange(const QString &from, const QString &to)
{
    _ranges.append(StringPair(from, to));
}

bool TextRangeRegExp::check(ErrorMap &, bool, bool)
{
    return false;
}

QDomNode TextRangeRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top = doc->createElement(QStringLiteral("TextRange"));

    if (_negate) {
        top.setAttribute(QStringLiteral("negate"), true);
    }

    if (_digit) {
        top.setAttribute(QStringLiteral("digit"), true);
    }

    if (_nonDigit) {
        top.setAttribute(QStringLiteral("nonDigit"), true);
    }

    if (_space) {
        top.setAttribute(QStringLiteral("space"), true);
    }

    if (_nonSpace) {
        top.setAttribute(QStringLiteral("nonSpace"), true);
    }

    if (_wordChar) {
        top.setAttribute(QStringLiteral("wordChar"), true);
    }

    if (_nonWordChar) {
        top.setAttribute(QStringLiteral("nonWordChar"), true);
    }

    for (QStringList::ConstIterator it = _chars.begin(); it != _chars.end(); ++it) {
        QDomElement elm = doc->createElement(QStringLiteral("Character"));
        elm.setAttribute(QStringLiteral("char"), *it);
        top.appendChild(elm);
    }

    foreach (const StringPair &pair, _ranges) {
        QDomElement elm = doc->createElement(QStringLiteral("Range"));
        elm.setAttribute(QStringLiteral("from"), pair.first);
        elm.setAttribute(QStringLiteral("to"), pair.second);
        top.appendChild(elm);
    }
    return top;
}

bool TextRangeRegExp::load(const QDomElement &top, const QString & /*version*/)
{
    Q_ASSERT(top.tagName() == QStringLiteral("TextRange"));
    QString str;
    QString one = QStringLiteral("1");
    QString zero = QStringLiteral("0");

    str = top.attribute(QStringLiteral("negate"), zero);
    _negate = (str == one);

    str = top.attribute(QStringLiteral("digit"), zero);
    _digit = (str == one);

    str = top.attribute(QStringLiteral("nonDigit"), zero);
    _nonDigit = (str == one);

    str = top.attribute(QStringLiteral("space"), zero);
    _space = (str == one);

    str = top.attribute(QStringLiteral("nonSpace"), zero);
    _nonSpace = (str == one);

    str = top.attribute(QStringLiteral("wordChar"), zero);
    _wordChar = (str == one);

    str = top.attribute(QStringLiteral("nonWordChar"), zero);
    _nonWordChar = (str == one);

    for (QDomNode node = top.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement()) {
            continue; // Skip comments.
        }
        QDomElement child = node.toElement();

        if (child.tagName() == QStringLiteral("Character")) {
            QString ch = child.attribute(QStringLiteral("char"));
            addCharacter(ch);
        } else if (child.tagName() == QStringLiteral("Range")) {
            QString from = child.attribute(QStringLiteral("from"));
            QString to = child.attribute(QStringLiteral("to"));
            addRange(from, to);
        } else {
            QMessageBox::warning(nullptr,
                    i18n("Error While Loading From XML File"),
                    i18n("<p>Invalid sub element to element <b>TextRange</b>. Tag was <b>%1</b></p>", child.tagName())
            );
            return false;
        }
    }
    return true;
}

bool TextRangeRegExp::operator==(const RegExp &other) const
{
    return RegExpConverter::current()->toStr(const_cast<TextRangeRegExp *>(this), false)
           == RegExpConverter::current()->toStr(const_cast<RegExp *>(&other), false);
}
