#include "verifier.h"
#include <qregexp.h>
#include <qtimer.h>
#include <qtextstream.h>
#include <qfile.h>
#include <klocale.h>
#include <qprogressdialog.h>

Verifier::Verifier( QWidget* parent, const char* name ) : QTextEdit( parent, name ), _caseSensitive( false ), _minimal( false )
    /* QT_ANCHOR_DO_NOT_WORK: ,_current( 0 ) */
{
    setMinimumSize(1,1);
    QTextEdit::setTextFormat( PlainText );
}

/**
   Update text edit to show matches of regular expression 
*/
void Verifier::verify( const QString& reg, const QString& str ) 
{
    QRegExp regexp( reg );
    regexp.setCaseSensitive( _caseSensitive );
    regexp.setMinimal( _minimal );
    
    int curPara, curIndex;
    getCursorPosition( &curPara, &curIndex );
    
    if ( !regexp.isValid() || regexp.isEmpty() ) {
        clearRegexp();
        return;
    }

    //-------------------------------------------------- Fetch the text from the text edit
    QString txt;
    if ( str.isNull() ) {
        QTextEdit::setTextFormat( PlainText );
        txt = text();
    }
    else {
        txt = str;
    }
    
    QStringList list;
    if ( true ) {
        // inserting \n and reading it again from the text edit gives a
        // unicode character 0x2028.
        list = QStringList::split( QRegExp( QString::fromLatin1("\n|") + QString( QChar( 0x2028 ) ) ), txt, true );
    }
    else {
        list = QStringList() << txt;
    }
    QTextEdit::setTextFormat( RichText );

    // ------------------------------ Process with the regular expression.
    QString res = QString::fromLatin1("<qt><pre>");
    QString cols[] = { QString::fromLatin1("red"), QString::fromLatin1("blue") };
    _count = 0;
    for( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
        int index = 0;
        int oldIndex = 0;
        int length, start;
        while ( (index = regexp.search( *it, index ) ) != -1 && index < (int) (*it).length()) {
            if ( regexp.pos(1) != -1 ) {
                start = regexp.pos(1);
                length = regexp.cap(1).length();
            }
            else {
                start = index;
                length = regexp.matchedLength();
            }
            
            res += escape( (*it).mid( oldIndex, index-oldIndex ) );
            if ( start != index ) {
                res += QString::fromLatin1("<font color=\"%1\">%2</font>")
                    .arg( cols[_count%2] )
                    .arg( escape( (*it).mid( index, start-index) ) );
            }
            res += QString::fromLatin1("<a name=\"match%1\"><u><font size=\"+2\" color=\"%1\">%3</font></u></a>")
                .arg( _count )
                .arg(cols[ _count%2 ])
                .arg(escape( (*it).mid( start, length ) ));
            if ( length + (start-index) != regexp.matchedLength() ) {
                res += QString::fromLatin1("<font color=\"%1\">%2</font>")
                    .arg( cols[_count%2] )
                    .arg( escape( (*it).mid( start+length, regexp.matchedLength()-length-(start-index) ) ) );
            }
            
            oldIndex = index + regexp.matchedLength();
            index +=  QMAX( 1, regexp.matchedLength() ); // ensure progress when matching for example ^ or \b
            _count++;
        }
        res += escape( (*it).mid( oldIndex ) ) + QString::fromLatin1( "\n" );
    }
    
    res += QString::fromLatin1("</qt></pre>");
    setText( res );
    setCursorPosition( curPara, curIndex );
    emit countChanged( _count );
}

/**
   Escape symbols which has special QML meaning.
*/
QString Verifier::escape( QString txt ) 
{
    txt = txt.replace( QRegExp(QString::fromLatin1("&")), QString::fromLatin1("&amp;") );
    txt = txt.replace( QRegExp(QString::fromLatin1("<")), QString::fromLatin1("&lt;") );
    txt = txt.replace( QRegExp(QString::fromLatin1(">")), QString::fromLatin1("&gt;") );
    return txt;
}
  
/**
   Make the text edit display the text without any matches.
*/
void Verifier::clearRegexp()
{
    QTextEdit::setTextFormat( PlainText );
    QString txt = text();
    QTextEdit::setTextFormat( RichText );
    setText( QString::fromLatin1("<qt><pre>") + escape(txt) + QString::fromLatin1("</pre></qt>") );
}

/**
   Set case sensitive matching
*/
void Verifier::setCaseSensitive( bool b ) 
{
    _caseSensitive = b;
}


/**
   set minimal mathcing
*/
void Verifier::setMinimal( bool b )
{
    _minimal = b;
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

