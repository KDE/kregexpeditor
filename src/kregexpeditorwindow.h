// SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
// SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: LGPL-2.0-only

#ifndef kregexpeditorguidialog_h
#define kregexpeditorguidialog_h

#include "kregexpeditorprivate.h"
#include <KXmlGuiWindow>

#include "kregexpeditorcommon_export.h"

class KREGEXPEDITORCOMMON_EXPORT KRegExpEditorWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit KRegExpEditorWindow(QWidget *parent);
    ~KRegExpEditorWindow() override;

    QString regExp() const;

    static const QString version;

Q_SIGNALS:
    void changes(bool);

private:
    void setupActions();
    void showHelp();

    KRegExpEditorPrivate *_editor = nullptr;
    QList<QPair<RegExpConverter *, QAction *>> m_converters;
};

#endif
