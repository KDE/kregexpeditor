#ifndef __VERIFYBUTTONS_H
#define __VERIFYBUTTONS_H

#include <qdockwindow.h>
class QPushButton;
class QLabel;

class VerifyButtons :public QDockWindow
{
    Q_OBJECT

public:
    VerifyButtons( QWidget* parent, const char* name );

signals:
    void verify();
    void autoVerify( bool );
    void loadVerifyText( const QString& );

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void gotoFirst();
    // void gotoPrev();
    // void gotoNext();
    // void gotoLast();

public slots:
    //     void enableForwardButtons( bool );
    //     void enableBackwardButtons( bool );
    void setMatchCount( int );

protected slots:  
    void updateVerifyButton( bool );
    void loadText();

private:
    QPushButton* _autoVerify;
    QPushButton* _verify;
    QLabel* _matches;

    // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // QPushButton* _first;
    // QPushButton* _prev;
    // QPushButton* _next;
    // QPushButton* _last;
};


#endif // __VERIFYBUTTONS_H
