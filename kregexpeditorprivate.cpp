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
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/

#ifdef QT_ONLY
  #include "compat.h"
#else
  #include <klocale.h>
  #include <kiconloader.h>
  #include <kstandarddirs.h>
  #include <kmessagebox.h>
  #include "kregexpeditorprivate.moc"
#endif

#include <qlineedit.h>
#include "kregexpeditorprivate.h"
#include "scrollededitorwindow.h"
#include "regexpbuttons.h"
#include "regexp.h"
//#include <unistd.h> // What do I need this for?
#include <stdio.h>
#include "infopage.h"
#include <qsplitter.h>
#include <qdockarea.h>
#include "userdefinedregexps.h"
#include "auxbuttons.h"
#include <qaccel.h>
#include <compoundregexp.h>
#include <qtimer.h>
#include "verifier.h"
#include <qfile.h>
#include "verifybuttons.h"
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qvalidator.h>
#include <qregexp.h>
#include "regexpconverter.h"

KRegExpEditorPrivate::KRegExpEditorPrivate(QWidget *parent, const char *name)
    : QWidget(parent, name), _updating( false ), _autoVerify( true )
{
  setMinimumSize(730,300);
  QDockArea* area = new QDockArea( Horizontal, QDockArea::Normal, this );
  area->setMinimumSize(2,2);
  QDockArea* verArea1 = new QDockArea( Vertical, QDockArea::Normal, this );
  verArea1->setMinimumSize(2,2);
  QDockArea* verArea2 = new QDockArea( Vertical, QDockArea::Reverse, this );
  verArea2->setMinimumSize(2,2);

  // The DockWindows.
  _regExpButtons = new RegExpButtons( area, "KRegExpEditorPrivate::regExpButton" );
  _verifyButtons = new VerifyButtons( area, "KRegExpEditorPrivate::VerifyButtons" );
  _auxButtons = new AuxButtons( area, "KRegExpEditorPrivate::AuxButtons" );
  _userRegExps = new UserDefinedRegExps( verArea1, "KRegExpEditorPrivate::userRegExps" );
  _userRegExps->setResizeEnabled( true );
  QWhatsThis::add( _userRegExps, i18n( "In this window you will find predefined regular expressions. Both regular expressions "
                                       "you have developed and saved, and regular expressions shipped with the system." ));

  // Editor window
  _editor = new QSplitter( Vertical, this, "KRegExpEditorPrivate::_editor" );

  _scrolledEditorWindow =
    new RegExpScrolledEditorWindow( _editor, "KRegExpEditorPrivate::_scrolledEditorWindow" );
  QWhatsThis::add( _scrolledEditorWindow, i18n( "In this window you will develop your regular expressions. "
                                               "Select one of the actions from the action buttons above, and click the mouse in this "
                                               "window to insert the given action."));

  _info = new InfoPage( this, "_info" );
  _verifier = new Verifier( _editor, "KRegExpEditorPrivate::_verifier" );
  connect( _verifier, SIGNAL( textChanged() ), this, SLOT( maybeVerify() ) );
  QWhatsThis::add( _verifier, i18n("Type in some text in this window, and see what the regular expression you have developed matches.<p>"
                                   "Each second match will be colored in red and each second match will be colored blue, simply so you "
                                   "can distinguish them from each other.<p>"
                                   "If you select part of the regular expression in the editor window, then this part will be "
                                   "highlighted - This allows you to <i>debug</i> your regular expressions") );

  _editor->hide();
  _editor->setSizes( QValueList<int>() << _editor->height()/2 << _editor->height()/2 );

  QVBoxLayout *topLayout = new QVBoxLayout( this, 0, 6, "KRegExpEditorPrivate::topLayout" );
  topLayout->addWidget( area );
  QHBoxLayout* rows = new QHBoxLayout; // I need to cal addLayout explicit to get stretching right.
  topLayout->addLayout( rows, 1 );

  rows->addWidget( verArea1 );
  rows->addWidget( _editor, 1 );
  rows->addWidget( _info, 1 );
  rows->addWidget( verArea2 );

  // Connect the buttons
  connect( _regExpButtons, SIGNAL( clicked( int ) ),   _scrolledEditorWindow, SLOT( slotInsertRegExp( int ) ) );
  connect( _regExpButtons, SIGNAL( doSelect() ), _scrolledEditorWindow, SLOT( slotDoSelect() ) );
  connect( _userRegExps, SIGNAL( load( RegExp* ) ),    _scrolledEditorWindow, SLOT( slotInsertRegExp( RegExp*  ) ) );

  connect( _regExpButtons, SIGNAL( clicked( int ) ), _userRegExps,   SLOT( slotUnSelect() ) );
  connect( _regExpButtons, SIGNAL( doSelect() ),     _userRegExps,   SLOT( slotUnSelect() ) );
  connect( _userRegExps, SIGNAL( load( RegExp* ) ),  _regExpButtons, SLOT( slotUnSelect() ) );

  connect( _scrolledEditorWindow, SIGNAL( doneEditing() ), _regExpButtons, SLOT( slotSelectNewAction() ) );
  connect( _scrolledEditorWindow, SIGNAL( doneEditing() ), _userRegExps, SLOT( slotSelectNewAction() ) );

  connect( _regExpButtons, SIGNAL( clicked( int ) ), this, SLOT( slotShowEditor() ) );
  connect( _userRegExps, SIGNAL( load( RegExp* ) ), this, SLOT( slotShowEditor() ) );
  connect( _regExpButtons, SIGNAL( doSelect() ), this, SLOT( slotShowEditor() ) );

  connect( _scrolledEditorWindow, SIGNAL( savedRegexp() ), _userRegExps, SLOT( slotPopulateUserRegexps() ) );

  connect( _auxButtons, SIGNAL( undo() ), this, SLOT( slotUndo() ) );
  connect( _auxButtons, SIGNAL( redo() ), this, SLOT( slotRedo() ) );
  connect( _auxButtons, SIGNAL( cut() ), _scrolledEditorWindow, SLOT( slotCut() ) );
  connect( _auxButtons, SIGNAL( copy() ), _scrolledEditorWindow, SLOT( slotCopy() ) );
  connect( _auxButtons, SIGNAL( paste() ), _scrolledEditorWindow, SLOT( slotPaste() ) );
  connect( _auxButtons, SIGNAL( save() ), _scrolledEditorWindow, SLOT( slotSave() ) );
  connect( _verifyButtons, SIGNAL( autoVerify( bool ) ), this, SLOT( setAutoVerify( bool ) ) );
  connect( _verifyButtons, SIGNAL( verify() ), this, SLOT( doVerify() ) );
  connect( _verifyButtons, SIGNAL( changeSyntax( const QString& ) ), this, SLOT( setSyntax( const QString& ) ) );

  connect( this, SIGNAL( canUndo( bool ) ), _auxButtons, SLOT( slotCanUndo( bool ) ) );
  connect( this, SIGNAL( canRedo( bool ) ), _auxButtons, SLOT( slotCanRedo( bool ) ) );
  connect( _scrolledEditorWindow, SIGNAL( anythingSelected( bool ) ), _auxButtons, SLOT( slotCanCut( bool ) ) );
  connect( _scrolledEditorWindow, SIGNAL( anythingSelected( bool ) ), _auxButtons, SLOT( slotCanCopy( bool ) ) );
  connect( _scrolledEditorWindow, SIGNAL( anythingOnClipboard( bool ) ), _auxButtons, SLOT( slotCanPaste( bool ) ) );
  connect( _scrolledEditorWindow, SIGNAL( canSave( bool ) ), _auxButtons, SLOT( slotCanSave( bool ) ) );

  connect( _scrolledEditorWindow, SIGNAL( verifyRegExp() ), this, SLOT( maybeVerify() ) );

  connect( _verifyButtons, SIGNAL( loadVerifyText( const QString& ) ), this, SLOT( setVerifyText( const QString& ) ) );

  // connect( _verifier, SIGNAL( countChanged( int ) ), _verifyButtons, SLOT( setMatchCount( int ) ) );

  // Qt anchors do not work for <pre>...</pre>, thefore scrolling to next/prev match
  // do not work. Enable this when they work.
  // connect( _verifyButtons, SIGNAL( gotoFirst() ), _verifier, SLOT( gotoFirst() ) );
  // connect( _verifyButtons, SIGNAL( gotoPrev() ), _verifier, SLOT( gotoPrev() ) );
  // connect( _verifyButtons, SIGNAL( gotoNext() ), _verifier, SLOT( gotoNext() ) );
  // connect( _verifyButtons, SIGNAL( gotoLast() ), _verifier, SLOT( gotoLast() ) );
  // connect( _verifier, SIGNAL( goForwardPossible( bool ) ), _verifyButtons, SLOT( enableForwardButtons( bool ) ) );
  // connect( _verifier, SIGNAL( goBackwardPossible( bool ) ), _verifyButtons, SLOT( enableBackwardButtons( bool ) ) );

  _auxButtons->slotCanPaste( false );
  _auxButtons->slotCanCut( false );
  _auxButtons->slotCanCopy( false );
  _auxButtons->slotCanSave( false );


  // Line Edit
  QHBoxLayout* layout = new QHBoxLayout( topLayout, 6 );
  QLabel* label = new QLabel( i18n("ASCII syntax:"), this );
  layout->addWidget( label );
  _regexpEdit = new QLineEdit( this );
  layout->addWidget( _regexpEdit );
  QWhatsThis::add( _regexpEdit, i18n( "This is the regular expression in ASCII syntax. You are likely only "
				      "to be interested in this if you are a programmer, and need to "
				      "develop a regular expression using QRegExp.<p>"
                                      "You may evelop your regular expression both by using the graphical "
				      "editor, and by typing the regular expression in this line edit.") );

#ifdef QT_ONLY
  QPixmap pix( "icons/error.png" );
#else
  QPixmap pix = KGlobal::iconLoader()->loadIcon(locate("data", QString::fromLatin1("kregexpeditor/pics/error.png") ), KIcon::Toolbar );
#endif
  _error = new QLabel( this );
  _error->setPixmap( pix );
  layout->addWidget( _error );
  _error->hide();

  _timer = new QTimer(this);

  connect( _scrolledEditorWindow, SIGNAL( change() ), this, SLOT( slotUpdateLineEdit() ) );
  connect( _regexpEdit, SIGNAL(textChanged( const QString& ) ), this, SLOT( slotTriggerUpdate() ) );
  connect( _timer, SIGNAL( timeout() ), this, SLOT( slotTimeout() ) );

  // Push an initial empty element on the stack.
  _undoStack.push( _scrolledEditorWindow->regExp() );
  _redoStack.setAutoDelete( true );

  QAccel* accel = new QAccel( this );
  accel->connectItem( accel->insertItem( CTRL+Key_Z ), this, SLOT( slotUndo() ) );
  accel->connectItem( accel->insertItem( CTRL+Key_R ), this, SLOT( slotRedo() ) );

  setSyntax( QString::fromLatin1( "Qt" ) );
}

