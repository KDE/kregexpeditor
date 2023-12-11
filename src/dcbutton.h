/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef dcbutton
#define dcbutton

#include <QToolButton>

/**
   QToolButton extended to emit a signal on double click.
   @internal
*/
class DoubleClickButton : public QToolButton
{
    Q_OBJECT

public:
    explicit DoubleClickButton(const QPixmap &pix, QWidget *parent, const QString &name = QString());

protected:
    void mouseDoubleClickEvent(QMouseEvent *) override;

Q_SIGNALS:
    void doubleClicked();
};

#endif // dcbutton
