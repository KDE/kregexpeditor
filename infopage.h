#ifndef infopage_h
#define infopage_h

#include <ktextbrowser.h>

class InfoPage :public KTextBrowser
{
public:
  InfoPage( QWidget* parent, const char* name );
  virtual void setSource ( const QString & name );
};



#endif
