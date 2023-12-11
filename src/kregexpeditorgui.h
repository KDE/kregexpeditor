/*
 *  SPDX-FileCopyrightText: 2002-2003 Jesper K. Pedersen <blackie@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-only
 **/

#ifndef kregexpeditorgui_h
#define kregexpeditorgui_h

#include <QDialog>
#include <QVariant>
#include <qwidget.h>

class KRegExpEditorPrivate;

/**
   Regular Expression Editor.

   @author Jesper Kjær Pedersen <blackie@kde.org>
   @version 0.1
 **/
class Q_DECL_EXPORT KRegExpEditorGUI : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString regexp READ regExp WRITE setRegExp)
public:
    explicit KRegExpEditorGUI(QWidget *parent = nullptr, const QVariantList & = QVariantList());
    ~KRegExpEditorGUI() override;
    QString regExp() const;

    static const QString version;

protected:
    void closeEvent(QCloseEvent *event) override;

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
    KRegExpEditorPrivate *_editor = nullptr;
};

#endif
