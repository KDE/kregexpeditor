#ifndef kregexpeditorprivate_h
#define kregexpeditorprivate_h

#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qptrstack.h>
#include <qcstring.h>
#include "regexp.h"
class QTimer;

class RegExpScrolledEditorWindow;
class QLineEdit;
class InfoPage;
class UserDefinedRegExps;

/**
   Widget used to build a regular expression

   @author Jesper Kjær Pedersen <blackie@kde.org> 
   @version 0.1
 **/
class KRegExpEditorPrivate  :public QWidget
{
Q_OBJECT

public:
  KRegExpEditorPrivate( QWidget *parent, const char *name = 0 );
  QString regexp();

protected slots:
  void slotUpdateEditor( const QString & );
  void slotUpdateLineEdit();
  void slotShowEditor();
  void slotTriggerUpdate();
  void slotTimeout();

public slots:
  void slotUndo();
  void slotRedo();
  void slotSetRegexp( QString regexp );

signals:
  void canUndo( bool );
  void canRedo( bool );
  void changes( bool );

protected:  
  void recordUndoInfo();
  void emitUndoRedoSignals();

private:
	RegExpScrolledEditorWindow* _scrolledEditorWindow;
  InfoPage* _info;
  QLineEdit* _regexpEdit;
  bool _updating;
  QLabel* _error;
  QPtrStack<RegExp> _undoStack;
  QPtrStack<RegExp> _redoStack;
  UserDefinedRegExps*  _userRegExps;
  QTimer* _timer;
  
  /**
     When the slotSetRegExp is invoked, the editor should not automaticly be shown
     (meaning hiding the spash page.)
     This flag is set to prevent this.
  */
  bool _preventShow;
};

#endif

