/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#pragma once

#include <QWidget>

#include "singlecontainerwidget.h"

class QLineEdit;
class KTextEdit;
class QDialog;
class QCheckBox;
class QPaintEvent;
class CompoundRegExp;

/**
   Widget containing configuration details for @ref CompoundWidget
   @internal
*/
class CompoundDetailWindow : public QWidget
{
public:
    explicit CompoundDetailWindow(QWidget *parent);
    QString title() const;
    QString description() const;
    bool allowReplace() const;
    void setTitle(const QString &);
    void setDescription(const QString &);
    void setAllowReplace(bool);

private:
    QLineEdit *_title = nullptr;
    KTextEdit *_description = nullptr;
    QCheckBox *_allowReplace = nullptr;
};

/**
   Comopund RegExp widget.

   This widget has two purposes:
   @li To make it possible for a user to collapse a huge regular expression
   to take up less screen space.
   @li To act as back references for later use.

   @internal
*/
class CompoundWidget : public SingleContainerWidget
{
    Q_OBJECT

public:
    CompoundWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    CompoundWidget(CompoundRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent);

    bool updateSelection(bool parentSelected) override;
    QSize sizeHint() const override;
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return COMPOUND;
    }

    int edit() override;

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void init();
    QPixmap getIcon(const QString &name);

protected Q_SLOTS:
    void slotConfigCanceled();
    void slotConfigWindowClosed();

private:
    bool _hidden;
    QPixmap _up, _down;
    mutable QSize _pixmapSize;
    mutable QPoint _pixmapPos;

    QDialog *_configWindow = nullptr;
    CompoundDetailWindow *_content = nullptr;

    mutable QSize _textSize;
    mutable QSize _childSize;
    QByteArray _backup;

    int _backRefId;
};
