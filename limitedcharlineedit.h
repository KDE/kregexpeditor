#ifndef __limitedcharlineedit
#define __limitedcharlineedit

#include <qlineedit.h>

/**
   QLineEdit which only accepts a prespecified number of character.
   @internal
*/
class LimitedCharLineEdit :public QLineEdit 
{
public:
  enum Mode { NORMAL = 0, HEX = 1, OCT = 2 };

	LimitedCharLineEdit(Mode mode, QWidget* parent, const char *name = 0);

protected:
  virtual void keyPressEvent ( QKeyEvent * );

private:
  Mode _mode;
  int _count;
};

#endif

