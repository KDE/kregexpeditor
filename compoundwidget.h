#ifndef __compoundwidget
#define __compoundwidget
class QLineEdit;
class QMultiLineEdit;
class KDialogBase;
#include "singlecontainerwidget.h"
#include "compoundregexp.h"

/**
   Widget containing configuration details for @ref CompoundWidget
   @internal
*/
class CompoundDetailWindow :public QWidget
{
public:
  CompoundDetailWindow(QWidget* parent, const char* name = 0);
  QString title() const;
  QString description() const;
  void setTitle( QString );
  void setDescription( QString );

private:
  QLineEdit* _title;
  QMultiLineEdit* _description;
};


/**
   Comopund RegExp widget.
   
   This widget has two purposes:
   @li To make it possible for a user to collapse a huge regular expression
   to take up less screen space.
   @li To act as back references for later use.

   @internal
*/
class CompoundWidget :public SingleContainerWidget
{
Q_OBJECT

public:
  CompoundWidget( RegExpEditorWindow* editorWindow, QWidget* parent,
                  const char* name = 0);
  CompoundWidget( CompoundRegExp* regexp, RegExpEditorWindow* editorWindow,
                  QWidget* parent, const char* name = 0);

  virtual bool updateSelection( bool parentSelected );
  virtual QSize sizeHint() const;
  virtual RegExp* regExp() const;
  virtual RegExpType type() const { return COMPOUND; }
  virtual int edit();

protected:
  virtual void paintEvent( QPaintEvent *e );
  virtual void mousePressEvent( QMouseEvent* e );
  virtual void mouseReleaseEvent( QMouseEvent* e);
  void init();
  
protected slots:
  void slotConfigCanceled();
  void slotConfigWindowClosed();
  
private:
  bool _hidden;
  QPixmap _up, _down;
  mutable QSize _pixmapSize;
  mutable QPoint _pixmapPos;
  
  KDialogBase* _configWindow;
  CompoundDetailWindow* _content;

  mutable QSize _textSize;
  mutable QSize _childSize;
  QByteArray _backup;
  
  int _backRefId;
};


#endif // __compoundwidget