QString KRegExpEditorPrivate::regexp()
{
  RegExp* regexp = _scrolledEditorWindow->regExp();
  QString res = RegExpConverter::current()->toStr( regexp, false );
  delete regexp;
  return res;
}

void KRegExpEditorPrivate::slotUpdateEditor( const QString & txt)
{
  _updating = true;
  bool ok;
  if ( !RegExpConverter::current()->canParse() ) {
      // This can happend if the application set a text through the API.
  }
  else {
      RegExp* result = RegExpConverter::current()->parse( txt, &ok );
      if ( ok ) {
          QPtrList<CompoundRegExp> list = _userRegExps->regExps();
          for ( QPtrListIterator<CompoundRegExp> it( list ); *it; ++it ) {
              result->replacePart( *it );
          }

          _scrolledEditorWindow->slotSetRegExp( result );
          _error->hide();
          maybeVerify( );
          recordUndoInfo();
          result->check( _errorMap );
      }
      else {
          _error->show();
          if ( _autoVerify )
              _verifier->clearRegexp();
      }
      delete result;
  }
  _updating = false;
}

void KRegExpEditorPrivate::slotUpdateLineEdit()
{
  if ( _updating )
    return;
  _updating = true;

  RegExp* regexp = _scrolledEditorWindow->regExp();
  regexp->check( _errorMap );

  QString str = RegExpConverter::current()->toStr( regexp, false );
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
    maybeVerify();
  }
}

