/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef VERIFYBUTTONS_H
#define VERIFYBUTTONS_H

#include <QToolBar>

class QToolButton;
class QLabel;
class QAction;
class QMenu;
class RegExpConverter;

class VerifyButtons : public QToolBar
{
    Q_OBJECT

public:
    explicit VerifyButtons(QWidget *parent, const QString &name = QString());
    RegExpConverter *setSyntax(const QString &);
    void setAllowNonQtSyntax(bool);

Q_SIGNALS:
    void verify();
    void autoVerify(bool);
    void loadVerifyText(const QString &);
    void matchGreedy(bool);

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void gotoFirst();
    // void gotoPrev();
    // void gotoNext();
    // void gotoLast();

    void changeSyntax(const QString &);

public Q_SLOTS:
    //     void enableForwardButtons( bool );
    //     void enableBackwardButtons( bool );
    void setMatchCount(int);

protected Q_SLOTS:
    void updateVerifyButton(bool);
    void loadText();
    void slotChangeSyntax(QAction *action);

private:
    QToolButton *_verify = nullptr;
    QLabel *_matches = nullptr;
    QMenu *_configMenu = nullptr;
    QMenu *_languages = nullptr;

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // QToolButton* _first;
    // QToolButton* _prev;
    // QToolButton* _next;
    // QToolButton* _last;

    QList<QPair<RegExpConverter *, QAction *>> _converters;
};

#endif // VERIFYBUTTONS_H
