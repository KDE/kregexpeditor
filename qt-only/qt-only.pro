TEMPLATE = app
INCLUDEPATH += .
DEFINES += QT_ONLY
TARGET = kregexpeditor

win32-msvc:QMAKE_CXXFLAGS += /GX /GR
QMAKE_CLEAN+=vc60.pdb kdexecutor.dsp


# If this is a compile for House Of Fusion, then add the following line
DEFINES+=HOUSEOFFUSION

# Input
HEADERS += altnregexp.h \
           altnwidget.h \
           auxbuttons.h \
           ccp.h \
           characterswidget.h \
           charselector.h \
           compoundregexp.h \
           compoundwidget.h \
           concregexp.h \
           concwidget.h \
           dcbutton.h \
           dotregexp.h \
           drag.h \
           dragaccepter.h \
           editorwindow.h \
           emacsregexpconverter.h \
           errormap.h \
           gen_qregexpparser.hh \
           indexWindow.h \
           infopage.h \
           kmultiformlistbox-multivisible.h \
           kmultiformlistbox-shower.h \
           kmultiformlistbox-windowed.h \
           kmultiformlistbox.h \
           kmultiformlistboxentry.h \
           kmultiformlistboxfactory.h \
           kregexpeditorgui.h \
           kregexpeditorprivate.h \
           kwidgetstreamer.h \
           limitedcharlineedit.h \
           lookaheadregexp.h \
           lookaheadwidget.h \
           multicontainerwidget.h \
           myfontmetrics.h \
           pair.h \
           positionregexp.h \
           qtregexpconverter.h \
           qtregexphighlighter.h \
           regexp.h \
           regexpbuttons.h \
           regexpconverter.h \
           regexphighlighter.h \
           regexpwidget.h \
           repeatregexp.h \
           repeatwidget.h \
           scrollededitorwindow.h \
           selectablelineedit.h \
           singlecontainerwidget.h \
           textrangeregexp.h \
           textregexp.h \
           textwidget.h \
           triple.h \
           userdefinedregexps.h \
           verifier.h \
           verifybuttons.h \
           widgetfactory.h \
           widgetwindow.h \
           windowlistboxitem.h \
           zerowidgets.h\
           compat.h \
           images.h

#LEXSOURCES += qregexpparser.l
#YACCSOURCES += qregexpparser.y
YACC=bison
SOURCES += altnregexp.cpp \
           altnwidget.cpp \
           auxbuttons.cpp \
           ccp.cpp \
           characterswidget.cpp \
           charselector.cpp \
           compoundregexp.cpp \
           compoundwidget.cpp \
           concregexp.cpp \
           concwidget.cpp \
           dcbutton.cpp \
           dotregexp.cpp \
           drag.cpp \
           dragaccepter.cpp \
           editorwindow.cpp \
           emacsregexpconverter.cpp \
           errormap.cpp \
           gen_qregexplexer.cpp \
           indexWindow.cpp \
           infopage.cpp \
           kmultiformlistbox-multivisible.cpp \
           kmultiformlistbox-windowed.cpp \
           kmultiformlistbox.cpp \
           kmultiformlistboxentry.cpp \
           kmultiformlistboxfactory.cpp \
           kregexpeditorgui.cpp \
           kregexpeditorprivate.cpp \
           kwidgetstreamer.cpp \
           limitedcharlineedit.cpp \
           lookaheadregexp.cpp \
           lookaheadwidget.cpp \
           main.cpp \
           multicontainerwidget.cpp \
           myfontmetrics.cpp \
           positionregexp.cpp \
           qtregexpconverter.cpp \
           qtregexphighlighter.cpp \
           regexp.cpp \
           regexpbuttons.cpp \
           regexpconverter.cpp \
           regexphighlighter.cpp \
           regexpwidget.cpp \
           repeatregexp.cpp \
           repeatwidget.cpp \
           scrollededitorwindow.cpp \
           selectablelineedit.cpp \
           singlecontainerwidget.cpp \
           textrangeregexp.cpp \
           textregexp.cpp \
           textwidget.cpp \
           userdefinedregexps.cpp \
           verifier.cpp \
           verifybuttons.cpp \
           widgetfactory.cpp \
           widgetwindow.cpp \
           windowlistboxitem.cpp \
           zerowidgets.cpp \
           gen_qregexpparser.cpp \
           compat.cpp
