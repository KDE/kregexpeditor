kde4_header()




########### next target ###############


########### install files ###############

install_files( /include FILES kmultiformlistboxentry.h kmultiformlistbox.h  	kmultiformlistboxfactory.h  )

kde4_footer()



#original Makefile.am contents follow:

#AM_CPPFLAGS = -DQT_NO_CAST_ASCII
#noinst_LTLIBRARIES  = libkmultiformlistbox.la
#
#INCLUDES= -I$(srcdir)/../KWidgetStreamer $(all_includes)
#
#include_HEADERS = kmultiformlistboxentry.h kmultiformlistbox.h \
#	kmultiformlistboxfactory.h 
#noinst_HEADERS = ccp.h indexWindow.h kmultiformlistbox-multivisible.h \
#	kmultiformlistbox-windowed.h widgetwindow.h windowlistboxitem.h
#libkmultiformlistbox_la_SOURCES = ccp.cpp kmultiformlistboxentry.cpp \
#	kmultiformlistbox.cpp kmultiformlistbox-multivisible.cpp \
#	kmultiformlistboxfactory.cpp indexWindow.cpp \
#	kmultiformlistbox-windowed.cpp widgetwindow.cpp windowlistboxitem.cpp
#
#METASOURCES = AUTO
