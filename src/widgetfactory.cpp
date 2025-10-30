/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#include "widgetfactory.h"

#include <KMessageBox>
#include <QDebug>

#include "altnwidget.h"
#include "characterswidget.h"
#include "compoundwidget.h"
#include "concwidget.h"
#include "lookaheadwidget.h"
#include "repeatwidget.h"
#include "textwidget.h"
#include "zerowidgets.h"

#include "altnregexp.h"
#include "compoundregexp.h"
#include "concregexp.h"
#include "dotregexp.h"
#include "kregexpeditorwindow.h"
#include "lookaheadregexp.h"
#include "positionregexp.h"
#include "repeatregexp.h"
#include "textrangeregexp.h"
#include "textregexp.h"

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
    case ALTN:
        return new AltnWidget(win, parent);
    case DOT:
        return new AnyCharWidget(win, parent);
    case BEGLINE:
        return new BegLineWidget(win, parent);
    case ENDLINE:
        return new EndLineWidget(win, parent);
    case WORDBOUNDARY:
        return new WordBoundaryWidget(win, parent);
    case NONWORDBOUNDARY:
        return new NonWordBoundaryWidget(win, parent);
    case POSLOOKAHEAD:
    case NEGLOOKAHEAD:
        return new LookAheadWidget(win, type, parent);
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
    if (regexp == nullptr) {
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

RegExp *WidgetFactory::createRegExp(const QDomElement &node, const QString &version)
{
    QString tag = node.tagName();
    RegExp *regexp;
    if (tag == QStringLiteral("TextRange")) {
        regexp = new TextRangeRegExp(false);
    } else if (tag == QStringLiteral("Text")) {
        regexp = new TextRegExp(false);
    } else if (tag == QStringLiteral("Concatenation")) {
        regexp = new ConcRegExp(false);
    } else if (tag == QStringLiteral("Alternatives")) {
        regexp = new AltnRegExp(false);
    } else if (tag == QStringLiteral("BegLine")) {
        regexp = new PositionRegExp(false, PositionRegExp::BEGLINE);
    } else if (tag == QStringLiteral("EndLine")) {
        regexp = new PositionRegExp(false, PositionRegExp::ENDLINE);
    } else if (tag == QStringLiteral("WordBoundary")) {
        regexp = new PositionRegExp(false, PositionRegExp::WORDBOUNDARY);
    } else if (tag == QStringLiteral("NonWordBoundary")) {
        regexp = new PositionRegExp(false, PositionRegExp::NONWORDBOUNDARY);
    } else if (tag == QStringLiteral("PositiveLookAhead")) {
        regexp = new LookAheadRegExp(false, LookAheadRegExp::POSITIVE);
    } else if (tag == QStringLiteral("NegativeLookAhead")) {
        regexp = new LookAheadRegExp(false, LookAheadRegExp::NEGATIVE);
    } else if (tag == QStringLiteral("Compound")) {
        regexp = new CompoundRegExp(false);
    } else if (tag == QStringLiteral("AnyChar")) {
        regexp = new DotRegExp(false);
    } else if (tag == QStringLiteral("Repeat")) {
        regexp = new RepeatRegExp(false);
    } else {
        KMessageBox::error(nullptr, i18n("<p>Unknown tag while reading XML. Tag was <b>%1</b></p>", tag), i18n("Error While Loading From XML File"));

        return nullptr;
    }

    bool ok = regexp->load(node, version);
    if (ok) {
        return regexp;
    }

    delete regexp;
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
        KMessageBox::error(nullptr,
                           i18n("Error while loading regular expression from XML.") + QLatin1Char('\n') + error,
                           i18n("Error While Loading Regular Expression From XML"));
    }

    // Read the RegularExpression element, and extract the version.
    QDomElement top = doc.documentElement();
    if (!(top.tagName() == QStringLiteral("RegularExpression"))) {
        KMessageBox::error(nullptr,
                           i18n("<p>XML file did not contain a <b>%1</b> tag.</p>", QStringLiteral("RegularExpression")),
                           i18n("Error While Loading From XML File"));
    }
    QString version = top.attribute(QStringLiteral("version"), KRegExpEditorWindow::version);
    QDomNode child = top.firstChild();
    if (!child.isElement()) {
        KMessageBox::error(nullptr,
                           i18n("<p>Error while reading XML file. The element just below the tag "
                                "<b>%1</b> was not an element.</p>",
                                QStringLiteral("RegularExpression")),
                           i18n("Error While Loading From XML File"));
    }

    RegExp *regexp = WidgetFactory::createRegExp(child.toElement(), version);

    return regexp;
}
