#ifndef __textwidget
#define __textwidget

#include "regexpwidget.h"
class SelectableLineEdit;
class TextRegExp;

/**
   RegExp widget representing text.

   @internal
*/
class TextWidget : public RegExpWidget
{
Q_OBJECT

public:
  TextWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
             const char *name = 0);
  TextWidget( TextRegExp* regexp,  RegExpEditorWindow* editorWindow,
              QWidget* parent, const char* name = 0);
  virtual QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return TEXT; }
  virtual void updateAll();

protected:
  void init( const QString& text );
  virtual void paintEvent( QPaintEvent *e );
  virtual bool updateSelection( bool parentSelected );
  virtual void clearSelection( bool update );
  virtual bool eventFilter( QObject*, QEvent* );

protected slots:
  void slotUpdate();

private:
  QString text;
  SelectableLineEdit *_edit;

  QSize textSize, boxSize, editSize;
};



#endif // __textwidget
