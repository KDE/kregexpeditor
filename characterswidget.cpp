#include "characterswidget.h"
#include "textrangeregexp.h"
#include "altnregexp.h"
#include <klocale.h>
#include <qpainter.h>
#include <iostream>
#include <qhbox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <kdialogbase.h>
#include <qvbox.h>
#include "charselector.h"

CharactersWidget::CharactersWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                                   const char *name)
  : RegExpWidget(editorWindow, parent, name)
{
  init();
}

CharactersWidget::CharactersWidget( TextRangeRegExp* regexp, RegExpEditorWindow* editorWindow,
                                    QWidget* parent, const char* name )
  : RegExpWidget( editorWindow, parent, name )
{
  init();
  _content->negate->setChecked( regexp->negate() );
  _content->digit->setChecked( regexp->digit() );
  _content->nonDigit->setChecked( regexp->nonDigit() );
  _content->space->setChecked( regexp->space() );
  _content->nonSpace->setChecked( regexp->nonSpace() );
  _content->wordChar->setChecked( regexp->wordChar() );
  _content->nonWordChar->setChecked( regexp->nonWordChar() );

  // Characters
  QStringList list = regexp->chars();
  for ( QStringList::Iterator it( list.begin() ); ! (*it).isNull(); ++it ) {
    _content->addCharacter( *it );
  }

  // Ranges
  QList<StringPair> ranges = regexp->range();
  for ( QListIterator<StringPair> it(ranges); *it; ++it ) {
    QString from = (*it)->first();
    QString to = (*it)->second();
    _content->addRange(from,to);
  }
}


void CharactersWidget::init()
{
  _configWindow = new KDialogBase( this, "_configWindow", true, 
                                   i18n("Specify Characters"),
                                   KDialogBase::Ok | KDialogBase::Cancel);
  _content = new CharacterEdits( _configWindow );
  _configWindow->setMainWidget( _content );
  
  connect(_configWindow, SIGNAL(cancelClicked()), this, SLOT(slotConfigCanceled()));
  connect(_configWindow, SIGNAL(finished()), this, SLOT(slotConfigWindowClosed()));
}

  

QSize CharactersWidget::sizeHint() const
{
  QFontMetrics metrics = fontMetrics();
  _textSize = metrics.size(0, _content->title());
  
  QSize headerSize = QSize(_textSize.width() + 4 * bdSize,
                           _textSize.height());
  
  _contentSize = metrics.size(0,_content->text());
  
  return QSize(QMAX(headerSize.width(), bdSize + _contentSize.width() + bdSize+ 2*pw),
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
  
  painter.drawText(offset, 0, _textSize.width(), _textSize.height(), 0, _content->title());
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
                   _contentSize.height(), 0, _content->text());
 
  RegExpWidget::paintEvent(e);
}

void CharactersWidget::slotConfigWindowClosed()
{
  _editorWindow->updateContent( 0 );
  update();
}

void CharactersWidget::slotConfigCanceled()
{
  QDataStream stream( _backup, IO_ReadOnly );
  KWidgetStreamer streamer;
  streamer.fromStream( stream, _content );
  repaint();
}

