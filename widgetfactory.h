#ifndef __widgetfactory
#define __widgetfactory

#include <qdom.h>
class RegExpWidget;
class RegExpEditorWindow;
class QWidget;
class ConcWidget;
class RegExp;

/**
   All the different regular expression types.
*/
enum RegExpType { 
  TEXT = 0, CHARSET = 1, DOT = 2, REPEAT = 3, ALTN = 4, COMPOUND = 5, BEGLINE = 6,
  ENDLINE = 7, WORDBOUNDARY = 8, NONWORDBOUNDARY = 9, CONC = 10, DRAGACCEPTER = 11,
  POSLOOKAHEAD = 12, NEGLOOKAHEAD = 13
};
  
/**
   Class used to encapsulate information about widgets.

   When reading widgets from a stream, it is necessary to know about all
   sub-widgets to the @ref RegExpWidget class. This class exists to make
   sure that such general information is only kept once.

   @internal
*/
class WidgetFactory 
{
public:
  static RegExpWidget* createWidget( RegExpEditorWindow* editorWindow, QWidget* parent, 
                                     RegExpType type );
  static RegExpWidget* createWidget( RegExp* regexp, RegExpEditorWindow* editorWindow,
                                     QWidget* parent );
  static RegExp* createRegExp( QDomElement node, const QString& version );
  static RegExp* createRegExp( QString str );
  static bool isContainer( RegExpType );
};


#endif // __widgetfactory
