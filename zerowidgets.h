#ifndef __zerowidget
#define __zerowidget

#include "regexpwidget.h"

/**
   Represents any widget in the regular expression dialog, which do not have sub widget.
   @internal
*/
class ZeroWidget :public RegExpWidget
{
public:
  ZeroWidget(QString text, RegExpEditorWindow* editorWindow, QWidget *parent,
             const char *name = 0);
  virtual void addNewChild(DragAccepter *accepter, RegExpWidget *child);
  virtual QSize sizeHint() const;

protected:
  virtual void paintEvent( QPaintEvent *e );
  
private:
  QString _text;

  mutable QSize _textSize;
  mutable QSize _boxSize;
};



/**
   RegExp widget for `any single character'
   @internal
*/
class AnyCharWidget :public ZeroWidget
{
public:
  AnyCharWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                const char *label = 0);

	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return DOT; }
};




/**
   RegExp widget for `beginning of line'
   @internal
*/
class BegLineWidget : public ZeroWidget
{
public:
  BegLineWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                const char *name = 0);
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return BEGLINE; }
};




/**
   RegExp widget for `end of line'
   @internal
*/
class EndLineWidget : public ZeroWidget
{
public:
  EndLineWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                const char *name = 0);
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return ENDLINE; }
};



/**
   RegExp widget for `word boundary'
   @internal
*/
class WordBoundaryWidget : public ZeroWidget
{
public:
  WordBoundaryWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                     const char *name = 0);
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return WORDBOUNDARY; }
};




/**
   RegExp widget for `end of word'
   @internal
*/
class NonWordBoundaryWidget : public ZeroWidget
{
public:
  NonWordBoundaryWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                        const char *name = 0);
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return NONWORDBOUNDARY; }
};


#endif // __zerowidget
