kde_module_LTLIBRARIES = libkregexpeditorgui.la
AM_CPPFLAGS = -DQT_NO_CAST_ASCII
SUBDIRS = KWidgetStreamer KMultiFormListBox picts predefined
INCLUDES = -I$(srcdir)/KWidgetStreamer -I$(srcdir)/KMultiFormListBox $(all_includes)

noinst_HEADERS = \
  kregexpeditorgui.h editorwindow.h regexpwidget.h drag.h limitedcharlineedit.h regexp.h selectablelineedit.h      \
  dragaccepter.h repeatwidget.h concwidget.h textwidget.h characterswidget.h zerowidgets.h multicontainerwidget.h  \
  scrollededitorwindow.h compoundwidget.h singlecontainerwidget.h widgetfactory.h dcbutton.h charselector.h        \
  lookaheadwidget.h textregexp.h textrangeregexp.h repeatregexp.h lookaheadregexp.h concregexp.h altnregexp.h      \
  positionregexp.h dotregexp.h infopage.h kregexpeditorprivate.h compoundregexp.h pair.h triple.h myfontmetrics.h  \
  regexpbuttons.h userdefinedregexps.h auxbuttons.h verifier.h verifybuttons.h errormap.h syntaxhighlighter.h \
  regexplineedit.h

libkregexpeditorgui_la_SOURCES = \
  kregexpeditorgui.cpp editorwindow.cpp regexpwidget.cpp drag.cpp  concwidget.cpp  zerowidgets.cpp dragaccepter.cpp \
  repeatwidget.cpp characterswidget.cpp altnwidget.cpp limitedcharlineedit.cpp  textregexp.cpp textrangeregexp.cpp  \
  concregexp.cpp altnregexp.cpp positionregexp.cpp  dotregexp.cpp repeatregexp.cpp textwidget.cpp                   \
  selectablelineedit.cpp multicontainerwidget.cpp  scrollededitorwindow.cpp compoundwidget.cpp                      \
  singlecontainerwidget.cpp widgetfactory.cpp  dcbutton.cpp gen_qregexplexer.cpp gen_qregexpparser.cc               \
  charselector.cpp lookaheadregexp.cpp lookaheadwidget.cpp  regexp.cpp infopage.cpp kregexpeditorprivate.cpp        \
  compoundregexp.cpp auxbuttons.cpp  myfontmetrics.cpp regexpbuttons.cpp userdefinedregexps.cpp verifier.cpp        \
  verifybuttons.cpp errormap.cpp syntaxhighlighter.cpp regexplineedit.cpp


libkregexpeditorgui_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module
libkregexpeditorgui_la_LIBADD = KMultiFormListBox/libkmultiformlistbox.la \
	KWidgetStreamer/libkwidgetstreamer.la $(LIB_KIO) 

METASOURCES = AUTO

service_DATA = kregexpeditorgui.desktop
servicedir = $(kde_servicesdir)

#### Binary

bin_PROGRAMS = kregexpeditor
kregexpeditor_SOURCES = main.cpp
kregexpeditor_LDFLAGS = $(all_libraries)
kregexpeditor_LDADD =  libkregexpeditorgui.la $(LIB_KIO) 
kregexpeditor_INCLUDES =  $(all_includes)

apps_DATA = kregexpeditor.desktop
appsdir = $(kde_appsdir)/Utilities


##### Build rules for the parser

.PHONY: parser
parser:
	cd $(srcdir) && flex -Pqregexp -ogen_qregexplexer.cpp qregexpparser.l
	cd $(srcdir) && bison -d -p qregexp -o gen_qregexpparser.cc qregexpparser.y

messages:
	$(srcdir)/extractrc-from-regexp $(srcdir)/predefined/General/*.regexp > predefined-regexps.cpp
	$(XGETTEXT) *.cpp *.h */*.cpp */*.h -o $(podir)/kregexpeditor.pot
