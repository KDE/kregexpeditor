/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only
 **/

#pragma once

#include <QWidget>

class QScrollArea;
class RegExpEditorWindow;
class RegExp;

/**
   Regular Expression editor window.
*/
class RegExpScrolledEditorWindow : public QWidget
{
    Q_OBJECT
public:
    RegExpScrolledEditorWindow(QWidget *parent);
    RegExp *regExp();

private:
    RegExpScrolledEditorWindow()
    {
    } // Do not use!

protected:
    void resizeEvent(QResizeEvent *) override;

public Q_SLOTS:
    void slotSetRegExp(RegExp *);
    void slotInsertRegExp(int);
    void slotInsertRegExp(RegExp *);
    void slotDeleteSelection();
    void slotDoSelect();
    void slotCut();
    void slotCopy();
    void slotSave();
    void slotPaste();

protected Q_SLOTS:
    void slotUpdateContentSize(QPointF focusPoint);
    void slotScroll(QPointF focusPoint);

Q_SIGNALS:
    /**
       This signal is emitted when the user has completed an editing
       action. The application may chose to call @ref slotDoSelect as a
       consequence to reset to selection mode.
    */
    void doneEditing();

    /**
       This signal is emitted whenever a change has taken place in the editor widget
    */
    void change();

    /**
       This signal is emitted when the user saves a regular expression.
    */
    void savedRegexp();

    void anythingSelected(bool);
    void anythingOnClipboard(bool);
    void canSave(bool);

    /**
       This signal is emitted when the regular expression has changed, or
       when the selection has changed.
     */
    void verifyRegExp();

private:
    RegExpEditorWindow *_editorWindow = nullptr;
    QScrollArea *_scrollArea = nullptr;
};
