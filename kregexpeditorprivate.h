#ifndef kregexpeditorprivate_h
#define kregexpeditorprivate_h

#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qstack.h>
#include <qcstring.h>
#include "regexp.h"

class RegExpScrolledEditorWindow;
class QLineEdit;
class InfoPage;

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
  QStack<RegExp> _undoStack;
  QStack<RegExp> _redoStack;
  CompoundInfo _ci;
};
#endif

