#include "compat.h"

QString i18n( const QString& a) {
    return QObject::tr(a);
}

QString i18n( const QString& a, const QString& b) {
    return QObject::tr(b,a);
}

KDialogBase::KDialogBase( int /*dialogFace*/, const QString &caption, int buttonMask,
                          ButtonCode defaultButton, QWidget *parent, const char *name,
                          bool modal )
    :QDialog( parent, name, modal )
{
    init( buttonMask, defaultButton, caption );
}

KDialogBase::KDialogBase( QWidget* parent, const char* name, bool modal,
                          const QString& caption, int buttonMask )
        : QDialog( parent, name, modal )
{
    init( buttonMask, Ok, caption );
}

void KDialogBase::init( int buttonMask, ButtonCode /*defaultButton*/, const QString& caption )
{
    setCaption( caption );
    _layout = new QVBoxLayout( this, 6 );
    QHBoxLayout* buts = new QHBoxLayout( _layout, 6 );
    QPushButton* but;
    if ( buttonMask & Help ) {
        but = new QPushButton( tr("Help"), this );
        buts->addWidget( but );
        connect( but, SIGNAL( clicked() ), this, SIGNAL( helpClicked() ) );
    }
    buts->addStretch(1);
    if ( buttonMask & Ok ) {
        but = new QPushButton( tr("OK"), this );
        buts->addWidget( but );
        connect( but, SIGNAL( clicked() ), this, SLOT( slotOk() ) );
    }
    if ( buttonMask & Cancel ) {
        but = new QPushButton( tr("Cancel"), this );
        buts->addWidget( but );
        connect( but, SIGNAL( clicked() ), this, SLOT( slotCancel() ) );
    }
}

void KDialogBase::setMainWidget( QWidget* top )
{
    top->reparent( this, 0, QPoint(0,0) );
    _layout->insertWidget( 0, top );
}

QFrame* KDialogBase::plainPage()
{
    QFrame* frame = new QFrame( this );
    setMainWidget( frame );
    return frame;
}

void KDialogBase::slotOk()
{
    accept();
    emit okClicked();
    emit finished();
}

void KDialogBase::slotCancel()
{
    reject();
    emit cancelClicked();
    emit finished();
}

int KMessageBox::warningYesNo(QWidget *parent, const QString &text, const QString &caption )
{
    int code = warning( parent, caption, text, tr("No"), tr("Yes") );
    if ( code == 0 )
        return Yes;
    else
        return No;
}

int KMessageBox::information( QWidget* parent, const QString& text, const QString& caption,
                              const QString& /*dontShowAgainName*/ )
{
    return QMessageBox::information( parent, caption, text );
}

int KMessageBox::sorry( QWidget* parent, const QString& text, const QString& caption )
{
    return QMessageBox::information( parent, caption, text );
}


