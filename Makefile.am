kde_module_LTLIBRARIES = libkregexpeditorgui.la
AM_CPPFLAGS = -DQT_NO_CAST_ASCII
SUBDIRS = KWidgetStreamer KMultiFormListBox picts
INCLUDES = -I$(srcdir)/KWidgetStreamer -I$(srcdir)/KMultiFormListBox $(all_includes)

noinst_HEADERS = kregexpeditorgui.h editorwindow.h widgetwindow.h regexpwidget.h drag.h\
		             limitedcharlineedit.h regexp.h selectablelineedit.h \
								 dragaccepter.h repeatwidget.h concwidget.h textwidget.h \
								 characterswidget.h zerowidgets.h multicontainerwidget.h
								 scrollededitorwindow.h compoundwidget.h singlecontainerwidget.h \
								 widgetfactory.h  \
								 dcbutton.h charselector.h lookaheadwidget.h \
							   textregexp.h textrangeregexp.h repeatregexp.h lookaheadregexp.h \
                 concregexp.h altnregexp.h positionregexp.h dotregexp.h \
								 infopage.h kregexpeditorprivate.h compoundregexp.h \
								 flow.h compoundinfo.h pair.h triple.h myfontmetrics.h
libkregexpeditorgui_la_SOURCES = kregexpeditorgui.cpp editorwindow.cpp\
													 widgetwindow.cpp regexpwidget.cpp drag.cpp \
													 concwidget.cpp \
													 zerowidgets.cpp dragaccepter.cpp repeatwidget.cpp	\
													 characterswidget.cpp altnwidget.cpp limitedcharlineedit.cpp \
													 textregexp.cpp textrangeregexp.cpp \
													 concregexp.cpp altnregexp.cpp positionregexp.cpp \
													 dotregexp.cpp repeatregexp.cpp textwidget.cpp \
													 selectablelineedit.cpp multicontainerwidget.cpp \
													 scrollededitorwindow.cpp compoundwidget.cpp \
													 singlecontainerwidget.cpp widgetfactory.cpp \
													 dcbutton.cpp gen_qregexplexer.cpp gen_qregexpparser.cc \
													 charselector.cpp lookaheadregexp.cpp lookaheadwidget.cpp \
													 regexp.cpp infopage.cpp kregexpeditorprivate.cpp \
													 compoundregexp.cpp flow.cpp compoundinfo.cpp \
													 myfontmetrics.cpp

libkregexpeditorgui_la_LDFLAGS = $(all_libraries) -module -no-undefined $(KDE_PLUGIN)
libkregexpeditorgui_la_LIBADD = KMultiFormListBox/libkmultiformlistbox.la \
                                KWidgetStreamer/libkwidgetstreamer.la \
                                $(LIB_KFILE) 

METASOURCES = AUTO

service_DATA = kregexpeditorgui.desktop
servicedir = $(kde_servicesdir)


##### Build rules for the parser

.PHONY: parser
parser: gen_qregexplexer.cpp gen_qregexpparser.cc

gen_qregexplexer.cpp: qregexpparser.l
	cd $(srcdir) && flex -Pqregexp -ogen_qregexplexer.cpp qregexpparser.l

# To avoid problems with --enable-final (YYSTYPE defined twice), this file is called .cc
gen_qregexpparser.cc: qregexpparser.y
	cd $(srcdir) && bison -d -p qregexp -o gen_qregexpparser.cc qregexpparser.y
