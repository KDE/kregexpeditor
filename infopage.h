#include "qtextbrowser.h"

class InfoPage :public QTextBrowser 
{
public:
  InfoPage( QWidget* parent, const char* name );
  virtual void setSource ( const QString & name );
};


