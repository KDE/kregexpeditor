/*
 *  Copyright (c) 2002-2003 Jesper K. Pedersen <blackie@kde.org>
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

#ifdef QT_ONLY
  #include "compat.h"
#else
  #include <kdialogbase.h>
  #include "characterswidget.moc"
#endif

#include "characterswidget.h"
#include "textrangeregexp.h"
#include "regexp.h"
#include "charselector.h"
#include "myfontmetrics.h"
#include "regexpconverter.h"
#include <QApplication>
#include <qpainter.h>
#include <q3grid.h>
//Added by qt3to4:
#include <QPaintEvent>
#include <Q3PtrList>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <iostream>
#include <qcursor.h>
#include <QCheckBox>
#include <Q3GroupBox>

CharacterEdits* CharactersWidget::_configWindow = 0;

CharactersWidget::CharactersWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                                   const char *name)
    : RegExpWidget(editorWindow, parent, name)
{
    _regexp = new TextRangeRegExp( false /* not used */);
}

CharactersWidget::CharactersWidget( TextRangeRegExp* regexp, RegExpEditorWindow* editorWindow,
                                    QWidget* parent, const char* name )
    : RegExpWidget( editorWindow, parent, name )
{
    _regexp = dynamic_cast<TextRangeRegExp*>( regexp->clone() );
    Q_ASSERT( _regexp );
}

CharactersWidget::~CharactersWidget()
{
    delete _regexp;
}


QSize CharactersWidget::sizeHint() const
{
    QFontMetrics metrics = fontMetrics();
    _textSize = HackCalculateFontSize(metrics, title());
    //  _textSize = metrics.size(0, title());

    QSize headerSize = QSize(_textSize.width() + 4 * bdSize,
                             _textSize.height());

    _contentSize = HackCalculateFontSize(metrics,text());
    //  _contentSize = metrics.size(0, text());

    return QSize(qMax(headerSize.width(), bdSize + _contentSize.width() + bdSize+ 2*pw),
                 headerSize.height() + bdSize + _contentSize.height() + bdSize + 2*pw);
}

void CharactersWidget::paintEvent(QPaintEvent *e)
{
    QSize mySize = sizeHint();

    QPainter painter(this);
    drawPossibleSelection( painter, mySize );

    int center = _textSize.height()/2;
    int offset = 0;

    // draw the horizontal line and the label
    painter.drawLine(pw,center, bdSize,center);
    offset += pw + 2*bdSize;

    painter.drawText(offset, 0, _textSize.width(), _textSize.height(), 0, title());
    offset += _textSize.width() + bdSize;

    painter.drawLine(offset, center, mySize.width(), center);

    // Draw the rest of the lines
    int y = mySize.width() - pw;
    int x = mySize.height() -pw;
    painter.drawLine(0,center,0,mySize.height());
    painter.drawLine(y,center,y,mySize.height());
    painter.drawLine(0,x,y,x);

    // Draw the text
    painter.drawText(bdSize, bdSize+_textSize.height(), _contentSize.width(),
                     _contentSize.height(), 0, text());

    RegExpWidget::paintEvent(e);
}

RegExp* CharactersWidget::regExp() const
{
	RegExp* r = _regexp->clone();
    r->setSelected( isSelected() );
    return r;
}

QString CharactersWidget::text() const
{
    QString res = QString::fromLatin1("");

    if (_regexp->wordChar())
        res += i18n("- A word character\n");

    if (_regexp->nonWordChar())
        res += i18n("- A non-word character\n");

    if (_regexp->digit())
        res += i18n("- A digit character\n");

    if (_regexp->nonDigit())
        res += i18n("- A non-digit character\n");

    if ( _regexp->space() )
        res += i18n("- A space character\n");

    if ( _regexp->nonSpace() )
        res += i18n("- A non-space character\n");

    // Single characters
    QStringList chars = _regexp->chars();
    if ( !chars.isEmpty() ) {
        QString str = chars.join( QString::fromLocal8Bit(", ") );
        res += QString::fromLocal8Bit("- ") + str + QString::fromLocal8Bit("\n");
    }

    // Ranges characters
    Q3PtrList<StringPair> range = _regexp->range();
    for ( Q3PtrListIterator<StringPair> it( range ); *it; ++it ) {
        StringPair* elm = static_cast<StringPair*>(*it);
        if (elm) {
            QString fromText = elm->first();
            QString toText = elm->second();

            res += QString::fromLocal8Bit("- ") + i18n("from ") + fromText + i18n(" to ") + toText + QString::fromLocal8Bit("\n");
        }
    }
    return res.left(res.length()-1);
}


QString CharactersWidget::title() const
{
    if (_regexp->negate())
        return i18n("Any Character Except");
    else
        return i18n("One of Following Characters");
}


