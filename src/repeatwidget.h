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

#ifndef __repeatwidget
#define __repeatwidget

#include "singlecontainerwidget.h"

class QGroupBox;
class QButtonGroup;
class QDialog;
class QSpinBox;
class RepeatRegExp;
class QGridLayout;

/**
   Widget containging the configuration for a @ref RepeatWidget
   @internal
*/
class RepeatRangeWindow : public QWidget
{
    Q_OBJECT

public:
    enum REPEATTYPE {
        ANY, ATLEAST, ATMOST, EXACTLY, MINMAX
    };                                                       // krazy:exclude=spelling

    RepeatRangeWindow(QWidget *parent);
    QString text();
    int min();
    int max();
    void set(REPEATTYPE tp, int min, int max);

protected Q_SLOTS:
    void slotItemChange(int which);
    void slotUpdateMinVal(int minVal);
    void slotUpdateMaxVal(int minVal);

private:
    void createLine(QGridLayout *layout, const QString &text, QSpinBox **spin, REPEATTYPE tp);

    QSpinBox *_leastTimes = nullptr;
    QSpinBox *_mostTimes = nullptr;
    QSpinBox *_exactlyTimes = nullptr;
    QSpinBox *_rangeFrom = nullptr;
    QSpinBox *_rangeTo = nullptr;
    QGroupBox *_groupWidget = nullptr;
    QButtonGroup *_group = nullptr;
};

/**
   RegExp widget for `repeated content'
   @internal
*/
class RepeatWidget : public SingleContainerWidget
{
    Q_OBJECT

public:
    RepeatWidget(RegExpEditorWindow *editorWindow, QWidget *parent);
    RepeatWidget(RepeatRegExp *regexp, RegExpEditorWindow *editorWindow, QWidget *parent);
    void init();
    QSize sizeHint() const override;
    RegExp *regExp() const override;
    RegExpType type() const override
    {
        return REPEAT;
    }

    int edit() override;

protected:
    void paintEvent(QPaintEvent *e) override;

protected Q_SLOTS:
    void slotConfigCanceled();
    void slotConfigWindowClosed();

private:
    QDialog *_configWindow = nullptr;
    RepeatRangeWindow *_content = nullptr;

    mutable QSize _textSize;
    mutable QSize _childSize;
    QByteArray _backup;
};

#endif // __repeatwidget
