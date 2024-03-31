/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include <QDialog>

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
class CharactersWidget : public RegExpWidget
{
public:
    CharactersWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    CharactersWidget(TextRangeRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent);
    ~CharactersWidget() override;
    QSize sizeHint() const override;
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return CHARSET;
    }

    RegExpWidget *findWidgetToEdit(QPointF globalPos) override;
    int edit() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    QString text() const;
    QString title() const;

private:
    TextRangeRegExp *_regexp = nullptr;
    static CharacterEdits *_configWindow;

    mutable QSize _textSize;
    mutable QSize _contentSize;
};

/**
   @internal
*/
class SingleEntry : public KMultiFormListBoxEntry
{
public:
    explicit SingleEntry(QWidget *parent);
    QString text() const;
    void setText(const QString &text);
    bool isEmpty() const;

private:
    CharSelector *_selector = nullptr;
};

/**
   @internal
*/
class RangeEntry : public KMultiFormListBoxEntry
{
public:
    explicit RangeEntry(QWidget *parent);
    QString fromText() const;
    QString toText() const;
    void setFrom(const QString &text);
    void setTo(const QString &text);
    bool isEmpty() const;

private:
    CharSelector *_from = nullptr;
    CharSelector *_to = nullptr;
};

/**
   @internal
*/
class SingleFactory : public KMultiFormListBoxFactory
{
public:
    KMultiFormListBoxEntry *create(QWidget *parent) override
    {
        return new SingleEntry(parent);
    }

    QWidget *separator(QWidget *) override
    {
        return nullptr;
    }
};

/**
   @internal
*/
class RangeFactory : public KMultiFormListBoxFactory
{
public:
    KMultiFormListBoxEntry *create(QWidget *parent) override
    {
        return new RangeEntry(parent);
    }

    QWidget *separator(QWidget *) override
    {
        return nullptr;
    }
};

/**
   @internal
*/
class CharacterEdits : public QDialog
{
    Q_OBJECT
public:
    explicit CharacterEdits(QWidget *parent = nullptr);

public Q_SLOTS:
    void setRegexp(TextRangeRegExp *regexp);

protected Q_SLOTS:
    void slotOK();

private:
    QCheckBox *negate = nullptr;
    QCheckBox *wordChar = nullptr;
    QCheckBox *_nonWordChar = nullptr;
    QCheckBox *digit = nullptr;
    QCheckBox *_nonDigit = nullptr;
    QCheckBox *space = nullptr;
    QCheckBox *_nonSpace = nullptr;
    KMultiFormListBox *_single = nullptr;
    KMultiFormListBox *_range = nullptr;

    void addCharacter(const QString &txt);
    void addRange(const QString &from, const QString &to);
    TextRangeRegExp *_regexp = nullptr;
};