RegExpWidget* CharactersWidget::findWidgetToEdit( QPoint globalPos )
{
    if ( QRect(mapToGlobal(QPoint(0,0)), size()).contains( globalPos ) )
        return this;
    else
        return 0;
}

int CharactersWidget::edit()
{
    if ( _configWindow == 0 ) {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        // No parent here, as this window should continue to exists.
        _configWindow = new CharacterEdits( 0 );
        _configWindow->setObjectName( "CharactersWidget::_configWindow" );
        QApplication::restoreOverrideCursor();
    }

    _configWindow->move(QCursor::pos() - QPoint(_configWindow->sizeHint().width()/2,
                                                _configWindow->sizeHint().height()/2));
    int ret = _configWindow->exec(_regexp );
    if ( ret == QDialog::Accepted ) {
        _editorWindow->updateContent( 0 );
        update();
    }
    return ret;
}

void CharacterEdits::addCharacter( QString txt )
{
    KMultiFormListBoxEntryList list = _single->elements();
    for ( Q3PtrListIterator<KMultiFormListBoxEntry> it(list); *it; ++it ) {
        SingleEntry* entry = dynamic_cast<SingleEntry*>( *it );
        if ( entry && entry->isEmpty() ) {
            entry->setText( txt );
            return;
        }
    }

    SingleEntry* entry = new SingleEntry( _single );
    entry->setText( txt );
    _single->append( entry );
}

void CharacterEdits::addRange( QString from, QString to )
{
    KMultiFormListBoxEntryList list = _range->elements();
    for ( Q3PtrListIterator<KMultiFormListBoxEntry> it(list); *it; ++it ) {
        RangeEntry* entry = dynamic_cast<RangeEntry*>( *it );
        if ( entry && entry->isEmpty() ) {
            entry->setFrom( from );
            entry->setTo( to );
            return;
        }
    }

    RangeEntry* entry = new RangeEntry( _range );
    entry->setFrom( from );
    entry->setTo( to );
    _range->append( entry );
}

int CharacterEdits::exec( TextRangeRegExp* regexp )
{
    _regexp = regexp;
    negate->setChecked( regexp->negate() );
    digit->setChecked( regexp->digit() );
    _nonDigit->setChecked( regexp->nonDigit() );
    space->setChecked( regexp->space() );
    _nonSpace->setChecked( regexp->nonSpace() );
    wordChar->setChecked( regexp->wordChar() );
    _nonWordChar->setChecked( regexp->nonWordChar() );

    bool enabled = (RegExpConverter::current()->features() & RegExpConverter::CharacterRangeNonItems);
    _nonWordChar->setEnabled( enabled );
    _nonDigit->setEnabled( enabled );
    _nonSpace->setEnabled( enabled );

    // Characters

    KMultiFormListBoxEntryList list1 = _single->elements();
    for ( Q3PtrListIterator<KMultiFormListBoxEntry> it(list1); *it; ++it ) {
        SingleEntry* entry = dynamic_cast<SingleEntry*>( *it );
        if (entry)
            entry->setText( QString::fromLocal8Bit("") );
    }
    QStringList list2 = regexp->chars();
    for ( QStringList::Iterator it2( list2.begin() ); ! (*it2).isNull(); ++it2 ) {
        addCharacter( *it2 );
    }

    // Ranges
    KMultiFormListBoxEntryList list3 = _range->elements();
    for ( Q3PtrListIterator<KMultiFormListBoxEntry> it3(list3); *it3; ++it3 ) {
        RangeEntry* entry = dynamic_cast<RangeEntry*>( *it3 );
        if (entry) {
            entry->setFrom( QString::fromLocal8Bit("") );
            entry->setTo( QString::fromLocal8Bit("") );
        }
    }

    Q3PtrList<StringPair> ranges = regexp->range();
    for ( Q3PtrListIterator<StringPair> it4(ranges); *it4; ++it4 ) {
        QString from = (*it4)->first();
        QString to = (*it4)->second();
        addRange(from,to);
    }

    int res = KDialogBase::exec();
    _regexp = 0;
    return res;
}


