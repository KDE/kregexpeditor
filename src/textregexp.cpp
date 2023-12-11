/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "textregexp.h"

#include <KLocalizedString>
#include <KMessageBox>

TextRegExp::TextRegExp(bool selected, const QString &text)
    : RegExp(selected)
{
    _text = text;
}

bool TextRegExp::check(ErrorMap &, bool, bool)
{
    return false;
}

void TextRegExp::append(const QString &str)
{
    _text.append(str);
}

QDomNode TextRegExp::toXml(QDomDocument *doc) const
{
    QDomElement top = doc->createElement(QStringLiteral("Text"));
    QDomText text = doc->createTextNode(_text);
    top.appendChild(text);
    return top;
}

bool TextRegExp::load(const QDomElement &top, const QString & /*version*/)
{
    Q_ASSERT(top.tagName() == QStringLiteral("Text"));
    if (top.hasChildNodes()) {
        QDomNode child = top.firstChild();
        if (!child.isText()) {
            KMessageBox::error(nullptr, i18n("<p>Element <b>Text</b> did not contain any textual data.</p>"), i18n("Error While Loading From XML File"));
            return false;
        }
        QDomText txtNode = child.toText();
        _text = txtNode.data();
    } else {
        _text = QString();
    }

    return true;
}

bool TextRegExp::operator==(const RegExp &other) const
{
    if (other.type() != type()) {
        return false;
    }

    const TextRegExp &theOther = dynamic_cast<const TextRegExp &>(other);
    if (text() == theOther.text()) {
        return true;
    }

    return false;
}
