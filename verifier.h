#ifndef __VERIFIER_H
#define __VERIFIER_H
#include <qtextedit.h>
#include <qregexp.h>
class QTimer;
class QProgressDialog;
class QLabel;
class Verifier :public QTextEdit 
{
Q_OBJECT
public:
    Verifier( QWidget* parent, const char* name = 0 );
    
public slots:
    void verify( const QString& regexp, const QString& txt = QString::null );
    void clearRegexp();
    void setCaseSensitive( bool );
    void setMinimal( bool );

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void gotoFirst();
    // void gotoPrev();
    // void gotoNext();
    // void gotoLast();
   
signals:
    void countChanged( int );

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void currentChanged( int );
    // void goBackwardPossible( bool );
    // void goForwardPossible( bool );
    
protected:
    QString escape( QString );
    void gotoNum( int );

private:
    bool _caseSensitive;
    bool _minimal;
    int _count;
    // int _current;

};

#endif // __VERIFIER_H
