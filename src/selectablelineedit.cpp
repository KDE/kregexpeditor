/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#include "selectablelineedit.h"

SelectableLineEdit::SelectableLineEdit(RegExpWidget *owner, QWidget *parent, const QString &name)
    : QLineEdit(parent)
    , _owner(owner)
{
    setObjectName(name);
    connect(this, &QLineEdit::textChanged, this,
            &SelectableLineEdit::slotKeyPressed);
    setAcceptDrops(false);
}

void SelectableLineEdit::setSelected(bool selected)
{
    if (selected) {
        QPalette pal = palette();
        pal.setBrush(QPalette::Base, Qt::gray);
        setPalette(pal);
    } else {
        setPalette(QPalette());
    }
    repaint();
}

QSize SelectableLineEdit::sizeHint() const
{
    int frameWidth = hasFrame() ? 12 : 6; // from QLineEdit source
    QFontMetrics metrics = fontMetrics();
    int actualSize = metrics.boundingRect(text()).width();
    int charWidth = metrics.maxWidth();
    int height = QLineEdit::sizeHint().height();

    int width;
    if (hasFocus()) {
        width = actualSize + 6 * charWidth + frameWidth;
    } else {
        width = qMax(actualSize, charWidth) + frameWidth;
    }

    return QSize(width, height);
}

void SelectableLineEdit::slotKeyPressed()
{
    int frameWidth = hasFrame() ? 8 : 4; // from QLineEdit source

    QFontMetrics metrics = fontMetrics();
    int actualSize = metrics.boundingRect(text()).width();

    if (actualSize > size().width() - frameWidth) {
        updateGeometry();
        Q_EMIT parentPleaseUpdate();
    }
}
