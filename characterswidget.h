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

#ifndef characterswidget
#define characterswidget

#include <QtWidgets/QDialog>

#include "kmultiformlistbox.h"
#include "kmultiformlistboxfactory.h"
#include "regexpwidget.h"

class CharacterEdits;
class TextRangeRegExp;
class CharSelector;
class QCheckBox;

/**
   RegExp widget for charcter ranges.
   @internal
*/
class CharactersWidget :public RegExpWidget
{
public:
    CharactersWidget(RegExpEditorWindow* editorWindow, QWidget *parent);
    CharactersWidget( TextRangeRegExp* regexp, RegExpEditorWindow* editorWindow,
                      QWidget* parent );
    ~CharactersWidget();
    virtual QSize sizeHint() const;
	virtual RegExp* regExp() const;
    virtual RegExpType type() const { return CHARSET; }
    virtual RegExpWidget* findWidgetToEdit( QPoint globalPos );
    virtual int edit();

protected:
    virtual void paintEvent(QPaintEvent *event);
    QString text() const;
    QString title() const;

private:
    TextRangeRegExp *_regexp;
    static CharacterEdits *_configWindow;

    mutable QSize _textSize;
    mutable QSize _contentSize;
};


/**
   @internal
*/
class SingleEntry :public KMultiFormListBoxEntry
{
public:
    SingleEntry(QWidget* parent );
    QString text() const;
    void setText( QString text );
    bool isEmpty() const;

private:
    CharSelector* _selector;
};

/**
   @internal
*/
class RangeEntry :public KMultiFormListBoxEntry
{
public:
    RangeEntry(QWidget* parent );
    QString fromText() const;
    QString toText() const;
    void setFrom( QString text );
    void setTo( QString text );
    bool isEmpty() const;
private:
    CharSelector *_from, *_to;
};

/**
   @internal
*/
class SingleFactory :public KMultiFormListBoxFactory
{
public:
    KMultiFormListBoxEntry *create(QWidget *parent) { return new SingleEntry( parent ); }
    QWidget *separator( QWidget* ) { return 0; }
};

/**
   @internal
*/
class RangeFactory :public KMultiFormListBoxFactory
{
public:
    KMultiFormListBoxEntry *create(QWidget *parent) { return new RangeEntry( parent ); }
    QWidget *separator( QWidget* ) { return 0; }
};

/**
   @internal
*/
class CharacterEdits : public QDialog
{
    Q_OBJECT
public:
    CharacterEdits(QWidget *parent = 0);

public slots:
    int exec( TextRangeRegExp* regexp );

protected slots:
    void slotOK();

private:
    QCheckBox *negate, *wordChar, *_nonWordChar, *digit, *_nonDigit, *space, *_nonSpace;
    KMultiFormListBox *_single, *_range;

    void addCharacter( QString txt );
    void addRange( QString from, QString to );
    TextRangeRegExp* _regexp;
};

#endif // characterswidget
