#include <qlineedit.h>
#include <klocale.h>
#include "kregexpeditorprivate.h"
#include "scrollededitorwindow.h"
#include "regexpbuttons.h"
#include "regexp.h"
#include <unistd.h>
#include <stdio.h>
#include <kiconloader.h>
#include "infopage.h"
#include <qsplitter.h>
#include <qdockarea.h>
#include "userdefinedregexps.h"
#include "auxbuttons.h"
#include <qaccel.h>
#include <kstandarddirs.h>
#include <compoundregexp.h>

extern bool parse( QString str );
extern RegExp* parseData();


KRegExpEditorPrivate::KRegExpEditorPrivate(QWidget *parent, const char *name) 
  : QWidget(parent, name), _updating( false )
{
  setMinimumSize(600,300);
  QDockArea* area = new QDockArea( Horizontal, QDockArea::Normal, this );
  QDockArea* verArea = new QDockArea( Vertical, QDockArea::Normal, this );

  // The DockWindows.
  RegExpButtons *regExpButtons = new RegExpButtons( area, "KRegExpEditorPrivate::regExpButton" );
  AuxButtons *auxButtons = new AuxButtons( area, "KRegExpEditorPrivate::AuxButtons" );
  _userRegExps = new UserDefinedRegExps( verArea, "KRegExpEditorPrivate::userRegExps" );
  _userRegExps->setResizeEnabled( true );


  _scrolledEditorWindow = 
    new RegExpScrolledEditorWindow( this, "KRegExpEditorPrivate::_scrolledEditorWindow" );

  _info = new InfoPage( this, "_info" );
  _scrolledEditorWindow->hide();

  QVBoxLayout *topLayout = new QVBoxLayout( this, 0, 6, "KRegExpEditorPrivate::topLayout" );
  topLayout->addWidget( area );
  QHBoxLayout* rows = new QHBoxLayout; // I need to cal addLayout explicit to get stretching right.
  topLayout->addLayout( rows, 1 );
  
  rows->addWidget( verArea );
  rows->addWidget( _scrolledEditorWindow,1 );
  rows->addWidget( _info,1 );

  // Connect the buttons
  connect( regExpButtons, SIGNAL( clicked( int ) ),   _scrolledEditorWindow, SLOT( slotInsertRegExp( int ) ) );
  connect( regExpButtons, SIGNAL( doSelect() ), _scrolledEditorWindow, SLOT( slotDoSelect() ) );
  connect( _userRegExps, SIGNAL( load( RegExp* ) ),    _scrolledEditorWindow, SLOT( slotInsertRegExp( RegExp*  ) ) );

  connect( regExpButtons, SIGNAL( clicked( int ) ), _userRegExps,   SLOT( slotUnSelect() ) );
  connect( regExpButtons, SIGNAL( doSelect() ),     _userRegExps,   SLOT( slotUnSelect() ) );
  connect( _userRegExps, SIGNAL( load( RegExp* ) ),  regExpButtons, SLOT( slotUnSelect() ) );

  connect( _scrolledEditorWindow, SIGNAL( doneEditing() ), regExpButtons, SLOT( slotSelectNewAction() ) );
  connect( _scrolledEditorWindow, SIGNAL( doneEditing() ), _userRegExps, SLOT( slotSelectNewAction() ) );

  connect( regExpButtons, SIGNAL( clicked( int ) ), this, SLOT( slotShowEditor() ) );
  connect( _userRegExps, SIGNAL( load( RegExp* ) ), this, SLOT( slotShowEditor() ) );
  connect( regExpButtons, SIGNAL( doSelect() ), this, SLOT( slotShowEditor() ) );

  connect( _scrolledEditorWindow, SIGNAL( savedRegexp() ), _userRegExps, SLOT( slotPopulateUserRegexps() ) );

  connect( auxButtons, SIGNAL( undo() ), this, SLOT( slotUndo() ) );
  connect( auxButtons, SIGNAL( redo() ), this, SLOT( slotRedo() ) );
  connect( this, SIGNAL( canUndo( bool ) ), auxButtons, SLOT( slotCanUndo( bool ) ) );
  connect( this, SIGNAL( canRedo( bool ) ), auxButtons, SLOT( slotCanRedo( bool ) ) );

  // Line Edit
  QHBoxLayout* layout = new QHBoxLayout( topLayout, 6 );
  QLabel* label = new QLabel( i18n("Ascii Syntax:"), this );
  layout->addWidget( label );
  _regexpEdit = new QLineEdit( this );
  layout->addWidget( _regexpEdit );

  
  QPixmap pix = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/error.png") ), KIcon::Toolbar );
  _error = new QLabel( this );
  _error->setPixmap( pix );
  layout->addWidget( _error );
  _error->hide();
  
  connect( _regexpEdit, SIGNAL(textChanged( const QString& ) ), this, SLOT( slotUpdateEditor( const QString & ) ) );
  connect( _scrolledEditorWindow, SIGNAL( change() ), this, SLOT( slotUpdateLineEdit() ) );
  connect( _regexpEdit, SIGNAL(textChanged( const QString& ) ), this, SLOT( slotShowEditor() ) );

  // Push an initial empty element on the stack.
  _undoStack.push( _scrolledEditorWindow->regExp() );
  _redoStack.setAutoDelete( true );

  QAccel* accel = new QAccel( this );
  accel->connectItem( accel->insertItem( CTRL+Key_Z ), this, SLOT( slotUndo() ) );
  accel->connectItem( accel->insertItem( CTRL+Key_R ), this, SLOT( slotRedo() ) );

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
  bool ok = parse( txt );
  RegExp* result = parseData();
  if ( ok ) {
    QPtrList<CompoundRegExp> list = _userRegExps->regExps();
    for ( QPtrListIterator<CompoundRegExp> it( list ); *it; ++it ) {
      result->replacePart( *it );
    }
    
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

  QString str = regexp->toString();
  _regexpEdit->setText( str );
  delete regexp;

  recordUndoInfo();
  
  _updating = false;
}

void KRegExpEditorPrivate::recordUndoInfo()
{
  Q_ASSERT( _updating );
  
  // Update undo/redo stacks
  RegExp* regexp = _scrolledEditorWindow->regExp();
  if ( regexp->toXmlString() != _undoStack.top()->toXmlString() ) {
    _undoStack.push( regexp );
    _redoStack = QPtrStack<RegExp>();
    emitUndoRedoSignals();
  }
}

void KRegExpEditorPrivate::slotRedo()
{
  if ( _redoStack.count() != 0 ) {
    _undoStack.push(_redoStack.pop());
    _scrolledEditorWindow->slotSetRegExp( _undoStack.top() );
    slotUpdateLineEdit();
    emitUndoRedoSignals();
  }
}

void KRegExpEditorPrivate::slotUndo()
{
  if ( _undoStack.count() > 1 ) {
    _redoStack.push(_undoStack.pop());
    _scrolledEditorWindow->slotSetRegExp( _undoStack.top() );
    slotUpdateLineEdit();
    emitUndoRedoSignals();
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

void KRegExpEditorPrivate::emitUndoRedoSignals()
{
  emit canUndo( _undoStack.count() > 1 );
  emit changes( _undoStack.count() > 1 );
  emit canRedo( _redoStack.count() > 0 );
}

void KRegExpEditorPrivate::slotSetRegexp( QString regexp )
{
  _regexpEdit->setText( regexp );
}


#include "kregexpeditorprivate.moc"
