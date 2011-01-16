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

#include "kregexpeditorprivate.h"

#include <KLocale>
#include <KIconLoader>
#include <KStandardDirs>
#include <KMessageBox>
#include <KLineEdit>

#include <QApplication>
#include <QToolButton>
#include <QTextStream>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTimer>
#include <QFile>
#include <QShortcut>
#include <QLabel>

#include "auxbuttons.h"
#include "verifybuttons.h"
#include "regexpbuttons.h"
#include "userdefinedregexps.h"
#include "scrollededitorwindow.h"
#include "infopage.h"
#include "verifier.h"
#include "regexpconverter.h"
#include "regexp.h"

KRegExpEditorPrivate::KRegExpEditorPrivate(QWidget *parent)
    : QMainWindow(parent), _updating( false ), _autoVerify( true ), _matchGreedy( false )
{
  setMinimumSize(730,300);
  setWindowFlags(Qt::Widget);

  // The DockWindows.
  _regExpButtons = new RegExpButtons( /*area*/this, "KRegExpEditorPrivate::regExpButton" );
  addToolBar(Qt::TopToolBarArea, _regExpButtons);

  _verifyButtons = new VerifyButtons( /*area*/this, "KRegExpEditorPrivate::VerifyButtons" );
  addToolBar(Qt::TopToolBarArea, _verifyButtons);
  
  _auxButtons = new AuxButtons( /*area*/this, "KRegExpEditorPrivate::AuxButtons" );
  addToolBar(Qt::TopToolBarArea, _auxButtons);
  
  _userRegExps = new UserDefinedRegExps( /*verArea1*/this, /*"KRegExpEditorPrivate::userRegExps"*/qPrintable(i18n("Compound regular expression:")) );
  _userRegExps->setWhatsThis( i18n( "In this window you will find predefined regular expressions. Both regular expressions "
                                       "you have developed and saved, and regular expressions shipped with the system." ));
  addDockWidget(Qt::LeftDockWidgetArea, _userRegExps);

  // Editor window
  _editor = new QSplitter( Qt::Vertical, this );
  _editor ->setObjectName( "KRegExpEditorPrivate::_editor" );

  _scrolledEditorWindow = new RegExpScrolledEditorWindow( _editor );
  _scrolledEditorWindow->setWhatsThis( i18n( "In this window you will develop your regular expressions. "
                                               "Select one of the actions from the action buttons above, and click the mouse in this "
                                               "window to insert the given action."));

  _info = new InfoPage( this );
  _info->setObjectName( "_info" );
  _verifier = new Verifier( _editor );
  connect( _verifier, SIGNAL( textChanged() ), this, SLOT( maybeVerify() ) );
  _verifier->setWhatsThis( i18n("<p>Type in some text in this window, and see what the regular expression you have developed matches.</p>"
                                   "<p>Each second match will be colored in red and each other match will be colored blue, simply so you "
                                   "can distinguish them from each other.</p>"
                                   "<p>If you select part of the regular expression in the editor window, then this part will be "
                                   "highlighted - This allows you to <i>debug</i> your regular expressions</p>") );

  _editor->hide();
  _editor->setSizes( QList<int>() << _editor->height()/2 << _editor->height()/2 );
  
  QWidget* centralWidget = new QWidget(this);
  QHBoxLayout* layout = new QHBoxLayout(centralWidget);
  layout->addWidget(_editor);
  layout->addWidget(_info);
  setCentralWidget(centralWidget);

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
  connect( _verifyButtons, SIGNAL( matchGreedy( bool ) ), this, SLOT( setMatchGreedy( bool ) ) );

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
  QDockWidget* editDock = new QDockWidget(i18n("ASCII syntax:"), this);
  editDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  addDockWidget(Qt::BottomDockWidgetArea, editDock);
  
  QWidget* editDockWidget = new QWidget(editDock);
  editDock->setWidget(editDockWidget);
  QHBoxLayout* dockLayout = new QHBoxLayout(editDockWidget);
  dockLayout->setSpacing( 6 );
  
  clearButton = new QToolButton( editDockWidget );
  const QString icon( QString::fromLatin1( QApplication::isRightToLeft() ? "edit-clear-locationbar-rtl" : "edit-clear-locationbar-ltr" ) );
  KIcon clearIcon( icon );
  clearButton->setIcon( clearIcon );
  dockLayout->addWidget( clearButton );
  clearButton->setToolTip( i18n("Clear expression") );
  
  _regexpEdit = new KLineEdit( editDockWidget );
  dockLayout->addWidget( _regexpEdit );
  _regexpEdit->setFocus(Qt::OtherFocusReason);
  _regexpEdit->setWhatsThis( i18n( "<p>This is the regular expression in ASCII syntax. You are likely only "
				      "to be interested in this if you are a programmer, and need to "
				      "develop a regular expression using QRegExp.</p>"
                                      "<p>You may develop your regular expression both by using the graphical "
				      "editor, and by typing the regular expression in this line edit.</p>") );
  
  QPixmap pix = KIconLoader::global()->loadIcon(KStandardDirs::locate("data", QString::fromLatin1("kregexpeditor/pics/error.png") ), KIconLoader::Toolbar );
  _error = new QLabel( editDockWidget );
  _error->setPixmap( pix );
  dockLayout->addWidget( _error );
  _error->hide();

  _timer = new QTimer(this);
  _timer->setSingleShot( true );

  connect( _scrolledEditorWindow, SIGNAL( change() ), this, SLOT( slotUpdateLineEdit() ) );
  connect( _regexpEdit, SIGNAL(textChanged( const QString& ) ), this, SLOT( slotTriggerUpdate() ) );
  connect( _timer, SIGNAL( timeout() ), this, SLOT( slotTimeout() ) );
  connect( clearButton, SIGNAL( clicked() ), _regexpEdit, SLOT( clear() ) );

  // Push an initial empty element on the stack.
  _undoStack.push( _scrolledEditorWindow->regExp() );

  (void) new QShortcut( Qt::CTRL+Qt::Key_Z , this, SLOT( slotUndo() ) );
  (void) new QShortcut( Qt::CTRL+Qt::Key_R , this, SLOT( slotRedo() ) );

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
      //qDebug("cannot parse");
  }
  else {
      RegExp* result = RegExpConverter::current()->parse( txt, &ok );
      if ( ok ) {
          QList<CompoundRegExp *> list = _userRegExps->regExps();
          foreach ( CompoundRegExp* regExp, list ) {
              result->replacePart( regExp );
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
    qDeleteAll(_redoStack);
    _redoStack = QStack<RegExp *>();
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
    _timer->start( 300 );
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
    if ( !file.open( QIODevice::ReadOnly ) ) {
        KMessageBox::sorry(0, i18n("Could not open file '%1' for reading", fileName ) );
    }
    else {
        QTextStream s( &file );
        QString txt = s.readAll();
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
        _regexpEdit->setBackgroundRole( QPalette::Base );
    }
    else {
        _regexpEdit->setReadOnly( true );
        _regexpEdit->setBackgroundRole( QPalette::Background );
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

void KRegExpEditorPrivate::setMatchGreedy( bool b )
{
  _matchGreedy = b;
  _verifier->setMinimal( !b );
  doVerify();
}

#include "kregexpeditorprivate.moc"
