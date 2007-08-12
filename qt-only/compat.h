/*
 *  Copyright (c) 2002-2004 Jesper K. Pedersen <blackie@kde.org>
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

#ifndef COMPAT_H
#define COMPAT_H
#include <QObject>
#include <qmessagebox.h>
#include <QLayout>
#include <QPushButton>
#include <qinputdialog.h>
#include <qframe.h>

QString i18n( const QString& a);
QString i18nc( const QString& a, const QString& b);
#define isatty(x) 0

#define KTextBrowser QTextBrowser
#define KListBox QListBox
#define KFileDialog QFileDialog
#define KPushButton QPushButton

class KDialogBase :public QDialog
{
    Q_OBJECT

public:
    enum ButtonCode {Ok = 1, Cancel, Help};
    enum DialogType { Plain };

    KDialogBase ( int dialogFace, const QString &caption, int buttonMask,
                  ButtonCode defaultButton,
                  QWidget *parent=0, const char *name=0, bool modal=true );

    KDialogBase( QWidget* parent, const char* name = 0, bool modal = true,
                 const QString& caption = QString(),
                 int buttonMask = 0 );

    void init( int buttonMask, ButtonCode /*defaultButton*/, const QString& caption );
    void setMainWidget( QWidget* top );
    QFrame* plainPage();
    void setHelp( const QString&, const QString& ) {}

public slots:
    void slotOk();
    void slotCancel();

signals:
    void okClicked();
    void cancelClicked();
    void finished();
    void helpClicked();

private:
    QVBoxLayout* _layout;
};

class KMessageBox :public QMessageBox
{
    Q_OBJECT
public:
    enum ButtonCode { Ok = 1, Cancel = 2, Yes = 3, No = 4, Continue = 5 };
    static int  warningYesNo (QWidget *parent, const QString &text,
                              const QString &caption = QString() );
    static int information( QWidget* parent, const QString& text, const QString& caption = QString(),
                            const QString& /*dontShowAgainName*/ = QString() );
    static int sorry( QWidget* parent, const QString& text, const QString& caption = QString() );
};

#endif /* COMPAT_H */

