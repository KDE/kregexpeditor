#ifndef __kmultiformlistboxshower
#define __kmultiformlistboxshower

// -------------------------------- includes ------------------------------
#include "kmultiformlistboxentry.h"
#include <qlist.h>
#include "kmultiformlistboxfactory.h"
#include "kmultiformlistboxentry.h"
//-------------------------------------------------------------------------
typedef QList<KMultiFormListBoxEntry> KMultiFormListBoxEntryList ;

/**
   Abstract class defining the interface for widgets showing a number of @ref KMultiFormListBoxEntry.

   @internal
*/
class KMultiFormListBoxShower {
  
public:
  virtual KMultiFormListBoxEntryList elements() = 0; 

  /** Appends the given argument to the list */
  virtual void append(KMultiFormListBoxEntry *) = 0;

  /** Return the element as a QWidget */
	virtual QWidget* qWidget() = 0;
  
  /** Adds an empty element. */
  virtual void addElement() = 0; 

  /** Deletes an element at the given index. */
	virtual void delElement(QWidget *) = 0;

  /** Deletes any element from the list */
	virtual void delAnElement() = 0;
};


#endif /* kmultiformlistboxshower */

