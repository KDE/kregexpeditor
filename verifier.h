#ifndef __VERIFIER_H
#define __VERIFIER_H
#include <qtextedit.h>
#include <qregexp.h>
class QTimer;
class QProgressDialog;
class QLabel;
class SyntaxHighlighter;
class Verifier :public QTextEdit
{
Q_OBJECT
public:
    Verifier( QWidget* parent, const char* name = 0 );

public slots:
    void verify( const QString& regexp );
    void clearRegexp();
    void setCaseSensitive( bool );
    void setMinimal( bool );

    // I have no way of telling the current paragrahp when highlighting - thefore scrolling to next/prev match
    // do not work. Enable this when they work.
    // void gotoFirst();
    // void gotoPrev();
    // void gotoNext();
    // void gotoLast();
    //signals:
    //    void countChanged( int );
    // void currentChanged( int );
    // void goBackwardPossible( bool );
    // void goForwardPossible( bool );

    // protected:
    //   void gotoNum( int );

private:
    int _count;
    // int _current;
    SyntaxHighlighter* _highlighter;

};

#endif // __VERIFIER_H
