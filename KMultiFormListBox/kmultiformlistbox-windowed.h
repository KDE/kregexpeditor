#ifndef __kmultiformlistboxwindowed
#define __kmultiformlistboxwindowed

#include "kmultiformlistboxfactory.h"
#include "kmultiformlistbox-shower.h"
#include "kmultiformlistboxentry.h"
#include "kmultiformlistbox.h"
#include <qlist.h>
#include <qlayout.h>
#include <qscrollview.h>
#include <klistbox.h>
class WindowListboxItem;


/**
   This class implements the windowed look for a @ref KMultiFormListBox

   @internal
*/
class KMultiFormListBoxWindowed  :public QWidget, KMultiFormListBoxShower {

Q_OBJECT

friend class KMultiFormListBox;
  
private:
  KMultiFormListBoxWindowed(KMultiFormListBoxFactory *factory, QWidget *parent, 
														bool showUpDownButtons, bool showHelpButton, QString addButtonText,
														const char *name);

  KMultiFormListBoxEntryList elements(); 
  const KMultiFormListBoxEntryList elements() const; 
  void append(KMultiFormListBoxEntry *);
  WindowListboxItem *selected();
	QWidget* qWidget() { return this; }	

	QVBoxLayout* _layout;
  KMultiFormListBoxFactory* _factory;
	QList<QPushButton> _buttonList;
  KListBox* _listbox;
	virtual void delElement(QWidget *); // Deletes the given element
	virtual void delAnElement();
  void addElement();


public slots:
  void addNewElement();

signals:
	void showHelp();
	
private slots:
  void slotEditSelected();
  void slotEditSelected(QListBoxItem *item);
  void slotCopySelected();
	void slotMoveItemUp();
	void slotMoveItemDown();
  void slotDeleteEntry();
	void slotUpdateButtonState();

};


#endif /* kmultiformlistboxwindowed */

