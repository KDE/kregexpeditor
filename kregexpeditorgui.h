#ifndef kregexpeditorgui_h
#define kregexpeditorgui_h

#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qvaluestack.h>
#include <qcstring.h>
#include <kdialogbase.h>
#include <kregexpdialoginterface.h>

class KRegExpEditorPrivate;

/**
   Regular Expression Editor.

   @author Jesper Kjær Pedersen <blackie@kde.org> 
   @version 0.1
 **/
class KRegExpEditorGUI  :public QWidget
{
  Q_OBJECT
  Q_PROPERTY( QString regexp READ regexp WRITE slotSetRegExp )
public:
  KRegExpEditorGUI( QObject *parent, const char *name = 0 );
  virtual QString regexp() const;
  static const QString version;

signals:
  void canUndo( bool );
  void canRedo( bool );
  void changes();
  void changes( bool );

public slots:
  void slotRedo();
  void slotUndo();
  void slotSetRegExp( const QString &regexp );

private:
	KRegExpEditorPrivate* _editor;
};

class KRegExpEditorGUIDialog : public KDialogBase, public KRegExpDialogInterface
{
    Q_OBJECT
public:
    KRegExpEditorGUIDialog( QObject *parent, const char *name = 0 );

    virtual QWidget *regExpEditor() const;

    virtual void slotHelp();

private:
    QWidget *_editor;
};

#endif

