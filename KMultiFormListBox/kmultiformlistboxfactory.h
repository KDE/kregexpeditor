// -------------------------------- includes ------------------------------
#ifndef __kmultiformlistboxfactory
#define __kmultiformlistboxfactory
#include <qobject.h>
#include "kwidgetstreamer.h"
class KMultiFormListBoxEntry;
//-------------------------------------------------------------------------


/**
   Factory use to generate elements for the @ref KMultiFormListBox widget.

   To use an @ref KMultiFormListBox, one must inherit from the class KMultiFormListBoxFactory
   and override the method @ref create. This method must return an empty
   element for the KMultiFormListBox (that is an @ref KMultiFormListBoxEntry or a subclass of
   it).

   If you dislike the default separator between each of the elements or
   simply do not want a separator in the KMultiFormListBox, then you may override
   the method @ref separator.
**/
class KMultiFormListBoxFactory : public KWidgetStreamer
{
public:
  virtual ~KMultiFormListBoxFactory() {}

  /**
     This method must be overridden in subclasses and must return an
     ``empty'' instance of the @ref KMultiFormListBoxEntry class, or perhaps rather
     a subclass of this class. This instance will be owned by the caller of
     this function.

     @param parent A pointer to the parent of this KMultiFormListBoxEntry widget
     returned.
     @return A fresh @ref KMultiFormListBoxEntry to be used in an instance of the
     @ref KMultiFormListBox class.
  **/
  virtual KMultiFormListBoxEntry *create(QWidget *parent) = 0;

  /**
     This method is used to get a separator between the elements in an @ref
     KMultiFormListBox. The widget returned from this method will be owned by the
     caller.

     @param parent A pointer to the parent of the QWidget returned.
     @return A widget which must be used as a separator between the @ref
     KMultiFormListBoxEntry elements in an @ref KMultiFormListBox.
  **/
  virtual QWidget *separator(QWidget *parent);
};

#endif /* kmultiformlistbox */

