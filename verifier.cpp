#include "verifier.h"
#include <qregexp.h>
#include <qtimer.h>
#include <qtextstream.h>
#include <qfile.h>
#include <klocale.h>
#include <qprogressdialog.h>
#include "syntaxhighlighter.h"

Verifier::Verifier( QWidget* parent, const char* name ) : QTextEdit( parent, name )
    /* QT_ANCHOR_DO_NOT_WORK: ,_current( 0 ) */
{
    _highlighter = new SyntaxHighlighter( this );
    setMinimumSize(1,1);
}

/**
   Update text edit to show matches of regular expression
*/
void Verifier::verify( const QString& reg )
{
    _highlighter->setRegExp( reg );
    _highlighter->rehighlight();
}


/**
   Make the text edit display the text without any matches.
*/
void Verifier::clearRegexp()
{
    _highlighter->setRegExp( QString::null );
    _highlighter->rehighlight();
}

/**
   Set case sensitive matching
*/
void Verifier::setCaseSensitive( bool b )
{
    _highlighter->setCaseSensitive( b );
}


/**
   set minimal mathcing
*/
void Verifier::setMinimal( bool b )
{
    _highlighter->setMinimal( b );
}

// Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
// do not work. Enable this when they work.
// void Verifier::gotoFirst()
// {
//     gotoNum(1);
// }
//
// void Verifier::gotoLast()
// {
//     gotoNum( _count-1 );
// }
//
// void Verifier::gotoPrev()
// {
//     gotoNum( _current-1 );
// }
//
// void Verifier::gotoNext()
// {
//     gotoNum( _current+1 );
// }
//
// void Verifier::gotoNum( int which )
// {
//     QString anchor = QString::fromLatin1("match%1").arg(which);
//     scrollToAnchor( anchor );
//     _current = which;
//     emit currentChanged( _current );
//     emit goBackwardPossible( which != 0 );
//     emit goForwardPossible( which != _count -1 );
// }


#include "verifier.moc"

