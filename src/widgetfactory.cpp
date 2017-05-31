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

#include "widgetfactory.h"

#include <KMessageBox>
#include <QDebug>

#include "repeatwidget.h"
#include "textwidget.h"
#include "characterswidget.h"
#include "altnwidget.h"
#include "zerowidgets.h"
#include "compoundwidget.h"
#include "concwidget.h"
#include "lookaheadwidget.h"

#include "textregexp.h"
#include "textrangeregexp.h"
#include "repeatregexp.h"
#include "lookaheadregexp.h"
#include "concregexp.h"
#include "altnregexp.h"
#include "positionregexp.h"
#include "dotregexp.h"
#include "compoundregexp.h"
#include "kregexpeditorgui.h"

bool WidgetFactory::isContainer(RegExpType tp)
{
    return tp == REPEAT || tp == ALTN || tp == COMPOUND;
}

RegExpWidget *WidgetFactory::createWidget(RegExpEditorWindow *win, QWidget *parent, RegExpType type)
{
    RegExpWidget *widget = nullptr;

    switch (type) {
    case TEXT:
        return new TextWidget(win, parent);
        break;
    case ALTN:
        return new AltnWidget(win, parent);
        break;
    case DOT:
        return new AnyCharWidget(win, parent);
        break;
    case BEGLINE:
        return new BegLineWidget(win, parent);
        break;
    case ENDLINE:
        return new EndLineWidget(win, parent);
        break;
    case WORDBOUNDARY:
        return new WordBoundaryWidget(win, parent);
        break;
    case NONWORDBOUNDARY:
        return new NonWordBoundaryWidget(win, parent);
        break;
    case POSLOOKAHEAD:
    case NEGLOOKAHEAD:
        return new LookAheadWidget(win, type, parent);
        break;
    case REPEAT:
        widget = new RepeatWidget(win, parent);
        break;
    case CHARSET:
        widget = new CharactersWidget(win, parent);
        break;
    case COMPOUND:
        widget = new CompoundWidget(win, parent);
        break;
    default:
        qFatal("It should not be possible to get here!");
        return nullptr;
    }

    if (widget->edit() == QDialog::Rejected) {
        delete widget;
        return nullptr;
    }
    return widget;
}

RegExpWidget *WidgetFactory::createWidget(RegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent)
{
    if (regexp == 0) {
        qFatal("%s:%d Regexp is 0", __FILE__, __LINE__);
    } else if (TextRegExp *reg = dynamic_cast<TextRegExp *>(regexp)) {
        return new TextWidget(reg, editorWindow, parent);
    } else if (TextRangeRegExp *reg = dynamic_cast<TextRangeRegExp *>(regexp)) {
        return new CharactersWidget(reg, editorWindow, parent);
    } else if (RepeatRegExp *reg = dynamic_cast<RepeatRegExp *>(regexp)) {
        return new RepeatWidget(reg, editorWindow, parent);
    } else if (LookAheadRegExp *reg = dynamic_cast<LookAheadRegExp *>(regexp)) {
        if (reg->lookAheadType() == LookAheadRegExp::POSITIVE) {
            return new LookAheadWidget(reg, editorWindow, POSLOOKAHEAD, parent);
        } else {
            return new LookAheadWidget(reg, editorWindow, NEGLOOKAHEAD, parent);
        }
    } else if (ConcRegExp *reg = dynamic_cast<ConcRegExp *>(regexp)) {
        return new ConcWidget(reg, editorWindow, parent);
    } else if (AltnRegExp *reg = dynamic_cast<AltnRegExp *>(regexp)) {
        return new AltnWidget(reg, editorWindow, parent);
    } else if (PositionRegExp *reg = dynamic_cast<PositionRegExp *>(regexp)) {
        switch (reg->position()) {
        case PositionRegExp::BEGLINE:
            return new BegLineWidget(editorWindow, parent);
        case PositionRegExp::ENDLINE:
            return new EndLineWidget(editorWindow, parent);
        case PositionRegExp::WORDBOUNDARY:
            return new WordBoundaryWidget(editorWindow, parent);
        case PositionRegExp::NONWORDBOUNDARY:
            return new NonWordBoundaryWidget(editorWindow, parent);
        }
    } else if (dynamic_cast<DotRegExp *>(regexp)) {
        return new AnyCharWidget(editorWindow, parent);
    } else if (CompoundRegExp *reg = dynamic_cast<CompoundRegExp *>(regexp)) {
        return new CompoundWidget(reg, editorWindow, parent);
    } else {
        qFatal("%s:%d Internal Error: Unknown RegExp type", __FILE__, __LINE__);
    }
    return nullptr;
}