void KRegExpEditorPrivate::slotUndo()
{
  if ( _undoStack.count() > 1 ) {
    _redoStack.push(_undoStack.pop());
    _scrolledEditorWindow->slotSetRegExp( _undoStack.top() );
    slotUpdateLineEdit();
    emitUndoRedoSignals();
    maybeVerify();
  }
}

void KRegExpEditorPrivate::slotShowEditor()
{
    _info->hide();
    _editor->show();
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

void KRegExpEditorPrivate::slotTriggerUpdate()
{
  /* ### Guess this timeout value should be configurable somewhere, or (even
   * better: do some kind of benchmark each time the editor view gets updated
   * to measure how long it takes on the client system to render the editor
   * with the current complexity. That way we'd get good response times for
   * simple regexps, and flicker-free display for complex regexps.
   * - Frerich
   */
  if ( !_updating ) {
    _timer->start( 300, true );
    slotShowEditor();
  }
}

void KRegExpEditorPrivate::slotTimeout()
{
  slotUpdateEditor( _regexpEdit->text() );
}

void KRegExpEditorPrivate::setMatchText( const QString& text )
{
    bool autoVerify = _autoVerify;
    _autoVerify = false;
    _verifier->setText( text );
    _autoVerify = autoVerify;
}

void KRegExpEditorPrivate::maybeVerify()
{
    if ( _autoVerify )
        doVerify();
    else
        _verifyButtons->setMatchCount(-1);
}

void KRegExpEditorPrivate::doVerify()
{
    bool autoVerify = _autoVerify; // prevent loop due to verify emit changed, which calls maybeVerify
    _autoVerify = false;
    RegExp* regexp = _scrolledEditorWindow->regExp();

    _verifier->verify( RegExpConverter::current()->toStr( regexp, true ) );
    delete regexp;
    _autoVerify = autoVerify;
}

void KRegExpEditorPrivate::setAutoVerify( bool on )
{
    _autoVerify = on;
    if ( !_autoVerify )
        _verifier->clearRegexp();
    else
        doVerify();
}

void KRegExpEditorPrivate::setVerifyText( const QString& fileName )
{
    bool autoVerify = _autoVerify;
    _autoVerify = false;
    QFile file( fileName );
    if ( !file.open( IO_ReadOnly ) ) {
        KMessageBox::sorry(0, i18n("Couldn't open file '%1' for reading").arg( fileName ) );
    }
    else {
        QTextStream s( &file );
        QString txt = s.read();
        file.close();
        RegExp* regexp = _scrolledEditorWindow->regExp();
        _verifier->setText( txt );
        _verifier->verify( RegExpConverter::current()->toStr( regexp, true ) );
        delete regexp;
    }
    _autoVerify = autoVerify;
}

void KRegExpEditorPrivate::setCaseSensitive( bool b )
{
    _verifier->setCaseSensitive( b );
}

void KRegExpEditorPrivate::setMinimal( bool b )
{
    _verifier->setMinimal( b );
}

void KRegExpEditorPrivate::setSyntax( const QString& syntax )
{
    RegExpConverter* converter = _verifyButtons->setSyntax( syntax );
    RegExpConverter::setCurrent( converter );
    if ( converter->canParse() ) {
        _regexpEdit->setReadOnly( false );
        _regexpEdit->setBackgroundMode( Qt::PaletteBase );
    }
    else {
        _regexpEdit->setReadOnly( true );
        _regexpEdit->setBackgroundMode( Qt::PaletteBackground );
    }
    _regExpButtons->setFeatures( converter->features() );
    _verifier->setHighlighter( converter->highlighter(_verifier) );
    slotUpdateLineEdit();
}

void KRegExpEditorPrivate::showHelp()
{
    _info->show();
    _editor->hide();
}

void KRegExpEditorPrivate::setAllowNonQtSyntax( bool b )
{
    _verifyButtons->setAllowNonQtSyntax( b );
}
