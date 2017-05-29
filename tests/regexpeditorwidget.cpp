/*
   Copyright (C) 2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "regexpeditorwidget.h"
#include <QHBoxLayout>
#include <KServiceTypeTrader>
#include <KTextWidgets/kregexpeditorinterface.h>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QDialog>

RegExpEditorWidget::RegExpEditorWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    bool regexEditorInstalled = !KServiceTypeTrader::self()->query(QStringLiteral("KRegExpEditor/KRegExpEditor")).isEmpty();
    if (regexEditorInstalled) {
        QPushButton *button = new QPushButton(QStringLiteral("Show regexpeditor"), this);
        layout->addWidget(button);
        const auto showRegExpDialog = [this]() {
            QDialog *editorDialog = KServiceTypeTrader::createInstanceFromQuery<QDialog>(QStringLiteral("KRegExpEditor/KRegExpEditor"), QString(), this);
            if ( editorDialog ) {
                KRegExpEditorInterface* iface = qobject_cast<KRegExpEditorInterface*>(editorDialog);
                Q_ASSERT( iface ); // This should not fail!

                // now use the editor.
                iface->setRegExp(QStringLiteral("foo"));

                if(editorDialog->exec() == QDialog::Accepted) {
                    //TODO
                }
            } else {
                qDebug() << " Unable to create QDialog";
            }
        };

        connect(button, &QPushButton::clicked, this, showRegExpDialog);
    } else {
        QLabel *label = new QLabel(QStringLiteral("KRegexpEditor is not installed!"), this);
        layout->addWidget(label);
    }
    layout->addStretch(1);
}
