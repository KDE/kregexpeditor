#include "dcbutton.h"

DoubleClickButton::DoubleClickButton( QPixmap pixmap, QWidget* parent, const char* name )
  : QPushButton( parent, name ? name : "DoubleClickButton" )
{
  setPixmap( pixmap );
}

void DoubleClickButton::mouseDoubleClickEvent( QMouseEvent* )
{
  emit doubleClicked();
}

#include "dcbutton.moc"
  
