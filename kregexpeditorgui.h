#ifndef kregexpeditorgui_h
#define kregexpeditorgui_h

#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qvaluestack.h>
#include <qcstring.h>
#include <klibloader.h>
#include <kregexpeditor.h>

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
  static const QString version;

signals:
  void canUndo( bool );
  void canRedo( bool );
  void changes();

public slots:
  void slotRedo();
  void slotUndo();
  void slotSetRegexp( QString regexp );

private:
	KRegExpEditorPrivate* _editor;
};


class KRegExpEditorFactoryImpl :public KLibFactory 
{
protected:
  virtual QObject* createObject( QObject* parent, const char* name, const char *className,
                                 const QStringList & /*args*/ ) 
  {
    if ( strcmp( className, "KRegExpEditor" ) != 0 )
      return 0;
    return new KRegExpEditorGUI( dynamic_cast<QWidget *>( parent ), name );
  }
};

#endif