RegExp *WidgetFactory::createRegExp(QDomElement node, const QString &version)
{
    QString tag = node.tagName();
    RegExp *regexp;
    if (tag == QString::fromLocal8Bit("TextRange")) {
        regexp = new TextRangeRegExp(false);
    } else if (tag == QString::fromLocal8Bit("Text")) {
        regexp = new TextRegExp(false);
    } else if (tag == QString::fromLocal8Bit("Concatenation")) {
        regexp = new ConcRegExp(false);
    } else if (tag == QString::fromLocal8Bit("Alternatives")) {
        regexp = new AltnRegExp(false);
    } else if (tag == QString::fromLocal8Bit("BegLine")) {
        regexp = new PositionRegExp(false, PositionRegExp::BEGLINE);
    } else if (tag == QString::fromLocal8Bit("EndLine")) {
        regexp = new PositionRegExp(false, PositionRegExp::ENDLINE);
    } else if (tag == QString::fromLocal8Bit("WordBoundary")) {
        regexp = new PositionRegExp(false, PositionRegExp::WORDBOUNDARY);
    } else if (tag == QString::fromLocal8Bit("NonWordBoundary")) {
        regexp = new PositionRegExp(false, PositionRegExp::NONWORDBOUNDARY);
    } else if (tag == QString::fromLocal8Bit("PositiveLookAhead")) {
        regexp = new LookAheadRegExp(false, LookAheadRegExp::POSITIVE);
    } else if (tag == QString::fromLocal8Bit("NegativeLookAhead")) {
        regexp = new LookAheadRegExp(false, LookAheadRegExp::NEGATIVE);
    } else if (tag == QString::fromLocal8Bit("Compound")) {
        regexp = new CompoundRegExp(false);
    } else if (tag == QString::fromLocal8Bit("AnyChar")) {
        regexp = new DotRegExp(false);
    } else if (tag == QString::fromLocal8Bit("Repeat")) {
        regexp = new RepeatRegExp(false);
    } else {
        KMessageBox::sorry(nullptr, i18n("<p>Unknown tag while reading XML. Tag was <b>%1</b></p>", tag),
                           i18n("Error While Loading From XML File"));

        return nullptr;
    }

    bool ok = regexp->load(node, version);
    if (ok) {
        return regexp;
    } else {
        delete regexp;
        return nullptr;
    }
    return nullptr;
}

RegExp *WidgetFactory::createRegExp(const QString &str)
{
    QDomDocument doc;
    QString error;
    int errorLine, errorCol;
    bool ok = doc.setContent(str, &error, &errorLine, &errorCol);
    if (!ok) {
        qDebug() << error << "at line" << errorLine << "xml was:";
        qDebug() << str;
        KMessageBox::sorry(nullptr, i18n("Error while loading regular expression from XML.") + QLatin1Char('\n') + error,
                           i18n("Error While Loading Regular Expression From XML"));
    }

    // Read the RegularExpression element, and extract the version.
    QDomElement top = doc.documentElement();
    if (!(top.tagName() == QString::fromLocal8Bit("RegularExpression"))) {
        KMessageBox::sorry(nullptr, i18n("<p>XML file did not contain a <b>%1</b> tag.</p>", QString::fromLatin1("RegularExpression")),
                           i18n("Error While Loading From XML File"));
    }
    QString version = top.attribute(QString::fromLocal8Bit("version"), KRegExpEditorGUI::version);
    QDomNode child = top.firstChild();
    if (!child.isElement()) {
        KMessageBox::sorry(nullptr, i18n("<p>Error while reading XML file. The element just below the tag "
                                         "<b>%1</b> was not an element.</p>", QString::fromLatin1("RegularExpression")),
                           i18n("Error While Loading From XML File"));
    }

    RegExp *regexp = WidgetFactory::createRegExp(child.toElement(), version);

    return regexp;
}
