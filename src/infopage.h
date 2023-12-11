/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef infopage_h
#define infopage_h

#include <QTextBrowser>

class InfoPage : public QTextBrowser
{
public:
    explicit InfoPage(QWidget *parent);

protected:
    void doSetSource(const QUrl &name, QTextDocument::ResourceType type = QTextDocument::UnknownResource) override;
};

#endif
