/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include <KTextEdit>

class RegexpHighlighter;

class Verifier : public KTextEdit
{
    Q_OBJECT
public:
    explicit Verifier(QWidget *parent);
    void setHighlighter(RegexpHighlighter *);

public Q_SLOTS:
    void verify(const QString &regexp);
    void clearRegexp();
    void setCaseSensitive(bool);
    void setMinimal(bool);

    // I have no way of telling the current paragrahp when highlighting - thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void gotoFirst();
    // void gotoPrev();
    // void gotoNext();
    // void gotoLast();
    // Q_SIGNALS:
    //    void countChanged( int );
    // void currentChanged( int );
    // void goBackwardPossible( bool );
    // void goForwardPossible( bool );

    // protected:
    //   void gotoNum( int );

private:
    int _count;
    // int _current;
    RegexpHighlighter *_highlighter = nullptr;
};
