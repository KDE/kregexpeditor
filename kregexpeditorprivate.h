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

// ------------------ FIX
class BackRefId;
typedef QValueList<BackRefId> RegExpIdList;
//-------------------

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
  bool canUndo() const;
  bool canRedo() const;

public: // for use for KRegexpReplace
  RegExpIdList backRefIds();

public slots:
  void slotRedo();
  void slotUndo();

protected slots:
  void slotUpdateEditor( const QString & );
  void slotUpdateLineEdit();
  void slotShowEditor();

protected:  
  void recordUndoInfo();

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

