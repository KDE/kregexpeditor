#include "kmultiformlistboxfactory.h"
#include <qframe.h>

QWidget *KMultiFormListBoxFactory::separator(QWidget *parent) {
  QFrame* sep = new QFrame( parent );
  sep->setFrameStyle( QFrame::HLine | QFrame::Sunken);
  sep->setLineWidth(1);
  return sep;
}
