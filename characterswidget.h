#ifndef __characterswidget
#define __characterswidget

#include <kmultiformlistbox.h>
#include "regexpwidget.h"
#include "limitedcharlineedit.h"
#include <qhbox.h>
#include <qcheckbox.h>
#include <qvgroupbox.h>
#include <kdialogbase.h>
class KDialogBase;
class CharacterEdits;
class TextRangeRegExp;
class CharSelector;

/**
   RegExp widget for charcter ranges.
   @internal
*/
class CharactersWidget :public RegExpWidget
{
public:
  CharactersWidget(RegExpEditorWindow* editorWindow, QWidget *parent,
                   const char *label = 0);
  CharactersWidget( TextRangeRegExp* regexp, RegExpEditorWindow* editorWindow,
                    QWidget* parent, const char* name = 0 );
  ~CharactersWidget();
  virtual QSize sizeHint() const;
	virtual RegExp* regExp() const;
  virtual RegExpType type() const { return CHARSET; }
  virtual RegExpWidget* findWidgetToEdit( QPoint globalPos );
  virtual int edit();
  
protected:
  virtual void paintEvent(QPaintEvent *event);
  QString text() const;
  QString title() const;

private:
  TextRangeRegExp *_regexp;
  CharacterEdits *_configWindow;

  mutable QSize _textSize;
  mutable QSize _contentSize;
};


/**
   @internal
*/
class SingleEntry :public KMultiFormListBoxEntry
{
public:
  SingleEntry(QWidget* parent, const char* name = 0 );
  QString text() const;
  void setText( QString text );
  bool isEmpty() const;
  
private:
  CharSelector* _selector;
};

/**
   @internal
*/
class RangeEntry :public KMultiFormListBoxEntry
{
public:
  RangeEntry(QWidget* parent, const char* name = 0 );
  QString fromText() const;
  QString toText() const;
  void setFrom( QString text );
  void setTo( QString text );
  bool isEmpty() const;
private:
  CharSelector *_from, *_to;  
};

/**
   @internal
*/
class SingleFactory :public KMultiFormListBoxFactory 
{
public:
  KMultiFormListBoxEntry *create(QWidget *parent) { return new SingleEntry( parent ); }    
  QWidget *separator(QWidget *parent) { return 0; }
};

/**
   @internal
*/
class RangeFactory :public KMultiFormListBoxFactory 
{
public:
  KMultiFormListBoxEntry *create(QWidget *parent) { return new RangeEntry( parent ); }
  QWidget *separator(QWidget *parent) { return 0; }
};

/**
   @internal
*/
class CharacterEdits : public KDialogBase
{
  Q_OBJECT
public:
  CharacterEdits(TextRangeRegExp* _regexp, QWidget *parent = 0, const char *name = 0);
  
public slots:
  int exec();

protected slots:
  void slotOK();
  
private:
  QCheckBox *negate, *wordChar, *nonWordChar, *digit, *nonDigit, *space, *nonSpace;  
  KMultiFormListBox *_single, *_range;

  void addCharacter( QString txt );
  void addRange( QString from, QString to );
  TextRangeRegExp* _regexp;  
};

#endif // __characterswidget
