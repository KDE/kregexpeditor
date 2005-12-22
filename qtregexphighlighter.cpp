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
#include "qtregexphighlighter.h"
#include <qregexp.h>
QtRegexpHighlighter::QtRegexpHighlighter( QTextEdit* editor )
    :RegexpHighlighter( editor ), _editor( editor )
{
}

int QtRegexpHighlighter::highlightParagraph( const QString & text, int endStateOfLastPara )
{
    QRegExp regexp( _regexp );
    regexp.setCaseSensitive( _caseSensitive );
    regexp.setMinimal( _minimal );

    setFormat( 0, text.length(), _editor->font(), Qt::black );

    if ( !regexp.isValid() || regexp.isEmpty() ) {
        return 0;
    }

    // ------------------------------ Process with the regular expression.
    QColor colors[] = { Qt::red, Qt::blue };
    int color = endStateOfLastPara;
    if ( color < 0 || color > 1 )
        color = 0;

    int index = 0;
    int start, length;
    while ( (index = regexp.search( text, index ) ) != -1 && index < (int) text.length()) {
        if ( regexp.pos(1) != -1 ) {
            start = regexp.pos(1);
            length = regexp.cap(1).length();
        }
        else {
            start = index;
            length = regexp.matchedLength();
        }

        if ( start != index )
            setFormat( index, start-index, colors[color] );

        QFont font = _editor->font();
        font.setUnderline( true );
        font.setPointSize( (int) (font.pointSize() * 1.3) );
        setFormat( start, length, font, colors[color] );

        if ( length + (start-index) != regexp.matchedLength() )
            setFormat( start+length, regexp.matchedLength()-length-(start-index), colors[color] );

        index +=  QMAX( 1, regexp.matchedLength() ); // ensure progress when matching for example ^ or \b
        color = (color+1)%2;
    }
    return color;
}

