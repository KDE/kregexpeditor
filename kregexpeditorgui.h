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

  static const QString version;

signals:
  /** This signal tells whether undo is available. */
  virtual void canRedo( bool );
  virtual void canUndo( bool );
  virtual void changes( bool );

public slots:
  virtual void redo();
  virtual void undo();
  virtual void setRegExp( const QString &regexp );
  virtual void doSomething( QString method, void* arguments );
  virtual void setMatchText( const QString& );

private:
	KRegExpEditorPrivate* _editor;
};

class KRegExpEditorGUIDialog : public KDialogBase, public KRegExpEditorInterface
{
    Q_OBJECT
    Q_PROPERTY( QString regexp READ regExp WRITE setRegExp )
public:
    KRegExpEditorGUIDialog( QWidget *parent, const char *name, const QStringList &args );

    virtual QString regExp() const;

signals:
  /** This signal tells whether undo is available. */
  virtual void canRedo( bool );
  virtual void canUndo( bool );
  virtual void changes( bool );

public slots:
  virtual void redo();
  virtual void undo();
  virtual void setRegExp( const QString &regexp );
  virtual void doSomething( QString method, void* arguments );
  virtual void setMatchText( const QString& );

private:
    KRegExpEditorGUI *_editor;
};

#endif

