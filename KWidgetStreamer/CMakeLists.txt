kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############


########### install files ###############

install_files( /include FILES kwidgetstreamer.h )

kde4_footer()



#original Makefile.am contents follow:

#AM_CPPFLAGS = -DQT_NO_CAST_ASCII
#noinst_LTLIBRARIES  = libkwidgetstreamer.la
#
#INCLUDES=  -I$(srcdir)/../KMultiFormListBox $(all_includes)
#
#include_HEADERS = kwidgetstreamer.h
#libkwidgetstreamer_la_SOURCES = kwidgetstreamer.cpp
#
#METASOURCES = AUTO
