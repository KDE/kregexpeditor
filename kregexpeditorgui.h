#ifndef kregexpeditorgui_h
#define kregexpeditorgui_h

#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qvaluestack.h>
#include <qcstring.h>
#include <kdialogbase.h>
#include <kregexpeditorinterface.h>

class KRegExpEditorPrivate;

/**
   Regular Expression Editor.

   @author Jesper Kjær Pedersen <blackie@kde.org> 
   @version 0.1
 **/
class KRegExpEditorGUI  :public QWidget, public KRegExpEditorInterface
{
  Q_OBJECT
  Q_PROPERTY( QString regexp READ regExp WRITE setRegExp )
public:
  KRegExpEditorGUI( QWidget *parent, const char *name = 0, 
	            const QStringList & = QStringList() );
  virtual QString regExp() const;
  virtual void setRegExp( const QString &regexp ) { slotSetRegExp( regexp ); }
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

class KRegExpEditorGUIDialog : public KDialogBase, public KRegExpEditorInterface
{
    Q_OBJECT
    Q_PROPERTY( QString regexp READ regExp WRITE setRegExp );
public:
    KRegExpEditorGUIDialog( QWidget *parent, const char *name, const QStringList &args );

    virtual QString regExp() const;
    virtual void setRegExp( const QString &regExp );

    virtual void slotHelp();

private:
    KRegExpEditorGUI *_editor;
};

#endif

