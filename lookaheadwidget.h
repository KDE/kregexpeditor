#ifndef __lookaheadwidget
#define __lookaheadwidget

#include "singlecontainerwidget.h"
class LookAheadRegExp;

class LookAheadWidget :public SingleContainerWidget
{
  Q_OBJECT
public:
  LookAheadWidget( RegExpEditorWindow* editorWindow, RegExpType tp, QWidget* parent, const char* name = 0 );
  LookAheadWidget( LookAheadRegExp* regexp, RegExpEditorWindow* editorWindow, RegExpType tp, 
                   QWidget* parent, const char* name = 0);

 	virtual RegExp* regExp() const;
 	virtual RegExpType type() const { return _tp; }
  virtual QSize sizeHint() const;

protected:
  void init();
  virtual void paintEvent( QPaintEvent *e );


private:
  RegExpType _tp;
  QString _text;

  mutable QSize _textSize;
  mutable QSize _childSize;  
};

#endif // __lookaheadwidget
