#include <qlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <klocale.h>
#include "kregexpeditorprivate.h"
#include "scrollededitorwindow.h"
#include "widgetwindow.h"
#include "regexp.h"
#include <unistd.h>
#include <stdio.h>
#include <kiconloader.h>
#include "infopage.h"
#include <kapp.h>
#include <qsplitter.h>

extern bool parse( QString str, const CompoundInfo& ci );
extern RegExp* parseData();


KRegExpEditorPrivate::KRegExpEditorPrivate(QWidget *parent, const char *name) 
  : QWidget(parent, name), _updating( false )
{
  setMinimumSize(600,300);
  QVBoxLayout *topLayout = new QVBoxLayout( this, 0, 6 );
  QSplitter* splitter = new QSplitter( Horizontal, this );
  topLayout->addWidget( splitter );
  
  RegExpWidgetWindow *widgetWin = 
    new RegExpWidgetWindow( splitter, "KRegExpEditorPrivate::widgetwin" );

  _scrolledEditorWindow = 
    new RegExpScrolledEditorWindow( splitter, "KRegExpEditorPrivate::_scrolledEditorWindow" );

  _info = new InfoPage( splitter, "_info" );
  _scrolledEditorWindow->hide();

  splitter->setResizeMode( widgetWin, QSplitter::KeepSize );
  splitter->setResizeMode( _info, QSplitter::Stretch );
  splitter->setResizeMode( _scrolledEditorWindow, QSplitter::Stretch );
  QValueList<int> list;
  list << widgetWin->minimumSizeHint().width();
  
  splitter->setSizes( list );

  // Connect the buttons
  connect( widgetWin, SIGNAL( clicked( int ) ),
           _scrolledEditorWindow, SLOT( slotInsertRegExp( int ) ) );
  connect( widgetWin, SIGNAL( load( RegExp* ) ),
           _scrolledEditorWindow, SLOT( slotInsertRegExp( RegExp*  ) ) );
  connect( widgetWin, SIGNAL( doSelect() ), _scrolledEditorWindow, SLOT( slotDoSelect() ) );
  connect( _scrolledEditorWindow, SIGNAL( doneEditing() ),
           widgetWin, SLOT( slotSelectNewAction() ) );


  connect( widgetWin, SIGNAL( clicked( int ) ), this, SLOT( slotShowEditor() ) );
  connect( widgetWin, SIGNAL( load( RegExp* ) ), this, SLOT( slotShowEditor() ) );
  connect( widgetWin, SIGNAL( doSelect() ), this, SLOT( slotShowEditor() ) );
  connect( _scrolledEditorWindow, SIGNAL( savedRegexp() ), widgetWin, SLOT( slotPopulateUserRegexps() ) );
  

  // Line Edit
  QHBoxLayout* layout = new QHBoxLayout( topLayout, 6 );
  QLabel* label = new QLabel( i18n("Ascii Syntax:"), this );
  layout->addWidget( label );
  _regexpEdit = new QLineEdit( this );
  layout->addWidget( _regexpEdit );

  _error = new QLabel( this );
  _error->setPixmap(BarIcon( QString::fromLocal8Bit("solo.xpm" ) ) );
  layout->addWidget( _error );
  _error->hide();
  
  connect( _regexpEdit, SIGNAL(textChanged( const QString& ) ), this, SLOT( slotUpdateEditor( const QString & ) ) );
  connect( _scrolledEditorWindow, SIGNAL( change() ), this, SLOT( slotUpdateLineEdit() ) );

  // Push an initial empty element on the stack.
  _undoStack.push( _scrolledEditorWindow->regExp() );
  _redoStack.setAutoDelete( true );
}

QString KRegExpEditorPrivate::regexp()
{
  RegExp* regexp = _scrolledEditorWindow->regExp();
  QString res = regexp->toString();
  delete regexp;
  return res;
}

void KRegExpEditorPrivate::slotUpdateEditor( const QString & txt) 
{
  if ( _updating )
    return;
  
  _updating = true;
  bool ok = parse( txt, _ci );
  RegExp* result = parseData();
  if ( ok ) {
    _scrolledEditorWindow->slotSetRegExp( result );
    _error->hide();
    recordUndoInfo();
  }
  else {
    _error->show();
  }
  delete result;
  _updating = false;
}

void KRegExpEditorPrivate::slotUpdateLineEdit()
{
  if ( _updating )
    return;
  _updating = true;
  
  RegExp* regexp = _scrolledEditorWindow->regExp();

  _ci.clear();
  regexp->updateCI( &_ci );
  
  QString str = regexp->toString();
  _regexpEdit->setText( str );
  delete regexp;

  recordUndoInfo();
  
  _updating = false;
}

void KRegExpEditorPrivate::recordUndoInfo()
{
  ASSERT( _updating );
  
  // Update undo/redo stacks
  RegExp* regexp = _scrolledEditorWindow->regExp();
  if ( regexp->toXmlString() != _undoStack.top()->toXmlString() ) {
    _undoStack.push( regexp );
    _redoStack = QStack<RegExp>();
  }
}

bool KRegExpEditorPrivate::canUndo() const
{
  return (_undoStack.count() > 1 );
}

bool KRegExpEditorPrivate::canRedo() const
{
  return (_redoStack.count() != 0);
}

void KRegExpEditorPrivate::slotRedo()
{
  if ( _redoStack.count() != 0 ) {
    _undoStack.push(_redoStack.pop());
    _scrolledEditorWindow->slotSetRegExp( _undoStack.top() );
    slotUpdateLineEdit();
  }
}

void KRegExpEditorPrivate::slotUndo()
{
  if ( _undoStack.count() > 1 ) {
    _redoStack.push(_undoStack.pop());
    _scrolledEditorWindow->slotSetRegExp( _undoStack.top() );
    slotUpdateLineEdit();
  }
}

void KRegExpEditorPrivate::slotShowEditor()
{
  if ( _info ) {
    delete _info;
    _info = 0;
    _scrolledEditorWindow->show();
  }
}