RegExp* CharactersWidget::regExp() const
{
	TextRangeRegExp *regexp = new TextRangeRegExp( );
  regexp->setNegate( _content->negate->isChecked() );

  regexp->setWordChar( _content->wordChar->isChecked() );
  regexp->setNonWordChar( _content->nonWordChar->isChecked() );

  regexp->setDigit( _content->digit->isChecked() );
  regexp->setNonDigit( _content->nonDigit->isChecked() );

  regexp->setSpace( _content->space->isChecked() );
  regexp->setNonSpace( _content->nonSpace->isChecked() );

	// single characters
  KMultiFormListBoxEntryList list = _content->_single->elements();
  for ( QListIterator<KMultiFormListBoxEntry> it( list ); *it; ++it ) {
    SingleEntry* entry = dynamic_cast<SingleEntry*>(*it);
    if ( !entry->isEmpty() ) {
      regexp->addCharacter( entry->regexpStr() );
    }
  }

  // Ranges
  list = _content->_range->elements();
  for ( QListIterator<KMultiFormListBoxEntry> it( list ); *it; ++it ) {
    RangeEntry* entry = dynamic_cast<RangeEntry*>(*it);
    if ( !entry->isEmpty() ) {
      regexp->addRange( entry->regexpFromStr(), entry->regexpToStr() );
    }
  }

	return regexp;	
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
  _configWindow->move(QCursor::pos() - QPoint(_configWindow->sizeHint().width()/2,
                                              _configWindow->sizeHint().height()/2));

  QDataStream stream( _backup, IO_WriteOnly );
  KWidgetStreamer streamer;
  streamer.toStream( _content, stream );

  return _configWindow->exec();
}

