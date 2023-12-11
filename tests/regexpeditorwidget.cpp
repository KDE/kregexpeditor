/*
   SPDX-FileCopyrightText: 2017 Montel Laurent <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "regexpeditorwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QDialog>

#include "../src/kregexpeditorguidialog.h"

RegExpEditorWidget::RegExpEditorWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *button = new QPushButton(QStringLiteral("Show regexpeditor"), this);
    layout->addWidget(button);
    const auto showRegExpDialog = [this]() {
        auto editorDialog = new KRegExpEditorGUIDialog(this);
        editorDialog->setRegExp(QStringLiteral("foo"));

        if(editorDialog->exec() == QDialog::Accepted) {
            //TODO
        } else {
            qDebug() << " Unable to create QDialog";
        }
    };

    connect(button, &QPushButton::clicked, this, showRegExpDialog);
    layout->addStretch(1);
}
