#include "kmultiformlistboxentry.h"
#include <qpushbutton.h>
//----------------------------------------------------------------------
// This function is needed to signal which of the KMultiFormListBox entries
// the Idx button was invoked from.
//----------------------------------------------------------------------
void KMultiFormListBoxEntry::acceptIndexButton() 
{
  emit gotoIndex(this);
}


void KMultiFormListBoxEntry::indexWindowPos(QPoint *start, int *width)
{
  // Calculate the position of the value widgets left-buttom border
  QPoint global_point = valueWidget()->mapToGlobal(QPoint(0,0));
  start->setX(global_point.x());
  start->setY(global_point.y() + valueWidget()->height());

  // Calculate the width of the list.
  global_point = indexButton()->mapToGlobal(QPoint(0,0));
  *width = global_point.x() + indexButton()->width() - start->x();
}
#include "kmultiformlistboxentry.moc"