void CharacterEdits::addCharacter( QString txt )
{
  KMultiFormListBoxEntryList list = _single->elements();
  for ( QListIterator<KMultiFormListBoxEntry> it(list); *it; ++it ) {
    SingleEntry* entry = dynamic_cast<SingleEntry*>( *it );
    if ( entry->isEmpty() ) {
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
  for ( QListIterator<KMultiFormListBoxEntry> it(list); *it; ++it ) {
    RangeEntry* entry = dynamic_cast<RangeEntry*>( *it );
    if ( entry->isEmpty() ) {
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


CharacterEdits::CharacterEdits(QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QVBoxLayout *topLayout = new QVBoxLayout(this, 6);
  
  negate = new QCheckBox(i18n("Do not match the characters specified here"),
                         this);
  topLayout->addWidget( negate );
  

  // The predefined box
  QHGroupBox* predefined = new QHGroupBox(i18n("Predefined Character Ranges"),this);
  topLayout->addWidget(predefined);
  QGrid* grid = new QGrid(3, predefined );

  wordChar = new QCheckBox(i18n("a word character"),grid);
  digit = new QCheckBox(i18n("A digit character"),grid);
  space = new QCheckBox(i18n("A space character"), grid);

  nonWordChar = new QCheckBox(i18n("a non-word character"),grid);
  nonDigit = new QCheckBox(i18n("A non-digit character"),grid);
  nonSpace = new QCheckBox(i18n("A space character"), grid);

  // Single characters
  QVGroupBox* singleBox = new QVGroupBox(i18n("Single Characters"), this );
  topLayout->addWidget( singleBox );
  _single = new KMultiFormListBox(new SingleFactory(), KMultiFormListBox::MultiVisible, 
                                  singleBox);
  _single->addElement(); _single->addElement(); _single->addElement();
  _single->addElement(); _single->addElement();

  QPushButton* more = new QPushButton( i18n("More Entries"), singleBox );
  connect(more,SIGNAL(clicked()), _single, SLOT(addElement()));  
  
  // Ranges
  QVGroupBox* rangeBox = new QVGroupBox(i18n("Character ranges"), this );
  topLayout->addWidget( rangeBox );

  _range = new KMultiFormListBox(new RangeFactory(), KMultiFormListBox::MultiVisible, 
                                 rangeBox);
  _range->addElement(); _range->addElement(); _range->addElement();
  _range->addElement(); _range->addElement();

  more = new QPushButton( i18n("More Entries"), rangeBox );
  connect(more,SIGNAL(clicked()), _range, SLOT(addElement()));  
}

QString CharacterEdits::title() const 
{
  if (negate->isChecked()) 
    return i18n("Any character except");
  else
    return i18n("One of the following characters");
}

QString CharacterEdits::text() const
{
  QString res = QString::fromLatin1("");
  
  if (wordChar->isChecked())
    res += i18n("- A word character\n");
  
  if (nonWordChar->isChecked())
    res += i18n("- A non-word character\n");
  
  if (digit->isChecked())
    res += i18n("- A digit character\n");

  if (nonDigit->isChecked())
    res += i18n("- A non-digit character\n");

  if ( space->isChecked() )
    res += i18n("- A space character\n");
  
  if ( nonSpace->isChecked() )
    res += i18n("- A non-space character\n");
  
  // Single characters
  KMultiFormListBoxEntryList list  = _single->elements();
  QString str;
  for ( QListIterator<KMultiFormListBoxEntry> it( list ); *it; ++it ) {
    QString txt = dynamic_cast<SingleEntry*>(*it)->text();
    if ( !txt.isEmpty() ) {
      if ( !str.isEmpty() )
        str += QString::fromLocal8Bit(", ");
      str += txt;
    }
  }
  if ( !str.isEmpty() )
    res += QString::fromLocal8Bit("- ") + str + QString::fromLocal8Bit("\n");
  
  // Ranges characters
  list  = _range->elements();
  for ( QListIterator<KMultiFormListBoxEntry> it( list ); *it; ++it ) {
    RangeEntry* elm = dynamic_cast<RangeEntry*>(*it);
    QString fromText = elm->fromText();
    QString toText = elm->toText();
    
    if ( !fromText.isEmpty() && ! toText.isEmpty() ) {
      res += QString::fromLocal8Bit("- ") + i18n("from ") + fromText + i18n(" to ") + toText + QString::fromLocal8Bit("\n");
    }
  }

  return res.left(res.length()-1);
}

QDataStream& operator<<(QDataStream& stream, const CharacterEdits& edit )
{
  stream << (Q_UINT8) edit.negate->isChecked() 
         << (Q_UINT8) edit.wordChar->isChecked()
         << (Q_UINT8) edit.nonWordChar->isChecked()
         << (Q_UINT8) edit.digit->isChecked()
         << (Q_UINT8) edit.nonDigit->isChecked()
         << (Q_UINT8) edit.space->isChecked()
         << (Q_UINT8) edit.nonSpace->isChecked();
  
  KWidgetStreamer streamer;
  streamer.toStream( edit._single, stream );
  streamer.toStream( edit._range, stream );
  
  return stream;
}


QDataStream& operator>>(QDataStream& stream, CharacterEdits& edit )
{
  bool checked;
  stream >> (Q_UINT8&) checked;
  edit.negate->setChecked( checked );
  
  stream >> (Q_UINT8&) checked;
  edit.wordChar->setChecked( checked );
  
  stream >> (Q_UINT8&) checked;
  edit.nonWordChar->setChecked( checked );
  
  stream >> (Q_UINT8&) checked;
  edit.digit->setChecked( checked );

  stream >> (Q_UINT8&) checked;
  edit.nonDigit->setChecked( checked );

  stream >> (Q_UINT8&) checked;
  edit.space->setChecked( checked );

  stream >> (Q_UINT8&) checked;
  edit.nonSpace->setChecked( checked );

  KWidgetStreamer streamer;
  streamer.fromStream( stream, edit._single );
  streamer.fromStream( stream, edit._range );

  return stream;
}

SingleEntry::SingleEntry(QWidget* parent, const char* name = 0 )
  :KMultiFormListBoxEntry( parent, name )
{
  QHBoxLayout* layout = new QHBoxLayout( this, 3, 6 );
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

QString SingleEntry::regexpStr() const
{
  return _selector->regexpStr();
}

QString RangeEntry::regexpFromStr() const
{
  return _from->regexpStr();
}

QString RangeEntry::regexpToStr() const
{
  return _to->regexpStr();
}

  



RangeEntry::RangeEntry(QWidget* parent, const char* name = 0 )
  :KMultiFormListBoxEntry( parent, name )
{
  QHBoxLayout* layout = new QHBoxLayout( this, 3, 6 );

  QLabel* label = new QLabel(i18n("From:"), this );
  _from = new CharSelector( this );
  layout->addWidget( label );
  layout->addWidget( _from );
  
  layout->addStretch(1);

  label = new QLabel(i18n("To:"), this );
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

#include "characterswidget.moc"
