#ifndef kregexpeditorgui_h
#define kregexpeditorgui_h

#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qvaluestack.h>
#include <qcstring.h>
#include <kregexpeditor.h>
#include <kregexpeditorfactory.h>

class KRegExpEditorPrivate;

/**
   Regular Expression Editor.

   @author Jesper Kjær Pedersen <blackie@kde.org> 
   @version 0.1
 **/
class KRegExpEditorGUI  :public KRegExpEditor
{
Q_OBJECT

public:
  KRegExpEditorGUI( QWidget *parent, const char *name = 0 );
  virtual QString regexp();
  virtual bool canUndo() const;
  virtual bool canRedo() const;

  static const QString version;


public slots:
  void slotRedo();
  void slotUndo();

private:
	KRegExpEditorPrivate* _editor;
};


class KRegExpEditorFactoryImpl :public KRegExpEditorFactory 
{
public:
  KRegExpEditorFactoryImpl( QWidget* parent = 0, const char* name = 0 ) : KRegExpEditorFactory( parent, name ) 
  {
  }
  
  KRegExpEditorGUI* create( QWidget* parent, const char* name ) 
  {
    return new KRegExpEditorGUI( parent, name );
  }
};

#endif