CharacterEdits::CharacterEdits( QWidget *parent)
  : KDialogBase( parent, 0, true,
                 i18n("Specify Characters"),
                 KDialogBase::Ok | KDialogBase::Cancel)
{
    setObjectName( "CharacterEdits" );
    QWidget* top = new QWidget( this );
    QVBoxLayout *topLayout = new QVBoxLayout(top);
    topLayout->setSpacing(6);
    setMainWidget( top );

    negate = new QCheckBox(i18n("Do not match the characters specified here"),
                           top);
    topLayout->addWidget( negate );


    // The predefined box
    Q3GroupBox* predefined = new Q3GroupBox(1, Qt::Vertical,i18n("Predefined Character Ranges"),top);
    topLayout->addWidget(predefined);
    Q3Grid* grid = new Q3Grid(3, predefined );

    wordChar = new QCheckBox(i18n("A word character"),grid);
    digit = new QCheckBox(i18n("A digit character"),grid);
    space = new QCheckBox(i18n("A space character"), grid);

    _nonWordChar = new QCheckBox(i18n("A non-word character"),grid);
    _nonDigit = new QCheckBox(i18n("A non-digit character"),grid);
    _nonSpace = new QCheckBox(i18n("A non-space character"), grid);

    // Single characters
    Q3GroupBox * singleBox = new Q3GroupBox(1, Qt::Horizontal,i18n("Single Characters"), top );
    topLayout->addWidget( singleBox );
    _single = new KMultiFormListBox(new SingleFactory(), KMultiFormListBox::MultiVisible,
                                    singleBox);
    _single->addElement(); _single->addElement(); _single->addElement();

    QWidget* moreW = new QWidget( singleBox );
    QHBoxLayout* moreLay = new QHBoxLayout( moreW );
    QPushButton* more = new QPushButton( i18n("More Entries"), moreW );
    moreLay->addWidget( more );
    moreLay->addStretch( 1 );

    connect(more,SIGNAL(clicked()), _single, SLOT(addElement()));

    // Ranges
    Q3GroupBox * rangeBox = new Q3GroupBox(1, Qt::Horizontal,i18n("Character Ranges"), top );
    topLayout->addWidget( rangeBox );

    _range = new KMultiFormListBox(new RangeFactory(), KMultiFormListBox::MultiVisible,
                                   rangeBox);
    _range->addElement(); _range->addElement(); _range->addElement();

    moreW = new QWidget( rangeBox );
    moreLay = new QHBoxLayout( moreW );
    more = new QPushButton( i18n("More Entries"), moreW );
    moreLay->addWidget( more );
    moreLay->addStretch( 1 );
    connect(more,SIGNAL(clicked()), _range, SLOT(addElement()));
    // Buttons
    connect(this, SIGNAL(okClicked()), this, SLOT(slotOK()));
}

void CharacterEdits::slotOK()
{
    _regexp->setNegate( negate->isChecked() );

    _regexp->setWordChar( wordChar->isChecked() );
    _regexp->setNonWordChar( _nonWordChar->isChecked() );

    _regexp->setDigit( digit->isChecked() );
    _regexp->setNonDigit( _nonDigit->isChecked() );

    _regexp->setSpace( space->isChecked() );
    _regexp->setNonSpace( _nonSpace->isChecked() );

	// single characters
    _regexp->clearChars();
    KMultiFormListBoxEntryList list = _single->elements();
    for ( Q3PtrListIterator<KMultiFormListBoxEntry> it( list ); *it; ++it ) {
        SingleEntry* entry = dynamic_cast<SingleEntry*>(*it);
        if ( entry && !entry->isEmpty() ) {
            _regexp->addCharacter( entry->text() );
        }
    }

    // Ranges
    _regexp->clearRange();
    list = _range->elements();
    for ( Q3PtrListIterator<KMultiFormListBoxEntry> it2( list ); *it2; ++it2 ) {
        RangeEntry* entry = dynamic_cast<RangeEntry*>(*it2);
        if ( entry && !entry->isEmpty() ) {
            _regexp->addRange( entry->fromText(), entry->toText() );
        }
    }
}


SingleEntry::SingleEntry(QWidget* parent)
    :KMultiFormListBoxEntry( parent )
{
    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setSpacing( 6 );
    layout->setMargin( 3 );
    _selector = new CharSelector( this );
    layout->addWidget( _selector );
    layout->addStretch(1);
}

QString SingleEntry::text() const
{
    return _selector->text();
}

void SingleEntry::setText( QString text )
{
    _selector->setText( text );
}

bool SingleEntry::isEmpty() const
{
    return _selector->isEmpty();
}


RangeEntry::RangeEntry(QWidget* parent)
    :KMultiFormListBoxEntry( parent )
{
    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setSpacing( 6 );
    layout->setMargin( 3 );

    QLabel* label = new QLabel(i18n("From:"), this );
    _from = new CharSelector( this );
    layout->addWidget( label );
    layout->addWidget( _from );

    layout->addStretch(1);

    label = new QLabel(i18nc("end of range","To:"), this );
    _to = new CharSelector( this );
    layout->addWidget( label );
    layout->addWidget( _to );
}

QString RangeEntry::fromText() const
{
    return _from->text();
}

QString RangeEntry::toText() const
{
    return _to->text();
}

void RangeEntry::setFrom( QString text )
{
    _from->setText( text );
}

void RangeEntry::setTo( QString text )
{
    _to->setText( text );
}

bool RangeEntry::isEmpty() const
{
    return _from->isEmpty() || _to->isEmpty();
}

