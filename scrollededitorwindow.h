#ifndef __scrolledEditorWindow
#define __scrolledEditorWindow

#include <qhbox.h>
#include "drag.h" // for enum RegExpType. How can I avoid this include?
class QScrollView;
class RegExpEditorWindow;
class RegExp;


/**
   Regular Expression editor window.
*/
class RegExpScrolledEditorWindow :public QWidget
{
Q_OBJECT
public:
  RegExpScrolledEditorWindow( QWidget *parent = 0, const char* name = 0);
  RegExp* regExp();

private:
  RegExpScrolledEditorWindow() {}; // Dont use!
  

protected:
  virtual void resizeEvent( QResizeEvent* );
  
public slots:
  void slotSetRegExp( RegExp* );
  void slotInsertRegExp( int );
  void slotInsertRegExp( RegExp* );
  void slotDeleteSelection();
  void slotDoSelect();

protected slots:
  void slotUpdateContentSize( QPoint focusPoint );
  void slotScroll( QPoint focusPoint );

signals:
  /**
     This signal is emited when the user has completed an editing
     action. The application may chose to call @ref slotDoSelect as a
     consequence to reset to selection mode.
  */
  void doneEditing();

  /**
     This signal is emited whenever a change has taked place in the editor widget
  */
  void change();

  /**
     This signal is emitted when the user saves a regular expression.
  */
  void savedRegexp();
  
private:
  RegExpEditorWindow* _editorWindow;
  QScrollView* _scrollView;
};

#endif // __scrolledEditorWindow
