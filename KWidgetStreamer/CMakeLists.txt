



########### next target ###############


########### install files ###############

install( FILES kwidgetstreamer.h  DESTINATION ${INCLUDE_INSTALL_DIR} )




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
