/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef kregexpeditorguidialog_h
#define kregexpeditorguidialog_h

#include <QDialog>
#include "kregexpeditorgui.h"

class Q_DECL_EXPORT KRegExpEditorGUIDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString regexp READ regExp WRITE setRegExp)
public:
    explicit KRegExpEditorGUIDialog(QWidget *parent, const QVariantList & = QVariantList());
    ~KRegExpEditorGUIDialog() override;

    QString regExp() const;

Q_SIGNALS:
    /** This signal tells whether undo is available. */
    void canRedo(bool);
    void canUndo(bool);
    void changes(bool);

public Q_SLOTS:
    void redo();
    void undo();
    void setRegExp(const QString &regexp);
    void doSomething(const QString &method, void *arguments);
    void setMatchText(const QString &);

private:
    void showHelp();
    KRegExpEditorGUI *_editor = nullptr;
};

#endif
