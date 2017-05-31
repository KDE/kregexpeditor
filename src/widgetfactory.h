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

#ifndef widgetfactory
#define widgetfactory

#include <QtXml/QDomElement>

class RegExpWidget;
class RegExpEditorWindow;
class QWidget;
class RegExp;

/**
   All the different regular expression types.
*/
enum RegExpType {
    TEXT = 0, CHARSET = 1, DOT = 2, REPEAT = 3, ALTN = 4, COMPOUND = 5, BEGLINE = 6,
    ENDLINE = 7, WORDBOUNDARY = 8, NONWORDBOUNDARY = 9, CONC = 10, DRAGACCEPTER = 11,
    POSLOOKAHEAD = 12, NEGLOOKAHEAD = 13
};

/**
   Class used to encapsulate information about widgets.

   When reading widgets from a stream, it is necessary to know about all
   sub-widgets to the @ref RegExpWidget class. This class exists to make
   sure that such general information is only kept once.

   @internal
*/
class WidgetFactory
{
public:
    static RegExpWidget *createWidget(RegExpEditorWindow *editorWindow, QWidget *parent, RegExpType type);
    static RegExpWidget *createWidget(RegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent);
    static RegExp *createRegExp(QDomElement node, const QString &version);
    static RegExp *createRegExp(const QString &str);
    static bool isContainer(RegExpType);
};

#endif // widgetfactory
