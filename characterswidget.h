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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/
#ifndef characterswidget
#define characterswidget

#ifdef QT_ONLY
#include "compat.h"
#else
#include <kdialogbase.h>
#endif

#include <kmultiformlistbox.h>
#include "regexpwidget.h"

//Added by qt3to4:
#include <QPaintEvent>

class KDialogBase;
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
    CharactersWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                     const char *label = 0);
    CharactersWidget( TextRangeRegExp* regexp, RegExpEditorWindow* editorWindow,
                      QWidget* parent, const char* name = 0 );
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
    SingleEntry(QWidget* parent, const char* name = 0 );
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
    RangeEntry(QWidget* parent, const char* name = 0 );
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
class CharacterEdits : public KDialogBase
{
    Q_OBJECT
public:
    CharacterEdits(QWidget *parent = 0, const char *name = 0);

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
